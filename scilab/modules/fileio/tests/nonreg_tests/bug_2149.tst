// <-- Non-regression test for bug 2149 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2149
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

// Copyright INRIA
// Scilab Project - Allan CORNET
// Copyright INRIA 2006

// Re-Written By Pierre MARECHAL
// Copyright INRIA 2007

M=[1+%i;2+3*%i];
if execstr('fprintfMat(TMPDIR+''/pipo.txt'',M,''%f'',[''comment un'';''comment 2''])','errcatch') == 0 then pause,end

M=[1.2,2.3];
if execstr('fprintfMat(TMPDIR+''/pipo.txt'',M,''%f'',[''comment un'';''comment 2''])','errcatch') <> 0 then pause,end

if execstr('fprintfMat(TMPDIR+''/pipo.txt'',''M'',''%f'',[''comment un'';''comment 2''])','errcatch') == 0 then pause,end
