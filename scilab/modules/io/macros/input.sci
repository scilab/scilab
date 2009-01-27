// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) XXXX-2008 - INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [x]=input(msg,flag)

	if type(msg) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"),"input",1));
	end
	
	if size(msg,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: A string expected.\n"),"input",1));
	end

  fmt="%["+ascii(32)+"-"+ascii(254)+"]";// a tricky way to get all ascii codes  sequences
  n=size(msg,"*")
  for k=1:n-1
    mprintf(msg(k)+"\n")
  end
  if argn(2)==2 then
    
    // Manage \n in last msg entry
    t = sprintf(msg(n) + "\n");
    tsize=size(t,"*")
    for k=1:tsize-1
      mprintf(t(k)+"\n")
    end
    
    prompt(t(tsize));
    
    x=mscanf(fmt) 
  else
    while %t
      
      // Manage \n in last msg entry
      t = sprintf(msg(n) + "\n");
      tsize=size(t,"*")
      for k=1:tsize-1
	mprintf(t(k)+"\n")
      end
      
      prompt(t(tsize));

      x=stripblanks(mscanf(fmt))
      if length(x)==0 then x="[]",end
      ierr=execstr("x="+x,"errcatch")
      if ierr==0 then break,end
      mprintf(strcat(lasterror(),"\n"))
    end
  end
endfunction
