//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Jean-Baptiste Silvy
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

//
// This is the demonstration script of Matplot1
// used when calling Matplot without any parameter.
//

function [] = %_Matplot1()
  axes = gca();
  axes.data_bounds = [0,0;10,10];
  m = ones(50,50);
  m = 3 * tril(m) + 2 * m;
  Matplot1(m,[4,4,9,9]);
endfunction
