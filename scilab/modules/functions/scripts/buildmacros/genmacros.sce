//------------------------------------
// Allan CORNET INRIA 2005
//------------------------------------
// build minimals macros for use genlib
// use only for macros/util directory
//------------------------------------
clear all;
mode(-1);
//------------------------------------
function BuildMinimalMacros()
  LineToDo='';
  startline='';
  endline='';
  if MSDOS then
    LineToDo='/bin/scilex -ns -nwni -f cmd.sce';
    startline='echo ';
    endline='>cmd.sce';
  else
    LineToDo='/bin/scilab -ns -nwni -f cmd.sce';
    startline='echo ""';
    endline='"">cmd.sce';
  end
  
  scilabstart=""
  scilabquit='clear;quit;'
  
	if (newest(SCI+'/modules/overloading/macros/%c_a_c.sci',SCI+'/modules/overloading/macros/%c_a_c.bin') == 1) then
	  cmd=scilabstart+'getf(SCI+''/modules/overloading/macros/%c_a_c.sci'');save(SCI+''/modules/overloading/macros/%c_a_c.bin'');'+scilabquit;
    unix(startline+cmd+endline);
    unix(SCI+LineToDo);
  end
  load(SCI+'/modules/overloading/macros/%c_a_c.bin');

	macroname=SCI+'/modules/core/macros/OS_Version';
	scibin=macroname+['.sci';'.bin'];
	if ( newest(scibin) == 1 ) then
    cmd=scilabstart+'getf('''+scibin(1)+''');save('''+scibin(2)+''');'+scilabquit;
    unix(startline+cmd+endline);
    unix(SCI+LineToDo);
  end

  macroname=SCI+'/modules/fileio/macros/mputl';
	scibin=macroname+['.sci';'.bin'];
	if ( newest(scibin) == 1 ) then
    cmd=scilabstart+'getf('''+scibin(1)+''');save('''+scibin(2)+''');'+scilabquit;
    unix(startline+cmd+endline);
    unix(SCI+LineToDo);
  end
  
  macroname=SCI+'/modules/fileio/macros/basename';
	scibin=macroname+['.sci';'.bin'];
  if ( newest(scibin) == 1 ) then
    cmd=scilabstart+'getf('''+scibin(1)+''');save('''+scibin(2)+''');'+scilabquit;
    unix(startline+cmd+endline);
    unix(SCI+LineToDo);
  end
  
  macroname=SCI+'/modules/fileio/macros/isdir';
	scibin=macroname+['.sci';'.bin'];
  if ( newest(scibin) == 1 ) then
    cmd=scilabstart+'getf('''+scibin(1)+''');save('''+scibin(2)+''');'+scilabquit;
    unix(startline+cmd+endline);
    unix(SCI+LineToDo);
  end

  macroname=SCI+'/modules/io/macros/unix_g';
	scibin=macroname+['.sci';'.bin'];
  if ( newest(scibin) == 1 ) then
    cmd=scilabstart+'getf('''+scibin(1)+''');save('''+scibin(2)+''');'+scilabquit;
    unix(startline+cmd+endline);
    unix(SCI+LineToDo);
  end
  
  macroname=SCI+'/modules/fileio/macros/listfiles';
	scibin=macroname+['.sci';'.bin'];
  if ( newest(scibin) == 1 ) then
    cmd=scilabstart+'getf('''+scibin(1)+''');save('''+scibin(2)+''');'+scilabquit;
    unix(startline+cmd+endline);
    unix(SCI+LineToDo);
  end

  macroname=SCI+'/modules/string/macros/stripblanks';
	scibin=macroname+['.sci';'.bin'];
  if ( newest(scibin) == 1 ) then
    cmd=scilabstart+'getf('''+scibin(1)+''');save('''+scibin(2)+''');'+scilabquit;
    unix(startline+cmd+endline);
    unix(SCI+LineToDo);
  end
  
  macroname=SCI+'/modules/fileio/macros/pathconvert';
	scibin=macroname+['.sci';'.bin'];
  if ( newest(scibin) == 1 ) then
    cmd=scilabstart+'getf('''+scibin(1)+''');save('''+scibin(2)+''');'+scilabquit;
    unix(startline+cmd+endline);
    unix(SCI+LineToDo);
  end
  
  macroname=SCI+'/modules/fileio/macros/fileparts';
	scibin=macroname+['.sci';'.bin'];
  if ( newest(scibin) == 1 ) then
    cmd=scilabstart+'getf('''+scibin(1)+''');save('''+scibin(2)+''');'+scilabquit;
    unix(startline+cmd+endline);
    unix(SCI+LineToDo);
  end
  
  macroname=SCI+'/modules/core/macros/getshell';
	scibin=macroname+['.sci';'.bin'];
  if ( newest(scibin) == 1 ) then
    cmd=scilabstart+'getf('''+scibin(1)+''');save('''+scibin(2)+''');'+scilabquit;
    unix(startline+cmd+endline);
    unix(SCI+LineToDo);
  end

  macroname=SCI+'/modules/functions/macros/genlib';
	scibin=macroname+['.sci';'.bin'];
  if ( newest(scibin) == 1 ) then
    cmd=scilabstart+'getf('''+scibin(1)+''');save('''+scibin(2)+''');'+scilabquit;
    unix(startline+cmd+endline);
    unix(SCI+LineToDo);
  end

	clear macroname;
	clear scibin;
  clear cmd;
  clear scilabstart;
  clear scilabquit;
  clear LineToDo;
  clear startline;
  clear endline;
  
  if MSDOS then
    unix('if EXIST ""'+'cmd.sce'+'"" del /F ""'+'cmd.sce'+'""');
  else
    unix('rm -f cmd.sce');
  end
  
endfunction
//------------------------------------
BuildMinimalMacros();
clear BuildMinimalMacros;
load(SCI+'/modules/fileio/macros/fileparts.bin');
load(SCI+'/modules/core/macros/getshell.bin');
load(SCI+'/modules/fileio/macros/pathconvert.bin');
load(SCI+'/modules/string/macros/stripblanks.bin');
load(SCI+'/modules/fileio/macros/listfiles.bin');
load(SCI+'/modules/io/macros/unix_g.bin');
load(SCI+'/modules/core/macros/OS_Version.bin');
load(SCI+'/modules/fileio/macros/isdir.bin');
load(SCI+'/modules/overloading/macros/%c_a_c.bin');
load(SCI+'/modules/fileio/macros/basename.bin');
load(SCI+'/modules/fileio/macros/mputl.bin');
load(SCI+'/modules/functions/macros/genlib.bin');
//------------------------------------
