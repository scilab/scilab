function [x,y,typ]=LOOKUP_f(job,arg1,arg2)
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
  model=arg1(3);rpar=model(8);
  n=size(rpar,'*')/2
  xx=rpar(1:n);yy=rpar(n+1:2*n)
  while %t do
    old_win=xget('window')
    win=maxi(winsid())+1
    xset('window',win);xsetech([0 0 1 1])
    [xx,yy,ok]=edit_curv(xx,yy,'axy')
    xdel(win)
    xset('window',old_win)
    if ~ok then break,end
    n=size(xx,'*')
    if or(xx(2:n)-xx(1:n-1)<=0) then
      message('You have not defined a function')
      ok=%f
    end
    if ok then
      model(8)=[xx(:);yy(:)]
      x(2)=graphics;x(3)=model
      break
    end
  end
  x(3)(11)=[] //compatibility
case 'define' then
  model=list('lookup',1,1,[],[],[],[],[-2;-1;1;2;-1;1;-1;1],[],'c',[],[%t %f],' ',list())
  gr_i=['rpar=model(8);n=size(rpar,''*'')/2;';
    'thick=xget(''thickness'');xset(''thickness'',2);';
    'xx=rpar(1:n);yy=rpar(n+1:2*n);';
    'mnx=mini(xx);xx=xx-mnx*ones(xx);mxx=maxi(xx);';
    'xx=orig(1)+sz(1)*(1/10+(4/5)*xx/mxx);';
    'mnx=mini(yy);yy=yy-mnx*ones(yy);mxx=maxi(yy);';
    'yy=orig(2)+sz(2)*(1/10+(4/5)*yy/mxx);';
    'xpoly(xx,yy,''lines'');';
    'xset(''thickness'',thick);']
  x=standard_define([2 2],model,[],gr_i)
end




