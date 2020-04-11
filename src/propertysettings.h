#ifndef PROPERTYSETTINGS_H
#define PROPERTYSETTINGS_H

#include <QObject>
#include <QQmlParserStatus>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMap>

namespace com { namespace bckmnn { namespace properties {

class PropertySettings : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
public:
    explicit PropertySettings(QObject *parent = nullptr);
    ~PropertySettings() override;

    Q_INVOKABLE void load();
    Q_INVOKABLE void save();

    void addSettingChild(PropertySettings* child);
    void removeSettingChild(PropertySettings* child);
    void setSettingParent(PropertySettings* parent);

signals:

public slots:

protected:
        static QMap<QObject*,PropertySettings*> registry;
        static PropertySettings* rootPropertySetting;
        static void saveProperties();
        static void loadProperties();

        QJsonObject serialize();
        void deserialize(QJsonObject json);

private:
        PropertySettings* m_settingParent = nullptr;
        QList<PropertySettings*> m_settingChildren = QList<PropertySettings*>();

        QMap<QString, QMetaProperty> _mapProperties();

        static void organizePropertySettings();

        static QJsonArray qvlistToJsonArray(const QVariantList l);
        static QJsonValue qvToJsonValue(const QVariant v);
        static QJsonObject qvmapToJsonObject(const QMap<QString, QVariant> m);

// --- interfaces ---
    // QQmlParserStatus interface

public:
    void classBegin() override;
    void componentComplete() override;

    // QObject interface
protected:
    void timerEvent(QTimerEvent *event) override;





};

}}}

#endif // PROPERTYSETTINGS_H
