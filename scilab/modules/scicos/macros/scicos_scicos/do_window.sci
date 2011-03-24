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

function wpar=do_window(wpar)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

  wd=wpar.wpar;w=wd(1);h=wd(2);

  while %t do
    [ok,h,w]=scicos_getvalue('Set parameters',[
	'Window height';
	'Window width'],list('vec',1,'vec',1),string([h;w]))
    if ~ok then break,end
    if or([h,w]<=0) then
      message('Parameters must be positive')
    else
      drawtitle(wpar)
      wpar.wpar(1)=w
      wpar.wpar(2)=h
      break
    end
  end
endfunction
