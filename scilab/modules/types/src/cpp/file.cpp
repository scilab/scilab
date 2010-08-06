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

    File::File()
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

    double File::getFileModeAsDouble()
    {
        double dblMode  = 0;
        double dblPlus  = 0;
        double dblBin   = 0;

        for(int i = 0 ; i < wcslen(m_pstMode.c_str()) ; i++)
        {
            if(m_pstMode[i] == L'r')
            {
                dblMode = 1;
            }
            else if(m_pstMode[i] == L'w')
            {
                dblMode = 2;
            }
            else if(m_pstMode[i] == L'a')
            {
                dblMode = 3;
            }
            else if(m_pstMode[i] == L'+')
            {
                dblPlus = 1;
            }
            else if(m_pstMode[i] == L'b')
            {
                dblBin = 1;
            }
        }

        return 100 * dblMode + 10 * dblPlus + dblBin;
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

    wstring File::getFileTypeAsString()
    {
        switch(getFileType())
        {
        case 1 : 
            return L"F";
            break;
        case 2 : 
            return L"C";
            break;
        default :
            return L"Error";
            break;
        }
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
