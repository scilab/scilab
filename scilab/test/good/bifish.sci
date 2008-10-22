function bifish(f_ch)
// bifish([f_ch])
// Biffurcation on the fish model 
// f_ch can be one of fish fishr fishr2
//!
// Copyright INRIA
x_message([" A bifurcation diagram of a discrete time system";
          " y=b*exp(-0.1*(x(k)_1+x(k)_2));";
          " x(k+1)=[ y 2*y ; s 0.0]*x(k);";
          " when s changes "]);
[lhs,rhs]=argn(0);
if rhs <= 0,f_ch=fish;end
for b = [11:0.2:12.4, 12.4:0.05:12.8,12.8:0.2:13],
 y=ode('discret',[18;17],1,2:2000,list(f_ch,b,0.9));
 xbasc();
 npts=1000:1999;
 plot2d(y(1,npts)',y(2,npts)',[0,-1],"111"," ",10*[0,0,4,4]);
 xtitle(string(b),' ',' ');
end
x_message([" We change the graphic scaling "]);

for b = [14.5:0.1:16],
 y=ode('discret',[18;17],1,2:2000,list(f_ch,b,0.9));
 xbasc();
 plot2d(y(1,npts)',y(2,npts)',[0,-1],"111"," ",25*[0,0,4,4]);
 xtitle(string(b),' ',' ');
end


endfunction
function xkp1=fish(k,xk,b,s)
y=b*exp(-0.1*(xk(1)+xk(2))) 
xkp1=[ y 2*y ; s 0.0]*xk

endfunction
function xkp1=fishr(k,xk,b,s)
y=b*exp(-0.1*(xk(1)+xk(2))) 
xkp1=[ y 2*y ; s*(1+0.1*(rand()-0.5)) 0.0]*xk


endfunction
function xkp1=fishr2(k,xk,b,s)
z=exp(-0.1*(xk(1)+xk(2))) 
xkp1=[ b*z**(1+0.1*(rand()-0.5)) 2*b*z**(1+0.1*(rand()-0.5)) ; s 0.0]*xk
endfunction
