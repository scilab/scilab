function [x,y,typ]=generic_block(job,arg1,arg2)
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
  model=arg1.model;graphics=arg1.graphics;label=graphics.exprs
  if size(label,'*')==14 then label(9)=[],end //compatiblity
  while %t do
    [ok,junction_name,funtyp,i,o,ci,co,xx,z,rpar,ipar,auto0,depu,dept,lab]=..
        getvalue('Set GENERIC block parameters',..
        ['simulation function';
        'function type (0,1,2,..)';
        'input ports sizes';
        'output port sizes';
        'input event ports sizes';
        'output events ports sizes';
        'initial continuous state';
        'initial discrete state';
        'Real parameters vector';
        'Integer parameters vector';      
        'initial firing vector (<0 for no firing)';
        'direct feedthrough (y or n)';                                       
        'time dependence (y or n)'],..
         list('str',1,'vec',1,'vec',-1,'vec',-1,'vec',-1,'vec',-1,..
         'vec',-1,'vec',-1,'vec',-1,'vec',-1,'vec','sum(%6)',..
         'str',1,'str',1),label)
    if ~ok then break,end
    label=lab
    junction_name=stripblanks(junction_name)
    xx=xx(:);z=z(:);rpar=rpar(:);ipar=int(ipar(:));
    i=int(i(:));
    o=int(o(:));
    ci=int(ci(:));
    co=int(co(:));
    funtyp=int(funtyp)
    if funtyp<0 then message('function type cannot be negative');ok=%f;end
    if [ci;co]<>[] then
      if maxi([ci;co])>1 then message('vector event links not supported');ok=%f;end
    end
    depu=stripblanks(depu);if part(depu,1)=='y' then depu=%t; else depu=%f;end
    dept=stripblanks(dept);if part(dept,1)=='y' then dept=%t; else dept=%f;end
    dep_ut=[depu dept];
    if ok then
      [model,graphics,ok]=check_io(model,graphics,i,o,ci,co)
    end
    if ok then
      // AVERIFIER
      if funtyp==3 then needcompile=4;end
      model.sim=list(junction_name,funtyp);
      model.state=xx
      model.dstate=z
      model.rpar=rpar
      model.ipar=ipar
      //      needcompile=4     AVERIFIER CANEMARCHEQUAVECFORTRAN
      model.firing=auto0
      model.dep_ut=dep_ut
      arg1.model=model
      graphics.exprs=label
      arg1.graphics=graphics
      x=arg1
      break
    end
  end
  needcompile=resume(needcompile)
case 'define' then
  model=scicos_model()
  junction_name='sinblk';
  funtyp=1;
  model.sim=list(junction_name,funtyp)
  
  model.in=1
  model.out=1
  model.evtin=[]
  model.evtout=[]
  model.state=[]
  model.dstate=[]
  model.rpar=[]
  model.ipar=[]
  model.blocktype='c' 
  model.firing=[]
  model.dep_ut=[%t %f]
  label=[junction_name;sci2exp(funtyp);
	       sci2exp(model.in);sci2exp(model.out);
	       sci2exp(model.evtin);sci2exp(model.evtout);
	       sci2exp(model.state);sci2exp(model.dstate);
	       sci2exp(model.rpar);sci2exp(model.ipar);
	       sci2exp(model.firing);'y';'n'];
  gr_i=['xstringb(orig(1),orig(2),''GENERIC'',sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,label,gr_i)
end
endfunction

