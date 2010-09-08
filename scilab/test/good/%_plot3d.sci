//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

//
// This is the demonstration script of plot3d
// used when calling plot3d without any parameter.
//

function [] = %_plot3d()
  x = %pi * [-1:0.05:1]';
  z = sin(x)*cos(x)';
  f = gcf();
  f.color_map = jetcolormap(32);
  plot3d(x, x, z, 70, 70);
  e=gce();
  e.color_flag = 1;
endfunction