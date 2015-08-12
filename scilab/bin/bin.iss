;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2008 - DIGITEO - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
;

; library blas standard
Source: bin\blasplus.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\blasplus.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;

#ifndef SCILAB_X64
Source: bin\iconv.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\msvcr71.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
#endif

;Visual C++ 2012 redist libraries
Source: bin\msvcp110.DLL; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\msvcr110.DLL; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\vccorlib110.DLL; DestDir: {app}\bin; Components: {#COMPN_SCILAB}

;Visual C++ 2010 redist libraries
Source: bin\msvcr100.DLL; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\msvcp100.DLL; DestDir: {app}\bin; Components: {#COMPN_SCILAB}


;MKL
Source: bin\LIBMMD.DLL; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libifcoremd.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libifcorertd.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libiomp5md.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\svml_dispmd.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
; used for localization
Source: bin\libintl.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libintl.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\GNU_gettext.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\MALLOC.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: bin\zlib1.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\GetWindowsVersion.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\lapack.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libf2c.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\scilab_windows.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libxml2.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libcurl.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\pcre.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\pcreposix.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: bin\libf2c.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\lapack.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: bin\scilab_windows.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: bin\Scilex.exe; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\WScilex.exe; DestDir: {app}\bin; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: bin\Dumpexts.exe; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\f2c.exe; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
; hdf5 libraries
Source: bin\szip.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\zlib.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\hdf5dll.dll ; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\hdf5_hldll.dll ; DestDir: {app}\bin; Components: {#COMPN_SCILAB}

;
; batch files
Source: bin\scinotes.bat; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\xcos.bat; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
