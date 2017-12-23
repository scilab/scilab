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
#include "struct.hxx"
#include "sciCurl.hxx"
#include "json.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"
#include "sci_malloc.h"
#include "getFullFilename.h"
}

/*--------------------------------------------------------------------------*/
static const char fname[] = "http_upload";
types::Function::ReturnValue sci_http_upload(types::typed_list &in, types::optional_list &opt, int _iRetCount, types::typed_list &out)
{
    SciCurl* sciCurlObj = SciCurl::getInstance();
    CURLcode res = CURLE_OK;

    if (in.size() < 3 || in.size() > 4)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), fname, 3, 4);
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

    // get input file name
    if(in[1]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix string expected.\n"), fname, 2);
        return types::Function::Error;
    }

    types::String* pStrFiles = in[1]->getAs<types::String>();
    std::vector<char*> files;
    for(int i = 0; i < pStrFiles->getSize(); i++)
    {
        wchar_t* pwcFileName = getFullFilenameW(pStrFiles->get(i));
        files.push_back(wide_string_to_UTF8(pwcFileName));
        FREE(pwcFileName);
    }

    // get variable name server side
    if(in[2]->isString() == false && in[2]->getAs<types::String>()->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar string expected.\n"), fname, 3);
        return types::Function::Error;
    }

    char* pcVarName = wide_string_to_UTF8(in[2]->getAs<types::String>()->get(0));

    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr  = NULL;

    // /!\ This doesn't work with nodejs multer array /!\
    // struct curl_forms form[files.size()+1];
    // int iter = 0;
    // for(auto f : files)
    // {
    //     form[iter].option  = CURLFORM_FILE;
    //     form[iter++].value = f;
    // }
    //
    // form[iter].option = CURLFORM_END;
    //
    // curl_formadd(&formpost,
    //              &lastptr,
    //              CURLFORM_COPYNAME, pcVarName,
    //              CURLFORM_ARRAY, form,
    //              CURLFORM_END);

    for(auto f : files)
    {
        curl_formadd(&formpost,
                     &lastptr,
                     CURLFORM_COPYNAME, pcVarName,
                     CURLFORM_FILE, f,
                     CURLFORM_END);
    }

    FREE(pcVarName);

    if(in.size() > 3)
    {
        // get data
        if(in[3]->isStruct() == false && in[3]->getAs<types::Struct>()->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A structure of size %d expected.\n"), fname, 3, 1);
            return types::Function::Error;
        }

        types::Struct* pStruct = in[3]->getAs<types::Struct>();
        std::unordered_map<std::wstring, int> fieldsMap = pStruct->get(0)->getFields();
        std::vector<types::InternalType*> pITData = pStruct->get(0)->getData();
        for (const auto & field : fieldsMap)
        {
            char* pcFieldName = wide_string_to_UTF8(field.first.data());
            std::string strData(toJSON(pITData[field.second]));
            //remove " @start and @end of string
            if (strData.front() == '\"' && strData.back() == '\"')
            {
                strData = strData.substr(1);
                strData.pop_back();
            }
            sciprint("|%s|: |%s|\n", pcFieldName, strData.data());
            curl_formadd(&formpost,
                         &lastptr,
                         CURLFORM_COPYNAME, pcFieldName,
                         CURLFORM_COPYCONTENTS, strData.data(),
                         CURLFORM_END);

            FREE(pcFieldName);
        }
    }

    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

    // set proxy information
    if(sciCurlObj->setProxy(curl))
    {
        Scierror(999, _("%s: Wrong proxy information, please check in the 'internet' Scilab preference.\n"), fname);
        return types::Function::Error;
    }

    // common optional argument
    if(checkCommonOpt((void*)curl, opt, fname))
    {
        return types::Function::Error;
    }

    // specific optional argument
    for (const auto& o : opt)
    {
        if (o.first == L"method")
        {
            if(o.second->isString() == false && o.second->getAs<types::String>()->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%s: A scalar string expected.\n"), fname, o.first.data());
                return types::Function::Error;
            }

            wchar_t* pMeth = o.second->getAs<types::String>()->get(0);
            if(wcscmp(pMeth, L"PUT") == 0)
            {
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
            }
            else if(wcscmp(pMeth, L"POST") != 0)
            {
                Scierror(999, _("%s: Wrong value for input argument #%s: 'PUT' or 'POST' expected.\n"), fname, o.first.data());
                return types::Function::Error;
            }
        }
    }

    sciCurlObj->getResultAsObject(curl);
    res = curl_easy_perform(curl);
    for(auto f : files)
    {
        FREE(f);
    }

    curl_formfree(formpost);

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
