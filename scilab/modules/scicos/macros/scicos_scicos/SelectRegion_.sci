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

function SelectRegion_()
//** 10 July 2006: find a
//** xx/01/07 : Alan - review (introduce rubberbox here)
//**            Objects must be totally included in
//*             "rubber box" in order to be selected.

//** 18 Mar 2008 : update to "scicos_rubberbox()" for Scilab 5


  if with_gtk()|MSDOS then  // first click under windows treated as
                            // press move (always the case under gtk)
    Cmenu =[]; Select=[] ;
  else
    Cmenu =[]; %ppt=[]; Select=[] ;
  end
  
  gh_winback = gcf(); //** save the current figure 
  gh_percentwin = scf(%win);

  drawnow(); 
  [rect,button] = scicos_rubberbox([%pt(1); %pt(2); 0; 0], %t) ;
  if or(button == [2 5 12 -1000]) then // right button exit OR active window has been closed
    return ; //** ---> Exit point
  end

  ox = rect(1); oy = rect(2) ; w = rect(3); h = rect(4);

  clear rect

  scf(gh_winback); //** restore the current figure
  kc = find(%win==windows(:,2))

  if kc==[] then
    message("This window is not an active palette");
    return ; //** Exit point

  elseif windows(kc,1)<0 then //click inside a palette window
    kpal = -windows(kc,1);
    [in,out] = get_objs_in_rect(palettes(kpal),ox,oy,w,h)

  elseif %win==curwin then //click inside the current window
    [in,out] = get_objs_in_rect(scs_m,ox,oy,w,h)

  elseif slevel>1 then
    execstr('[in,out]=get_objs_in_rect(scs_m_'+string(windows(kc,1))',ox,oy,w,h)

  else
    return ; //** ---> Exit point
  end

  clear ox ;clear oy ;clear w; clear h;

  Select = [in',%win*ones(in')]; 

endfunction

//**---------------------------------------------------------------------------
function [in,out] = get_objs_in_rect(scs_m,ox,oy,w,h)
  in = []; out = [] ; ok = %f;
  for k=1:lstsize(scs_m.objs)
    ok = %f;
    o=scs_m.objs(k)
    if typeof(o)=='Block'|typeof(o)=='Text' then
//       x=[0 1 1 0]*o.graphics.sz(1)+o.graphics.orig(1)
//       y=[0 0 1 1]*o.graphics.sz(2)+o.graphics.orig(2)
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

//     for kk=1:size(x,'*')
//       data=[(ox-x(kk))'*(ox+w-x(kk)),(oy-h-y(kk))'*(oy-y(kk))];
//       if data(1)<0&data(2)<0 then ok=%t;in=[in k];break;end
//     end

    if ~ok then out=[out k],end

  end
endfunction
