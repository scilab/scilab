function [x,y,typ]=FROMMO(job,arg1,arg2)
  x=[];y=[];typ=[]
  select job
   case 'plot' then
    pat=xget('pattern'); xset('pattern',default_color(0))
    orig=arg1.graphics.orig;
    sz=arg1.graphics.sz;
    orient=arg1.graphics.flip;
    tg='['+arg1.graphics.exprs(1)+']';
    if orient then
      xx=orig(1)+[0 0 0;0 1/2 3/4;3/4 7/8 1;1 7/8 3/4;3/4 1/2 0]'*sz(1);
      yy=orig(2)+[0 1/2 1;1 1 1;1 3/4 1/2;1/2 1/4 0;0 0 0]'*sz(2);
      x1=0
    else
      xx=orig(1)+[0 1/8 1/4;1/4 1/2 1;1 1 1;1 1/2 1/4;1/4 1/8 0]'*sz(1);
      yy=orig(2)+[1/2 3/4 1 ;1 1 1;1 1/2 0;0 0 0;0 1/4 1/2]'*sz(2);
      x1=0
    end
    xpolys(xx,yy)
    xstringb(orig(1)+x1*sz(1),orig(2),tg,(1-x1)*sz(1),sz(2));

    xf=60
    yf=40
    nin=0;nout=1

      // set port shape
      out=[0   -1/14
	   1/7 -1/14
	   1/7 1/14
	   0   1/14
	   0   -1/14]*diag([xf,yf])
      in= [-1/7  -1/14
	   0     -1/14
	   0      1/14
	   -1/7   1/14
	   -1/7  -1/14]*diag([xf,yf])
      dy=sz(2)/(nout+1)
       xset('pattern',default_color(1))
    if orient then  //standard orientation
      for k=1:nout
	xpoly(out(:,1)+ones(5,1)*(orig(1)+sz(1)),..
	       out(:,2)+ones(5,1)*(orig(2)+sz(2)-dy*k),"lines",1)
      end
    else //tilded orientation
      for k=1:nout
	xpoly(out(:,1)+ones(5,1)*orig(1)-1/7*xf,..
	       out(:,2)+ones(5,1)*(orig(2)+sz(2)-dy*k),"lines",1)
      end
    end
    xset('pattern',pat)
        //** ------- Identification ---------------------------
    ident = o.graphics.id
    gh_axes = gca(); //** get the Axes proprieties 
  
    // draw Identification
    if ident <> []& ident <> ''  then
  
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
  //** ----- Identification End -----------------------------

   case 'getinputs' then
    [x,y,typ]=standard_inputs(arg1)
   case 'getoutputs' then
    [x,y,typ]=standard_outputs(arg1)
   case 'getorigin' then
    [x,y]=standard_origin(arg1)
   case 'set' then
    x=arg1;
    graphics=arg1.graphics;exprs=graphics.exprs
    model=arg1.model;
    while %t do
      [ok,tag,exprs]=getvalue('Set parameters',..
		['Tag'],..
		    list('str',-1),exprs)
      if ~ok then break,end
      if ok then 
         if model.opar<>list(tag) then needcompile=4;y=needcompile,end
	 graphics.exprs=exprs;
	 model.opar=list(tag)
	 x.model=model
	 x.graphics=graphics
	 break
      end
    end
   needcompile=resume(needcompile)
   case 'define' then
    model=scicos_model()
    model.sim='frommo'
    model.in=[]
    model.in2=[]
    model.intyp=1
    model.out=-1
    model.out2=-2
    model.outtyp=-1
    model.ipar=[]
    model.opar=list('A')
    model.blocktype='c'
    model.dep_ut=[%f %f]
    mo=modelica()
    mo.model='frommo'
    mo.outputs='n'
    exprs=['A']
    
    gr_i='';
    x=standard_define([1.5 1.5],model,exprs,gr_i)
    x.graphics.out_implicit=['I']
  end
endfunction
