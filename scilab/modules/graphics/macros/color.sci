// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function id=color(varargin)
// get a color from the current colormap
// or add it to current colormap if it does not exist

if size(varargin)==1 then 
  c1=varargin(1);
  if (type(c1)<>10 | size(c1,"*")<>1) then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"), "color", 1));
  else
    c=name2rgb(c1);
    if c==[] then
      error(msprintf(gettext("%s: Wrong values for input argument #%d: ''%s'' expected.\n"), "color", 1, "know color"));
    end
    c=c/255;
  end
elseif size(varargin)==3 then
  for i=1:3,
    if type(varargin(i))<>1 then
      error(msprintf(gettext("%s: Wrong type for input argument #%d: A real expected.\n"), "color", i));
    end
    if size(varargin(i),"*")<>1 then 
      error(msprintf(gettext("%s: Wrong size for input argument #%d: A scalar expected.\n"), "color", i));
    end
    if varargin(i)<0 | varargin(i)>255 then
      error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the interval [%s, %s].\n"),"color",i,"0","255"));
    end
  end
  c=[varargin(1),varargin(2),varargin(3)]/255;
else
    error(msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"), "color", 1, 3));
end

f=gcf();
cmap=f.color_map;

ncol=max(size(cmap,"r"));
k=find(abs(cmap-ones(ncol,1)*c)*ones(3,1)<=1.d-5);
if k==[] then
  // color does not exist, add it
  cmap=[cmap;c];
  id=ncol+1;
  f.color_map=cmap;
else
  id=k(1);
end

endfunction
