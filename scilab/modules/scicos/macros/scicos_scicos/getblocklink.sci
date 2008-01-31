//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//                      - Alan Layec <alan.layec@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

function [k,wh]=getblocklink(scs_m,pt)
//** 24/07/07: Al@n's patch for rotation of blocks
  n=lstsize(scs_m.objs)
  wh=[];
  x=pt(1);
  y=pt(2);
  data=[]
  k=[]
  eps=6
  for i=1:n //loop on objects
    o=scs_m.objs(i)
    if typeof(o)=='Block' then

      [orig,sz]=(o.graphics.orig,o.graphics.sz)

      orig=orig-eps;
      sz=sz+2*eps;

      if %pt<>[] then
        xxx=rotate([pt(1);pt(2)],...
                   -o.graphics.theta*%pi/180,...
                   [orig(1)+sz(1)/2;orig(2)+sz(2)/2]);
        x=xxx(1);
        y=xxx(2);
      end

      data=[(orig(1)-x)*(orig(1)+sz(1)-x),...
            (orig(2)-y)*(orig(2)+sz(2)-y)]

      if data(1)<0&data(2)<0 then
        k=i
        break
      end
    elseif typeof(o)=='Link' then
      xx=o.xx;
      yy=o.yy;
      [d,ptp,ind]=dist2polyline(xx,yy,pt)
      if d<eps/2 then
        k=i
        wh=ind
        break
      end
    end
  end
endfunction
