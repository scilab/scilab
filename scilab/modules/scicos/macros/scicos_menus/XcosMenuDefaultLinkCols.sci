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

function XcosMenuDefaultLinkCols
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

//
  Cmenu = []

  [edited, options]   = do_options(scs_m.props.options, "LinkColor")
  scs_m.props.options = options

  if edited then

    //** Acquire the current clicked window
    gh_curwin = scf(%win)
    gh_axes = gca()

    //** Clear the graphics window WITHOUT changing his parameters ! :)
    drawlater()

    //** Clear any existing graphic objects inside the axes
    if ~isempty(gh_axes.children)      
      delete(gh_axes.children)
    end
    drawobjs(scs_m, gh_curwin) //** Re-draw all the graphics object

    Cmenu = [], %pt = []
  end

endfunction

