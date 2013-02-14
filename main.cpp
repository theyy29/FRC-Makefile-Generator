#include <QtCore/QtCore>
#include <QDebug>
#include <QDir>
#include <QStringList>

#include <iostream>

#include "settings.h"

#include "os.h"

#define PROJECT_DIR "/home/ethan/Development/2013"
// Need to get this size and replace where there are 28/29

void writeover(FILE *f, FILE *fdo);


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString runPath = qApp->applicationDirPath();

    Settings settings(OS, runPath, argc, argv);

//    printf("File call/Arguments: %s", argv[0]);
//    for(int i = 1; i < argc; i++){
//        printf(" %s", argv[i]);
//    }
//    printf("\n");

    QStringList list;
    QStringList dirlist;

    dirlist << PROJECT_DIR;

    QStringList dlist = QDir(PROJECT_DIR, "*.cpp").entryList(); // Get all source files in the project folder
    while(!dlist.empty()){
        QString file = QString("%1/%2").arg(PROJECT_DIR).arg(dlist.takeLast()); // need to add on the first part of file directory name
        file = file.right(file.length()-29);
        file.chop(4);
        list += file; // remove constant filename prefix, and suffix
    }

    QDirIterator directories(PROJECT_DIR, QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    while(directories.hasNext()){
        directories.next();
        if(!directories.filePath().startsWith(PROJECT_DIR"/PPC603gnu", Qt::CaseInsensitive)){
            QString path = directories.filePath();

            QDir projectDir( path, "*.cpp");
            QStringList nlist = projectDir.entryList();

            while(!nlist.empty()){
                QString file = QString("%1/%2").arg(path).arg(nlist.takeLast()); // need to add on the first part of file directory name
                file = file.right(file.length()-29); // remove constant filename prefix
                file.chop(4); // remove suffix
                list += file;
            }

            dirlist.append(path);
        }
    }

    qDebug() << list;
    qDebug() << dirlist;

    if(list.empty()){
        std::cout << "Error: No .cpp files to compile!\n";
        exit(-2);
    }

    unlink("Makefile"); // remove if it exists.
    FILE *f = fopen("Makefile", "w"); // open to write

    FILE *fdp1 = fopen("mkp1", "r"); // open to read
    FILE *fdp2 = fopen("mkp2", "r");
    FILE *fdp3 = fopen("mkp3", "r");

    if(f == NULL || fdp1 == NULL || fdp2 == NULL || fdp3 == NULL){
        std::cout << "opening failed :(\n";
        exit(-3);
    }


    /////// WRITING ////////

    // Write first part. There are project specific settings here, ie. project root folder
    writeover(f, fdp1);

    QStringList tlist = list;
    while(!tlist.empty()){
        QString fl = tlist.takeLast();
        fprintf(f, "2013_partialImage/$(MODE_DIR)/Objects/2013/%s.o : $(PRJ_ROOT_DIR)/%s.cpp $(FORCE_FILE_BUILD)\n", fl.toStdString().c_str(), fl.toStdString().c_str());
        fprintf(f, "\t$(TRACE_FLAG)if [ ! -d \"`dirname \"$@\"`\" ]; then mkdir -p \"`dirname \"$@\"`\"; fi;echo \"building $@\"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o \"$@\" -c \"$<\"\n\n\n");
    }

    tlist = list;
    if(!tlist.empty()){
        QString fl = tlist.takeLast();
        fprintf(f, "OBJECTS_2013_partialImage = 2013_partialImage/$(MODE_DIR)/Objects/2013/%s.o ", fl.toStdString().c_str());
    }

    while(!tlist.empty()){
        QString fl = tlist.takeLast();
        fprintf(f, " \\\n\t2013_partialImage/$(MODE_DIR)/Objects/2013/%s.o ", fl.toStdString().c_str());
    }
    fprintf(f, "\n\n");

    // Write second consistent part
    writeover(f, fdp2);

    tlist = list;
    if(!tlist.empty()){
        QString fl = tlist.takeLast();
        fprintf(f, "DEP_FILES := 2013_partialImage/$(MODE_DIR)/Objects/2013/%s.d ", fl.toStdString().c_str());
    }

    while(!tlist.empty()){
        QString fl = tlist.takeLast();
        fprintf(f, " \\\n\t2013_partialImage/$(MODE_DIR)/Objects/2013/%s.d ", fl.toStdString().c_str());
    }

    // Write the last part
    writeover(f, fdp3);

    return 0;
}


// Write from file descriptor of original over to fd of new makefile
void writeover(FILE *f, FILE *fdo){
    while(!feof(fdo)){
        char c = fgetc(fdo);
        if(c != 'ÿ')
            fputc(c, f);
        else
            qDebug() << "Found invalid character \'ÿ\'\n"; // '\255' is the character.
    }
}
