
//matrix (double) created by C function
x1=ex4c_1();
if norm(x1-matrix((1:5*3),3,5)) > %eps then pause,end

//matrix (int) created by C function
x2=ex4c_2();
x2ref = matrix((1:5*3),3,5);
x2ref(1)=12;
if norm(x2- x2ref) > %eps then pause,end

//Character string created by C function
x3=ex4c_3();
if x3<>"Scilab is ..." then pause,end

// all together 

[y1,y2,y3]=ex4c_4();
if y1<>"Xcilab is ..." then pause,end
if norm(y2-x2ref) > %eps then pause,end
x1ref=x1; x1ref(1)=89;
if norm(y3-x1ref) > %eps then pause,end


//matrix (double) created by C function
x1=ex4f_1();
if norm(x1-matrix((1:5*3),3,5)) > %eps then pause,end

//matrix (int) created by C function
x2=ex4f_2();
if norm(x2-matrix((1:5*3),3,5)) > %eps then pause,end

//Character string created by C function
x3=ex4f_3();
if x3<>"Scilab is ..." then pause,end


// all together 

[y1,y2,y3]=ex4f_4();
if y1<>x3 then pause,end
if norm(y2-x2) > %eps then pause,end
if norm(y3-x1) > %eps then pause,end
