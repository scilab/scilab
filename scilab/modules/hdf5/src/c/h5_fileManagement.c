/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010 - DIGITEO - Allan CORNET
*  Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
*  
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <hdf5.h>
#include <string.h>
#include "h5_fileManagement.h"
#include "FileExist.h"
#include "deleteafile.h"
#include "isdir.h"
#include "splitpath.h"
#include "scicurdir.h"
#include "MALLOC.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
static char *getPathFilename(char *fullfilename);
static char *getFilenameWithExtension(char *fullfilename);
/*--------------------------------------------------------------------------*/
int createHDF5File(char *name)
{
    hid_t       file;
    hid_t fapl = H5Pcreate(H5P_FILE_ACCESS);
    char *pathdest = getPathFilename(name);
    char *currentpath = NULL;
    char *filename = getFilenameWithExtension(name);
    int ierr = 0;

    //H5Pset_fclose_degree(fapl, H5F_CLOSE_STRONG);

    /* TO DO : remove when HDF5 will be fixed ... */
    /* HDF5 does not manage no ANSI characters */
    /* UGLY workaround :( */
    /* We split path, move in this path, open file */
    /* and return in previous place */
    /* see BUG 6440 */
    currentpath = scigetcwd(&ierr);

    //prevent error msg to change directory to ""
    if (strcmp(pathdest, "") != 0)
    {
        scichdir(pathdest);
    }

    FREE(pathdest);
    /*bug 5629 : to prevent replace directory by file*/
    if (isdir(filename))
    {
        FREE(filename);
        FREE(currentpath);
        return -2;
    }

    if (FileExist(filename))
    {
        deleteafile(filename);
    }
    /*
    * Create a new file using the default properties.
    */

    file = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, fapl);

    scichdir(currentpath);

    FREE(currentpath);
    FREE(filename);

    return file;
}
/*--------------------------------------------------------------------------*/
int openHDF5File(char *name, int _iAppendMode)
{
    hid_t           file;
    char *pathdest = getPathFilename(name);
    char *currentpath = NULL;
    char *filename = getFilenameWithExtension(name);
    int ierr = 0;
    void *oldclientdata = NULL;
    /* Used to avoid stack trace to be displayed */
    H5E_auto2_t oldfunc;

    /* TO DO : remove when HDF5 will be fixed ... */
    /* HDF5 does not manage no ANSI characters */
    /* UGLY workaround :( */
    /* We split path, move in this path, open file */
    /* and return in previous place */
    /* see BUG 6440 */
    currentpath = scigetcwd(&ierr);

    //prevent error msg to change directory to ""
    if (strcmp(pathdest, "") != 0)
    {
        scichdir(pathdest);
    }

    /* Save old error handler */
    H5Eget_auto2(H5E_DEFAULT, &oldfunc, &oldclientdata);

    /* Turn off error handling */
    H5Eset_auto2(H5E_DEFAULT, NULL, NULL);

    if(_iAppendMode == 0)
    {//read only
        file = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
    }
    else
    {//read write to append
        file = H5Fopen(filename, H5F_ACC_RDWR, H5P_DEFAULT);
    }
    /* The following test will display the backtrace in case of error */
    /* Deactivated because displayed each time we call 'load' to open a non-HDF5 file */
    /*if (file < 0)
    {
        H5Eprint(stderr);
        }*/
    /* Restore previous error handler */
    H5Eset_auto2(H5E_DEFAULT, oldfunc, oldclientdata);

    scichdir(currentpath);

    if (currentpath)
    {
        FREE(currentpath);
        currentpath = NULL;
    }
    if (filename)
    {
        FREE(filename);
        filename = NULL;
    }
    if (pathdest)
    {
        FREE(pathdest);
        pathdest = NULL;
    }

    return file;
}
/*--------------------------------------------------------------------------*/
int isHDF5File(char* _pstFilename)
{
    int iRet = 0;
    char *pathdest = getPathFilename(_pstFilename);
    char *currentpath = NULL;
    char *filename = getFilenameWithExtension(_pstFilename);
    int ierr = 0;

    /* TO DO : remove when HDF5 will be fixed ... */
    /* HDF5 does not manage no ANSI characters */
    /* UGLY workaround :( */
    /* We split path, move in this path, open file */
    /* and return in previous place */
    /* see BUG 6440 */
    currentpath = scigetcwd(&ierr);
    {

        //prevent error msg to change directory to ""
        if (strcmp(pathdest, "") != 0)
        {
            scichdir(pathdest);
        }
        FREE(pathdest);

        iRet = H5Fis_hdf5(filename);
        FREE(filename);
    }
    scichdir(currentpath);
    FREE(currentpath);

    return iRet > 0 ? 1 : 0;
}

void closeHDF5File(int file)
{
    herr_t status					= 0;

    //	H5Fflush(file, H5F_SCOPE_GLOBAL);
    status = H5Fclose(file);
    if (status < 0)
    {
        fprintf(stderr, "%s", "failed to close file");
    }
}
/*--------------------------------------------------------------------------*/
static char *getPathFilename(char *fullfilename)
{
    char *path = NULL;
    if (fullfilename)
    {
        char* drv  = strdup(fullfilename);
        char* dir  = strdup(fullfilename);
        char* name = strdup(fullfilename);
        char* ext  = strdup(fullfilename);

        path = strdup(fullfilename);

        if (drv && dir && name && ext && path)
        {
            splitpath(fullfilename, FALSE, drv, dir, name, ext);

            if (strcmp(drv, "") == 0)
            {
                strcpy(path, dir);
            }
            else
            {
                strcpy(path, drv);
                strcat(path, dir);
            }
        }

        if (drv)
        {
            FREE(drv);
            drv = NULL;
        }
        if (dir)
        {
            FREE(dir);
            dir = NULL;
        }
        if (name)
        {
            FREE(name);
            name = NULL;
        }
        if (ext)
        {
            FREE(ext);
            ext = NULL;
        }
    }
    return path;
}
/*--------------------------------------------------------------------------*/
static char *getFilenameWithExtension(char *fullfilename)
{
    char *filename = NULL;
    if (fullfilename)
    {
        char* drv  = strdup(fullfilename);
        char* dir  = strdup(fullfilename);
        char* name = strdup(fullfilename);
        char* ext  = strdup(fullfilename);

        filename = strdup(fullfilename);

        if (drv && dir && name && ext && filename)
        {
            splitpath(fullfilename, FALSE, drv, dir, name, ext);

            if (strcmp(ext, "") == 0)
            {
                strcpy(filename, name);
            }
            else
            {
                strcpy(filename, name);
                strcat(filename, ext);
            }
        }

        if (drv)
        {
            FREE(drv);
            drv = NULL;
        }
        if (dir)
        {
            FREE(dir);
            dir = NULL;
        }
        if (name)
        {
            FREE(name);
            name = NULL;
        }
        if (ext)
        {
            FREE(ext);
            ext = NULL;
        }
    }
    return filename;
}
/*--------------------------------------------------------------------------*/

