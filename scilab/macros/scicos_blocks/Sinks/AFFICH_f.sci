function [x,y,typ]=AFFICH_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[]
select job
case 'plot' then
  ipar=arg1(3)(9)
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
  if size(label,'*')==5 then label(6)='0';end //compatibility
  if size(label,'*')==4 then label(4)=' ';label(5)=' ';end //compatibility
  if size(model(7),'*')<>6 then model(7)=[0;-1;0;0;1;1];end //compatibility
  while %t do
    [ok,font,fontsize,color,nt,nd,herit,label]=getvalue(..
	'Set  parameters',..
	['Font number';
	 'Font size';
	 'Color';
	 'Total number of digits';
	 'Number of rational part digits';
	 'Block inherits (1) or not (0)'],..
	 list('vec',1,'vec',1,'vec',1,'vec',1,'vec',1,'vec',1),label)
    if ~ok then break,end //user cancel modification
    mess=[]

    if font<=0 then
      mess=[mess;'Font number must be positive';' ']
      ok=%f
    end
    if fontsize<=0 then
      mess=[mess;'Font size must be positive';' ']
      ok=%f
    end
    if nt<=3 then
      mess=[mess;'Total number of digits must be greater than 3';' ']
      ok=%f
    end
    if nd<0 then
      mess=[mess;'Number of rational part digits must be '
	  'greater or equal 0';' ']
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
      ipar=[font;fontsize;color;xget('window');nt;nd]
      model(9)=ipar;model(4)=ones(1-herit,1)
      graphics(4)=label;
      x(2)=graphics;x(3)=model
      break
    end
  end
case 'define' then
  font=1
  fontsize=1
  color=1
  nt=9
  nd=2
  label=[string(font);
      string(fontsize);
      string(color);
      string(nt);
      string(nd)]
  ipar=[font;fontsize;color;0;nt;nd]
  model=list('affich',1,[],1,[],[],[0;-1;0;0;1;1],[],ipar,'d',[],[%t %f],' ',list())
      
  gr_i=['xset(''font'',ipar(1),ipar(2))';
	'str='' ''+part(''0'',ones(1,ipar(5)-ipar(6)-2))+''.''+part(''0'',ones(1,ipar(6)))'
	'rr=xstringl(orig(1),orig(2),str)'
        'xstring(orig(1)+max(0,(sz(1)-rr(3))/2),orig(2)+max(0,(sz(2)-rr(4))/2),str)']
  x=standard_define([3 2],model,label,gr_i)
end

