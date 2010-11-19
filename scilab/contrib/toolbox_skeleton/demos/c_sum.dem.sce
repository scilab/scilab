// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function demo_c_sum()

  mode(-1);
  lines(0);

  disp("c_sum(3,4)");
  disp(c_sum(3,4));
  
endfunction

demo_c_sum();
clear demo_c_sum;
