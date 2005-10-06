function RegiontoSuperBlock_()
  disablemenus()
  [%pt,scs_m]=do_region2block(%pt,scs_m)
  enablemenus()
  Cmenu=[];%pt=[];
endfunction
