/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
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

#ifndef __FILEMANAGER_HXX__
#define __FILEMANAGER_HXX__

#include <list>
#include <string>
#include "dynlib_fileio.h"

#include "file.hxx"

class FILEIO_IMPEXP FileManager
{
    //static attributes instantiation
public :
    /*commom attributes and function for all File*/
    static int                  getFileMaxID();
    static bool                 isOpened(const std::wstring& _stFilename);
    static int                  getFileID(const std::wstring& _stFilename);
    static types::File*         getFile(int _iID);
    static int                  getCurrentFile();
    static int                  addFile(types::File* _file);
    static int                  getFirstFreeFileID();
    static void                 deleteFile(int _iID);
    static int                  getOpenedCount();
    static wchar_t**            getTypesAsString();
    static wchar_t**            getFilenames();
    static double*              getModes();
    static double*              getSwaps();
    static int*                 getIDs();

    static void                 initialize();
    static void                 destroy();
private :
    typedef std::vector<types::File*>  vectFile;
    static vectFile             m_fileList;
    static int                  m_iCurrentFile;     //memorize current using file
};

#endif /* __FILEMANAGER_HXX__ */
