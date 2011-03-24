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
function [in,out] = get_objs_in_rect(scs_m,ox,oy,w,h)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

//returns in "in" the index of Scicos objects which are inside the
//selected rectangle and in "out" index of Scicos objects which are
//outside the rectangle
  in = []; out = [] ; ok = %f;
  for k=1:lstsize(scs_m.objs)
    ok = %f;
    o=scs_m.objs(k)
    if typeof(o)=='Block'|typeof(o)=='Text' then
      if (ox <= o.graphics.orig(1)) & ...
         (oy >= o.graphics.orig(2)+o.graphics.sz(2)) & ...
         ((ox+w) >= (o.graphics.orig(1)+o.graphics.sz(1))) & ...
         ((oy-h) <= o.graphics.orig(2)) then
           ok=%t
           in=[in k]
      end

     elseif  typeof(o)=='Link' then
//       [x,y]=(o.xx,o.yy)
      if (ox <= max(o.xx)) & ...
         (oy >= max(o.yy)) & ...
         ((ox+w) >= max(o.xx)) & ...
         ((oy-h) <= min(o.yy)) then
           ok=%t
           in=[in k]
      end

    else
      x=[] // object is "Deleted"
    end
    if ~ok then out=[out k],end

  end
endfunction
