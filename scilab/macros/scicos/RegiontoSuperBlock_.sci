function RegiontoSuperBlock_()
    Cmenu='Open/Set'
    xinfo(' Click, drag region and click (left to fix, right to cancel)')
    disablemenus()
    [%pt,scs_m]=do_region2block(%pt,scs_m)
    enablemenus()
endfunction
