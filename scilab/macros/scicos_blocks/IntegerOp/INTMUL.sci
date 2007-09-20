function [x,y,typ]=INTMUL(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[];
select job
case 'plot' then
  sgn=arg1.model.ipar
  standard_draw(arg1)
case 'getinputs' then
  [x,y,typ]=standard_inputs(arg1)
case 'getoutputs' then
  [x,y,typ]=standard_outputs(arg1)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1.graphics
  model=arg1.model
  exprs=graphics.exprs
  while %t do
    [ok,Datatype,np,exprs]=getvalue('Set INTMUL block parameters',..
			    ['Datatype (3=int32  4=int16 5=int8 ...)';..
                             'Do on Overflow(0=Nothing 1=Saturate 2=Error)'],..
                             list('vec',1,'vec',1),exprs)
    if ~ok then break,end
    if (np~=0 & np~=1 & np~=2) then message ("type is not supported");ok=%f;end
    it=Datatype*ones(1,2);
    ot=Datatype;
    if Datatype==3 then
	if np==0 then
           model.sim=list('matmul_i32n',4)
	elseif np==1 then
           model.sim=list('matmul_i32s',4)
	else 
	   model.sim=list('matmul_i32e',4)
	end
    elseif Datatype==4 then
	if np==0 then
           model.sim=list('matmul_i16n',4)
	elseif np==1 then
           model.sim=list('matmul_i16s',4)
	else 
	   model.sim=list('matmul_i16e',4)
	end
    elseif Datatype==5 then
	if np==0 then
           model.sim=list('matmul_i8n',4)
	elseif np==1 then
           model.sim=list('matmul_i8s',4)
	else 
	   model.sim=list('matmul_i8e',4)
	end
    elseif Datatype==6 then
	if np==0 then
           model.sim=list('matmul_ui32n',4)
	elseif np==1 then
           model.sim=list('matmul_ui32s',4)
	else 
	   model.sim=list('matmul_ui32e',4)
	end
    elseif Datatype==7 then
	if np==0 then
           model.sim=list('matmul_ui16n',4)
	elseif np==1 then
           model.sim=list('matmul_ui16s',4)
	else 
	   model.sim=list('matmul_ui16e',4)
	end
    elseif Datatype==8 then
	if np==0 then
           model.sim=list('matmul_ui8n',4)
	elseif np==1 then
           model.sim=list('matmul_ui8s',4)
	else 
	   model.sim=list('matmul_ui8e',4)
	end
    else message("Datatype is not supported");ok=%f;
    end
    in=[model.in model.in2]
    out=[model.out model.out2]
    if ok then
      [model,graphics,ok]=set_io(model,graphics,...
                                 list(in,it),...
                                 list(out,ot),[],[])
    end
    if ok then
      model.ipar=np
      graphics.exprs=exprs
      x.graphics=graphics;x.model=model
      break
    end
  end
case 'define' then
  sgn=0
  model=scicos_model()
  model.sim=list('matmul_i32',4)
  model.in=[-1;-2]
  model.out=-1
  model.in2=[-2;-3]
  model.out2=-3
  model.intyp=[3 3]
  model.outtyp=3
  model.rpar=[]
  model.ipar=sgn
  model.blocktype='c'
  model.dep_ut=[%t %f]

  
  exprs=[sci2exp(3);sci2exp(0)]
gr_i=['txt=['' INTMUL ''];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']
  x=standard_define([2 2],model, exprs,gr_i)
end
endfunction
