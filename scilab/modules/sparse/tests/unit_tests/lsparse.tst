// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

lines(0)
zer=sparse([],[],[6,6]);
//
//
//-------------------------------------------------------------
//test des primitives sparse, full
//-------------------------------------------------------------
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);A=full(a);
b=sparse([1 6;1 2;5 5],[-1;-1;-1],[5 6]);B=full(b);
if or(full(a<>0)<>(A<>0)) then pause,end
if or(full(b<>0)<>(B<>0)) then pause,end
if or(full(b>0)<>(B>0)) then pause,end
if or(full(b<0)<>(B<0)) then pause,end
if or(full(a>100)<>(A>100)) then pause,end
if sparse(B>0)<>(b>0) then pause,end
if sparse(B<0)<>(b<0) then pause,end
//-------------------------------------------------------------
//test de la primitives spget
//-------------------------------------------------------------
//-----------------------------------------------
// | and & tests
//-----------------------------------------------
// -- sparse sparse
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);a=a<>0;A=full(a);
b=sparse([1 6;1 2;5 5],[-1;-1;-1],[6 6]);b=b<>0;B=full(b);
if (a|b)<>(A|B) then pause,end
if (a&b)<>(A&B) then pause,end
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);a=a>100;A=full(a);
if (a|b)<>(A|B) then pause,end
if (a&b)<>(A&B) then pause,end
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);a=a<>0;A=full(a);
b=sparse([1 6;1 2;5 5],[-1;-1;-1],[6 6]);b=b>0;B=full(b);
if (a|b)<>(A|B) then pause,end
if (a&b)<>(A&B) then pause,end
b=sparse([1 6;1 2;5 5],[-1;-1;-1],[6 6]);b=b>0;B=full(b);
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);a=a>100;A=full(a);
if (a|b)<>(A|B) then pause,end
if (a&b)<>(A&B) then pause,end
// -- sparse full
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);a=a<>0;A=full(a);
b=sparse([1 6;1 2;5 5],[-1;-1;-1],[6 6]);b=b<>0;B=full(b);
if (a|B)<>(A|B) then pause,end
if (a&B)<>(A&B) then pause,end
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);a=a>100;A=full(a);
if (a|B)<>(A|B) then pause,end
if (a&B)<>(A&B) then pause,end
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);a=a<>0;A=full(a);
b=sparse([1 6;1 2;5 5],[-1;-1;-1],[6 6]);b=b>0;B=full(b);
if (a|B)<>(A|B) then pause,end
if (a&B)<>(A&B) then pause,end
b=sparse([1 6;1 2;5 5],[-1;-1;-1],[6 6]);b=b>0;B=full(b);
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);a=a>100;A=full(a);
if (a|B)<>(A|B) then pause,end
if (a&B)<>(A&B) then pause,end
// -- full sparse
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);a=a<>0;A=full(a);
b=sparse([1 6;1 2;5 5],[-1;-1;-1],[6 6]);b=b<>0;B=full(b);
if (A|b)<>(A|B) then pause,end
if (A&b)<>(A&B) then pause,end
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);a=a>100;A=full(a);
if (A|b)<>(A|B) then pause,end
if (A&b)<>(A&B) then pause,end
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);a=a<>0;A=full(a);
b=sparse([1 6;1 2;5 5],[-1;-1;-1],[6 6]);b=b>0;B=full(b);
if (A|b)<>(A|B) then pause,end
if (A&b)<>(A&B) then pause,end
b=sparse([1 6;1 2;5 5],[-1;-1;-1],[6 6]);b=b>0;B=full(b);
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);a=a>100;A=full(a);
if (A|b)<>(A|B) then pause,end
if (A&b)<>(A&B) then pause,end
// ----------------------------------------------------------
// test de la transposition
//-----------------------------------------------------------
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);a=a<>0;A=full(a);
b=sparse([1 6;1 2;5 5],[-1;-1;-1],[6 6]);b=b<>0;B=full(b);
if or(full(a')<>full(a)') then pause,end
if or(full(b')<>full(b)') then pause,end
b=sparse([1 6;1 2;5 5],[-1;-1;-1],[6 6]);b=b>0;B=full(b);
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);a=a>100;A=full(a);
if or(full(a')<>full(a)') then pause,end
if or(full(b')<>full(b)') then pause,end
// ----------------------------------------------------------
// test des concatenation
//-----------------------------------------------------------
// sparse sparse
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);a=a<>0;A=full(a);
b=sparse([1 6;1 2;5 5],[-1;-1;-1],[6 6]);b=b<>0;B=full(b);
if or(full([a])<>A) then pause,end
if or(full([a b])<>[A B]) then pause,end
if or(full([a;b])<>[A;B]) then pause,end
if or(full([a []])<>A) then pause,end
if or(full([a;[]])<>A) then pause,end
// sparse full
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);a=a<>0;A=full(a);
b=sparse([1 6;1 2;5 5],[-1;-1;-1],[6 6]);b=b<>0;B=full(b);
if or(full([a B])<>[A B]) then pause,end
if or(full([a;B])<>[A;B]) then pause,end
b=sparse([1 6;1 2;5 5],[-1;-1;-1],[6 6]);b=b>0;B=full(b);
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);a=a>100;A=full(a);
if or(full([a B])<>[A B]) then pause,end
if or(full([a;B])<>[A;B]) then pause,end
// full sparse
if or(full([A b])<>[A B]) then pause,end
if or(full([A;b])<>[A;B]) then pause,end
b=sparse([1 6;1 2;5 5],[-1;-1;-1],[6 6]);b=b>0;B=full(b);
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);a=a>100;A=full(a);
if or(full([A b])<>[A B]) then pause,end
if or(full([B;b])<>[A;B]) then pause,end
// ----------------------------------------------------------
// test des extractions
//-----------------------------------------------------------
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);a=a<>0;af=full(a);
if or(full(a(1,3))<>af(1,3)) then pause,end
if or(full(a(1,4))<>af(1,4)) then pause,end
if or(full(a(1,:))<>af(1,:)) then pause,end
v=sparse([2 1;3 1;4 1;6 1],[10;11;12;13],[6,1]);v=v<>0;vf=full(v);
if or(full(v(:))<>vf(:)) then pause,end
if or(full(v(3:4))<>vf(3:4)) then pause,end
if or(full(v([1 5]))<>vf([1 5])) then pause,end
if or(full(v([4 3]))<>vf([4 3])) then pause,end
if or(full(v([4 4]))<>vf([4 4])) then pause,end
if or(full(v([1 1]))<>vf([1 1])) then pause,end
v=v';vf=vf';
if or(full(v(:))<>vf(:)) then pause,end
if or(full(v(3:4))<>vf(3:4)) then pause,end
if or(full(v([1 5]))<>vf([1 5])) then pause,end
if or(full(v([4 3]))<>vf([4 3])) then pause,end
if or(full(v([4 4]))<>vf([4 4])) then pause,end
if or(full(v([1 1]))<>vf([1 1])) then pause,end
// full line insertion
//----------------------
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);a=a<>0;af=full(a);
vt=sparse([1 2;1 3;1 4;1 6],[10;11;12;13],[1,6]);vt=vt<>0;
a1=a;a1(1,:)=vt;A=full(a);Vt=full(vt);A(1,:)=Vt;
if or(full(a1)<>A) then pause,end
a1=a;a1(3,:)=vt;A=full(a);Vt=full(vt);A(3,:)=Vt;
if or(full(a1)<>A) then pause,end
a1=a;a1(1,:)=vt;A=full(a);Vt=full(vt);A(1,:)=Vt;
if or(full(a1)<>A) then pause,end
a1=a;a1(3,:)=vt;A=full(a);Vt=full(vt);A(3,:)=Vt;
if or(full(a1)<>A) then pause,end
a1=a;a1(7,:)=vt;A=full(a);Vt=full(vt);A(7,:)=Vt;
if or(full(a1)<>A) then pause,end
a1=a;a1(8,:)=vt;A=full(a);Vt=full(vt);A(8,:)=Vt;
if or(full(a1)<>A) then pause,end
b=sparse([1 1;1 3;1 6;2 1;2 2;2 4],10:15,[2,6])<>0;
a1=a;a1([1 3],:)=b;A=full(a);B=full(b);A([1 3],:)=B;
if or(full(a1)<>A) then pause,end
a1=a;a1([3 1],:)=b;A=full(a);B=full(b);A([3 1],:)=B;
if or(full(a1)<>A) then pause,end
b=sparse([1 1;1 3;1 6;2 1;2 2;2 4;3 3;3 5],10:17,[3,6])<>0;
a1=a;a1([1 3 1],:)=b;A=full(a);B=full(b);A([1 3 1],:)=B;
if or(full(a1)<>A) then pause,end
//  insert zero vector
vt=sparse([],[],[1,6])<>0;
a1=a;a1(1,:)=vt;A=full(a);Vt=full(vt);A(1,:)=Vt;
if or(full(a1)<>A) then pause,end
a1=a;a1(3,:)=vt;A=full(a);Vt=full(vt);A(3,:)=Vt;
if or(full(a1)<>A) then pause,end
a1=a;a1(1,:)=vt;A=full(a);Vt=full(vt);A(1,:)=Vt;
if or(full(a1)<>A) then pause,end
a1=a;a1(3,:)=vt;A=full(a);Vt=full(vt);A(3,:)=Vt;
if or(full(a1)<>A) then pause,end
a1=a;a1(7,:)=vt;A=full(a);Vt=full(vt);A(7,:)=Vt;
if or(full(a1)<>A) then pause,end
a1=a;a1(8,:)=vt;A=full(a);Vt=full(vt);A(8,:)=Vt;
if or(full(a1)<>A) then pause,end
b=sparse([1 1;1 3;1 6;2 1;2 2;2 4],10:15,[2,6]);b=b<>0;
a1=a;a1([1 3],:)=b;A=full(a);B=full(b);A([1 3],:)=B;
if or(full(a1)<>A) then pause,end
a1=a;a1([3 1],:)=b;A=full(a);B=full(b);A([3 1],:)=B;
if or(full(a1)<>A) then pause,end
b=sparse([1 1;1 3;1 6;2 1;2 2;2 4;3 3;3 5],10:17,[3,6]);b=b<>0;
a1=a;a1([1 3 1],:)=b;A=full(a);B=full(b);A([1 3 1],:)=B;
if or(full(a1)<>A) then pause,end
// full column insertion
//----------------------
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);a=a<>0;
v=sparse([2 1;3 1;4 1;6 1],[10;11;12;13],[6,1]);v=v<>0;
a1=a;a1(:,1)=v;A=full(a);V=full(v);A(:,1)=V;
if or(full(a1)<>A) then pause,end
a1=a;a1(:,2)=v;A=full(a);V=full(v);A(:,2)=V;
if or(full(a1)<>A) then pause,end
a1=a;a1(:,1)=v;A=full(a);V=full(v);A(:,1)=V;
if or(full(a1)<>A) then pause,end
a1=a;a1(:,2)=v;A=full(a);V=full(v);A(:,2)=V;
if or(full(a1)<>A) then pause,end
a1=a;a1(:,3)=v;A=full(a);V=full(v);A(:,3)=V;
if or(full(a1)<>A) then pause,end
//
a1=a;a1(:,7)=v;A=full(a);V=full(v);A(:,7)=V;
if or(full(a1)<>A) then pause,end
a1=a;a1(:,8)=v;A=full(a);V=full(v);A(:,8)=V;
if or(full(a1)<>A) then pause,end
b=sparse([1 2;2 1;3 1;3 2;4 1;6 2],10:15,[6,2]);b=b<>0;
a1=a;a1(:,[1 3])=b;A=full(a);B=full(b);A(:,[1 3])=B;
if or(full(a1)<>A) then pause,end
a1=a;a1(:,[3 1])=b;A=full(a);B=full(b);A(:,[3 1])=B;
if or(full(a1)<>A) then pause,end
b=sparse([1 2;2 1;2 3;3 1;3 2;4 1;5 3;6 2],10:17,[6,3]);b=b<>0;
a1=a;a1(:,[1 3 1])=b;A=full(a);B=full(b);A(:,[1 3 1])=B;
if or(full(a1)<>A) then pause,end
// row column insertion
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);a=a<>0;
b=sparse([1 2;1 3;3 3],-(1:3),[3,3]);b=b<>0;
a1=a;a1(1,1)=sparse(%t);A=full(a);A(1,1)=%t;
if or(full(a1)<>A) then pause,end
a1=a;a1(1,6)=sparse(%t);A=full(a);A(1,6)=%t;
if or(full(a1)<>A) then pause,end
a1=a;a1(1,8)=sparse(%t);A=full(a);A(1,8)=%t;
if or(full(a1)<>A) then pause,end
a1=a;a1(1:3,1:3)=b;A=full(a);A(1:3,1:3)=full(b);
if or(full(a1)<>A) then pause,end
a1=a;a1(1:3,6:8)=b;A=full(a);A(1:3,6:8)=full(b);
if or(full(a1)<>A) then pause,end
a1=a;a1(6:8,1:3)=b;A=full(a);A(6:8,1:3)=full(b);
if or(full(a1)<>A) then pause,end
a1=a;a1([3 2 1],1:3)=b;A=full(a);A([3 2 1],1:3)=full(b);
if or(full(a1)<>A) then pause,end
a1=a;a1([1 2 1],1:3)=b;A=full(a);A([1 2 1],1:3)=full(b);
if or(full(a1)<>A) then pause,end
a1=a;a1([3 2 1],[3 2 1])=b;A=full(a);A([3 2 1],[3 2 1])=full(b);
if or(full(a1)<>A) then pause,end
a1=a;a1([1 2 1],[3 2 1])=b;A=full(a);A([1 2 1],[3 2 1])=full(b);
if or(full(a1)<>A) then pause,end
a1=a;a1([1 2 1],[1 2 1])=b;A=full(a);A([1 2 1],[1 2 1])=full(b);
if or(full(a1)<>A) then pause,end
//sparse full
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);a=a<>0;
vt=[%t %f %t %t %t %t];
// full line insertion
//----------------------
a1=a;a1(1,:)=vt;A=full(a);A(1,:)=vt;
if or(full(a1)<>A) then pause,end
a1=a;a1(3,:)=vt;A=full(a);A(3,:)=vt;
if or(full(a1)<>A) then pause,end
a1=a;a1(1,:)=vt;A=full(a);A(1,:)=vt;
if or(full(a1)<>A) then pause,end
a1=a;a1(3,:)=vt;A=full(a);A(3,:)=vt;
if or(full(a1)<>A) then pause,end
a1=a;a1(7,:)=vt;A=full(a);A(7,:)=vt;
if or(full(a1)<>A) then pause,end
a1=a;a1(8,:)=vt;A=full(a);A(8,:)=vt;
if or(full(a1)<>A) then pause,end
b=[%t %f %t %t %t %t;%f %f %t %t %f %t];
a1=a;a1([1 3],:)=b;A=full(a);A([1 3],:)=b;
if or(full(a1)<>A) then pause,end
a1=a;a1([3 1],:)=b;A=full(a);A([3 1],:)=b;
if or(full(a1)<>A) then pause,end
b=[%t %f %t %t %t %t;%f %f %t %t %f %t;%t %f %t %f %t %t];
a1=a;a1([1 3 1],:)=b;A=full(a);A([1 3 1],:)=b;
if or(full(a1)<>A) then pause,end
// full column insertion
//----------------------
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);a=a<>0;
v=[%t %f %t %t %t %t]';
a1=a;a1(:,1)=v;A=full(a);A(:,1)=v;
if or(full(a1)<>A) then pause,end
a1=a;a1(:,2)=v;A=full(a);A(:,2)=v;
if or(full(a1)<>A) then pause,end
a1=a;a1(:,1)=v;A=full(a);A(:,1)=v;
if or(full(a1)<>A) then pause,end
a1=a;a1(:,2)=v;A=full(a);A(:,2)=v;
if or(full(a1)<>A) then pause,end
a1=a;a1(:,3)=v;A=full(a);A(:,3)=v;
if or(full(a1)<>A) then pause,end
//
a1=a;a1(:,7)=v;A=full(a);A(:,7)=v;
if or(full(a1)<>A) then pause,end
a1=a;a1(:,8)=v;A=full(a);A(:,8)=v;
if or(full(a1)<>A) then pause,end
b=[%t %f %t %t %t %t;%f %f %t %t %f %t]';
a1=a;a1(:,[1 3])=b;A=full(a);A(:,[1 3])=b;
if or(full(a1)<>A) then pause,end
a1=a;a1(:,[3 1])=b;A=full(a);A(:,[3 1])=b;
if or(full(a1)<>A) then pause,end
b=[%t %f %t %t %t %t;%f %f %t %t %f %t;%f %t %t %t %t %f]';
a1=a;a1(:,[1 3 1])=b;A=full(a);A(:,[1 3 1])=b;
if or(full(a1)<>A) then pause,end
// row column insertion
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);a=a<>0;
b=[%t %t %f;%f %t %f;%t %t %t];
a1=a;a1(1,1)=%t;A=full(a);A(1,1)=%t;
if or(full(a1)<>A) then pause,end
a1=a;a1(1,6)=%t;A=full(a);A(1,6)=%t;
if or(full(a1)<>A) then pause,end
a1=a;a1(1,8)=%t;A=full(a);A(1,8)=%t;
if or(full(a1)<>A) then pause,end
a1=a;a1(1:3,1:3)=b;A=full(a);A(1:3,1:3)=b;
if or(full(a1)<>A) then pause,end
a1=a;a1(1:3,6:8)=b;A=full(a);A(1:3,6:8)=b;
if or(full(a1)<>A) then pause,end
a1=a;a1(6:8,1:3)=b;A=full(a);A(6:8,1:3)=b;
if or(full(a1)<>A) then pause,end
a1=a;a1([3 2 1],1:3)=b;A=full(a);A([3 2 1],1:3)=b;
if or(full(a1)<>A) then pause,end
a1=a;a1([1 2 1],1:3)=b;A=full(a);A([1 2 1],1:3)=b;
if or(full(a1)<>A) then pause,end
a1=a;a1([3 2 1],[3 2 1])=b;A=full(a);A([3 2 1],[3 2 1])=b;
if or(full(a1)<>A) then pause,end
a1=a;a1([1 2 1],[3 2 1])=b;A=full(a);A([1 2 1],[3 2 1])=b;
if or(full(a1)<>A) then pause,end
a1=a;a1([1 2 1],[1 2 1])=b;A=full(a);A([1 2 1],[1 2 1])=b;
if or(full(a1)<>A) then pause,end
// vector insertion
v=sparse([1 1;3 1;6 1],[10 11 12],[6 1]);v=v<>0;
v1=v;v1(1)=%f;V=full(v);V(1)=%f;
if or(full(v1)<>V) then pause,end
v1=v;v1(2)=%t;V=full(v);V(2)=%t;
if or(full(v1)<>V) then pause,end
v1=v;v1(8)=%f;V=full(v);V(8)=%f;
if or(full(v1)<>V) then pause,end
v1=v;v1([1 2 8])=[%t;%t;%f];V=full(v);V([1 2 8])=[%t;%t;%f];
if or(full(v1)<>V) then pause,end
v1=v;v1(:)=[];
if or(full(v1)<>[]) then pause,end
v1=v;v1(1)=sparse(%f);V=full(v);V(1)=%f;
if or(full(v1)<>V) then pause,end
v1=v;v1(2)=sparse(%f);V=full(v);V(2)=%f;
if or(full(v1)<>V) then pause,end
v1=v;v1(8)=sparse(%f);V=full(v);V(8)=%f;
if or(full(v1)<>V) then pause,end
v1=v;v1([1 2 8])=sparse([%f;%t;%f]);V=full(v);V([1 2 8])=[%f;%t;%f];
if or(full(v1)<>V) then pause,end
v1=v;v1([1 2 1])=sparse([%f;%f;%f]);V=full(v);V([1 2 1])=[%f;%f;%f];
if or(full(v1)<>V) then pause,end
v1=v;v1(:)=[];
if or(full(v1)<>[]) then pause,end
v1=v;v1(:)=(sparse([2 1],44,[6 1])<>0);V=full(v);V(:)=[%f;%t;%f;%f;%f;%f];
if or(full(v1)<>V) then pause,end
v=v';
v1=v;v1(1)=%f;V=full(v);V(1)=%f;
if or(full(v1)<>V) then pause,end
v1=v;v1(2)=%f;V=full(v);V(2)=%f;
if or(full(v1)<>V) then pause,end
v1=v;v1(8)=%f;V=full(v);V(8)=%f;
if or(full(v1)<>V) then pause,end
v1=v;v1([1 2 8])=[%t %f %f];V=full(v);V([1 2 8])=[%t %f %f];
if or(full(v1)<>V) then pause,end
v1=v;v1(1)=sparse(%f);V=full(v);V(1)=%f;
if or(full(v1)<>V) then pause,end
v1=v;v1(2)=sparse(%f);V=full(v);V(2)=%f;
if or(full(v1)<>V) then pause,end
v1=v;v1(8)=sparse(%f);V=full(v);V(8)=%f;
if or(full(v1)<>V) then pause,end
v1=v;v1([1 2 8])=sparse([%f %t %f]);V=full(v);V([1 2 8])=[%f %t %f];
if or(full(v1)<>V) then pause,end
v1=v;v1([1 2 1])=sparse([%f %f %f]);V=full(v);V([1 2 1])=[%f %f %f];
if or(full(v1)<>V) then pause,end
v1=v;v1(:)=(sparse([1 2],44,[1,6]))<>0;V=full(v);V(:)=[%f %t %f %f %f %f];
if or(full(v1)<>V) then pause,end
v1=v;v1(1)=[];V=full(v);V(1)=[];
if or(full(v1)<>V) then pause,end
//test des comparaisons
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6])<>0;
b=sparse([1 6;1 2;6 5],[10;-1;-1],[6 6])<>0;;
if full(a==a)<>full(a)==full(a) then pause,end
if full(a<>a)<>(full(a)<>full(a)) then pause,end
if full(a==full(a))<>full(a)==full(a) then pause,end
if full(a<>full(a))<>(full(a)<>full(a)) then pause,end
if full(full(a)==a)<>full(a)==full(a) then pause,end
if full(full(a)<>a)<>(full(a)<>full(a)) then pause,end
a=sparse([1 1;3 1;6 1],[10 11 12],[6 1])<>0;
if full(a==a)<>full(a)==full(a) then pause,end
if full(a<>a)<>(full(a)<>full(a)) then pause,end
if full(a==full(a))<>full(a)==full(a) then pause,end
if full(a<>full(a))<>(full(a)<>full(a)) then pause,end
if full(full(a)==a)<>full(a)==full(a) then pause,end
if full(full(a)<>a)<>(full(a)<>full(a)) then pause,end
a=a';
if full(a==a)<>full(a)==full(a) then pause,end
if full(a<>a)<>(full(a)<>full(a)) then pause,end
if full(a==full(a))<>full(a)==full(a) then pause,end
if full(a<>full(a))<>(full(a)<>full(a)) then pause,end
if full(full(a)==a)<>full(a)==full(a) then pause,end
if full(full(a)<>a)<>(full(a)<>full(a)) then pause,end
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6])<>0;
b=sparse([1 6;1 2;5 5],[10;-1;-1],[5 6])<>0;
if a==b<>%f then pause,end
if a<>b<>%t then pause,end

