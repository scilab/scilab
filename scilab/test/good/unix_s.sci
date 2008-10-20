function unix_s(cmd)
//unix_s - silent shell command execution
//%Syntax
// unix_s(cmd)
//%Parameters
// cmd - a character string
//%Description
// cmd instruction (sh syntax) is passed to shell, the standard output 
// is redirected  to  /dev/null
//%Examples
// unix_s("\rm -f foo")
//%See also
// host unix_g unix_x
//!
// Copyright INRIA
// Modified by Allan CORNET

  if prod(size(cmd))<>1 then   error(55,1),end

  if MSDOS then 
    [rep,stat]=dos(cmd);
    if (~stat) then
      for i=1:size(rep,'*') do write(%io(2),'   '+rep(i));end
      error('unix_s: error during ``'+cmd+''''' execution')
    end
  else
     cmd1='('+cmd+')>/dev/null 2>'+TMPDIR+'/unix.err;';
     stat=host(cmd1);
     select stat
        case 0 then
        case -1 then // host failed
           error(85)
        else //sh failed
           msg=read(TMPDIR+'/unix.err',-1,1,'(a)');
           for i=1:size(msg,'*') do write(%io(2),'   '+msg(i));end
           error('unix_s: error during ``'+cmd+''''' execution')
        end
     end 
endfunction
