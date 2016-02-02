/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* Copyright (C) DIGITEO - 2010 - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#define _WIN32_WINNT 0x0501
#define _WIN32_IE 0x0501
#include <windows.h>
#include <wininet.h>
#include <urlmon.h>
#include "sci_malloc.h"
#include "httpdownloadfile.h"
/* http://msdn2.microsoft.com/en-us/library/aa385098.aspx */
/* http://msdn.microsoft.com/en-us/library/ms775123(VS.85).aspx */
/*--------------------------------------------------------------------------*/
#define MO 0x100000 /* Read 1 Mo by 1Mo. */
#define OPENURL_MODE INTERNET_OPEN_TYPE_PRECONFIG
/*--------------------------------------------------------------------------*/
static HINSTANCE WinINETDll = NULL;
static HINSTANCE UrlmonDll = NULL;

typedef HRESULT (WINAPI * URLDownloadToFilePROC)(LPUNKNOWN pCaller,
        LPCTSTR szURL,
        LPCTSTR szFileName,
        DWORD dwReserved,
        LPBINDSTATUSCALLBACK lpfnCB);

static HRESULT dynlib_URLDownloadToFile(LPUNKNOWN pCaller,
                                        LPCTSTR szURL,
                                        LPCTSTR szFileName,
                                        DWORD dwReserved,
                                        LPBINDSTATUSCALLBACK lpfnCB);

static httpdownloadfile_error_code urlDownloadFile(char * szURL, char * szSaveFilePath);
/*--------------------------------------------------------------------------*/
typedef  HINTERNET (WINAPI * InternetOpenUrlPROC) (HINTERNET hInternet,
        LPCTSTR lpszUrl,
        LPCTSTR lpszHeaders,
        DWORD dwHeadersLength,
        DWORD dwFlags,
        DWORD_PTR dwContext);

static HINTERNET dynlib_InternetOpenUrl(HINTERNET hInternet,
                                        LPCTSTR lpszUrl,
                                        LPCTSTR lpszHeaders,
                                        DWORD dwHeadersLength,
                                        DWORD dwFlags,
                                        DWORD_PTR dwContext);
/*--------------------------------------------------------------------------*/
typedef  HINTERNET (WINAPI * InternetOpenPROC) (LPCTSTR lpszAgent,
        DWORD dwAccessType,
        LPCTSTR lpszProxyName,
        LPCTSTR lpszProxyBypass,
        DWORD dwFlags);

static HINTERNET dynlib_InternetOpen(LPCTSTR lpszAgent,
                                     DWORD dwAccessType,
                                     LPCTSTR lpszProxyName,
                                     LPCTSTR lpszProxyBypass,
                                     DWORD dwFlags);
/*--------------------------------------------------------------------------*/
typedef BOOL (WINAPI * InternetCloseHandlePROC) (HINTERNET hInternet);

static BOOL dynlib_InternetCloseHandle(HINTERNET hInternet);
/*--------------------------------------------------------------------------*/
typedef BOOL (WINAPI * HttpQueryInfoPROC) (HINTERNET hRequest,
        DWORD dwInfoLevel,
        LPVOID lpvBuffer,
        LPDWORD lpdwBufferLength,
        LPDWORD lpdwIndex);

static BOOL dynlib_HttpQueryInfo(HINTERNET hRequest,
                                 DWORD dwInfoLevel,
                                 LPVOID lpvBuffer,
                                 LPDWORD lpdwBufferLength,
                                 LPDWORD lpdwIndex);
/*--------------------------------------------------------------------------*/
typedef BOOL (WINAPI * InternetReadFilePROC) (HINTERNET hFile,
        LPVOID lpBuffer,
        DWORD dwNumberOfBytesToRead,
        LPDWORD lpdwNumberOfBytesRead);

static BOOL dynlib_InternetReadFile(HINTERNET hFile,
                                    LPVOID lpBuffer,
                                    DWORD dwNumberOfBytesToRead,
                                    LPDWORD lpdwNumberOfBytesRead);
/*--------------------------------------------------------------------------*/
void httpdownload(char * szURL, char * szSaveFilePath, double *ierr)
{
    *ierr = urlDownloadFile(szURL, szSaveFilePath);
    if (*ierr != HTTP_DOWNLOAD_ERROR_OK)
    {
        // fails to download by standard way
        // we try by another method
        // last chance ...
        *ierr = httpDownloadFile(szURL, szSaveFilePath);
    }

    if (WinINETDll)
    {
        FreeLibrary(WinINETDll);
        WinINETDll = NULL;
    }

    if (UrlmonDll)
    {
        FreeLibrary(UrlmonDll);
        UrlmonDll = NULL;
    }
}
/*--------------------------------------------------------------------------*/
httpdownloadfile_error_code httpDownloadFile(char * szURL, char * szSaveFilePath)
{
    HINTERNET hiConnex = NULL;
    /* * / * : /*rfc 2616 protocole http.  all files type accepted*/
    char szHeader[] = "Accept: */*\r\n\r\n";
    HINTERNET hiDownload;

    hiConnex = dynlib_InternetOpen("Scilab_Download", OPENURL_MODE, NULL, NULL, 0);
    if (hiConnex == NULL)
    {
        return HTTP_DOWNLOAD_ERROR_INTERNET_OPEN;
    }

    hiDownload = dynlib_InternetOpenUrl(hiConnex, szURL, szHeader, lstrlen(szHeader), INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_RELOAD | INTERNET_FLAG_PRAGMA_NOCACHE, 0);
    if (!hiDownload)
    {
        dynlib_InternetCloseHandle(hiConnex);
        return HTTP_DOWNLOAD_ERROR_OPEN_URL;
    }
    else
    {
        DWORD dwStatus = 0;
        DWORD dwStatusSize = sizeof(dwStatus);
        DWORD dwIndex = 0;

        dynlib_HttpQueryInfo(hiDownload, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwStatus, &dwStatusSize, &dwIndex);

        if (dwStatus != HTTP_STATUS_OK)
        {
            dynlib_InternetCloseHandle(hiConnex);
            dynlib_InternetCloseHandle(hiDownload);
            return HTTP_DOWNLOAD_ERROR_INVALID_URL;
        }
        else
        {
            HANDLE haFile;

            haFile = CreateFile(szSaveFilePath, GENERIC_WRITE, FILE_SHARE_WRITE, 0, CREATE_ALWAYS, 0, 0);
            if (haFile == INVALID_HANDLE_VALUE)
            {
                dynlib_InternetCloseHandle(hiConnex);
                return HTTP_DOWNLOAD_ERROR_CREATEFILE;
            }
            else
            {
                char *szBuff = NULL;

                DWORD dwBytesRequired = 0;
                DWORD dwSizeOfByReq = 4;
                DWORD dwBytesRead = 0;
                DWORD dwBytesWritten = 0;

                /* Get file size */
                if (!dynlib_HttpQueryInfo(hiDownload, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, (LPVOID)&dwBytesRequired, &dwSizeOfByReq, 0))
                {
                    dynlib_InternetCloseHandle(hiConnex);
                    return HTTP_DOWNLOAD_ERROR_INVALID_FILE_SIZE;
                }
                else
                {
                    if (dwBytesRequired > MO)
                    {
                        szBuff = (char*)MALLOC(MO);
                        if (szBuff == NULL)
                        {
                            CloseHandle(haFile);
                            dynlib_InternetCloseHandle(hiConnex);
                            return HTTP_DOWNLOAD_OUTOFMEMORY;
                        }
                    }
                    else
                    {
                        szBuff = (char*)MALLOC(dwBytesRequired);
                        if (szBuff == NULL)
                        {
                            CloseHandle(haFile);
                            dynlib_InternetCloseHandle(hiConnex);
                            return HTTP_DOWNLOAD_OUTOFMEMORY;
                        }
                    }

                    while (dwBytesRequired > 0)
                    {
                        /* we read 1Mo from file. */
                        if (dwBytesRequired >= MO)
                        {
                            if (!dynlib_InternetReadFile(hiDownload, szBuff, MO, &dwBytesRead) || dwBytesRead != MO)
                            {
                                CloseHandle(haFile);
                                dynlib_InternetCloseHandle(hiConnex);
                                FREE(szBuff);
                                return HTTP_DOWNLOAD_ERROR_INTERNET_READFILE;
                            }
                            dwBytesRequired -= MO;


                            /* we write buffer */
                            if (!WriteFile(haFile, szBuff, MO, &dwBytesWritten, NULL) || dwBytesWritten != MO)
                            {
                                CloseHandle(haFile);
                                dynlib_InternetCloseHandle(hiConnex);
                                FREE(szBuff);
                                return HTTP_DOWNLOAD_ERROR_WRITEFILE;
                            }
                        }
                        else
                        {
                            if (!dynlib_InternetReadFile(hiDownload, szBuff, dwBytesRequired, &dwBytesRead) || dwBytesRead != dwBytesRequired)
                            {
                                CloseHandle(haFile);
                                dynlib_InternetCloseHandle(hiConnex);
                                FREE(szBuff);
                                return HTTP_DOWNLOAD_ERROR_INTERNET_READFILE;
                            }

                            /* we write buffer in a backup file*/
                            if (!WriteFile(haFile, szBuff, dwBytesRequired, &dwBytesWritten, NULL) || dwBytesWritten != dwBytesRequired)
                            {
                                CloseHandle(haFile);
                                dynlib_InternetCloseHandle(hiConnex);
                                FREE(szBuff);
                                return HTTP_DOWNLOAD_ERROR_WRITEFILE;
                            }

                            dwBytesRequired = 0;
                        }
                    }

                    dynlib_InternetCloseHandle(hiConnex);
                    CloseHandle(haFile);
                    FREE(szBuff);
                    return HTTP_DOWNLOAD_ERROR_OK;
                }
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
httpdownloadfile_error_code urlDownloadFile(char * szURL, char * szSaveFilePath)
{
    HRESULT hr = dynlib_URLDownloadToFile(NULL, szURL, szSaveFilePath, 0, NULL);
    switch (hr)
    {
        case S_OK:
        {
            return HTTP_DOWNLOAD_ERROR_OK;
        }
        break;

        case E_OUTOFMEMORY:
        {
            return HTTP_DOWNLOAD_OUTOFMEMORY;
        }
        break;

        case INET_E_DOWNLOAD_FAILURE:
        default:
        {
            return HTTP_DOWNLOAD_FAILURE;
        }
        break;
    }
}
/*--------------------------------------------------------------------------*/
static HRESULT dynlib_URLDownloadToFile(LPUNKNOWN pCaller,
                                        LPCTSTR szURL,
                                        LPCTSTR szFileName,
                                        DWORD dwReserved,
                                        LPBINDSTATUSCALLBACK lpfnCB)
{
    if (UrlmonDll == NULL)
    {
        UrlmonDll = LoadLibrary ("urlmon.dll");
    }
    if (UrlmonDll)
    {
        URLDownloadToFilePROC dllURLDownloadToFile = (URLDownloadToFilePROC)GetProcAddress(UrlmonDll, "URLDownloadToFileA");
        if (dllURLDownloadToFile)
        {
            return (HRESULT)(dllURLDownloadToFile)(pCaller, szURL, szFileName, dwReserved, lpfnCB);
        }
    }
    return S_FALSE;
}
/*--------------------------------------------------------------------------*/
HINTERNET dynlib_InternetOpenUrl(HINTERNET hInternet,
                                 LPCTSTR lpszUrl,
                                 LPCTSTR lpszHeaders,
                                 DWORD dwHeadersLength,
                                 DWORD dwFlags,
                                 DWORD_PTR dwContext)
{
    if (WinINETDll == NULL)
    {
        WinINETDll = LoadLibrary ("WININET.dll");
    }
    if (WinINETDll)
    {
        InternetOpenUrlPROC dllInternetOpenUrl = (InternetOpenUrlPROC)GetProcAddress(WinINETDll, "InternetOpenUrlA");
        if (dllInternetOpenUrl)
        {
            return (HINTERNET)(dllInternetOpenUrl)(hInternet,
                                                   lpszUrl,
                                                   lpszHeaders,
                                                   dwHeadersLength,
                                                   dwFlags,
                                                   dwContext);
        }
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/
HINTERNET dynlib_InternetOpen(LPCTSTR lpszAgent,
                              DWORD dwAccessType,
                              LPCTSTR lpszProxyName,
                              LPCTSTR lpszProxyBypass,
                              DWORD dwFlags)
{
    if (WinINETDll == NULL)
    {
        WinINETDll = LoadLibrary ("WININET.dll");
    }
    if (WinINETDll)
    {
        InternetOpenPROC dllInternetOpen = (InternetOpenPROC)GetProcAddress(WinINETDll, "InternetOpenA");
        if (dllInternetOpen)
        {
            return (HINTERNET)(dllInternetOpen)(lpszAgent,
                                                dwAccessType,
                                                lpszProxyName,
                                                lpszProxyBypass,
                                                dwFlags);
        }
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/
BOOL dynlib_InternetCloseHandle(HINTERNET hInternet)
{
    if (WinINETDll == NULL)
    {
        WinINETDll = LoadLibrary ("WININET.dll");
    }
    if (WinINETDll)
    {
        InternetCloseHandlePROC dllInternetCloseHandle = (InternetCloseHandlePROC)
                GetProcAddress(WinINETDll, "InternetCloseHandle");

        if (dllInternetCloseHandle)
        {
            return (BOOL)(dllInternetCloseHandle)(hInternet);
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL dynlib_HttpQueryInfo(HINTERNET hRequest,
                          DWORD dwInfoLevel,
                          LPVOID lpvBuffer,
                          LPDWORD lpdwBufferLength,
                          LPDWORD lpdwIndex)
{
    if (WinINETDll == NULL)
    {
        WinINETDll = LoadLibrary ("WININET.dll");
    }
    if (WinINETDll)
    {
        HttpQueryInfoPROC dllHttpQueryInfo = (HttpQueryInfoPROC)
                                             GetProcAddress(WinINETDll, "HttpQueryInfoA");
        if (dllHttpQueryInfo)
        {
            return (BOOL)(dllHttpQueryInfo)(hRequest,
                                            dwInfoLevel,
                                            lpvBuffer,
                                            lpdwBufferLength,
                                            lpdwIndex);
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL dynlib_InternetReadFile(HINTERNET hFile,
                             LPVOID lpBuffer,
                             DWORD dwNumberOfBytesToRead,
                             LPDWORD lpdwNumberOfBytesRead)
{
    if (WinINETDll == NULL)
    {
        WinINETDll = LoadLibrary ("WININET.dll");
    }
    if (WinINETDll)
    {
        InternetReadFilePROC dllInternetReadFile = (InternetReadFilePROC)
                GetProcAddress(WinINETDll, "InternetReadFile");
        if (dllInternetReadFile)
        {
            return (BOOL)(dllInternetReadFile)(hFile,
                                               lpBuffer,
                                               dwNumberOfBytesToRead,
                                               lpdwNumberOfBytesRead);
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
