
//           CALLING EXTERNAL ROUTINE
txt=['      subroutine foof(a,b,c)';
     '      double precision  a,b,c';
     '      c=a+b ';
     '      return'; '       end'];

// we use TMPDIR for compilation 
	
if ~c_link('foof') then
  path = getcwd(); 
  chdir(TMPDIR); 
  mputl(txt,'foof.f');
  ilib_for_link(['foof'],'foof.o',[],"f");
  exec loader.sce 
  chdir(path) 
end	

//5+7 by C function
call('foof',5,1,'d',7,2,'d','out',[1,1],3,'d')

