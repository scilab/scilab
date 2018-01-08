/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*
* Copyright (C) 2017 - ESI-Group - Cedric Delamarre
*
* This file is hereby licensed under the terms of the GNU GPL v2.0,
* pursuant to article 5.3.4 of the CeCILL v.2.1.
* This file was originally licensed under the terms of the CeCILL v2.1,
* and continues to be available under such terms.
* For more information, see the COPYING file which you should have received
* along with this program.
*
*/
#include <stdio.h>
#include "sciCurl.hxx"
#include "string.hxx"
#include "json.hxx"

extern "C"
{
    #include "getScilabPreference.h"
    #include "freeArrayOfString.h"
}

SciCurl* SciCurl::me = nullptr;
std::string SciCurl::data;
bool SciCurl::useFile = false;

SciCurl* SciCurl::getInstance(void)
{
    if(me == nullptr)
    {
        me = new SciCurl();
    }

    return me;
}

void SciCurl::destroyInstance(void)
{
    if (me)
    {
        delete me;
        me = nullptr;
    }
}

SciCurl::SciCurl()
{
    curl_global_init(CURL_GLOBAL_ALL);
}

SciCurl::~SciCurl()
{
    curl_global_cleanup();
}

void SciCurl::getResultAsObject(CURL* curl)
{
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, SciCurl::write_result);
    useFile = false;
}

void SciCurl::getResultAsFile(CURL* curl, FILE* fd)
{
#ifdef _MSC_VER
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, SciCurl::write_result);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fd);
    useFile = true;
#else
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fd);
#endif
}

types::InternalType* SciCurl::getResult()
{
    types::InternalType* res = fromJSON(data);
    if (res == nullptr)
    {
        res = new types::String(data.c_str());
    }

    clear();

    return res;
}

void SciCurl::clear()
{
    data.clear();
}

int SciCurl::write_result(char* pcInput, size_t size, size_t nmemb, void* output)
{
#ifdef _MSC_VER
    if(useFile)
    {
        FILE* fd = (FILE*)output;
        fwrite(pcInput, size, nmemb, fd);
        return static_cast<int>(size*nmemb);
    }
#endif

    std::string d(pcInput, size * nmemb);
    data += d;
    return static_cast<int>(size*nmemb);
}

// Proxy is configured in scilab preferences (internet tab)
int SciCurl::setProxy(CURL* curl)
{
    char* proxyUserPwd = NULL;
    const char* attrs[] = {"enabled", "host", "port", "user", "password"};
    const unsigned int N = sizeof(attrs) / sizeof(char*);
    char** values = getPrefAttributesValues("//web/body/proxy", attrs, N);

    if (values == NULL)
    {
        // no proxy configured
        return 0;
    }

    // proxy is configured and not enabled
    if (stricmp(values[0]/*enabled*/, "false") == 0)
    {
        freeArrayOfString(values, N);
        return 0;
    }

    const unsigned int host_len = (const unsigned int)strlen(values[1]);
    const unsigned int port_len = (const unsigned int)strlen(values[2]);
    const unsigned int user_len = (const unsigned int)strlen(values[3]);
    const unsigned int pwd_len  = (const unsigned int)strlen(values[4]);

    if(host_len == 0 || port_len == 0 || user_len == 0)
    {
        freeArrayOfString(values, N);
        return 1;
    }

    if (pwd_len == 0)
    {
        proxyUserPwd = values[3]; //user
    }
    else
    {
        proxyUserPwd = (char *)MALLOC((user_len + 1 + pwd_len + 1) * sizeof(char));
        sprintf(proxyUserPwd, "%s:%s", values[3]/*user*/, values[4]/*password*/);
        proxyUserPwd[user_len + 1 + pwd_len] = '\0';
    }

    // set cURL options
    if(curl_easy_setopt(curl, CURLOPT_PROXY, values[1]) != CURLE_OK) //host
    {
        FREE(proxyUserPwd);
        freeArrayOfString(values, N);
        return 1;
    }

    if(curl_easy_setopt(curl, CURLOPT_PROXYPORT, strtol(values[2], NULL, 10)) != CURLE_OK) //port
    {
        FREE(proxyUserPwd);
        freeArrayOfString(values, N);
        return 1;
    }

    if(curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, proxyUserPwd) != CURLE_OK) //port
    {
        FREE(proxyUserPwd);
        freeArrayOfString(values, N);
        return 1;
    }

    FREE(proxyUserPwd);
    freeArrayOfString(values, N);
    return 0;
}
