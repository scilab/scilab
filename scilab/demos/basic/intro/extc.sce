//           CALLING EXTERNAL ROUTINE
foo=['void fooc(a,b,c)';
     'double *a,*b,*c;';
     '{ *c = *a + *b; }'  ];
if getenv('WIN32','NO')=='OK' & getenv('COMPILER','NO')=='VC++' then 
	unix_s('del fooc.c')
	write('fooc.c',foo);
	WSCI=getenv('WSCI');
	cmd='nmake /f ""'+WSCI+'\demos\intro\MakeC.mak"" TARGET=foo SCIDIR1=""'+WSCI+'""';
	unix_s(cmd)  //Compiling...(needs a compiler);
	link('fooc.dll','fooc','C');    //Linking to Scilab
else 
	unix_s('rm -f fooc.c');
	write('fooc.c',foo);
	unix_s('make -f '+SCI+'/demos/intro/Makefile fooc.o')     //Compiling...(needs a compiler)
	link('fooc.o','fooc','C')    //Linking to Scilab
end 

//5+7 by C function
fort('fooc',5,1,'d',7,2,'d','out',[1,1],3,'d')
