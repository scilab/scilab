function scs_show(scs_m,win)
// Copyright INRIA
  oldwin=xget('window')
  xset('window',win);xbasc()
  if ~set_cmap(scs_m.props.options('Cmap')) then // add colors if required
    scs_m.props.options('3D')(1)=%f //disable 3D block shape
  end
  wsiz=scs_m.props.wpar
  options=scs_m.props.options
  set_background()

  xset('wdim',wsiz(1),wsiz(2))
  rect=dig_bound(scs_m)

  wa=(rect(3)-rect(1))
  ha=(rect(4)-rect(2))
  aa=wa/ha
  rr=wsiz(1)/wsiz(2)

  if aa<rr then 
    wa2=wa*rr/aa;rect(1)=rect(1)-(wa2-wa)/2;rect(3)=rect(1)+wa2
  else
    ha2=ha*aa/rr;rect(2)=rect(2)-(ha2-ha)/2;rect(4)=rect(2)+ha2
  end


  dxx=(rect(3)-rect(1))/20;
  dyy=(rect(4)-rect(2))/20;
  rect(1)=rect(1)-dxx;rect(3)=rect(3)+dxx;
  rect(2)=rect(2)-dyy;rect(4)=rect(4)+dyy;
  xsetech([-1 -1 8 8]/6,rect)

  xset('alufunction',3)
  scs_m.props.title(1)='Scilab Graphics of '+scs_m.props.title(1)
  drawobjs(scs_m),
  //***********************************************************

  if pixmap then xset('wshow'),end
endfunction
