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
 x=arg1;
  model=arg1.model;graphics=arg1.graphics;
  label=graphics.exprs;
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
    if model.sim(1)<>funam|size(model.in,'*')<>size(i,'*')..
        |size(model.out,'*')<>size(o,'*') then
      tt=[]
    end
    [ok,tt]=FORTR(funam,tt,i,o)
    if ~ok then break,end
    [model,graphics,ok]=check_io(model,graphics,i,o,[],[])
    if ok then
      model.sim(1)=funam
      model.rpar=rpar
      label(2)=tt
      x.model=model
      graphics.exprs=label
      x.graphics=graphics
      break
    end
  end
case 'define' then
  model=scicos_model()
  model.sim=list(' ',1001)
  model.in=1
  model.out=1
  model.evtin=[]
  model.evtout=[]
  model.state=[]
  model.dstate=[]
  model.rpar=[]
  model.ipar=0
  model.blocktype='c'
  model.firing=[]
  model.dep_ut=[%t %f]
  funam='forty'
  label=list([sci2exp(model.in);sci2exp(model.out);..
  strcat(sci2exp(model.rpar));funam],list([]))
  gr_i=['xstringb(orig(1),orig(2),''Fortran'',sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,label,gr_i)
end
endfunction
