// bug461

mode (-1)
clear

correct=%F
x=[0. 1.01 2.01 3. 4.02]; 
f=[0. 1. 3.9 8.75 16.5]; 
plot(x,f); 
d=splin(x,f ); 
xx=0:0.1:4.2; 
[s0,s1,s2,s3]=interp(xx,x,f,d);
s0=s0'
s1=s1'
s2=s2'
s3=s3'
result=execstr("plot2d(xx,[s0 s1 s2 s3])","errcatch","n")  
xdel()
affich_result(result==0, 461)

clear

// Error message :
