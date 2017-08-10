/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *
 *  Copyright (C) 2017 - ESI-Group - Cedric DELAMARRE
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __SCICURL_HXX__
#define __SCICURL_HXX__

#include <curl/curl.h>
#include <sys/stat.h>

#include "internal.hxx"

extern "C"
{
#include "dynlib_webtools.h"
}

class WEBTOOLS_IMPEXP SciCurl
{
public:
    static SciCurl* getInstance(void);
    static void destroyInstance(void);

    void getResultAsObject(CURL* curl);
    void getResultAsFile(CURL* curl, FILE* fd);
    int setProxy(CURL* curl);

    types::InternalType* getResult(void);
    void clear(void);

    static int write_result(char* pcInput, size_t size, size_t nmemb, void* output);

private:
    SciCurl();
    ~SciCurl();

    static SciCurl* me;
    static std::string data;
    static bool useFile;
};

#endif /* !__SCICURL_HXX__ */
