function [x,y,typ]=SWITCH_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[]
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
  model=arg1(3);ipar=model(9)
  while %t do
    [ok,nin,z0,label]=getvalue('Set switch parameters',..
	['number of inputs';'connected input'],..
	list('vec',1,'vec',1),label)
    if ~ok then break,end
    if z0>nin|z0<=0 then
      message('initial connected input is not a valid input port number')
    else
      [model,graphics,ok]=check_io(model,graphics,-ones(nin,1),1,[],[])
      if ok then
	graphics(4)=label;
	model(9)=z0-1
	x(2)=graphics;x(3)=model
	break
      end
    end
  end
case 'define' then
  i0=0
  in=[-1;-1]
  nin=2
  model=list(list('switchn',2),in,-1,[],[],[],[],[],i0,'c',[],[%t %t],' ',list())
  label=[string(nin);string(i0+1)]
  gr_i=['xstringb(orig(1),orig(2),[''switch'';string(i0+1)],sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,label,gr_i)
end




