function [ok,x]=adjust_s_ports(arg1)
// Copyright INRIA
graphics=arg1(2);
x=arg1
model=arg1(3);


xx=model(8)

nin=0;nout=0;nclkin=0;nclkout=0;
in=[],out=[],cin=[],cout=[]
inp=[],outp=[],cinp=[],coutp=[]
for k=2:size(xx)
  o=xx(k)
  if o(1)=='Block' then
    modelb=o(3)
    select o(5)
    case 'IN_f' then
      nin=nin+1
      inp=[inp modelb(9)]
      in=[in;modelb(3)]
    case 'OUT_f' then
      nout=nout+1
      outp=[outp modelb(9)]
      out=[out;modelb(2)]
    case 'CLKIN_f' then
      nclkin=nclkin+1
      cinp=[cinp modelb(9)]
      cin=[cin;modelb(5)]
    case 'CLKINV_f' then
      nclkin=nclkin+1
      cinp=[cinp modelb(9)]
      cin=[cin;modelb(5)]         
    case 'CLKOUT_f' then
      nclkout=nclkout+1
      coutp=[coutp modelb(9)]
      cout=[cout;modelb(4)]
    case 'CLKOUTV_f' then
      nclkout=nclkout+1
      coutp=[coutp modelb(9)]
      cout=[cout;modelb(4)]          
    end
  end
end
ok=%t
mess=[]
if nin>0 then
  [inp,k]=sort(-inp)
  if ~and(inp==-(1:nin)) then
    mess=[mess;
	'Super_block input ports must be numbered';
	'from 1 to '+string(nin);' ']
    ok=%f
  end
  in=in(k)
end
if nout>0 then
  [outp,k]=sort(-outp)
  if ~and(outp==-(1:nout)) then
    mess=[mess;
	'Super_block output ports must be numbered';
	'from 1 to '+string(nout);' ']
    ok=%f
  end
  out=out(k)
end


if nclkin>0 then
  [cinp,k]=sort(-cinp)
  if ~and(cinp==-(1:nclkin)) then
    mess=[mess;
	'Super_block event input ports must be numbered';
	'from 1 to '+string(nclkin);' ']
    ok=%f
  end
  cin=cin(k)
end
if nclkout>0 then
  [coutp,k]=sort(-coutp)
  if ~and(coutp==-(1:nclkout)) then
    mess=[mess;
	'Super_block event output ports must be numbered';
	'from 1 to '+string(nclkout);' ']
    ok=%f
  end
  cout=cout(k)
end
if ok then
  [model,graphics,ok]=check_io(model,graphics,in,out,cin,cout)
else
  message(mess)
end
if ok then
  model(8)=xx
  model(11)=[] //compatibility
  x=arg1;x(3)=model;x(2)=graphics;
end
