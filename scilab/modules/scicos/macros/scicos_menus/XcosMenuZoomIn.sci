//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//                      - Alan Layec <alan.layec@inria.fr>

//                      - Ramine Nikoukhah <ramine.nikoukhah@inria.fr>
//                      - Simone Mannori <simone.mannori@inria.fr>
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

function XcosMenuZoomIn()
//** 12 Feb. 2006:  new graphic (Simone), grid insertion (Alan)
//**                geometrical correction (Ramine)
    Cmenu = [];
    xinfo("Zoom in");
    gh_window = gh_current_window; 
    //** Get the current postion of the visible part of graphics in the panner. 
    viewport  = gh_window.viewport; //** [x,y]
    zoomfactor = 1.2 ; 
    %zoom = %zoom * zoomfactor;

    //** geometrical correction: zoom in the center
    viewport = viewport * zoomfactor - 0.5 * gh_window.figure_size * (1 - zoomfactor)  ;
    //    viewport = max([0,0],min(viewport,-gh_window.figure_size +gh_window.axes_size)) 
    
    
    drawlater();
 
    window_set_size(gh_window, viewport);

    if exists('%scicos_with_grid') & %scicos_with_grid==%t then
      drawgrid(); //** set the new grid 
    end

    drawnow();
    xinfo(' ');
endfunction
