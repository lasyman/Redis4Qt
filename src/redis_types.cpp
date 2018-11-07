#include "redis_types.h"
#include <QSettings>

Redis4QtCommon::Redis4QtCommon()
{

}

Redis4QtCommon::~Redis4QtCommon()
{

}

QString Redis4QtCommon::getValue(const QString &strsection,
                                 const QString &strKey,
                                 const QString &strCfgFile,
                                 const QVariant &defaultValue)
{
    QSettings settings(strCfgFile, QSettings::IniFormat);
    settings.beginGroup(strsection);
    QString strValue = settings.value(strKey, "").toString();
    settings.endGroup();
    if (strValue.isEmpty())
        return defaultValue.toString();
    else
        return strValue;
}

void Redis4QtCommon::setValue(const QString &strsection, const QString &strKey, const QString &strValue, const QString &strCfgFile)
{
    QSettings settings(strCfgFile, QSettings::IniFormat);
    settings.beginGroup(strsection);
    settings.setValue(strKey, strValue);
    settings.endGroup();
}
