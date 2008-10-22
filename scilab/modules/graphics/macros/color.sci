// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function id=color(c1,c2,c3)
// get a color from the current colormap
// or add it to current colormap if it does not exist

[lhs,rhs]=argn(0)
if rhs==1 then 
  if (type(c1)<>10 | size(c1,"*")<>1) then
    error("color: argument must be a string")
  else
    c=name2rgb(c1);
    if c==[] then
      error(""""+c1+""" is not a known color")
    end
    c=c/255;
  end
elseif rhs==3 then
  if (type(c1)<>1 | type(c2)<>1 | type(c3)<>1 | size(c1,"*")<>1 | size(c2,"*")<>1 | size(c3,"*")<>1) then
    error("color: arguments must be integers")
  end
  if (c1>255 | c1<0 | c2>255 | c2<0 | c3>255 | c3<0) then
    error("color: arguments must be integers between 0 and 255")
  end
  c=[c1,c2,c3]/255;
else
  error("function ""color"" must have 1 or 3 arguments")
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
