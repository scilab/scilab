Phi:=q3/(1+q3^2)^(3/2)-3/4*(2*q3^2-3)*q3/(1+q3^2)^(7/2)*(q1^2+q2^2);
H:=1/2*(p1^2+p2^2+p3^2+p4^2/a+(p5-p6*cos(q4))^2/(a*sin(q4)^2)+p6^2/c)-M*(sin(q4)*(cos(q5)*diff(Phi,q1)+sin(q5)*diff(Phi,q2))+cos(q4)*diff(Phi,q3))+q3;
dq1:=diff(H,p1);
dq2:=diff(H,p2);
dq3:=diff(H,p3);
dq4:=diff(H,p4);
dq5:=diff(H,p5);
dq6:=diff(H,p6);

dq7:=diff(-H,q1);
dq8:=diff(-H,q2);
dq9:=diff(-H,q3);
dq10:=diff(-H,q4);
dq11:=diff(-H,q5);
dq12:=diff(-H,q6)
generate::fortran(dq1);generate::fortran(dq2);generate::fortran(dq3);generate::fortran(dq4);generate::fortran(dq5);generate::fortran(dq6);generate::fortran(dq7);generate::fortran(dq8);generate::fortran(dq9);generate::fortran(dq10);generate::fortran(dq11);generate::fortran(dq12);
