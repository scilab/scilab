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

function XcosMenuMove()

  Cmenu = []; //** NB : preserve %pt information 
  //** Filter out selection on other windows 
  Select=Select(find(Select(:,2)==curwin),:)
  if Select==[] then
    k = getobj(scs_m,%pt)
    if k<>[] then
       Select=[k,curwin]
    else
      Cmenu = "XcosMenuMove" //re-enter waitaing for a click
      return
    end
  end

  if  size(Select,1)==1 & typeof(scs_m.objs(Select(1,1)))=="Link" then
    //** ONE  link is selected (move a corner or add and move a corner)
    scs_m = do_stupidmove_corner(%pt, Select,scs_m) ; 
  else //** multiple object or single block move
    scs_m = do_stupidMultimove(%pt, Select, scs_m) ; //** move multiple objects
  end

  %pt = [];

endfunction
