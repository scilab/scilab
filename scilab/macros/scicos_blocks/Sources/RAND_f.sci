function [x,y,typ]=RAND_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[];
select job
case 'plot' then //normal  position
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
  if size(label,'*')==5 then label=label(1:3),end //compatibility
  while %t do
    [ok,flag,a,b,label]=getvalue([
	'Set Random generator block parameters';
	'flag = 0 : Uniform distribution A is min and A+B max';
	'flag = 1 : Normal distribution A is mean and B deviation';
	' ';
	'A and B must be vector with equal sizes'],..
	['flag';'A';'B'],..
	list('vec',1,'vec',-1,'vec','size(x2,''*'')'),label)
    if ~ok then break,end
    if flag<>0&flag<>1 then
      message('flag must be equal to 1 or 0')
    else
      nout=size(a,'*')
      graphics(4)=label
      model(3)=nout
      model(6)(1)=rand()
      model(9)=flag
      model(8)=[a(:);b(:);0]
      model(7)=[0*a(:);0]
      model(11)=[] //compatibility
      x(2)=graphics;x(3)=model
      break
    end
  end
case 'define' then
  a=0
  b=1
  dt=0
  out=1
  flag=0
  model=list('rndblk',[],out,1,[],[],[rand();0*a(:)],[a(:);b(:);dt],flag,'d',[],[%f %f],' ',list())
  label=[string(flag);sci2exp(a(:));sci2exp(b(:))]
  gr_i=['txt=[''random'';''generator''];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']
  x=standard_define([3 2],model,label,gr_i)
end




