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

function XcosMenuAlign()
//** Align unconnected blocks 
//** 24/06/2009: Serge Steer  not to take care of selected blocs  

  Cmenu = [] ;
  if find(Select(:,2)==curwin)<>[] then  %pt  = []  ;end

  scs_m_save = scs_m;
  nc_save    = needcompile;
  [scs_m]    = prt_align(scs_m); 

endfunction
