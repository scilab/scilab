function [x,y,typ]=MUX_f(job,arg1,arg2)
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
    [ok,in,label]=getvalue('Set MUX block parameters',..
	'number of input ports',list('vec',1),label)
    if ~ok then break,end
    if in<2|in>8then
      message('Block must have at least two input ports and at most eight')
    else
      [model,graphics,ok]=check_io(model,graphics,-[1:in]',0,[],[])
      if ok then
	graphics(4)=label;model(9)=in
	x(2)=graphics;x(3)=model
	break
      end
    end
  end
  x(3)(11)=[] //compatibility
case 'define' then
  in=2
  model=list(list('mux',1),-[1:in]',0,[],[],[],[],[],[in],..
      'c',[],[%t %f],' ',list())
  label=string(in)
  gr_i='xstringb(orig(1),orig(2),''Mux'',sz(1),sz(2),''fill'')'
  x=standard_define([2 2],model,label,gr_i)
end




