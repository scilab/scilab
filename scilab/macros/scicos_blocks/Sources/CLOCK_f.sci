function [x,y,typ]=CLOCK_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[],typ=[]
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
  path = 3
  newpar=list();
  xx=arg1(3)(8)(path)// get the evtdly block
  label=xx(2)(4)
  model=xx(3);
  t0_old=model(11)
  dt_old= model(8)
  model_n=model
  while %t do
    [ok,dt,t0,label]=getvalue('Set Clock  block parameters',..
	['Period';'Init time'],list('vec',1,'vec',1),label)
    if ~ok then break,end
    if dt<=0 then
      message('period must be positive')
      ok=%f
    end
    if ok then
      xx(2)(4)=label
      model(8)=dt
      model(11)=t0
      xx(3)=model
      arg1(3)(8)(path)=xx// Update
      break
    end
  end
  if ~and([t0_old dt_old]==[t0 dt]) then 
    // parameter  changed
    newpar(size(newpar)+1)=path// Notify modification
  end
  if t0_old<>t0 then needcompile=2,else needcompile=0,end
  x=arg1
  y=needcompile
  typ=newpar
case 'define' then
x = list('Block',..
list([0,0],[2,2],%t,[],[],[],[],0,..
list(..
['wd=xget(''wdim'').*[1.016,1.12];';
'thick=xget(''thickness'');xset(''thickness'',2);';
'p=wd(2)/wd(1);p=1;';
'rx=sz(1)*p/2;ry=sz(2)/2;';
'xarcs([orig(1)+0.05*sz(1);';
'orig(2)+0.95*sz(2);';
'   0.9*sz(1)*p;';
'   0.9*sz(2);';
'   0;';
'   360*64],scs_color(5));';
'xset(''thickness'',1);';
'xx=[orig(1)+rx    orig(1)+rx;';
'    orig(1)+rx    orig(1)+rx+0.6*rx*cos(%pi/6)];';
'yy=[orig(2)+ry    orig(2)+ry ;';
'  orig(2)+1.8*ry  orig(2)+ry+0.6*ry*sin(%pi/6)];';
'xsegs(xx,yy,scs_color(10));';
'xset(''thickness'',thick);'],[])),..
list('csuper',[],[],[],1,[],' ',..
list(list([600,400,0,0],'foo',[],[]),..
list('Block',list([399,162],[20,20],%t,'1',[],[],6,[],[]),..
list('output',[],[],1,[],[],[],[],1,'d',[],[%f,%f],' ',list()),' ','CLKOUT_f'),..
list('Block',list([320,232],[40,40],%t,['0.1';'0.1'],[],[],7,4,..
['dt=model(8);';
'txt=[''Delay'';string(dt)];';
'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']),..
list('evtdly',[],[],1,1,[],[],0.1,[],'d',0.1,[%f,%f],' ',list()),' ','EVTDLY_f'),..
list('Link',[340;340;380.71066],[226.28571;172;172],'drawlink',' ',[0,0],[5,-1],[3,1],..
[5,1]),..
list('Block',list([380.71066;172],[1,1],%t,' ',[],[],4,[6;7]),..
list('lsplit',[],[],1,[1;1],[],[],[],[],'d',[-1,-1],[%t,%f],' ',list()),' ','CLKSPLIT_f'),..
list('Link',[380.71066;399],[172;172],'drawlink',' ',[0,0],[5,-1],[5,1],[2,1]),..
list('Link',[380.71066;380.71066;340;340],[172;301.9943;301.9943;277.71429],'drawlink',' ',..
[0,0],[5,-1],[5,2],[3,1])),[],'h',[],[%f,%f]),' ','CLOCK_f')
end
