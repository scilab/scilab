function [x,y,typ]=fortran_block(job,arg1,arg2)
//
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
  x=arg1
  model=arg1(3);graphics=arg1(2);
  label=graphics(4);
  while %t do
    [ok,i,o,rpar,funam,lab]=..
	getvalue('Set fortran_block parameters',..
	  ['input ports sizes';
	  'output port sizes';
	  'System parameters vector';
	  'function name'],..
	  list('vec',-1,'vec',-1,'vec',-1,'str',-1),label(1))
    if ~ok then break,end
    if funam==' ' then break,end
    label(1)=lab
    rpar=rpar(:)
    i=int(i(:));ni=size(i,1);
    o=int(o(:));no=size(o,1);
    tt=label(2);
    if model(1)(1)<>funam|size(model(2),'*')<>size(i,'*')..
	|size(model(3),'*')<>size(o,'*') then
      tt=[]
    end
    [ok,tt]=FORTR(funam,tt,i,o)
    if ~ok then break,end
    [model,graphics,ok]=check_io(model,graphics,i,o,[],[])
    if ok then
      model(1)(1)=funam
      model(8)=rpar
      label(2)=tt
      x(3)=model
      graphics(4)=label
      x(2)=graphics
      break
    end
  end
case 'define' then
  in=1
  out=1
  clkin=[]
  clkout=[]
  x0=[]
  z0=[]
  typ='c'
  auto=[]
  rpar=[]
  funam='forty'
  model=list(list(' ',1001),in,out,clkin,clkout,x0,z0,rpar,0,typ,auto,[%t %f],..
      ' ',list());
  label=list([sci2exp(in);sci2exp(out);	strcat(sci2exp(rpar));funam],..
	    list([]))
  gr_i=['xstringb(orig(1),orig(2),''Fortran'',sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,label,gr_i)
end

