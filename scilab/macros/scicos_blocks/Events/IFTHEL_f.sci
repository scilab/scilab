function [x,y,typ]=IFTHEL_f(job,arg1,arg2)
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
  if label==[] then label=string(1);end
  while %t do
    [ok,inh,label]=getvalue('Set parameters',..
	['Inherit (1: no, 0: yes)'],list('vec',1),label)
    if ~ok then break,end
    if inh==0 then inh=[]; else inh=1;end
    [model,graphics,ok]=check_io(model,graphics,1,[],inh,[1;1])
      if ok then
	graphics(4)=label;
	model(4)=inh;
	model(1)(2)=-1
	x(2)=graphics;x(3)=model
	break
      end
  end
case 'define' then
  model=list(list('ifthel',-1),1,[],1,[1;1],[],[],[],[],'l',[-1 -1],[%t %f],' ',list())
  gr_i=['txt=[''If in>0'';'' '';'' then    else''];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
label=string(1);
  x=standard_define([3 3],model,label,gr_i)
end


