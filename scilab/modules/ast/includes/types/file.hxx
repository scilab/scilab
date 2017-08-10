/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
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

#ifndef __FILE_HH__
#define __FILE_HH__

#include "arrayof.hxx"

namespace types
{
class EXTERN_AST File
{
public :
    File();
    ~File();

    void                        setFileMode(const std::wstring& _pstMode);
    std::wstring&               getFileMode();
    void                        setFileModeAsInt(int _iMode);
    int                         getFileModeAsInt();
    void                        setFileFortranMode(int _iMode);
    int                         getFileFortranMode();

    void                        setFileDesc(FILE* _fileDesc);
    FILE*                       getFiledesc();

    void                        setFileSwap(int _iSwap);
    int                         getFileSwap();

    void                        setFileType(int _iType);
    int                         getFileType();
    std::wstring                getFileTypeAsString();

    void                        setFilename(const std::wstring& _stFilename);
    std::wstring&               getFilename();

    int                         getCountLines();

    bool                        isFile()
    {
        return true;
    }
private :
    FILE*                       m_fileDesc;
    int                         m_iSwap; /* swap status for each file */
    std::wstring                m_pstMode; /* mode for each file */
    int                         m_iFortranMode; /* mode for file opened with fortran open*/
    int                         m_iType; /* type (Fortran,C) for each file must be zero initialized */
    std::wstring                m_stFilename; /* name for each file */

};
}

#endif /* __FILE_HH__ */
