function Window_()
    Cmenu='Open/Set'
    disablemenus()

    %wpar=scs_m(1);%wdx=%wpar(1);
    %wpar=do_window(%wpar)
    edited=or(%wpar<>scs_m(1))
    if or(%wdx<>%wpar(1)) then
      xset('alufunction',3);xbasc();xselect();xset('alufunction',6);
      %wd=%wpar(1)
      
      window_set_size()

	%wdm12=%wd
      
      scs_m(1)(1)(1)=%wdm12(1);scs_m(1)(1)(2)=%wdm12(2);
      %wdm=scs_m(1)(1)
      %wdm(5:6)=(%wd(1:2)./%wdx(1:2)).*%wdm(5:6)
      scs_m(1)(1)(5)=%wdm(5);scs_m(1)(1)(6)=%wdm(6);
      

      drawobjs(scs_m),
      if pixmap then xset('wshow'),end
    end
    enablemenus()

