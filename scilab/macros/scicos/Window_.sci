function Window_()
// Copyright INRIA
    Cmenu=[]
    disablemenus()

    params=scs_m.props;
    params=do_window(params)
    edited=or(params<>scs_m.props)
    
    if or(scs_m.props.wpar<>params.wpar) then
      xset('alufunction',3);xbasc();xselect();xset('alufunction',6);
      window_set_size()

      scs_m.props.wpar=params.wpar
      %wdm=scs_m.props.wpar
      %wdm(5:6)=(params.wpar(1:2)./scs_m.props.wpar(1:2)).*%wdm(5:6)
      scs_m.props.wpar(5)=%wdm(5);scs_m.props.wpar(6)=%wdm(6);

      drawobjs(scs_m),
      if pixmap then xset('wshow'),end
    end
    enablemenus()
endfunction
