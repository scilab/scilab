/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
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
#include <Windows.h>
#include "writeLog.h"
#include "getCpuInfos.h"
#include "readBlasSpec.h"
#include "copyBlasFile.h"
#include "GetPathOfThisExe.h"
/*--------------------------------------------------------------------------*/
#pragma comment(lib,"../../bin/libxml2.lib")
/*--------------------------------------------------------------------------*/
#define LOG_FILENAME L"SetupAtlas.log"
#define ATLASSPEC_FILENAME L"Atlas.spec"
/*--------------------------------------------------------------------------*/
static wchar_t *LogFilename = NULL;
static wchar_t msgtolog[512];
static wchar_t *buildFilename(wchar_t *destpath, wchar_t *filename);
static wchar_t *getCpuDllFilename(struct cpu_struct**CPUS_SPEC, int sizeCPUS_SPEC);
static BOOL freeCpuStruct(struct cpu_struct**CPUS_SPEC, int sizeCPUS_SPEC);
/*--------------------------------------------------------------------------*/
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    int ierr = 1;
    wchar_t *pathThisExe = GetPathOfThisExe();
    if (pathThisExe)
    {
        LogFilename  = buildFilename(pathThisExe, LOG_FILENAME);
        if (LogFilename)
        {
#ifdef _WIN64
            {
                wchar_t* strtime = getTimeString();
                AppendMessageToLog(strtime, LogFilename);
                free(strtime);
                strtime = NULL;
                AppendMessageToLog(L"**************************************************", LogFilename);
                AppendMessageToLog(L"*            ONLY WITH X86 VERSION               *", LogFilename);
                AppendMessageToLog(L"**************************************************", LogFilename);
                free(LogFilename);
                LogFilename = NULL;
                free(pathThisExe);
                pathThisExe = NULL;
                return ierr;
            }
#else

            wchar_t* AtlasSpecFilename = buildFilename(pathThisExe, ATLASSPEC_FILENAME);

            if (AtlasSpecFilename)
            {
                int  sizeArray = 0;
                struct cpu_struct **CPUS_SPEC = readBlasSpec(AtlasSpecFilename, &sizeArray);

                if (CPUS_SPEC)
                {
                    wchar_t * blasdllname = getCpuDllFilename(CPUS_SPEC, sizeArray);
                    if (blasdllname)
                    {
                        int err = copyBlasFile(blasdllname);

                        wchar_t* strtime = getTimeString();
                        AppendMessageToLog(strtime, LogFilename);
                        free(strtime);
                        strtime = NULL;

                        AppendMessageToLog(L"**************************************************", LogFilename);
                        switch (err)
                        {
                            case COPY_NO_RIGHT_TO_WRITE:
                                wcscpy(msgtolog, L"NO RIGHT TO WRITE: ");
                                wcscat(msgtolog, blasdllname);
                                AppendMessageToLog(msgtolog, LogFilename);
                                ierr = 1;
                                break;
                            case COPY_OK:
                                wcscpy(msgtolog, L"COPY OK OF: ");
                                wcscat(msgtolog, blasdllname);
                                AppendMessageToLog(msgtolog, LogFilename);
                                ierr = 0;
                                break;
                            case COPY_FILE_SRC_NOT_EXISTS:
                                wcscpy(msgtolog, L"FILE NOT EXIST: ");
                                wcscat(msgtolog, blasdllname);
                                AppendMessageToLog(msgtolog, LogFilename);
                                ierr = 1;
                                break;
                            case COPY_DESTINATION_NOT_EXISTS:
                                wcscpy(msgtolog, L"DESTINATION NOT EXIST");
                                AppendMessageToLog(msgtolog, LogFilename);
                                ierr = 1;
                                break;
                            case COPY_FILE_FAILED:
                                wcscpy(msgtolog, L"COPY FAILED");
                                AppendMessageToLog(msgtolog, LogFilename);
                                ierr = 1;
                                break;
                            default:
                                wcscpy(msgtolog, L"UNKNOW ERROR");
                                AppendMessageToLog(msgtolog, LogFilename);
                                ierr = 1;
                                break;
                        }
                        AppendMessageToLog(L"**************************************************", LogFilename);

                        free(blasdllname);
                        blasdllname = NULL;
                    }
                    else
                    {
                        wchar_t* strtime = getTimeString();
                        AppendMessageToLog(strtime, LogFilename);
                        free(strtime);
                        strtime = NULL;

                        AppendMessageToLog(L"**************************************************", LogFilename);
                        wcscpy(msgtolog, L"IMPOSSIBLE TO FIND A OPTIMIZED DLL");
                        AppendMessageToLog(msgtolog, LogFilename);
                        AppendMessageToLog(L"**************************************************", LogFilename);
                        ierr = 1;
                    }

                    freeCpuStruct(CPUS_SPEC, sizeArray);
                }
                else
                {
                    wchar_t* strtime = getTimeString();
                    AppendMessageToLog(strtime, LogFilename);
                    free(strtime);
                    strtime = NULL;

                    AppendMessageToLog(L"**************************************************", LogFilename);
                    wcscpy(msgtolog, L"IMPOSSIBLE TO READ ATLAS.SPEC");
                    AppendMessageToLog(msgtolog, LogFilename);
                    AppendMessageToLog(L"**************************************************", LogFilename);
                    ierr = 1;
                }
                free(AtlasSpecFilename);
                AtlasSpecFilename = NULL;
            }
            else
            {
                wchar_t* strtime = getTimeString();
                AppendMessageToLog(strtime, LogFilename);
                free(strtime);
                strtime = NULL;

                AppendMessageToLog(L"**************************************************", LogFilename);
                wcscpy(msgtolog, L"INCORRECT FILENAME ATLAS.SPEC");
                AppendMessageToLog(msgtolog, LogFilename);
                AppendMessageToLog(L"**************************************************", LogFilename);
                ierr = 1;
            }

            free(LogFilename);
            LogFilename = NULL;
#endif /* _WIN64 */
        }
        else
        {
            wchar_t* strtime = getTimeString();
            AppendMessageToLog(strtime, LogFilename);
            free(strtime);
            strtime = NULL;

            AppendMessageToLog(L"**************************************************", LOG_FILENAME);
            AppendMessageToLog(L"*        ERROR CREATES LOG FILENAME              *", LOG_FILENAME);
            AppendMessageToLog(L"**************************************************", LOG_FILENAME);
            ierr = 1;
        }

        free(pathThisExe);
        pathThisExe = NULL;
    }
    else
    {
        wchar_t* strtime = getTimeString();
        AppendMessageToLog(strtime, LogFilename);
        free(strtime);
        strtime = NULL;

        AppendMessageToLog(L"**************************************************", LOG_FILENAME);
        AppendMessageToLog(L"*        ERROR CURRENT PATH NOT FOUND            *", LOG_FILENAME);
        AppendMessageToLog(L"**************************************************", LOG_FILENAME);
        ierr = 1;
    }

    return ierr;
}
/*--------------------------------------------------------------------------*/
wchar_t *buildFilename(wchar_t *destpath, wchar_t *filename)
{
    wchar_t *fullfilename = NULL;
    if (destpath && filename)
    {
        int len = (int)(wcslen(destpath) + wcslen(filename) + wcslen(L"\\") + 1);
        fullfilename = (wchar_t *) calloc(len, sizeof(wchar_t));
        if (fullfilename)
        {
            wcscpy(fullfilename, destpath);
            wcscat(fullfilename, L"\\");
            wcscat(fullfilename, filename);
        }
    }
    return fullfilename;
}
/*--------------------------------------------------------------------------*/
wchar_t *getCpuDllFilename(struct cpu_struct **CPUS_SPEC, int sizeCPUS_SPEC)
{
    int i = 0;

    wchar_t *CurrentCpuManufacturer = getCpuVendor();
    int CurrentCpuFamily = getCpuFamily();
    int CurrentCpuModel = getCpuModel();

    wchar_t* strtime = getTimeString();
    AppendMessageToLog(strtime, LogFilename);
    free(strtime);
    strtime = NULL;

    wsprintfW(msgtolog, L"Vendor %s Family %d Model %d",
              CurrentCpuManufacturer,
              CurrentCpuFamily,
              CurrentCpuModel);
    AppendMessageToLog(L"**************************************************", LogFilename);
    AppendMessageToLog(L"CPU DETECTION:", LogFilename);
    AppendMessageToLog(msgtolog, LogFilename);
    AppendMessageToLog(L"**************************************************", LogFilename);

    for (i = 0; i < sizeCPUS_SPEC; i++)
    {
        if (wcscmp(CPUS_SPEC[i]->cpu_vendor, CurrentCpuManufacturer) == 0)
        {
            if (CPUS_SPEC[i]->cpu_family == CurrentCpuFamily)
            {
                if (CPUS_SPEC[i]->cpu_model == CurrentCpuModel)
                {
                    return _wcsdup(CPUS_SPEC[i]->dll_filename);
                }
                else
                {
                    if (CPUS_SPEC[i]->cpu_model == 0)
                    {
                        return _wcsdup(CPUS_SPEC[i]->dll_filename);
                    }
                }
            }
        }
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/
static BOOL freeCpuStruct(struct cpu_struct** CPUS_SPEC, int sizeCPUS_SPEC)
{
    int i = 0;
    if (CPUS_SPEC)
    {
        for (i = 0; i < sizeCPUS_SPEC; i++)
        {
            if (CPUS_SPEC[i])
            {
                if (CPUS_SPEC[i]->comments)
                {
                    free(CPUS_SPEC[i]->comments);
                    CPUS_SPEC[i]->comments = NULL;
                }
                if (CPUS_SPEC[i]->cpu_vendor)
                {
                    free(CPUS_SPEC[i]->cpu_vendor);
                    CPUS_SPEC[i]->cpu_vendor = NULL;
                }
                if (CPUS_SPEC[i]->dll_filename)
                {
                    free(CPUS_SPEC[i]->dll_filename);
                    CPUS_SPEC[i]->dll_filename = NULL;
                }
                free(CPUS_SPEC[i]);
                CPUS_SPEC[i] = NULL;
            }
        }
        free(CPUS_SPEC);
        CPUS_SPEC = NULL;
        return TRUE;
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
