/*
*  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
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
#include "struct.hxx"
#include "list.hxx"
#include "json.hxx"
#include "configvariable.hxx"

extern "C"
{
    #include "getScilabPreference.h"
    #include "freeArrayOfString.h"
    #include "getos.h"
    #include "getversion.h"
    #include "sciprint.h"
}

SciCurl::SciCurl()
{
    _curl       = nullptr;
    _status     = CURLE_OK;
    _data       = "";
    _fd         = nullptr;
    _follow     = false;
    _headers    = nullptr;
    _form       = nullptr;
}

SciCurl::~SciCurl()
{
    curl_easy_cleanup(_curl);
    _data.clear();
    _recvHeaders.clear();

    if(_headers)
    {
        curl_slist_free_all(_headers);
    }

    if(_form)
    {
        curl_mime_free(_form);
    }
}

bool SciCurl::init()
{
    _curl = curl_easy_init();
    if(_curl == nullptr)
    {
        return false;
    }

    // set some headers
    char* OperatingSystem = getOSFullName();
    char* Release = getOSRelease();

    // Scilab version
    std::string pcUserAgent = "Scilab/" + std::to_string(getScilabVersionMajor()) + "." + std::to_string(getScilabVersionMinor()) + "." + std::to_string(getScilabVersionMaintenance());
    // OS name
    pcUserAgent += " (" + std::string(OperatingSystem) + " " + std::string(Release) + ")";
    // set user agent header
    curl_easy_setopt(_curl, CURLOPT_USERAGENT, pcUserAgent.data());

    // set Accept-Encoding whatever curl was built with
    curl_easy_setopt(_curl, CURLOPT_ACCEPT_ENCODING, "");

    curl_easy_setopt(_curl, CURLOPT_HEADERFUNCTION, write_headers);
    curl_easy_setopt(_curl, CURLOPT_HEADERDATA, this);

    FREE(OperatingSystem);
    FREE(Release);

    return true;
}

void SciCurl::setURL(const char* url)
{
    curl_easy_setopt(_curl, CURLOPT_URL, url);
}

void SciCurl::setMethod(const char* method)
{
    curl_easy_setopt(_curl, CURLOPT_CUSTOMREQUEST, method);
}

void SciCurl::ssl(bool verifyPeer)
{
    curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, verifyPeer);
}

void SciCurl::follow(int follow)
{
    _follow = follow > 0;
    curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, follow);
}

void SciCurl::auth(const char* auth)
{
    curl_easy_setopt(_curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
    curl_easy_setopt(_curl, CURLOPT_USERPWD, auth);
}

void SciCurl::verbose(bool verbose, const char* fname)
{
    if(verbose)
    {
        curl_easy_setopt(_curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(_curl, CURLOPT_DEBUGDATA, fname); 
        curl_easy_setopt(_curl, CURLOPT_DEBUGFUNCTION, SciCurl::debug_callback); 
    }
    else
    {
        curl_easy_setopt(_curl, CURLOPT_VERBOSE, 0L);
    }
}

types::InternalType* SciCurl::getResult()
{
    std::string err = "";
    types::InternalType* res = fromJSON(_data, err);
    if (res == nullptr)
    {
        res = new types::String(_data.c_str());
    }

    return res;
}

types::InternalType* SciCurl::getHeaders()
{
    types::SingleStruct* pSStr = nullptr;
    std::vector<types::Struct*> vectStr;
    for(const auto& p : _recvHeaders)
    {
        if(p.first == "new")
        {
            types::Struct* pStr = new types::Struct(1, 1);
            pSStr = pStr->get(0);
            vectStr.push_back(pStr);
            continue;
        }

        wchar_t* field = to_wide_string(p.first.c_str());
        pSStr->addField(field);
        pSStr->set(field, new types::String(p.second.c_str()));
    }

    // when the follow arg is set to true,
    // returns on group of headers by requests.
    if(_follow)
    {
        types::List* pList = new types::List();
        for(const auto& str : vectStr)
        {
            pList->append(str);
        }

        return pList;
    }

    return vectStr[0];
}

// Proxy is configured in scilab preferences (internet tab)
bool SciCurl::setProxy()
{
    char* proxyUserPwd = NULL;
    const char* attrs[] = {"enabled", "host", "port", "user", "password"};
    const unsigned int N = sizeof(attrs) / sizeof(char*);
    char** values = getPrefAttributesValues("//web/body/proxy", attrs, N);

    if (values == NULL)
    {
        // no proxy configured
        return true;
    }

    // proxy is configured and not enabled
    if (stricmp(values[0]/*enabled*/, "false") == 0)
    {
        freeArrayOfString(values, N);
        return true;
    }

    const unsigned int host_len = (const unsigned int)strlen(values[1]);
    const unsigned int port_len = (const unsigned int)strlen(values[2]);
    const unsigned int user_len = (const unsigned int)strlen(values[3]);
    const unsigned int pwd_len  = (const unsigned int)strlen(values[4]);

    if(host_len == 0)
    {
        freeArrayOfString(values, N);
        return false;
    }

    // set cURL options
    // host
    if(curl_easy_setopt(_curl, CURLOPT_PROXY, values[1]) != CURLE_OK)
    {
        FREE(proxyUserPwd);
        freeArrayOfString(values, N);
        return false;
    }

    // port
    int iPort = port_len ? strtol(values[2], NULL, 10) : 8080;
    if(curl_easy_setopt(_curl, CURLOPT_PROXYPORT, iPort) != CURLE_OK)
    {
        FREE(proxyUserPwd);
        freeArrayOfString(values, N);
        return false;
    }

    // user/password
    if(user_len)
    {
        if(pwd_len == 0)
        {
            proxyUserPwd = values[3]; //user
        }
        else
        {
            proxyUserPwd = (char *)MALLOC((user_len + 1 + pwd_len + 1) * sizeof(char));
            sprintf(proxyUserPwd, "%s:%s", values[3]/*user*/, values[4]/*password*/);
            proxyUserPwd[user_len + 1 + pwd_len] = '\0';
        }

        if(curl_easy_setopt(_curl, CURLOPT_PROXYUSERPWD, proxyUserPwd) != CURLE_OK)
        {
            if(pwd_len)
            {
                FREE(proxyUserPwd);
            }

            freeArrayOfString(values, N);
            return false;
        }

        if(pwd_len)
        {
            FREE(proxyUserPwd);
        }
    }

    freeArrayOfString(values, N);
    return true;
}

// Cookies are configured in scilab preferences (internet tab)
bool SciCurl::setCookies()
{
    const char* attrs[] = {"mode"};
    const unsigned int N = sizeof(attrs) / sizeof(char*);
    char** values = getPrefAttributesValues("//web/body/cookies", attrs, N);

    if (values == NULL)
    {
        // no cookies configured
        return true;
    }

    const unsigned int mode_len = (const unsigned int)strlen(values[0]);
    if(mode_len == 0)
    {
        freeArrayOfString(values, N);
        return false;
    }

    int mode = atoi(values[0]);
    std::wstring path;
    switch(mode)
    {
        case 0: // disabled
        {
            break;
        }
        case 1: // normal
        {
            path = ConfigVariable::getSCIHOME();
        }
        break;
        case 2: // private
        {
            path = ConfigVariable::getTMPDIR();
        }
        break;
        default:
        {
            freeArrayOfString(values, N);
            return false;
        }
    }

    if(mode == 1 || mode == 2)
    {
        // set cookie file
        std::wstring cookie_file = path + DIR_SEPARATORW + L"cookies.txt";
        char* pcCookieFile = wide_string_to_UTF8(cookie_file.data());

        // Add cookies to the query
        CURLcode code = curl_easy_setopt(_curl, CURLOPT_COOKIEFILE, pcCookieFile);
        if(code != CURLE_OK)
        {
            freeArrayOfString(values, N);
            return false;
        }

        // store cookies sent back by the server
        code = curl_easy_setopt(_curl, CURLOPT_COOKIEJAR, pcCookieFile);
        FREE(pcCookieFile);
        if(code != CURLE_OK)
        {
            freeArrayOfString(values, N);
            return false;
        }
    }

    freeArrayOfString(values, N);
    return true;
}

int SciCurl::setTimeOut(double second)
{
    return curl_easy_setopt(_curl, CURLOPT_TIMEOUT_MS, (long)(second * 1000));
}

void SciCurl::addFileToForm(const std::string& name, const std::string& file, const std::string& filename)
{
    if (_form == nullptr)
    {
        _form = curl_mime_init(_curl);
    }

    curl_mimepart* part = curl_mime_addpart(_form);
    curl_mime_name(part, name.c_str());
    curl_mime_filedata(part, file.c_str());
    if (!filename.empty())
    {
        curl_mime_filename(part, filename.c_str());
    }
}

void SciCurl::addContentToForm(const char* name, const char* data)
{
    if (_form == nullptr)
    {
        _form = curl_mime_init(_curl);
    }

    curl_mimepart* part = curl_mime_addpart(_form);
    curl_mime_name(part, name);
    curl_mime_data(part, data, CURL_ZERO_TERMINATED);
}

void SciCurl::setForm()
{
    curl_easy_setopt(_curl, CURLOPT_MIMEPOST, _form);
}

void SciCurl::setCustomCookies(const char* cookies)
{
    // cookies string: "name=value; id=42;"
    curl_easy_setopt(_curl, CURLOPT_COOKIE, cookies);
}

void SciCurl::addHTTPHeader(const char* header)
{
    // header string: "name=value"
    _headers = curl_slist_append(_headers, header);
}

void SciCurl::setHTTPHeader()
{
    if(_headers)
    {
        curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, _headers);
    }
}

void SciCurl::setData(const char* data)
{
    curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, data);
}

bool SciCurl::hasFailed()
{
    return _status != CURLE_OK;
}

const char* SciCurl::getError()
{
    return curl_easy_strerror(_status);
}

FILE* SciCurl::getFile()
{
    return _fd;
}

void SciCurl::appendHeaders(std::string& field, std::string& value)
{
    _recvHeaders.emplace_back(field, value);
}

void SciCurl::appendData(std::string& part)
{
    _data += part;
}

long SciCurl::getResponseCode()
{
    long http_code = 0;
    curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE, &http_code);
    return http_code;
}

void SciCurl::perform(FILE* fd)
{
    // when NOT Windows, let curl write the result in the file.
    #ifndef _MSC_VER
    if(fd)
    {
        curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(_curl, CURLOPT_WRITEDATA, fd);
        _status = curl_easy_perform(_curl);
        return;
    }
    #endif

    // writing in a buffer
    // writing in a file for Windows
    _fd = fd;
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, write_result);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, this);
    _status = curl_easy_perform(_curl);
}


/*** internal ***/
// concat query result
// manage writing in a file for Windows only
// and writing in a buffer for all OS.
size_t SciCurl::write_result(char* pcInput, size_t size, size_t nmemb, void* output)
{
    SciCurl* query = (SciCurl*)output;

#ifdef _MSC_VER
    FILE* fd = query->getFile();
    if(fd)
    {
        fwrite(pcInput, size, nmemb, fd);
        return size*nmemb;
    }
#endif

    std::string d(pcInput, size * nmemb);
    query->appendData(d);
    return size*nmemb;
}

// concat query headers
size_t SciCurl::write_headers(char* pcInput, size_t size, size_t nmemb, void* output)
{
    size_t length = size * nmemb;
    if(length < 3)
    {
        return size*nmemb;
    }

    SciCurl* query = (SciCurl*)output;
    std::string d(pcInput, length);
    std::string::size_type pos = d.find(":");

    if(pos != std::string::npos)
    {
        std::string field = d.substr(0, pos);
        // +2 skip ": "   -2 to remove \r\n
        std::string value = d.substr(pos + 2, length - (pos + 2) - 2);
        query->appendHeaders(field, value);
    }
    else
    {
        // The first header row is for exemple "HTTP/1.1 200 OK"
        // use it to create a new header group.
        // Multiple header group can happend in case of following redirection.
        std::string newHeader("new");
        query->appendHeaders(newHeader, newHeader);
    }

    return size*nmemb;
}

// verbose
size_t SciCurl::debug_callback(CURL* handle, curl_infotype type, char* data, size_t size, void* clientp)
{
    const char* fname = (char*) clientp;

    switch(type)
    {
        case CURLINFO_TEXT:
            //sciprint("%s: %.*s", fname, size, data);
            break;
        case CURLINFO_HEADER_IN:
            sciprint("%s: header in: %.*s", fname, size, data);
            break;
        case CURLINFO_HEADER_OUT:
            sciprint("%s: header out: %.*s", fname, size, data);
            break;
        case CURLINFO_DATA_IN:
            sciprint("%s: data in: %d bytes\n", fname, size);
            break;
        case CURLINFO_DATA_OUT:
            sciprint("%s: data out: %d bytes\n", fname, size);
            break;
        case CURLINFO_SSL_DATA_IN:
            sciprint("%s: SSL data in: %d bytes\n", fname, size);
            break;
        case CURLINFO_SSL_DATA_OUT:
            sciprint("%s: SSL data out: %d bytes\n", fname, size);
            break;
        case CURLINFO_END:
            // this is the end of the stream
            break;
    }

    return 0;
}
