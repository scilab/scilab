function [x,y,typ]=ANDBLK(job,arg1,arg2)
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
case 'define' then
  model = list('csuper',[],[],[1;1],1,[],[],..
  list(list([600,400,0,0],'ANDBLK',[],[]),..
  list('Block',..
  list([194,133],[60,60],%t,[],[],10,[5;12],[],..
  ['txt=[''LOGICAL'';'' '';'' AND ''];';
  'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']),..
  list('andlog',[],1,[1;1],[],[],[],[],[],'d',[],[%f,%f],' ',list()),' ','ANDLOG_f'),..
  list('Block',list([149,287],[20,20],%t,'1',[],[],[],5,[]),..
  list('input',[],[],[],1,[],[],[],1,'d',-1,[%f,%f],' ',list()),' ','CLKIN_f'),..
  list('Block',list([450,83],[20,20],%t,'1',[],[],9,[],[]),..
  list('output',[],[],1,[],[],[],[],1,'d',[],[%f,%f],' ',list()),' ','CLKOUT_f'),..
  list('Link',[169;214;214],[297;297;198.71429],'drawlink',' ',[0,0],[5,-1],[3,1],[2,1]),..
  list('Block',list([141,330],[20,20],%t,'2',[],[],[],7,[]),..
  list('input',[],[],[],1,[],[],[],2,'d',%t,[-1,-1],' ',list()),' ','CLKIN_f'),..
  list('Link',[161;234;234],[340;340;275.78348],'drawlink',' ',[0,0],[5,-1],[6,1],[11,1]),..
  list('Block',..
  list([331,137],[60,60],%t,[],10,[],13,[9;0],..
  ['txt=[''If in>=0'';'' '';'' then    else''];';
  'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']),..
  list('ifthel',1,[],1,[1;1],[],[],[],[],'l',[-1,-1],[%f,%f],' ',list()),' ','IFTHEL_f'),..
  list('Link',[351;351;450],[131.28571;93;93],'drawlink',' ',[0,0],[5,-1],[8,1],[4,1]),..
  list('Link',[262.57143;322.42857],[163;167],'drawlink',' ',[0,0],[1,1],[2,1],[8,1]),..
  list('Block',list([234;275.78348],[1,1],%t,' ',[],[],7,[12;13]),..
  list('lsplit',[],[],1,[1;1],[],[],[],[],'d',[-1,-1],[%t,%f],' ',list()),' ','CLKSPLIT_f'),..
  list('Link',[234;234],[275.78348;198.71429],'drawlink',' ',[0,0],[5,-1],[11,1],[2,2]),..
  list('Link',[234;361;361],[275.78348;275.78348;202.71429],'drawlink',' ',[0,0],[5,-1],..
  [11,2],[8,1])),[],'h',%f,[%f,%f])
  gr_i='xstringb(orig(1),orig(2),''ANDBLK'',sz(1),sz(2),''fill'')';
  x=standard_define([3 2],model,[],gr_i)
end
