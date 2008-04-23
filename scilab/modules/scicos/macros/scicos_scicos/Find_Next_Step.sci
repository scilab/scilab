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

function [Cmenu,Select]=Find_Next_Step(path_objective,path)
i=length(path_objective)
j=length(path)
m=min(i,j)
k=min(find(path_objective(1:m)<>path(1:m)))
if k==[] then
  if i<j then 
    Cmenu='Quit'
  elseif j<i then
    Cmenu='OpenSet'
    Select=[path_objective(j+1),curwin]
  else
    disp('pas possible'),pause
  end
else
  Cmenu='Quit'
end
endfunction

