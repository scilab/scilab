function SmartMove_()
    Cmenu='Open/Set'
    xinfo('Click object to move, drag and click (left to fix, right to cancel)')
    [%pt,scs_m]=do_move(%pt,scs_m)
    xinfo(' ')

