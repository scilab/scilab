a=1:10;b=a+1;c=ones(2,3)+2;
[x,y,z,t]=ex1f('mul',a,b,c);

// Check the result 
if norm(t-(a*2)) > %eps then pause,end
if norm(z-(b*2) ) > %eps then pause,end
if norm(y-(c*2)) > %eps then pause,end
deff('[y]=f(i,j)','y=i+j');
if norm(x- ( y.* feval(1:2,1:3,f))) > %eps then pause,end

[x,y,z,t]=ex1f('add',a,b,c);

// Check the result 
if norm(t-(a+2)) > %eps then pause,end
if norm(z-(b+2) ) > %eps then pause,end
if norm(y-(c+2)) > %eps then pause,end
deff('[y]=f(i,j)','y=i+j');
if norm(x- ( c +2 + feval(1:2,1:3,f))) > %eps then pause,end



