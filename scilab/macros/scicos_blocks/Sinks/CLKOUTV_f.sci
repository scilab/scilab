function [x,y,typ]=CLKOUTV_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[];
select job
case 'plot' then
  xf=60
  yf=40
  graphics=arg1(2); [orig,sz,orient]=graphics(1:3)
  model=arg1(3);prt=model(9)
  pat=xget('pattern');xset('pattern',default_color(-1))
  thick=xget('thickness');xset('thickness',2)
  x=orig(1)+sz(1)*[1/2;1;  1;0;0  ]
  y=orig(2)+sz(2)*[0;  1/3;1;1;1/3]
  xo=orig(1);yo=orig(2)+sz(2)/3;
    
  gr_i=arg1(2)(9);
  if type(gr_i)==15 then 
    coli=gr_i(2);pcoli=xget('pattern')
    xfpolys(x,y,coli);
    xset('pattern',coli)
    xstringb(xo,yo,string(prt),sz(1),sz(2)/1.5)
    xset('pattern',pcoli)
    xstringb(xo,yo,string(prt),sz(1),sz(2)/1.5)
  else
    xstringb(xo,yo,string(prt),sz(1),sz(2)/1.5)
    xpoly(x,y,'lines',1)
  end
 
  in= [-1/14  1/7
     0      0
     1/14   1/7
     -1/14  1/7]*diag([xf,yf])
  xfpoly(in(:,1)+ones(4,1)*(orig(1)+sz(1)/2),..
      in(:,2)+ones(4,1)*(orig(2)+sz(2)),1)
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
    rectangle = xstringl(orig(1), orig(2), ident)
    w = rectangle(3)
    h = rectangle(4)
    xstringb(orig(1)+sz(1)/2-w/2, orig(2)-3*h/2 , ident , w, h)
    xset('font', font(1), font(2))
  end
  x=[];y=[];
case 'getinputs' then
  graphics=arg1(2)
  [orig,sz,orient]=graphics(1:3)
  x=orig(1)+sz(1)/2
  y=orig(2)+sz(2)
  typ=-ones(x) 
case 'getoutputs' then
  x=[];y=[];typ=[];
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  [graphics,model]=arg1(2:3);
  label=graphics(4);
    while %t do
    [ok,prt,label]=getvalue('Set Event Output block parameters',..
	'Port number',list('vec',1),label)
    if ~ok then break,end
    prt=int(prt)
    if prt<=0 then
      message('Port number must be a positive integer')
    else
      model(9)=prt
      model(4)=1
      graphics(4)=label
      x(2)=graphics;
      x(3)=model
      break
    end
  end
case 'define' then
  prt=1
  model=list('output',[],[],1,[],[],[],[],prt,'d',[],[%f %f],' ',list())
  label=string(prt)
  x=standard_define([1 1.5],model,label,' ')
end








