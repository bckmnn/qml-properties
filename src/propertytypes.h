#ifndef PROPERTYTYPES_H
#define PROPERTYTYPES_H

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
    void valueChanged();

protected:
    QString m_label;
    QString m_delegate_qml;
    QVariant m_value;
};

class BoolProperty : public BaseProperty
{
    Q_OBJECT
    Q_PROPERTY(bool value READ boolValue WRITE setBoolValue NOTIFY valueChanged)

public:
    explicit BoolProperty(QObject *parent = nullptr) :
        BaseProperty(parent)
    {
        m_delegate_qml = "BoolPropertyEditor.qml";
    }

signals:

private:
    bool boolValue(){
        return m_value.toBool();
    }
    void setBoolValue(bool value){
        m_value = QVariant::fromValue(value);
        emit valueChanged();
    }
};

class StringProperty : public BaseProperty
{
    Q_OBJECT
    Q_PROPERTY(QString value READ stringValue WRITE setStringValue NOTIFY valueChanged)
    Q_PROPERTY(bool multiline MEMBER m_multiline NOTIFY multilineChanged)

public:
    explicit StringProperty(QObject *parent = nullptr) :
        BaseProperty(parent)
    {
        m_delegate_qml = "StringPropertyEditor.qml";
    }

signals:
    void multilineChanged();
private:
    QString stringValue(){
        return m_value.toString();
    }
    void setStringValue(QString value){
        m_value = QVariant::fromValue(value);
        emit valueChanged();
    }
    bool m_multiline = false;
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

}}}

#endif // PROPERTYTYPES_H
