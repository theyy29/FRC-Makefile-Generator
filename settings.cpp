#include "settings.h"

Settings::Settings(QString os, QString cd, int argc, char *argv[])
{
    for(int i = 0; i < argc; i++){
        arguments << argv[i];
    }

    std::cout << "Arguments:";
    qDebug() << arguments;

    //// Config File Name (begin) ////
    QStringList tlist = arguments; // I really need to stop using the temporary lists.

    // Check to see what config file we should use.
    QString configFileName = cd + "/MakeGen.ini";

    while(!tlist.empty()){
        QString arg = tlist.takeLast();

        // Case sensitive if running linux. Not bothering about the case sensitive osx file system possibiltiy.
        if (arg.startsWith("--config=", ( OS == OS_LINUX ? Qt::CaseSensitive : Qt::CaseInsensitive ))){
            configFileName = arg.right(arg.length() - 9); // remove --config=
            break; // dont care if there is another --config=
        }
    }

    // Add this to the config hash -- useful for a error, and resulting config dump.
    settings.insert("config", configFileName);

    //// Config File Name (end) ////

    //// Open(?) Config File (begin) ////
    configFile = new QSettings(configFileName, QSettings::IniFormat);
    if(!(configFile->status() == QSettings::NoError)){
        std::cerr << "Error after opening settingFile with path \'" << configFileName.toStdString().c_str() << "\'\n";
        std::cerr << "If this is because all things are being defined by arguments than ignore this message.\n";
    }
    //// Open(?) Config File (end) ////


    QStringList keys = configFile->allKeys();


    //// Project Name (begin) ////

    QString projectName = "";

    // Check for a value from the config file.
    if(keys.contains("projectName", ( OS == OS_LINUX ? Qt::CaseSensitive : Qt::CaseInsensitive ))){
        projectName = configFile->value("projectName").toString();
    }

    // Now check to see if an argument for it was passed in

    tlist = arguments; // I really need to stop using the temporary lists.

    while(!tlist.empty()){
        QString arg = tlist.takeLast();

        // Case sensitive if running linux. Not bothering about the case sensitive osx file system possibiltiy.
        if (arg.startsWith("--project-name=", ( OS == OS_LINUX ? Qt::CaseSensitive : Qt::CaseInsensitive ))){
            projectName = arg.right(arg.length() - 15); // remove --project-name=
            break;
        }
    }

    // Add to config hash
    settings.insert("projectName", projectName);

    //// Project Name (end)////



    //// Build Subdirectory (begin) ////

    QString buildDir = "PPC603gnu";

    // Check for a value from the config file.
    if(keys.contains("buildDir", ( OS == OS_LINUX ? Qt::CaseSensitive : Qt::CaseInsensitive ))){
        buildDir = configFile->value("buildDir").toString();
    }

    // Now check to see if an argument for it was passed in

    tlist = arguments; // I really need to stop using the temporary lists.

    while(!tlist.empty()){
        QString arg = tlist.takeLast();

        // Case sensitive if running linux. Not bothering about the case sensitive osx file system possibiltiy.
        if (arg.startsWith("--build-dir=", ( OS == OS_LINUX ? Qt::CaseSensitive : Qt::CaseInsensitive ))){
            buildDir = arg.right(arg.length() - 12); // remove --build-dir=
            break;
        }
    }

    // Add to config hash
    settings.insert("buildDir", buildDir);

    //// Build Subdirectory (end) ////



    //// Excludes (begin) ////
    // These need a lot of work. it needs to explode on ','
    // and to be added to the directory thingy

    QString exclude = "";

    // Check for a value from the config file.
    if(keys.contains("exclude", ( OS == OS_LINUX ? Qt::CaseSensitive : Qt::CaseInsensitive ))){
        exclude = configFile->value("exclude").toString();
    }

    // Now check to see if an argument for it was passed in

    tlist = arguments; // I really need to stop using the temporary lists.

    while(!tlist.empty()){
        QString arg = tlist.takeLast();

        // Case sensitive if running linux. Not bothering about the case sensitive osx file system possibiltiy.
        if (arg.startsWith("--exclude=", ( OS == OS_LINUX ? Qt::CaseSensitive : Qt::CaseInsensitive ))){
            exclude += ",";
            exclude += arg.right(arg.length() - 10); // remove --exclude=
            break;
        }
    }

    // Add to config hash
    settings.insert("exclude", exclude);

    //// Excludes (end) ////



    //// Projects Directory (begin) ////
    // Note: OS specific.
    QString projectsDir = cd;

    // Check for a value from the config file.
    if(keys.contains(OS"/projectsDir", ( OS == OS_LINUX ? Qt::CaseSensitive : Qt::CaseInsensitive ))){
        projectsDir = configFile->value(OS"/projectsDir").toString();
    }

    // Now check to see if an argument for it was passed in

    tlist = arguments; // I really need to stop using the temporary lists.

    while(!tlist.empty()){
        QString arg = tlist.takeLast();

        // Case sensitive if running linux. Not bothering about the case sensitive osx file system possibiltiy.
        if (arg.startsWith("--projects-dir=", ( OS == OS_LINUX ? Qt::CaseSensitive : Qt::CaseInsensitive ))){
            projectsDir = arg.right(arg.length() - 15); // remove --projects-dir=
            break;
        }
    }

    // Add to config hash
    settings.insert("projectsDir", projectsDir);

    //// Projects Directory (end) ////

    dump();
}

bool Settings::requirementsFuffilled(){
    /*
    bool contains(const Key &key) const;
    const Key key(const T &value) const;
    const Key key(const T &value, const Key &defaultKey) const;
    const T value(const Key &key) const;
    const T value(const Key &key, const T &defaultValue) const;
    T &operator[](const Key &key);
    const T operator[](const Key &key) const;

    QList<Key> uniqueKeys() const;
    QList<Key> keys() const;
    QList<Key> keys(const T &value) const;
    QList<T> values() const;
    QList<T> values(const Key &key) const;
    int count(const Key &key) const;
     */
    QStringList reqs;
    reqs << "projectName";
}

bool Settings::reportRequirements(){
    ;
}

void Settings::dump(){

    qDebug() << "QStringList arguments:" << arguments;

    qDebug() << "QHash settings:" << settings;

//    Fix this
//    configFile->dumpObjectTree();
//    configFile->dumpObjectInfo();
}
