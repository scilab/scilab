// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006      - INRIA - Allan CORNET <allan.cornet@inria.fr>
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2149 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2149
//
// <-- Short Description -->
//    Missing error message in fprintfMat
//
//
//    help fprintfMat states that the matrix to save must be a matrix of real
//    numbers. Therefore:
//
//    -->M=[1+%i;1-%i];
//
//    -->fprintfMat("mymatrix.txt",M,"%f")
//
//    -->
//    should spit an error. It does not but it silently saves the real part
//    of M instead.
//
//    Another (better) option: extend fprintfMat/scanfMat so that they work
//    with complex matrices (or matrices of integers, booleans, etc).
//
//    Francois

M=[1+%i;2+3*%i];
if execstr('fprintfMat(TMPDIR+''/pipo.txt'',M,''%f'',[''comment un'';''comment 2''])','errcatch') == 0 then pause,end

M=[1.2,2.3];
if execstr('fprintfMat(TMPDIR+''/pipo.txt'',M,''%f'',[''comment un'';''comment 2''])','errcatch') <> 0 then pause,end

if execstr('fprintfMat(TMPDIR+''/pipo.txt'',''M'',''%f'',[''comment un'';''comment 2''])','errcatch') == 0 then pause,end
