function unix_x(cmd)
//unix_x - shell command execution, results redirected in an xless window
//%Syntax
// unix_x(cmd)
//%Parameters
// cmd - a character string
//%Description
// cmd instruction is passed to shell, the standard output is redirected 
// to  a background xless window
//%Examples
// unix_x("ls")
//%See also
// host unix_g unix_s
//!
// Copyright INRIA
if prod(size(cmd))<>1 then   error(55,1),end

if MSDOS then 
  tmp=strsubst(TMPDIR,'/','\')+'\unix.out';
  cmd1= cmd + ' > '+ tmp;
else 
  tmp=TMPDIR+'/unix.out';
  cmd1='('+cmd+')>'+ tmp +' 2>'+TMPDIR+'/unix.err;';
end 
stat=host(cmd1);
select stat
case 0 then
  if MSDOS then 
  	host(""""+strsubst(SCI,'/','\')+'\bin\xless.exe"" '+ tmp);
  else 
  	host('$SCI/bin/xless '+tmp+' & 2>/dev/null;')
  end
case -1 then // host failed
  error(85)
else //sh failed
  if MSDOS then 
	error('unix_x: shell error');
  else 
	  msg=read(TMPDIR+'/unix.err',-1,1,'(a)')
	  error('unix_x: '+msg(1))
  end 
end
// do not delete file because it is possible xless has not yet been
// launched. CLG
//if MSDOS then
//  host('del '+tmp);
//else
//  host('rm -f '+tmp);
//end
