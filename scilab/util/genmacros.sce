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
    chdir(SCI+'/macros/util');
    unix('if EXIST ""'+'genlib.bin'+'"" del /F ""'+'genlib.bin'+'""');
    unix('if EXIST ""'+'fileparts.bin'+'"" del /F ""'+'fileparts.bin'+'""');
    unix('if EXIST ""'+'getshell.bin'+'"" del /F ""'+'getshell.bin'+'""');
    unix('if EXIST ""'+'pathconvert.bin'+'"" del /F ""'+'pathconvert.bin'+'""');
    unix('if EXIST ""'+'stripblanks.bin'+'"" del /F ""'+'stripblanks.bin'+'""');
    unix('if EXIST ""'+'listfiles.bin'+'"" del /F ""'+'listfiles.bin'+'""');
    unix('if EXIST ""'+'unix_g.bin'+'"" del /F ""'+'unix_g.bin'+'""');
    unix('if EXIST ""'+'OS_Version.bin'+'"" del /F ""'+'OS_Version.bin'+'""');
    unix('if EXIST ""'+'isdir.bin'+'"" del /F ""'+'isdir.bin'+'""');
    unix('if EXIST ""'+'basename.bin'+'"" del /F ""'+'basename.bin'+'""');
    unix('if EXIST ""'+'mputl.bin'+'"" del /F ""'+'mputl.bin'+'""');
    chdir(SCI+'/macros/percent');
    unix('if EXIST ""'+'%c_a_c.bin'+'"" del /F ""'+'%c_a_c.bin'+'""');
    chdir(CurrentDir);
  else
    unix('rm -f '+SCI+'/macros/util/genlib.bin');
    unix('rm -f '+SCI+'/macros/util/fileparts.bin');
    unix('rm -f '+SCI+'/macros/util/getshell.bin');
    unix('rm -f '+SCI+'/macros/util/pathconvert.bin');
    unix('rm -f '+SCI+'/macros/util/stripblanks.bin');
    unix('rm -f '+SCI+'/macros/util/listfiles.bin');
    unix('rm -f '+SCI+'/macros/util/unix_g.bin');
    unix('rm -f '+SCI+'/macros/util/OS_Version.bin');
    unix('rm -f '+SCI+'/macros/util/isdir.bin');
    unix('rm -f '+SCI+'/macros/percent/%c_a_c.bin');
    unix('rm -f '+SCI+'/macros/util/basename.bin');
    unix('rm -f '+SCI+'/macros/util/mputl.bin');
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
  
  cmd=scilabstart+'getf(SCI+''/macros/util/OS_Version.sci'');save(SCI+''/macros/util/OS_Version.bin'');'+scilabquit
  unix(startline+cmd+endline);
  unix(SCI+LineToDo);
  
  cmd=scilabstart+'getf(SCI+''/macros/util/OS_Version.sci'');save(SCI+''/macros/util/OS_Version.bin'');'+scilabquit;
  unix(startline+cmd+endline);
  unix(SCI+LineToDo);

  cmd=scilabstart+'getf(SCI+''/macros/util/mputl.sci'');save(SCI+''/macros/util/mputl.bin'');'+scilabquit;
  unix(startline+cmd+endline);
  unix(SCI+LineToDo);
  
  cmd=scilabstart+'getf(SCI+''/macros/util/basename.sci'');save(SCI+''/macros/util/basename.bin'');'+scilabquit;
  unix(startline+cmd+endline);
  unix(SCI+LineToDo);

  cmd=scilabstart+'getf(SCI+''/macros/percent/%c_a_c.sci'');save(SCI+''/macros/percent/%c_a_c.bin'');'+scilabquit;
  unix(startline+cmd+endline);
  unix(SCI+LineToDo);
  
  cmd=scilabstart+'getf(SCI+''/macros/util/isdir.sci'');save(SCI+''/macros/util/isdir.bin'');'+scilabquit;
  unix(startline+cmd+endline);
  unix(SCI+LineToDo);

  cmd=scilabstart+'getf(SCI+''/macros/util/unix_g.sci'');save(SCI+''/macros/util/unix_g.bin'');'+scilabquit;
  unix(startline+cmd+endline);
  unix(SCI+LineToDo);
  
  cmd=scilabstart+'getf(SCI+''/macros/util/listfiles.sci'');save(SCI+''/macros/util/listfiles.bin'');'+scilabquit;
  unix(startline+cmd+endline);
  unix(SCI+LineToDo);

  cmd=scilabstart+'getf(SCI+''/macros/util/stripblanks.sci'');save(SCI+''/macros/util/stripblanks.bin'');'+scilabquit;
  unix(startline+cmd+endline);
  unix(SCI+LineToDo);
  
  cmd=scilabstart+'getf(SCI+''/macros/util/pathconvert.sci'');save(SCI+''/macros/util/pathconvert.bin'');'+scilabquit;
  unix(startline+cmd+endline);
  unix(SCI+LineToDo);
  
  cmd=scilabstart+'getf(SCI+''/macros/util/fileparts.sci'');save(SCI+''/macros/util/fileparts.bin'');'+scilabquit;
  unix(startline+cmd+endline);
  unix(SCI+LineToDo);
  
  cmd=scilabstart+'getf(SCI+''/macros/util/getshell.sci'');save(SCI+''/macros/util/getshell.bin'');'+scilabquit;
  unix(startline+cmd+endline);
  unix(SCI+LineToDo);

  cmd=scilabstart+'getf(SCI+''/macros/util/genlib.sci'');save(SCI+''/macros/util/genlib.bin'');'+scilabquit;
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
load(SCI+'/macros/util/fileparts.bin');
load(SCI+'/macros/util/getshell.bin');
load(SCI+'/macros/util/pathconvert.bin');
load(SCI+'/macros/util/stripblanks.bin');
load(SCI+'/macros/util/listfiles.bin');
load(SCI+'/macros/util/unix_g.bin');
load(SCI+'/macros/util/OS_Version.bin');
load(SCI+'/macros/util/isdir.bin');
load(SCI+'/macros/percent/%c_a_c.bin');
load(SCI+'/macros/util/basename.bin');
load(SCI+'/macros/util/mputl.bin');
load(SCI+'/macros/util/genlib.bin');
clear SCI;
// Need to remove macros binary files before build by genlib
RemoveMinimalMacros();
clear RemoveMinimalMacros;
//------------------------------------
