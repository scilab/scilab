function [stk,txt,top]=sci_global()
// Copyright INRIA
txt=[]
vars=[]
vnms;vtps;
for k=1:rhs
  s=stk(top)(1)
  if s<>'''''' then
    nam=part(s,2:length(s)-1)
    if find(nam==vnms(:,2))==[] then 
      vnms($+1,:)=[nam,nam]
      vtps($+1)=list('?','?','?',0)
    end
    vars=[s,vars]
  end
  top=top-1
end

stk=list('global('+makeargs(vars)+')','0','0','0','1')
[vnms,vtps]=resume(vnms,vtps)
endfunction
