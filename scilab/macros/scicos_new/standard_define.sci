function o=standard_define(sz,model,label,gr_i)
//initialize graphic part of the block data structure
// Copyright INRIA

  if argn(2)<4 then gr_i=[],end

  nin=size(model.in,1);
  if nin>0 then pin(nin,1)=0,else pin=[],end

  nout=size(model.out,1);
  if nout>0 then pout(nout,1)=0,else pout=[],end

  ncin=size(model.evtin,1);
  if ncin>0 then pein(ncin,1)=0,else pein=[],end

  ncout=size(model.evtout,1);
  if ncout>0 then peout(ncout,1)=0,else peout=[],end
  
  if type(gr_i)<>15 then gr_i=list(gr_i,8),end
  if gr_i(2)==[] then gr_i(2)=8,end
  if gr_i(2)==0 then gr_i(2)=[],end

  graphics=scicos_graphics(sz=sz,pin=pin,pout=pout,pein=pein,peout=peout, ...
			   gr_i=gr_i,exprs=label) 
  
//  if model.sim(1)=='super' then
//    o=scicos_block(graphics=graphics,model=model,gui='SUPER_f')
//  else
    [ln,mc]=where()
    o=scicos_block(graphics=graphics,model=model,gui=mc(2))
//  end
endfunction
