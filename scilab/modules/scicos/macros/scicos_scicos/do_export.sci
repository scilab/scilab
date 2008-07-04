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

function [wa, ha] = do_export(scs_m, fname, titleflag)
//** default call inside Export_.sci
//**
//**  do_export(scs_m) ;
//**
//**  rhs :=  1
// titleflag:0 or 1 (place or not place title in eps file)
// used only with fname

  [lhs, rhs] = argn(0) //** verify the presence of the arguments

  if rhs < 3 then //** "titleflag" not specified
    titleflag = 0
  end
  
  rect = dig_bound(scs_m);
  if rect == [] then
    return //** empty diagram
  end
  wa = rect(3) - rect(1)
  ha = rect(4) - rect(2)

  if ~exists('%scicos_lr_margin') then %scicos_lr_margin = 0.1; end
  if ~exists('%scicos_ud_margin') then %scicos_ud_margin = 0.1; end

  rect(1) = rect(1) - wa*%scicos_lr_margin
  rect(3) = rect(3) + wa*%scicos_lr_margin
  rect(2) = rect(2) - ha*%scicos_ud_margin
  rect(4) = rect(4) + ha*%scicos_ud_margin

  wa = rect(3) - rect(1) //** geometrical correction
  ha = rect(4) - rect(2)

  num = 1 //** the default is Postscript file output

  //**------------------------------------------------------------------
  if rhs == 1 then //** default "scs_m only" case, no filename has been

    num = x_choose( ..
     ['Postscript file'; ..
		  'Graphics window'], ..
      'How do you want to export?')

    //** Postscript file -> 1
    //** Graphics window -> 2
    //** [Cancel]        -> 0

    if num == 1 then //** Postscript
      // Ask for filename
      fname = xgetfile('*.EPS',title='Save Postcript file as...')
      // Exit if user cancelled
      if fname == "" then return; end
    end
  end

  if num == 0 then return; end //** EXIT point

  //** In both other cases (Postscript or graphics window), create a new window
  // to host diagram + title then get some useful handles
  gh_winc = scf(max(winsid()) + 1) 
  drawlater()
  winc = gh_winc.figure_id
  gh_axes = gh_winc.children

  if num == 1 then //** Postscript file

    if ~MSDOS then // remove blanks
      fname = stripblanks(fname)
      ff    = str2code(fname);
			ff(find(ff == 40 | ff == 53)) = 36;
			fname = code2str(ff);
      if fname == emptystr() then return; end
    end

  end

  //** Geometrical correction
  gh_axes.axes_bounds  = [0 0 1 1]                           //** use whole graphic window
  gh_axes.tight_limits = "on"                                //** set the limit "gh_axes.data_bounds" in "hard mode"
  mrect                = [rect(1) rect(2) ; rect(3) rect(4)] //** vector to matrix conversion
  gh_axes.data_bounds  = mrect                               //** default : data_bounds = [xmin ymin ; xmax ymax ]
  gh_axes.margins      = [0,0,0,0]                           //** no margins
  gh_winc.axes_size    = [wa ha]                             //figure_size = [wa ha] ;

  //  set options
  options = scs_m.props.options

  // **----------- set colormap -----------------------------------------
  // RN: this is not satisfactory. It uses the exact algorithm that in
  // set_cmap to make sure the same numbering is used. A common
  // function must be defined instead.
  cmap = options.Cmap
  d    = gh_winc.color_map
  for k = 1:size(cmap,1)
    [mc,kk] = mini(abs(d-ones(size(d,1),1)*cmap(k,:))*[1;1;1])
    if mc > .0001 then
      d = [d ; cmap(k,:)]
    end
  end
  ierr = execstr('set(gh_winc,'"color_map'",d)','errcatch')
  if ierr<>0 then
    warning("%s: Cannot allocate colormap", "do_export")
  end

  //**----------- Background handling -----------------------------------------
  if ~exists('%scicos_with_background') then %scicos_with_background = 1; end
  if ~%scicos_with_background then
    gh_axes.background = -2;
    gh_winc.background = -2;
  else
    if options.Background(1) <= size(gh_winc.color_map,1) then  //is
                                                                //background
                                                                //in colormap
      gh_axes.background = options.Background(1) ;
      gh_winc.background = options.Background(1) ;
    end
  end
  //**------------------------------------------------------------------------

  //** --------------------- Output ------------------------------------------

  //** In both cases, (Postscript output or graphical window only), draw title and replot diagram

  scf(gh_winc) //** put the focus in the graphics window
  %draw_title()
  drawobjs(scs_m, gcf())
  drawnow()

  if num == 1 then //** Postscript: export to EPS

		// Capture to EPS and delete figure
		try
    	xs2eps(winc, fname, 'landscape')
      delete(gh_winc)
		catch
			disp(lasterror)
		end

  end

endfunction

////////////////////////////////////////////////////////////////////////////////

function %draw_title()
  //** Put the title in the ouput window/file
  if ~(rhs==3 & titleflag==0) then
    width  = (rect(3)-rect(1))/3;
    height = (rect(4)-rect(2))/6;
    xstringb(rect(1)+width,rect(4)-height*(1+%scicos_ud_margin),scs_m.props.title(1),width,height,'fill')
  end
endfunction
