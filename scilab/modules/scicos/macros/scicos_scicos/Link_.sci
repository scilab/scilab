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

function Link_()

//** 28 Nov 2008 : begin with the "Simulink like" (SL) mode 
  
  //** "0" standard scicos oblique link ; "1" SL orthogonanal links 
  global SL_mode ; 

  //** use a global variable in oder to switch 
  //** from/to the two operating modes 
  if SL_mode==0 then 
    //** This is the original Scicos routine
    [scs_m, needcompile] = getlink(%pt, scs_m, needcompile); 
  else
  //** This is the new mode "getlink_qd"
    [scs_m, needcompile] = getlink_qd(%pt, scs_m, needcompile);
  end 

  Cmenu = []; 
  %pt = []; 
  Select = [] ;

endfunction
