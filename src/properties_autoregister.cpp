#include <QtCore/QCoreApplication>
#include <QtQml>

#include "propertymanager.h"


namespace com { namespace bckmnn { namespace properties {

static const char* PROPERTIES_NS = "com.bckmnn.properties";

static void registerTypes()
{
    qmlRegisterType<PropertiesAttachedType>();
    qmlRegisterType<BaseProperty>(PROPERTIES_NS, 1, 0, "BaseProperty");
    qmlRegisterType<IntProperty>(PROPERTIES_NS, 1, 0, "IntProperty");
    qmlRegisterType<RangeProperty>(PROPERTIES_NS, 1, 0, "RangeProperty");
    qmlRegisterUncreatableType<Properties>(PROPERTIES_NS, 1, 0, "Properties", "attached type");
    qmlRegisterSingletonType<PropertyManager>(PROPERTIES_NS, 1, 0, "PropertyManager", PropertyManager::PropertyManager_Provider);
}

Q_COREAPP_STARTUP_FUNCTION(registerTypes)

} } }

