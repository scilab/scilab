function [n]=nonParametricNormals(zu,zv)

dim=prod(size(zu));

n=[-zu(:)';
   -zv(:)';
   ones(1,dim)];

n=n*sparse([1:dim;1:dim]',1 ./sqrt(sum(n.^2,'r')));


