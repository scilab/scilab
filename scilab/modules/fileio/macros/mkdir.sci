function [status,msg]=mkdir(varargin)
// mkdir Make a directory

//------------------------------------------------------------------------
// Allan CORNET
// INRIA 2004
//------------------------------------------------------------------------
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

  NewDirectory=fullfile(DirName,NewDirName)
  
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
        NewDirectory='""'+NewDirectory+'""'
        ver=OS_Version();
        if ver == 'Windows 98' | ver == 'Windows 95' | ver == 'Windows ME' then
          batchlog = ' >'+ TMPDIR+'\mkdir.out';
        else
          batchlog = ' >""'+ TMPDIR+'\mkdir.out""' +' 2>""'+TMPDIR+'\mkdir.err""';
        end
        cmd='mkdir '+NewDirectory;
        cmdline=cmd+batchlog;
      else
        batchlog = ' >'+ TMPDIR+'/mkdir.out' +' 2>'+TMPDIR+'/mkdir.err';
        cmd='mkdir '+NewDirectory;
        cmdline=cmd+batchlog;
      end
      status=unix(cmdline);
      if (status~=0) then
        if MSDOS then
      	  ver=OS_Version();
          if ver == 'Windows 98' | ver == 'Windows 95' | ver == 'Windows ME' then
            msg='Error :'+cmd;
          else
            msg='Error : '+mgetl(TMPDIR+'\mkdir.err');
            msg=msg+' '+mgetl(TMPDIR+'\mkdir.out');
          end
        else
          msg='Error : '+mgetl(TMPDIR+'/mkdir.err');
          msg=msg+' '+mgetl(TMPDIR+'/mkdir.out');
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
