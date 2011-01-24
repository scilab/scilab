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

function XcosMenuUndo()
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")


    Cmenu = [] ;
    %pt = [];

    if enable_undo==2 then  // open SB's may have been affected
      %r=2
      %r=messagebox(['Undo operation undoes changes in a subdiagram';
                  'These changes will be lost for ever'], 'modal', ['Proceed'; ...
		    'Cancel'])
      if %r==2 then 
         return,
      else
         scs_m = scs_m_save ; //** recover the previous diagram
	 Select = [] ; //** unselect object  
         needcompile = nc_save ;
         supers=findopenchildren(scs_m,super_path,list(super_path))
         Scicos_commands=[]
         for i=1:size(supers)
            Scicos_commands=[Scicos_commands;
         	 '%diagram_path_objective='+sci2exp(supers(i))+';%scicos_navig=1';
                 'Cmenu='"XcosMenuReplot'"']
         end
         enable_undo = %f ; //** "Undo" is a single level function 
      end
    
    elseif enable_undo then
         scs_m = scs_m_save ; //** recover the previous diagram
	 Select = [] ; //** unselect object  
         needcompile = nc_save ;
         Cmenu='XcosMenuReplot'     
         enable_undo = %f ; //** "Undo" is a single level function 
    else
      messagebox('No more undo available.','modal')
    end
    
endfunction
