#include "propertiesattachedtype.h"
namespace com { namespace bckmnn { namespace properties {
PropertiesAttachedType::PropertiesAttachedType(QObject *parent) : QObject(parent)
{

}

QQmlListProperty<BaseProperty> PropertiesAttachedType::props()
{
    return QQmlListProperty<BaseProperty>(this, this,
                                          &PropertiesAttachedType::appendProp,
                                          &PropertiesAttachedType::propCount,
                                          &PropertiesAttachedType::prop,
                                          &PropertiesAttachedType::clearProps
                                          );
}

void PropertiesAttachedType::appendProp(BaseProperty * prop)
{
    m_props.append(prop);
}

int PropertiesAttachedType::propCount() const
{
    return m_props.size();
}

BaseProperty *PropertiesAttachedType::prop(int index) const
{
    return m_props.at(index);
}

void PropertiesAttachedType::clearProps()
{
    m_props.clear();
}

void PropertiesAttachedType::appendProp(QQmlListProperty<BaseProperty> * list, BaseProperty * prop)
{
    reinterpret_cast< PropertiesAttachedType* >(list->data)->appendProp(prop);
}

int PropertiesAttachedType::propCount(QQmlListProperty<BaseProperty> * list)
{
    return reinterpret_cast<PropertiesAttachedType *>(list->data)->propCount();
}

BaseProperty *PropertiesAttachedType::prop(QQmlListProperty<BaseProperty> * list, int index)
{
    return reinterpret_cast<PropertiesAttachedType *>(list->data)->prop(index);
}

void PropertiesAttachedType::clearProps(QQmlListProperty<BaseProperty> * list)
{
     reinterpret_cast<PropertiesAttachedType *>(list->data)->clearProps();
}
}}}
