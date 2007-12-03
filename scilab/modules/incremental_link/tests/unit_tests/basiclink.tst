// Copyright Enpc 

// basic test for link 

foo=['void foo(double *a,double *b,double *c)';
     '{ *c = *a + *b; }'  ];

// we use TMPDIR for compilation 
	
if ~c_link('foo') then
  path = getcwd(); 
  chdir(TMPDIR); 
  mputl(foo,'foo.c');
  ilib_for_link(['foo'],'foo.o',[],"c");
  exec loader.sce ;
  chdir(path) 
end	

//5+7 by C function
call('foo',5,1,'d',7,2,'d','out',[1,1],3,'d')

// basic test for addinter 
//--------------------------
 
if ~c_link('libintertest') then
  files=['addinter.o'];
  ilib_build('libintertest',['scifun1','intfun1'],files,[]);
  // load the shared library 
  exec loader.sce ;
  if norm(scifun1(%pi)- sin(%pi+1)/%pi ) > %eps then pause;end 
end

