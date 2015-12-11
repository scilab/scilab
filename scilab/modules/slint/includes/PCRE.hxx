/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __PCRE_TRAITS_HXX__
#define __PCRE_TRAITS_HXX__

#include <cstring>
#include <pcre.h>

extern "C"
{
#include "sci_malloc.h"
#include "charEncoding.h"
}

namespace slint
{

struct PCRETraits
{

#ifdef _MSC_VER

    typedef pcre16 pcre_t;
    typedef pcre16_extra pcre_extra_t;

    inline static pcre16 * pcre_compile(const wchar_t * pattern, int options, const char ** errptr, int * erroffset, const unsigned char * tableptr)
    {
        return pcre16_compile((PCRE_SPTR16)pattern, PCRE_UTF16 | options, errptr, erroffset, tableptr);
    }

    inline static pcre16_extra * pcre_study(const pcre16 * code, int options, const char ** errptr)
    {
        return pcre16_study(code, options, errptr);
    }

    inline static void pcre_free_study(pcre16_extra * extra)
    {
        pcre16_free_study(extra);
    }

    inline static int pcre_exec(const pcre16 * code, const pcre16_extra * extra, const wchar_t * subject, int length, int startoffset, int options, int * ovector, int ovecsize)
    {
        return pcre16_exec(code, extra, (PCRE_SPTR16)subject, length, startoffset, options, ovector, ovecsize);
    }
#else

    typedef pcre32 pcre_t;
    typedef pcre32_extra pcre_extra_t;

    inline static pcre32 * pcre_compile(const wchar_t * pattern, int options, const char ** errptr, int * erroffset, const unsigned char * tableptr)
    {
        return pcre32_compile((PCRE_SPTR32)pattern, PCRE_UTF32 | options, errptr, erroffset, tableptr);
    }

    inline static pcre32_extra * pcre_study(const pcre32 * code, int options, const char ** errptr)
    {
        return pcre32_study(code, options, errptr);
    }

    inline static void pcre_free_study(pcre32_extra * extra)
    {
        pcre32_free_study(extra);
    }

    inline static int pcre_exec(const pcre32 * code, const pcre32_extra * extra, const wchar_t * subject, int length, int startoffset, int options, int * ovector, int ovecsize)
    {
        return pcre32_exec(code, extra, (PCRE_SPTR32)subject, length, startoffset, options, ovector, ovecsize);
    }
#endif

    inline static void pcre_free(void * ptr)
    {
        ::pcre_free(ptr);
    }

};

} // namespace slint

#endif // __PCRE_TRAITS_HXX__
