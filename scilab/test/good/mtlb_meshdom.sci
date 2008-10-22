function [X,Y]=mtlb_meshdom(x,y)
[lhs,rhs]=argn()
x=matrix(x,1,-1);
y=matrix(y,-1,1);
X=ones(y)*x
Y=y($:-1:1)*ones(x)
endfunction
