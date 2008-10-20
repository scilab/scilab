function win=systshow(scs_m,win)
// Copyright INRIA
  [lhs,rhs]=argn(0)
  if rhs<2 then win=xget('window'),end
  xset('window',win);xbasc()
  wpar=scs_m.props

  wsiz=wpar.wpar
  options=wpar.options


  Xshift=wsiz(3)
  Yshift=wsiz(4)

  xset('wdim',wsiz(1),wsiz(2))
  [frect1,frect]=xgetech()
  xsetech([-1 -1 8 8]/6,[Xshift,Yshift ,Xshift+wsiz(1),Yshift+wsiz(2)])

  drawobjs(scs_m)

  for k=1:lstsize(scs_m.objs)
    o=scs_m.objs(k)
    if typeof(o)=='Block' then
      model=o.model
      if model.sim=='super' then
	win=win+1
	win=systshow(model.rpar,win)
      end
    end
  end
endfunction
