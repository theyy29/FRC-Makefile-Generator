#include <QtCore/QtCore>
#include <QDebug>
#include <QDir>
#include <QStringList>

#include <iostream>
#include <fstream>
#include "settings.h"

#include "os.h"

void writeover(FILE *f, FILE *fdo);

int main(int argc, char *argv[]){
    QCoreApplication a(argc, argv);
    QString runPath = qApp->applicationDirPath();

    Settings settings(OS, runPath, argc, argv);

    QString projectsDir = settings.getProjectDir();
    QString projectName = settings.getProjectName();
    QString buildDir    = settings.getBuildDir();

    QString projectPath = projectsDir + "/" + projectName;
    QString buildPath   = projectPath + "/" + buildDir;

    QStringList list;
    QStringList dirlist;

    dirlist << projectPath;

    QStringList dlist = QDir(projectPath, "*.cpp").entryList(); // Get all source files in the project folder
    while(!dlist.empty()){
        QString file = QString("%1/%2").arg(projectPath).arg(dlist.takeLast()); // need to add on the first part of file directory name
        file = file.right(file.length() - (projectPath.length() + 1)); // Length of entire path past project dir (includes trailing /)
        file.chop(4);
        list += file; // remove constant filename prefix, and suffix
    }

    QDirIterator directories(projectPath, QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    while(directories.hasNext()){
        directories.next();
        if(!directories.filePath().startsWith(buildPath, Qt::CaseInsensitive)){
            QString path = directories.filePath();

            QDir projectDir( path, "*.cpp");
            QStringList nlist = projectDir.entryList();

            while(!nlist.empty()){
                QString file = QString("%1/%2").arg(path).arg(nlist.takeLast()); // need to add on the first part of file directory name
                file = file.right(file.length() - (projectPath.length() + 1)); // remove constant filename prefix
                file.chop(4); // remove suffix
                list += file;
            }

            dirlist.append(path);
        }
    }

    qDebug() << "list" << list;
    qDebug() << "dirlist" << dirlist;

    if(list.empty()){
        std::cout << "Error: No .cpp files to compile!\n";
        exit(-2);
    }


    /////// WRITING ////////

    unlink("Makefile"); // remove if it exists.
    std::ofstream of;
    of.open("Makefile");

    if(!of.is_open()){
        std::cout << "Counld not open file.";
        return -7;
    }

    of << "WIND_HOME := $(subst \\,/,$(WIND_HOME))" << std::endl;
    of << "WIND_BASE := $(subst \\,/,$(WIND_BASE))" << std::endl;
    of << "WIND_USR := $(subst \\,/,$(WIND_USR))" << std::endl;
    of << "" << std::endl;
    of << "all : pre_build main_all post_build" << std::endl;
    of << "" << std::endl;
    of << "_clean ::" << std::endl;
    of << "@echo \"make: removing targets and objects of `pwd`\"" << std::endl;
    of << "" << std::endl;
    of << "TRACE=0" << std::endl;
    of << "TRACEON=$(TRACE:0=@)" << std::endl;
    of << "TRACE_FLAG=$(TRACEON:1=)" << std::endl;
    of << "" << std::endl;
    of << "MAKEFILE := Makefile" << std::endl;
    of << "" << std::endl;
    of << "FLEXIBLE_BUILD := 1" << std::endl;
    of << "" << std::endl;
    of << "BUILD_SPEC = PPC603gnu" << std::endl;
    of << "DEBUG_MODE = 1" << std::endl;
    of << "ifeq ($(DEBUG_MODE),1)" << std::endl;
    of << "MODE_DIR := Debug" << std::endl;
    of << "else" << std::endl;
    of << "MODE_DIR := NonDebug" << std::endl;
    of << "endif" << std::endl;
    of << "OBJ_DIR := ." << std::endl;
    of << "WS_ROOT_DIR := " << projectsDir.toStdString() << std::endl;
    of << "PRJ_ROOT_DIR := $(WS_ROOT_DIR)/" << projectName.toStdString() << std::endl;
    of << "" << std::endl;
    of << "" << std::endl;
    of << "" << std::endl;
    of << "#Global Build Macros" << std::endl;
    of << "PROJECT_TYPE = DKM" << std::endl;
    of << "DEFINES =" << std::endl;
    of << "EXPAND_DBG = 0" << std::endl;
    of << "" << std::endl;
    of << "" << std::endl;
    of << "#BuildSpec specific Build Macros" << std::endl;
    of << "VX_CPU_FAMILY = ppc" << std::endl;
    of << "CPU = PPC603" << std::endl;
    of << "TOOL_FAMILY = gnu" << std::endl;
    of << "TOOL = gnu" << std::endl;
    of << "TOOL_PATH =" << std::endl;
    of << "CC_ARCH_SPEC = -mcpu=603 -mstrict-align -mno-implicit-fp -mlongcall" << std::endl;
    of << "LIBPATH =" << std::endl;
    of << "LIBS =" << std::endl;
    of << "" << std::endl;
    of << "IDE_INCLUDES = -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/WPILib -I$(WIND_BASE)/target/h/wrn/coreip -I$(PRJ_ROOT_DIR) -I$(PRJ_ROOT_DIR)/Commands -I$(PRJ_ROOT_DIR)/Subsystems" << std::endl;
    of << "IDE_LIBRARIES = $(WIND_BASE)/target/lib/WPILib.a" << std::endl;
    of << "" << std::endl;
    of << "IDE_DEFINES =" << std::endl;
    of << "" << std::endl;
    of << "" << std::endl;
    of << "#BuildTool flags" << std::endl;
    of << "ifeq ($(DEBUG_MODE),1)" << std::endl;
    of << "DEBUGFLAGS_C-Compiler = -g" << std::endl;
    of << "DEBUGFLAGS_C++-Compiler = -g" << std::endl;
    of << "DEBUGFLAGS_Linker = -g" << std::endl;
    of << "DEBUGFLAGS_Partial-Image-Linker =" << std::endl;
    of << "DEBUGFLAGS_Librarian =" << std::endl;
    of << "DEBUGFLAGS_Assembler = -g" << std::endl;
    of << "else" << std::endl;
    of << "DEBUGFLAGS_C-Compiler = -O2 -fstrength-reduce -fno-builtin" << std::endl;
    of << "DEBUGFLAGS_C++-Compiler = -O2 -fstrength-reduce -fno-builtin" << std::endl;
    of << "DEBUGFLAGS_Linker = -O2 -fstrength-reduce -fno-builtin" << std::endl;
    of << "DEBUGFLAGS_Partial-Image-Linker =" << std::endl;
    of << "DEBUGFLAGS_Librarian =" << std::endl;
    of << "DEBUGFLAGS_Assembler = -O2 -fstrength-reduce -fno-builtin" << std::endl;
    of << "endif" << std::endl;
    of << "" << std::endl;
    of << "" << std::endl;
    of << "#Project Targets" << std::endl;
    of << "PROJECT_TARGETS = " << projectName.toStdString() << "/$(MODE_DIR)/" << projectName.toStdString() << ".out \\" << std::endl;
    of << "\t" << projectName.toStdString() << "_partialImage/$(MODE_DIR)/" << projectName.toStdString() << "_partialImage.o" << std::endl;
    of << "" << std::endl;
    of << "#Rules" << std::endl;
    of << "# "<< projectName.toStdString() << std::endl;
    of << "ifeq ($(DEBUG_MODE),1)" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -g" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker =" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/% : DEBUGFLAGS_Librarian =" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g" << std::endl;
    of << "else" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -O2 -fstrength-reduce -fno-builtin" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -O2 -fstrength-reduce -fno-builtin" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -O2 -fstrength-reduce -fno-builtin" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker =" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/% : DEBUGFLAGS_Librarian =" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -O2 -fstrength-reduce -fno-builtin" << std::endl;
    of << "endif" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/% : IDE_INCLUDES = -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/WPILib -I$(WIND_BASE)/target/h/wrn/coreip" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/% : IDE_LIBRARIES = $(WIND_BASE)/target/lib/WPILib.a" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/% : IDE_DEFINES =" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/% : PROJECT_TYPE = DKM" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/% : DEFINES =" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/% : EXPAND_DBG = 0" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/% : VX_CPU_FAMILY = ppc" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/% : CPU = PPC603" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/% : TOOL_FAMILY = gnu" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/% : TOOL = gnu" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/% : TOOL_PATH =" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/% : CC_ARCH_SPEC = -mcpu=603 -mstrict-align -mno-implicit-fp -mlongcall" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/% : LIBPATH =" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/% : LIBS =" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/% : OBJ_DIR := " << projectName.toStdString() << "/$(MODE_DIR)" << std::endl;
    of << "" << std::endl;
    of << "OBJECTS_"<< projectName.toStdString() << " = " << projectName.toStdString() << "_partialImage/$(MODE_DIR)/" << projectName.toStdString() << "_partialImage.o" << std::endl;
    of << "" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/" << projectName.toStdString() << ".out : $(OBJECTS_"<< projectName.toStdString() <<")" << std::endl;
    of << "\t$(TRACE_FLAG)if [ ! -d \"`dirname \"$@\"`\" ]; then mkdir -p \"`dirname \"$@\"`\"; fi;echo \"building $@\";rm -f \"$@\";nmppc $(OBJECTS_" << projectName.toStdString() << ") $(IDE_LIBRARIES) | tclsh $(WIND_BASE)/host/resource/hutils/tcl/munch.tcl -c ppc > $(OBJ_DIR)/ctdt.c; $(TOOL_PATH)ccppc $(DEBUGFLAGS_Linker) $(CC_ARCH_SPEC) -fdollars-in-identifiers -Wall $(ADDED_CFLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES)  -o $(OBJ_DIR)/ctdt.o -c $(OBJ_DIR)/ctdt.c; $(TOOL_PATH)ccppc -r -nostdlib -Wl,-X -T $(WIND_BASE)/target/h/tool/gnu/ldscripts/link.OUT -o \"$@\" $(OBJ_DIR)/ctdt.o $(OBJECTS_" << projectName.toStdString() << ") $(IDE_LIBRARIES) $(LIBPATH) $(LIBS)  $(ADDED_LIBPATH) $(ADDED_LIBS) && if [ \"$(EXPAND_DBG)\" = \"1\" ]; then plink \"$@\";fi" << std::endl;
    of << "" << std::endl;
    of << projectName.toStdString() << "/$(MODE_DIR)/" << projectName.toStdString() << "_compile_file : $(FILE) ;" << std::endl;
    of << "" << std::endl;
    of << "_clean :: " << projectName.toStdString() << "/$(MODE_DIR)/" << projectName.toStdString() << "_clean" << std::endl;
    of << "" << std::endl;
    of << projectName.toStdString() <<  "/$(MODE_DIR)/" << projectName.toStdString() << "_clean :" << std::endl;
    of << "\t$(TRACE_FLAG)if [ -d \"" << projectName.toStdString() << "\" ]; then cd \"" << projectName.toStdString() << "\"; rm -rf $(MODE_DIR); fi" << std::endl;
    of << "" << std::endl;
    of << "" << std::endl;
    of << "# " << projectName.toStdString() << "_partialImage" << std::endl;
    of << "ifeq ($(DEBUG_MODE),1)" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -g" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker =" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Librarian =" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g" << std::endl;
    of << "else" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -O2 -fstrength-reduce -fno-builtin" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -O2 -fstrength-reduce -fno-builtin" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -O2 -fstrength-reduce -fno-builtin" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker =" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Librarian =" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -O2 -fstrength-reduce -fno-builtin" << std::endl;
    of << "endif" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/% : IDE_INCLUDES = -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/WPILib -I$(WIND_BASE)/target/h/wrn/coreip" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/% : IDE_LIBRARIES = $(WIND_BASE)/target/lib/WPILib.a" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/% : IDE_DEFINES =" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/% : PROJECT_TYPE = DKM" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/% : DEFINES =" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/% : EXPAND_DBG = 0" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/% : VX_CPU_FAMILY = ppc" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/% : CPU = PPC603" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/% : TOOL_FAMILY = gnu" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/% : TOOL = gnu" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/% : TOOL_PATH =" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/% : CC_ARCH_SPEC = -mcpu=603 -mstrict-align -mno-implicit-fp -mlongcall" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/% : LIBPATH =" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/% : LIBS =" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/% : OBJ_DIR := " << projectName.toStdString() << "_partialImage/$(MODE_DIR)" << std::endl;
    of << "" << std::endl;
    of << "" << std::endl;
    of << "" << std::endl;
    of << "" << std::endl;

    QStringList tlist = list;
    while(!tlist.empty()){
        QString fl = tlist.takeLast();
        of << projectName.toStdString() << "_partialImage/$(MODE_DIR)/Objects/" << projectName.toStdString() << "/" << fl.toStdString() << ".o : $(PRJ_ROOT_DIR)/" << fl.toStdString() << ".cpp $(FORCE_FILE_BUILD)" << std::endl;
        of << "\t$(TRACE_FLAG)if [ ! -d \"`dirname \"$@\"`\" ]; then mkdir -p \"`dirname \"$@\"`\"; fi;echo \"building $@\"; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP -mlongcall $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL   $(DEFINES) -o \"$@\" -c \"$<\"" << std::endl << std::endl << std::endl;
    }

    tlist = list;
    if(!tlist.empty()){
        QString fl = tlist.takeLast();
        of << "OBJECTS_" << projectName.toStdString() << "_partialImage = " << projectName.toStdString() << "_partialImage/$(MODE_DIR)/Objects/" << projectName.toStdString() << "/" << fl.toStdString() << ".o ";
    }

    while(!tlist.empty()){
        QString fl = tlist.takeLast();
        of << " \\\n\t" << projectName.toStdString() << "_partialImage/$(MODE_DIR)/Objects/" << projectName.toStdString() << "/" << fl.toStdString() << ".o ";
    }
    of << std::endl << std::endl;


    of << "" << std::endl;
    of << "" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/" << projectName.toStdString() << "_partialImage.o : $(OBJECTS_" << projectName.toStdString() << "_partialImage)" << std::endl;
    of << "\t$(TRACE_FLAG)if [ ! -d \"`dirname \"$@\"`\" ]; then mkdir -p \"`dirname \"$@\"`\"; fi;echo \"building $@\"; $(TOOL_PATH)ccppc -r -nostdlib -Wl,-X  -o \"$@\" $(OBJECTS_" << projectName.toStdString() << "_partialImage) $(LIBPATH) $(LIBS) $(ADDED_LIBPATH) $(ADDED_LIBS)  && if [ \"$(EXPAND_DBG)\" = \"1\" ]; then plink \"$@\";fi" << std::endl;
    of << "" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/" << projectName.toStdString() << "_partialImage_compile_file : $(FILE) ;" << std::endl;
    of << "" << std::endl;
    of << "_clean :: " << projectName.toStdString() << "_partialImage/$(MODE_DIR)/" << projectName.toStdString() << "_partialImage_clean" << std::endl;
    of << "" << std::endl;
    of <<  projectName.toStdString() << "_partialImage/$(MODE_DIR)/" << projectName.toStdString() << "_partialImage_clean :" << std::endl;
    of << "\t$(TRACE_FLAG)if [ -d \"" << projectName.toStdString() << "_partialImage\" ]; then cd \"" << projectName.toStdString() << "_partialImage\"; rm -rf $(MODE_DIR); fi" << std::endl;
    of << "" << std::endl;
    of << "force :" << std::endl;

    tlist = list;
    if(!tlist.empty()){
        QString fl = tlist.takeLast();
        of << "DEP_FILES := " << projectName.toStdString() << "_partialImage/$(MODE_DIR)/Objects/" << projectName.toStdString() << "/" << fl.toStdString() << ".d ";
    }

    while(!tlist.empty()){
        QString fl = tlist.takeLast();
        of << " \\\n\t" << projectName.toStdString() << "_partialImage/$(MODE_DIR)/Objects/" << projectName.toStdString() << "/" << fl.toStdString() << ".d ";
    }

    of << std::endl << std::endl;

    of << "-include $(DEP_FILES)" << std::endl;
    of << "" << std::endl;
    of << "-include $(PRJ_ROOT_DIR)/*.makefile" << std::endl;
    of << "" << std::endl;
    of << "-include *.makefile" << std::endl;
    of << "" << std::endl;
    of << "main_all : external_build $(PROJECT_TARGETS)" << std::endl;
    of << "\t@echo \"make: built targets of `pwd`\"" << std::endl;
    of << "" << std::endl;
    of << "" << std::endl;
    of << "# entry point for extending the build" << std::endl;
    of << "external_build ::" << std::endl;
    of << "\t@echo \"\"" << std::endl;
    of << "" << std::endl;
    of << "# main entry point for pre processing prior to the build" << std::endl;
    of << "pre_build :: $(PRE_BUILD_STEP) generate_sources" << std::endl;
    of << "\t@echo """ << std::endl;
    of << "" << std::endl;
    of << "# entry point for generating sources prior to the build" << std::endl;
    of << "generate_sources ::" << std::endl;
    of << "\t@echo """ << std::endl;
    of << "" << std::endl;
    of << "# main entry point for post processing after the build" << std::endl;
    of << "post_build :: $(POST_BUILD_STEP) deploy_output" << std::endl;
    of << "\t@echo """ << std::endl;
    of << "" << std::endl;
    of << "# entry point for deploying output after the build" << std::endl;
    of << "deploy_output ::" << std::endl;
    of << "\t@echo """ << std::endl;
    of << "" << std::endl;
    of << "clean :: external_clean $(CLEAN_STEP) _clean" << std::endl;
    of << "" << std::endl;
    of << "# entry point for extending the build clean" << std::endl;
    of << "external_clean ::" << std::endl;
    of << "\t@echo """ << std::endl;
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
