function DuplicateRegion_()
  Cmenu=[];// Cmenu Quit can come out
  [scs_m,needcompile]=do_copy_region(scs_m,needcompile,%pt);
  %pt=[];
endfunction
