function Resize_()
// Copyright INRIA

  scs_m_save = scs_m     ;

  nc_save = needcompile  ;

  enable_undo = %t       ;

  //** new graphic functions is now included in do_resize
  [%pt,scs_m] = do_resize(%pt,scs_m)

  edited = %t ;
  Cmenu = [] ; %pt = [];
  
endfunction
