// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - V.C , A.C
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function tictoctime=toc()

// toc() read the stopwatch timer.
// toc(), by itself, prints the elapsed time (in milliseconds) since tic() was used.


global TICTOC;

if isempty(TICTOC) then
  error(msprintf(gettext('%s: You must call tic() before calling toc().\n'),'toc'));
end

tictoctime=etime(getdate(),TICTOC);

endfunction
