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
//

function %_Matplot()
   // Example #1
   ny = 400;
   nx = 300;
   M = pmodulo((0:(ny-1))'*ones(1:nx) + ones(ny,1)*(0:nx-1),ceil(max(nx,ny)/3));
   clf
   f = gcf();
   f.color_map = coolcolormap(nx);
   subplot(1,2,1)
   Matplot(M)
   a = gca();
   a.tight_limits = "on";
   a.isoview = "on";
   a.title.font_size = 2;
   xtitle([ msprintf(_("Matplot(M)  with  size(M)=>(%d,%d)"),ny,nx); 
            _("Color''s number of pixel(i,j)");
            _("   = rounded value of M(i,j)")]);
   
   // Example #2
   nc = 200;
   np = 2;
   ni = -(np*nc/2):(np*nc/2);
   f.color_map = [f.color_map ; hsvcolormap(nc)];
   [X,Y] = meshgrid(ni,ni);
   R = nx + 1 + pmodulo(sqrt(X.^2+Y.^2),nc/np);
   //M = nx + ceil(sqrt(((-ni:ni)'*(-ni:ni))+ni*ni+1));
   subplot(1,2,2)
   Matplot(R)
   a = gca();
   a.tight_limits = "on";
   a.isoview = "on";
   a.title.font_size = 2;
   xtitle([ _("Matplot(M) scales axes with"); 
            _("indices of M''s components")], ..
            _("For axes scaled with data, please use Matplot1(..)") )
endfunction
