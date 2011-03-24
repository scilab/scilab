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
// Copyright INRIA
function [o,needcompile,ok]=do_RemoveAtomic(o)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

ok=%t
if typeof(o)=='Block' then
  model=o.model
  graphics=o.graphics;
  if model.sim(1)=='asuper' then  //
    gh_curwin=scf(curwin)
    o.model.sim='super';
    o.model.in=-ones(size(model.in,1),size(model.in,2))
    o.model.in2=-2*ones(size(model.in2,1),size(model.in2,2))
    o.model.out=-ones(size(model.out,1),size(model.out,2))
    o.model.out2=-2*ones(size(model.out2,1),size(model.out,2))
    o.model.intyp=-ones(1,size(model.intyp,'*'))
    o.model.outtyp=o.model.intyp
    o.graphics.exprs=graphics.exprs(1)
    needcompile=4;
   // needcompile=resume(needcompile)
  else
    messagebox('Remove Atomic can only be affected to Atomic Super Blocks.','modal');ok=%f; return
  end
else
  messagebox('Remove Atomic can only be affected to Atomic Super Blocks.','modal');ok=%f; return 
end
endfunction 
 
