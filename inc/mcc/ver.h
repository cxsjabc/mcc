#ifndef VER_H
#define VER_H

#define MAJOR "1"
#define MINOR "0"
#define BUILD_VER_DATE __DATE__
#define BUILD_VER_TIME __TIME__

#define VERSION MAJOR "." MINOR

__BEGIN_DECLS

const char *gen_build_ver();

__END_DECLS

#endif