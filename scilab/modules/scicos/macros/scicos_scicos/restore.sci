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

function %zoom = restore(gh_curwin)
// Copyright INRIA
  gh_current_window = gh_curwin ;
  gh_curwin.pixmap = "off"

  if ~set_cmap(scs_m.props.options('Cmap')) then // add colors if required
    scs_m.props.options('3D')(1)=%f //disable 3D block shape
  end

  set_background(gh_curwin)   ;

  if size(scs_m.props.wpar,'*')>12 then

    winsize  = scs_m.props.wpar(9:10)  ;
    winpos   = scs_m.props.wpar(11:12) ;
     //-- screensz = evstr(TCL_EvalStr('wm  maxsize .'))

      screensz = get(0, "screensize_px");
      screensz = screensz(3:4);
    if min(winsize)>0  then  // window is not iconified
      winpos = max(0,winpos-max(0,-screensz+winpos+winsize) ) ;
      scs_m;  // only used locally, does not affect the real scs_m
      scs_m.props.wpar(11:12) = winpos  // make sure window remains inside screen
    end

    %zoom = scs_m.props.wpar(13) ;
    pwindow_read_size(gh_curwin) ;
    window_read_size(gh_curwin)  ;
  else
    pwindow_set_size(gh_curwin) ;
    window_set_size(gh_curwin)  ;
  end

  menu_stuff() ;

  if ~super_block then
    delmenu(curwin,'Stop')
    //** Bruno/Simone
    //** the Devil is hidden in the follwing line
    //** "haltscicos" is just a Scilab dummy function that does
    //** almost nothing ;)
    addmenu(curwin,'Stop',list(2,'haltscicos'));
    //**
    unsetmenu(curwin,'Stop')
  else
    unsetmenu(curwin,'Simulate')
  end
endfunction
