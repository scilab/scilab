// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->

TMP_OS_DIR=TMPDIR;

TMP_DIR = TMP_OS_DIR+filesep()+'ilib_build';

rmdir(TMP_DIR,'s');
mkdir(TMP_OS_DIR,'ilib_build');


if ~isdir(TMP_DIR) then pause,end;

//Here with give a complete example on adding new primitive to Scilab
//create the procedure files
f1=['extern double fun2();'
    'void fun1(double *x, double *y)'
    '{*y=fun2(*x)/(*x);}'];

mputl(f1,TMP_DIR+filesep()+'fun1.c')

f2=['#include <math.h>'
    'double fun2(double x)'
    '{ return( sin(x+1.));}'];
mputl(f2,TMP_DIR+filesep()+'fun2.c');

//creating the interface file
i=['#include ""stack-c.h""'
	'#include ""stackTypeVariable.h""'
   'extern int fun1 ( double *x, double *y);'
   'int intfun1(fname)' 
   'char * fname;'
   '{'
   '  int m1,n1,l1;'
   '  CheckRhs(1,1);'
   '  CheckLhs(1,1);'
   '  GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);'
   '  fun1(stk(l1),stk(l1));'
   '  LhsVar(1) = 1;'
   '  return 0;'
   '}'];
mputl(i,TMP_DIR+filesep()+'intfun1.c')


//creating the shared library (a gateway, a Makefile and a loader are 
//generated. 
cur_dir = pwd();
chdir(TMP_DIR);

files=['fun1.o','fun2.o','intfun1.o'];
ilib_build('foo',['scifun1','intfun1'],files,[]);


// disable message
warning_mode = warning('query');
warning('off');

// load the shared library 
exec loader.sce

// enable message 
warning(warning_mode);

chdir(cur_dir);

//using the new primitive
if ( norm(scifun1(33) - .0160328) > 1 ) then pause,end

// ulink() all libraries
ulink();

//remove TMP_DIR
rmdir(TMP_DIR,'s');