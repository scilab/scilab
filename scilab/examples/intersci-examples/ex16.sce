//ex16 example ( boolean matrices )
//1-Creating interface source file 
// Making object files
//   Interface file '/tmp/ex16fi.o';
//   User's files '/tmp/ex16c.o';
files=G_make(['/tmp/ex16fi.o','/tmp/ex16c.o';],'ex16.dll');
//2-Link object files .o with addinter
//addinter(files,'intex16',intex1_funs);
exec('ex16fi.sce');
//Run Scilab functions:
a=[%t,%t;%f,%f];

// simple matrix argument 

b=ext16ca(a);
if b<>~a then pause,end

// matrix and return a matrix in a list 

b=ext16cb(a);
if b(1)<>~a then pause,end

// new matrix in intersci 

b=ext16cc(a);
if b<>~a then pause,end

// new matrix returned in a list 

b=ext16cd(a);
if b(1)<>~a then pause,end

// list argument with a matrix  

b=ext16ce(list(a))
if b<>~a then pause,end

// list argument + list output 

b=ext16cf(list(a))
if b(1)<>~a then pause,end

// cboolf 

b=ext16cg();
if b<>[%f,%t;%t,%f;%f,%t] then pause,end

// cboolf + list 

b=ext16ch();
if b(1)<>[%f,%t;%t,%f;%f,%t] then pause,end



