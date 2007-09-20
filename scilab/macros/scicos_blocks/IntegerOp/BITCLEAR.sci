function [x,y,typ]=BITCLEAR(job,arg1,arg2)
x=[];y=[];typ=[]
select job
 case 'plot' then
  OPER=string( arg1.graphics.exprs(2))
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
  while %t do
    [ok,Datatype,bit,exprs]=getvalue('Set parameters',..
	['Datatype(3=int32 4=int16 5=int8 ...)';..
	 'index of bit (0 is leat significant)'],..
	list('vec',1,'vec',1),exprs)
    if ~ok then break,end
    in=[model.in model.in2];
    if (bit<0) then
      message('Incorrect index '+string(bit)+' ; must be >0.');ok=%f;
    end
    if floor(bit)<> bit then message("index of bit must be integer");ok=%f;end
    if (Datatype==3)|(Datatype==6) then
	if bit > 31 then message ('Incorrect index '+string(bit)+' ; must be <32.');ok=%f;
	else 
	 bit=uint32(bit)
	 n=(2^32-1)-2^bit;
	 n=uint32(n)
	 model.sim=list('bit_clear_32',4)
	end
    elseif (Datatype==4)|(Datatype==7) then
	if bit > 15 then message ('Incorrect index '+string(bit)+' ; must be <16.');ok=%f;
	else 
	 bit=uint16(bit)
	 n=(2^16-1)-2^bit;
	 n=uint16(n)
	 model.sim=list('bit_clear_16',4)
	end
    elseif (Datatype==5)|(Datatype==8) then
	if bit > 7 then message ('Incorrect index '+string(bit)+' ; must be <8.');ok=%f;
	else 
	 bit=uint8(bit)
	 n=(2^8-1)-2^bit;
	 n=uint8(n)
	 model.sim=list('bit_clear_8',4)
	end
    else message ('Datatype '+string(Datatype)+' is not supported ; It must be 3 to 8');ok=%f;
    end
      if ok then
	it=Datatype
	ot=Datatype
	out=[1 1]
	[model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),[],[])
      end
      if ok then
	graphics.exprs=exprs;
	model.opar=list(n)
	x.graphics=graphics;x.model=model;
	break
      end
  end

case 'define' then
  model=scicos_model()
  model.sim=list('bit_clear_32',4)
  model.in=1
  model.in2=1
  model.out=1
  model.out2=1;
  model.intyp=3
  model.outtyp=3
  model.opar=list(int32(0))
  model.blocktype='c'
  model.dep_ut=[%t %f]
  
  exprs=[sci2exp(3);sci2exp(0)]
  gr_i=['xstringb(orig(1),orig(2),[''Clear'';''bit: ''+OPER],sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,exprs,gr_i)
  
end
endfunction
