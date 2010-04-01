% Test file for function plot
% Copyright INRIA
% Matlab version: 6.5.1.199709 (R13) Service Pack 1
% V.C.

figure(1)
x=0:pi/15:4*pi; 
y=exp(2*cos(x));
plot(x,y,'r+');

figure(2)
t=0:pi/100:2*pi;
y=sin(t)
plot(t,y)

figure(3)
y2 = sin(t-0.25); 
y3 = sin(t-0.5);
plot(t,y,t,y2,t,y3)

figure(4)
t = 0:pi/100:2*pi;
y = sin(t);
y2 = sin(t-0.25); 
y3 = sin(t-0.5);
plot(t,y,'-',t,y2,'--',t,y3,':')

figure(5)
x = -pi:pi/10:pi;
y = tan(sin(x)) - sin(tan(x));
plot(x,y,'--sr','LineWidth',2,...
                'MarkerEdgeColor','k',...
                'MarkerFaceColor','g',...
                'MarkerSize',10)

figure(6)
x = -pi:pi/10:pi;
y = tan(sin(x)) - sin(tan(x));
plot(x,y,'--sr')

