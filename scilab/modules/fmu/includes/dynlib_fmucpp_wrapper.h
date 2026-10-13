/*--------------------------------------------------------------------------*/
#ifndef __DYNLIB_FMUCPP_WRAPPER_H__
#define __DYNLIB_FMUCPP_WRAPPER_H__

#ifdef _MSC_VER
#ifdef LIBFMUCPP_WRAPPER_GW_EXPORTS
#define FMUCPP_IMPEXP __declspec(dllexport)
#else
#define FMUCPP_IMPEXP __declspec(dllimport)
#endif
#else
#define FMUCPP_IMPEXP 
#endif

#endif /* __DYNLIB_FMUCPP_WRAPPER_H__ */
/*--------------------------------------------------------------------------*/
