function [Q,Z,Qd,Zd,numbeps,numbeta]=kroneck(E,A)
//  Kronecker form: 
//
//            | sE(eps)-A(eps) |        X       |      X     |      X        |
//            |----------------|----------------|------------|---------------|
//            |        O       | sE(inf)-A(inf) |      X     |      X        |
// Q(sE-A)Z = |---------------------------------|----------------------------|
//            |                |                |            |               |
//            |        0       |       0        | sE(f)-A(f) |      X        |
//            |--------------------------------------------------------------|
//            |                |                |            |               |
//            |        0       |       0        |      0     | sE(eta)-A(eta)|
//         
//  Ec=Q*E*Z, Ac=Q*A*Z, eps=Qd(1) x Zd(1) ,inf=Qd(2) x Zd(2)
//  f = Qd(3) x Zd(3),  eta=Qd(4)xZd(4)
//
// numbeps(1) = # of eps blocks of size 0 x 1
// numbeps(2) = # of eps blocks of size 1 x 2
// numbeps(3) = # of eps blocks of size 2 x 3     etc...
//
// numbeta(1) = # of eta blocks of size 1 x 0
// numbeta(2) = # of eta blocks of size 2 x 1
// numbeta(3) = # of eta blocks of size 3 x 2     etc...
//
// interface  F.D. from Slicot-fstair
// T. Beelen's routines
// Copyright INRIA
  [LHS,RHS]=argn(0);
  if RHS==1 then [E,A]=pen2ea(E);end
  [Q,Z,Ec,Ac,Qd,Zd,numbeps]=quaskro(E,A);
  rows=Qd(1)+Qd(2)+1:Qd(1)+Qd(2)+Qd(3);
  cols=Zd(1)+Zd(2)+1:Zd(1)+Zd(2)+Zd(3);
  if cols==[] then 
    Zd(4)=Zd(3);Qd(4)=Qd(3);Qd(3)=0;
    numbeta=Qd(4)-Zd(4);
    return;
  end;
  if rows==[] then Qd(4)=0;Zd(4)=0;numbeta=0;return;end;
  Er=Ec(rows,cols);
  Ar=Ac(rows,cols);
  E1=pertrans(Er);
  A1=pertrans(Ar);
  [Q2,Z2,Ec2,Ac2,Qd2,Zd2,numbeta]=quaskro(E1,A1);
  // Here check that...
  //Zd2(2)=0  Qd2(2)=0;
  //Zd2(1)+Zd2(3)=Zd(3)
  //Qd2(1)+Qd2(3)=Qd(3)
  Z1=pertrans(Q2);
  Q1=pertrans(Z2);
  Zd1=[Qd2(3),Qd2(1)];    
  Qd1=[Zd2(3),Zd2(1)];
  Zd=[Zd(1),Zd(2),Zd1];
  Qd=[Qd(1),Qd(2),Qd1];
  Z(:,cols)=Z(:,cols)*Z1;
  Q(rows,:)=Q1*Q(rows,:);
endfunction


