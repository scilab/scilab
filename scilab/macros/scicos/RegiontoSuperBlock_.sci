function RegiontoSuperBlock_()
    Cmenu='Open/Set'
    xinfo(' Click, drag region and click (left to fix, right to cancel)')
    disablemenus()
    scs_m=do_region2block(scs_m)
    enablemenus()

