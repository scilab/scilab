/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012-2013 - S/E - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef _MSC_VER
#define _GNU_SOURCE             /* basename crashes this extension otherwise */
#endif
#include <curl/curl.h>
#include <libxml/uri.h>
#include <string.h>
#include "dlManager.h"
#include "Scierror.h"
#include "SCIHOME.h"
#include "getos.h"
#include "PATH_MAX.h"
#include "MALLOC.h"
#include "isdir.h"
#include "charEncoding.h"
#include "localization.h"
#include "getos.h"
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
struct inputString
{
    char *ptr;
    size_t len;
};
/* ==================================================================== */
static void init_string(struct inputString *s)
{
    s->len = 0;
    s->ptr = (char*)CALLOC(s->len + 1, sizeof(char));
    if (s->ptr == NULL)
    {
        Scierror(999, "Internal error: calloc() failed.\n");
        return;
    }
    s->ptr[0] = '\0';
}
/* ==================================================================== */
static size_t writefunc(void *ptr, size_t size, size_t nmemb, struct inputString *s)
{
    size_t new_len = s->len + size * nmemb;

    s->ptr = (char*)REALLOC(s->ptr, new_len + 1);
    if (s->ptr == NULL)
    {
        Scierror(999, "Internal error: realloc() failed.\n");
        return NULL;
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

    if (c->path == NULL || strcmp(c->path, "/") == 0)
    {
        filename = (char *)MALLOC((strlen(DEFAULT_FILENAME) + 1) * sizeof(char));
        strcpy(filename, DEFAULT_FILENAME);
    }
    else
    {
        char bname[PATH_MAX];

        if (c->path == NULL)
        {
            Scierror(43, "Internal error: c->path is null ?!\n");
        }
        strcpy(bname, basename(c->path));
        filename = (char *)MALLOC((strlen(bname) + 1) * sizeof(char));
        strcpy(filename, bname);
    }
    return filename;

}
/* ==================================================================== */
int getProxyValues(char **proxyHost, long *proxyPort, char **proxyUserPwd)
{
    FILE * pFile;
    long lSize;
    char * buffer;
    size_t result;

    char *configPtr;

    char *host, *user, *password, *userpwd;
    long port;
    int useproxy;

    char *tp, *field, *value, *eqptr;
    int eqpos = 0 , tplen;

    //construct ATOMS config file path
    configPtr = (char *)MALLOC(PATH_MAX * sizeof(char));
    strcpy(configPtr, getSCIHOME());

    if (strcmp(getOSFullName(), "Windows") == 0)
    {
        if (strstr(getOSRelease(), "x64") != NULL)
        {
            strcat(configPtr, "/.atoms/x64/config");
        }
        else
        {
            strcat(configPtr, "/.atoms/config");
        }
    }
    else
    {
        strcat(configPtr, "/.atoms/config");
    }


    wcfopen (pFile, configPtr , "rb" );
    if (pFile == NULL)
    {
        // No configuration file. Leave
        FREE(configPtr);
        return 0;
    }

    fseek (pFile , 0 , SEEK_END);
    lSize = ftell(pFile);
    if (lSize < 0)
    {
        Scierror(999, _("Could not read the configuration file '%s'.\n"), configPtr);
        FREE(configPtr);
        return 0;
    }

    rewind (pFile);

    // allocate memory to contain the whole file
    buffer = (char*)MALLOC((lSize + 1) * sizeof(char));
    if (buffer == NULL)
    {
        FREE(pFile);
        return 0;
    }
    buffer[lSize] = '\0';

    // copy the file into the buffer
    result = fread (buffer, 1, lSize, pFile);
    if (result != lSize)
    {
        Scierror(999, _("Failed to read the scicurl_config file '%s'.\n"), configPtr);
        return 0;
    }

    host = user = password = userpwd = NULL;
    useproxy = 0;

    tp = field = value = eqptr = NULL;


    // parse each line to extract variables
    tp = strtok(buffer, "\n");
    while (tp != NULL)
    {

        eqptr = strrchr(tp, '=');
        tplen = (int)strlen(tp);
        if (eqptr == NULL)
        {
            Scierror(999, _("Improper syntax of scicurl_config file ('%s'), '=' not found %d:%s\n"), configPtr, tplen, tp);
            return 0;
        }
        eqpos = (int)(eqptr - tp);
        if (tplen <= eqpos + 1)
        {
            Scierror(999, _("Improper syntax of scicurl_config file ('%s'), after an '='\n"), configPtr);
            return 0;
        }
        if (tp[eqpos - 1] != ' ' || tp[eqpos + 1] != ' ')
        {
            Scierror(999, _("Improper syntax of scicurl_config file ('%s'), space before and after '=' expected\n"), configPtr);
            return 0;
        }

        //get field and value from each line
        field = (char *)MALLOC(sizeof(char) * (eqpos));
        value = (char *)MALLOC(sizeof(char) * (strlen(tp) - eqpos - 1));

        memcpy(field, tp, eqpos - 1);
        field[eqpos - 1] = '\0';

        memcpy(value, tp + eqpos + 2, strlen(tp) - eqpos - 2);
        value[strlen(tp) - eqpos - 2] = '\0';


        //check and read proxy variables
        if (strcmp(field, "useProxy") == 0)
        {
            if (strcmp(value, "False") == 0)
            {
                FREE(field);
                FREE(value);
                FREE(buffer);
                return 0;
            }
            if (strcmp(value, "True") == 0)
            {
                useproxy = 1;
            }
        }
        else if (strcmp(field, "proxyHost") == 0)
        {
            host = (char *)MALLOC((strlen(value) + 1) * sizeof(char));
            strcpy(host, value);
        }
        else if (strcmp(field, "proxyPort") == 0)
        {
            port = strtol(value, NULL, 10);
        }
        else if (strcmp(field, "proxyUser") == 0)
        {
            user = (char *)MALLOC((strlen(value) + 1) * sizeof(char));
            strcpy(user, value);
        }
        else if (strcmp(field, "proxyPassword") == 0)
        {
            password = (char *)MALLOC((strlen(value) + 1) * sizeof(char));
            strcpy(password, value);
        }

        free(field);
        free(value);

        tp = strtok(NULL, "\n");
    }

    // if proxy is set, update the parameters
    if (useproxy == 1)
    {

        // proxyUserPwd = "user:password"
        int userlen, passlen;
        userlen = passlen = 0;
        if (user != NULL)
        {
            userlen = (int)strlen(user);
        }
        if (password != NULL)
        {
            passlen = (int)strlen(user);
        }
        if (userlen + passlen != 0)
        {
            userpwd = (char *)MALLOC((userlen + passlen + 2) * sizeof(char));
            strcpy(userpwd, user);
            strcat(userpwd, ":");
            if (password != NULL)
            {
                strcat(userpwd, password);
            }
        }

        *proxyHost = host;
        *proxyPort = port;
        *proxyUserPwd = userpwd;

    }

    fclose(pFile);
    free(buffer);
    return useproxy;
}
/* ==================================================================== */
char *downloadFile(char *url, char *dest, char *username, char *password, char **content)
{
    CURL *curl;
    CURLcode res;
    char *filename = NULL;

    curl = curl_easy_init();

    if (curl)
    {
        FILE *file;

        struct inputString buffer;

        init_string(&buffer);

        res = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
        if (res != CURLE_OK)
        {
            Scierror(999, "Failed to set error buffer [%d]\n", res);
            return NULL;
        }

        if (dest == NULL)
        {
            /* No second argument provided */
            filename = getFileNameFromURL(url);
        }
        else
        {
            if (isdir(dest))
            {
                /* The target is a directory. Select the name from the URL */
                char *name = getFileNameFromURL(url);

                filename = (char *)MALLOC((strlen(name) + strlen("/") + strlen(dest) + 1) * sizeof(char));
                strcpy(filename, dest);
                strcat(filename, "/");
                strcat(filename, name);
                FREE(name);
            }
            else
            {
                filename = (char *)MALLOC((strlen(dest) + 1) * sizeof(char));
                strcpy(filename, dest);
            }
        }

        wcfopen(file, (char*)filename, "wb");

        if (file == NULL)
        {
            Scierror(999, _("Failed opening '%s' for writing.\n"), filename);
            FREE(filename);
            return NULL;
        }

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

            userpass = (char *)MALLOC((uplen + 2) * sizeof(char));

            strcpy(userpass, username);
            strcat(userpass, ":");
            if (password != NULL)
            {
                strcat(userpass, password);
            }

            res = curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
            if (res != CURLE_OK)
            {
                FREE(userpass);
                Scierror(999, "Failed to set httpauth type to ANY [%s]\n", errorBuffer);
                return NULL;
            }

            res = curl_easy_setopt(curl, CURLOPT_USERPWD, userpass);
            if (res != CURLE_OK)
            {
                Scierror(999, _("Failed to set user:pwd [%s]\n"), errorBuffer);
                return NULL;
            }
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
                    return NULL;
                }

                res = curl_easy_setopt(curl, CURLOPT_PROXYPORT, proxyPort);
                if (res != CURLE_OK)
                {
                    Scierror(999, _("Failed to set proxy port [%s]\n"), errorBuffer);
                    return NULL;
                }
                if (proxyUserPwd != NULL)
                {
                    res = curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, proxyUserPwd);
                    if (res != CURLE_OK)
                    {
                        Scierror(999, _("Failed to set proxy user:password [%s]\n"), errorBuffer);
                        return NULL;
                    }
                }

            }
        } /* end of the set of the proxy */

        res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);

        if (res != CURLE_OK)
        {
            Scierror(999, _("Failed to set write function [%s]\n"), errorBuffer);
            return NULL;
        }

        //Get data to be written to the variable
        res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        if (res != CURLE_OK)
        {
            Scierror(999, _("Failed to set write data [%s]\n"), errorBuffer);
            return NULL;
        }

        // Follow redirects
        res = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        if (res != CURLE_OK)
        {
            Scierror(999, _("Failed to set 'Follow Location' [%s]\n"), errorBuffer);
            return NULL;
        }

        res = curl_easy_perform(curl);

        if (res != 0)
        {
            Scierror(999, _("Transfer did not complete successfully: %s\n"), errorBuffer);
            fclose(file);
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
    else
    {
        Scierror(999, "Failed opening the curl handle.\n");
        return NULL;
    }
    return NULL;
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
