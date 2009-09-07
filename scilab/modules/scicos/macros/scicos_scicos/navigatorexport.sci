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
function ok=navigatorexport(scs_m, exp_dir,exp_format)
  fname = fullfile(%exp_dir, 'navigator')

  // Create a new window to host diagram and title, then get some useful handles
  win_nag = scf(max(winsid()) + 1)
  drawlater()
  gh_axes  = gca()
  gh_navig = gcf()

  y0 = 0
  x0 = 0
  path = []
  larg = 0
  [xx, yy, lp] = build_scs_tree(scs_m)

  n_node = size(lp)
  mnx = min(xx)-0.1
  mxx = max(xx)+0.1

  if n_node <= 20 then
    dx  = (mxx - mnx)
    mnx = mnx - dx/5
    x0  = [mnx, mnx + dx/5]
  else
    dx  = (mxx - mnx)
    mnx = mnx - dx/2
    x0  = [mnx, mnx + dx/4, mnx + dx/2]
  end

  //** axes settings
  gh_navig_axes = gh_navig.children   //** axes handle
  gh_navig_axes.tight_limits = "on"   //** set the limit "gh_axes.data_bounds" in "hard mode"

  arect = [ 0.125 0.125 0.125 0.125 ] //** margins (default normalized values)
  gh_navig_axes.margins = arect

  wrect = [ -1/6 -1/6 8/6 8/6 ]
  gh_navig_axes.axes_bounds = wrect   //** default : axes_bounds = [0 0 1 1] = [xmin ymin with height]

  //** map the diagram size on the window size
  //**    (1,1) (1,2)         (2,1) (2,2)
  bnds = [mnx,  min(yy)-0.2 ; mxx,  max(yy)+0.2]
  gh_navig_axes.data_bounds = bnds    //** default : data_bounds = [0 0 ; 1 1] = [xmin ymin ; xmax ymax]

  // draw tree
  xsegs(xx, yy, 1) //** draw unconnected segments

  xx = [xx(1,1) ; xx(2,:)']
  yy = [yy(1,1) ; yy(2,:)']

  plot2d(xx, yy, -9, '000')

  xtitle(_("Navigator window"))

  for k = 1:size(xx,1)
    xstring(xx(k), yy(k), string(k))
  end

  r  = xstringl(x0(1), y0, 'X')
  h  = r(4)
  y0 = bnds(2,2) - h
  kx = 1

  xrect(x0(kx), bnds(2,2), x0(kx+1) - x0(kx), bnds(2,2) - bnds(1,2))

  for k = 1:size(xx,1)

    if k == 1 then
      path = []
    else
      path = lp(k-1)
    end

    Path = list()

    for pk = path
      Path = lstcat(Path,'objs',pk,'model','rpar')
    end

    Path = lstcat(Path,'props','title',1)

    xstring(x0(kx), y0, string(k) + ': ' + scs_m(Path))

    y0 = y0 - h

    if k == 20 then
      y0 = bnds(2,2) - h
      kx = kx + 1
      xrect(x0(kx), bnds(2,2), x0(kx+1) - x0(kx), bnds(2,2) - bnds(1,2))
    end

  end

  drawnow()
  fname = pathconvert(fname, %f, %t)
 
  
  
  lf=length(fname)
  if part(fname,lf-length(exp_format):lf)<>'.'+exp_format then 
    fname=fname+'.'+exp_format
  end

  if exp_format =='scg' then
    //scilab binary graphic format
    cmd="save(fname,gca())"
  elseif or(exp_format == ["emf";"eps";"fig";"pdf";"ps";"svg" ])
    cmd = 'xs2' + exp_format + '(win_nag.figure_id, fname, ''landscape'')'
  else
    cmd = 'xs2' + exp_format + '(win_nag.figure_id, fname)'
  end
    
  try
    execstr(cmd)
  catch
    messagebox([msprintf(_("Export in file %s failed\n"),fname);lasterror()],"error","modal")
    ok=%f
  end

  
  delete(win_nag)
endfunction
