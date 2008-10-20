function r=with_macros_source()
  if MSDOS then
    [msg,r]=dos('dir '+WSCI+'\macros\*.sci /s');
  else
  	dirtmp=pwd()
  	cd (SCI+'/macros')
  	a=unix('ls */*.sci 1>NULL 2>NULL');
  	if a then
  	  r=%F;
  	else
  	  r=%T;
  	end
  	cd(dirtmp)
  end
endfunction
