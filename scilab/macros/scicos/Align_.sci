function Align_()
// Copyright INRIA
//
  Cmenu=[];
  %pt1=%pt;%pt=[];
  scs_m_save=scs_m;nc_save=needcompile;
  [scs_m]=prt_align(%pt1,scs_m)
endfunction
