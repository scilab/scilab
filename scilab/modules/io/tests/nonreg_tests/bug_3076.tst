// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3076 -->
//
// <-- Short Description -->
//    getrelativefilename() doesn't return error message when input arguments 
//    havn't the right size.
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3076
//

A = [ "/usr/local/scilab/bin" ; "/home/user/Desktop" ];
B = [ "/usr/local/scilab/share/scilab/modules/core/etc/core.start" ;"/home/user/Desktop/pictures/test.png" ];

if execstr("getrelativefilename(A,B);","errcatch") == 0 then pause,end
