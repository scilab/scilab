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

function Addcolor_()
//** 
    [edited,options] = do_options(scs_m.props.options,'Cmap')

    if edited then
      scs_m.props.options = options ;
      set_cmap(scs_m.props.options('Cmap')) ;
      set_background() ;
      //** Acquire the current clicked window handles
      gh_curwin = scf(%win) ;
      gh_axes = gca(); 
      //** Clear the graphic window WITHOUT changing his parameters ! :)
      drawlater() ;
        delete(gh_axes.children) ;   //** wipe out all the graphics object in the axe 
        drawobjs(scs_m, gh_curwin) ; //** re-create all the graphics object
      //** drawnow(); //** re-draw the graphic object and show on screen (now included in drawobjs)
      //** show_pixmap() ; //** not useful on Scilab 5      
     
      Cmenu = [] ; %pt = [];sx

    else
      Cmenu = [] ; %pt = [];
    end

endfunction
