function Resize_()
  scs_m_save=scs_m;nc_save=needcompile;enable_undo=%t
  [%pt,scs_m]=do_resize(%pt,scs_m)
  edited=%t
  Cmenu=[];%pt=[];
endfunction
