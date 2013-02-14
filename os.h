#ifndef OS_H
#define OS_H

#include <qglobal.h>

//http://stackoverflow.com/questions/3063110/get-the-current-operating-system-during-runtime-in-c

#define OS_LINUX   "Linux"
#define OS_WINDOWS "Windows"
#define OS_OSX     "OSX"
#define OS_OTHER   "Other"

#if defined(Q_OS_LINUX)
  #define OS OS_LINUX
  #include <unistd.h>
#elif defined(Q_OS_WIN32)
  #define OS OS_WINDOWS
  #include <fcntl.h>
#elif defined(Q_OS_DARWIN)
  #error Cannot compile for osx :(
  #define OS OS_OSX
#else
  #define OS OS_OTHER
#endif


#endif // OS_H
