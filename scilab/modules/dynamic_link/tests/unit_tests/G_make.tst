if MSDOS then
  txt = ['message:',
	       '  @echo ------------------------------------------',
         '  @echo From Makefile.mak',
	       '  @echo ------------------------------------------',
	       ' '];
  mputl(txt,TMPDIR+'/makefile.mak')
  current_dir = pwd();  
  cd TMPDIR
  files = G_make(['',''],'message');
  cd(current_dir);
end