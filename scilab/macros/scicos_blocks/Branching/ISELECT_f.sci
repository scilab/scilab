function [x,y,typ]=ISELECT_f(job,arg1,arg2)
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
  model=arg1(3);
  while %t do
    [ok,nout,z0,label]=getvalue('Set parameters',..
	['number of outputs';'initial connected output'],..
	list('vec',1,'vec',1),label)
    if ~ok then break,end
    if z0>nout|z0<=0 then
      message('initial connected input is not a valid input port number')
    else
      [model,graphics,ok]=check_io(model,graphics,-1,-ones(nout,1),ones(nout,1),[])
      if ok then
	graphics(4)=label;
	model(7)=z0-1,
	model(11)=[] //compatibility
	x(2)=graphics;x(3)=model
	break
      end
    end
  end
case 'define' then
  z0=0
  out=[-1;-1]
  nout=2
  model=list(list('selector',2),-1,out,ones(out),[],[],z0,[],[],'c',[],[%t %f],' ',list())
  label=[string(nout);string(z0+1)]
  gr_i=['xstringb(orig(1),orig(2),''Selector'',sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,label,gr_i)
end




