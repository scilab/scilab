function sd=st_deviation(x,cr)
//Copyright Inria/Enpc 
//Philippe.Castagliola Ecole des Mines de Nantes
// 
[argout,argin]=argn(0);
if (argin<1)|(argin>2)
  error('incorrect number of arguments');
end
if x == [] then sd=%nan;return ;end 
[m n]=size(x);
if argin==1
  sd=sqrt(sum((x-mean(x)).^2)/(m*n-1));
elseif cr=='c'|cr==2
  sd=sqrt(sum((x-mean(x,'c')*ones(x(1,:))).^2,'c')/(n-1));
elseif cr=='r'|cr==1
  sd=sqrt(sum((x-ones(x(:,1))*mean(x,'r')).^2,'r')/(m-1));
else
  error('2nd argument cr must be equal to ''c'' or 2, ''r''or  1');
end
