// Copyright INRIA
eps=100*%eps;
// inf 
if norm([1,2,3,-1,-2,-3],0)<>%inf then pause,end 
if ~isnan(norm([1,2,3,-1,-2,-3],%nan)) then pause,end 
if norm([])<>0 then pause,end
// vector
x=[1,2,3,-4];
if abs(norm(x,1) - sum(abs(x))) > eps then pause,end
if abs(norm(x,2) - sqrt(sum(abs(x).*abs(x)))) > eps then pause,end
if abs(norm(x,2) - norm(x)) > eps then pause,end
p=0.5;
if abs(norm(x,p) - sum(abs(x)^p)^(1/p)) > eps then pause,end
p=2.5;
if abs(norm(x,p) - sum(abs(x)^p)^(1/p)) > eps then pause,end
if abs(norm(x,'inf') -maxi(abs(x))) > eps then pause,end
if abs(norm(x,'inf') -norm(x,%inf)) > eps then pause,end
if abs(norm(x,'fro') -norm(x,2)) > eps then pause,end

// complex 
x=x+%i*x;
if abs(norm(x,1) - sum(abs(x))) > eps then pause,end
if abs(norm(x,2) - sqrt(sum(abs(x).*abs(x)))) > eps then pause,end
if abs(norm(x,2) - norm(x)) > eps then pause,end
p=0.5;
// 100*%eps is needed for linux 
if abs(norm(x,p) - maxi(abs(x))*sum((abs(x)/maxi(abs(x)))^p)^(1/p))> 100*%eps then pause,end
p=2.5;
if abs(norm(x,p) - maxi(abs(x))*sum((abs(x)/maxi(abs(x)))^p)^(1/p))> 100*%eps then pause,end
if abs(norm(x,'inf') -maxi(abs(x)))> eps then pause,end
if abs(norm(x,'inf') -norm(x,%inf)) > eps then pause,end
if abs(norm(x,'fro') -norm(x,2))> eps then pause,end

// scalar 

x=[1.23];
if abs(norm(x,1) - sum(abs(x))) > eps then pause,end
if abs(norm(x,2) - sqrt(sum(abs(x).*abs(x)))) > eps then pause,end
if abs(norm(x,2) - norm(x)) > eps then pause,end
p=0.5;
if abs(norm(x,p) - sum(abs(x)^p)^(1/p)) > eps then pause,end
p=2.5;
if abs(norm(x,p) - sum(abs(x)^p)^(1/p)) > eps then pause,end
if abs(norm(x,'inf') -maxi(abs(x))) > eps then pause,end
if abs(norm(x,'inf') -norm(x,%inf)) > eps then pause,end
if abs(norm(x,'fro') -norm(x,2)) > eps then pause,end

// Matrices 
a=rand(10,10,'g');
if abs(norm(a,1) - maxi(sum(abs(a),'r'))) > eps then pause,end
if abs(norm(a,'inf') - maxi(sum(abs(a),'c'))) > eps then pause,end
if abs(norm(a,%inf) - maxi(sum(abs(a),'c'))) > eps then pause,end
if abs(norm(a,2) - maxi(svd(a))) > eps then pause,end
if abs(norm(a,'fro') - norm(matrix(a,1,size(a,'*')),2)) > eps then pause,end

a=a+%i*a;
if abs(norm(a,1) - maxi(sum(abs(a),'r'))) > eps then pause,end
if abs(norm(a,'inf') - maxi(sum(abs(a),'c'))) > eps then pause,end
if abs(norm(a,%inf) - maxi(sum(abs(a),'c'))) > eps then pause,end
if abs(norm(a,2) - maxi(svd(a))) > eps then pause,end
if abs(norm(a,'fro') - norm(matrix(a,1,size(a,'*')),2)) > eps then pause,end



