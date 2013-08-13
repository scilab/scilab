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
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//

function scs_show(scs_m, win)
    //**
    //** 24 Nov 2006: Super simplified version
    //**

    if %scicos_debug_gr then
        disp(_("scs_show active..."))
    end

    gh_curwin = scf(win) ;

    if ~set_cmap(scs_m.props.options("Cmap")) then // add colors if required
        scs_m.props.options("3D")(1)=%f //disable 3D block shape
    end

    options = scs_m.props.options ; //** not used

    set_background(gh_curwin)     ;
    pwindow_set_size(gh_curwin)   ;
    window_set_size(gh_curwin)    ;

    scs_m.props.title(1) = msprintf(_("Scilab Graphics of %s"),scs_m.props.title(1))

    drawobjs(scs_m, gh_curwin)

endfunction
