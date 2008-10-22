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

function [rect] = dig_bound(scs_m)

  n = lstsize(scs_m.objs);
  
  if n < 1 then rect=[]
      return
  end
  
  
  xmin = 100000;
  xmax = -xmin;
  ymin = xmin;
  ymax = -xmin;
  
  for i=1:n          // loop on objects
  
    o = scs_m.objs(i)
    
    if typeof(o)=='Block'|typeof(o)=='Text' then
      if execstr('orig=o.graphics.orig','errcatch')<>0 then pause,end //** for debugging purpose
      sz=o.graphics.sz
      xmin=min(xmin,orig(1))
      xmax=max(xmax,orig(1)+sz(1))
      ymin=min(ymin,orig(2))
      ymax=max(ymax,orig(2)+sz(2))
    elseif typeof(o)=='Link' then
      xx=o.xx(:);yy=o.yy(:);
      xmin=min([xmin;xx])
      xmax=max([xmax;xx])
      ymin=min([ymin;yy])
      ymax=max([ymax;yy])
    end
  
  end //** --- of the for loop 
  
  rect=[xmin,ymin,xmax,ymax] //** return with the coordinate of an immaginary 
                             //** rectangle that include all the graphics object 

endfunction
