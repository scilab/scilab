function tictoctime=toc()
// Copyright INRIA
// toc() read the stopwatch timer.
// toc(), by itself, prints the elapsed time (in milliseconds) since tic() was used.
// V.C. & Allan CORNET 2004

global TICTOC;

if isempty(TICTOC) then
  error("You must call tic() before calling toc().")
end

tictoctime=etime(getdate(),TICTOC);

endfunction
