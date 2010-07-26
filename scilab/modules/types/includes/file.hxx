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

#ifndef __FILE_HH__
#define __FILE_HH__

#include "double.hxx"

namespace types
{
    class File
    {
    public :
                                    File();
                                    ~File();

        void                        setFileMode(wstring _pstMode);
        wstring&                    getFileMode();

        void                        setFileDesc(FILE* _fileDesc);
        FILE*                       getFiledesc();

        void                        setFileSwap(int _iSwap);
        int                         getFileSwap();

        void                        setFileType(int _iType);
        int                         getFileType();

        void                        setFilename(wstring _stFilename);
        wstring                     getFilename();

    private :
        FILE*                       m_fileDesc;
        int                         m_iSwap; /* swap status for each file */
        wstring                     m_pstMode; /* mode for each file */
        int                         m_iType; /* type (Fortran,C) for each file must be zero initialized */
        wstring                     m_stFilename; /* name for each file */

    };
}

#endif /* __FILE_HH__ */
