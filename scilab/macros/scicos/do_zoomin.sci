function wdm=do_zoomin(scs_m)
// Copyright INRIA
wpar=scs_m(1);wdm=wpar(1)
if size(wdm,'*')<6 then wdm(3)=0;wdm(4)=0;wdm(5)=wdm(1);wdm(6)=wdm(2);end

  wdm(5)=wdm(5)*.9;wdm(6)=wdm(6)*.9;

//dd=wdm(3:6)*.9;
//wdm(3:6)=dd;
