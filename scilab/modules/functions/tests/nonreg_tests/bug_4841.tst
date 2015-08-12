// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 4841 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4841
//
// <-- Short Description -->
// functions created by 'getd' were unknown to scilab.

cd TMPDIR;
mkdir('bug_2807');
txt = ['function bug_2807()';
       'endfunction'];
mputl(txt, TMPDIR + '/bug_2807/bug_2807.sci');
cd(SCI);
getd(TMPDIR + '/bug_2807');
v = getvariablesonstack();
if grep(v,'bug_2807') == [] then pause, end
