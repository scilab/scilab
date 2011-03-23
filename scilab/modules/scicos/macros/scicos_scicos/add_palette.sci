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

function palettes = add_palette(palettes,path,kpal)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

  path = stripblanks(path)
  
  for k=size(palettes)+1:kpal
     palettes(k)=list()
  end

  [ok,scs_m,cpr,edited] = do_load(path,'palette')
  
  if ok & size(scs_m.objs)>0 then 
    palettes(kpal) = scs_m;
  else 
    palettes(kpal) = list();
  end

endfunction
