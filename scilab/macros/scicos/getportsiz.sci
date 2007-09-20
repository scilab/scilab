function sz=getportsiz(o,prt_number,typ)
// Copyright INRIA
  sz=[]
  typs=['in','out','evtin','evtout']
  k=find(typ==typs)
  if k==[] then return,end
  select k
   case 1 then
     in=o.model.in(prt_number)
     if size(o.model.in2,'*')==0|...
        size(o.model.in2,'*')>=size(o.model.in,'*') then
       in2=o.model.in2(prt_number)
     else
       in2=1
     end
     sz=[in in2]
   case 2 then
     out=o.model.out(prt_number)
     if size(o.model.out2,'*')==0|...
        size(o.model.out2,'*')>=size(o.model.out2,'*') then
       out2= o.model.out2(prt_number)
     else
       out2=1
     end
     sz=[out out2]
   case 3 then
     psiz=o.model(typs(k))
     sz=psiz(prt_number)
   case 4 then
     psiz=o.model(typs(k))
     sz=psiz(prt_number)
  end
endfunction
