#include "propertymanager.h"

namespace com { namespace bckmnn { namespace properties {

PropertyManager::PropertyManager(QObject *parent) :
    QObject(parent),
    m_editable(nullptr),
    m_emptyList(QQmlListProperty<BaseProperty>())
{

}

PropertyManager::~PropertyManager()
{

}

void PropertyManager::edit(QQuickItem *editable)
{
    m_editable = editable;
    PropertiesAttachedType * p = PropertyManager::getAttachedType(m_editable);
    if(p != nullptr){
        qDebug() << p->propCount();
        emit propsChanged();
    }
}

QQmlListProperty<BaseProperty> PropertyManager::props()
{
    if(m_editable != nullptr){
        PropertiesAttachedType * p = PropertyManager::getAttachedType(m_editable);
        if(p != nullptr){
            return p->props();
        }
    }
    return m_emptyList;
}

PropertiesAttachedType *PropertyManager::getAttachedType(QQuickItem *item)
{
    return qobject_cast<PropertiesAttachedType *> (qmlAttachedPropertiesObject<Properties> (item, false));
}

}}}
