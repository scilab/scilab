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
#include "sciCurl.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"
#include "sci_malloc.h"
#include "getFullFilename.h"
}
/*--------------------------------------------------------------------------*/
static const char fname[] = "http_get";
types::Function::ReturnValue sci_http_get(types::typed_list &in, types::optional_list &opt, int _iRetCount, types::typed_list &out)
{
    SciCurl* sciCurlObj = SciCurl::getInstance();
    CURLcode res = CURLE_OK;
    FILE* fd = NULL;
    types::InternalType* pOut = NULL;

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
    if(in[0]->isString() == false || in[0]->getAs<types::String>()->isScalar() == false)
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

    if(in.size() == 2)
    {
        // get output file name
        if(in[1]->isString() == false || in[1]->getAs<types::String>()->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar string expected.\n"), fname, 2);
            return types::Function::Error;
        }

        wchar_t* pwcFileName = getFullFilenameW(in[1]->getAs<types::String>()->get(0));
        char* pcFileName = wide_string_to_UTF8(pwcFileName);

        fd = fopen(pcFileName, "wb");
        FREE(pcFileName);
        if(fd == NULL)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: The given path does not exist.\n"), fname, 2);
            FREE(pwcFileName);
            return types::Function::Error;
        }

        sciCurlObj->getResultAsFile(curl, fd);

        pOut = new types::String(pwcFileName);
        FREE(pwcFileName);
    }
    else
    {
        sciCurlObj->getResultAsObject(curl);
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

    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
    {
        Scierror(999, _("%s: CURL execution failed.\n%s\n"), fname, curl_easy_strerror(res));
        sciCurlObj->clear();
        if(fd)
        {
            fclose(fd);
        }
        return types::Function::Error;
    }

    if(in.size() == 2)
    {
        fclose(fd);
        sciCurlObj->clear();
    }
    else
    {
        pOut = sciCurlObj->getResult();
    }

    out.push_back(pOut);

    if(_iRetCount == 2)
    {
        long http_code = 0;
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
        out.push_back(new types::Double((double)http_code));
    }

    curl_easy_cleanup(curl);
    return types::Function::OK;
}
