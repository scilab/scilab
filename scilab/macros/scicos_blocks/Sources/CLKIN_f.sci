function [x,y,typ]=CLKIN_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[]
select job
case 'plot' then
  graphics=arg1(2); [orig,sz,orient]=graphics(1:3)
  model=arg1(3);prt=model(9)
  pat=xget('pattern');xset('pattern',default_color(-1))
  thick=xget('thickness');xset('thickness',2)
  if orient then
    x=orig(1)+sz(1)*[-1/6;-1/6;1/1.5;1;1/1.5]
    y=orig(2)+sz(2)*[0;1;1; 1/2;0]
    xo=orig(1);yo=orig(2)+sz(2)/12
  else
    x=orig(1)+sz(1)*[0;1/3;7/6;7/6;1/3]
    y=orig(2)+sz(2)*[1/2;1;1;0;0]
    xo=orig(1)+sz(1)/3;yo=orig(2)+sz(2)/12
  end
  
  gr_i=arg1(2)(9);
  if type(gr_i)==15 then 
    coli=gr_i(2);pcoli=xget('pattern')
    xfpolys(x,y,coli);
    xset('pattern',coli)
    xstringb(xo,yo,string(prt),sz(1)/1.5,sz(2))
    xset('pattern',pcoli)
    xstringb(xo,yo,string(prt),sz(1)/1.5,sz(2))
  else
    xstringb(xo,yo,string(prt),sz(1)/1.5,sz(2))
    xpoly(x,y,'lines',1)
  end

  xset('thickness',thick)
  xset('pattern',pat)
    //identification
  if size(arg1(3)) >= 15 then
    ident = arg1(3)(15)
  else
    ident = []
  end
  if ident <> [] then
    font=xget('font')
    xset('font', options('ID')(1)(1), options('ID')(1)(2))
    rectangle = xstringl(orig(1)+3/2*sz(1), orig(2), ident)
    w = rectangle(3)
    h = rectangle(4)
    xstringb(orig(1) + 3/2 * sz(1), orig(2) + sz(2) * 0.5 , ident , w, h)
    xset('font', font(1), font(2))
  end
  x=[];y=[]
case 'getinputs' then
  x=[];y=[];typ=[]
case 'getoutputs' then
  graphics=arg1(2)
  [orig,sz,orient]=graphics(1:3)
  if orient then
    x=orig(1)+sz(1)
    y=orig(2)+sz(2)/2
  else
    x=orig(1)
    y=orig(2)+sz(2)/2
  end
  typ=-ones(x)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  [graphics,model]=arg1(2:3);
  label=graphics(4)
  label=label(1) // compatibility
  while %t do
    [ok,prt,label]=getvalue('Set Event Input block parameters',..
	'Port number',list('vec',1),label)
    prt=int(prt)
    if ~ok then break,end
    if prt<=0 then
      message('Port number must be a positive integer')
    else
      model(9)=prt
      model(5)=1
      model(11)=-1//compatibility
      graphics(4)=label
      x(2)=graphics
      x(3)=model
      break
    end
  end
case 'define' then
  prt=1
  model=list('input',[],[],[],1,[],[],[],prt,'d',-1,[%f %f],' ',list())
  label=string(prt)
  x=standard_define([1 1],model,label,' ')
end


