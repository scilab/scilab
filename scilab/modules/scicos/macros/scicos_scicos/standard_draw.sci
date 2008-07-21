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

function standard_draw (o, frame, draw_ports, up)
//
//
//** This function is used to draw the object from the most of (but NOT all) the interface functions
//**
//** 21 Nov 2006 : some save/restore patch moved to upper level "drawobj.sci"
//**

  xf = 60
  yf = 40

  [lhs,rhs] = argn(0)

  if rhs == 1 then
      frame = %t
  end

  if rhs < 3 then
      draw_ports = standard_draw_ports     //** left right ports
                                           //** the function 'standard_draw_ports' it's copied in 'draw_ports'
  elseif rhs == 4 then                     //** otherwise
      draw_ports = standard_draw_ports_up  //** up / down ports
  end                                      //** the function 'standard_draw_ports_up' it's copied in 'draw_ports'

  nin    = size(o.model.in,    1);
  nout   = size(o.model.out,   1);
  clkin  = size(o.model.evtin, 1);
  clkout = size(o.model.evtout,1);
  [orig, sz, orient] = (o.graphics.orig, o.graphics.sz, o.graphics.flip)

//** Default values -------------------
  thick  = 1 //** patch Simone
  e      = 4
  With3D = options('3D')(1)
//** ----------------------------------

//** local handler (read/write) = semiglobalhandler (read/only)

  gr_i = o.graphics.gr_i

  if type(gr_i) == 15 then
    [gr_i, coli] = gr_i(1:2) ;
  else
    coli = [] ;
  end

  // draw box
  if frame then

    if With3D then  //** this is the code relative to the block's "window dressing"

      //**---------- 3D Mode ON ------------------------------------------------

      #Color3D = options('3D')(2)
      // xpoly([orig(1)+e;orig(1)+sz(1);orig(1)+sz(1)],[orig(2)+sz(2);orig(2)+sz(2);orig(2)+e],'lines')
      xrect(orig(1)+e, orig(2)+sz(2), sz(1)-e, sz(2)-e) ;
      gh_e            = gce() //** new graphics
      gh_e.thickness  = 0
      gh_e.foreground = #Color3D

      if coli<>[] then
        gh_e.fill_mode = "on"  ;
        gh_e.background = coli ;
      end

      xx = [ orig(1)   , orig(1)
             orig(1)   , orig(1)+sz(1)-e
             orig(1)+e , orig(1)+sz(1)
             orig(1)+e , orig(1)+e       ]

      yy = [ orig(2)         , orig(2)
	           orig(2)+sz(2)-e , orig(2)
	           orig(2)+sz(2)   , orig(2)+e
	           orig(2)+e       , orig(2)+e ]

      xfpolys(xx,yy,-[1,1]*#Color3D); //** fill a set of polygons
      gh_e            = gce()
      gh_c            = gh_e.children(1:2)
      gh_c.foreground = default_color(0)
      gh_c.thickness  = 2

    else //** not in 3D mode

      //**----------3D Mode OFF ------------------------------------------------

      e = 0
      xrect(orig(1),orig(2)+sz(2),sz(1),sz(2))
      gh_e = gce()
      gh_e.thickness = 2

      if coli <> [] then
          gh_e.fill_mode = "on"
          gh_e.background = coli
      end

    end //** of 3D mode ON/OFF

  end //** of the frame mode

  draw_ports(o) ; //** 'standard_draw_ports' or 'standard_draw_ports_up'

//** ---------------------------------------------------------------------------
//** - scs_m , %cpr Indexes Show including information on internal superblocks -

//** quick and dirty patch to show the scs_s and %cpr indexes
if exists('%scicos_with_grid') then
  if %scicos_with_grid then
    scs_m_i=[]; cpr_i=[] ; sflag=[] ;
    gh_curwin = gh_current_window     ; //** get the handle of the current graphics window
    win_id = gh_curwin.figure_id

    kc = find(win_id==windows(:,2))

    pal = windows(kc,1);

    if pal > 0 then
      if ~exists('scs_m_index') then scs_m_index=-1, end
      [scs_m_i, cpr_i, sflag] = get2index(o, scs_m_index);

      txt_index = [];
       if scs_m_i<>[] & cpr_i<>[] then
          if sflag<>[] then
            txt_index = string(scs_m_i)+"s, "+cpr_i+"c";
          else
            txt_index = string(scs_m_i)+"s, "+string(cpr_i)+"c";
         end
       end

      gh_axes = gca(); //** get the Axes proprieties

      // draw indexes
      if txt_index<>[] then
        //** Save font state
        axes_font_style = gh_axes.font_style ;
        axes_font_size  = gh_axes.font_size  ;
        //** axes_font_color = gh_axes.font_color ; //** optional

        gh_axes.font_style = options.ID(1)(1) ;
        gh_axes.font_size  = options.ID(1)(2) ;
        //** font color not yet used
        rectangle = xstringl(orig(1), orig(2), txt_index) ;
        w = max(rectangle(3), sz(1)) ;
        h = rectangle(4) * 0.5 ;
        xstringb(orig(1) + sz(1) / 2 - w / 2, orig(2) + sz(2) + h , txt_index , w, h) ;

        //** Restore font state
        gh_axes.font_style = axes_font_style ;
        gh_axes.font_size  = axes_font_size  ;
        //** gh_axes.font_color = axes_font_color ; //** optional
      end
    end
  end
end //** of on/off control

//** -------------------------------- Identification ---------------------------

  ident = o.graphics.id

  gh_axes = gca(); //** get the Axes proprieties

  // draw Identification
  if ident<>[] & ident<>'' then

      //** Save font state
      axes_font_style = gh_axes.font_style ;
      axes_font_size  = gh_axes.font_size  ;
      //** axes_font_color = gh_axes.font_color ; //** optional

      gh_axes.font_style = options.ID(1)(1) ;
      gh_axes.font_size  = options.ID(1)(2) ;
      //** font color not yet used
      rectangle = xstringl(orig(1), orig(2), ident) ;
      w = max(rectangle(3), sz(1)) ;
      h = rectangle(4) * 1.3 ;
      xstringb(orig(1) + sz(1) / 2 - w / 2, orig(2) - h , ident , w, h) ;

     //** Restore font state
     gh_axes.font_style = axes_font_style ;
     gh_axes.font_size  = axes_font_size  ;
     //** gh_axes.font_color = axes_font_color ; //** optional

  end
//** --------------------------- Identification End ----------------------------

  deff('c=scs_color(c)',' ') //** on line function definition

  gr_i = [ gr_i ] //** simple but not simpler :)

  model = o.model

  if With3D & frame then
    orig = orig + e ;
    sz   = sz - e   ;
  end

  //** Check of the graphics object has generated any error
  if execstr(gr_i,'errcatch','n')<>0 then
    message(['Error in Icon defintion'; lasterror()])
  end

endfunction

