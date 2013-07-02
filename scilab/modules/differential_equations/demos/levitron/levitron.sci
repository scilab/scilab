//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge Steer <Serge.Steer@scilab.org>
//
// This file is distributed under the same license as the Scilab package.
//

function qpdot=levitron_dyn(t,qp,a,c,Mc)
    //Dynamics of the Levitron(TM)
    //Roger F Gans, Thomas B Jones and Masao Washizu 1998 J. Phys. D: Appl. Phys. 31 671-679   doi:10.1088/0022-3727/31/6/015
    //http://www.koepken.de/levitron/

    //Notations
    //---------
    //M:  The magnetic moment of the top
    //Me: the net strength of the ring dipole
    //R : the effective radius of the ring dipole (used as a normalization factor)
    //A : transverse moment of inertia of the top
    //C : polar  moment of inertia of the top
    //m : the mass of the top
    // x,y,z:    the coordinates of the center of mass of the top
    //
    // X=x/R, Y=y/R, Z=z/R, normalized coordinates
    // a=A/(m*R^2)    normalized transverse moment of inertia of the top
    // c=C/(m*R^2)    normalized polar  moment of inertia of the top
    // Mc=(M*Me)/(4*%pi*m*g*R^4),   normalized magnetic moment of the top

    q1=qp(1);//X
    q2=qp(2);//Y
    q3=qp(3);//Z
    q4=qp(4);//Theta
    q5=qp(5);//Phi
    q6=qp(6);//Psi

    p1=qp(7);//X'
    p2=qp(8);//Y'
    p3=qp(9);//Z'
    p4=qp(10);//a * Theta'
    p5=qp(11);// Psi'*(a*sin(Theta)^2+c*cos(Theta)^2)+c*Phi'*cos(Theta)
    p6=qp(12);// c*(Phi'+Psi'*cos(Theta))

    //intermediate computations
    cq4=cos(q4);
    sq4=sin(q4);
    sq5=sin(q5);
    cq5=cos(q5);
    q32=q3*q3;
    t1=((q32+1));
    t3=t1^(-3/2);
    t5=t1^(-5/2);
    t7=t1^(-7/2);
    t9=t1^(-9/2);
    t11=t1^(-11/2);
    s1=q32*2-3;
    u1=q32*t9*s1;
    u2=q32*t7;
    qq=q1*q1+q2*q2;
    w0=sq4*(3*q32*t5-t3+(3/4)*t7*s1*qq+3*q32*t7*qq-(21/4)*q32*t9*s1*qq)
    w1=(cq4*sq4^(-3)*(p5-p6*cq4)^2)/a
    w2=-(p6*(p5-p6*cq4))/(a*sq4)
    //derivative of q
    dqdt=[p1
    p2
    p3
    p4/a
    (sq4^(-2)*(p5*2-2*p6*cq4))/(2*a)
    p6/c-(cq4*sq4^(-2)*(p5-p6*cq4))/a];
    //derivative of p
    dpdt=-Mc*[(cq4*(3*q1*t7*s1 + 12*q1*q32*t7 - 21*q1*q32*t9*s1) + 3*q3*cq5*sq4*t7*s1)/2
    (cq4*(3*q2*t7*s1 + 12*q2*q32*t7 - 21*q2*q32*t9*s1) + 3*q3*sq5*sq4*t7*s1)/2
    (cq4*q3*(36*t5  + (36*t7-63*t9*s1)*qq + q32*(-60*t7 + (-168*t9+189*t11*s1)*qq))/4  + ..
    sq4*((3*t7*s1+ 12*t7*q32 - 21*q32*t9*s1)*(q1*cq5+q2*sq5))/2)
    (cq4*(3*q1*q3*cq5*t7*s1 + 3*q2*q3*sq5*t7*s1)/2 - sq4*(12*q32*t5 - 4*t3 + 3*t7*s1*qq+3*q32*t7*qq - 21*q32*t9*s1*qq)/4)
    sq4*(3*q2*q3*cq5*t7*s1 - 3*q1*q3*sq5*t7*s1)/2
    0]+ [0;0;-1;w1+w2;0;0]

    qpdot=[dqdt;dpdt];
endfunction



function R=euler(psi,theta,phi)
    cpsi=cos(psi);spsi=sin(psi);
    ct=cos(theta);st=sin(theta);
    cphi=cos(phi);sphi=sin(phi);
    R=[cpsi*ct*cphi-spsi*sphi,-cpsi*ct*sphi-spsi*cphi,  cpsi*st;
    spsi*ct*cphi+cpsi*sphi,-spsi*ct*sphi+cpsi*cphi,  spsi*st;
    -st*cphi                              ,st*sphi,  ct]
endfunction

function R=eulerm1(psi,theta,phi)
    cpsi=cos(psi);spsi=sin(psi);
    ct=cos(theta);st=sin(theta);
    cphi=cos(phi);sphi=sin(phi);
    R=[cpsi*cphi*ct-spsi*sphi,  cpsi*sphi+spsi*cphi*ct, -cphi*st;
    -spsi*cphi-cpsi*sphi*ct, cpsi*cphi-spsi*sphi*ct, sphi*st;
    cpsi*st,                 spsi*st,                ct  ];
endfunction
