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

function [wa, ha] = do_export(scs_m, fname, titleflag, exp_format)
//** default call inside Export_.sci
//**
//**   do_export(scs_m);
//**
//** => rhs = 1
//
// other input arguments used only with fname:
//  - titleflag:  0 or 1 (place or not place title in eps file)
//  - exp_format: export format ('EPS', 'PNG', ...)

  [lhs, rhs] = argn(0) //** verify the presence of the arguments

  if rhs < 4 then //** "exp_format" not specified => default is 'eps'
    exp_format = 'eps'
  end
  
  if rhs < 3 then //** "titleflag" not specified => default is '0'
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
  if rhs == 1 then //** default "scs_m only" case, no filename has been given

  
    // Ask for format
    exp_format=choose_export_format();
    if isempty(exp_format) then // User cancelled
      return
    end
    // Ask for filename
    format_mask = '*.' + exp_format // for example: '*.png' or '*.eps'
    dialog_title = 'Save ' + exp_format + ' file as...'
    fname = getfile(format_mask, title = dialog_title)
      
    // Exit if user cancelled
    if fname == "" then return; end

  end

  // In both export cases (file or graphics window), create a new window
  // to host diagram + title then get some useful handles
  gh_winc = scf(max(winsid()) + 1) 
  drawlater()
  winc = gh_winc.figure_id
  gh_axes = gh_winc.children



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
    [mc,kk] = min(abs(d-ones(size(d,1),1)*cmap(k,:))*[1;1;1])
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
    if options.Background(1) <= size(gh_winc.color_map,1) then  //is background n colormap
      gh_axes.background = options.Background(1) ;
      gh_winc.background = options.Background(1) ;
    end
  end
  //**------------------------------------------------------------------------

  //** --------------------- Output ------------------------------------------

  // In both cases (file output or graphical window), draw title and replot diagram
  
  scf(gh_winc) //** Give focus to the graphics window
  
  //** Put the title in the ouput window/file
  if titleflag then
    width  = (rect(3) - rect(1))/6
    height = (rect(4) - rect(2))/12
    xstringb( ..
	      rect(1) + width, ..
	      rect(4) - height*(1 + %scicos_ud_margin), ..
	      scs_m.props.title(1), ..
	      width, ..
	      height, ..
	      'fill')
  end
  drawobjs(scs_m, gcf())
  drawnow()
  lf=length(fname)
  if part(fname,lf-length(exp_format):lf)<>'.'+exp_format then 
    fname=fname+'.'+exp_format
  end
  if exp_format =='scg' then
    //scilab binary graphic format
    cmd="save(fname,gca())"
  elseif or(exp_format == ["emf";"eps";"fig";"pdf";"ps";"svg" ])
    cmd = 'xs2' + exp_format + '(winc, fname, ''landscape'')'
  else
    cmd = 'xs2' + exp_format + '(winc, fname)'
  end
    
  try
    execstr(cmd)
  catch
    messagebox([msprintf(_("Export in file %s failed\n"),fname);lasterror()],"error","modal")
    ok=%f
  end

  delete(gh_winc)

endfunction

