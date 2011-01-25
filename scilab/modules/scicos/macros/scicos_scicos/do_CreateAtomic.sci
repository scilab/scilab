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
function [o,needcompile,ok]=do_CreateAtomic(o,k,scs_m)
ok=%t

model=o.model
graphics=o.graphics;

XX = o;
[params,param_types]=FindSBParams(model.rpar,[])
if params<>[] then
  messagebox(msprintf(_("The parameter(s) ""%s"" \nmust be defined in the"+...
		       " context of the atomic super block."),strcat(params,', ')),"modal")
  return;
end
ALL=%f;
xx=XcosMenuCodeGeneration;
[ok, XX, gui_path,flgcdgen, szclkINTemp, freof,c_atomic_code] = ...
    do_compile_superblock42(scs_m, k, %t); 
if ~ok then return; end

if freof <> [] then 
  messagebox(_("An Atomic Superblock cannot contain sample clocks"),'modal')
  return
end
//**quick fix for sblock that contains scope
gh_curwin=scf(curwin)
o.model.sim=list('asuper',2004);
o.model.in=XX.model.in
o.model.in2=XX.model.in2
o.model.out=XX.model.out
o.model.out2=XX.model.out2
o.model.intyp=XX.model.intyp
o.model.outtyp=XX.model.outtyp
XX.model.sim(2)=2004;
o.graphics.exprs=list(XX.graphics.exprs,c_atomic_code,XX.model);
needcompile=4;
[a,b]=c_link(XX.model.sim(1))
while a do
  ulink(b)
  [a,b]=c_link(XX.model.sim(1))
end
// needcompile=resume(needcompile)
endfunction
