//matrix (double) created by C function
x1=ex3f();
if norm(x1-matrix((1:5*3),3,5)) > %eps then pause,end

//matrix (int) created by C function
x2=ex4f();
if norm(x2-matrix((1:5*3),3,5)) > %eps then pause,end

//Character string created by C function
x3=ex5f();
if x3<>"Scilab is ..." then pause,end


// all together 

[y1,y2,y3]=ex6f();
if y1<>x3 then pause,end
if norm(y2-x2) > %eps then pause,end
if norm(y3-x1) > %eps then pause,end
