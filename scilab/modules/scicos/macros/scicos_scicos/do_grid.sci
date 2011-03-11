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

function [%scs_wgrid]=do_grid(%scs_wgrid)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")


// ** 22/06/2009 Serge Steer INRIA
// - gh_window.children replaced by gh_axes because of uimenu which are
//   children of the figure as well as axes
  exprs = [string(%scs_wgrid(1)),string(%scs_wgrid(2)),string(%scs_wgrid(3))]
  while %t do
    [ok,b1,b2,colorr,exprs]=scicos_getvalue(['Set Grid'],..
				     ['x','y','color'],list('vec',1,'vec',1,'vec',1),exprs)
    if ~ok then
      break
    else
      %scs_wgrid(1)=b1
      %scs_wgrid(2)=b2
      %scs_wgrid(3)=colorr
      drawlater()
      drawgrid();
      drawnow();
      break
    end
  end
endfunction
