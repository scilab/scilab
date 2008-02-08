//------------------------------------------------------------------------
// Allan CORNET
// INRIA 2004
//------------------------------------------------------------------------
// copy file
function [status,msg]=copyfile(varargin)
  lhs=argn(1);
  rhs=argn(2);
  
  fname = 'copyfile';

  SourceFile='';
  DestinationFile='';
  flag='';
  Writable=%F;
  status=0;
  msg='';

  SourceDir='';
  DestDir='';

  SrcFile='';
  DestFile='';

  Src='';
  Dest='';

  noErrorOccurred=1;

  select rhs
   case 2
    SourceFile=varargin(1);
    DestinationFile=varargin(2);
    break
   case 3
    SourceFile=varargin(1);
    DestinationFile=varargin(2);
    flag=convstr(varargin(3),'u');
    if (flag=='WRITABLE') then
      Writable=%T;
    else
      error(msprintf(gettext("%s: Wrong value for third input argument: %s expected.\n"),fname,"WRITABLE"));
    end
    break
  else
    error(msprintf(gettext("%s: Wrong number of input arguments.\n"),fname));
  end

  [SourceDir,SourceFile]=GetDirFile(SourceFile);
  [DestDir,DestFile]=GetDirFile(DestinationFile);

  Src = """"+fullfile(SourceDir,SourceFile)+"""";
  Dest = """"+fullfile(DestDir,DestFile)+"""";

  Rep=isdir(SourceDir);
  if (Rep==%F) then
// Check the source directory is a directory
    noErrorOccurred=0;
    ErrorMessage=msprintf(gettext("%s: Source directory %s does not exist or is unreadable.\n"), fname ,SourceDir);
  else

   if MSDOS then
     Rep=ls(fullfile(SourceDir,SourceFile))
   else
     Rep=ls(Src)
   end

    if (Rep==[]) then
// Check the source file existes
      noErrorOccurred=0;
      ErrorMessage=msprintf(gettext("%s: Source file %s does not exist.\n"),fname,Src);
    end
// Now checking destination directory...
    Rep=isdir(DestDir);
    if (Rep==%F) then
      noErrorOccurred=0;
      ErrorMessage=msprintf(gettext("%s: Destination directory %s does not exist or is unreadable.\n"),fname,DestDir);
    end
  end

  if (noErrorOccurred==1) then
    if MSDOS then
      cmd='copy '+Src+' '+Dest;
	    batchlog = ' >'+ TMPDIR+'\copyfile.out' +' 2>'+TMPDIR+'\copyfile.err';
    else
      cmd='cp '+Src+' '+Dest;
      batchlog = ' >'+ TMPDIR+'/copyfile.out' +' 2>'+TMPDIR+'/copyfile.err';
    end
    cmdline =cmd+batchlog;
	// TODO: Rewrite this stuff
    status=unix(cmdline);
    if (status~=0) then
      if MSDOS then
	      msg='Error : '+mgetl(TMPDIR+'\copyfile.err');
	      msg=msg+' '+mgetl(TMPDIR+'\copyfile.out');
      else
	      msg='Error : '+mgetl(TMPDIR+'/copyfile.err');
	      msg=msg+' '+mgetl(TMPDIR+'/copyfile.out');
      end
      status=0;
    else
      msg='';
      status=1;
    end

    if (Writable==%T) & (status==1) then
      if MSDOS then
	      cmd='attrib -r '+ Dest;
	      batchlog = ' >'+ TMPDIR+'\attrib.out' +' 2>'+TMPDIR+'\attrib.err';
      else
	      cmd='chmod +w '+ Dest;
	      batchlog = ' >'+ TMPDIR+'\attrib.out' +' 2>'+TMPDIR+'\attrib.err';
      end
      cmdline =cmd+batchlog;
      status=unix(cmdline);
      if (status~=0) then
	      if MSDOS then
	        msg='Error : '+mgetl(TMPDIR+'\attrib.err');
	        msg=msg+' '+mgetl(TMPDIR+'\attrib.out');
	      else
	        msg='Error : '+mgetl(TMPDIR+'/attrib.err');
	        msg=msg+' '+mgetl(TMPDIR+'/attrib.out');
	      end
      end
    end
  else
    msg=ErrorMessage+msprintf(gettext("%s: Cannot copy file %s to %s.\n"),fname,Src,Dest);
  end

endfunction
//------------------------------------------------------------------------
function [Directory,Filename]=GetDirFile(OrigFile)
  [path,fname,ext]=fileparts(OrigFile);
  if (path=='') then
    Directory=pwd();
    Filename=OrigFile;
  else
    Directory=path;
    Filename=fname+ext;
  end
endfunction
//------------------------------------------------------------------------
