#include "propertysettings.h"

#include <QMetaProperty>
#include <QDebug>
#include <QQuickItem>
#include <QJsonArray>
#include <QJSValue>

namespace com { namespace bckmnn { namespace properties {

QMap<QObject*,PropertySettings*> PropertySettings::registry = QMap<QObject*,PropertySettings*>();
PropertySettings* PropertySettings::rootPropertySetting = nullptr;

PropertySettings::PropertySettings(QObject *parent) : QObject(parent)
{

}

PropertySettings::~PropertySettings()
{
    registry.remove(this->parent());
}

void PropertySettings::load()
{
    const QMetaObject *mo = this->metaObject();
    const int offset = mo->propertyOffset();
    const int count = mo->propertyCount();

    for (int i = offset; i < count; ++i) {
        QMetaProperty property = mo->property(i);
        qDebug() << property.name() << property.type() << property.read(this);
    }
}

void PropertySettings::save()
{
    PropertySettings::saveProperties();
}

void PropertySettings::addSettingChild(PropertySettings *child)
{
    if(!m_settingChildren.contains(child)){
        m_settingChildren.append(child);
        child->setSettingParent(this);
    }
}

void PropertySettings::setSettingParent(PropertySettings *parent)
{
    if(m_settingParent == nullptr){
        m_settingParent = parent;
    }else{
        qWarning() << "parent already set";
    }
}

void PropertySettings::organizePropertySettings()
{
    foreach (PropertySettings* setting, registry){
        QObject* p = setting->parent();
        while(p != nullptr){
            QQuickItem* qi = qobject_cast<QQuickItem*>(p);
            if(registry.contains(p) && registry.value(p) != setting){
                registry.value(p)->addSettingChild(setting);
                break;
            }
            if(qi != nullptr){
                p=qi->parentItem();
            }else{
                p = p->parent();
            }
        }
    }
    QList<PropertySettings*> unparentedPropertySettings;
    foreach (PropertySettings* setting, registry){
        if(setting->m_settingParent == nullptr){
            unparentedPropertySettings.append(setting);
        }
    }
    rootPropertySetting = unparentedPropertySettings.first();
    if(unparentedPropertySettings.length() > 1){
        qWarning() << "more than 1 unparented property settings";
    }
}

void PropertySettings::saveProperties()
{
    organizePropertySettings();
    QJsonObject j = rootPropertySetting->serialize();
    QFile jsonFile("out.json");
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write( QJsonDocument(j).toJson(QJsonDocument::JsonFormat::Indented));
}

QJsonArray PropertySettings::qvlistToJsonArray(const QVariantList l)
{
    QJsonArray a;
    foreach(QVariant v, l){
        QJsonValue jv;
        if(v.canConvert<QString>()){
            jv = qvToJsonValue(v);
            a.append(jv);
        }
    }
    return a;
}

QJsonValue PropertySettings::qvToJsonValue(const QVariant v)
{
    QJsonValue jv(v.toString());
    if(jv.toString() == ""){
        qWarning() << " empty string fron QVariant " << v;
    }
    return jv;
}

QJsonObject PropertySettings::qvmapToJsonObject(const QMap<QString, QVariant> m)
{
    QJsonObject o;
    QMapIterator<QString, QVariant> i(m);
    while (i.hasNext()) {
        i.next();
        o.insert(i.key(),qvToJsonValue(i.value()));
    }
    return o;
}

QJsonObject PropertySettings::serialize()
{
    QJsonObject json;

    const QMetaObject *mo = this->metaObject();
    const int offset = mo->propertyOffset();
    const int count = mo->propertyCount();

    for (int i = offset; i < count; ++i) {
        QMetaProperty property = mo->property(i);
        qDebug() << property.name() << property.type() << property.read(this);
        QVariant value = property.read(this);
        if(value.canConvert<QString>()){
            QJsonValue jv = qvToJsonValue(value);
            json.insert(property.name(),jv);
        }else{
            qDebug()<< " --->" << property.name() << property.type()  << property.userType() << property.read(this);
            QVariant v = value.value<QVariant>();
            if(v.type() == QVariant::Type::UserType){
                if(QString(v.typeName())=="QJSValue"){
                   QJSValue jsv = v.value<QJSValue>();
                   v = jsv.toVariant();
                   if(v.canConvert<QVariantList>()){
                       QJsonArray a = qvlistToJsonArray(v.value<QVariantList>());
                       json.insert(property.name(),a);
                   }else if(v.canConvert<QVariantMap>()){
                       QJsonObject o = qvmapToJsonObject(v.value<QVariantMap>());
                       json.insert(property.name(),o);
                   }else{
                       qDebug() << "   ->" << jsv.toVariant() << jsv.isArray() << jsv.isObject();
                   }
                }
            }
        }
    }

    if(this->m_settingChildren.size() > 0){
        QJsonArray children;
        foreach (PropertySettings* c, this->m_settingChildren){
            children.append(c->serialize());
        }
        json.insert("|children",children);
    }

    return json;
}


void PropertySettings::classBegin()
{
}

void PropertySettings::componentComplete()
{
    if(this->parent() != nullptr){
        if(registry.contains(this->parent())){
            qWarning() << "registry already contains property setting for this parent";
        }else{
            registry.insert(this->parent(),this);
            qDebug() << this->parent() << this;
        }
    }else{
        qWarning() << "settings has no parent";
    }
}

void PropertySettings::timerEvent(QTimerEvent *event)
{
}

}}}
