function [x,y,typ]=CLKFROM(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[]
select job
case 'plot' then
  orig=arg1.graphics.orig;sz=arg1.graphics.sz;orient=arg1.graphics.flip;
  prt='['+arg1.graphics.exprs+']'
  pat=xget('pattern');xset('pattern',default_color(-1))
  thick=xget('thickness');xset('thickness',2)
  if orient then
    y=orig(2)+sz(2)*[1/4 1/2 1;1 1 1;1 1/2 1/4;1/4 1/8 0;0 1/8 1/4]'
    x=orig(1)+sz(1)*[0 0 0;0 1/2 1;1 1 1;1 3/4 1/2;1/2 1/4 0]'
    x1=0
  else
    y=orig(2)+sz(2)*[0 1/2 3/4;3/4 7/8 1;1 7/8 3/4;3/4 1/2 0;0 0 0]'
    x=orig(1)+sz(1)*[0 0 0;0 1/4 1/2;1/2 3/4 1;1 1 1;1 1/2 0]'
    x1=0
  end
  xpolys(x,y,5*ones(1,5))
  xstringb(orig(1)+x1*sz(1),orig(2),prt,(1-x1)*sz(1),sz(2))

  xset('thickness',thick)
  xset('pattern',pat)
  //identification
  ident = arg1.graphics.id
  if ident <> [] & ident <> '' then
    font=xget('font')
    xset('font', options.ID(1)(1), options.ID(1)(2))
    if ~orient then
       rectangle = xstringl(orig(1), orig(2), ident) ;
       w = max(rectangle(3), sz(1)) ;
       h = rectangle(4) * 1.3 ;
       xstringb(orig(1) + sz(1) / 2 - w / 2, orig(2) - h , ident , w, h) ;
    else
       rectangle = xstringl(orig(1), orig(2)+sz(2), ident) ;
       w = max(rectangle(3), sz(1)) ;
       h = rectangle(4) * 1.3 ;
       xstringb(orig(1) + sz(1) / 2 - w / 2, orig(2)+ sz(2) , ident , w, h) ;
    end
    xset('font', font(1), font(2))
  end
  x=[];y=[]
case 'getinputs' then
  x=[];y=[];typ=[]
case 'getoutputs' then
  orig=arg1.graphics.orig;sz=arg1.graphics.sz;
  if arg1.graphics.flip then
    x=orig(1)+sz(1)/2
    y=orig(2)
  else
    x=orig(1)+sz(1)/2
    y=orig(2)+sz(2)
  end
  typ=-ones(x)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1.graphics;
  model=arg1.model;
  exprs=graphics.exprs
  while %t do
    [ok,tag,exprs]=getvalue('Set block parameters',..
	'Tag',list('str',-1),exprs)
    if ~ok then break,end
    if model.opar<>list(tag) then needcompile=4;y=needcompile,end
      model.opar=list(tag)
      model.evtout=1
      model.firing=-1//compatibility
      graphics.exprs=exprs
      x.graphics=graphics
      x.model=model
      break
  end
  needcompile=resume(needcompile)
case 'define' then
  model=scicos_model()
  model.sim='clkfrom'
  model.evtout=1
  model.opar=list('A')
  model.blocktype='d'
  model.firing=-1
  model.dep_ut=[%f %f]

  exprs='A'
  x=standard_define([1.5 1.5],model,exprs,' ')
  x.graphics.id="From"
end
endfunction
