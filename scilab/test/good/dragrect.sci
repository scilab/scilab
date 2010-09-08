// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [rects,btn]=dragrect(varargin)

  // Check number of input argument
  if size(varargin)<>1 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "dragrect", 1));
  end
  rects=varargin(1);

  if size(rects,1)==1 then rects=rects(:),end
  n=size(rects,2)
  f=gcf();pix=f.pixmap;f.pixmap='on';
  xrects(rects) 
  R=gce(); //Compound of rectangles
  rep=[rects(1),rects(2),-1]
  while rep(3)==-1 then
    show_pixmap()
    repn=xgetmouse()
    if repn(3)==-100  then //window has been closed 
      btn=repn(3)
      return
    end
    rects(1:2,:)=rects(1:2,:)+(repn(1:2)-rep(1:2))'*ones(1,n);
    move(R,repn(1:2)-rep(1:2))
    rep=repn
  end
  f.pixmap=stripblanks(pix)
  delete(R)
  btn=rep(3)
endfunction
