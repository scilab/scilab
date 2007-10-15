function Duplicate_()
// Copyright INRIA

  Cmenu=[]; // Cmenu Quit can come out

  [scs_m,needcompile,Select] = do_duplicate(%pt, scs_m, needcompile,Select) ;
  
  %pt=[];

endfunction
