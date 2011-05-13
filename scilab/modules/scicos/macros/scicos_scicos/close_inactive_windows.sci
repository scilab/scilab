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

function inactive_windows=close_inactive_windows(inactive_windows,path)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

//close all inactive windows assciated with children of Super Block path
  DELL=[]  // inactive windows to kill
  if size(inactive_windows(2),'*')>0 then
    n=size(path,'*');
    mainopen=or(curwin==winsid()) // is current window open
    if mainopen then gh_save=gcf(),end
    for kk=1:size(inactive_windows(2),'*')
      if size(inactive_windows(1)(kk),'*')>n & isequal(inactive_windows(1)(kk)(1:n),path) then
	DELL=[DELL kk];
	win=inactive_windows(2)(kk)
	if or(win==winsid()) then
	  gh_del = scf(win) ; 
	  delete (gh_del)
	end
      end
    end
    if mainopen then scf(gh_save),end
  end
  for kk=DELL($:-1:1)  // backward to keep indices valid
    inactive_windows(1)(kk)=null()
    inactive_windows(2)(kk)=[]
  end
endfunction
