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
#include <fstream>
#include "file.hxx"

extern "C"
{
#include "charEncoding.h"
#include "sci_malloc.h"
}

namespace types
{

File::File()
{
    m_fileDesc = NULL;
    m_iSwap = 0;
    m_pstMode = "";
    m_iFortranMode = -1; // see clunit.f
    m_iType = 0; // 1 : fortran open   2 : c open   3 : std::err std::out std::in
    m_stFilename = "";
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

void File::setFileMode(const std::string& _pstMode)
{
    m_pstMode = _pstMode;
}

std::string& File::getFileMode()
{
    return m_pstMode;
}

int File::getFileModeAsInt()
{
    int iMode  = 0;
    int iPlus  = 0;
    int iBin   = 0;

    for (int i = 0 ; i < (int)strlen(m_pstMode.c_str()) ; i++)
    {
        if (m_pstMode[i] == 'r')
        {
            iMode = 1;
        }
        else if (m_pstMode[i] == 'w')
        {
            iMode = 2;
        }
        else if (m_pstMode[i] == 'a')
        {
            iMode = 3;
        }
        else if (m_pstMode[i] == '+')
        {
            iPlus = 1;
        }
        else if (m_pstMode[i] == 'b')
        {
            iBin = 1;
        }
    }

    return 100 * iMode + 10 * iPlus + iBin;
}

void File::setFileModeAsInt(int _iMode)
{
    int iMode  = (int)(_iMode / 100);
    int iPlus  = (int)((_iMode - iMode * 100) / 10);
    int iBin   = _iMode - iMode * 100 - iPlus * 10;

    m_pstMode = "";

    switch (iMode)
    {
        case 2 :
            m_pstMode += "w";
            break;
        case 3 :
            m_pstMode += "a";
            break;
        default:
            m_pstMode += "r"; // default mode "rb"
    }

    if (iPlus)
    {
        m_pstMode += "+";
    }

    if (iBin)
    {
        m_pstMode += "b";
    }
}

void File::setFileFortranMode(int _iMode)
{
    m_iFortranMode = _iMode;
}

int File::getFileFortranMode()
{
    return m_iFortranMode;
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

std::string File::getFileTypeAsString()
{
    switch (getFileType())
    {
        case 1 :
            return "F";
            break;
        case 2 :
            return "C";
            break;
        case 3 :
            return "STD";
            break;
        default :
            return "Error";
            break;
    }
}

void File::setFilename(const std::string& _stFilename)
{
    m_stFilename = _stFilename;
}

std::string& File::getFilename()
{
    return m_stFilename;
}

int File::getCountLines()
{
    std::ifstream in(m_stFilename);
    std::string stLine;
    int iLines = 0;

    while (std::getline(in, stLine))
    {
        iLines++;
    }

    in.close();
    return iLines;
}

}
