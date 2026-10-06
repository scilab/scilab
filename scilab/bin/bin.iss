;
; Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2008 - DIGITEO - Allan CORNET
;
; Copyright (C) 2012 - 2016 - Scilab Enterprises
;
; This file is hereby licensed under the terms of the GNU GPL v2.0,
; pursuant to article 5.3.4 of the CeCILL v.2.1.
; This file was originally licensed under the terms of the CeCILL v2.1,
; and continues to be available under such terms.
; For more information, see the COPYING file which you should have received
; along with this program.
;

; library blas standard
#if defined(SCILAB_ARM64)
Source: bin\libblas.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
#else
Source: bin\blasplus.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB} and not ({#COMPN_MKL_CPU_LIBRARY} or {#COMPN_FFTW_MKL_LIBRARY})
#endif
Source: bin\blasplus.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;

#if !defined(SCILAB_X64) && !defined(SCILAB_ARM64)
Source: bin\iconv.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\msvcr71.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
#endif

Source: bin\msvcp140.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\msvcp140_1.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\msvcp140_2.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\msvcp140_atomic_wait.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\msvcp140_codecvt_ids.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}

Source: bin\vcruntime140.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\vcruntime140_1.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\vcruntime140_threads.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\vccorlib140.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\concrt140.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}

;Visual C++ 2013 redist libraries
Source: bin\msvcp120.DLL; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\msvcr120.DLL; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\vccorlib120.DLL; DestDir: {app}\bin; Components: {#COMPN_SCILAB}

;Visual C++ 2012 redist libraries
Source: bin\msvcp110.DLL; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\msvcr110.DLL; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\vccorlib110.DLL; DestDir: {app}\bin; Components: {#COMPN_SCILAB}

;Visual C++ 2010 redist libraries
Source: bin\msvcr100.DLL; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\msvcp100.DLL; DestDir: {app}\bin; Components: {#COMPN_SCILAB}


;MKL
#ifndef SCILAB_ARM64
Source: bin\libmmd.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB} and not ({#COMPN_MKL_CPU_LIBRARY} or {#COMPN_FFTW_MKL_LIBRARY})
Source: bin\libifcoremd.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB} and not ({#COMPN_MKL_CPU_LIBRARY} or {#COMPN_FFTW_MKL_LIBRARY})
Source: bin\libiomp5md.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB} and not ({#COMPN_MKL_CPU_LIBRARY} or {#COMPN_FFTW_MKL_LIBRARY})
Source: bin\libifcorertd.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\svml_dispmd.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
#endif
;
; used for localization
Source: bin\libintl.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libintl.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
#ifndef SCILAB_ARM64
Source: bin\GNU_gettext.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
#endif
;

#if defined(SCILAB_ARM64)
Source: bin\liblapack.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libf2c-2.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\iconv-2.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
#else
Source: bin\lapack.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB} and not ({#COMPN_MKL_CPU_LIBRARY} or {#COMPN_FFTW_MKL_LIBRARY})
Source: bin\libf2c.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
#endif
Source: bin\zlib1.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\GetWindowsVersion.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\scilab_windows.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\scilab_windows.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libxml2.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libcurl.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\pcre2-16.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: bin\libf2c.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\lapack.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
;
Source: bin\Scilex.exe; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\WScilex.exe; DestDir: {app}\bin; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: bin\WScilex-cli.exe; DestDir: {app}\bin; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: bin\Dumpexts.exe; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\f2c.exe; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
; hdf5 libraries
Source: bin\szip.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\hdf5.dll ; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\hdf5.lib ; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\hdf5_cpp.dll ; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\hdf5_cpp.lib ; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\hdf5_hl.dll ; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\hdf5_hl.lib ; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\hdf5_hl_cpp.dll ; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\hdf5_hl_cpp.lib ; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
#ifndef SCILAB_ARM64
Source: bin\zlib.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
#endif
;
; libarchive libraries
Source: bin\archive.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\bz2.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\liblzma.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\lz4.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\zstd.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
#if defined(SCILAB_ARM64)
Source: bin\libcrypto-3-arm64.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
#else
Source: bin\libcrypto-3-x64.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
#endif
;
; JCEF
Source: bin\jcef\*.*; DestDir: {app}\bin\jcef; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
; xslt
Source: bin\libxslt.dll; DestDir: {app}\bin; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
; XLNT
Source: bin\xlnt.dll; DestDir: {app}\bin; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
; ARROW/PARQUET
Source: bin\arrow.dll; DestDir: {app}\bin; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: bin\brotlicommon.dll; DestDir: {app}\bin; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: bin\brotlidec.dll; DestDir: {app}\bin; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: bin\brotlienc.dll; DestDir: {app}\bin; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: bin\parquet.dll; DestDir: {app}\bin; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: bin\snappy.dll; DestDir: {app}\bin; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
; JavaFX
Source: bin\glass.dll; DestDir: {app}\bin; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: bin\prism_d3d.dll; DestDir: {app}\bin; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: bin\prism_sw.dll; DestDir: {app}\bin; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
; batch files
Source: bin\scinotes.bat; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\xcos.bat; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\scilab.bat; DestDir: {app}\bin; Components: {#COMPN_SCILAB}

