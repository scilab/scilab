function [x,y,typ]=STEP_FUNCTION(job,arg1,arg2)
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
  ppath = list(1)
  newpar=list();
  for path=ppath do
    np=size(path,'*')
    spath=list()
    for k=1:np
      spath($+1)='model'
      spath($+1)='rpar'
      spath($+1)='objs'
      spath($+1)=path(k)
    end
    xx=arg1(spath)// get the block
    execstr('xxn='+xx.gui+'(''set'',xx)')
    if diffobjs(xxn,xx) then 
      // parameter or states changed
      arg1(spath)=xxn// Update
      newpar(size(newpar)+1)=path// Notify modification
    end
  end
  x=arg1
  y=0
  typ=newpar
case 'define' then
  model = mlist(..
  ['model','sim','in','out','evtin','evtout','state','dstate','rpar','ipar','blocktype',..
  'firing','dep_ut','label','nzcross','nmode','equations'],'csuper',[],-1,[],[],[],[],..
  mlist(['diagram','props','objs'],..
  tlist(..
  ['params','wpar','title','tol','tf','context','void1','options','void2','void3','doc'],..
  [600,450,0,0,600,450],['STEP_FUNCTION','./'],[0.0001;1.000E-06;1.000E-10;100001;0;0],14,..
  ' ',[],..
  tlist(['scsopt','3D','Background','Link','ID','Cmap'],list(%t,33),[8,1],[1,5],..
  list([5,0],[4,0]),[0.8,0.8,0.8]),[],[],list()),..
  list(..
  mlist(['Block','graphics','model','gui','doc'],..
  mlist(..
  ['graphics','orig','sz','flip','exprs','pin','pout','pein','peout','gr_i','id',..
  'in_implicit','out_implicit'],[82.230597,652.6813],[40,40],%t,['1';'0';'1'],[],4,2,2,..
  list(['txt=[''Step''];';'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');'],8),..
  emptystr(),[],[]),..
  mlist(..
  ['model','sim','in','out','evtin','evtout','state','dstate','rpar','ipar','blocktype',..
  'firing','dep_ut','label','nzcross','nmode','equations'],list('step_func',4),[],1,1,1,[],..
  [],[0;1],[],'c',1,[%f,%f],emptystr(),0,0,list()),'STEP',list()),..
  mlist(['Link','xx','yy','id','thick','ct','from','to'],..
  [102.2306;102.2306;63.708992;63.708992;102.2306;102.2306],..
  [646.96701;622.2884;622.2884;711.98452;711.98452;698.39559],'drawlink',[0,0],[5,-1],..
  [1,1,0],[1,1,1]),..
  mlist(['Block','graphics','model','gui','doc'],..
  mlist(..
  ['graphics','orig','sz','flip','exprs','pin','pout','pein','peout','gr_i','id',..
  'in_implicit','out_implicit'],[150.80203,662.6813],[20,20],%t,'1',4,[],[],[],list(' ',8),..
  emptystr(),[],[]),..
  mlist(..
  ['model','sim','in','out','evtin','evtout','state','dstate','rpar','ipar','blocktype',..
  'firing','dep_ut','label','nzcross','nmode','equations'],'output',-1,[],[],[],[],[],[],1,..
  'c',[],[%f,%f],emptystr(),0,0,list()),'OUT_f',list()),..
  mlist(['Link','xx','yy','id','thick','ct','from','to'],[130.80203;150.80203],..
  [672.6813;672.6813],'drawlink',[0,0],[1,1],[1,1,0],[3,1,1]))),[],'h',[],[%f,%f],emptystr(),..
  0,0,list())
  gr_i=[  'thick=xget(''thickness'')'
	  'pat=xget(''pattern'')'
	  'fnt=xget(''font'')'
	  'xpoly(orig(1)+[0.071;0.413;0.413;0.773]*sz(1),orig(2)+[0.195;0.195;0.635;0.635]*sz(2),'"lines"')';
	  'xset(''thickness'',thick)'
	  'xset(''pattern'',pat)'
	  'xset(''font'',fnt(1),fnt(2))']
  x=standard_define([2 2],model,[],gr_i)
end
