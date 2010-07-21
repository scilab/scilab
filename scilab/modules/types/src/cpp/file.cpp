/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
* 
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
* 
*/

#include "file.hxx"

namespace types
{

    File::File() : Double(0)
    {
        m_fileDesc = NULL;
        m_iSwap = 0;
        m_pstMode = L"";
        m_iType = 0;
        m_stFilename = L"";
    }

    File::~File()
    {
    }

    void File::setFileDesc(FILE* _fileDesc)
    {
        m_fileDesc = _fileDesc;
    }

    FILE* File::getFiledesc()
    {
        return m_fileDesc;
    }

    void File::setFileMode(wstring _pstMode)
    {
        m_pstMode = _pstMode;
    }

    wstring& File::getFileMode()
    {
        return m_pstMode;
    }

    void File::setFileSwap(int _iSwap)
    {
        m_iSwap = _iSwap;
    }

    int File::getFileSwap()
    {
        return m_iSwap;
    }

    void File::setFileType(int _iType)
    {
        m_iType = _iType;
    }

    int File::getFileType()
    {
        return m_iType;
    }

    void File::setFilename(wstring _stFilename)
    {
        m_stFilename = _stFilename;
    }

    wstring File::getFilename()
    {
        return m_stFilename;
    }
}
