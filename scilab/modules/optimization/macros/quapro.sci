function [x,lagr,f]=quapro(Q,p,C,b,ci,cs,mi,x0,imp)
//[x,lagr,f]=quapro(Q,p,C,b [,x0])
//[x,lagr,f]=quapro(Q,p,C,b,ci,cs [,x0])
//[x,lagr,f]=quapro(Q,p,C,b,ci,cs,mi [,x0])
//[x,lagr,f]=quapro(Q,p,C,b,ci,cs,mi,x0 [,imp])
// Copyright INRIA
[lhs,rhs]=argn(0)

if rhs==9 then
//quapro(Q,p,C,b,ci,cs,mi,x0,imp)
//define modo,x0 for qpqpqp
   modo=3;
   if x0=='v' then modo=1;x0=0*ones(size(Q,1),1);end
   if x0=='g' then modo=2;x0=0*ones(size(Q,1),1);end
   [x,f,lagr]=qpqpqp(x0,Q,p,C',b,ci,cs,mi,modo,imp)
   return
end
if rhs==8 then
//quapro(Q,p,C,b,ci,cs,mi,x0)
// define modo, true x0,imp
   imp=0;modo=3;
   if x0=='v' then modo=1;x0=0*ones(size(Q,1),1);end
   if x0=='g' then modo=2;x0=0*ones(size(Q,1),1);end
   [x,f,lagr]=qpqpqp(x0,Q,p,C',b,ci,cs,mi,modo,imp)
   return;
end
if rhs==7 then
//quapro(Q,p,C,b,ci,cs,mi) or quapro(Q,p,C,b,ci,cs,x0) 
// define modo,x0,imp           define modo,mi,imp
   if size(mi,'*')==1 then
     //mi is given
     imp=0;
     x0=0*ones(size(Q,1),1);modo=1;
   else
     //x0 is given
     imp=0;x0=mi;
     mi=0;modo=3;
     if x0=='v' then modo=1;x0=0*ones(size(Q,1),1);end
     if x0=='g' then modo=2;x0=0*ones(size(Q,1),1);end
   end
   [x,f,lagr]=qpqpqp(x0,Q,p,C',b,ci,cs,mi,modo,imp)
   return
end
if rhs==6 then
//quapro(Q,p,C,b,ci,cs)
// define modo, x0, mi, imp
   imp=0;x0=0*ones(size(Q,1),1);modo=1;mi=0;
   [x,f,lagr]=qpqpqp(x0,Q,p,C',b,ci,cs,mi,modo,imp)
   return
end
if rhs==5 then
//quapro(Q,p,C,b,x0)
// define modo, ci, cs, mi, imp, true x0
   modo=3;
   if x0=='v' then modo=1;x0=0*ones(size(Q,1),1);end
   if x0=='g' then modo=2;x0=0*ones(size(Q,1),1);end
   ci=[];cs=[];mi=0;imp=0;
   [x,f,lagr]=qpqpqp(x0,Q,p,C',b,ci,cs,mi,modo,imp)
   return
end
if rhs==4 then
//quapro(Q,p,C,b)
// define modo, ci, cs, mi, imp, x0
   modo=1;x0=0*ones(size(Q,1),1);
   ci=[];cs=[];mi=0;imp=0;
   [x,f,lagr]=qpqpqp(x0,Q,p,C',b,ci,cs,mi,modo,imp)
   return
end
endfunction
