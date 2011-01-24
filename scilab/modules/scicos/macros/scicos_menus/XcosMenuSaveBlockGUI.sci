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

function XcosMenuSaveBlockGUI()
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

//** 25/06/2009 : Serge Steer, 
//   -fix a bug un case there are selected objects in other windows  
  K=find(Select(:,2)==curwin)
  if size(K,'*')<>1 then
     messagebox('Select one and only one block in the current window.','modal')
     Cmenu=[]
     return
  else
    if size(scs_m.objs)<Select(K,1) | typeof(scs_m.objs(Select(K,1)))<>"Block" then
       Select=[]
       return
    else
       Cmenu=[]
       if scs_m.objs(Select(K,1)).gui<>'DSUPER' then
          messagebox('Only Masked blocks can be saved.','modal')
       else
       	  fname=do_saveblockgui(scs_m.objs(Select(K,1)))
          if fname<>emptystr() then
            Scicos_commands=['%diagram_path_objective=[];%scicos_navig=1';
                             'exec('+sci2exp(fname)+');%diagram_path_objective='+sci2exp(super_path)+';%scicos_navig=1';
		            ]
          end
       end
    end
  end
endfunction

