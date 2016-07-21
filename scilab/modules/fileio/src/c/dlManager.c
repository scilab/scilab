/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012-2013 - S/E - Sylvestre LEDRU
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

#ifndef _MSC_VER
#define _GNU_SOURCE             /* basename crashes this extension otherwise */
#endif
#include "sci_malloc.h"
#include <curl/curl.h>
#include <libxml/uri.h>
#include <string.h>
#include "dlManager.h"
#include "Scierror.h"
#include "sci_home.h"
#include "PATH_MAX.h"
#include "isdir.h"
#include "charEncoding.h"
#include "localization.h"
#include "getos.h"
#include "machine.h"
#include "scicurdir.h"
#include "splitpath.h"
#include "getScilabPreference.h"
#include "os_string.h"
#include "freeArrayOfString.h"
/* ==================================================================== */
#ifndef HAVE_BASENAME
static char *Curl_basename(char *path);
#define basename(x)  Curl_basename((x))
#endif
/* ==================================================================== */
static char errorBuffer[CURL_ERROR_SIZE];
/* ==================================================================== */
static int getProxyValues(char **proxyHost, long *proxyPort, char **proxyUserPwd);
/* ==================================================================== */
typedef struct __INPUTSTRING__
{
    char *ptr;
    size_t len;
} inputString;
/* ==================================================================== */
static void init_string(inputString *s)
{
    s->len = 0;
    s->ptr = (char*)CALLOC(s->len + 1, sizeof(char));
    if (s->ptr == NULL)
    {
        Scierror(999, "Internal error: calloc() failed.\n");
        return;
    }
}
/* ==================================================================== */
static void free_string(inputString *s)
{
    if (s->ptr)
    {
        FREE(s->ptr);
        s->ptr = NULL;
    }
}
/* ==================================================================== */
static size_t writefunc(void *ptr, size_t size, size_t nmemb, inputString *s)
{
    size_t new_len = s->len + size * nmemb;

    s->ptr = (char*)REALLOC(s->ptr, new_len + 1);
    if (s->ptr == NULL)
    {
        Scierror(999, "Internal error: realloc() failed.\n");
        return 0;
    }
    memcpy(s->ptr + s->len, ptr, size * nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;

    return size * nmemb;
}
/* ==================================================================== */
static char *getFileNameFromURL(char *url)
{
    char *filename = NULL;
    xmlURIPtr c = xmlParseURI(url);

    if (c == NULL)
    {
        Scierror(999, _("Could not parse the URL.\n"));
        return NULL;
    }

    if (c->path == NULL || strstr(c->path, "/") == 0 || strcmp(c->path, "/") == 0)
    {
        filename = (char *)MALLOC((strlen(DEFAULT_FILENAME) + 1) * sizeof(char));
        strcpy(filename, DEFAULT_FILENAME);
    }
    else
    {
        char bname[PATH_MAX] = {0};
        strncpy(bname, basename(c->path), sizeof(bname));
        filename = (char *)MALLOC((strlen(bname) + 1) * sizeof(char));
        strcpy(filename, bname);
    }
    return filename;

}
/* ==================================================================== */
int getProxyValues(char **proxyHost, long *proxyPort, char **proxyUserPwd)
{
    const char * attrs[] = {"enabled", "host", "port", "user", "password"};
    const unsigned int N = sizeof(attrs) / sizeof(char*);
    char ** values = getPrefAttributesValues("//web/body/proxy", attrs, N);

    if (!values)
    {
        return 0;
    }

    if (stricmp(values[0]/*enabled*/, "true") == 0)
    {
        const unsigned int ulen = (const unsigned int)strlen(values[3]);
        const unsigned int plen = (const unsigned int)strlen(values[4]);

        *proxyHost = values[1]; //host;
        *proxyPort = strtol(values[2], NULL, 10); //port;
        FREE(values[2]);

        if (plen == 0)
        {
            *proxyUserPwd = values[3]; //user
        }
        else
        {
            *proxyUserPwd = (char *)MALLOC((ulen + 1 + plen + 1) * sizeof(char));
            sprintf(*proxyUserPwd, "%s:%s", values[3]/*user*/, values[4]/*password*/);
            (*proxyUserPwd)[ulen + 1 + plen] = '\0';
            FREE(values[3]);
        }

        FREE(values[4]);
        FREE(values[0]);
        FREE(values);
    }
    else
    {
        freeArrayOfString(values, N);
        return 0;
    }

    return 1;
}
/* ==================================================================== */
char *downloadFile(char *url, char *dest, char *username, char *password, char **content)
{
    CURL *curl;
    CURLcode res;
    char *filename = NULL;
    FILE *file;
    inputString buffer;
    char *destdir = NULL;
    char *destfile = NULL;

    curl = curl_easy_init();
    if (curl == NULL)
    {
        Scierror(999, "Failed opening the curl handle.\n");
        return NULL;
    }

    res = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
    if (res != CURLE_OK)
    {
        Scierror(999, "Failed to set error buffer [%d]\n", res);
        return NULL;
    }

    // Get destination directory and filename
    if (dest != NULL)
    {
        // Destination is specified in argument
        char* pathdrive = (char*)MALLOC(sizeof(char) * (PATH_MAX + 1));
        char* pathdir = (char*)MALLOC(sizeof(char) * (PATH_MAX + 1));
        char* pathfile = (char*)MALLOC(sizeof(char) * (PATH_MAX + 1));
        char* pathext = (char*)MALLOC(sizeof(char) * (PATH_MAX + 1));

        splitpath(dest, TRUE, pathdrive, pathdir, pathfile, pathext);

        if (!isdir(dest))
        {
            // Destination is a file
            destdir = (char *)MALLOC((strlen(pathdrive) + strlen(pathdir) + 1) * sizeof(char));
            strcpy(destdir, pathdrive);
            strcat(destdir, pathdir);

            // Get filename
            destfile = (char *)MALLOC((strlen(pathfile) + strlen(pathext) + 1) * sizeof(char));
            strcpy(destfile, pathfile);
            strcat(destfile, pathext);
        }
        else
        {
            // Destination is a directory
            destdir = (char *)MALLOC((strlen(pathdrive) + strlen(pathdir) + strlen(pathfile) + strlen(pathext) + strlen(DIR_SEPARATOR) + 1) * sizeof(char));
            strcpy(destdir, pathdrive);
            strcat(destdir, pathdir);
            strcat(destdir, pathfile);
            strcat(destdir, pathext);
            strcat(destdir, DIR_SEPARATOR);

            // Retrieve filename from URL
            destfile = getFileNameFromURL(url);
        }

        FREE(pathdrive);
        FREE(pathdir);
        FREE(pathfile);
        FREE(pathext);
    }
    else
    {
        // Destination is not specified in argument
        // Destination directory is current dir
        int err = 0;
        char *currentdir;
        currentdir = scigetcwd(&err);
        if (!err)
        {
            destdir = (char *)MALLOC((strlen(currentdir) + strlen(DIR_SEPARATOR) + 1) * sizeof(char));
            strcpy(destdir, currentdir);
            strcat(destdir, DIR_SEPARATOR);
            FREE(currentdir);
        }
        else
        {
            Scierror(999, _("Failed getting current dir, error code: %d\n"), err);
            return NULL;
        }

        // Destination filename retrieved from URL
        destfile = getFileNameFromURL(url);
    }

    if (destfile == NULL)
    {
        FREE(destdir);
        return NULL;
    }

    // Build file path
    filename = (char *)MALLOC((strlen(destdir) + strlen(destfile) + 1) * sizeof(char));
    strcpy(filename, destdir);
    strcat(filename, destfile);
    FREE(destdir);
    FREE(destfile);

    res = curl_easy_setopt(curl, CURLOPT_URL, url);
    if (res != CURLE_OK)
    {
        Scierror(999, _("Failed to set URL [%s]\n"), errorBuffer);
        FREE(filename);
        return NULL;
    }

    //Set authentication variables
    if (username != NULL)
    {
        char * userpass;
        int uplen = (int)strlen(username);
        if (password != NULL)
        {
            uplen = uplen + (int)strlen(password);
        }

        res = curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
        if (res != CURLE_OK)
        {
            Scierror(999, "Failed to set httpauth type to ANY [%s]\n", errorBuffer);
            FREE(filename);
            return NULL;
        }

        userpass = (char *)MALLOC((uplen + 2) * sizeof(char));
        strcpy(userpass, username);
        strcat(userpass, ":");
        if (password != NULL)
        {
            strcat(userpass, password);
        }

        res = curl_easy_setopt(curl, CURLOPT_USERPWD, userpass);
        if (res != CURLE_OK)
        {
            Scierror(999, _("Failed to set user:pwd [%s]\n"), errorBuffer);
            FREE(filename);
            return NULL;
        }

        FREE(userpass);
    } /* end authentication section */

    {
        //Set proxy variables
        char *proxyHost = NULL;
        char *proxyUserPwd = NULL;
        long proxyPort = 1080;
        int proxySet = 0;

        proxySet = getProxyValues(&proxyHost, &proxyPort, &proxyUserPwd);

        if (proxySet == 1)
        {
            res = curl_easy_setopt(curl, CURLOPT_PROXY, proxyHost);
            if (res != CURLE_OK)
            {
                Scierror(999, _("Failed to set proxy host [%s]\n"), errorBuffer);
                FREE(proxyHost);
                FREE(proxyUserPwd);
                FREE(filename);
                return NULL;
            }

            res = curl_easy_setopt(curl, CURLOPT_PROXYPORT, proxyPort);
            if (res != CURLE_OK)
            {
                Scierror(999, _("Failed to set proxy port [%s]\n"), errorBuffer);
                FREE(proxyHost);
                FREE(proxyUserPwd);
                FREE(filename);
                return NULL;
            }
            if (proxyUserPwd != NULL)
            {
                res = curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, proxyUserPwd);
                if (res != CURLE_OK)
                {
                    Scierror(999, _("Failed to set proxy user:password [%s]\n"), errorBuffer);
                    FREE(proxyHost);
                    FREE(proxyUserPwd);
                    FREE(filename);
                    return NULL;
                }
            }

            FREE(proxyHost);
            FREE(proxyUserPwd);
        }
    } /* end of the set of the proxy */

    res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    if (res != CURLE_OK)
    {
        Scierror(999, _("Failed to set write function [%s]\n"), errorBuffer);
        FREE(filename);
        return NULL;
    }

    init_string(&buffer);

    //Get data to be written to the variable
    res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    if (res != CURLE_OK)
    {
        Scierror(999, _("Failed to set write data [%s]\n"), errorBuffer);
        FREE(filename);
        free_string(&buffer);
        return NULL;
    }

    // Follow redirects
    res = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    if (res != CURLE_OK)
    {
        Scierror(999, _("Failed to set 'Follow Location' [%s]\n"), errorBuffer);
        FREE(filename);
        free_string(&buffer);
        return NULL;
    }

    res = curl_easy_perform(curl);
    if (res != 0)
    {
        Scierror(999, _("Transfer did not complete successfully: %s\n"), errorBuffer);
        FREE(filename);
        free_string(&buffer);
        return NULL;
    }

    wcfopen(file, (char*)filename, "wb");
    if (file == NULL)
    {
        Scierror(999, _("Failed opening '%s' for writing.\n"), filename);
        free_string(&buffer);
        FREE(filename);
        return NULL;
    }

    /* Write the file */
    fwrite(buffer.ptr, sizeof(char), buffer.len, file);

    /* Create the variable which contains the output argument */
    *content = buffer.ptr;

    /* always cleanup */
    curl_easy_cleanup(curl);

    fclose(file);
    return filename;
}
/* ==================================================================== */
static char *Curl_basename(char *path)
{
    char *s1 = NULL;
    char *s2 = NULL;

    s1 = strrchr(path, '/');
    s2 = strrchr(path, '\\');

    if (s1 && s2)
    {
        path = (s1 > s2 ? s1 : s2) + 1;
    }
    else if (s1)
    {
        path = s1 + 1;
    }
    else if (s2)
    {
        path = s2 + 1;
    }
    return path;
}
/* ==================================================================== */
