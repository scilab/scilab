// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - ESI Group - Delamarre Cedric
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- INTERACTIVE TEST -->
// Execute blocks one by one and check if the callstack is good.

// block #1
function subblock1(a)
    b = a
    1+1
    b(0) // will error
endfunction
function block1(a)
    b = a;
    subblock1(b)
endfunction
block1(1)
// at line     4 of function subblock1
// at line     3 of function block1

// Invalid index.


// block #2
function block2()
  a="a string"
  a(2)=0
endfunction
block2()
// at line     9 of function %s_i_c ( SCI/modules/overloading/macros/%s_i_c.sci line 21 )
// at line     3 of function block2

// %s_i_c: Non implemented feature.


// block #3
function block3()
  1+1;
  "a string"*2
endfunction
function r=%c_m_s(a,b)
  cos("a string")
end
block3()
// in builtin                cos
// at line     2 of function %c_m_s
// at line     3 of function block3

// Function not defined for given argument type(s).
//   check arguments or define function %c_cos for overloading.


// block #4
function block4()
  1+1

  ["a string", 2]
endfunction
function r=%c_c_s(a,b)
  cos("a string")
end
block4()
// in builtin                cos
// at line     2 of function %c_c_s
// at line     4 of function block4

// Function not defined for given argument type(s).
//   check arguments or define function %c_cos for overloading.


// block #5
function r=%i_cos(a)
    r=cos("a string")
endfunction

function block5()
    cos(int8(2))
endfunction
block5()
// in builtin                cos
// at line     2 of function %i_cos
// in builtin                cos
// at line     2 of function block5

// Function not defined for given argument type(s).
//   check arguments or define function %c_cos for overloading.