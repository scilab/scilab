function [ok]=do_ccomlink(funam,txt)
// Copyright INRIA
  if stripblanks(funam)==emptystr() then 
    ok=%f;x_message('sorry C file name not defined');return
  end
  cur_wd = getcwd();
  chdir(TMPDIR);
   mputl(txt,funam+'.c');
  ilib_for_link;
  ilib_link_gen_loader(funam,'c','loader.sce',[],"");
  ilib_link_gen_Make(funam,funam+'.o',"",'Makelib',"",...
		     "","","","");
  ierr= execstr('ilib_compile(''lib''+funam,''Makelib'',funam+''.o'')', ...
		'errcatch');
  if ierr<> 0 then 
    chdir(cur_wd)
    x_message('Sorry compilation problem');ok=%f;return;
  end
  [a,b]=c_link(funam); while a;  ulink(b);[a,b]=c_link(funam);end
  ierr= exec('loader.sce','errcatch')
  if ierr<> 0 then     
    chdir(cur_wd)
    x_message('Sorry link problem');ok=%f;return;
  end
  chdir(cur_wd)
  ok=%t
endfunction
