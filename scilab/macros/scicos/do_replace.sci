function [scs_m,needcompile]=do_replace(scs_m,needcompile)
// Copyright INRIA
while %t
  // get replacement block
  [btn,%pt,win,Cmenu]=cosclick()
  if Cmenu<>[] then
    Cmenu=resume(Cmenu)
  end
  xc=%pt(1);yc=%pt(2);
  kc=find(win==windows(:,2))
  if kc==[] then
    message('This window is not an active palette')
    k=[];break
  elseif windows(kc,1)<0 then //click dans une palette
    kpal=-windows(kc,1)
    palette=palettes(kpal)
    k=getblock(palette,[xc;yc])
    if k<>[] then o=palette.objs(k),break,end
  elseif win==curwin then //click dans la fenetre courante
    k=getblock(scs_m,[xc;yc])
    if k<>[] then
      o=scs_m.objs(k);graphics=o.graphics
     graphics.pin=0*graphics.pin
     graphics.pout=0*graphics.pout
     graphics.pein=0*graphics.pein
     graphics.peout=0*graphics.peout
      o.graphics=graphics
      break,
    end
  else
    message('This window is not an active palette')
    k=[];break
  end
end
if k==[] then return,end
// get block to replace
xset('window',curwin);
while %t do
  [btn,%pt,win,Cmenu]=cosclick()
  if Cmenu<>[] then
    Cmenu=resume(Cmenu)
  end
  xc=%pt(1);yc=%pt(2);
  k_n=getobj(scs_m,[xc;yc])
  if k_n<>[] then
    o_n=scs_m.objs(k_n)
    if typeof(o_n)=='Block' then  break,end
  end
end
o.graphics.flip=o_n.graphics.flip // set same flip position
[ip,op,cip,cop]=(o.graphics.pin,o.graphics.pout,o.graphics.pein,..
o.graphics.peout)
[in,out,clkin,clkout]=(o.model.in,o.model.out,o.model.evtin,o.model.evtout)

nin=size(in,'*')
nout=size(out,'*')
nclkin=size(clkin,'*')
nclkout=size(clkout,'*')

[ip_n,op_n,cip_n,cop_n]=(o_n.graphics.pin,o_n.graphics.pout,..
o_n.graphics.pein,o_n.graphics.peout)
[in_n,out_n,clkin_n,clkout_n]=(o_n.model.in,o_n.model.out,..
o_n.model.evtin,o_n.model.evtout)
nin_n=size(in_n,'*')
nout_n=size(out_n,'*')
nclkin_n=size(clkin_n,'*')
nclkout_n=size(clkout_n,'*')


[ox,oy]=getorigin(o)
[ox_n,oy_n]=getorigin(o_n)


[xx,yy,t]=getinputs(o);xx=xx-ox;yy=yy-oy
[x_n,y_n,t_n]=getinputs(o_n);x_n=x_n-ox_n;y_n=y_n-oy_n
//Check inputs
wasconnected=%f
if ip_n<>[] then if find(ip_n>0)<>[] then wasconnected=%t,end,end
if wasconnected then
  if nin<>nin_n then
    message(['It is not yet possible to replace a block with'
	'connected inputs with an other block with '
	'different number of inputs'])
    ok=%f
    return
  elseif norm(xx(1:nin)-x_n(1:nin),1)>0.1|norm(yy(1:nin)-y_n(1:nin),1)>0.1 then
    message(['It is not yet possible to replace a block with'
	'connected inputs with an other block with inputs not '
	'relatively located in the same position'])
    ok=%f
    return
  else
    ip=ip_n
  end
end
//Check clock inputs
wasconnected=%f
if cip_n<>[] then if find(cip_n>0)<>[] then wasconnected=%t, end, end,
if wasconnected then
  if nclkin<>nclkin_n then
    message(['It is not yet possible to replace a block with'
	'connected clock inputs with an other block with '
	'different number of clock inputs'])
    ok=%f
    return
  elseif norm(xx(t==-1)-x_n(t_n==-1),1)>0.1|norm(yy(t==-1)-y_n(t_n==-1),1)>0.1 then
    message(['It is not yet possible to replace a block with'
	'connected clock inputs with an other block with clock inputs '
	'not relatively located in the same position'])
    ok=%f
    return
  else
    cip=cip_n
  end
end


[xx,yy,t]=getoutputs(o);xx=xx-ox;yy=yy-oy
[x_n,y_n,t_n]=getoutputs(o_n);x_n=x_n-ox_n;y_n=y_n-oy_n
//Check outputs
wasconnected=%f
if op_n<>[] then if find(op_n>0)<>[] then wasconnected=%t, end, end,
if wasconnected then
  if nout<>nout_n then
    message(['It is not yet possible to replace a block with'
	'connected outputs with an other block with '
	'different number of outputs'])
    ok=%f
    return
  elseif norm(xx(1:nout)-x_n(1:nout),1)>0.1|norm(yy(1:nout)-y_n(1:nout),1)>0.1 then
    message(['It is not yet possible to replace a block with'
	'connected outputs with an other block with outputs not '
	'relatively located in the same position'])
    ok=%f
    return
  else
    op=op_n
  end
end
//Check clock outputs
wasconnected=%f
if cop_n<>[] then if find(cop_n>0)<>[] then wasconnected=%t, end, end,
if wasconnected then
  if nclkout<>nclkout_n then
    message(['It is not yet possible to replace a block with'
	'connected clock outputs with an other block with '
	'different number of clock outputs'])
    ok=%f
    return
  elseif norm(xx(t==-1)-x_n(t_n==-1),1)>0.1|norm(yy(t==-1)-y_n(t_n==-1),1)>0.1 then
    message(['It is not yet possible to replace a block with'
	'connected clock outputs with an other block with clock outputs '
	'not relatively located in the same position'])
    ok=%f
    return
  else
    cop=cop_n
  end
end

o.graphics.orig=[ox_n,oy_n]
o.graphics.pin=ip
o.graphics.pout=op
o.graphics.pein=cip
o.graphics.peout=cop
drawobj(o_n)
drawobj(o)
scs_m.objs(k_n)=o
if pixmap then xset('wshow'),end
scs_m_save=scs_m,nc_save=needcompile
needcompile=4
[scs_m_save,nc_save,enable_undo,edited]=resume(scs_m_save,nc_save,%t,%t)
endfunction
