// Copyright INRIA
// simple tests for arsimul and odedi 
// x_{n+1}=n*x_{n}, x_{1}=1;
deff('[xnp1]=ttt(n,xn)','xnp1=(xn**2);');
// use y=ode('discret',y1,1,2:n,macro);y=[y1,y]
// to get  y=[y1,y2,....,y_n];
//!
ero_n=0;
y=ode('discret',2,1,2:4,ttt);y=[2,y];
if y <> [2,4,16,256] then  pause,end 
// arsimul tests 
y=arsimul([1],[1],[0],0,1:10);
if y <> (1:10) then pause,end 
y=arsimul([1],[0,1],[0],0,1:10);
if y <> [0,1:9] then  pause,end 
y=arsimul([1],[0,1],[0],0,1:10,[-5]);
if y <> [-5,1:9] then pause,end





