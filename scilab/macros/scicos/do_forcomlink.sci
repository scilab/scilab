function [ok]=do_forcomlink(funam,txt)
// Copyright INRIA
if stripblanks(funam)==emptystr() then 
  ok=%f;x_message('sorry fortran file name not defined');return
end
if MSDOS then 
	out_f = strsubst(TMPDIR,'/','\')+'\'+funam+'.f';
  	host('del '+ out_f);
else 
	unix_s('\rm -f '+TMPDIR+'/'+funam+'.f');
end
write(TMPDIR+'/'+funam+'.f',txt,'(a)')
if MSDOS then 
  [a,b]=c_link(funam); while a ; ulink(b);[a,b]=c_link(funam);end
  cur_wd = getcwd();
  chdir(TMPDIR);
  if (getenv('COMPILER','NO')=='ABSOFT')
    cmd_win='amake /f '"'+SCI+'\demos\intro\MakeF.amk'"  TARGET=';
  else
    cmd_win='nmake /f '"'+SCI+'\demos\intro\MakeF.mak'"  TARGET=';
  end
  cmd_win=cmd_win+funam+' SCIDIR1='"'+strsubst(SCI,'/','\')+''"';
//  disp(cmd_win);
  ww=unix_g(cmd_win);
  chdir(cur_wd);
else
  ww=unix_g('cd '+TMPDIR+';make -f '+SCI+'/util/Make.unx '+funam+'.o SCIDIR='+SCI)
end 
if ww==emptystr() then 
  ok=%f;x_message('sorry compilation problem');return
else
  errcatch(-1,'continue')
if MSDOS then 
  junk=link(TMPDIR+'/'+funam+'.dll',funam);
else 
  [a,b]=c_link(funam); while a ; ulink(b);[a,b]=c_link(funam);end  
  junk=link(TMPDIR+'/'+funam+'.o',funam);
end
  if iserror(-1)==1 then 
    ok=%f;x_message('sorry link problem');
    errclear(-1)
    errcatch(-1)
    return;
  end
end
ok=%t

