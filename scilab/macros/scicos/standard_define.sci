function o=standard_define(sz,model,label,gr_i)
//initialize graphic part of the block data structure
// Copyright INRIA

  if argn(2)<4 then gr_i=[],end

  nin=size(model.in,1);
  if nin>0 then pin(nin,1)=0,else pin=[],end

  nout=size(model.out,1);
  if nout>0 then pout(nout,1)=0,else pout=[],end

  ncin=size(model.evtin,1);
  if ncin>0 then pcin(ncin,1)=0,else pcin=[],end

  ncout=size(model.evtout,1);
  if ncout>0 then pcout(ncout,1)=0,else pcout=[],end

  graphics=scicos_graphics(sz=sz,pin=pin,pout=pout,pcin=pcin,pcout=pcout, ...
			   gr_i=gr_i,exprs=label) 
  
  if model.sim(1)=='super' then
    o=scicos_block(graphics=graphics,model=model,gui='SUPER_f')
  else
    [ln,mc]=where()
    o=scicos_block(graphics=graphics,model=model,gui=mc(2))
  end
endfunction
