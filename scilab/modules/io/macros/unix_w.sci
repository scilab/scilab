// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) XXXX-2008 - INRIA
// Copyright (C) XXXX-2008 - INRIA - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


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

	[lhs,rhs] = argn(0);
	
	if rhs <> 1 then
		error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"unix_w",1));
	end
	
	if type(cmd) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"unix_w",1));
	end
	
	if size(cmd,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A string expected.\n"),"unix_w",1));
	end
	
  if MSDOS then 
    [rep,stat]=dos(cmd,'-echo');
    if (~stat) then
      error(msprintf(gettext("%s: error during ""%s"" execution"),"unix_w",cmd));
    end
  else 
     tmp=TMPDIR+'/unix.out';
     cmd1='('+cmd+')>'+ tmp +' 2>'+TMPDIR+'/unix.err;';
     stat=host(cmd1);
  
     select stat
     case 0 then
       write(%io(2),read(tmp,-1,1,'(a)'))
     case -1 then // host failed
       error(msprintf(gettext("%s: The system interpreter does not answer..."),"unix_w"))
     else
	msg=read(TMPDIR+'/unix.err',-1,1,'(a)')
	error('unix_w: '+msg(1))
     end
     host('rm -f '+tmp);
  end 
endfunction
