//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//                      - Alan Layec <alan.layec@inria.fr>
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

function gr_k=get_gri(k,l)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

  //** 03/01/07 : Alan
  //** semi empirical equation :)
  //
  // input : k the index of the object in the
  //         not compiled scicos structure
  //        (scs_m.objs)
  //
  //        l the total number of graphic objects
  //        in the current graphical structure
  //        size(gh.children.children)
  //
  // output : gr_k the corresponding index of the
  //          not compiled element in the current
  //          graphical structure
  //

    gr_k = l - k + 1;

endfunction
