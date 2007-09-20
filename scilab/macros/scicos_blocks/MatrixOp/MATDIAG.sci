function [x,y,typ]=MATDIAG(job,arg1,arg2)
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
    [ok,typ,exprs]=getvalue('Set MATDIAG Block',..
	    ['Datatype (1=real double  2=Complex)'],list('vec',1),label)
    if ~ok then break,end
    if (typ==1) then
	junction_name='mat_diag';
      	ot=1;
	it=1;
    elseif (typ==2) then
 	junction_name='matz_diag';
      	ot=2;
	it=2;
    else message("Datatype is not supported");ok=%f;
    end
    in=[model.in model.in2];
    out=[model.out model.out2];
    funtyp=4;
    if ok then
	label=exprs;
      [model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),[],[])
      model.sim=list(junction_name,funtyp);
      graphics.exprs=label;
      arg1.graphics=graphics;arg1.model=model;
      x=arg1
      break
    end
  end
case 'define' then
  model=scicos_model()
  junction_name='mat_diag';
  funtyp=4;
  model.sim=list(junction_name,funtyp)

  model.in=-1
  model.in2=1
  model.intyp=1
  model.out=-1
  model.out2=-1
  model.outtyp=1
  model.evtin=[]
  model.evtout=[]
  model.state=[]
  model.dstate=[]
  model.rpar=[]
  model.ipar=[]
  model.blocktype='c' 
  model.firing=[]
  model.dep_ut=[%t %f]
  label=[sci2exp(1)];
  gr_i=['xstringb(orig(1),orig(2),''DIAG'',sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,label,gr_i)
end
endfunction

