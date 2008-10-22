function wdm=do_zoomin(scs_m)
// Copyright INRIA
  wpar=scs_m.props;
  wdm=wpar.wpar
  if size(wdm,'*')<6 then wdm(3)=0;wdm(4)=0;wdm(5)=wdm(1);wdm(6)=wdm(2);end

  wdm(5)=wdm(5)*.9;wdm(6)=wdm(6)*.9;

endfunction
