function triangle 

x=[0 1 1];
y=[0 0 1];
N=4;
h=1/N;
x=0;
for i=0:N-1;
  y=0
  for j=0:N-i-1
     t1=[x x+h x; y y y+h];
     t2=[x+h x+h x;y y+h y];
     xc1=x+h/3;
     yc1=y+h/3;
     xc2=x+2*h/3;
     yc2=x+2*h/3;
     y=y+h;
  end
  x=x+h
end
