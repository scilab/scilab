function [txt]=lin2for(lst,ln)
//si lst est une liste representant une macro compilee,mac2for)
//restourne le code ascii de la macro
//!
// Copyright INRIA
count=0;level=0
[kt,count]=findlin(lst,ln)
nt=prod(size(kt))
for i=nt:-1:2
  lst=lst(kt(i))
end
setparam()
 
txt=ins2for(lst,1)
endfunction
