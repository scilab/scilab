# Copyright INRIA

# sliding pendulum: curve #3

X:=x->x-2*k3*x*sin(k1*sqrt(1+4*k3^2*x^2))/sqrt(1+4*k3^2*x^2)/k2;
Y:=x->k3*x^2+sin(k1*sqrt(1+4*k3^2*x^2))/sqrt(1+4*k3^2*x^2)/k2;
k1:=20; k2:=18; k3:=1/2;
plot([X(x),Y(x),x=-1..1],scaling=CONSTRAINED);

k1:='k1'; k2:='k2'; k3:='k3';
FX:=1/diff(X(x),x);
FX:=simplify(eval(FX,1+4*k3^2*x^2=S^2),symbolic);
FY:=-1/diff(Y(x),x);
FY:=simplify(eval(FY,1+4*k3^2*x^2=S^2),symbolic);

