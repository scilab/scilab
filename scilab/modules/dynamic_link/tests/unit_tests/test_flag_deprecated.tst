// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF --> 
//
ilib_verbose(0);

if getos() == 'Windows' then
  TMP_OS_DIR = getenv('TMP','err');
  assert_checkfalse(TMP_OS_DIR == 'err');
else
  TMP_OS_DIR = '/tmp';
  assert_checkequal(isdir(TMP_OS_DIR), %t);
end

TMP_DIR = TMP_OS_DIR+filesep()+'link';

rmdir(TMP_DIR,'s');
mkdir(TMP_OS_DIR,'link');

assert_checkequal(isdir(TMP_DIR), %t);

//Example of the use of ilib_for_link with  a simple C code
    f1=['#include <math.h>'; ..
    '#include ""stack-c.h""'; ..
    'void fooc(double c[],double a[], double *b,int *m,int *n)'; ..
    '{'; ..
    '   int i;'; ..
    '   for ( i =0 ; i < (*m)*(*n) ; i++) '; ..
    '     c[i] = sin(a[i]) + *b; '; ..
    '}'];

mputl(f1, TMP_DIR + filesep() + 'test_flag_deprecated.c');

cur_dir = pwd();
chdir(TMP_DIR);


setenv('__USE_DEPRECATED_STACK_FUNCTIONS__', 'NO');
ierr = execstr("ilib_for_link(''deprecated'', ''test_flag_deprecated.c'', [], ""c"");", "errcatch");
assert_checkequal(ierr, 10000);

setenv('__USE_DEPRECATED_STACK_FUNCTIONS__', 'YES');
ierr = execstr("ilib_for_link(''deprecated'', ''test_flag_deprecated.c'', [], ""c"");", "errcatch");
assert_checkequal(ierr, 0);
