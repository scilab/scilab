function SelectAll_()
// Copyright INRIA
Cmenu=[];%pt=[];

Select=[];
for k=1:lstsize(scs_m.objs)
   o=scs_m.objs(k)
   if typeof(o)<>'Deleted' then
       Select=[Select;[k,curwin]]
   end
end
endfunction
