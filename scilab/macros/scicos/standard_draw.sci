//** 19 Jan 2006
//** Comments and mods by Simone Mannori 
//
// Copyright INRIA

//** This function needs careful attention because contains a lots of graphics
//** instructions 

//** This function draw the most of the graphics blocks 


function standard_draw (o,frame,draw_ports,up)

//** ---------- Tracing ------------
 // disp("*standard_draw(..)");     //
//** -------------------------------
  
  
  xf = 60 ; yf = 40 ;
  
  [lhs,rhs] = argn(0)

  if rhs==1 then
      frame=%t
  end

  if rhs<3 then
      draw_ports = standard_draw_ports     //** left right ports
                                           //** the function 'standard_draw_ports' it's copied in 'draw_ports'
  elseif rhs==4 then                   //** otherwise 
      draw_ports = standard_draw_ports_up  //** up / down ports 
  end                                      //** the function 'standard_draw_ports_up' it's copied in 'draw_ports'

  nin  = size(o.model.in,1);
  nout = size(o.model.out,1);
  clkin = size(o.model.evtin,1);
  clkout= size(o.model.evtout,1);
  [orig,sz,orient] = (o.graphics.orig,o.graphics.sz,o.graphics.flip)

//  [orig,sz,orient]=o(2)(1:3)


//** Default values -------------------
  //thick = xget('thickness');
  // xset('thickness',2)
  thick = 1 ; //** patch di Simone 
  
  // pat = xget('pattern')
  // xset('pattern',default_color(0))
  pat = default_color(0) ; //** patch di Simone 
  
  e=4
  With3D=options('3D')(1)
//** ----------------------------------

//** local haldler(read/write) = semiglobalhandler(read/only)  
  gh_curwin = gh_current_window ; //** get the handle of the current graphics window
  //** drawlater ; //** all the draw operation are postponed 

  // draw box

  if frame then
    
    gr_i = o.graphics.gr_i
  
      if type(gr_i) == 15 then 
           [gr_i,coli]=gr_i(1:2),
      else 
	       coli=[]
      end

  
    if With3D then  //** this is the code relative to the block's "window dressing" 
      #Color3D = options('3D')(2)
      //3D aspect
      // xset('thickness',2);
      // xset('pattern',#Color3D)
      
      // pause ;
      
      // xpoly([orig(1)+e;orig(1)+sz(1);orig(1)+sz(1)],[orig(2)+sz(2);orig(2)+sz(2);orig(2)+e],'lines')
      xrect( orig(1)+e, orig(2)+sz(2), sz(1)-e, sz(2)-e) ;
      gh_e = gce();
      gh_e.thickness = 0 ; //** 
      gh_e.foreground = #Color3D ;
      
      if coli<>[] then 
          gh_e.fill_mode = "on"  ;
          gh_e.background = coli ;
      end
      
      eps=0.3
      
      xx = [ orig(1) , orig(1)
	           orig(1) , orig(1)+sz(1)-e
	           orig(1)+e   , orig(1)+sz(1)
	           orig(1)+e   , orig(1)+e];
      
      yy = [orig(2)         , orig(2)
	          orig(2)+sz(2)-e   , orig(2)
	          orig(2)+sz(2) , orig(2)+e
	          orig(2)+e           , orig(2)+e];     
      
      //** xset('pattern',default_color(0))
      //** xset('thickness',1)
      xfpolys(xx,yy,-[1,1]*#Color3D)
      gh_e = gce();gh_c=gh_e.children(1:2)
      gh_c.foreground = default_color(0) ;
      gh_c.thickness = 2 ;
      
    else
      e = 0   
      xrect(orig(1),orig(2)+sz(2),sz(1),sz(2));
      gh_e = gce();
      gh_e.thickness = 2 ;
      
      if coli<>[] then 
          gh_e.fill_mode = "on"  ;
          gh_e.background = coli ;
      end
      
      
    end //** of 3D mode 
    
    //**  xset('thickness',2); 
  
  end //** of the frame mode 

  draw_ports(o) ; //** 'standard_draw_ports' or 'standard_draw_ports_up'

//** --------------------------------------------------------------------------------------------------------------

//** -------------------------------- Identification --------------------------------------------------------------
 // draw Identification
  //------------------------
  ident = o.graphics.id
  fnt = xget('font')
  
  if ident <> []& ident <> ''  then
      xset('font', options.ID(1)(1), options.ID(1)(2))
      rectangle = xstringl(orig(1), orig(2), ident)
      w = max(rectangle(3), sz(1)) ;
      h = rectangle(4) * 1.3 ;
      xstringb(orig(1) + sz(1) / 2 - w / 2, orig(2) - h , ident , w, h) ;
      xset('font', fnt(1), fnt(2)) 
  end

 // xset('thickness',thick)

  deff('c=scs_color(c)',' ') 
  flag='foreground'

//  gr_i = o.graphics.gr_i
//  
//   if type(gr_i) == 15 then 
//         [gr_i,coli]=gr_i(1:2),
//    else 
//       coli=[]
//   end
//
//    if coli==[] then
//      colixrect(orig(1),orig(2)+sz(2),sz(1),sz(2)) = xget('background')
//    end
    
   gr_i = [gr_i ] ; //** simple but not simpler :) 
        
//** ------- Old code that support the xor mode --------------------------------------

//    gr_i=['pcoli=xget(''pattern'')';..
//	  'xset(''pattern'',coli)';
//	  'xfrect(orig(1),orig(2)+sz(2),sz(1),sz(2))';
//	  'flag=''background'';'
//	  gr_i;
//	  'xset(''pattern'',pcoli)'
//	  'flag=''foreground'';';
//	  gr_i]

//**----------------------------------------------------------------------------------

  model = o.model
  
  if With3D&frame then
    orig=orig+e
    sz=sz-e
  end

//** Phisical draw AND check of the graphics object   
  if execstr(gr_i,'errcatch')<>0 then 
      message(['Error in Icon defintion'; 'See error message in scilab window'])
  end
  
  //** drawnow ;
  //** show_pixmap ; //** eliminato perche' NON funziona ! 
  
  
  
  //**-------- RESET the default value : OLD CODE TO REMOVE LATER ! 
  //** xset('font',fnt(1),fnt(2))
  
  // xset('thickness',1)

  // xset('pattern',pat)

endfunction

