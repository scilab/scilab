function [model,graphics,ok]=check_io(model,graphics,in,out,clkin,clkout,in_implicit,out_implicit)
// Copyright INRIA
// check_io first check if given number of ports agree with block connection
// and then changes block structure
//%parameters
// model   : initial and resulting block model structure
// graphics: initial and resulting block graphics structure
// in      : new input ports sizes
// nout    : new output ports sizes
// nclkin  : new event input ports sizes
// nclkout : new event output ports sizes
// in_impl : index of implicit input ports, the other ports are supposed to be explicit
// out_impl: index of implicit output ports , the other ports are supposed to be explicit
// ok      : error flag
//           ok==%f : some of input parameters are incorrects or
//                    attempt to add/delete ports when some are connected
//           ok==%t  : changes of block structure has been performed
//!
// Copyright INRIA
  if argn(2)<=6 then in_implicit=[],out_implicit=[],end
// check_io first check if given number of ports agree with block connection
  in=int(in(:));nin=size(in,1)

  out=int(out(:));nout=size(out,1);

  clkin=int(clkin(:));nclkin=size(clkin,1);
  if nclkin>0 then
    if mini(clkin)<1 then
      message('Event input ports sizes must be positive')
      ok=%f
      return
    end
  end


  clkout=int(clkout(:));nclkout=size(clkout,1);
  if nclkout>0 then
    if mini(clkout)<1 then
      message('Event output ports sizes must be positive')
      ok=%f
      return
    end
  end


  [label,ip1,op1,cip1,cop1]=(graphics.exprs,graphics.pin,graphics.pout,..
					    graphics.pein,graphics.peout)

  [in1,out1,clkin1,clkout1]=(model.in,model.out,model.evtin,model.evtout)

  n1=size(in1(:,1),'*');n=size(in(:,1),'*')
  if n1>n then
    if or(ip1(n+1:$)>0) then
      message('Connected ports cannot be suppressed')
      ok=%f
      return
    end
    ip1=ip1(1:n)
  else
    ip1=[ip1;zeros(n-n1,1)]
  end

  n1=size(out1,'*');n=size(out,'*')
  if n1>n then
    if or(op1(n+1:$)>0) then
      message('Connected ports cannot be suppressed')
      ok=%f
      return
    end
    op1=op1(1:n)
  else
    op1=[op1;zeros(n-n1,1)]
  end

  n1=size(clkin1,'*');n=size(clkin,'*')
  if n1>n then
    if or(cip1(n+1:$)>0) then
      message('Connected ports cannot be suppressed')
      ok=%f
      return
    end
    cip1=cip1(1:n)
  else
    cip1=[cip1;zeros(n-n1,1)];
  end

  n1=size(clkout1,'*');n=size(clkout,'*')
  if n1>n then
    if or(cop1(n+1:$)>0) then
      message('Connected ports cannot be suppressed')
      ok=%f
      return
    end
    cop1=cop1(1:n);
  else
    cop1=[cop1;zeros(n-n1,1)];
  end

  I='E';


  if ip1<>[] then
    in_impl=I(ones(ip1)); in_impl(in_implicit)='I';
    kk=size(graphics.in_implicit,'*')
    k=find(ip1<>0) //connected links
    k=k(find(k<=kk));
    if or(graphics.in_implicit(k)<>in_impl(k)) then
      message('Connected ports types cannot be changed')
      ok=%f
      return
    end
  else
    in_impl=[]
  end


  if op1<>[] then
    out_impl=I(ones(op1));  out_impl(out_implicit)='I';
    kk=size(graphics.out_implicit,'*')
    k=find(op1<>0) //connected links
    k=k(find(k<=kk));
    if or(graphics.out_implicit(k)<>out_impl(k)) then
      message('Connected ports types cannot be changed')
      ok=%f
      return
    end
  else
    out_impl=[]
  end
  
  ok=%t

  graphics.pin=ip1
  graphics.pout=op1
  graphics.pein=cip1
  graphics.peout=cop1
  graphics.in_implicit=in_impl
  graphics.out_implicit=out_impl
  
  if size(in1,2)<=1 then
    model.in=in
  elseif size(in1,2)==2 then //This appears not to be useful, retained if case of?
    disp("check_io : unexpected case")
    model.in=[in,2*ones(in)];
  end
  
  if size(out1,2)<=1 then
    model.out=out
  elseif size(out1,2)==2 then //This appears not to be useful, retained if case of?
    disp("check_io : unexpected case")
    model.out=[out,2*ones(out)];
  end
  model.evtin=clkin
  model.evtout=clkout


endfunction

