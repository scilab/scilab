function [x,y,typ]=SLIDER_f(job,arg1,arg2)
// Copyright Enpc
x=[];y=[];typ=[]
select job
case 'plot' then
  ipar=arg1(3)(9)
  dpar=arg1(3)(8)
  standard_draw(arg1)
case 'getinputs' then
    [x,y,typ]=standard_inputs(arg1)
case 'getoutputs' then
  x=[];y=[];typ=[];
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1(2);label=graphics(4)
  model=arg1(3);
  while %t do
    [ok,min_r,max_r,sl_type,color,herit,label]=getvalue(..
	'Set  parameters',..
	['Min range';
	 'Max range';
	 'Type';
	 'Color';
	 'Block inherits (1) or not (0)'],..
	 list('vec',1,'vec',1,'vec',1,'vec',1,'vec',1),label)
    if ~ok then break,end //user cancel modification
    mess=[]
    if max_r <= min_r then
      mess=[mess;'min range < max range ';' ']
      ok=%f
    end
    if ~ok then
      message(['Some specified values are inconsistent:';
	' ';mess]);
    end
    if ~or(herit==[0 1]) then
      mess=[mess;'Accept inherited values are 0 and 1';' ']
      ok=%f
    end
     if ~ok then
      message(['Some specified values are inconsistent:';
	  ' ';mess])
    end
    if ok then
      [model,graphics,ok]=check_io(model,graphics,-1,[],ones(1-herit,1),[])
    end
    if ok then
      ipar=[sl_type;color;xget('window')];
      dpar=[min_r;max_r];
      model(9)=ipar;
      model(8)=dpar;
      model(4)=ones(1-herit,1)
      graphics(4)=label;
      x(2)=graphics;x(3)=model
      break
    end
  end
case 'define' then
  min_r= 0
  max_r= 1
  color= 1
  sl_type = 1
  label=[string(min_r);
      string(max_r);
      string(sl_type);
      string(color);
      string(0)]
  ipar=[sl_type;color;0]
  dpar=[min_r;max_r]
  model=list('slider',1,[],1,[],[],[0;-1;1;2;3;4],dpar,ipar,'d',[],[%t %f],' ',list())
  gr_i=[' ']
  x=standard_define([3 1],model,label,gr_i)
end

