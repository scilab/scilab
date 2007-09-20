function [x,y,typ]=MATCATV(job,arg1,arg2)
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
    [ok,typ,l1,lab]=..
        getvalue('Set MATCATV block parameters',..
        ['Datatype(1=real double  2=Complex)';
	 'number of line of each matrix'],..
         list('vec',1,'mat',[-1 1]),label)
    if ~ok then break,end
    l1=l1(:);
    if (typ==1) then
	junction_name='mat_catv';
      	ot=1;
	it=[ones(size(l1,1),1)'];
    elseif (typ==2) then
 	junction_name='matz_catv';
      	ot=2;
	it=[2*ones(size(l1,1),1)'];
    else message("Datatype is not supported");ok=%f;
    end
    if ok then
      label=lab
      in=[l1 -ones(size(l1,1),1)]
      out=[sum(l1),-1]
      [model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),[],[])
    end
    if ok then
      funtyp=4;
      model.sim=list(junction_name,funtyp)
      graphics.exprs=label
      arg1.graphics=graphics
      arg1.model=model
      x=arg1
      break
    end
  end
case 'define' then
  l1=[2;2]
  model=scicos_model()
  junction_name='mat_catv';
  funtyp=4;
  model.sim=list(junction_name,funtyp)
  model.in=[-1;-1]
  model.in2=[-2;-3]
  model.intyp=[1 1]
  model.out=1
  model.out2=1
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
  label=[sci2exp(1);sci2exp(l1)];
  gr_i=['text=[''Vert'';'' Cat''];';'xstringb(orig(1),orig(2),text,sz(1),sz(2),''fill'');']
  x=standard_define([2 3],model,label,gr_i)
end
endfunction

