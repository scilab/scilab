// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- ENGLISH IMPOSED -->
// <-- CLI SHELL MODE -->
ilib_verbose(0);
curDir=pwd();
cd TMPDIR;
f1=["int ext1c(int *n, double *a_, double *b, double *c)"
    "{"
    "char *a=0;"
    "strcpy(a,""plop"");"
    "  return(0);}"];

mputl(f1,"fun1.c");

libname = ilib_for_link("ext1c", "fun1.c", [], "c", [], "loader.sce", "test");
refname = "libtest" + getdynlibext();
if refname <> libname then pause, end
exec('loader.sce');

a=[1,2,3];b=[4,5,6];n=3;
expression="call(''ext1c'',n,1,''i'',a,2,''d'',b,3,''d'',''out'',[1,3],4,''d'')"
if execstr(expression,'errcatch')==0 then pause,end
// if we are here, that means that the seg fault did not kill Scilab
if lasterror()==[] then pause,end
cd(curDir);
