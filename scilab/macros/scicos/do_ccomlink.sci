function [ok]=do_ccomlink(funam,txt)
// Copyright INRIA
  if stripblanks(funam)==emptystr() then 
    ok=%f;x_message('sorry C file name not defined');return
  end
  if MSDOS then 
    out_f = strsubst(TMPDIR,'/','\')+'\'+funam+'.c';
    host('del '+ out_f);
  else 
    unix_s('\rm -f '+TMPDIR+'/'+funam+'.c');
  end
  write(TMPDIR+'/'+funam+'.c',txt,'(a)')
  if MSDOS then 
    [a,b]=c_link(funam); while a ; ulink(b);[a,b]=c_link(funam);end  
    cur_wd = getcwd();
    chdir(TMPDIR);
    if (getenv('COMPILER','NO')=='ABSOFT')
      cmd_win='amake /f '"'+SCI+'\util\MakeC.amk'" TARGET=';
    else
      cmd_win='nmake /f '"'+SCI+'\util\MakeC.mak'" TARGET=';
    end
    cmd_win=cmd_win+funam+' SCIDIR1='"'+strsubst(SCI,'/','\')+''"';
    ww=unix_g(cmd_win);
    chdir(cur_wd);
  else
    ww=unix_g('cd '+TMPDIR+'; make -f '+SCI+'/util/Make.unx '+funam+'.o SCIDIR='+SCI);
  end 

  if ww==emptystr() then 
    ok=%f;message('sorry compilation problem');return
  else
    if MSDOS then 
      instr='junk=link(TMPDIR+''/''+funam+''.dll'',funam,''c'');'
    else
      instr=['[a,b]=c_link(funam); while a;  ulink(b);[a,b]=c_link(funam);end'
	     'junk=link(TMPDIR+''/''+funam+''.o'',funam,''c'');']
    end 
    if execstr(instr,'errcatch')<>0 then
      ok=%f;message('sorry link problem');
      return;
    end
  end
  ok=%t
endfunction
