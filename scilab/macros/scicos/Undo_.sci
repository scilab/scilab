function Undo_()
    Cmenu='Open/Set'
    %pt=[]
    if enable_undo then
      disablemenus()
      scs_m=scs_m_save;needcompile=nc_save
      %wdm=scs_m.props.wpar

      xbasc()
      window_set_size()
      drawobjs(scs_m),

      if pixmap then xset('wshow'),end
      enable_undo=%f
      enablemenus()
    end
endfunction
