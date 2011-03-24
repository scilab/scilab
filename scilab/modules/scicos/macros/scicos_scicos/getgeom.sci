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

function g=getgeom()
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

// call within a simulation function of a scicos block this function 
//returns a vector g containing [win x,y,w,h] of the block. win is either 
//the number of main scicos window or -1 if the block is not in the main
//window.



path=%cpr.corinv(curblock())

g=[-1;zeros(5,1)],

direc=path(1:$-1)

if direc==[]&or(curwin==winsid()) then
  g(1)=curwin
else
  dd=inactive_windows(1)
  for i=1:size(dd)
    if isequal(dd(i),direc) then 
       g(1)=inactive_windows(2)(i),
       if ~or(g(1)==winsid()) then g(1)=-1,end
       break,
    end
  end
end

if g(1)>-1 then
  Path=list('objs');
  for pk=direc
    Path=lstcat(Path,pk,'model','rpar','objs');
  end
  Path=lstcat(Path,path($))
  o = scs_m(Path);  
  orig = o.graphics.orig;
  sz = o.graphics.sz 
  k  =  Path($) ; //** scs_m index
  f = scf(g(1)); 
  a = gca();
  p_size = size(a.children, '*'); 
  gr_k = get_gri(k,p_size);
  // gh_obj = a.children(gr_k);
  // gh_text = gh_obj.children(2);
  //** 
  g(2:5) = [orig(:);sz(:)];  
  g(6) =  gr_k ;

end 

endfunction

