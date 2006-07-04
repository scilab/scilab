function sztyp=getporttyp(o,prt_number,typ)
// Copyright INRIA
  sztyp=[]
  typs=['in','out']
  k=find(typ==typs)
  if k==[] then return,end

  select k
   case 1 then
     if size(o.model.intyp,'*')<prt_number then //for compatibilty
      sztyp=1;
     else
      sztyp=o.model.intyp(prt_number);
     end
   case 2 then
     if size(o.model.outtyp,'*')<prt_number then //for compatibilty
      sztyp=1;
     else
      sztyp=o.model.outtyp(prt_number)
     end
  end
endfunction
