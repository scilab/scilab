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
    std::filesystem::path relPath = std::filesystem::path(expanded);
    FREE(expanded);

    std::filesystem::path canonPath = std::filesystem::weakly_canonical(relPath);
    auto relPathIt = relPath.end();
    auto canonPathIt = canonPath.end();
    if ((--relPathIt)->string().empty() && !(--canonPathIt)->string().empty())
    {
        canonPath /= "";
    }

    return os_strdup(std::filesystem::absolute(canonPath).string().c_str());
#endif // _MSC_VER
}

/*--------------------------------------------------------------------------*/
wchar_t *get_full_pathW(const wchar_t * _wcPath)
{
#ifdef _MSC_VER
    wchar_t* pwstExpand = expandPathVariableW(_wcPath);
    std::wstring s(pwstExpand);

    size_t pos = s.find_last_of(L"/\\");

    //remove "file" part
    bool bAppend = false;
    if (pos != std::wstring::npos && s.substr(pos + 1) != L"..")
    {
        s = s.substr(0, pos);
        bAppend = true;
    }

    std::filesystem::path relPath = std::filesystem::path(s);
    std::filesystem::path canonPath = std::filesystem::weakly_canonical(relPath);
    auto relPathIt = relPath.end();
    auto canonPathIt = canonPath.end();
    if ((--relPathIt)->wstring().empty() && !(--canonPathIt)->wstring().empty())
    {
        canonPath /= "";
    }

    std::filesystem::path p = std::filesystem::absolute(canonPath);

    //add "file" part
    if (bAppend)
    {
        p /= (pwstExpand + pos + 1);
    }

    FREE(pwstExpand);
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
