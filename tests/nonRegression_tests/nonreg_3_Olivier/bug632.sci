// Data fitting problem
// 1 build the data
//exec("632.sce");  

// this test will fail if scilab crashes
a=34;b=12;c=14;
deff('y=FF(x)','y=a*(x-b)+c*x.*x');
X=(0:.1:3)';Y=FF(X)+100*(rand()-.5);

//solve
function e=f1(abc,m)
  a=abc(1);b=abc(2),c=abc(3),
  e=Y-(a*(X-b)+c*X.*X);
endfunction
[abc,v]=lsqrsolve([10;10;10],f1,size(X,1));
abc
norm(v)


affich_result(%T,632);
