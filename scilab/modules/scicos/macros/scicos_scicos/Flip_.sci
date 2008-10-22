//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//                      - Alan Layec <alan.layec@inria.fr>
//                      - Simone Mannori <simone.mannori@inria.fr>
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

function Flip_()
//** Alan/Simone 13/12/06 : Replot no more needed
   scs_m_save = scs_m    ; //** save the old diagram for "undo" operation 
   nc_save = needcompile ; //** save the old diagram state
   
   [%pt, scs_m] = do_tild(%pt, scs_m) ; //** do the "tilt" operation 
   
   Cmenu = []; //** 
   %pt   = [];
endfunction
