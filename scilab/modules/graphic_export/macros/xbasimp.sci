// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function xbasimp(win_num,filen,printer)
// This function will send the recorded graphics 
// to a Postscript file 
//!

warnobsolete("toprint", "5.1");
warning(gettext("""xbasimp"" has been replaced by ""toprint"" for printing and by ""xs2ps"" for the file export"));

[lhs,rhs]=argn(0);

// conver tfigure numbers into a row vector
n=size(win_num,'*')
win_num=matrix(win_num,1,n);

// check if a file name has been specified
if rhs == 1,
  filen=TMPDIR+'/scilab.ps';
end

// create postcript files
fname=' ';
for i=1:n,
  fnamel = filen + '.' + string(win_num(i));
  // export to a file
  xs2ps(win_num(i),fnamel);
end

// print figures if needed
// if rhs == 3 we should select the printer to use
if (rhs == 1 | rhs == 3) then
  for i = 1:n,
    toprint(win_num(i));
  end
end

endfunction

