function [x,y,typ]=GAINBLK_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[];
select job
case 'plot' then
  graphics=arg1(2); [orig,sz,orient]=graphics(1:3)
  gain=arg1(2)(4);
  if orient then
    xx=orig(1)+[0 1 0 0]*sz(1);
    yy=orig(2)+[0 1/2 1 0]*sz(2);
    x1=0
  else
    xx=orig(1)+[0   1 1 0]*sz(1);
    yy=orig(2)+[1/2 0 1 1/2]*sz(2);
    x1=1/4
  end
    gr_i=arg1(2)(9);
    if type(gr_i)==15 then
      coli=gr_i(2);
      if coli<>[] then
	xfpolys(xx',yy',coli);
	pcoli=xget('pattern');xset('pattern',coli)
	xstringb(orig(1)+x1*sz(1),orig(2),gain,(1-x1)*sz(1),sz(2));
	xset('pattern',pcoli)
	xstringb(orig(1)+x1*sz(1),orig(2),gain,(1-x1)*sz(1),sz(2));
      else
	xpoly(xx,yy,'lines');
	xstringb(orig(1)+x1*sz(1),orig(2),gain,(1-x1)*sz(1),sz(2));
      end
    else
      xpoly(xx,yy,'lines');
      xstringb(orig(1)+x1*sz(1),orig(2),gain,(1-x1)*sz(1),sz(2));
    end
xf=60
yf=40
nin=1;nout=1

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
//
case 'getinputs' then
  [x,y,typ]=standard_inputs(arg1)
case 'getoutputs' then
  [x,y,typ]=standard_outputs(arg1)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1(2);label=graphics(4)
  model=arg1(3);
  while %t do
    [ok,gain,label]=getvalue('Set gain block parameters',..
	['Gain'],list('mat',[-1,-1]),label(1))
    if ~ok then break,end
    if gain==[] then
      message('Gain must have at least one element')
    else
      [out,in]=size(gain)
      [model,graphics,ok]=check_io(model,graphics,in,out,[],[])
      if ok then
	graphics(4)=label
	model(8)=gain(:);
	x(2)=graphics;x(3)=model
	break
      end
    end
  end
  x(3)(11)=[] //compatibility
case 'define' then
  in=1;out=1;gain=1
  model=list('gain',in,out,[],[],[],[],gain,[],'c',[],[%t %f],' ',list())
  label=[strcat(sci2exp(gain));strcat(sci2exp(in));strcat(sci2exp(out))]
  gr_i=['']
  x=standard_define([2 2],model,label,gr_i)
end




