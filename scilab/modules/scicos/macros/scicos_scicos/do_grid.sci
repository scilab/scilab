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
 exprs = [string(%scs_wgrid(1)),string(%scs_wgrid(2)),string(%scs_wgrid(3))]
 while %t do
    [ok,b1,b2,colorr,exprs]=getvalue(['Set Grid'],..
	    ['x','y','color'],list('vec',1,'vec',1,'vec',1),exprs)
    if ~ok then
     break
    else
     %scs_wgrid(1)=b1
     %scs_wgrid(2)=b2
     %scs_wgrid(3)=colorr
     drawgrid();
     
     gh_window = scf(%win)
     
     swap_handles(gh_window.children.children($),...
                  gh_window.children.children(1));
     delete(gh_window.children.children(1));
     drawnow();
     //** show_pixmap() ; //** not useful on Scilab 5
     break
    end
  end
endfunction
