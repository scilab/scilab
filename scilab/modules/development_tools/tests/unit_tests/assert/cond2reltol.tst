// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

function flag = MY_assert_equal ( computed , expected )
  if computed==expected then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction

//
condition = [
 0
 1
 1.e1
 1.e2
 1.e3
 1.e13
 1.e14
 1.e15
 1.e16
 1.e17
 1.e18
];
computed = assert_cond2reltol ( condition );
expected = [
    1.110223024625162210D-16  
    1.110223024625162210D-16  
    1.110223024625162260D-15  
    1.110223024625162220D-14  
    1.110223024625162220D-13
    1.110223024625157625D-03
    1.110223024625162265D-02  
    1.110223024625157789D-01  
    1.000000000000000000D+00  
    1.000000000000000000D+00  
    1.000000000000000000D+00  
];
assert_checkalmostequal ( computed , expected , 100*%eps);
//
// With offset
computed = assert_cond2reltol ( 1.e2 , [5 4 3 2 1 0 -1 -2 -3 -4 -5]' );
expected = [
    1.110223024625162210D-16  
    1.110223024625162210D-16  
    1.110223024625162210D-16  
    1.110223024625162210D-16  
    1.110223024625162260D-15  
    1.110223024625162220D-14  
    1.110223024625162220D-13  
    1.110223024625162195D-12  
    1.110223024625162195D-11  
    1.110223024625162227D-10  
    1.110223024625162331D-09  
];
assert_checkalmostequal ( computed , expected , 100*%eps);
//
computed = assert_cond2reltol ( 1.e14 , [0 -1 -2 -3] );
expected = [1.110223024625162265D-02    1.110223024625162230D-01    1.000000000000000000D+00    1.000000000000000000D+00];
assert_checkalmostequal ( computed , expected , 100*%eps);
//
computed = assert_cond2reltol ( 1.e2 , [0 1 2 3] ) ;
expected = [1.110223024625162220D-14    1.110223024625162260D-15    1.110223024625162210D-16    1.110223024625162210D-16];
assert_checkalmostequal ( computed , expected , 100*%eps);

