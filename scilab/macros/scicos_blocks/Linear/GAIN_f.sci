function [x,y,typ]=GAIN_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[];
select job
case 'plot' then
  standard_draw(arg1)
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
  gr_i=['[nin,nout]=model(2:3);';
      'if nin*nout==1 then gain=string(model(8)),else gain=''Gain'',end';
      'dx=sz(1)/5;';
      'dy=sz(2)/10;';
      'xx=orig(1)+      [1 4 1 1]*dx;';
      'yy=orig(2)+sz(2)-[1 5 9 1]*dy;';
      'xpoly(xx,yy,''lines'');';
      'w=sz(1)-2*dx;h=sz(2)-2*dy;';
      'xstringb(orig(1)+dx,orig(2)+dy,gain,w,h,''fill'');']
  x=standard_define([2 2],model,label,gr_i)
end




