//           CALLING EXTERNAL ROUTINE
foo=['      subroutine foo(a,b,c)';
     '      double precision  a,b,c';
     '      c=a+b ';
     '      return'; '       end'];
if getenv('WIN32','NO')=='OK' & getenv('COMPILER','NO')=='VC++' then 
	unix_s('del foo.f')
	write('foo.f',foo);
	WSCI=getenv('WSCI');
	cmd='nmake /f ""'+WSCI+'\demos\intro\MakeF.mak"" TARGET=foo SCIDIR1=""'+WSCI+'""';
	unix_s(cmd)  //Compiling...(needs a compiler);
	link('foo.dll','foo');   //Linking to Scilab
else 
	unix_s('rm -f foo.c');
	write('foo.c',foo);
	unix_s('make -f '+SCI+'/demos/intro/Makefile foo.o')     //Compiling...(needs a compiler)
	link('foo.o','foo','C')    //Linking to Scilab
end 

//5+7 by C function
fort('foo',5,1,'d',7,2,'d','out',[1,1],3,'d')

