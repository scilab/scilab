// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


// addcolor : add colors to the current colormap
function new=addcolor(varargin)

  // Check number of unput argument
  if size(varargin)<>1 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "addcolor", 1));
  end
  c=varargin(1);
  
  //Modif. to accept 3x1 column vector
  if or(size(c)==1) then c=matrix(c,1,-1),end
  
  if size(c,'c')<>3 | max(c)>1 | min(c)<0 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: %s data structure expected.\n"), "addcolor", 1, "color_map"));
  end
  f=gcf();
  cmap=f.color_map;
  ncol=size(cmap,'r')
  ncol0=ncol
  new=[]
  for k=1:size(c,'r')
    ck=c(k,:)
    k2=find(sum(abs(cmap-ones(ncol,1)*ck),2)<=1.d-5)
    if k2==[] then 
      cmap=[cmap;ck],
      ncol=ncol+1
      new=[new ncol]
    else
      new=[new k2(1)]
    end 
  end
  if ncol>ncol0 then //at least one color added
    f.color_map=cmap
  end
endfunction
