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
// This is the demonstration script of grayplot
// used when calling grayplot without any parameter.
//

function [] = %_grayplot()
  t=-%pi:0.05:%pi;
  m=sin(t)'*cos(t);
  f = gcf();
  f.color_map = hotcolormap(128);
  grayplot(t,t,m);
endfunction
