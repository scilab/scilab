function wdm=do_view(scs_m)
// Copyright INRIA
  wpar=scs_m.props;wdm=wpar.wpar
  if size(wdm,'*')<6 then wdm(3)=0;wdm(4)=0;wdm(5)=wdm(1);wdm(6)=wdm(2);end
  Xshift=wdm(3)
  Yshift=wdm(4)
  oxc=Xshift+(wdm(5))/2
  oyc=Yshift+(wdm(6))/2
  plot2d(oxc,oyc,-1,'000')


  [btn,xc,yc]=xclick(0) //get center of new view
  Xshift=Xshift+(xc-oxc)
  Yshift=Yshift+(yc-oyc)


  wdm(3)=Xshift;wdm(4)=Yshift;
endfunction
