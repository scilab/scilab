function [x,y,typ]=ZCROSS_f(job,arg1,arg2)
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
  model=arg1(3);
  while %t do
    [ok,in,label]=getvalue(['Set Zero-Crossing parameters';..
	                    'All surfaces must cross together'],..
			    'Input size',list('vec',1),label)
    if ~ok then break,end
    in=int(in)
    if in<=0 then
      message('Block must have at least one input')
    else
      kk=0
      for jj=1:in
	kk=kk+2^(in+jj-1)
      end
      model(8)=[-ones(kk,1);zeros(2^(2*in)-kk,1)]
      graphics(4)=label
      model(2)=in
      model(11)=-1 //compatibility
      x(2)=graphics;x(3)=model
      break
    end
  end
case 'define' then
  rpar=[-1;-1;0;0]
  in=1
  model=list('zcross',in,[],[],1,[],[],rpar,[],'z',-1,[%t %f],' ',list())
  label=strcat(sci2exp(in))
  gr_i=['xstringb(orig(1),orig(2),''Zcross'',sz(1),sz(2),''fill'');']
  x=standard_define([2 2],model,label,gr_i)
end




