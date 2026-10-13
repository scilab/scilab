/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
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
    SciCurl();
    ~SciCurl();

    bool init();

    void setURL(const char* url);
    void setMethod(const char* method);
    void setData(const char* data);
    bool setProxy();
    bool setCookies();
    void setCustomCookies(const char* pcCookies);
    int  setTimeOut(double second);

    void ssl(bool verifyPeer);
    void follow(int follow);
    void auth(const char* auth);
    void verbose(bool verbose, const char* fname);

    void perform(FILE* fd = nullptr);
    bool hasFailed();
    const char* getError();
    long getResponseCode();

    void addFileToForm(const std::string& name, const std::string& file, const std::string& filename);
    void addContentToForm(const char* name, const char* data);
    void setForm();

    void addHTTPHeader(const char* pcHeader);
    void setHTTPHeader();

    types::InternalType* getResult(void);
    types::InternalType* getHeaders(void);
    FILE* getFile();
    void appendData(std::string& part);
    void appendHeaders(std::string& field,std::string& value);

    static size_t write_result(char* pcInput, size_t size, size_t nmemb, void* output);
    static size_t write_headers(char* pcInput, size_t size, size_t nmemb, void* output);
    static size_t debug_callback(CURL* handle, curl_infotype type, char* data, size_t size, void* clientp);

private:
    CURL* _curl;
    CURLcode _status;

    std::string _data;
    std::vector<std::pair<std::string, std::string>> _recvHeaders;
    FILE* _fd;
    bool _follow;
    struct curl_slist* _headers;
    curl_mime* _form;
};

#endif /* !__SCICURL_HXX__ */
