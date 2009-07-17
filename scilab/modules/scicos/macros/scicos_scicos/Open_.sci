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

function Open_()
//
//
//** Open a saved diagram

  Cmenu  = []  ;
  Select = []  ;  //** empty
  //** Select_back = [] ; //** empty
  //** %ppt = []; //** used to store last valid click position for "Paste" operation
  //** Clipboard = []; //** used in Copy Cut and Paste function

  gh_curwin = scf(curwin);

  if edited & ~super_block then //** if is edited and is NOT a superblock
    num = messagebox(["Diagram has not been saved"],"modal","question",['OK','Go Back'])
    if num==2 then
      return ; //** if "Go Back" then EXIT
    end

   if alreadyran then
     do_terminate() ; // terminate current simulation
   end

    clear('%scicos_solver')  // avoids forcing implicit compilation in some situations

    alreadyran = %f ;

  end

  inactive_windows = close_inactive_windows(inactive_windows, super_path);

  xselect();

  [ok, scs_m, %cpr, edited] = do_load(); //** this is the function that really load the diagram

  if super_block then
    edited = %t;
  end

  if ok then
    if ~set_cmap(scs_m.props.options('Cmap')) then
      scs_m.props.options('3D')(1) = %f; // disable 3D block shape
    end

    options = scs_m.props.options ;

    set_background();

    if size(scs_m.props.wpar,'*')>12 then

      winsize = scs_m.props.wpar(9:10)
      winpos  = scs_m.props.wpar(11:12)
      //-- screensz = evstr(TCL_EvalStr('wm  maxsize .'))

      screensz = get(0, "screensize_px");
      screensz = screensz(3:4);
      if min(winsize)>0  then  // window is not iconified
         winpos = max(0,winpos-max(0,-screensz+winpos+winsize) )
         scs_m;  // only used locally, does not affect the real scs_m
         scs_m.props.wpar(11:12) = winpos  // make sure window remains inside
      end

      %zoom = scs_m.props.wpar(13)
      pwindow_read_size(gh_curwin) ;
      window_read_size(gh_curwin)  ;
    else
      pwindow_set_size(gh_curwin);
      window_set_size(gh_curwin);
    end

    //** -----------------

    //** ------------------------ Context evaluation --------------------------
    if type(scs_m.props.context)==10 then

      %now_win = get("current_figure");

      [%scicos_context,ierr] = script2var(scs_m.props.context, %scicos_context);

      //** for backward compatibility for scifunc
      if ierr==0 then
	%mm = getfield(1,%scicos_context)
	for %mi=%mm(3:$)
	  ierr = execstr(%mi+'=%scicos_context(%mi)','errcatch')
	  if ierr<>0 then
	    break
	  end
	end //** of for
      end
      //** end of for backward compatibility for scifunc

      if ierr<>0 then
	message(["Error occur when evaluating context:" lasterror()])
      else
	deff('%fonct()',scs_m.props.context)
	%outfun = macrovar(%fonct);
	clear('%fonct')
	// perform eval only if context contains functions which may give
	// different results from one execution to next
	if or(%outfun(4)=='rand')|or(%outfun(4)=='exec')|or(%outfun(4)=='load') then

	  [scs_m, %cpr, needcompile, ok] = do_eval(scs_m, %cpr) ;

	end
      end

      set("current_figure", %now_win);

    else
      scs_m.props.context = ' '; //** put the context to void
    end
    //**--------------

    gh_axes = gca();
    if gh_axes.children<>[] then
      drawlater();
      delete(gh_axes.children); //** delete all the object in the window
    end
    drawobjs(scs_m) ; //** draw all the objects in the diagram data structure

    if size(%cpr)==0 then
      needcompile = 4 ;
      alreadyran = %f ;
    else
      %state0 = %cpr.state ;
      needcompile = 0 ;
      alreadyran = %f ;
    end

  end

endfunction

