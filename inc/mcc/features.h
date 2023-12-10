#ifndef FEATURES_H
#define FEATURES_H

#ifdef __cplusplus
#undef __BEGIN_DECLS
#define __BEGIN_DECLS extern "C" {
#undef __END_DECLS
#define __END_DECLS }
#else
#define __BEGIN_DECLS
#define __END_DECLS
#endif

#endif