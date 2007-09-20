function [x,y,typ]=REGISTER(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[]
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
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
  if size(exprs,1)==1 then exprs=[exprs,sci2exp(1)];end
  while %t do
    [ok,z0,it,exprs]=getvalue('Set delay parameters',..
	['Register initial condition';..
         'Datatype (1=double 3=int32 ...)'],..
          list('vec',-1,'vec',1),exprs)
    if ~ok then break,end
    if prod(size(z0))<2 then
      message('Register length must be at least 2')
      ok=%f
    end
    if it==1 then
       model.sim=list('delay4',4);z0=double(z0);
       model.dstate=z0;model.odstate=list();
    else
      if it==3 then
	model.sim=list('delay4_i32',4)
	z0=int32(z0)
      elseif it==4 then
	model.sim=list('delay4_i16',4)
	z0=int16(z0)
      elseif it==5 then
	model.sim=list('delay4_i8',4)
	z0=int8(z0)
      elseif it==6 then
	model.sim=list('delay4_ui32',4)
	z0=uint32(z0)
      elseif it==7 then
	model.sim=list('delay4_ui16',4)
	z0=uint16(z0)
      elseif it==8 then
	model.sim=list('delay4_ui8',4)
	z0=uint8(z0)
      else message("Datatype is not supported");ok=%f
      end
      model.odstate=list(z0);model.dstate=[]
    end
    if ok then 
	in=[1 1]
	[model,graphics,ok]=set_io(model,graphics,list(in,it),list(in,it),1,[])
    end
    if ok then
      graphics.exprs=exprs;
      x.graphics=graphics;x.model=model
      break
    end
  end

case 'define' then
  z0=zeros(10,1)
  model=scicos_model()
  model.sim=list('delay4',4)
  model.in=1
  model.out=1
  model.evtin=1
  model.dstate=z0
  model.blocktype='d'
  model.dep_ut=[%f %f]

  exprs=strcat(string(z0),';')
  gr_i=['dly=model.rpar;';
    'txt=[''Shift'';''Register'';string(dly)];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']
  x=standard_define([2.5 2.5],model,exprs,gr_i)
end
endfunction
