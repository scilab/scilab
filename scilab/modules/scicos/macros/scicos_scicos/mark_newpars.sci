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

function newparameters=mark_newpars(k,newparametersb,newparameters)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

// k block index in x
//
  o=scs_m(get_subobj_path(k))
  model=o.model
  if model.sim=='super'|model.sim=='csuper'
    for npb=newparametersb
      ok=%t;
      for np=newparameters
	if and(np==[k npb]) then
	  ok=%f;break,
	end
      end
      if ok then
	newparameters(size(newparameters)+1)=[k npb];
      end
    end
  else
    ok=%t
    for np=newparameters
      if np==k then
	ok=%f;break;
      end
    end
    if ok then
      newparameters(size(newparameters)+1)=k
    end
  end
endfunction
