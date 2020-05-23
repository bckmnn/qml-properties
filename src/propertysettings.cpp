#include "propertysettings.h"

#include <QDebug>
#include <QJSValue>
#include <QJsonArray>
#include <QMetaProperty>
#include <QMetaType>
#include <QQuickItem>
#include <QtMath>
#include <QtQml>
#include <QtQuick>

namespace com {
namespace bckmnn {
namespace properties {

QMap<QObject *, PropertySettings *> PropertySettings::registry =
        QMap<QObject *, PropertySettings *>();
PropertySettings *PropertySettings::rootPropertySetting = nullptr;

PropertySettings::PropertySettings(QObject *parent) : QObject(parent) {}

PropertySettings::~PropertySettings() {
    registry.remove(this->parent());
    foreach (PropertySettings *p, m_settingChildren) {
        p->m_settingParent = nullptr;
    }
    if (m_settingParent) {
        m_settingParent->removeSettingChild(this);
    }
}

void PropertySettings::load() { PropertySettings::loadProperties(); }

void PropertySettings::save() { PropertySettings::saveProperties(); }

void PropertySettings::addSettingChild(PropertySettings *child) {
    if (!m_settingChildren.contains(child)) {
        m_settingChildren.append(child);
        child->setSettingParent(this);
    }
}

void PropertySettings::removeSettingChild(PropertySettings *child) {
    if (this->m_settingChildren.size() > 0) {
        this->m_settingChildren.removeAll(child);
    }
}

void PropertySettings::setSettingParent(PropertySettings *parent) {
    if (m_settingParent == nullptr) {
        m_settingParent = parent;
    } else {
        qWarning() << "parent already set";
    }
}

void PropertySettings::organizePropertySettings() {
    foreach (PropertySettings *setting, registry) {
        QObject *p = setting->parent();
        QString path;
        while (p != nullptr) {
            QQuickItem *qi = qobject_cast<QQuickItem *>(p);
            if (registry.contains(p) && registry.value(p) != setting) {
                registry.value(p)->addSettingChild(setting);
                setting->setProperty("domPath", QVariant(path.remove(0,3)));
                break;
            }
            if (qi != nullptr) {
                p = qi->parentItem();
            } else {
                p = p->parent();
            }
            if (p) {
                QString id = _getQuickItemId(qobject_cast<QQuickItem *>(p));
                if(id.size()>0){
                    id.prepend("(");
                    id.append(")");
                }
                path.prepend(id).prepend(p->metaObject()->className()).prepend(" » ");
            }
        }
    }
    QList<PropertySettings *> unparentedPropertySettings;
    foreach (PropertySettings *setting, registry) {
        if (setting->m_settingParent == nullptr) {
            unparentedPropertySettings.append(setting);
        }
    }
    rootPropertySetting = unparentedPropertySettings.first();
    if (unparentedPropertySettings.length() > 1) {
        qWarning() << "more than 1 unparented property settings";
    }
}

void PropertySettings::saveProperties() {
    organizePropertySettings();
    QJsonObject j = rootPropertySetting->serialize();
    QFile jsonFile("out.json");
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(QJsonDocument(j).toJson(QJsonDocument::JsonFormat::Indented));
}

void PropertySettings::loadProperties() {
    organizePropertySettings();
    QFile jsonFile("out.json");
    jsonFile.open(QFile::ReadOnly);
    QJsonDocument j = QJsonDocument::fromJson(jsonFile.readAll());
    rootPropertySetting->deserialize(j.object());
}

QJsonArray PropertySettings::variantlistToJsonArray(const QVariantList l) {
    QJsonArray a;
    foreach (QVariant v, l) {
        QJsonValue jv = variantToJsonValue(v);
        a.append(jv);
    }
    return a;
}

QJsonValue PropertySettings::variantToJsonValue(const QVariant v) {
    if(!v.isValid()){
        return QJsonValue();
    }
    if (v.type() == QVariant::Type::Bool) {
        return QJsonValue(v.toBool());
    } else if (v.type() == QVariant::Type::Int) {
        return QJsonValue(v.toInt());
    } else if (v.type() == QVariant::Type::Double) {
        return QJsonValue(v.toDouble());
    } else if (v.type() == QVariant::Type::String) {
        return QJsonValue(v.toString());
    } else if (v.type() == QVariant::Type::Url) {
        // TODO: make paths relative to sg path
        qDebug() << ":: QUrl needs to be made relative to sg path";
        return QJsonValue(v.toString());
    } else if (v.type() == QVariant::Type::Color) {
        return QJsonValue(v.toString());
    } else if (v.type() == QVariant::Type::PointF) {
        QJsonObject point;
        QPointF p = v.value<QPointF>();
        point.insert("@type", v.typeName());
        point.insert("x", QJsonValue(p.x()));
        point.insert("y", QJsonValue(p.y()));
        return QJsonValue(point);
    } else if (v.type() == QVariant::Type::RectF) {
        QJsonObject rect;
        QRectF r = v.value<QRectF>();
        rect.insert("@type", v.typeName());
        rect.insert("x", QJsonValue(r.x()));
        rect.insert("y", QJsonValue(r.y()));
        rect.insert("w", QJsonValue(r.width()));
        rect.insert("h", QJsonValue(r.height()));
        return QJsonValue(rect);
    } else if (v.type() == QVariant::Type::List) {
        return QJsonValue(variantlistToJsonArray(v.value<QVariantList>()));
    } else if (v.type() == QVariant::Type::Map) {
        return QJsonValue(variantmapToJsonObject(v.value<QVariantMap>()));
    } else if (v.canConvert<QAbstractListModel *>()) {
        QAbstractListModel *m = v.value<QAbstractListModel *>();
        QJsonObject listmodel;
        listmodel.insert("@type", m->metaObject()->className());
        QJsonArray entries;
        auto roles = m->roleNames();
        for (int idx = 0; idx < m->rowCount(); idx++) {
            QJsonObject entry;
            for (auto it = roles.constBegin(); it != roles.constEnd(); ++it) {
                QVariant data = m->data(m->index(idx), it.key());
                QJsonValue v = variantToJsonValue(data);
                entry.insert(it.value(), v);
            }
            entries.append(entry);
        }
        listmodel.insert("»entries", entries);
        return QJsonValue(listmodel);
    } else if (v.type() == QVariant::Type::UserType) {
        QString typeName = QString(v.typeName());
        QJsonObject userType;
        userType.insert("@type", typeName);
        if (typeName == "QJSValue") {
            QJSValue jsvalue = v.value<QJSValue>();
            QVariant jsv = jsvalue.toVariant();
            userType.insert("value", variantToJsonValue(jsv));
            return QJsonValue(userType);
        }else{
            qDebug() << "skipped serialization of" << typeName << v;
        }
    } else {
        qDebug() << "skipped serialization of" << v.type() << v;
    }
    return QJsonValue();
}

QJsonObject PropertySettings::variantmapToJsonObject(
        const QMap<QString, QVariant> m) {
    QJsonObject o;
    QMapIterator<QString, QVariant> i(m);
    while (i.hasNext()) {
        i.next();
        o.insert(i.key(), variantToJsonValue(i.value()));
    }
    return o;
}

QVariant PropertySettings::jsonValueToVariant(const QJsonValue v, QVariant currentValue)
{
    QVariant::Type targetType = currentValue.type();
    if (targetType == QVariant::Type::Bool) {
        return v.toVariant();
    } else if (targetType == QVariant::Type::Int) {
        return v.toVariant();
    } else if (targetType == QVariant::Type::Double) {
        return v.toVariant();
    } else if (targetType == QVariant::Type::String) {
        return v.toVariant();
    } else if (targetType == QVariant::Type::Url) {
        // TODO: make paths relative to sg path
        qDebug() << ":: QUrl needs to be made relative to sg path";
        return v.toVariant();
    } else if (targetType == QVariant::Type::Color) {
        return v.toVariant();
    } else if (targetType == QVariant::Type::PointF) {
        QPointF p;
        QJsonObject o = v.toObject();
        if (o.value("@type").toString("") == "QPointF") {
            p.setX(o.value("x").toDouble());
            p.setY(o.value("y").toDouble());
            return QVariant(p);
        }
    } else if (targetType == QVariant::Type::RectF) {
        QRectF r;
        QJsonObject o = v.toObject();
        if (o.value("@type").toString("") == "QRectF") {
            r.setX(o.value("x").toDouble());
            r.setY(o.value("y").toDouble());
            r.setWidth(o.value("w").toDouble());
            r.setHeight(o.value("h").toDouble());
            return QVariant(r);
        }
    } else if (int(targetType) == QMetaType::Type::QObjectStar){
        if(currentValue.canConvert<QAbstractListModel *>()){
            //TODO: better would be returning a QVariantMap and doing the list update elsewhere?
            QAbstractListModel *m = currentValue.value<QAbstractListModel *>();
            auto roles = m->roleNames();
            QJsonObject o = v.toObject();
            QJsonArray a = o.value("»entries").toArray();
            int pos = 0;
            currentValue.clear();
            for (int idx = 0; idx < a.count(); idx++){
                QJsonValue jv = a.at(idx);
                QJsonObject jo = jv.toObject();
                if(pos <= m->rowCount()){
                    m->insertRow(m->rowCount());
                }
                for (auto it = roles.constBegin(); it != roles.constEnd(); ++it) {
                    if(jo.contains(it.value())){
                        m->setData(m->index(pos), jo.value(it.value()).toVariant(), it.key());
                    }
                }
                pos = pos + 1;
            }
            if(m->rowCount() > pos){
                qDebug() << m->rowCount() << pos;
                qDebug() << "removing stuff";
                for(int i = pos; i < m->rowCount();i++ ){
                    bool success = m->removeRow(pos);
                    qDebug() << success;
                }
            }
            m->layoutChanged();
        }
    } else if (targetType == QVariant::Type::UserType){

            QJsonObject o = v.toObject();
            if (o.value("@type").toString("") == "QJSValue") {
                QVariant jsvalue = o.value("value").toVariant();
                return jsvalue;
            }

    }
    return QVariant();
}

QVariantList PropertySettings::jsonArrayToVariantList(const QJsonArray a)
{
    QVariantList l;
    qDebug() << a;
    foreach(QJsonValue v, a){
        qDebug() << v;
        QVariant qv = v.toVariant();
        l << qv;
    }
    qDebug() << l;
    return l;
}

QJsonObject PropertySettings::serialize() {
    QJsonObject json;

    QMap<QString, QMetaProperty> propMap = _mapProperties();
    QMapIterator<QString, QMetaProperty> i(propMap);
    while (i.hasNext()) {
        i.next();
        QMetaProperty property = i.value();
        if (property.isWritable()) {
            QVariant value = property.read(this);
            json.insert(property.name(), variantToJsonValue(value));
        }
    }
    QString pid =
            this->_getQuickItemId(qobject_cast<QQuickItem *>(this->parent()));
    if (pid.length() > 0) {
        json.insert("»pid", pid);
    }
    json.insert("»domPath", m_domPath);
    if (m_list != nullptr) {
        QString lid = this->_getQuickItemId(this->m_list);
        json.insert("»lid", lid);
    }
    if (m_listIndex > -1) {
        json.insert("»idx", this->m_listIndex);
    }
    if (this->m_settingChildren.size() > 0) {
        QJsonArray children;
        foreach (PropertySettings *c, this->m_settingChildren) {
            children.append(c->serialize());
        }
        json.insert("»children", children);
    }

    return json;
}

void PropertySettings::deserialize(QJsonObject json) {
    QMap<QString, QMetaProperty> propMap = _mapProperties();

    QJsonArray children;
    foreach (QString key, json.keys()) {
        if (key == "»children") {
            children = json.take(key).toArray();
        } else {
            if (propMap.contains(key) && propMap.value(key).isWritable()) {
                QVariant currentValue = propMap.value(key).read(this);
                QJsonValue jv = json.value(key);
                QVariant restoredVariant = jsonValueToVariant(jv, currentValue);
                if(restoredVariant.isValid()){
                    propMap.value(key).write(this,restoredVariant);
                }else{
                    qWarning() << ":: property" << propMap.value(key).name()
                               << "not restored.";
                }
            }
        }
    }
    if (this->m_settingChildren.size() > 0) {
        for (int i = 0; i < qMin(children.size(), this->m_settingChildren.size());
             i++) {
            this->m_settingChildren.at(i)->deserialize(children.at(i).toObject());
        }
    }
}

QMap<QString, QMetaProperty> PropertySettings::_mapProperties() {
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

int PropertySettings::_getParentIndex() {
    QQuickItem *p = qobject_cast<QQuickItem *>(this->parent());
    if (p) {
        QVariant index = p->property("data");

        qDebug() << p->metaObject()->propertyCount();
        if (index.isValid()) {
            qDebug() << "index is" << index;
            QQmlListProperty<QObject> lp = index.value<QQmlListProperty<QObject>>();
            qDebug() << lp.count(&lp);
            if (lp.count(&lp) > 1) qDebug() << " << " << lp.at(&lp, 1);
            return index.toInt();
        }
    }
    return -1;
}

QString PropertySettings::_getQuickItemId(QQuickItem *item) {
    if (item) {
        QQmlContext *ctx = qmlContext(item);
        QString id(ctx ? ctx->nameForObject(item) : "");
        return id;
    }
    qWarning() << "is not a QQuickItem" << item;
    return QString();
}

void PropertySettings::classBegin() {}

void PropertySettings::componentComplete() {
    if (this->parent() != nullptr) {
        if (registry.contains(this->parent())) {
            qWarning()
                    << "registry already contains property setting for this parent";
        } else {
            registry.insert(this->parent(), this);
            qDebug() << this->parent() << this;
        }
    } else {
        qWarning() << "settings has no parent";
    }
}

void PropertySettings::timerEvent(QTimerEvent *event) {}

}  // namespace properties
}  // namespace bckmnn
}  // namespace com
