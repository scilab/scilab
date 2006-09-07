//------------------------------------------------------------------------
// Allan CORNET
// INRIA 2004
//------------------------------------------------------------------------
// copy file
function [status,msg]=copyfile(varargin)
  lhs=argn(1);   
  rhs=argn(2);
  
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
  
  Status=1;
  
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
        error('Third parameters incorrect.');
      end
    break
  else
     error('Number of parameters incorrect.');
  end

  [SourceDir,SourceFile]=GetDirFile(SourceFile);
  [DestDir,DestFile]=GetDirFile(DestinationFile);

   Src = """"+fullfile(SourceDir,SourceFile)+"""";
   Dest = """"+fullfile(DestDir,DestFile)+"""";
   
   Rep=dir(SourceDir);
   if (Rep(2)==[]) then
     Status=0;
     ErrorMessage='Source directory '+SourceDir+' does not exist or is unreadable.';
   else
     Rep=dir(DestDir);
     if (Rep(2)==[]) then
       Status=0;
       ErrorMessage='Destination directory '+DestDir+' does not exist or is unreadable.';
     else
       Rep=dir(DestDir);
       if (Rep(2)==[]) then
         Status=0;
         ErrorMessage='source file '+fullfile(SourceDir,SourceFile)+' does not exist or is unreadable.';
       end
     end
   end

   if (Status==1) then
     if MSDOS then
       cmd='copy '+Src+' '+Dest;
       ver=OS_Version();
       if ver == 'Windows 98' | ver == 'Windows 95' | ver == 'Windows ME' then
         batchlog = ' >'+ TMPDIR+'\copyfile.out';
       else
         batchlog = ' >'+ TMPDIR+'\copyfile.out' +' 2>'+TMPDIR+'\copyfile.err';
       end
     else
       cmd='cp '+Src+' '+Dest;
       batchlog = ' >'+ TMPDIR+'/copyfile.out' +' 2>'+TMPDIR+'/copyfile.err';
     end
     cmdline =cmd+batchlog;
     status=unix(cmdline);
     if (status~=0) then
       if MSDOS then
         ver=OS_Version();
         if ver == 'Windows 98' | ver == 'Windows 95' | ver == 'Windows ME' then
           msg='Error :'+cmd;
         else
           msg='Error : '+mgetl(TMPDIR+'\copyfile.err');
           msg=msg+' '+mgetl(TMPDIR+'\copyfile.out');
         end
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
         ver=OS_Version();
         cmd='attrib -r '+ Dest;
         if ver == 'Windows 98' | ver == 'Windows 95' | ver == 'Windows ME' then
           batchlog = ' >'+ TMPDIR+'\attrib.out';
         else
           batchlog = ' >'+ TMPDIR+'\attrib.out' +' 2>'+TMPDIR+'\attrib.err';
         end
       else
         cmd='chmod +w '+ Dest;
         batchlog = ' >'+ TMPDIR+'\attrib.out' +' 2>'+TMPDIR+'\attrib.err';
       end
       cmdline =cmd+batchlog;
       status=unix(cmdline);
       if (status~=0) then
         if MSDOS then
           ver=OS_Version();
           if ver == 'Windows 98' | ver == 'Windows 95' then
             msg='Error :'+cmd;
           else
             msg='Error : '+mgetl(TMPDIR+'\attrib.err');
             msg=msg+' '+mgetl(TMPDIR+'\attrib.out');
           end
         else
           msg='Error : '+mgetl(TMPDIR+'/attrib.err');
           msg=msg+' '+mgetl(TMPDIR+'/attrib.out');
         end
       end
     end   
   else
   	msg=ErrorMessage+' Cannot copy file, '+Src+' to '+Dest;
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
