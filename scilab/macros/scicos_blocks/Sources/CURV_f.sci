function [x,y,typ]=CURV_f(job,arg1,arg2)

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
  x=arg1
  model=arg1(3)
  graphics=arg1(2)
  [rpar,ipar]=model(8:9)
  n=ipar(1)
  xx=rpar(1:n);yy=rpar(n+1:2*n)
  curwin=xget('window')
  win=maxi(windows(:,2))+1
  xset('window',win);xsetech([0 0 1 1])
  gc=list(rpar(2*n+1:2*n+4),ipar(2:5))
  while %t do
    [xx,yy,ok,gc]=edit_curv(xx,yy,'axy',[' ',' ',' '],gc)
    if ~ok then break,end
    n=size(xx,'*')
    if or(xx(2:n)-xx(1:n-1)<0) then
      message('You have not defined a function')
      ok=%f
    end
    if ok then
      model(1)='intplt'
      model(11)=[] //compatibility
      rect=gc(1)
      model(8)=[xx(:);yy(:);rect(:)]
      axisdata=gc(2)
      model(9)=[size(xx,'*');axisdata(:)]
      x(2)=graphics;x(3)=model
      break
    end
  end
  xdel(win)
  xset('window',curwin)
case 'define' then
  xx=[0;1;2];yy=[-5;5;0]
  rect=[0,-5,2,5];
  axisdata=[2;10;2;10]
  ipar=[size(xx,1);axisdata(:)]
  rpar=[xx;yy;rect(:)]
  model=list('intplt',[],1,[],[],[],[],rpar,ipar,'c',[],[%f %t],' ',list())
  gr_i=['model=arg1(3);rpar=model(8);ipar=model(9);n=ipar(1);';
    'thick=xget(''thickness'');xset(''thickness'',2);';
    'xx=rpar(1:n);yy=rpar(n+1:2*n);';
    'rect=rpar(2*n+1:2*n+4);';
    'mxx=rect(3)-rect(1);';
    'mxy=rect(4)-rect(2);';
    'xx=orig(1)+sz(1)*(1/10+(4/5)*((xx-rect(1))/mxx));';
    'yy=orig(2)+sz(2)*(1/10+(4/5)*((yy-rect(2))/mxy));';
    'xpoly(xx,yy,''lines'');';
    'xset(''thickness'',thick);']
  x=standard_define([2 2],model,[],gr_i)
end
