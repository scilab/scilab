function [x,y,typ]=GENERAL_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[];
select job
case 'plot' then
  standard_draw(arg1)
case 'getinputs' then
  [x,y,typ]=standard_inputs(arg1)
case 'getoutputs' then
  [x,y,typ]=standard_outputs(arg1)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1(2);label=graphics(4)
  model=arg1(3);rpar=model(8)
  in=model(2);out=model(5)
  nin=sum(in)
  nout=sum(out)
    [ok,in,out,label]=getvalue('Set General Zero-Crossing parameters',..
      ['Input size';
       'Number of event output'],..
      list('vec',1,'vec',1),label)
  if ok then
    [model,graphics,ok]=check_io(model,graphics,in,[],[],ones(out,1))
    if ok then
      nout1=out
      nin1=in
      if nout==nout1 & nin==nin1 then
	rp=matrix(rpar,nout,2^(2*nin));
      else
	rp=-1*ones(nout1,2^(2*nin1))
      end
      n=size(rp,2)/2
      result=x_mdialog('routing matrix',string(1:nout1),..
	  string(1:2^(2*nin1)),string(rp(:,:)))
      if result<>[] then
	rp(1:nout1,1:2*n)=evstr(result)
	model(8)=rp(:)
	model(11)=-ones(out,1)
	graphics(4)=label
	x(2)=graphics;x(3)=model
      end
    end
  end
case 'define' then
  rpar=[0;0;0;0]
  in=1;out=1;
  model=list('zcross',in,[],[],ones(out,1),[],[],rpar,[],'z',-ones(out,1),[%t %f],' ',list())
  label=[strcat(sci2exp(in));strcat(sci2exp(out))]
  gr_i=['xstringb(orig(1),orig(2),''GENERAL'',sz(1),sz(2),''fill'');']
  x=standard_define([3 2],model,label, gr_i)
end




