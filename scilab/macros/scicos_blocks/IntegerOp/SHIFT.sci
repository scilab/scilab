function [x,y,typ]=SHIFT(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[];
select job
case 'plot' then
  sgn=arg1.model.ipar
  VOP=["Arithmetic", "Circular"]
  OPER=VOP(evstr( arg1.graphics.exprs(3))+1)
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
    [ok,Datatype,nb,np,exprs]=getvalue('Set Shift block parameters',..
			    ['Datatype (3=int32  4=int16 5=int8 ...)';..
                             'Number of bits to shift left (use negatif number to shift right)';..
                             'Shifttype(0=Arithmetic 1=Circular)'],..
                             list('vec',1,'vec',1,'vec',1),exprs)
    if ~ok then break,end
    if (np~=0 & np~=1) then message ("shifttyp is not supported");ok=%f;end
    it=Datatype;
    ot=Datatype;
//    model.sim=list('shift_ia',4)
    if (Datatype==3 | Datatype==6) then
	if nb>0 then
	   select np
		case 0 then model.sim=list('shift_32_LA',4)
		case 1 then model.sim=list('shift_32_LC',4)
	   end
	elseif nb<0
	   select np
		case 0 then 
			select Datatype
			    case 3 model.sim=list('shift_32_RA',4)
			    case 6 model.sim=list('shift_u32_RA',4)
			end
		case 1 then model.sim=list('shift_32_RC',4)
	   end
	end
    elseif (Datatype==4 | Datatype==7) then
	if nb>0 then
	   select np
		case 0 then model.sim=list('shift_16_LA',4)
		case 1 then model.sim=list('shift_16_LC',4)
	   end
	elseif nb<0
	   select np
		case 0 then 
			select Datatype
			    case 4 model.sim=list('shift_16_RA',4)
			    case 7 model.sim=list('shift_u16_RA',4)
			end
		case 1 then model.sim=list('shift_16_RC',4)
	   end
	end
    elseif (Datatype==5 | Datatype==8) then
	if nb>0 then
	   select np
		case 0 then model.sim=list('shift_8_LA',4)
		case 1 then model.sim=list('shift_8_LC',4)
	   end
	elseif nb<0
	   select np
		case 0 then 
			select Datatype
			    case 5 model.sim=list('shift_8_RA',4)
			    case 8 model.sim=list('shift_u8_RA',4)
			end
		case 1 then model.sim=list('shift_8_RC',4)
	   end
	end
    else message("Datatype is not supported");ok=%f;
    end
    if ok then
      [model,graphics,ok]=set_io(model,graphics,...
                                 list([-1,-2],it),...
                                 list([-1,-2],ot),[],[])
    end
    if ok then
      model.ipar=nb
      graphics.exprs=exprs
      x.graphics=graphics;x.model=model
      break
    end
  end
case 'define' then
  sgn=[0;0]
  OPER=0;
  model=scicos_model()
  model.sim=list('shift_32_LA',4)
  model.in=-1
  model.out=-1
  model.in2=-2
  model.out2=-2
  model.intyp=3
  model.outtyp=3
  model.rpar=[]
  model.ipar=sgn
  model.blocktype='c'
  model.dep_ut=[%t %f]

  
  exprs=[sci2exp(3);sci2exp(0);sci2exp(0)]
gr_i=['xstringb(orig(1),orig(2),[OPER;''   Shift  ''],sz(1),sz(2),''fill'')']
  x=standard_define([3 2],model, exprs,gr_i)
end
endfunction
