// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004 - INRIA - Allan CORNET
// Copyright (C) 2008 - INRIA - Sylvestre
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


// copy file

function [status,msg]=copyfile(varargin)
lhs=argn(1);
rhs=argn(2);

fname = 'copyfile';

Writable=%F;
status=0;
msg='';

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
	error(msprintf(gettext("%s: Wrong value for input argument #%d: %s expected.\n"),fname,3,"WRITABLE"));
  end
  break
else
  error(msprintf(gettext("%s: Wrong number of input arguments.\n"),fname));
end

[DestDir,DestFile]=GetDirFile(DestinationFile);

if MSDOS then
  [SourceDir,SourceFile]=GetDirFile(SourceFile);
  
  Src = """"+fullfile(SourceDir,SourceFile)+"""";
  Dest = """"+fullfile(DestDir,DestFile)+"""";
  
  Rep=isdir(SourceDir);
  if (Rep==%F) then
	// Check the source directory is a directory
	msg=msprintf(gettext("%s: Source directory %s does not exist or is unreadable.\n"), fname ,SourceDir);
	return;
  end
  Rep=ls(fullfile(SourceDir,SourceFile))

else
  Src=SourceFile
  Dest=DestinationFile
  Rep=ls(Src)
end

if (Rep==[]) then
  // Check the source file existes
  msg=msprintf(gettext("%s: Source file %s does not exist or unable to access.\n"),fname,SourceFile);
  return;
end
// Now checking destination directory...
Rep=isdir(DestDir);
if (Rep==%F) then
  msg=msprintf(gettext("%s: Destination directory %s does not exist or is unreadable.\n"),fname,DestDir);
  return;
end


if MSDOS then
  cmd='copy '+Src+' '+Dest;
  batchlog = ' >'+ TMPDIR+'\copyfile.out' +' 2>'+TMPDIR+'\copyfile.err';
else
  // strsubst are here to "protect" the space in the path
  cmd='cp '+strsubst(Src," ","\ ")+' '+strsubst(Dest," ","\ ");
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
