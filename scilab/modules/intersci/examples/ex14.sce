//ex14 example
//1-Creating interface source file 
// from ex14fi.desc file by call to intersci
// Making object files
//   Interface file '/tmp/ex14fi.o';
//   User's files '/tmp/ex14c.o'
files=G_make(['/tmp/ex14fi.o','/tmp/ex14c.o';],'ex14.dll');
//2-Link object files .o with addinter
//addinter(files,'intex14',intex1_funs);
exec('ex14fi.sce');
//Run Scilab functions:
a=[0,0,1.23;0,2.32,0;3.45,0,0];
ai=[0,0,9;0,6,0;7,0,0];
spa=sparse(a);
spai=sparse(a+%i*ai);
// simple sparse argument 

b=spt1(spa);
if norm(full(b- spa)) > %eps then pause,end

b=spt1(spai);
if norm(full(b- spai)) > %eps then pause,end

// sparse argument + conversion to int 

b=spt2(spa);
if norm(full(b- int(spa))) > %eps then pause,end

b=spt2(spai);
if norm(full(b- int(spai))) > %eps then pause,end

// sparse and return a sparse in a list 

b=spt3(spa);
if norm(full(b(1)- spa)) > %eps then pause,end

b=spt3(spai);
if norm(full(b(1)- spai)) > %eps then pause,end

// new sparse in intersci 

b=spt4(spa);
if norm(full(b- 2*spa)) > %eps then pause,end

b=spt4(spai);
if norm(full(b- 2*spai)) > %eps then pause,end

// new sparse + conversion to int

b=spt5(spa);
if norm(full(b- int(2*spa))) > %eps then pause,end

b=spt5(spai);
if norm(full(b- int(2*spai))) > %eps then pause,end

// new sparse returned in a list 

b=spt6(spa);
if norm(full(b(1)- 2*spa)) > %eps then pause,end

b=spt6(spai);
if norm(full(b(1)- 2*spai)) > %eps then pause,end

// list argument with a sparse 

b=spt7(list(spa));
if norm(full(b- spa)) > %eps then pause,end

b=spt7(list(spai));
if norm(full(b- spai)) > %eps then pause,end

// list argument + conversion 

b=spt8(list(spa));
if norm(full(b- int(spa))) > %eps then pause,end

b=spt8(list(spai));
if norm(full(b- int(spai))) > %eps then pause,end

// list argument + list output 

b=spt9(list(spa));
if norm(full(b(1)- spa)) > %eps then pause,end

b=spt9(list(spai));
if norm(full(b(1)- spai)) > %eps then pause,end

b=spt10(spa);
if norm(full(b- 2*spa)) > %eps then pause,end

