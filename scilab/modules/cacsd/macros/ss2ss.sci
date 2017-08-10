// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [Sl1,right,left]=ss2ss(Sl,T,F,G,flag)
    // State-space to state-space conversion
    // Returns the linear system Sl1=[A1,B1,C1,D1]
    // where A1=inv(T)*A*T,B1=inv(T)*B,C1=C*T,D1=D.
    // Optional parameters F and G are state feedback
    // and output injection respectively. For example,
    // Sl1=ss2ss(Sl,T,F) returns Sl1=[A1,B1,C1,D1] with
    // A1=inv(T)*(A+B*F)*T;B1=inv(T)*B;C1=(C+D*F)*T;D1=D;
    // If F is given as input then right is a non singular
    // linear system such that Sl1=Sl*right.
    // Sl1*invsyslin(right) is a factorization of Sl.
    // Idem for left: if F and G are given, Sl1=left*Sl*right.
    // Example: Sl=ssrand(2,2,5); trzeros(Sl);
    // Sl1=ss2ss(Sl,rand(5,5),rand(2,5),rand(5,2));
    // trzeros(Sl1), trzeros(rand(2,2)*Sl1*rand(2,2))
    // See also : projsl

    [A,B,C,D]=abcd(Sl);
    [LHS,RHS]=argn(0);
    select RHS
    case 2 then
        Ti=inv(T)
        Sl1=syslin(Sl.dt,Ti*A*T,Ti*B,C*T,D,Ti*Sl.X0);
        right=eye(A);left=right;
    case 3 then
        A1=A+B*F;C1=C+D*F;
        A1=inv(T)*A1*T;B1=inv(T)*B;C1=C1*T;D1=D
        Sl1=syslin(Sl.dt,A1,B1,C1,D1,Sl.X0);
        right=syslin(Sl.dt,A+B*F,B,F,eye(F*B));
        left=eye(size(C,1),size(C,1));
    case 4 then
        A1=A+B*F+G*C+G*D*F;C1=C+D*F;B1=B+G*D
        A1=inv(T)*A1*T;B1=inv(T)*B1;C1=C1*T;D1=D
        Sl1=syslin(Sl.dt,A1,B1,C1,D1,Sl.X0);
        right=syslin(Sl.dt,A+B*F,B,F,eye(F*B));
        // Warning left is computed as [ At + G*Ct,G;Ct,I]
        // where [At Bt; Ct Dt] is Sl1*right
        At=A+B*F; Ct=C+D*F
        left=syslin(Sl.dt,At+G*Ct,G,Ct,eye(Ct*G));
    case 5 then
        if flag==1 then
            // x in R^n , y in R^p, u in R^m
            // output injection [ A + GC, (B+GD,-G)]
            //                  [   C   , (D   , 0)]
            // then feeback ( since output injection increase the
            // size of the feedback the F matrix must be of size
            // (m+p,n) --> F=[F1;F2] with F1 (m,n) and F2 (p,n)
            //
            // Sl1= [ A+GC +BF1+G*D*F1 -GF2, (B+GD,-G)]
            // 	[ C+D*F1	       , (D   , 0)]
            //
            // We have then the following property
            // Sl1 equiv  left*sysdiag(sys,eye(p,p))*right
            //
            //
            n=size(A,"r");p=size(C,"r");
            A1=A+G*C+[B+G*D,-G]*F;B1=[B+G*D,-G];C1=C+[D,zeros(p,p)]*F;
            D1=[D,zeros(p,p)];
            A1=inv(T)*A1*T;B1=inv(T)*B1;C1=C1*T;D1=D1
            Sl1=syslin(Sl.dt,A1,B1,C1,D1,Sl.X0);
            left=syslin(Sl.dt,A+G*C,[G,-G],C,[eye(p,p),zeros(p,p)]);
            // Now we compute the right associated to left*Sl1
            A1=A+G*C;B1=[B+G*D,-G];C1=C;D1=[D,zeros(p,p)];
            right=syslin(Sl.dt,A1+B1*F,B1,F,eye(F*B1));
            return
        end
        if flag==2 then
            // x in R^n , y in R^p, u in R^m
            // feedback first F of size(m,n)
            //   		    [ A+BF,B]
            //                  [ C+DF,D]
            // then output injection
            // Sl1= [ A+GC +BF+G*D*F, (B+GD,-G)]
            // 	[ C+D*F   	, (D   , 0)]
            // this is a generalization of the case 4
            // We have then the following property
            // Sl1 equiv left*sysdiag(sys*right,eye(p,p)))
            //
            p = size(C,"r");
            A1=A+B*F+G*C+G*D*F;
            B1=[B+G*D,-G];
            C1=C+ D*F;
            D1=[D,zeros(p,p)];
            A1=inv(T)*A1*T;B1=inv(T)*B1;C1=C1*T;D1=D1
            Sl1=syslin(Sl.dt,A1,B1,C1,D1,Sl.X0);
            right=syslin(Sl.dt,A+B*F,B,F,eye(F*B));
            // Warning left is computed as [ At + G*Ct,(G,-G);
            //                             [    Ct    ,(I, 0)]
            // where [At Bt; Ct Dt] is Sl1*right
            At=A+B*F; Ct=C+D*F
            left=syslin(Sl.dt,At+G*Ct,[G,-G],Ct,[eye(Ct*G),zeros(Ct*G)]);
        end
    end
endfunction
