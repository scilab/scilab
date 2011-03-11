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

function XcosMenuSaveAs()
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

  Cmenu=[]

  //** Inside a super block 
  if super_block then
    r = x_choose(['Diagram';'Super Block'],..
	         ['Save content of the Super Block or'
		  'the complete diagram?'],'Cancel')
    if r==0 then 
      return
    end
    if r==1 then
      Scicos_commands=['%diagram_path_objective=[];%scicos_navig=1';
		       'Cmenu='"Save As'";%scicos_navig=[]';
		       '%diagram_path_objective='+sci2exp(super_path)+';%scicos_navig=1';
		      ]
      return
    end
  end

  //** From the main diagram 
  [scs_m,editedx] = do_SaveAs(); 

  if ~super_block then
     edited = editedx
  end

endfunction
