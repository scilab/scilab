function [x,y,typ]=TEXT_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[];
select job
case 'plot' then //normal  position
  graphics=arg1.graphics; 
  model=arg1.model;
  if model.rpar==[] then model.rpar=graphics.exprs(1),end //compatibility
  ppat=xget('pattern')
  xset('pattern',default_color(1))
  oldfont=xget('font');  xset('font',model.ipar(1),model.ipar(2))
  xstring(graphics.orig(1),graphics.orig(2),model.rpar)
  xset('font',oldfont(1),oldfont(2))
  xset('pattern',ppat)
case 'getinputs' then
case 'getoutputs' then
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1.graphics;
  orig=graphics.orig
  exprs=graphics.exprs
  model=arg1.model
  if size(exprs,'*')==1 then exprs=[exprs;'3';'1'],end // compatibility
  while %t do
    [ok,txt,font,siz,exprs]=getvalue('Set Text block parameters',..
	['Text';'Font number';'Font size'],list('str',-1,'vec',1,'vec',1),exprs)
    if ~ok then break,end
    if font<=0|font>6 then
      message('Font number must be greater than 0 and less than 7')
      ok=%f
    end
    if siz<0 then
      message('Font size must be positive')
      ok=%f
    end
    if ok then
      graphics.exprs=exprs
      oldfont=xget('font')
      xset('font',font,siz)
      r=xstringl(0,0,exprs(1))
      xset('font',oldfont(1),oldfont(2))
      sz=r(3:4)
      graphics.sz=sz
      x.graphics=graphics;
      ipar=[font;siz]
      model.rpar=txt
      model.ipar=ipar;
      x.model=model
      break
    end
  end
case 'define' then
  font=2
  siz=1

  model=scicos_model()
  model.sim='text'
  model.rpar='Text'
  model.ipar=[font;siz]

  exprs=['Text';string(font);string(siz)]
  graphics=scicos_graphics()
  graphics.orig=[0,0];
  graphics.sz=[2 1];
  graphics.exprs=exprs


  x=mlist(['Text','graphics','model','void','gui'],graphics,model,' ','TEXT_f')

end
endfunction
