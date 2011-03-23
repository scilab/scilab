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

function systexport(scs_m, exp_dir, exp_format)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")


  do_export(scs_m, fullfile(exp_dir, scs_m.props.title(1)), 0, exp_format) 
  
  for k = 1:lstsize(scs_m.objs)
    o = scs_m.objs(k)
    if typeof(o) == 'Block' then
      model = o.model
      if model.sim == 'super' then
        systexport(model.rpar, exp_dir, exp_format)
      end
    end
  end

endfunction
