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

function XcosMenuReplot()

    Cmenu = [] ;
    %pt   = [] ;
    Select= [] ;
    
    //** BEWARE: this assignement can be erroneus because the user can destroy manually
    //**         the Scicos window.
    //**         How to reproduce the error: 
    //**         1) launch "scicos();"
    //**         2) use "Tools->Calc" ; look at the scilex window
    //**         3) close manually the Scicos window (click over [X])
    //**         4) type "resume" on the scilex window  
    
    gh_curwin = scf(gh_current_window) ; //** get the handle of the current graphics window
    gh_axes = gca();   
    
    drawlater();

       if gh_axes.children<>[] then   //** protection : you cannot delete "non existant" "empty" object ! 
           delete(gh_axes.children) ; //** clear the current graphic window: OBJECTS ONLY ! NO Menus
       end

       %wdm = scs_m.props.wpar    ; //** 
       window_set_size(gh_curwin) ; //** OK
       set_background(gh_curwin)  ; //** OK
       drawobjs(scs_m,gh_curwin)  ;  //** Redraw all
    
endfunction
