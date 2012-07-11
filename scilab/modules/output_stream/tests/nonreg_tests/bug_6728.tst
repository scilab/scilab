// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 6728 -->
//
// <-- Short Description -->
// The function lib didn't print the same thing when it was entered several times...

//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6728
//

cd(TMPDIR);
mkdir('bug_6728');
cd('bug_6728');

fun1 = ['function r = aaaa()';
'disp(''aaaa'')';
'endfunction'];
mputl(fun1,'aaaa.sci');

fun2 = ['function r = bbbb()';
'disp(''bbbb'')';
'endfunction'];
mputl(fun2,'bbbb.sci');

genlib('bug_6728lib','.');

bug_6728lib = lib('.');
strs = string(bug_6728lib);
clear bug_6728lib

for i=1:1000
  bug_6728lib = lib('.');
  strbis = string(bug_6728lib);
  if strs <> strbis then pause,end
  clear bug_6728lib
  clear strbis
end
