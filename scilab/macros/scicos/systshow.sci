function win=systshow(scs_m,win)
[lhs,rhs]=argn(0)
if rhs<2 then win=xget('window'),end
xset('window',win);xbasc()
wpar=scs_m(1)
wsiz=wpar(1)
options=wpar(7)

// Copyright INRIA
Xshift=wsiz(3)
Yshift=wsiz(4)

xset('wdim',wsiz(1),wsiz(2))
[frect1,frect]=xgetech()
xsetech([-1 -1 8 8]/6,[Xshift,Yshift ,Xshift+wsiz(1),Yshift+wsiz(2)])

drawobjs(scs_m)
nx=size(scs_m)
for k=2:nx
  o=scs_m(k)
  if o(1)=='Block' then
    model=o(3)
    if model(1)=='super' then
      win=win+1
      win=systshow(model(8),win)
    end
  end
end


