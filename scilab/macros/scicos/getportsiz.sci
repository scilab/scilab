function sz=getportsiz(o,prt_number,typ)
// Copyright INRIA
  sz=[]
  typs=['in','out','evtin','evtout']
  k=find(typ==typs)
  if k==[] then return,end
  psiz=o.model(typs(k))
  sz=psiz(prt_number)
endfunction
