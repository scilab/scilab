/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
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
#include <windows.h>
#include "machine.h" /* SHARED_LIB_EXT */
#include "JVM_commons.h"
#include "JVM_functions.h"
#include "sci_malloc.h"
#include "PATH_MAX.h"
#include "FileExist.h"
#include "GetWindowsVersion.h"
/*--------------------------------------------------------------------------*/
#ifdef _WIN64
/* Sun doesn't distribute "client" version of jvm with jre for x64 version */
#define JVM_TYPE "server"
#else
#define JVM_TYPE "client"
#endif
/* #define JVM_TYPE "server" */
/* Every form of Sun's Java runtime comes with both the "client VM" and the "server VM."
Unfortunately, Java applications and applets run by default in the client VM.
The Server VM is much faster than the Client VM,
but it has the downside of taking around 10% longer to start up, and it uses more memory.
*/
/*--------------------------------------------------------------------------*/
static char *Search_Java_RuntimeLib_in_Windows_Registry(void);
static JavaVM *SearchCreatedJavaVMPath(void);
static JavaVM *SearchCreatedJavaVMEmbedded(char *SCILAB_PATH);
static JavaVM *SearchCreatedJavaVMRegistry(void);
/*--------------------------------------------------------------------------*/
static BOOL EMBEDDED_JRE = FALSE;
/*--------------------------------------------------------------------------*/
BOOL LoadDynLibJVM(char *SCILAB_PATH)
{
    /* 1] search in SCI/java/jre */
    /* 2] search in windows registry */
    /* 3] search in PATH */
    /* else ERROR Java not found */

    BOOL bOK = FALSE;
    char *JVMDLLFULLNAME = NULL;

    /* 1] search in scilab SCI/java/jre */
    JVMDLLFULLNAME = (char*)MALLOC( (strlen(SCILAB_PATH) + strlen(JRE_PATH) + strlen("/bin/") + strlen(JVM_TYPE) + strlen("/jvm") + strlen(SHARED_LIB_EXT) + 1) * sizeof(char));
    sprintf(JVMDLLFULLNAME, "%s%s%s%s%s%s", SCILAB_PATH, JRE_PATH, "/bin/", JVM_TYPE, "/jvm", SHARED_LIB_EXT);

    if (!LoadFunctionsJVM(JVMDLLFULLNAME))
    {
        /* 2] search in windows registry */
        /* We try to find JRE on Windows registry*/
        if (JVMDLLFULLNAME)
        {
            FREE(JVMDLLFULLNAME);
            JVMDLLFULLNAME = NULL;
        };
        JVMDLLFULLNAME = Search_Java_RuntimeLib_in_Windows_Registry();

        /* 3] search in PATH */
        if (!LoadFunctionsJVM(JVMDLLFULLNAME))
        {
            if (JVMDLLFULLNAME)
            {
                FREE(JVMDLLFULLNAME);
                JVMDLLFULLNAME = NULL;
            };
            JVMDLLFULLNAME = (char*)MALLOC( (strlen("jvm") + strlen(SHARED_LIB_EXT) + 1) * sizeof(char));
            sprintf(JVMDLLFULLNAME, "%s%s%s%s", SCILAB_PATH, JRE_PATH, "jvm", SHARED_LIB_EXT);
            if ( LoadFunctionsJVM(JVMDLLFULLNAME) )
            {
                bOK = TRUE;
            }
        }
        else
        {
            bOK = TRUE;
        }
    }
    else
    {
        EMBEDDED_JRE = TRUE;
        bOK = TRUE;
    }

    if (JVMDLLFULLNAME)
    {
        FREE(JVMDLLFULLNAME);
        JVMDLLFULLNAME = NULL;
    };

    return bOK;
}
/*--------------------------------------------------------------------------*/
char *Search_Java_RuntimeLib_in_Windows_Registry(void)
{
#define JRE_HKEY "Software\\JavaSoft\\Java Runtime Environment"
    char *RuntimeLib = NULL;
    char value[PATH_MAX];
    char newKey[PATH_MAX];

    HKEY  regKey;
    DWORD size  = PATH_MAX;
    int   result;

    DWORD OpensKeyOptions = 0;

    /* if Win64 search only 64 bits JRE version */
#ifdef _WIN64 /* Scilab x64 on x64 windows */
    OpensKeyOptions = KEY_READ | KEY_WOW64_64KEY;
#else
    if (IsWow64()) /* Scilab 32 bits on x64 windows */
    {
        OpensKeyOptions = KEY_READ | KEY_WOW64_32KEY;
    }
    else /* Scilab 32 bits on windows 32 bits */
    {
        OpensKeyOptions = KEY_READ;
    }
#endif

    if ((result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, JRE_HKEY, 0, OpensKeyOptions, &regKey)) != ERROR_SUCCESS)
    {
        return NULL;
    }

    if ((result = RegQueryValueEx(regKey, "CurrentVersion", NULL, NULL, (LPBYTE)value, &size)) != ERROR_SUCCESS)
    {
        RegCloseKey(regKey);
        return NULL;
    }

    RegCloseKey(regKey);
    value[size] = '\0';
    size = PATH_MAX;

    strcpy(newKey, JRE_HKEY);
    strcat(newKey, "\\");
    strcat(newKey, value);


    if ((result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, newKey, 0, OpensKeyOptions, &regKey)) != ERROR_SUCCESS)
    {
        return NULL;
    }

    if ((result = RegQueryValueEx(regKey, "RuntimeLib", NULL, NULL, (LPBYTE)value, &size)) != ERROR_SUCCESS)
    {
        RegCloseKey(regKey);
        return NULL;
    }

    RegCloseKey(regKey);

    /* check file Exist */
    if (FileExist(value))
    {
        RuntimeLib = (char*)MALLOC(sizeof(char) * (strlen(value) + 1));
        strcpy(RuntimeLib, value);
    }

    return RuntimeLib;
}
/*--------------------------------------------------------------------------*/
BOOL withEmbeddedJRE(void)
{
    return EMBEDDED_JRE;
}
/*--------------------------------------------------------------------------*/
static JavaVM *SearchCreatedJavaVMEmbedded(char *SCILAB_PATH)
{
    JavaVM *jvm = NULL;
    jsize jvm_count = 0;
    jint res = 0;
    char *JVMDLLFULLNAME = NULL;

    /* search in SCI/java/jre */
    JVMDLLFULLNAME = (char*)MALLOC( (strlen(SCILAB_PATH) + strlen(JRE_PATH) + strlen("/bin/") + strlen(JVM_TYPE) + strlen("/jvm") + strlen(SHARED_LIB_EXT) + 1) * sizeof(char));
    sprintf(JVMDLLFULLNAME, "%s%s%s%s%s%s", SCILAB_PATH, JRE_PATH, "/bin/", JVM_TYPE, "/jvm", SHARED_LIB_EXT);

    FreeDynLibJVM();

    if (LoadFunctionsJVM(JVMDLLFULLNAME))
    {
        res = SciJNI_GetCreatedJavaVMs (&jvm, 1, &jvm_count);

        if ( jvm_count == 1 )
        {
            if (JVMDLLFULLNAME)
            {
                FREE(JVMDLLFULLNAME);
                JVMDLLFULLNAME = NULL;
            }
            return jvm;
        }
        else
        {
            jvm = NULL;
        }
    }
    if (JVMDLLFULLNAME)
    {
        FREE(JVMDLLFULLNAME);
        JVMDLLFULLNAME = NULL;
    }
    return jvm;
}
/*----------------------------------------------------------------------------------*/
JavaVM *SearchCreatedJavaVMRegistry(void)
{
    JavaVM *jvm = NULL;
    jsize jvm_count = 0;
    jint res = 0;
    char *JVMDLLFULLNAME = NULL;

    JVMDLLFULLNAME = Search_Java_RuntimeLib_in_Windows_Registry();
    FreeDynLibJVM();

    if (LoadFunctionsJVM(JVMDLLFULLNAME))
    {
        res = SciJNI_GetCreatedJavaVMs (&jvm, 1, &jvm_count);

        if ( jvm_count == 1 )
        {
            if (JVMDLLFULLNAME)
            {
                FREE(JVMDLLFULLNAME);
                JVMDLLFULLNAME = NULL;
            }
            return jvm;
        }
        else
        {
            jvm = NULL;
        }
    }
    if (JVMDLLFULLNAME)
    {
        FREE(JVMDLLFULLNAME);
        JVMDLLFULLNAME = NULL;
    }
    return jvm;
}
/*----------------------------------------------------------------------------------*/
static JavaVM *SearchCreatedJavaVMPath(void)
{
    JavaVM *jvm = NULL;
    jsize jvm_count = 0;
    jint res = 0;
    char *JVMDLLFULLNAME = NULL;

    FreeDynLibJVM();

    JVMDLLFULLNAME = (char*)MALLOC( (strlen("jvm") + strlen(SHARED_LIB_EXT) + 1) * sizeof(char));
    sprintf(JVMDLLFULLNAME, "%s%s", "jvm", SHARED_LIB_EXT);

    if (LoadFunctionsJVM(JVMDLLFULLNAME))
    {
        res = SciJNI_GetCreatedJavaVMs (&jvm, 1, &jvm_count);
        if ( jvm_count == 1 )
        {
            if (JVMDLLFULLNAME)
            {
                FREE(JVMDLLFULLNAME);
                JVMDLLFULLNAME = NULL;
            };
            return jvm;
        }
        else
        {
            jvm = NULL;
        }
    }
    return jvm;
}
/*----------------------------------------------------------------------------------*/
JavaVM *FindCreatedJavaVM(char *SCILAB_PATH)
{
    JavaVM *jvm = NULL;

    jvm = SearchCreatedJavaVMEmbedded(SCILAB_PATH);
    if (jvm)
    {
        return jvm;
    }
    else
    {
        jvm = SearchCreatedJavaVMRegistry();
        if (jvm)
        {
            return jvm;
        }
        else
        {
            jvm = SearchCreatedJavaVMPath();
            if (jvm)
            {
                return jvm;
            }
        }
    }
    return NULL;
}
/*--------------------------------------------------------------------------*/
