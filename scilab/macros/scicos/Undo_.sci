function Undo_()
    Cmenu='Open/Set'
    %pt=[]
    if enable_undo then
      disablemenus()
      scs_m=scs_m_save;needcompile=nc_save
      %wpar=scs_m(1)
      %wdm=%wpar(1)
      xbasc()
      window_set_size()
      drawobjs(scs_m),

      if pixmap then xset('wshow'),end
      enable_undo=%f
      enablemenus()
    end


