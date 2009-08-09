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
function CreateAtomic_()
// Copyright INRIA
if alreadyran then
  Scicos_commands=['%diagram_path_objective=[];%scicos_navig=1';
      '[alreadyran,%cpr]=do_terminate();%diagram_path_objective='+sci2exp(super_path)+';%scicos_navig=1';
      'Select='+sci2exp(Select)+';Cmenu='"Create Atomic'"';]
  //return
else
  Cmenu=[];%pt=[];
  if size(Select,1)<>1 | curwin<>Select(1,2) then
    return
  end
  i=Select(1)
  o=scs_m.objs(i)
  if typeof(o)=='Block' then
    if o.model.sim=='super' then
      [o,needcompile,ok]=do_CreateAtomic(o,i,scs_m)
      if ~ok then messagebox('Error in Creating Atomic','modal');return ;end
      scs_m = update_redraw_obj(scs_m,list('objs',i),o)
    else
      messagebox('Atomic can only be affected to Non Atomic Super Blocks.','modal');
    end
  else
    messagebox('Atomic can only be affected to Non Atomic Super Blocks.','modal');
  end
end
endfunction
