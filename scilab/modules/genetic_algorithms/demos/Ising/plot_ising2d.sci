// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function plot_ising2d(Var)
drawlater;
a = gca();
Size = size(Var,1);
a.data_bounds = [0.5,0.5;Size+0.5,Size+0.5];
for i=1:Size
  for j=1:Size
    if (Var(i,j)==1) then
      plot(i,j,'go');
    else
      plot(i,j,'r+');
    end
  end
end
xtitle('Ising 2D model');
drawnow;
endfunction
