
// exec("bug650.sce")
format(15);
m=100; n=15;

t=(0:m-1)'/(m-1); A=[]; 
for i=1:n,  A = [A t.^(i-1)]; end
b=exp(sin(4*t)); b=b/2006.7874531048527;

x1=A\b
[Q,R]=qr(A); x2=R(1:n,1:n)\(Q(:, 1:n)'*b)
[U, S, V] =svd(A); x3=V*(diag(1 ./ diag(S))*U(:,1:n)'*b)

disp([x1 x2 x3])
