function [rep,stat]=unix_g(cmd)
//unix_g - shell command execution 
//%Syntax
//rep=unix_g(cmd)
//%Parameters
// cmd - a character string
// rep - a column vector of character strings
//%Description
// cmd instruction (sh syntax) is passed to shell, the standard output 
// is redirected  to scilab variable rep.
//%Examples
// unix_g("ls")
//%See also
// host unix_x unix_s
//!
// Copyright INRIA
// Modified by Allan CORNET

  [lhs,rhs]=argn(0)
  if prod(size(cmd))<>1 then error(55,1),end

  if MSDOS then 
    [rep,stat]=dos(cmd);
    if (stat==%t) then
      stat=0;
    else
      for i=1:size(rep,'*') do write(%io(2),'   '+rep(i));end
      stat=1;
      rep='';
    end	
  else 
     tmp=TMPDIR+'/unix.out';
     cmd1='('+cmd+')>'+ tmp +' 2>'+TMPDIR+'/unix.err;';
     stat=host(cmd1);
     
     select stat
     
       case 0 then
         rep=mgetl(tmp);
         if (size(rep,'*')==0) | (length(rep)==0) then
           rep=[];
	 end;
       
       case -1 then
         // host failed
         disp('host does not answer...')
         rep=emptystr()
       else
         msg=mgetl(TMPDIR+'/unix.err')
         disp(msg(1))
         rep=emptystr()
	 
     end
     
     host('rm -f '+tmp);   
     
  end
endfunction
