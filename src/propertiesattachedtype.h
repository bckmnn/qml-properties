#ifndef PROPERTIESATTACHEDTYPE_H
#define PROPERTIESATTACHEDTYPE_H

#include <QObject>
#include <QtQml>

namespace com { namespace bckmnn { namespace properties {

class BaseProperty : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString label MEMBER m_label)

protected:
    QString m_label;
    QVariant m_value;
};

class IntProperty : public BaseProperty
{
    Q_OBJECT
    Q_PROPERTY(int value READ intValue WRITE setIntValue NOTIFY valueChanged)
signals:
    void valueChanged();
private:
    int intValue(){
        return m_value.toInt();
    }
    void setIntValue(int value){
        m_value = QVariant::fromValue(value);
        emit valueChanged();
    }
};

class PropertiesAttachedType : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<com::bckmnn::properties::BaseProperty> list READ props)
public:
    explicit PropertiesAttachedType(QObject *parent = nullptr);

    QQmlListProperty<BaseProperty> props();

    void appendProp(BaseProperty* prop);
    int propCount() const;
    BaseProperty *prop(int index) const;
    void clearProps();

signals:

public slots:

private:
    static void appendProp(QQmlListProperty<BaseProperty>*, BaseProperty*);
    static int propCount(QQmlListProperty<BaseProperty>*);
    static BaseProperty* prop(QQmlListProperty<BaseProperty>*, int);
    static void clearProps(QQmlListProperty<BaseProperty>*list);

    QList<BaseProperty *> m_props;
};

}}}

QML_DECLARE_TYPE(com::bckmnn::properties::PropertiesAttachedType)
QML_DECLARE_TYPE(com::bckmnn::properties::BaseProperty)

#endif // PROPERTIESATTACHEDTYPE_H
