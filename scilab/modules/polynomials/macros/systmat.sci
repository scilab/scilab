function [Sm]=systmat(Sl);
// System matrix of the linear system Sl (syslin list)
// in state-space form.
// Sm = [-sE + A   B;
//      [    C     D]
// To get the zeros use det or detr (for square systems)
//!
// Copyright INRIA
ty=Sl(1);
ty=ty(1);
if ty=='lss' then
if Sl(7)=='d' then
     s=poly(0,'z');
else
    s=poly(0,'s');
end
Sm=[-s*eye(Sl(2))+Sl(2),Sl(3);Sl(4),Sl(5)];
return
end
if part(ty,1)=='d' then
s=poly(0,'s');
Sm=[-s*Sl(6)+Sl(2),Sl(3);Sl(4),Sl(5)];
return
end
endfunction
