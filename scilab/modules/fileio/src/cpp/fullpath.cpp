/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2019-2011 - Stéphane MOTTELET
*
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/
/*--------------------------------------------------------------------------*/

#include <filesystem>

extern "C"
{
#include "charEncoding.h"
#include "fullpath.h"
#include "sci_malloc.h"
#include "os_string.h"
#include "expandPathVariable.h"
}

char *get_full_path(const char *_Path)
{
#ifdef _MSC_VER
    char *_FullPath = NULL;
    wchar_t *wFullPath = NULL;
    wchar_t *wPath = to_wide_string((char *)_Path);

    if (wPath)
    {
        wFullPath = get_full_pathW(wPath);
        FREE(wPath);
        if (wFullPath)
        {
            _FullPath = wide_string_to_UTF8(wFullPath);
            FREE(wFullPath);
        }
    }
    return _FullPath;
#else // POSIX
    char* expanded = expandPathVariable(_Path);
    std::filesystem::path p = std::filesystem::path(expanded);
    FREE(expanded);
    if (p.empty())
    {
        p = std::filesystem::current_path();
    }
    else
    {
        if (p.is_relative())
        {
            p = std::filesystem::absolute(p);
        }
        p = std::filesystem::weakly_canonical(p);
    }

    // preserve trailing slash
    size_t len = strlen(_Path);
    if (len > 0 && _Path[len - 1] == std::filesystem::path::preferred_separator)
    {
        p /= "";
    }

    return os_strdup(p.string().c_str());
#endif // _MSC_VER
}

/*--------------------------------------------------------------------------*/
wchar_t *get_full_pathW(const wchar_t * _wcPath)
{
#ifdef _MSC_VER
    wchar_t* pwstExpand = expandPathVariableW(_wcPath);
    std::filesystem::path p = std::filesystem::path(pwstExpand);
    FREE(pwstExpand);
    if (p.empty())
    {
        p = std::filesystem::current_path();
    }
    else
    {
        if (p.is_relative())
        {
            p = std::filesystem::absolute(p);
        }
        p = std::filesystem::weakly_canonical(p);
    }

    // preserve trailing slash
    size_t len = wcslen(_wcPath);
    if (len > 0 && _wcPath[len - 1] == std::filesystem::path::preferred_separator)
    {
        p /= "";
    }

    return os_wcsdup(p.wstring().c_str());
#else // POSIX
    char *_FullPath = NULL;
    wchar_t *wFullPath = NULL;
    char *_Path = wide_string_to_UTF8(_wcPath);

    if (_Path)
    {
        _FullPath = get_full_path(_Path);
        FREE(_Path);
        if (_FullPath)
        {
            wFullPath = to_wide_string(_FullPath);
            FREE(_FullPath);
        }
    }
    return wFullPath;
#endif // _MSC_VER
}
