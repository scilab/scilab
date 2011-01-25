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

function XcosMenuCompile()
    nc_save = 4  ;
    Cmenu   = [] ;
    needcompile = 4; 
    
    [%cpr, ok] = do_compile(scs_m)
    
    if ok then
      newparameters = list(); 
      %tcur = 0; //** current simulation time
      alreadyran = %f ; 
      %state0    = %cpr.state;
      needcompile= 0  ;
    end
endfunction
