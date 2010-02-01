;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2008 - DIGITEO - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
;

; library blas standard
Source: bin\blasplus.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\blasplus.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
#ifndef SCILAB_X64
; JRE x86 requires this files
Source: bin\msvcr71.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
#endif
;
#ifdef SCILAB_X64
;used by libifcoremd
Source: bin\msvcrt.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: bin\msvcr80.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\msvcm80.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\msvcp80.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\Microsoft.VC80.CRT.manifest; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
#endif
;
Source: bin\msvcr90.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\msvcm90.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\msvcp90.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\Microsoft.VC90.CRT.manifest; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: bin\LIBMMD.DLL; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libifcoremd.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libifcorertd.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: bin\dynamiclibrary.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: bin\libst.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}

; used for localization
Source: bin\libiconv.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libintl.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libintl.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\GNU_gettext.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
#ifndef SCILAB_X64
Source: bin\iconv.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
#endif
;
Source: bin\zlib1.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\DetectFramework.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\GetWindowsVersion.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\hashtable.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\doublylinkedlist.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\lapack.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libf2c.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libjvm.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\LibScilab.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\scilab_windows.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libxml2.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\MALLOC.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\time.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\intersci.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\pcre.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\pcreposix.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: bin\MALLOC.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libf2c.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\lapack.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\LibScilab.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\intersci.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\scilab_windows.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\hashtable.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: bin\Scilex.exe; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\WScilex.exe; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\Dumpexts.exe; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\f2c.exe; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
; hdf5 libraries
Source: bin\jhdf5.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\szlibdll.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\hdf5dll.dll ; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\hdf5_cppdll.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
