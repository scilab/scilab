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

function supers=findopenchildren(scs_m,super_path,supers)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

//find paths to all open (inactive) sub-diagrams

  if argn(2)<3 then supers=list(),end
  for k=1:lstsize(scs_m.objs)
    o=scs_m.objs(k)
    if typeof(o)=='Block' then
      model=o.model
      if model.sim=='super' then
        n=size(inactive_windows(1))
        for i=1:n
          path=inactive_windows(1)(i)
          if isequal(path,[super_path,k]) & or(winsid()==inactive_windows(2)(i)) then
             supers($+1)=[super_path,k]
             break
          end
        end
        supers=findopenchildren(model.rpar,[super_path,k],supers)
      end
    end
  end
endfunction
