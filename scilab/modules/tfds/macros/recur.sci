function [y]=recur(x0,var,k,n)
//[y]=recur(x0,var,k,n)
// equation recurrente bilineaire
// x(i+1)=-x(i)*(k + sqrt(var)*br(i))
// partant de x0 et
// pilotee par un bruit gaussien de variance var.
//
// le programme dessine la trajectoire et
// retourne l'exposant de Liapunov empirique y
// ( x(i) est peu different de exp(y*i) )
//!
// Copyright INRIA
rand('normal');
br=rand(1,n);
x=ones(1,n);
x(1)=x0;
for i=2:n-1,x(i+1)=-x(i)*(k+sqrt(var)*br(i));end;
xbasc();
plot2d((1:n)',x',[-1],"111"," suite x(n)",[0,-10,n,10]);
y=log(abs(k*ones(br)+sqrt(var)*br));
y=sum(y)/n;
endfunction
