#ifndef PROPERTIESATTACHEDTYPE_H
#define PROPERTIESATTACHEDTYPE_H

#include <QObject>
#include <QtQml>
#include <QtMath>

namespace com { namespace bckmnn { namespace properties {

class BaseProperty : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString label MEMBER m_label NOTIFY labelChanged)
    Q_PROPERTY(QString delegate MEMBER m_delegate_qml NOTIFY delegateChanged)

public:
    explicit BaseProperty(QObject *parent = nullptr) : QObject(parent) {

    }

signals:
    void labelChanged();
    void delegateChanged();

protected:
    QString m_label;
    QString m_delegate_qml;
    QVariant m_value;
};

class IntProperty : public BaseProperty
{
    Q_OBJECT
    Q_PROPERTY(int value READ intValue WRITE setIntValue NOTIFY valueChanged)

public:
    explicit IntProperty(QObject *parent = nullptr) :
        BaseProperty(parent)
    {
        m_delegate_qml = "IntPropertyEditor.qml";
    }

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

class RangeProperty : public BaseProperty
{
    Q_OBJECT
    Q_PROPERTY(int value READ intValue WRITE setIntValue NOTIFY valueChanged)
    Q_PROPERTY(double stepSize MEMBER m_stepSize NOTIFY stepSizeChanged)
    Q_PROPERTY(int from MEMBER m_from NOTIFY fromChanged)
    Q_PROPERTY(int to MEMBER m_to NOTIFY toChanged)
public:
    explicit RangeProperty(QObject *parent = nullptr) :
        BaseProperty(parent),
        m_from(0),
        m_to(100),
        m_stepSize(0)
    {
        m_delegate_qml = "RangePropertyEditor.qml";
    }

signals:
    void valueChanged();
    void fromChanged();
    void toChanged();
    void stepSizeChanged();
private:
    int intValue(){
        return m_value.toInt();
    }
    void setIntValue(int value){
        if(value < m_from){
            value = m_from;
        }
        if(value > m_to){
            value = m_to;
        }
        m_value = QVariant::fromValue(value);
        emit valueChanged();
    }
    int m_from;
    int m_to;
    double m_stepSize;
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
