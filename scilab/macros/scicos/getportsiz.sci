function sz=getportsiz(o,prt_number,typ)
// Copyright INRIA
sz=[]
typs=['in','out','clkin','clkout']
k=find(typ==typs)+1
if k==[] then return,end
model=o(3)
psiz=model(k)
sz=psiz(prt_number)


