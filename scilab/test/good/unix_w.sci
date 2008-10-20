function unix_w(cmd)
//unix_w - shell command execution results redirected in main scilab window
//%Syntax
// unix_w(cmd)
//%Parameters
// cmd - a character string
//%Description
// cmd instruction (sh syntax) is passed to shell, the standard output 
// is redirected  to main scilab window
//%Examples
// unix_w("ls")
//%See also
// host unix_x unix_s unix_g
//!
// Copyright INRIA
// Modified by Allan CORNET
  if prod(size(cmd))<>1 then   error(55,1),end
  
  if MSDOS then 
    [rep,stat]=dos(cmd,'-echo');
    if (~stat) then
      error('unix_w: error during ``'+cmd+''''' execution')
    end
  else 
     tmp=TMPDIR+'/unix.out';
     cmd1='('+cmd+')>'+ tmp +' 2>'+TMPDIR+'/unix.err;';
     stat=host(cmd1);
  
     select stat
     case 0 then
       write(%io(2),read(tmp,-1,1,'(a)'))
     case -1 then // host failed
       error(85)
     else
	msg=read(TMPDIR+'/unix.err',-1,1,'(a)')
	error('unix_w: '+msg(1))
     end
     host('rm -f '+tmp);
  end 
endfunction
