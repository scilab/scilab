// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Samuel GOUGEON
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
// This is the demonstration script of Matplot
// used when calling Matplot without any parameter.

function %_Matplot()
   ny = 400;
   nx = 300;
   M = pmodulo((0:(ny-1))'*ones(1:nx) + ones(ny,1)*(0:nx-1),ceil(max(nx,ny)/3));
   clf
   f = gcf();
   f.color_map = coolcolormap(nx);
   Matplot(M)
   a = gca();
   a.tight_limits = "on";
   a.isoview = "on";
   a.title.font_size = 2;
   Tlab = _("Matplot(M)  with  size(M)=>(%d,%d)\nThe color''s number of pixel(i,j)\n  = rounded value of M(i,j)");
   Xlab = _("For data-scaled axes, please use Matplot1(..)");
   Ylab = _("Axes are scaled with M''s indices");
   xtitle(msprintf(Tlab,ny,nx), Xlab, Ylab); 
endfunction

