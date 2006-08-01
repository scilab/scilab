function [x,y,typ]=CLKINV_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[]
select job
case 'plot' then
  orig=arg1.graphics.orig;
  sz=arg1.graphics.sz;
  gr_i=arg1.graphics.gr_i;
  
  prt=arg1.model.ipar
  pat=xget('pattern');xset('pattern',default_color(-1))
  thick=xget('thickness');xset('thickness',2)
  x=orig(1)+sz(1)*[1/2;1;  1;0;0  ]
  y=orig(2)+sz(2)*[0;  1/3;1;1;1/3]
  xo=orig(1);yo=orig(2)+sz(2)/3
 
  if type(gr_i)==15 then 
    coli=gr_i(2);
    xfpolys(x,y,coli);
    pcoli=xget('pattern');xset('pattern',coli)
    xstringb(xo,yo,string(prt),sz(1),sz(2)/1.5)
    xset('pattern',pcoli)
    xstringb(xo,yo,string(prt),sz(1),sz(2)/1.5)
  else
    xstringb(xo,yo,string(prt),sz(1),sz(2)/1.5)
    xpoly(x,y,'lines',1)
  end
  
  xset('thickness',thick)
  xset('pattern',pat)
  //identification
  ident = arg1.graphics.id
  if ident <> [] & ident <> '' then
    font=xget('font')
    xset('font', options.ID(1)(1), options.ID(1)(2))
    rectangle = xstringl(orig(1), orig(2)+3/2*sz(2), ident)
    w = rectangle(3)
    h = rectangle(4)
    xstringb(orig(1) + sz(1)/2 -w/2, orig(2)+sz(2)+h/2 , ident , w, h)
    xset('font', font(1), font(2))
  end
case 'getinputs' then
  x=[];y=[];typ=[]
case 'getoutputs' then
  orig=arg1.graphics.orig
  sz=arg1.graphics.sz
  x=orig(1)+sz(1)/2
  y=orig(2)
  typ=-ones(x)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1.graphics
  model=arg1.model

  exprs=graphics.exprs
  exprs=exprs(1) // compatibility
  while %t do
    [ok,prt,exprs]=getvalue('Set Event Input block parameters',..
	'Port number',list('vec',1),exprs)
    prt=int(prt)
    if ~ok then break,end
    if prt<=0 then
      message('Port number must be a positive integer')
    else
      model.ipar=prt
      model.evtout=1
      model.firing=-1//compatibility
      graphics.exprs=exprs
      x.graphics=graphics
      x.model=model
      break
    end
  end
case 'define' then
  prt=1
  model=scicos_model()
  model.sim='input'
  model.evtout=1
  model.ipar=prt
  model.blocktype='d'
  model.firing=-1
  model.dep_ut=[%f %f]
  
  exprs=string(prt)
  gr_i=['xo=orig(1);yo=orig(2)+sz(2)/3';
        'xstringb(xo,yo,string(prt),sz(1),sz(2)/1.5)']
  x=standard_define([1 1.5],model,exprs,gr_i)
end
endfunction
