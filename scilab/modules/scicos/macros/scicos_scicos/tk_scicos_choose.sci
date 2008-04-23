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

function result = tk_scicos_choose(mat,lab,but);
//** 17 April 2007: name changed from "tk_choose" to "tk_scicos_choose"
  result = 0
  
  l=list()
  for t=mat(:)'
    l($+1)=t,
  end
  Cmenu = mpopup(l);
  
  if Cmenu==[] then return, end
  
  result = find(Cmenu==mat)
  result = result(1)
endfunction

