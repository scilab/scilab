// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Yann COLLETTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 5637 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5637
//
// <-- Short Description -->
// The plot function does not work when the data variations are small.

sigma = sqrt(2);

function p = f( x , mu , sigma )
  p = exp ( -0.5 * (log(x) - mu).^2/sigma.^2 ) ./ ( sigma * x * sqrt(2*%pi) )
endfunction

x = linspace ( 200 , 400 , 1000 );

p = f( x , 300.0 , 30.0 );

assert_checkfalse(execstr('plot ( x , p );','errcatch') <> 0);
