function [ok,sbloc]=adjust_s_ports(sbloc)
// Copyright INRIA

  graphics=sbloc.graphics;
  model=sbloc.model;

  nin=0;nout=0;nclkin=0;nclkout=0;
  in=[],out=[],cin=[],cout=[]
  inp=[],outp=[],cinp=[],coutp=[]


  scs_m=model.rpar
  for k=1:lstsize(scs_m.objs)
    o=scs_m.objs(k)
    if typeof(o)=='Block' then
      modelb=o.model
      select o.gui
	case 'IN_f' then
	nin=nin+1
	inp=[inp o.model.ipar]
	in=[in;o.model.out]
	case 'OUT_f' then
	nout=nout+1
	outp=[outp o.model.ipar]
	out=[out;o.model.in]
	case 'CLKIN_f' then
	nclkin=nclkin+1
	cinp=[cinp o.model.ipar]
	cin=[cin;o.model.evtout]
	case 'CLKINV_f' then
	nclkin=nclkin+1
	cinp=[cinp o.model.ipar]
	cin=[cin;o.model.evtout]  
	case 'CLKOUT_f' then
	nclkout=nclkout+1
	coutp=[coutp o.model.ipar]
	cout=[cout;o.model.evtin]
	case 'CLKOUTV_f' then
	nclkout=nclkout+1
	coutp=[coutp o.model.ipar]
	cout=[cout;o.model.evtin]         
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
    model.rpar=scs_m
    sbloc.model=model;sbloc.graphics=graphics;
  end

endfunction
