function SetDiagramInfo_()
// Copyright INRIA
    Cmenu='Open/Set'
    [ok,info]=do_set_info(scs_m(1).doc)
    if ok then scs_m(1).doc=info,end
endfunction
