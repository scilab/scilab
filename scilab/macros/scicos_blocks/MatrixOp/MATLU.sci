function [x,y,typ]=MATLU(job,arg1,arg2)
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
    [ok,typ,lab]=..
        getvalue('Set MATLU block parameters',['Datatype(1=real double  2=Complex)'],list('vec',1),label);
	if ~ok then break,end
	if (typ==1) then
		junction_name='mat_lu';
		ot=[1 1];
		it=1;
	elseif (typ==2) then
		junction_name='matz_lu';
		ot=[2 2];
		it=2;
	else message("Datatype is not supported");ok=%f;
	end
	if ok then
               [model,graphics,ok]=set_io(model,graphics,list([model.in model.in2],it),list([model.out model.out2],ot),[],[])
	end
	if ok then 
	funtyp=4;
  	model.sim=list(junction_name,funtyp)
	graphics.exprs=lab;
      	x.graphics=graphics;x.model=model
      	break
	end
  end
case 'define' then
  model=scicos_model()
  junction_name='mat_lu';
  funtyp=4;
  model.sim=list(junction_name,funtyp)

  model.in=-1
  model.in2=-1
  model.intyp=1
  //model.out=[siz(1);min(siz)]
  //model.out2=[min(siz);siz(2)]
  model.out=[-1;-1]
  model.out2=[-1;-1]
  model.outtyp=[1 1]
  model.evtin=[]
  model.evtout=[]
  model.state=[]
  model.dstate=[]
  model.rpar=[]
  model.ipar=[]
  model.blocktype='c' 
  model.firing=[]
  model.dep_ut=[%t %f]
  label=sci2exp(1);
  gr_i=['xstringb(orig(1),orig(2),'' LU '',sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,label,gr_i)
end
endfunction

