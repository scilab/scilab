//------------------------------------------------------------------------
// Allan CORNET
// INRIA 2004
//------------------------------------------------------------------------
// mkdir Make a directory
function [status,msg]=mkdir(varargin)
  lhs=argn(1);   
  rhs=argn(2);
  
  DirName='';
  NewDirName='';
  status=0;
  msg='';
  
  select rhs
    case 0
     error('Number of parameters incorrect.');	
    break
    case 1
      DirName=pwd();
      NewDirName=varargin(1);
    break
    case 2
      DirName=varargin(1);
      NewDirName=varargin(2);
    break
  else
     error('Number of parameters incorrect.');
  end

  NewDirectory=""""+fullfile(DirName,NewDirName)+"""";
  
  if (fileinfo(NewDirectory)==[]) then 
    // Le repertoire n'existe pas
    status=1;
  else
    // Le fichier ou repertoire existe
    if (isdir(NewDirectory)) then
      // c'est un repertoire
      status=2; 
    else
      // c'est un fichier
      status=-2;
    end
  end
  
  select status
    case 0
      ErrMsg='a file with the same name already exists in '+DirName+' .';
      msg=ErrMsg;
    break
    
    case 1
      if MSDOS then
        cmdline='mkdir '+NewDirectory+'> '+TMPDIR+'\mkdir.log';
      else
        cmdline='mkdir '+NewDirectory+'> '+TMPDIR+'/mkdir.log';
      end
      status=unix(cmdline);
      if (status~=0) then
        if MSDOS then
          msg=mgetl(TMPDIR+'\mkdir.log');  
        else
          msg=mgetl(TMPDIR+'/mkdir.log');  
        end
        status=0;
      else
        msg=''; 
        status=1;
      end
    break
    
    case 2
      ErrMsg='this directory already exists in '+DirName+' .';
      msg=ErrMsg;
    break
    
  else
     ErrMsg='Error(s) Impossible to create directory '+NewDirectory+' .';
     msg=ErrMsg;
  end
 
endfunction
//------------------------------------------------------------------------

