#pragma once

#include "TakionVersionBase.h"

#define EXTENSIONCOLUMNS_MAJORNUMBER          1
#define EXTENSIONCOLUMNS_MINORNUMBER          0
#define EXTENSIONCOLUMNS_BUILDNUMBER          5
#define EXTENSIONCOLUMNS_MODIFICATIONNUMBER   199

//#define TU_STR(value) #value
//#define TU_STRINGIZE(value) TU_STR(value)

#define ExtensionColumnsVersion EXTENSIONCOLUMNS_MAJORNUMBER,EXTENSIONCOLUMNS_MINORNUMBER,EXTENSIONCOLUMNS_BUILDNUMBER,EXTENSIONCOLUMNS_MODIFICATIONNUMBER
#define ExtensionColumnsVersionDot EXTENSIONCOLUMNS_MAJORNUMBER.EXTENSIONCOLUMNS_MINORNUMBER.EXTENSIONCOLUMNS_BUILDNUMBER.EXTENSIONCOLUMNS_MODIFICATIONNUMBER
#define ExtensionColumnsVersionStr T_STRINGIZE(ExtensionColumnsVersionDot)

#define ExtensionColumnsProductName TakionAdditionalColumns
#define ExtensionColumnsProductNameStr T_STRINGIZE(ExtensionColumnsProductName)
#define ExtensionColumnsFileNameStr T_STRINGIZE(ExtensionColumnsProductName.dll)

#ifdef TAKION_NO_OPTIONS
#define ExtensionColumnsPlusVersionDot ExtensionColumnsProductName NoOption ExtensionColumnsVersionDot TakionPlatform TakionConfiguration
#else
#define ExtensionColumnsPlusVersionDot ExtensionColumnsProductName ExtensionColumnsVersionDot TakionPlatform TakionConfiguration
#endif
#define ExtensionColumnsDescriptionStr T_STRINGIZE(ExtensionColumnsPlusVersionDot)
