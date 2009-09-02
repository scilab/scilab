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

function XcosMenuNew()

    Cmenu = [] ; //** clear previous command 
    
    %r = 2     ; //** preload the default answer
    if edited then
      %r = messagebox(["Diagram has not been saved";
	            "Are you sure you want to proceed ?"],'modal',['No';'Yes'])
    else 
      %r = 2 ;
    end             //** No -> %r = 1 ; Yes -> %r = 2  
    
    if %r==2 then
      inactive_windows=close_inactive_windows(inactive_windows,super_path)
      if alreadyran then
          do_terminate() ; // terminate current simulation
      end  
      clear('%scicos_solver')  // avoids forcing implicit compilation in
                           // some situations 
      alreadyran = %f ; 
      scs_m = scicos_diagram(version=get_scicos_version()) ; //** create a empty diagram 
      %wpar = scs_m.props ; 
      %zoom  = 1.4        ; //** original value by Ramine
      Cmenu  = "XcosMenuReplot"   ; //** force a Replot of the New (empty) diagram
      edited = %f         ; //** not edited    
    end
endfunction
