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

function Move_()

  Cmenu = []; //** NB : preserve %pt information 

  SelectSize = size (Select) ; //** [row, col]
  SelectSize = SelectSize(1) ; //**  row
  
  //** Filter out selection on other windows 
  if Select<>[] then
     if find(Select(:,2)<>curwin)<>[] then
       Select=[]
       Cmenu = "Move"
       return
     end
  end

  if  SelectSize == 1 & typeof(scs_m.objs(Select(1)))=="Link" then
  //** if ONE or NO object are selected 
     [scs_m] = do_stupidmove(%pt, Select,scs_m) ; //** Stupid is whos stupid does :)
                                           //** Select is not used here ....
  else //** multiple object are selected

      [scs_m] = do_stupidMultimove(%pt, Select, scs_m) ; //** move multiple objects
                //** %pt    : last valid user LEFT BUTTON PRESS
		//** Select : matrix of selected object
		//**  Each line is:  [object_id win_id] : "object_id" is the same INDEX used in "scs_m.obj"
		//**                                  and "win_id"    is the Scilab window id.
		//**  Multiple selection is permitted: each object is a line of the matrix.
		//** scs_m  : diagram datastucture
  end

  %pt = [];

endfunction
