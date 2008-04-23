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

function SaveBlockGUI_()
  if size(find(Select(:,2)==curwin),2)<>1 then
     message('Select one and only one block in the current window.')
     Cmenu=[]
     return
  else
    if size(scs_m.objs)<Select(1) | typeof(scs_m.objs(Select(1)))<>"Block" then
       Select=[]
       return
    else
       Cmenu=[]
       if scs_m.objs(Select(1)).gui<>'DSUPER' then
          message('Only Masked blocks can be saved.')
       else
       	  fname=do_saveblockgui(scs_m.objs(Select(1)))
          if fname<>emptystr() then
            Scicos_commands=['%diagram_path_objective=[];%scicos_navig=1';
                             'exec('+sci2exp(fname)+');%diagram_path_objective='+sci2exp(super_path)+';%scicos_navig=1';
		            ]
          end
       end
    end
  end
endfunction

