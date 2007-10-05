// Copyright INRIA
deff('[z]=f(x,y,t)','z=x^2+y^2+t');
res=feval(1:10,1:5,list(f,10));
res1=[];for j=1:5,res1=[res1, ((1:10).*(1:10))'+(j**2+10)*ones(10,1)];end
if res<>res1 then pause,end
deff('[z]=f(x,y)','z=x+%i*y');
res=feval(1:10,1:5,f);
res1=[];for j=1:5,res1=[res1, (1:10)'+%i*j*ones(10,1)];end
if res<>res1 then pause,end
