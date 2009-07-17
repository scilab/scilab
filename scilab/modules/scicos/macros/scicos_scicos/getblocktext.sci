//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
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

function k=getblocktext(scs_m,pt)
  if pt==[] then k=[],return,end
  n=lstsize(scs_m.objs)
  xf=60;
  yf=40;
  x=pt(1);
  y=pt(2)
  data=[]
  for i=n:-1:1
    k=i
    o=scs_m.objs(i)
    if typeof(o)=='Block' then

      [orig,sz]=(o.graphics.orig,o.graphics.sz);

      dx=xf/7;
      dy=yf/7;

      //if %pt<>[] then //??? what this test means??
      xxx=rotate([pt(1);pt(2)],...
		 -o.graphics.theta*%pi/180,...
		 [orig(1)+sz(1)/2;orig(2)+sz(2)/2]);
      x=xxx(1);
      y=xxx(2);
      //end

      data=[(orig(1)-dx-x)*(orig(1)+sz(1)+dx-x),..
	    (orig(2)-dy-y)*(orig(2)+sz(2)+dy-y)]

      if data(1)<0&data(2)<0 then
	return
      end

    end
  end

  //  _____________________________________________________________
  //  _____________ First look for blocks then for texts _________

  for i=n:-1:1
    k=i
    o=scs_m.objs(i)
    if typeof(o)=='Text' then

      [orig,sz]=(o.graphics.orig,o.graphics.sz);

      dx=xf/7;
      dy=yf/7;

      //if %pt<>[] then //??? what this test means??
      xxx=rotate([pt(1);pt(2)],...
		 -o.graphics.theta*%pi/180,...
		 [orig(1)+sz(1)/2;orig(2)+sz(2)/2]);
      x=xxx(1);
      y=xxx(2);
      //end

      data=[(orig(1)-dx-x)*(orig(1)+sz(1)+dx-x),..
	    (orig(2)-dy-y)*(orig(2)+sz(2)+dy-y)]

      if data(1)<0&data(2)<0 then
	return
      end

    end
  end
  k=[]
endfunction
