// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

lines(0)
ij=[1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6];
v=[1;2;3;4;5;6;7;8;9];
ij1=[1 6;1 2;6 5];
v1=[-1;-1;-1];
vc=v+%i*(21:29)';
v1c=v1+%i*[0;0.3;-1.2];
zer=sparse([],[],[6,6]);
//
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);
b=sparse([1 6;1 2;6 5],[-1;-1;-1],[6 6]);
ma=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],-(1:9),[6 6]);
mb=sparse([1 6;1 2;6 5],-[-1;-1;-1],[6 6]);
ac=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],(1:9)+%i*(21:29),[6 6]);
bc=sparse([1 6;1 2;6 5],[-1;-1;-1]+%i*[0;0.3;-1.2],[6 6]);
mac=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],-(1:9)-%i*(21:29),[6 6]);
mbc=sparse([1 6;1 2;6 5],[1;1;1]+%i*[0;-0.3;1.2],[6 6]);
//

//-------------------------------------------------------------
//test des primitives sparse, full
//--------------------------------------------------------------
//  -- for real matrix
if or(full(sparse(0.3))<>0.3) then pause,end
v=0*ones(1,3);v(3)=5;v(1)=1;
if or(full(sparse([1 1;1 3],[1 5]))<>v) then pause,end
v=0*ones(1,300);v(300)=5;v(1)=1;
if or(full(sparse([1 1;1 300],[1 5]))<>v) then pause,end
v=0*ones(3,1);v(3)=5;v(1)=1;
if or(full(sparse([1 1;3 1],[1 5]))<>v) then pause,end
v=0*ones(300,1);v(300)=5;v(1)=1;
if or(full(sparse([1 1;300 1],[1 5]))<>v) then pause,end
sp=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);
a=[0,0,3,0,2,1;;5,0,0,4,0,0;0,0,0,0,0,0;0,0,7,6,0,0;8,0,0,0,0,0;0,0,0,0,0,9];
if or(full(sp)<>a) then pause,end
sp=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[8 6]);a(8,6)=0;
if or(full(sp)<>a) then pause,end
if or(full(sparse([],[],[4 10]))<>0*ones(4,10)) then pause,end
v=sparse([2 1;3 1;4 1;6 1],[10;11;12;13],[6,1]);
a=[0;10;11;12;0;13];
if or(full(v)<>a) then pause,end
v=sparse([1 2;1 3;1 4;1 6],[10;11;12;13],[1,6]);
a=[0,10,11,12,0,13];
if or(full(v)<>a) then pause,end
//  -- for complex matrix
sp=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],(1:9)+%i*(21:29),[6 6]);
a=[0,0,3+%i*23,0,2+%i*22,1+%i*21
   5+%i*25,0,0,4+%i*24,0,0
   0,0,0,0,0,0
   0,0,7+%i*27,6+%i*26,0,0
   8+%i*28,0,0,0,0,0
   0,0,0,0,0,9+%i*29];
if or(full(sp)<>a) then pause,end
sp=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],(1:9)+%i*(21:29),[8 6]);
a(8,6)=0;
if or(full(sp)<>a) then pause,end
v=sparse([2 1;3 1;4 1;6 1],[10-3*%i;11;12+5*%i;13+0.5*%i],[6,1]);
a=[0;10-%i*3;11;12+%i*5;0;13+%i*0.5];
if or(full(v)<>a) then pause,end
v=sparse([1 2;1 3;1 4;1 6],[10-3*%i;11;12+5*%i;13+0.5*%i],[1,6]);
a=[0,10-%i*3,11,12+%i*5,0,13+%i*0.5];
if or(full(v)<>a) then pause,end
//-------------------------------------------------------------
//test de la primitives spget
//--------------------------------------------------------------
sp=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);
[ij,v]=spget(sp);
if or(ij<>[1,3;1,5;1,6;2,1;2,4;4,3;4,4;5,1;6,6])|or(v<>[3;2;1;5;4;7;6;8;9]) then pause,end
zer=sparse([],[],[6,6]);[ij,v]=spget(zer);
if ij<>[]|v<>[] then pause,end
//-----------------------------------------------
// addition and subtraction tests
//-----------------------------------------------
// -- real real
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);
b=sparse([1 6;1 2;6 5],[-1;-1;-1],[6 6]);
v=sparse([2 1;3 1;4 1;6 1],[10;11;12;13],[6,1]);
vt=sparse([1 2;1 3;1 4;1 6],[10;11;12;13],[1,6]);
ma=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],-(1:9),[6 6]);
mb=sparse([1 6;1 2;6 5],-[-1;-1;-1],[6 6]);
zer=sparse([],[],[6,6]);
ac=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],(1:9)+%i*(21:29),[6 6]);
bc=sparse([1 6;1 2;6 5],[-1;-1;-1]+%i*[0;0.3;-1.2],[6 6]);
vc=sparse([2 1;3 1;4 1;6 1],[10-3*%i;11;12+5*%i;13+0.5*%i],[6,1]);
vct=sparse([1 2;1 3;1 4;1 6],[10-3*%i;11;12+5*%i;13+0.5*%i],[1,6]);
mac=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],-(1:9)-%i*(21:29),[6 6]);
mbc=sparse([1 6;1 2;6 5],[1;1;1]+%i*[0;-0.3;1.2],[6 6]);
if or(full(v+sparse([],[],[6,1]))<>full(v)) then pause,end
if or(full(sparse([],[],[6,1])+v)<>full(v)) then pause,end
if or(full(v+v)<>full(v)+full(v)) then pause,end
if or(full(v-v)<>full(v)-full(v)) then pause,end
if or(full(vt+sparse([],[],[1,6]))<>full(vt)) then pause,end
if or(full(vt+vt)<>full(vt)+full(vt)) then pause,end
if or(full(vt-vt)<>full(vt)-full(vt)) then pause,end
if or(full(zer+zer)<>0*ones(6,6)) then pause,end
if or(full(a+a)<>full(a)+full(a)) then pause,end
if or(full(b+b)<>full(b)+full(b)) then pause,end
if or(full(a+zer)<>full(a)) then pause,end
if or(full(zer+a)<>full(a)) then pause,end
if or(full(b+a)<>full(b)+full(a)) then pause,end
if or(full(a+b)<>full(b)+full(a)) then pause,end
if or(full(a+ma)<>full(a)+full(ma)) then pause,end
if or(full(a-a)<>full(a)-full(a)) then pause,end
if or(full(a-ma)<>full(a)-full(ma)) then pause,end
if or(full(b-mb)<>full(b)-full(mb)) then pause,end
if or(full(a-zer)<>full(a)) then pause,end
if or(full(zer-a)<>-full(a)) then pause,end
if or(full(a-mb)<>full(a)-full(mb)) then pause,end
//  -- real complex
if or(full(sparse([],[],[6,1])+vc)<>full(vc)) then pause,end
if or(full(v+vc)<>full(v)+full(vc)) then pause,end
if or(full(v-vc)<>full(v)-full(vc)) then pause,end
if or(full(vt+vct)<>full(vt)+full(vct)) then pause,end
if or(full(vt-vct)<>full(vt)-full(vct)) then pause,end
if or(full(a+ac)<>full(a)+full(ac)) then pause,end
if or(full(b+bc)<>full(b)+full(bc)) then pause,end
if or(full(a+bc)<>full(a)+full(bc)) then pause,end
if or(full(zer+ac)<>full(zer)+full(ac)) then pause,end
if or(full(b+ac)<>full(b)+full(ac)) then pause,end
if or(full(a-ac)<>full(a)-full(ac)) then pause,end
if or(full(b-bc)<>full(b)-full(bc)) then pause,end
if or(full(a-bc)<>full(a)-full(bc)) then pause,end
if or(full(zer-ac)<>full(zer)-full(ac)) then pause,end
if or(full(b-ac)<>full(b)-full(ac)) then pause,end
// -- complex real
if or(full(vc+v)<>full(vc)+full(v)) then pause,end
if or(full(vc-v)<>full(vc)-full(v)) then pause,end
if or(full(vct+vt)<>full(vct)+full(vt)) then pause,end
if or(full(vct-vt)<>full(vct)-full(vt)) then pause,end
if or(full(ac+a)<>full(ac)+full(a)) then pause,end
if or(full(bc+b)<>full(bc)+full(b)) then pause,end
if or(full(ac+b)<>full(ac)+full(b)) then pause,end
if or(full(ac+zer)<>full(zer)+full(ac)) then pause,end
if or(full(bc+a)<>full(bc)+full(a)) then pause,end
if or(full(ac-a)<>full(ac)-full(a)) then pause,end
if or(full(bc-b)<>full(bc)-full(b)) then pause,end
if or(full(ac-b)<>full(ac)-full(b)) then pause,end
if or(full(ac-zer)<>-full(zer)+full(ac)) then pause,end
if or(full(bc-a)<>full(bc)-full(a)) then pause,end
// -- complex complex
if or(full(vc+vc)<>full(vc)+full(vc)) then pause,end
if or(full(vc-vc)<>full(vc)-full(vc)) then pause,end
if or(full(vct+vct)<>full(vct)+full(vct)) then pause,end
if or(full(vct-vct)<>full(vct)-full(vct)) then pause,end
if or(full(ac+ac)<>full(ac)+full(ac)) then pause,end
if or(full(bc+bc)<>full(bc)+full(bc)) then pause,end
if or(full(ac+bc)<>full(ac)+full(bc)) then pause,end
if or(full(bc+ac)<>full(bc)+full(ac)) then pause,end
if or(full(ac-ac)<>full(zer)) then pause,end
if or(full(bc-bc)<>full(zer)) then pause,end
if or(full(ac-bc)<>full(ac)-full(bc)) then pause,end
if or(full(bc-ac)<>full(bc)-full(ac)) then pause,end
// sparse full and full sparse operation (soft coded apoerations)
if or(full(a+1)<>full(a)+1) then pause,end
if or(full(1+a)<>full(a)+1) then pause,end
if or(full(a+ones(6,6))<>full(a)+ones(6,6)) then pause,end
if or(full(ones(6,6)+a)<>full(a)+ones(6,6)) then pause,end
if or(full(a+2*eye())<>full(a)+2*eye()) then pause,end
if or(full(2*eye()+a)<>full(a)+2*eye()) then pause,end
if or(full(a-1)<>full(a)-1) then pause,end
if or(full(1-a)<>1-full(a)) then pause,end
if or(full(a-ones(6,6))<>full(a)-ones(6,6)) then pause,end
if or(full(ones(6,6)-a)<>ones(6,6)-full(a)) then pause,end
if or(full(a-2*eye())<>full(a)-2*eye()) then pause,end
if or(full(2*eye()-a)<>2*eye()-full(a)) then pause,end
if or(full(ac+1)<>full(ac)+1) then pause,end
if or(full(1+ac)<>full(ac)+1) then pause,end
if or(full(ac+ones(6,6))<>full(ac)+ones(6,6)) then pause,end
if or(full(ones(6,6)+ac)<>full(ac)+ones(6,6)) then pause,end
if or(full(ac+2*eye())<>full(ac)+2*eye()) then pause,end
if or(full(2*eye()+ac)<>full(ac)+2*eye()) then pause,end
if or(full(ac-1)<>full(ac)-1) then pause,end
if or(full(1-ac)<>1-full(ac)) then pause,end
if or(full(ac-ones(6,6))<>full(ac)-ones(6,6)) then pause,end
if or(full(ones(6,6)-ac)<>ones(6,6)-full(ac)) then pause,end
if or(full(ac-2*eye())<>full(ac)-2*eye()) then pause,end
if or(full(2*eye()-ac)<>2*eye()-full(ac)) then pause,end
if or(full(ac+full(bc))<>full(ac)+full(bc)) then pause,end
if or(full(ac-full(bc))<>full(ac)-full(bc)) then pause,end
if or(full(full(ac)+full(bc))<>full(ac)+full(bc)) then pause,end
//-----------------------------------------------
// multiplication tests
//-----------------------------------------------
// real x real
// sparse scalar , saclar sparse
if or(full(a*2)<>full(a)*2) then pause,end
if or(full(2*a)<>full(a)*2) then pause,end
if  a*[]<>[] then pause,end
if  []*a<>[] then pause,end
c=rand(6,6);
if norm(a*c-full(a)*c) >100*%eps then pause,end
if norm(c*a-c*full(a)) >100*%eps then pause,end
// sparse sparse
if or(full(zer*zer)<>full(zer)) then pause,end
if or(full(a*zer)<>full(zer)) then pause,end
if or(full(zer*a)<>full(zer)) then pause,end
if  norm(full(a*a)-full(a)*full(a),1)>100*%eps then pause,end
if  norm(full(b*b)-full(b)*full(b),1)>100*%eps then pause,end
if  norm(full(a*b)-full(a)*full(b),1)>100*%eps then pause,end
if  norm(full(b*a)-full(b)*full(a),1)>100*%eps then pause,end
// complex x real real x complex
// sparse scalar , scalar sparse
if or(full(ac*2)<>full(ac)*2) then pause,end
if or(full(2*ac)<>full(ac)*2) then pause,end
if norm(full((2+%i)*a)-(2+%i)*full(a),1) >100*%eps then pause,end
if norm(full(a*(2+%i))-(2+%i)*full(a),1) >100*%eps then pause,end
if  ac*[]<>[] then pause,end
if  []*ac<>[] then pause,end
c=rand(6,6);
cc=c+rand(6,6)*%i;
if norm(ac*c-full(ac)*c) >100*%eps then pause,end
if norm(cc*a-cc*full(a)) >100*%eps then pause,end
// sparse sparse
if or(full(ac*zer)<>full(zer)) then pause,end
if or(full(zer*ac)<>full(zer)) then pause,end
if  norm(full(ac*a)-full(ac)*full(a),1)>100*%eps then pause,end
if  norm(full(bc*b)-full(bc)*full(b),1)>100*%eps then pause,end
if  norm(full(a*bc)-full(a)*full(bc),1)>100*%eps then pause,end
if  norm(full(b*ac)-full(b)*full(ac),1)>100*%eps then pause,end
// // complex x complex
if norm(ac*cc-full(ac)*cc) >100*%eps then pause,end
if norm(cc*ac-cc*full(ac)) >100*%eps then pause,end
// sparse sparse
if  norm(full(ac*ac)-full(ac)*full(ac),1)>100*%eps then pause,end
if  norm(full(bc*bc)-full(bc)*full(bc),1)>100*%eps then pause,end
if  norm(full(bc*ac)-full(bc)*full(ac),1)>100*%eps then pause,end
//----------------------------------------------------------
// element wise multiplication tests
//----------------------------------------------------------
if or(full(ac.*2)<>full(ac)*2) then pause,end
if or(full((2).*ac)<>full(ac)*2) then pause,end
if  a.*[]<>[] then pause,end
if  [].*a<>[] then pause,end
c=rand(6,6);
//if norm(a*c-full(a)*c) >100*%eps then pause,end
//if norm(c*a-c*full(a)) >100*%eps then pause,end
// sparse sparse
if or(full(zer.*zer)<>full(zer)) then pause,end
if or(full(a.*zer)<>full(zer)) then pause,end
if or(full(zer.*a)<>full(zer)) then pause,end
if  norm(full(a.*a)-full(a).*full(a),1)>100*%eps then pause,end
if  norm(full(b.*b)-full(b).*full(b),1)>100*%eps then pause,end
if  norm(full(a.*b)-full(a).*full(b),1)>100*%eps then pause,end
if  norm(full(b.*a)-full(b).*full(a),1)>100*%eps then pause,end
// complex x real real x complex
// sparse scalar , scalar sparse
if norm(full((2+%i).*a)-(2+%i).*full(a),1) >100*%eps then pause,end
if norm(full(a.*(2+%i))-(2+%i).*full(a),1) >100*%eps then pause,end
if  ac.*[]<>[] then pause,end
if  [].*ac<>[] then pause,end
c=rand(6,6);
cc=c+rand(6,6)*%i;
if norm(full(ac.*c)-full(ac).*c) >100*%eps then pause,end
if norm(full(cc.*a)-cc.*full(a)) >100*%eps then pause,end
// sparse sparse
if or(full(ac.*zer)<>full(zer)) then pause,end
if or(full(zer.*ac)<>full(zer)) then pause,end
if  norm(full(ac.*a)-full(ac).*full(a),1)>100*%eps then pause,end
if  norm(full(bc.*b)-full(bc).*full(b),1)>100*%eps then pause,end
if  norm(full(a.*bc)-full(a).*full(bc),1)>100*%eps then pause,end
if  norm(full(b.*ac)-full(b).*full(ac),1)>100*%eps then pause,end
// // complex x complex
if norm(ac.*cc-full(ac).*cc) >100*%eps then pause,end
if norm(cc.*ac-cc.*full(ac)) >100*%eps then pause,end
// sparse sparse
if  norm(full(ac.*ac)-full(ac).*full(ac),1)>100*%eps then pause,end
if  norm(full(bc.*bc)-full(bc).*full(bc),1)>100*%eps then pause,end
if  norm(full(bc.*ac)-full(bc).*full(ac),1)>100*%eps then pause,end
// ----------------------------------------------------------
// test de la transposition
//-----------------------------------------------------------
if or(full(a')<>full(a)') then pause,end
if or(full(ac')<>full(ac)') then pause,end
if or(full(zer')<>full(zer)' ) then pause,end
v=sparse([2 1;3 1;4 1;6 1],[10;11;12;13],[6,1]);
if or(full(v')<>full(v)') then pause,end
if or(full((v')')<>full(v)) then pause,end
vc=sparse([2 1;3 1;4 1;6 1],[10-3*%i;11;12+5*%i;13+0.5*%i],[6,1]);
if or(full(vc')<>full(vc)') then pause,end
if or(full((vc')')<>full(vc)) then pause,end
// ----------------------------------------------------------
// test des concatenation
//-----------------------------------------------------------
if or(full([a])<>full(a)) then pause,end
if or(full([a b])<>[full(a) full(b)]) then pause,end
if or(full([a;b])<>[full(a);full(b)]) then pause,end
if or(full([a []])<>full(a)) then pause,end
if or(full([a;[]])<>full(a)) then pause,end
if or(full([a zer])<>[full(a) full(zer)]) then pause,end
if or(full([zer;b])<>[full(zer);full(b)]) then pause,end
if or(full([ac])<>full(ac)) then pause,end
if or(full([ac b])<>[full(ac) full(b)]) then pause,end
if or(full([ac;b])<>[full(ac);full(b)]) then pause,end
if or(full([ac []])<>full(ac)) then pause,end
if or(full([ac;[]])<>full(ac)) then pause,end
if or(full([a bc])<>[full(a) full(bc)]) then pause,end
if or(full([a;bc])<>[full(a);full(bc)]) then pause,end
if or(full([ac bc])<>[full(ac) full(bc)]) then pause,end
if or(full([ac;bc])<>[full(ac);full(bc)]) then pause,end
// ----------------------------------------------------------
// test des extractions
//-----------------------------------------------------------
af=full(a);
if or(full(a(1,3))<>af(1,3)) then pause,end
if or(full(a(1,4))<>af(1,4)) then pause,end
if or(full(a(1,:))<>af(1,:)) then pause,end
v=sparse([2 1;3 1;4 1;6 1],[10;11;12;13],[6,1]);vf=full(v);
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
acff=full(ac);
if or(full(ac(1,3))<>acff(1,3)) then pause,end
if or(full(ac(1,4))<>acff(1,4)) then pause,end
if or(full(ac(1,:))<>acff(1,:)) then pause,end
vc=sparse([2 1;3 1;4 1;6 1],[10-3*%i;11;12+5*%i;13+0.5*%i],[6,1]);vcf=full(vc);
if or(full(vc(:))<>vcf(:)) then pause,end
if or(full(vc(3:4))<>vcf(3:4)) then pause,end
if or(full(vc([1 5]))<>vcf([1 5])) then pause,end
if or(full(vc([4 3]))<>vcf([4 3])) then pause,end
if or(full(vc([4 4]))<>vcf([4 4])) then pause,end
if or(full(vc([1 1]))<>vcf([1 1])) then pause,end
vc=vc';vcf=vcf';
if or(full(vc(:))<>vcf(:)) then pause,end
if or(full(vc(3:4))<>vcf(3:4)) then pause,end
if or(full(vc([1 5]))<>vcf([1 5])) then pause,end
if or(full(vc([4 3]))<>vcf([4 3])) then pause,end
if or(full(vc([4 4]))<>vcf([4 4])) then pause,end
if or(full(vc([1 1]))<>vcf([1 1])) then pause,end
// ----------------------------------------------------------
// test des insertions
//-----------------------------------------------------------

a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);
vt=sparse([1 2;1 3;1 4;1 6],[10;11;12;13],[1,6]);
// full line insertion
//----------------------
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
a1=a;a1([1 3],:)=[vt;2*vt];A=full(a);Vt=full(vt);A([1 3],:)=[Vt;2*Vt];
if or(full(a1)<>A) then pause,end
a1=a;a1([3 1],:)=[vt;2*vt];A=full(a);Vt=full(vt);A([3 1],:)=[Vt;2*Vt];
if or(full(a1)<>A) then pause,end
a1=a;a1([1 3 1],:)=[vt;2*vt;3*vt];A=full(a);Vt=full(vt);A([1 3 1],:)=[Vt;2*Vt;3*Vt];
if or(full(a1)<>A) then pause,end
//  insert zero vector
vt=sparse([],[],[1,6]);
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
a1=a;a1([1 3],:)=[vt;2*vt];A=full(a);Vt=full(vt);A([1 3],:)=[Vt;2*Vt];
if or(full(a1)<>A) then pause,end
a1=a;a1([3 1],:)=[vt;2*vt];A=full(a);Vt=full(vt);A([3 1],:)=[Vt;2*Vt];
if or(full(a1)<>A) then pause,end
a1=a;a1([1 3 1],:)=[vt;2*vt;3*vt];A=full(a);Vt=full(vt);A([1 3 1],:)=[Vt;2*Vt;3*Vt];
if or(full(a1)<>A) then pause,end
a=sparse([],[],[6,6]);
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
b=sparse([1 1;1 3;1 6;2 1;2 2;2 4],10:15,[2,6]);
a1=a;a1([1 3],:)=b;A=full(a);B=full(b);A([1 3],:)=B;
if or(full(a1)<>A) then pause,end
a1=a;a1([3 1],:)=b;A=full(a);B=full(b);A([3 1],:)=B;
if or(full(a1)<>A) then pause,end
b=sparse([1 1;1 3;1 6;2 1;2 2;2 4;3 3;3 5],10:17,[3,6]);
a1=a;a1([1 3 1],:)=b;A=full(a);B=full(b);A([1 3 1],:)=B;
if or(full(a1)<>A) then pause,end
//  insert zero vector
vt=sparse([],[],[1,6]);
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
b=sparse([1 1;1 3;1 6;2 1;2 2;2 4],10:15,[2,6]);
a1=a;a1([1 3],:)=b;A=full(a);B=full(b);A([1 3],:)=B;
if or(full(a1)<>A) then pause,end
a1=a;a1([3 1],:)=b;A=full(a);B=full(b);A([3 1],:)=B;
if or(full(a1)<>A) then pause,end
b=sparse([1 1;1 3;1 6;2 1;2 2;2 4;3 3;3 5],10:17,[3,6]);
a1=a;a1([1 3 1],:)=b;A=full(a);B=full(b);A([1 3 1],:)=B;
if or(full(a1)<>A) then pause,end
// full column insertion
//----------------------
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);
v=sparse([2 1;3 1;4 1;6 1],[10;11;12;13],[6,1]);
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
b=sparse([1 2;2 1;3 1;3 2;4 1;6 2],10:15,[6,2]);
a1=a;a1(:,[1 3])=b;A=full(a);B=full(b);A(:,[1 3])=B;
if or(full(a1)<>A) then pause,end
a1=a;a1(:,[3 1])=b;A=full(a);B=full(b);A(:,[3 1])=B;
if or(full(a1)<>A) then pause,end
b=sparse([1 2;2 1;2 3;3 1;3 2;4 1;5 3;6 2],10:17,[6,3]);
a1=a;a1(:,[1 3 1])=b;A=full(a);B=full(b);A(:,[1 3 1])=B;
if or(full(a1)<>A) then pause,end
v=sparse([],[],[6,1]);
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
b=sparse([],[],[6,2]);
a1=a;a1(:,[1 3])=b;A=full(a);B=full(b);A(:,[1 3])=B;
if or(full(a1)<>A) then pause,end
a1=a;a1(:,[3 1])=b;A=full(a);B=full(b);A(:,[3 1])=B;
if or(full(a1)<>A) then pause,end
b=sparse([],[],[6,3]);
a1=a;a1(:,[1 3 1])=b;A=full(a);B=full(b);A(:,[1 3 1])=B;
if or(full(a1)<>A) then pause,end
a=sparse([],[],[6,6]);
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
b=sparse([1 2;2 1;3 1;3 2;4 1;6 2],10:15,[6,2]);
a1=a;a1(:,[1 3])=b;A=full(a);B=full(b);A(:,[1 3])=B;
if or(full(a1)<>A) then pause,end
a1=a;a1(:,[3 1])=b;A=full(a);B=full(b);A(:,[3 1])=B;
if or(full(a1)<>A) then pause,end
b=sparse([1 2;2 1;2 3;3 1;3 2;4 1;5 3;6 2],10:17,[6,3]);
a1=a;a1(:,[1 3 1])=b;A=full(a);B=full(b);A(:,[1 3 1])=B;
if or(full(a1)<>A) then pause,end
v=sparse([],[],[6,1]);
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
b=sparse([1 2;2 1;3 1;3 2;4 1;6 2],10:15,[6,2]);
a1=a;a1(:,[1 3])=b;A=full(a);B=full(b);A(:,[1 3])=B;
if or(full(a1)<>A) then pause,end
a1=a;a1(:,[3 1])=b;A=full(a);B=full(b);A(:,[3 1])=B;
if or(full(a1)<>A) then pause,end
b=sparse([1 2;2 1;2 3;3 1;3 2;4 1;5 3;6 2],10:17,[6,3]);
a1=a;a1(:,[1 3 1])=b;A=full(a);B=full(b);A(:,[1 3 1])=B;
if or(full(a1)<>A) then pause,end
v=sparse([],[],[6,1]);
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
b=sparse([],[],[6,2]);
a1=a;a1(:,[1 3])=b;A=full(a);B=full(b);A(:,[1 3])=B;
if or(full(a1)<>A) then pause,end
a1=a;a1(:,[3 1])=b;A=full(a);B=full(b);A(:,[3 1])=B;
if or(full(a1)<>A) then pause,end
b=sparse([],[],[6,3]);
a1=a;a1(:,[1 3 1])=b;A=full(a);B=full(b);A(:,[1 3 1])=B;
if or(full(a1)<>A) then pause,end
// row column insertion
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);
b=sparse([1 2;1 3;3 3],-(1:3),[3,3]);
a1=a;a1(1,1)=sparse(30);A=full(a);A(1,1)=30;
if or(full(a1)<>A) then pause,end
a1=a;a1(1,6)=sparse(30);A=full(a);A(1,6)=30;
if or(full(a1)<>A) then pause,end
a1=a;a1(1,8)=sparse(30);A=full(a);A(1,8)=30;
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
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);
vt=11:16;
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
a1=a;a1([1 3],:)=[vt;2*vt];A=full(a);A([1 3],:)=[vt;2*vt];
if or(full(a1)<>A) then pause,end
a1=a;a1([3 1],:)=[vt;2*vt];A=full(a);A([3 1],:)=[vt;2*vt];
if or(full(a1)<>A) then pause,end
a1=a;a1([1 3 1],:)=[vt;2*vt;3*vt];A=full(a);A([1 3 1],:)=[vt;2*vt;3*vt];
if or(full(a1)<>A) then pause,end
a=sparse([],[],[6,6]);
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
b=[1:6;11:16];
a1=a;a1([1 3],:)=b;A=full(a);A([1 3],:)=b;
if or(full(a1)<>A) then pause,end
a1=a;a1([3 1],:)=b;A=full(a);A([3 1],:)=b;
if or(full(a1)<>A) then pause,end
b=[1:6;11:16;21:26];
a1=a;a1([1 3 1],:)=b;A=full(a);A([1 3 1],:)=b;
if or(full(a1)<>A) then pause,end
// full column insertion
//----------------------
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);
v=(1:6)';
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
b=[(1:6)' (11:16)'];
a1=a;a1(:,[1 3])=b;A=full(a);A(:,[1 3])=b;
if or(full(a1)<>A) then pause,end
a1=a;a1(:,[3 1])=b;A=full(a);A(:,[3 1])=b;
if or(full(a1)<>A) then pause,end
b=[(1:6)' (11:16)' (21:26)'];
a1=a;a1(:,[1 3 1])=b;A=full(a);A(:,[1 3 1])=b;
if or(full(a1)<>A) then pause,end
//********
// row column insertion
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);
b=-[1 2 3;4 5 6;7 8 9];
a1=a;a1(1,1)=30;A=full(a);A(1,1)=30;
if or(full(a1)<>A) then pause,end
a1=a;a1(1,6)=30;A=full(a);A(1,6)=30;
if or(full(a1)<>A) then pause,end
a1=a;a1(1,8)=30;A=full(a);A(1,8)=30;
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
v=sparse([1 1;3 1;6 1],[10 11 12],[6 1]);
v1=v;v1(1)=33;V=full(v);V(1)=33;
if or(full(v1)<>V) then pause,end
v1=v;v1(2)=33;V=full(v);V(2)=33;
if or(full(v1)<>V) then pause,end
v1=v;v1(8)=33;V=full(v);V(8)=33;
if or(full(v1)<>V) then pause,end
v1=v;v1([1 2 8])=[5;10;33];V=full(v);V([1 2 8])=[5;10;33];
if or(full(v1)<>V) then pause,end
v1=v;v1(:)=[];
if or(full(v1)<>[]) then pause,end
v1=v;v1(1)=sparse(33);V=full(v);V(1)=33;
if or(full(v1)<>V) then pause,end
v1=v;v1(2)=sparse(33);V=full(v);V(2)=33;
if or(full(v1)<>V) then pause,end
v1=v;v1(8)=sparse(33);V=full(v);V(8)=33;
if or(full(v1)<>V) then pause,end
v1=v;v1([1 2 8])=sparse([5;10;33]);V=full(v);V([1 2 8])=[5;10;33];
if or(full(v1)<>V) then pause,end
v1=v;v1([1 2 1])=sparse([5;10;33]);V=full(v);V([1 2 1])=[5;10;33];
if or(full(v1)<>V) then pause,end
v1=v;v1(:)=[];
if or(full(v1)<>[]) then pause,end
v1=v;v1(:)=sparse([2 1],44,[6 1]);V=full(v);V(:)=[0;44;0;0;0;0];
if or(full(v1)<>V) then pause,end
v=v';
v1=v;v1(1)=33;V=full(v);V(1)=33;
if or(full(v1)<>V) then pause,end
v1=v;v1(2)=33;V=full(v);V(2)=33;
if or(full(v1)<>V) then pause,end
v1=v;v1(8)=33;V=full(v);V(8)=33;
if or(full(v1)<>V) then pause,end
v1=v;v1([1 2 8])=[5 10 33];V=full(v);V([1 2 8])=[5 10 33];
if or(full(v1)<>V) then pause,end
v1=v;v1(1)=sparse(33);V=full(v);V(1)=33;
if or(full(v1)<>V) then pause,end
v1=v;v1(2)=sparse(33);V=full(v);V(2)=33;
if or(full(v1)<>V) then pause,end
v1=v;v1(8)=sparse(33);V=full(v);V(8)=33;
if or(full(v1)<>V) then pause,end
v1=v;v1([1 2 8])=sparse([5 10 33]);V=full(v);V([1 2 8])=[5 10 33];
if or(full(v1)<>V) then pause,end
v1=v;v1([1 2 1])=sparse([5 10 33]);V=full(v);V([1 2 1])=[5 10 33];
if or(full(v1)<>V) then pause,end
v1=v;v1(:)=sparse([1 2],44,[1,6]);V=full(v);V(:)=[0 44 0 0 0 0];
if or(full(v1)<>V) then pause,end
v1=v;v1(1)=[];V=full(v);V(1)=[];
if or(full(v1)<>V) then pause,end
//test des comparaisons
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);
b=sparse([1 6;1 2;6 5],[10;-1;-1],[6 6]);
if full(a==a)<>full(a)==full(a) then pause,end
if full(a<>a)<>(full(a)<>full(a)) then pause,end
if full(a>sparse(5))<>(full(a)>5) then pause,end
if full(sparse(5)>a)<>(5>full(a)) then pause,end
if full(b>a)<>(full(b)>full(a))  then pause,end
if full(a==full(a))<>full(a)==full(a) then pause,end
if full(a<>full(a))<>(full(a)<>full(a)) then pause,end
if full(a>5)<>(full(a)>5) then pause,end
if full(5>a)<>(5>full(a)) then pause,end
if full(b>full(a))<>(full(b)>full(a))  then pause,end
if full(full(a)==a)<>full(a)==full(a) then pause,end
if full(full(a)<>a)<>(full(a)<>full(a)) then pause,end
if full(full(a)>sparse(5))<>(full(a)>5) then pause,end
if full(full(b)>a)<>(full(b)>full(a))  then pause,end
a=sparse([1 1;3 1;6 1],[10 11 12],[6 1]);
if full(a==a)<>full(a)==full(a) then pause,end
if full(a<>a)<>(full(a)<>full(a)) then pause,end
if full(a>sparse(5))<>(full(a)>5) then pause,end
if full(sparse(5)>a)<>(5>full(a)) then pause,end
if full(a==full(a))<>full(a)==full(a) then pause,end
if full(a<>full(a))<>(full(a)<>full(a)) then pause,end
if full(a>5)<>(full(a)>5) then pause,end
if full(5>a)<>(5>full(a)) then pause,end
if full(full(a)==a)<>full(a)==full(a) then pause,end
if full(full(a)<>a)<>(full(a)<>full(a)) then pause,end
if full(full(a)>sparse(5))<>(full(a)>5) then pause,end
a=a';
if full(a==a)<>full(a)==full(a) then pause,end
if full(a<>a)<>(full(a)<>full(a)) then pause,end
if full(a>sparse(5))<>(full(a)>5) then pause,end
if full(sparse(5)>a)<>(5>full(a)) then pause,end
if full(a==full(a))<>full(a)==full(a) then pause,end
if full(a<>full(a))<>(full(a)<>full(a)) then pause,end
if full(a>5)<>(full(a)>5) then pause,end
if full(5>a)<>(5>full(a)) then pause,end
if full(full(a)==a)<>full(a)==full(a) then pause,end
if full(full(a)<>a)<>(full(a)<>full(a)) then pause,end
if full(full(a)>sparse(5))<>(full(a)>5) then pause,end
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);
b=sparse([1 6;1 2;5 5],[10;-1;-1],[5 6]);
if a==b<>%f then pause,end
if a<>b<>%t then pause,end

a=sparse([%f %t %f %t]);
b=sparse([%t %f %t %f]);
assert_checkequal(~a, b);
