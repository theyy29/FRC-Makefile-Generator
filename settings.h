#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QStringList>
#include <QString>
#include <QDebug>
#include <QMap>
#include <QHash>

#include <iostream>

#include "os.h"

class Settings
{
public:
    Settings(QString os, QString cd, int argc, char *argv[]);

    QStringList getArgumentList();

    void dump();
private:
    bool requirementsFuffilled();
    bool reportRequirements();

private:
    QSettings *configFile;
    QStringList arguments;

    // Use the settingFile and arguments along with
    // some brains (assumptions) to compile the final
    // list of settings.
    QHash<QString, QString> settings;
};

#endif // SETTINGS_H
