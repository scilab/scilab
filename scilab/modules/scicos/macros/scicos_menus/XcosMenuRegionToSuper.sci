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

function XcosMenuRegionToSuper()
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")


//** Very complex and critical functions inside : handle with care ;)     

//** BEWARE: both functions ('do_region2block' and 'do_select2block' contains some 
//** completely un useful drawobj() operations inside the 'do_delete2' sub function
//** For the moment the drawobj() are not executed because this function ia called
//** with the options %f that disable the graphics update.
//** In all the cases the the situation is restored by the final "Replot" operation.
//
  Cmenu=[]
   
  if Select==[] then 
    if (%win <> curwin) then
      return// only in main window
    end
    
    //** if nothing is selected -> call a region selection function 
    [%pt, scs_m] = do_region2block(%pt,scs_m); //** see file: "do_region2block.sci"
	      //** this type of operations is OK (old Scicos code here :)                                  
  else
    
    if Select(1,2)<>curwin then
      //** if the selected object are not in the current window ... exit  
      return ; //** --> Exit point 
    end
    
    //** if the selected object are in the current window use the function below in this file 
    [%pt, scs_m] = do_select2block(%pt, scs_m);  //** ---> see below in this file :)
	//** with the last bugfix the 'Text' object are correctly handled, but this routine is
	//** much slower that the "old" standard mode.     
  end
  
  Cmenu='XcosMenuReplot'; %pt=[];

endfunction


