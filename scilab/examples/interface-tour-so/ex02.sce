// C 
//--------------------------------------------------

a=ex2c_1([1,2,3]+%i*[4,5,5]);
if norm(a-2*[1,2,3]-%i*[4,5,5]*3) > %eps then pause,end

a=ex2c_1([1,2,3]);
if norm(a-2*[1,2,3]) > %eps then pause,end

// Complex matrix as complex array 

a=ex2c_2([1,2,3]+%i*[4,5,5]);
if norm(a-2*[1,2,3]-%i*[4,5,5]*3) > %eps then pause,end



a=ex2f_1([1,2,3]+%i*[4,5,5]);
if norm(a-2*[1,2,3]-%i*[4,5,5]*3) > %eps then pause,end

a=ex2f_1([1,2,3]);
if norm(a-2*[1,2,3]) > %eps then pause,end

// Complex matrix as complex array 

a=ex2f_2([1,2,3]+%i*[4,5,5]);
if norm(a-2*([1,2,3]+%i*[4,5,5])) > %eps then pause,end
















