/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2021 - ESI Group - Clément DAVID
*
*
* Copyright (C) 2012 - 2016 - Scilab Enterprises
*
* This file is hereby licensed under the terms of the GNU GPL v2.0,
* pursuant to article 5.3.4 of the CeCILL v.2.1.
* This file was originally licensed under the terms of the CeCILL v2.1,
* and continues to be available under such terms.
* For more information, see the COPYING file which you should have received
* along with this program.
*
*/

#include <cstdio>
#include <cstring>
#include <string>
#include "hash/md5.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "md5.h"
}

char* md5_file(FILE* f)
{
    MD5 md5_digest;
    char pStData[64];
    size_t iLen = 0;

    while (!feof(f))
    {
        iLen = fread(pStData, 1, sizeof(pStData), f);
        md5_digest.add(pStData, iLen);
    }

    std::string hash(md5_digest.getHash());
    char* pStHash = (char*) MALLOC(hash.length() + 1);
    std::memcpy(pStHash, hash.data(), hash.length());
    pStHash[hash.length()] = '\0';
    return pStHash;
}
