function tic()
// Copyright INRIA
// tic() start a stopwatch timer.
// The sequence of commands tic(), operation, toc()
// prints the number of milliseconds required for the operation.
// V.C. & Allan CORNET 2004

global TICTOC;

TICTOC=getdate();

endfunction
