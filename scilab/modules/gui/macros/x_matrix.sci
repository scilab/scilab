function [y]=x_matrix(mes,x)
// Copyright INRIA
if type(x)<>1 then error(52," x must be a real matrix");end
[n1,n2]=size(x);
z=string(x);
y=[];
for i=1:n1,y=[y;strcat(z(i,:),", ")];end
y(1)='['+y(1);
for i=2:n1,y(i)=' '+y(i);end
y(n1)=y(n1)+']';
y=evstr(x_dialog(mes,y));
endfunction
