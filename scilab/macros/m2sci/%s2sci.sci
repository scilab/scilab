function [stk,txt,top]=%s2sci()
// genere le code relatif a la soustraction et au changement de signe
//!
// Copyright INRIA
txt=[]
s2=stk(top)
if s2(2)=='2'|s2(2)=='3' then s2(1)='('+s2(1)+')',end
if op(3)=='2' then
   s1=stk(top-1)
   if s1(2)=='3' then s1(1)='('+s1(1)+')',end
   if s1(3)=='1'&s1(4)=='1' then
     stk=list(s1(1)+'-'+s2(1),'2',s2(3),s2(4),s1(5))
   elseif s2(3)=='1'&s2(4)=='1' then
     stk=list(s1(1)+'-'+s2(1),'2',s1(3),s1(4),s1(5))
   else
     stk=list(s1(1)+'-'+s2(1),'2',s1(3),s1(4),s1(5))
   end
   top=top-1
else
  stk=list('-'+s2(1),'2',s2(3),s2(4),s2(5))
end



