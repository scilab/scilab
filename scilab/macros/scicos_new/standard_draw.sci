function standard_draw(o,frame,draw_ports,up)
//
// Copyright INRIA
  xf=60
  yf=40
  [lhs,rhs]=argn(0)
  if rhs==1 then
    frame=%t
  end

  if rhs<3 then
    draw_ports=standard_draw_ports
  elseif rhs==4 then
    draw_ports=standard_draw_ports_up
  end

  nin=size(o.model.in,1);
  nout=size(o.model.out,1);
  clkin=size(o.model.evtin,1);
  clkout=size(o.model.evtout,1);
 [orig,sz,orient]=(o.graphics.orig,o.graphics.sz,o.graphics.flip)
//  [orig,sz,orient]=o(2)(1:3)
  thick=xget('thickness');xset('thickness',2)

  // draw box
  pat=xget('pattern')
  xset('pattern',default_color(0))
  e=4
  With3D=options('3D')(1)

  if frame then
    
    if With3D then
      #Color3D=options('3D')(2)
      //3D aspect
      xset('thickness',2);
      xset('pattern',#Color3D)
      xpoly([orig(1)+e;orig(1)+sz(1);orig(1)+sz(1)],..
	    [orig(2)+sz(2);orig(2)+sz(2);orig(2)+e],'lines')
      xset('pattern',default_color(0))
      xset('thickness',1)
      eps=0.3
      xx=[orig(1) , orig(1)
	  orig(1) , orig(1)+sz(1)-e
	  orig(1)+e   , orig(1)+sz(1)
	  orig(1)+e   , orig(1)+e];
      
      yy=[orig(2)         , orig(2)
	  orig(2)+sz(2)-e   , orig(2)
	  orig(2)+sz(2) , orig(2)+e
	  orig(2)+e           , orig(2)+e];     
      xfpolys(xx,yy,-[1,1]*#Color3D)
      xset('thickness',2);
    else
      e=0
      xset('thickness',2);
      xrect(orig(1),orig(2)+sz(2),sz(1),sz(2))
    end
  end

  draw_ports(o)

 // draw Identification
  //------------------------
  ident = o.graphics.id
  fnt=xget('font')
  if ident <> []& ident <> ''  then
    xset('font', options.ID(1)(1), options.ID(1)(2))
    rectangle = xstringl(orig(1), orig(2), ident)
    w = max(rectangle(3), sz(1))
    h = rectangle(4) * 1.3
    xstringb(orig(1) + sz(1) / 2 - w / 2, orig(2) - h ,	..
	     ident , w, h)
    xset('font', fnt(1), fnt(2))
  end

  xset('thickness',thick)

  deff('c=scs_color(c)','if flag==''background'' then c=coli,end  ')
  flag='foreground'

    gr_i=o.graphics.gr_i
    if type(gr_i)==15 then 
      [gr_i,coli]=gr_i(1:2),
    else 
	coli=[]
    end

    if coli==[] then
      coli=xget('background')
    end
    gr_i=['pcoli=xget(''pattern'')';..
	  'xset(''pattern'',coli)';
	  'xfrect(orig(1),orig(2)+sz(2),sz(1),sz(2))';
	  'flag=''background'';'
	  gr_i;
	  'xset(''pattern'',pcoli)'
	  'flag=''foreground'';';
	  gr_i]


  model=o.model
  if With3D&frame then
    orig=orig+e
    sz=sz-e
  end

  if execstr(gr_i,'errcatch')<>0 then 
    message(['Error in Icon defintion';
	     'See error message in scilab window'])
  end
  xset('pattern',pat)
  xset('font',fnt(1),fnt(2))
  xset('thickness',1)
endfunction

