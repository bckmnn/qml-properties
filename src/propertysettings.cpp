#include "propertysettings.h"

#include <QMetaProperty>
#include <QMetaType>
#include <QDebug>
#include <QQuickItem>
#include <QJsonArray>
#include <QJSValue>
#include <QtMath>

namespace com { namespace bckmnn { namespace properties {

QMap<QObject*,PropertySettings*> PropertySettings::registry = QMap<QObject*,PropertySettings*>();
PropertySettings* PropertySettings::rootPropertySetting = nullptr;

PropertySettings::PropertySettings(QObject *parent) : QObject(parent)
{
    qDebug() << "called creator";
}

PropertySettings::~PropertySettings()
{
    registry.remove(this->parent());
    if(m_settingParent){
        m_settingParent->removeSettingChild(this);
    }
    qDebug() << "called destructor";
}

void PropertySettings::load()
{
    PropertySettings::loadProperties();
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

void PropertySettings::removeSettingChild(PropertySettings *child)
{
    m_settingChildren.removeAll(child);
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

void PropertySettings::loadProperties()
{
    organizePropertySettings();
    QFile jsonFile("out.json");
    jsonFile.open(QFile::ReadOnly);
    QJsonDocument j = QJsonDocument::fromJson(jsonFile.readAll());
    rootPropertySetting->deserialize(j.object());
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

    QMap<QString, QMetaProperty>propMap = _mapProperties();
    QMapIterator<QString, QMetaProperty> i(propMap);
    while (i.hasNext()) {
        i.next();
        QMetaProperty property = i.value();
        if(property.isWritable()){
            QVariant value = property.read(this);
            if(value.type() == QVariant::Type::Int ){
                QJsonValue jv(value.toInt());
                json.insert(property.name(),jv);
            }else if(value.type() == QVariant::Type::Double ){
                QJsonValue jv(value.toDouble());
                json.insert(property.name(),jv);
            }else if(value.canConvert<QUrl>()){
                //TODO: make paths relative to sg path
                //qDebug() << ":: QUrl needs to be made relative to sg path";
                QJsonValue jv = qvToJsonValue(value);
                json.insert(property.name(),jv);
            }else if(value.canConvert<QString>()){
                QJsonValue jv = qvToJsonValue(value);
                json.insert(property.name(),jv);
            }else if(value.canConvert<QPointF>()){
                QPointF p = value.value<QPointF>();
                QJsonObject jo;
                jo.insert("@type", value.typeName());
                jo.insert("x", QJsonValue(p.x()));
                jo.insert("y", QJsonValue(p.y()));
                json.insert(property.name(),jo);
            }else{
                QVariant v = value.value<QVariant>();
                if(v.type() == QVariant::Type::UserType){
                    QString typeName = QString(v.typeName());
                    if(typeName == "QJSValue"){
                       QJSValue jsv = v.value<QJSValue>();
                       v = jsv.toVariant();
                       if(v.canConvert<QVariantList>()){
                           QJsonArray a = qvlistToJsonArray(v.value<QVariantList>());
                           json.insert(property.name(),a);
                       }else if(v.canConvert<QVariantMap>()){
                           QJsonObject o = qvmapToJsonObject(v.value<QVariantMap>());
                           json.insert(property.name(),o);
                       }else{
                           qWarning() << ":: property" << property.name() << "QJSValue of type" << v.typeName() << "was not serialized";
                       }
                    }else{
                        qWarning() << ":: property" << property.name() << "of user type" << v.typeName() << "was not serialized";
                    }
                }else{
                    qWarning() << ":: property" << property.name() << "of regular type" << v.typeName() << "was not serialized";
                }
            }

        }
    }
    if(this->m_settingChildren.size() > 0){
        QJsonArray children;
        foreach (PropertySettings* c, this->m_settingChildren){
            children.append(c->serialize());
        }
        json.insert("»children",children);
    }

    return json;
}

void PropertySettings::deserialize(QJsonObject json)
{
    QMap<QString, QMetaProperty>propMap = _mapProperties();

    QJsonArray children;
    foreach(QString key, json.keys()){
        if(key == "»children"){
            children = json.take(key).toArray();
        }else{
            if(propMap.contains(key) && propMap.value(key).isWritable()){
                QVariant currentValue = propMap.value(key).read(this);
                if(currentValue.canConvert<QString>()){
                    propMap.value(key).write(this, json.value(key).toVariant());
                }else if(currentValue.canConvert<QPointF>()){
                    QPointF p;
                    QJsonObject o = json.value(key).toObject();
                    if(o.value("@type").toString("") == "QPointF"){
                        p.setX(o.value("x").toDouble());
                        p.setY(o.value("y").toDouble());
                        propMap.value(key).write(this, QVariant(p));
                    }else{
                        qWarning() << ":: property" << propMap.value(key).name() << "not restored";
                    }
                }else{
                    QVariant v = currentValue.value<QVariant>();
                    if(v.type() == QVariant::Type::UserType){
                        QString typeName = QString(v.typeName());
                        if(typeName == "QJSValue"){
                           QJSValue jsv = v.value<QJSValue>();
                           v = jsv.toVariant();
                           if(v.canConvert<QVariantList>()){
                                propMap.value(key).write(this, json.value(key).toVariant());
                           }else if(v.canConvert<QVariantMap>()){
                                propMap.value(key).write(this, json.value(key).toVariant());
                           }else{
                               qWarning() << ":: property" << propMap.value(key).name() << "QJSValue of type" << v.typeName() << "was not restored";
                           }
                        }
                    }else{
                        qWarning() << ":: property" << propMap.value(key).name() << "of type" << currentValue.typeName() << "was not restored";
                    }
                }
            }
        }
    }
    if(this->m_settingChildren.size() > 0){
        for (int i = 0; i< qMin(children.size(), this->m_settingChildren.size()); i++){
            this->m_settingChildren.at(i)->deserialize(children.at(i).toObject());
        }
    }
}

QMap<QString, QMetaProperty> PropertySettings::_mapProperties()
{
    QMap<QString, QMetaProperty> propMap;
    const QMetaObject *mo = this->metaObject();
    const int offset = mo->propertyOffset();
    const int count = mo->propertyCount();
    for (int i = offset; i < count; ++i) {
        QMetaProperty property = mo->property(i);
        propMap.insert(property.name(), property);
    }
    return propMap;
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
