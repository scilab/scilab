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
function k=get_objs(gr_k,l)
// Copyright INRIA
  //** 21/11/08 : Alan
  //** semi empirical equation :)
  //
  // input : gr_k the corresponding index of the
  //          not compiled element in the current
  //          graphical structure
  //
  //        l the total number of graphic objects
  //        in the current graphical structure
  //        size(gh.children.children)
  //
  // output : k the index of the object in the
  //         not compiled scicos structure 
  //        (scs_m.objs)
  //

    k = l - gr_k + 1;

endfunction
