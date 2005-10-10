function RegiontoSuperBlock_()
  disablemenus()
  %win=curwin // only in main window
  [%pt,scs_m]=do_region2block(%pt,scs_m)
  enablemenus()
  Cmenu=[];%pt=[];
endfunction
