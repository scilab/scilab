
#ifndef __DYNLIB_AST_TOOLS_HXX__
#define __DYNLIB_AST_TOOLS_HXX__

#ifdef _MSC_VER
    #if AST_TOOLS_EXPORTS
        #define EXTERN_AST_TOOLS __declspec (dllexport)
    #else
        #define EXTERN_AST_TOOLS __declspec (dllimport)
    #endif
#else
    #define EXTERN_AST_TOOLS 
#endif

#endif /* !__DYNLIB_AST_TOOLS_HXX__ */