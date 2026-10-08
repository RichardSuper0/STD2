#ifndef STD2VARIABLES_H
#define STD2VARIABLES_H

#if defined(__cplusplus)
    #if (__cplusplus >= 201103L || __cplusplus >= 201402L || __cplusplus >= 201703L || __cplusplus >= 202002L || __cplusplus >= 202302L)
        #define let auto
        #define const const auto
    #else
        #define let __typeof__
        #define const const __typeof__
    #endif
#else
    #if (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L || __STDC_VERSION__ >= 201112L || __STDC_VERSION__ >= 201710L || __STDC_VERSION__ >= 202311L))
        #define let __auto_type
        #define const const __auto_type
    #else
        #define let
        #define const const
    #endif
#endif

#endif
