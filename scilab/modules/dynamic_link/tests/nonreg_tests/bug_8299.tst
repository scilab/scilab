// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- WINDOWS ONLY -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 8299 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8299
//
// <-- Short Description -->
// ilib_compile did not return enough feedback on Windows when compiling failed.

// edit SCI/contrib/toolbox_skeleton/src/c/csub.c
editor SCI/contrib/toolbox_skeleton/src/c/csub.c

// add a line with a error by example : "blabla it is a error"
// save and type in scilab
cd SCI/contrib/toolbox_skeleton/src/c/
exec builder_c.sce

// you should have something :

-->exec builder_c.sce;
   Generate a loader file
   Generate a Makefile
   Running the Makefile
   Compilation of csum.c
   Compilation of csub.c
   Building shared library (be patient)
 
!------------- Compile file csub.c --------------                                                                                                                                !
!                                                                                                                                                                                !
!	IF NOT EXIST  Release mkdir Release                                                                                                                                            !
!                                                                                                                                                                                !
!	cl -D__MSC__ -DFORDLL -DWIN32 -c -DSTRICT -D_CRT_SECURE_NO_DEPRECATE -D__MAKEFILEVC__ -nologo -I"D:/GIT-SC~1.3-B/scilab/scilab/libs/MALLOC/includes"  -I"D:/GIT-SC~1.3-B/scilab!
!     /scilab/modules/core/includes"  -I"D:/GIT-SC~1.3-B/scilab/scilab/modules/api_scilab/includes"  -I"D:/GIT-SC~1.3-B/scilab/scilab/modules/call_scilab/includes"  -I"D:/GIT-SC!
!     ~1.3-B/scilab/scilab/modules/output_stream/includes"  -I"D:/GIT-SC~1.3-B/scilab/scilab/modules/jvm/includes"  -I"D:/GIT-SC~1.3-B/scilab/scilab/modules/localization/include!
!     s"  -I"D:/GIT-SC~1.3-B/scilab/scilab/modules/dynamic_link/includes"  -I"D:/GIT-SC~1.3-B/scilab/scilab/modules/mexlib/includes"  -I"D:/GIT-SC~1.3-B/scilab/scilab/modules/ti!
!     me/includes"  -I"D:/GIT-SC~1.3-B/scilab/scilab/modules/windows_tools/includes"  -I"D:/GIT-SC~1.3-B/scilab/scilab/libs/f2c"  -I"D:/GIT-SC~1.3-B/scilab/scilab/libs/hashtable!
!     "  -I"D:/GIT-SC~1.3-B/scilab/scilab/libs/intl" -W3 -Gd -Z7 -O2 -MT /Fo"Release/" /Fd"Release/" -DFORDLL -ID:\GIT-Scilab-5.3-branch\scilab\scilab\contrib\toolbox_skeleton\s!
!     rc\c\ csub.c                                                                                                                                                               !
!                                                                                                                                                                                !
!csub.c                                                                                                                                                                          !
!                                                                                                                                                                                !
!csub.c(15) : error C2061: syntax error : identifier 'it'                                                                                                                        !
!                                                                                                                                                                                !
!csub.c(15) : error C2059: syntax error : ';'                                                                                                                                    !
!                                                                                                                                                                                !
!csub.c(15) : error C2061: syntax error : identifier 'a'                                                                                                                         !
!                                                                                                                                                                                !
!csub.c(15) : error C2059: syntax error : ';'                                                                                                                                    !
!                                                                                                                                                                                !
!csub.c(16) : fatal error C1004: unexpected end-of-file found                                                                                                                    !
!                                                                                                                                                                                !
!NMAKE : fatal error U1077: '"C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\bin\cl.EXE"' : return code '0x2'                                                             !
!                                                                                                                                                                                !
!Stop.                                                                                                                                                                           !
 !--error 10000 
ilib_compile: Error while executing Makelib.mak.
at line      66 of function ilib_compile called by :  
at line      86 of function ilib_for_link called by :  
at line      28 of function tbx_build_src called by :  
              "",                               "",                               CFLAGS)
at line      13 of exec file called by :    
exec builder_c.sce;
 
 