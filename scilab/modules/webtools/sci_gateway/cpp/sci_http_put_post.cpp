/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2017 - ESI-Group - Cedric DELAMARRE
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

#include <curl/curl.h>
#include "webtools_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "double.hxx"
#include "json.hxx"
#include "sciCurl.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"
#include "sci_malloc.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_http_put_post(types::typed_list &in, types::optional_list &opt, int _iRetCount, types::typed_list &out, const char* fname)
{
    SciCurl* sciCurlObj = SciCurl::getInstance();
    CURLcode res = CURLE_OK;
    struct curl_slist *headers = NULL;
    bool isJson = false;
    char* pcData = NULL;

    if (in.size() < 1 || in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), fname, 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), fname, 1, 2);
        return types::Function::Error;
    }

    // get URL
    if(in[0]->isString() == false && in[0]->getAs<types::String>()->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar string expected.\n"), fname, 1);
        return types::Function::Error;
    }

    CURL* curl = curl_easy_init();
    if(curl == nullptr)
    {
        Scierror(999, _("%s: CURL initialization failed.\n"), fname);
        return types::Function::Error;
    }

    char* pcURL = wide_string_to_UTF8(in[0]->getAs<types::String>()->get(0));
    curl_easy_setopt(curl, CURLOPT_URL, pcURL);
    FREE(pcURL);

    sciCurlObj->getResultAsObject(curl);
    if(strcmp(fname, "http_put") == 0)
    {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    }
    else if(strcmp(fname, "http_patch") == 0)
    {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
    }
    else
    {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    }

    // common optional argument
    if(checkCommonOpt((void*)curl, opt, fname))
    {
        return types::Function::Error;
    }

    // set proxy information
    if(sciCurlObj->setProxy(curl))
    {
        Scierror(999, _("%s: Wrong proxy information, please check in the 'internet' Scilab preference.\n"), fname);
        return types::Function::Error;
    }

    // specific optional argument
    for (const auto& o : opt)
    {
        if(o.first == L"format")
        {
            if(o.second->isString() == false && o.second->getAs<types::String>()->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%s: A scalar string expected.\n"), fname, o.first.data());
                return types::Function::Error;
            }

            if( (wcscmp(o.second->getAs<types::String>()->get(0), L"JSON") == 0) ||
                (wcscmp(o.second->getAs<types::String>()->get(0), L"json") == 0))
            {
                isJson = true;
            }
        }
    }

    if(in.size() > 1)
    {
        // get data
        if(in[1]->isString() && in[1]->getAs<types::String>()->isScalar())
        {
            pcData = wide_string_to_UTF8(in[1]->getAs<types::String>()->get(0));
        }
        else
        {
            pcData = os_strdup(toJSON(in[1]).c_str());
            isJson = true;
        }

        if(isJson)
        {
            headers = curl_slist_append(headers, "Accept: application/json");
            headers = curl_slist_append(headers, "Content-Type: application/json");
            headers = curl_slist_append(headers, "charsets: utf-8");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        }

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, pcData);
    }

    res = curl_easy_perform(curl);
    if (pcData)
    {
        FREE(pcData);
    }

    if(headers)
    {
        curl_slist_free_all(headers);
    }

    if(res != CURLE_OK)
    {
        Scierror(999, _("%s: CURL execution failed.\n%s\n"), fname, curl_easy_strerror(res));
        sciCurlObj->clear();
        return types::Function::Error;
    }

    out.push_back(sciCurlObj->getResult());

    if(_iRetCount == 2)
    {
        long http_code = 0;
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
        out.push_back(new types::Double((double)http_code));
    }

    curl_easy_cleanup(curl);
    return types::Function::OK;
}
