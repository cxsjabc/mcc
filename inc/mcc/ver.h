#ifndef VER_H
#define VER_H

#define MAJOR "1"
#define MINOR "0"
#define BUILD_VER_DATE __DATE__
#define BUILD_VER_TIME __TIME__

#define VERSION MAJOR "." MINOR

const char *gen_build_ver();

#endif