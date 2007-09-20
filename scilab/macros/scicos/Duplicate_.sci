function Duplicate_()
//** INRIA
//**
//** Comments by Simone Mannori 
//**

  Cmenu=[]; // Cmenu Quit can come out

  [scs_m,needcompile,Select] = do_duplicate(%pt, scs_m, needcompile,Select) ;
  
  %pt=[];

endfunction
