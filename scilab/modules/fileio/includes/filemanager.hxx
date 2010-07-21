/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __FILEMANAGER_HXX__
#define __FILEMANAGER_HXX__

#include <list>
#include <string>
#include "dynlib_fileio.h"

#include "file.hxx"

using namespace std;
using namespace types;

class FILEIO_IMPEXP FileManager
{
//static attributs instantiation
public :
    /*commom attributs and function for all File*/
    static int                  getFileMaxID();
    static bool                 isOpened(wstring _stFilename);
    static int                  getFileID(wstring _stFilename);
    static File*                getFile(int _iID);
    static int                  getCurrentFile();
    static int                  addFile(File* _file);
    static void                 deleteFile(int _iID);

private :
    static std::vector<File*>   m_fileList;
    static int                  m_iCurrentFile;     //memorize current using file

};

#endif /* __FILEMANAGER_HXX__ */
