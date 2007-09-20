function [x,y,typ]=RICC(job,arg1,arg2)
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
  graphics=arg1.graphics;label=graphics.exprs
  model=arg1.model;
  if size(label,'*')==14 then label(9)=[],end //compatiblity
  while %t do
    [ok,tpe,mod,exprs]=getvalue('Set RICC Block',['Type (1=Cont  2=Disc)';'Model(1=Schr  2=sign(cont) inv(disc))'],list('vec',1,'vec',1),label)
    if ~ok then break,end
    in=[model.in model.in2];
    out=[model.out model.out2];
    it=[1 1 1]
    ot=1
    label=exprs;
    [model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),[],[])
      if ok then
          model.ipar=[tpe;mod]
          graphics.exprs=label;
          arg1.graphics=graphics;arg1.model=model;
          x=arg1
          break
      end
  end
case 'define' then
  model=scicos_model()
  junction_name='ricc_m';
  funtyp=4;
  model.sim=list(junction_name,funtyp)
  model.in=[-1;-1;-1]
  model.in2=[-1;-1;-1]
  model.intyp=[1 1 1]
  model.out=-1
  model.out2=-1
  model.outtyp=1
  model.evtin=[]
  model.evtout=[]
  model.state=[]
  model.dstate=[]
  model.rpar=[]
  model.ipar=[1;1]
  model.blocktype='c' 
  model.firing=[]
  model.dep_ut=[%t %f]
  label=[sci2exp(1);sci2exp(1)];
  gr_i=['xstringb(orig(1),orig(2),''RICC'',sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,label,gr_i)
end
endfunction