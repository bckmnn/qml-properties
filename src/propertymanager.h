#ifndef PROPERTYMANAGER_H
#define PROPERTYMANAGER_H

#include <QObject>
#include <QQuickItem>
#include <QtQml>
#include "propertiesattachedtype.h"

namespace com { namespace bckmnn { namespace properties {


class Properties : public QObject
{
    Q_OBJECT
public:
    Properties(QObject *parent);
    static PropertiesAttachedType *qmlAttachedProperties(QObject *object){
        return new PropertiesAttachedType(object);
    }

signals:

public slots:

private:


};

class PropertyManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<com::bckmnn::properties::BaseProperty> props READ props NOTIFY propsChanged)


public:
    explicit PropertyManager(QObject *parent = nullptr);
    ~PropertyManager() override;

    static QObject *PropertyManager_Provider(QQmlEngine *engine, QJSEngine *scriptEngine)
    {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        PropertyManager *instance = new PropertyManager();
        return instance;
    }

    Q_INVOKABLE void edit(QQuickItem * editable);

    QQmlListProperty<com::bckmnn::properties::BaseProperty> props();

    static PropertiesAttachedType *getAttachedType(QQuickItem * item);

signals:
    void propsChanged();

public slots:

private:
      QQuickItem * m_editable;

      QQmlListProperty<com::bckmnn::properties::BaseProperty> m_emptyList;
};

}}}


QML_DECLARE_TYPEINFO(com::bckmnn::properties::Properties, QML_HAS_ATTACHED_PROPERTIES)
QML_DECLARE_TYPE(com::bckmnn::properties::PropertyManager)


#endif // PROPERTYMANAGER_H
