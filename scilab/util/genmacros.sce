//------------------------------------
// Allan CORNET INRIA 2005
//------------------------------------
// build minimals macros for use genlib
// use only for macros/util directory
//------------------------------------
clear all;
mode(-1);
//------------------------------------
function RemoveMinimalMacros()
  SCI=getenv('SCI');
  MSDOS=(getos()=='Windows')
  if MSDOS then
    CurrentDir=pwd();
    chdir(SCI+'/modules/functions/macros');
    unix('if EXIST ""'+'genlib.bin'+'"" del /F ""'+'genlib.bin'+'""');
    chdir(SCI+'/modules/fileio/macros');
    unix('if EXIST ""'+'fileparts.bin'+'"" del /F ""'+'fileparts.bin'+'""');
    chdir(SCI+'/modules/core/macros');
    unix('if EXIST ""'+'getshell.bin'+'"" del /F ""'+'getshell.bin'+'""');
    chdir(SCI+'/modules/fileio/macros');
    unix('if EXIST ""'+'pathconvert.bin'+'"" del /F ""'+'pathconvert.bin'+'""');
    chdir(SCI+'/modules/string/macros');
    unix('if EXIST ""'+'stripblanks.bin'+'"" del /F ""'+'stripblanks.bin'+'""');
    chdir(SCI+'/modules/fileio/macros');
    unix('if EXIST ""'+'listfiles.bin'+'"" del /F ""'+'listfiles.bin'+'""');
    chdir(SCI+'/modules/io/macros');
    unix('if EXIST ""'+'unix_g.bin'+'"" del /F ""'+'unix_g.bin'+'""');
    chdir(SCI+'/modules/core/macros');
    unix('if EXIST ""'+'OS_Version.bin'+'"" del /F ""'+'OS_Version.bin'+'""');
    chdir(SCI+'/modules/fileio/macros');
    unix('if EXIST ""'+'isdir.bin'+'"" del /F ""'+'isdir.bin'+'""');
    unix('if EXIST ""'+'basename.bin'+'"" del /F ""'+'basename.bin'+'""');
    chdir(SCI+'/modules/fileio/macros');
    unix('if EXIST ""'+'mputl.bin'+'"" del /F ""'+'mputl.bin'+'""');
    chdir(SCI+'/modules/overloading/macros');
    unix('if EXIST ""'+'%c_a_c.bin'+'"" del /F ""'+'%c_a_c.bin'+'""');
    chdir(CurrentDir);
  else
    unix('rm -f '+SCI+'/modules/functions/macros/genlib.bin');
    unix('rm -f '+SCI+'/modules/fileio/macros/fileparts.bin');
    unix('rm -f '+SCI+'/modules/core/macros/getshell.bin');
    unix('rm -f '+SCI+'/modules/fileio/macros/pathconvert.bin');
    unix('rm -f '+SCI+'/modules/string/macros/stripblanks.bin');
    unix('rm -f '+SCI+'/modules/fileio/macros/listfiles.bin');
    unix('rm -f '+SCI+'/modules/io/macros/unix_g.bin');
    unix('rm -f '+SCI+'/modules/core/macros/OS_Version.bin');
    unix('rm -f '+SCI+'/modules/fileio/macros/isdir.bin');
    unix('rm -f '+SCI+'/modules/overloading/macros/%c_a_c.bin');
    unix('rm -f '+SCI+'/modules/fileio/macros/basename.bin');
    unix('rm -f '+SCI+'/modules/fileio/macros/mputl.bin');
  end
  clear MSDOS;
  clear SCI;
endfunction
//------------------------------------
function BuildMinimalMacros()
  MSDOS=(getos()=='Windows');
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
  
  SCI=getenv('SCI');
  scilabstart="SCI=getenv(''SCI'');MSDOS=(getos()==''Windows'');TMPDIR=getenv(''TMPDIR'');"
  scilabquit='clear;quit;'
  
  cmd=scilabstart+'getf(SCI+''/modules/core/macros/OS_Version.sci'');save(SCI+''/modules/core/macros/OS_Version.bin'');'+scilabquit
  unix(startline+cmd+endline);
  unix(SCI+LineToDo);

  cmd=scilabstart+'getf(SCI+''/modules/fileio/macros/mputl.sci'');save(SCI+''/modules/fileio/macros/mputl.bin'');'+scilabquit;
  unix(startline+cmd+endline);
  unix(SCI+LineToDo);
  
  cmd=scilabstart+'getf(SCI+''/modules/fileio/macros/basename.sci'');save(SCI+''/modules/fileio/macros/basename.bin'');'+scilabquit;
  unix(startline+cmd+endline);
  unix(SCI+LineToDo);

  cmd=scilabstart+'getf(SCI+''/modules/overloading/macros/%c_a_c.sci'');save(SCI+''/modules/overloading/macros/%c_a_c.bin'');'+scilabquit;
  unix(startline+cmd+endline);
  unix(SCI+LineToDo);
  
  cmd=scilabstart+'getf(SCI+''/modules/fileio/macros/isdir.sci'');save(SCI+''/modules/fileio/macros/isdir.bin'');'+scilabquit;
  unix(startline+cmd+endline);
  unix(SCI+LineToDo);

  cmd=scilabstart+'getf(SCI+''/modules/io/macros/unix_g.sci'');save(SCI+''/modules/io/macros/unix_g.bin'');'+scilabquit;
  unix(startline+cmd+endline);
  unix(SCI+LineToDo);
  
  cmd=scilabstart+'getf(SCI+''/modules/fileio/macros/listfiles.sci'');save(SCI+''/modules/fileio/macros/listfiles.bin'');'+scilabquit;
  unix(startline+cmd+endline);
  unix(SCI+LineToDo);

  cmd=scilabstart+'getf(SCI+''/modules/string/macros/stripblanks.sci'');save(SCI+''/modules/string/macros/stripblanks.bin'');'+scilabquit;
  unix(startline+cmd+endline);
  unix(SCI+LineToDo);
  
  cmd=scilabstart+'getf(SCI+''/modules/fileio/macros/pathconvert.sci'');save(SCI+''/modules/fileio/macros/pathconvert.bin'');'+scilabquit;
  unix(startline+cmd+endline);
  unix(SCI+LineToDo);
  
  cmd=scilabstart+'getf(SCI+''/modules/fileio/macros/fileparts.sci'');save(SCI+''/modules/fileio/macros/fileparts.bin'');'+scilabquit;
  unix(startline+cmd+endline);
  unix(SCI+LineToDo);
  
  cmd=scilabstart+'getf(SCI+''/modules/core/macros/getshell.sci'');save(SCI+''/modules/core/macros/getshell.bin'');'+scilabquit;
  unix(startline+cmd+endline);
  unix(SCI+LineToDo);

  cmd=scilabstart+'getf(SCI+''/modules/functions/macros/genlib.sci'');save(SCI+''/modules/functions/macros/genlib.bin'');'+scilabquit;
  unix(startline+cmd+endline);
  unix(SCI+LineToDo);

  clear cmd
  clear scilabstart;
  clear scilabquit;
  clear LineToDo
  clear startline
  clear endline
  
  if MSDOS then
    unix('if EXIST ""'+'cmd.sce'+'"" del /F ""'+'cmd.sce'+'""');
  else
    unix('rm -f cmd.sce');
  end
  
  clear MSDOS
  
endfunction
//------------------------------------
RemoveMinimalMacros();
BuildMinimalMacros();
clear BuildMinimalMacros;
SCI=getenv('SCI');
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
clear SCI;
// Need to remove macros binary files before build by genlib
RemoveMinimalMacros();
clear RemoveMinimalMacros;
//------------------------------------
