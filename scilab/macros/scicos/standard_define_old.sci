function o = standard_define_old(sz, model, label, gr_i)
// Copyright INRIA
//initialize graphic part of the block data structure
//**    
[lhs,rhs]=argn(0)
if rhs<4 then gr_i=[],end
[nin,nout,ncin,ncout]=model(2:5)
nin=size(nin,1);if nin>0 then pin(nin,1)=0,else pin=[],end
nout=size(nout,1);if nout>0 then pout(nout,1)=0,else pout=[],end
ncin=size(ncin,1);if ncin>0 then pcin(ncin,1)=0,else pcin=[],end
ncout=size(ncout,1);if ncout>0 then pcout(ncout,1)=0,else pcout=[],end
graphics=list([0,0],sz,%t,label,pin,pout,pcin,pcout,gr_i)
if model(1)(1)=='super' then
   o=list('Block',graphics,model,' ','SUPER_f')
else
  [ln,mc]=where()
  o=list('Block',graphics,model,' ',mc(2))
end

endfunction

