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

function update_gr(gr_k,o)
  gh_blk = drawobj(o) ; //** new object in gh_curwin.children.children(1)
  gh_blk.children(1).mark_mode = ...   //** for Select
      gh_curwin.children.children(gr_k+1).children(1).mark_mode;
  swap_handles(gh_curwin.children.children(gr_k+1),... //** update the object in the old position
               gh_curwin.children.children(1));
  delete(gh_curwin.children.children(1));
endfunction
