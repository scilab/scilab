// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) XXXX-2008 - INRIA
// Copyright (C) XXXX-2008 - INRIA - Allan CORNET

// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function varargout = unix_g(cmd)
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
	
	[lhs,rhs] = argn(0);
	
	if rhs <> 1 then
		error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"unix_g",1));
	end
	
	if type(cmd) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"unix_g",1));
	end
	
	if size(cmd,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A string expected.\n"),"unix_g",1));
	end
	
	if lhs > 3 then
		error(msprintf(gettext("%s: Wrong number of output argument(s).\n"),"unix_g"));
	end

  // initialize variables
  stderr = emptystr();
  stat = 1;
  rep = emptystr();

  if getos() == 'Windows' then 
    [rep,stat] = dos(cmd);
    if (stat == %t) then
      stat = 0;
    else
      if lhs == 3 then
        stderr = rep;
      else
        for i=1:size(rep,'*') do write(%io(2),'   '+rep(i));end
      end
      
      stat = 1;
      rep = emptystr();
    end	
  else 
     tmp = TMPDIR+'/unix.out';
     cmd1 = '('+cmd+')>'+ tmp +' 2>'+TMPDIR+'/unix.err;';
     stat = host(cmd1);
     
     select stat
     
       case 0 then
         rep = mgetl(tmp);
         if (size(rep,'*')==0) | (length(rep)==0) then
           rep = [];
	       end;
       
       case -1 then
         // host failed
         if lhs == 3 then
           stderr = msprintf(gettext("%s: The system interpreter does not answer..."),"unix_g");
         else
           disp(msprintf(gettext("%s: The system interpreter does not answer..."),"unix_g"));
         end
         rep = emptystr();
       else
         msg = mgetl(TMPDIR+'/unix.err');
         if lhs == 3 then
           stderr = msg;
         else
           disp(msg(1));
         end
         rep = emptystr();
       end
     
     mdelete(tmp);
  end
  
  // output arguments
  
  varargout(1) = rep;
  
  if lhs >= 2 then
    varargout(2) = stat;
  end
  
  if lhs >= 3 then
    varargout(3) = stderr;
  end
  
endfunction
