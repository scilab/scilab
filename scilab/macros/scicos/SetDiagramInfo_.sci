function SetDiagramInfo_()
    Cmenu='Open/Set'
    [ok,info]=do_set_info(scs_m(1)(10))
    if ok then scs_m(1)(10)=info,end
