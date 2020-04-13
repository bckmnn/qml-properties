#ifndef PROPERTYSETTINGS_H
#define PROPERTYSETTINGS_H

#include <QObject>
#include <QQmlParserStatus>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMap>
#include <QQuickItem>

namespace com { namespace bckmnn { namespace properties {

class PropertySettings : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(int listIndex MEMBER m_listIndex NOTIFY listIndexChanged)
    Q_PROPERTY(QQuickItem* list MEMBER m_list NOTIFY listChanged)
    Q_PROPERTY(QString pid MEMBER m_parentId NOTIFY pidChanged)
    Q_PROPERTY(QString domPath MEMBER m_domPath NOTIFY domPathChanged)
public:
    explicit PropertySettings(QObject *parent = nullptr);
    ~PropertySettings() override;

    Q_INVOKABLE void load();
    Q_INVOKABLE void save();

    void addSettingChild(PropertySettings* child);
    void removeSettingChild(PropertySettings* child);
    void setSettingParent(PropertySettings* parent);

signals:
    void pidChanged();
    void domPathChanged();
    void listChanged();
    void listIndexChanged();

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
        int m_listIndex = -1;
        QQuickItem* m_list = nullptr;
        QString m_parentId = "";
        QString m_domPath = "";

        QMap<QString, QMetaProperty> _mapProperties();
        int _getParentIndex();
        static QString _getQuickItemId(QQuickItem* item);

        static void organizePropertySettings();

        static QJsonArray variantlistToJsonArray(const QVariantList l);
        static QJsonValue variantToJsonValue(const QVariant v);
        static QJsonObject variantmapToJsonObject(const QMap<QString, QVariant> m);

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
