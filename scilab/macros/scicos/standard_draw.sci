function standard_draw(o,frame)
//
// Copyright INRIA

  xf=60
  yf=40
  [lhs,rhs]=argn(0)
  if rhs==1 then
    frame=%t
  end

  nin=size(o.model.in,1);
  nout=size(o.model.out,1);
  clkin=size(o.model.evtin,1);
  clkout=size(o.model.evtout,1);
  
  [orig,sz,orient]=(o.graphics.orig,o.graphics.sz,o.graphics.flip)

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
      
      //    xset('pattern',1)
      xfpolys(xx,yy,-[1,1]*#Color3D)
      xset('thickness',2);
    else
      e=0
      xset('thickness',2);
      xrect(orig(1),orig(2)+sz(2),sz(1),sz(2))
    end
  end

  xset('pattern',default_color(0))
  // draw input/output ports
  //------------------------

  if orient then  //standard orientation
		  // set port shape
		  out=[0  -1/14
		       1/7 0
		       0   1/14
		       0  -1/14]*diag([xf,yf])
		  in= [-1/7  -1/14
		       0    0
		       -1/7   1/14
		       -1/7  -1/14]*diag([xf,yf])
		  dy=sz(2)/(nout+1)
		  xset('pattern',default_color(1))
		  for k=1:nout
		    xfpoly(out(:,1)+ones(4,1)*(orig(1)+sz(1)),..
			   out(:,2)+ones(4,1)*(orig(2)+sz(2)-dy*k),1)
		  end


		  dy=sz(2)/(nin+1)
		  for k=1:nin
		    xfpoly(in(:,1)+ones(4,1)*orig(1),..
			   in(:,2)+ones(4,1)*(orig(2)+sz(2)-dy*k),1)
		  end
  else //tilded orientation
    out=[0  -1/14
	 -1/7 0
	 0   1/14
	 0  -1/14]*diag([xf,yf])
    in= [1/7  -1/14
	 0    0
	 1/7   1/14
	 1/7  -1/14]*diag([xf,yf])
    dy=sz(2)/(nout+1)
    xset('pattern',default_color(1))
    for k=1:nout
      xfpoly(out(:,1)+ones(4,1)*orig(1)-1,..
	     out(:,2)+ones(4,1)*(orig(2)+sz(2)-dy*k),1)
    end
    dy=sz(2)/(nin+1)
    for k=1:nin
      xfpoly(in(:,1)+ones(4,1)*(orig(1)+sz(1))+1,..
	     in(:,2)+ones(4,1)*(orig(2)+sz(2)-dy*k),1)
    end
  end

  // draw input/output clock ports
  //------------------------
  // set port shape

  out= [-1/14  0
	0      -1/7
	1/14   0
	-1/14  0]*diag([xf,yf])


  in= [-1/14  1/7
       0      0
       1/14   1/7
       -1/14  1/7]*diag([xf,yf])


  dx=sz(1)/(clkout+1)
  xset('pattern',default_color(-1))
  for k=1:clkout
    xfpoly(out(:,1)+ones(4,1)*(orig(1)+k*dx),..
	   out(:,2)+ones(4,1)*orig(2),1)
    //        out(:,2)+ones(4,1)*orig(2),1)
  end
  dx=sz(1)/(clkin+1)
  for k=1:clkin
    xfpoly(in(:,1)+ones(4,1)*(orig(1)+k*dx),..
	   in(:,2)+ones(4,1)*(orig(2)+sz(2)),1)
    //        in(:,2)+ones(4,1)*(orig(2)+sz(2)),1)
  end
  xset('pattern',default_color(0))
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
  if coli<>[] then
      gr_i=['pcoli=xget(''pattern'')';..
	  'xset(''pattern'',coli)';
	  'xfrect(orig(1),orig(2)+sz(2),sz(1),sz(2))';
	  'flag=''background'';'
	  gr_i;
	  'xset(''pattern'',pcoli)'
	  'flag=''foreground'';';
	  gr_i]
  end
  model=o.model

  if With3D&frame then
    orig=orig+e
    sz=sz-e
  end

  if execstr(gr_i,'errcatch')<>0 then 
    message(['Error in Icon defintion';lasterror()])
  end
  xset('pattern',pat)
  xset('font',fnt(1),fnt(2))
  xset('thickness',1)
endfunction
