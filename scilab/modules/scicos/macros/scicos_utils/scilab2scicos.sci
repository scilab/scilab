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
// See the file ./license.txt
//

function scilab2scicos(win,x,y,ibut)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

  //utility function for the return to scicos by event handler
  if ibut==-1000|ibut==-1 then return,end
  ierr=execstr('load(TMPDIR+''/AllWindows'')','errcatch')
  if ierr==0 then
    for win_i= AllWindows
      scf(win_i)
      seteventhandler('')
    end
  end
  scicos();
  [txt,files]=returntoscilab()
  n=size(files,1)
  for i=1:n
    load(TMPDIR+'/Workspace/'+files(i))
    execstr(files(i)+'=struct('"values'",x,'"time'",t)')
  end
  execstr(txt)
endfunction

