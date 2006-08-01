function sz=getportsiz(o,prt_number,typ)
// Copyright INRIA
  sz=[]
  typs=['in','out','evtin','evtout']
  k=find(typ==typs)
  if k==[] then return,end
  select k
   case 1 then
     sz=[o.model.in(prt_number) o.model.in2(prt_number)]
   case 2 then
     sz=[o.model.out(prt_number) o.model.out2(prt_number)]
   case 3 then
     psiz=o.model(typs(k))
     sz=psiz(prt_number)
   case 4 then
     psiz=o.model(typs(k))
     sz=psiz(prt_number)
  end
endfunction
