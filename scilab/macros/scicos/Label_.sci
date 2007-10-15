function Label_()
// Copyright INRIA
    scs_m_save=scs_m;nc_save=needcompile;enable_undo=%t
    [%mod,scs_m]=do_label(%pt,scs_m)
    edited=edited|%mod
    if %mod then needcompile=max(2,needcompile),end
    Cmenu=[];%pt=[];
endfunction
