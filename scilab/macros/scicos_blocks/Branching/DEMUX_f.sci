function [x,y,typ]=DEMUX_f(job,arg1,arg2)
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
    [ok,out,label]=getvalue('Set DEMUX block parameters',..
	['number of output ports'],list('vec',1),label)
    if ~ok then break,end
    if out<2 | out>8 then
      message('Block must have at least 2 and at most 8 output ports')
    else
      [model,graphics,ok]=check_io(model,graphics,0,-[1:out]',[],[])
      if ok then
	graphics(4)=label;model(9)=out
	x(2)=graphics;x(3)=model
	break
      end
    end
  end
  x(3)(11)=[]//compatibility
case 'define' then
  out=2
  model=list(list('demux',1),0,-[1:out]',[],[],[],[],[],out,..
      'c',[],[%t %f],' ',list())
  label=string(out)
  gr_i='xstringb(orig(1),orig(2),''Demux'',sz(1),sz(2),''fill'')'
  x=standard_define([2 2],model,label,gr_i)
end




