function [x,y,typ]=ESELECT_f(job,arg1,arg2)
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
    [ok,out,label]=getvalue('Set ESELECT block parameters',..
	'number of output event ports',list('vec',1),label)
    if ~ok then break,end
    if out<2 then
      message('Block must have at least two output ports')
    else
      [model,graphics,ok]=check_io(model,graphics,1,[],[1],[ones(out,1)])
      if ok then
	graphics(4)=label;model(5)=ones(out,1);model(9)=out
	model(1)(2)=-1
	x(2)=graphics;x(3)=model
	break
      end
    end
  end
case 'define' then
  model=list(list('eselect',-1),1,[],1,[1;1],[],[],[],[2],'l',[-1 -1],[%f %f],' ',list())
  gr_i=['txt=[''event select''];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
label=string(2)
  x=standard_define([3 2],model,label,gr_i)
end








