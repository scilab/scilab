function show_arcs(p,sup)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==1 then
  sup='pas de sup'
else
  if rhs<>2 then error(39), end 
end
// p
s=size(p)
if s(1)*s(2) == 0 then return end
if s(1)<>1 then
  error('First argument must be a row vector')
end
// sup
select sup
case 'pas de sup' then
  isup=0
case 'sup' then 
  isup=1
else
  error('Unknown argument ""'+sup+'""')  
end
m6showp(p,isup)
