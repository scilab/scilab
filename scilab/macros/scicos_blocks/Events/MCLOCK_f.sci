function [x,y,typ]=MCLOCK_f(job,arg1,arg2)
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
  // paths to updatable parameters or states
  ppath = list(2)
  newpar=list();
  for path=ppath ,
    np=size(path,'*')
    spath=[matrix([3*ones(1,np);8*ones(1,np);path],1,3*np)]
    xx=get_tree_elt(arg1,spath)// get the block
    execstr('xxn='+xx(5)+'(''set'',xx)')
    if ~and(xxn==xx) then
      // parameter or states changed
      arg1=change_tree_elt(arg1,spath,xxn)// Update
      newpar(size(newpar)+1)=path// Notify modification
    end
  end
  x=arg1
  y=0
  typ=newpar
case 'define' then
  nn=2
  dt=0.1
  label=[string(dt);string(nn)]
  model = list('csuper',[],[],[],[1;1],[],[],..
  list(list([600,400],' ',[],[],[]),..
  list('Block',list([334,199],[40,40],%t,label,[],[],13,[5;4]),..
  list('mfclck',[],[],1,[1;1],[],0, .1,5,'d',[-1 0],[%f,%f],' ',list()),' ','MFCLCK_f'),..
  list('Block',list([457,161],[16.666667,16.666667],%t,' ',[],[],[5;10;0],6),..
  list('sum',[],[],[1;1;1],1,[],[],[],[],'d',[],[%f,%f],' ',list()),' ','CLKSOM_f'),..
  list('Link',[360.7;360.7;411.9],[193.3;169.3;169.3],'drawlink',' ',[0,0],[10,-1],[2,2],[9,1]),..
  list('Link',[347.3;347.3;461.8;461.8],[193.3;155.5;155.5;161],'drawlink',' ',[0,0],[10,-1],[2,1],[3,1]),..
  list('Link',[468.9;482.5],[169.3;169.3],'drawlink',' ',[0,0],[10,-1],[3,1],[12,1]),..
  list('Block',list([509,261],[20,20],%t,'1',[],[],11,[]),..
  list('output',[],[],1,[],[],[],[],1,'d',[],[%f,%f],' ',list()),' ','CLKOUT_f'),..
  list('Block',list([509,142],[20,20],%t,'2',[],[],14,[]),..
  list('output',[],[],1,[],[],[],[],2,'d',[],[%f,%f],' ',list()),' ','CLKOUT_f'),..
  list('Block',list([411.92504;169.33333],[1,1],%t,' ',[],[],4,[10;11]),..
  list('lsplit',[],[],1,[1;1],[],[],[],[],'d',[-1,-1],[%t,%f],' ',list()),' ','CLKSPLIT_f'),..
  list('Link',[411.9;457],[169.3;169.3],'drawlink',' ',[0,0],[10,-1],[9,1],[3,2]),..
  list('Link',[411.9;411.9;509],[169.3;271;271],'drawlink',' ',[0,0],[10,-1],[9,2],[7,1]),..
  list('Block',list([482.45315;169.33333],[1,1],%t,' ',[],[],6,[13;14]),..
  list('lsplit',[],[],1,[1;1],[],[],[],[],'d',[-1,-1],[%t,%f],' ',list()),' ','CLKSPLIT_f'),..
  list('Link',[482.5;489.6;489.6;354;354],[169.3;169.3;338.3;338.3;244.7],'drawlink',' ',[0,0],[10,-1],[12,1],[2,1]),..
  list('Link',[482.4;482.4;509],[169.3;152;152],'drawlink',' ',[0,0],[10,-1],[12,2],[8,1])),[],'h',%f,[%f,%f])
  gr_i=['txt=[''2freq clock'';''  f/n     f''];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']
  x=standard_define([3 3],model,' ',gr_i)
end




