//------------------------------------------------------------------------
// Allan CORNET
// INRIA 2004
//------------------------------------------------------------------------
// rmdir Make a directory
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
	  if SubDirMode then
      cmdline = 'rmdir /s /q '+DirName+' > '+TMPDIR+'\rmdir.log';
        else
      cmdline = 'rmdir '+DirName+' > '+TMPDIR+'\rmdir.log';
        end
	else
	  if SubDirMode then
      cmdline = 'rm -r -f '+DirName+' > '+TMPDIR+'/rmdir.log';
    else
      cmdline = 'rmdir '+DirName+' > '+TMPDIR+'/rmdir.log';
    end
	end
	
	status=unix(cmdline);
    if (status~=0) then
      if MSDOS then
        msg='Error :'+cmdline;
      else
        msg='Error :'+cmdline;  
      end
      status=0;
    else
      msg=''; 
      status=1;
    end
	
  
endfunction
//------------------------------------------------------------------------

