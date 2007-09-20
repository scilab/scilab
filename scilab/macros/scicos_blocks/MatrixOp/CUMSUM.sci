function [x,y,typ]=CUMSUM(job,arg1,arg2)
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
    [ok,typ,decomptyp,lab]=getvalue('Set CUMSUM block parameters',['Datatype(1=real double  2=Complex)';'Sum along (0=the first non singleton dimension  1=Rows  2=Columns)'],list('vec',1,'vec',1),label)
    if ~ok then break,end
    label=lab
	if (typ==1) then
	if (decomptyp==0) then
		junction_name='cumsum_m';
		out=[-1 -2]
	elseif (decomptyp==1) then
		junction_name='cumsum_r';
		out=[-1 1];
	elseif (decomptyp==2) then
		junction_name='cumsum_c';
		out=[1 -2];
	else message('decomposition type is not supported');ok=%f;
 	end
	it=1;
	ot=1;
	elseif (typ==2) then
	if (decomptyp==0) then
		junction_name='cumsumz_m';
	elseif (decomptyp==1) then
		junction_name='cumsumz_r';
	elseif (decomptyp==2) then
		junction_name='cumsumz_c';
	else message('decomposition type is not supported');ok=%f;
 	end
	it=2;
	ot=2
	else message("Datatype is not supported");ok=%f;
	end
	in=[model.in model.in2];
	out=[model.out model.out2];
	funtyp=4;
    if ok then
	[model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),[],[])
    end
    if ok then
      model.sim=list(junction_name,funtyp);
      arg1.model=model
      graphics.exprs=label
      arg1.graphics=graphics
      x=arg1
      break
    end
  end
case 'define' then
  model=scicos_model()
  junction_name='cumsum_m';
  funtyp=4;
  model.sim=list(junction_name,funtyp)

  model.in=-1
  model.in2=-2
  model.intyp=1
  model.out=-1
  model.out2=-2
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
  label=[sci2exp(1),sci2exp(0)];
  gr_i=['xstringb(orig(1),orig(2),''CUMSUM'',sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,label,gr_i)
end
endfunction

