//------------------------------------------------------------------------
// Allan CORNET
// INRIA 2004
//------------------------------------------------------------------------
// rmdir remove a directory
function [status,msg]=rmdir(varargin)
  lhs=argn(1);   
  rhs=argn(2);
  
  DirName='';
  status=0;
  msg='';
  SubDirMode=%F;
  
  select rhs
    case 0
     error('Number of parameters incorrect.');	
    break
    case 1
      DirName=""""+varargin(1)+"""";
    break
    case 2
      DirName=""""+varargin(1)+"""";
      SubDir=convstr(varargin(2),'u');
      if (SubDir=='S') then
        SubDirMode=%T;
      else
        error('Second parameters incorrect.');
      end
    break
  else
     error('Number of parameters incorrect.');
  end
  
  if MSDOS then
    ver=OS_Version();
    if ver == 'Windows 98' | ver == 'Windows 95' | ver == 'Windows ME' then
      batchlog = ' >'+ TMPDIR+'\rmdir.out';
    else
      batchlog = ' >'+ TMPDIR+'\rmdir.out' +' 2>'+TMPDIR+'\rmdir.err';
    end
    if SubDirMode then
      cmd = 'rmdir /s /q '+DirName; 
    else
      cmd = 'rmdir '+DirName;
    end
  else
    batchlog = ' >'+ TMPDIR+'/rmdir.out' +' 2>'+TMPDIR+'/rmdir.err';
    if SubDirMode then
      cmd = 'rm -r -f '+DirName;
    else
      cmd = 'rmdir '+DirName;
    end
  end
  
  cmdline =cmd+batchlog;	
  status=unix(cmdline);
  if (status~=0) then
    if MSDOS then
      ver=OS_Version();
      if ver == 'Windows 98' | ver == 'Windows 95' | ver == 'Windows ME' then
         msg='Error :'+cmd;
      else
         msg='Error : '+mgetl(TMPDIR+'\rmdir.err');
         msg=msg+' '+mgetl(TMPDIR+'\rmdir.out');
      end
    else
      msg='Error : '+mgetl(TMPDIR+'/rmdir.err');
      msg=msg+' '+mgetl(TMPDIR+'/rmdir.out');
    end
    status=0;
  else
    msg=''; 
    status=1;
  end

endfunction
//------------------------------------------------------------------------

