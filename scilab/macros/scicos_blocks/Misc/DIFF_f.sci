function [x,y,typ]=DIFF_f(job,arg1,arg2)
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
  x=arg1
  graphics=arg1(2);label=graphics(4)
  model=arg1(3);
  while %t do
   [ok,x0,xd0,label]=getvalue('Set continuous linear system parameters',..
	['Initial state','Initial Derivative'],list('vec',1,'vec',1),label)


    if ~ok then break,end
    graphics(4)=label;
    model(6)=[x0(:);xd0(:)];
    x(2)=graphics;x(3)=model
    break
  end
  x(3)(11)=[] //compatibility
case 'define' then
  x0=[0;0]
  model=list(list('diffblk',10001),1,1,[],[],x0,[],[],[],'c',[],[%f %t],' ',list())
  label=[strcat(sci2exp(x0(1)));strcat(sci2exp(x0(2)))]
  gr_i=['xstringb(orig(1),orig(2),''  s  '',sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,label,gr_i)
end
endfunction
