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
      model=xx.model
      model_n=xxn.model
      if ~is_modelica_block(xx) then
	modified=or(model.sim<>model_n.sim)|..
		 ~isequal(model.state,model_n.state)|..
		 ~isequal(model.dstate,model_n.dstate)|..
		 ~isequal(model.rpar,model_n.rpar)|..
		 ~isequal(model.ipar,model_n.ipar)|..
		 ~isequal(model.label,model_n.label)
	if or(model.in<>model_n.in)|or(model.out<>model_n.out) then  
	  // input or output port sizes changed
	  needcompile=1
	end
	if or(model.firing<>model_n.firing)  then 
	  // initexe changed
	  needcompile=2
	end
	if model.sim=='input'|model.sim=='output' then
	  if model.ipar<>model_n.ipar then
	    needcompile=4
	  end
	end
	if or(model.blocktype<>model_n.blocktype)|..
	      or(model.dep_ut<>model_n.dep_ut)  then 
	  // type 'c','d','z','l' or dep_ut changed
	  needcompile=4
	end
	if (model.nzcross<>model_n.nzcross)|(model.nmode<>model_n.nmode) then 
	  // size of zero cross changed
	  needcompile=4
	end
	if prod(size(model_n.sim))>1 then
	  if model_n.sim(2)>1000 then  // Fortran or C Block
	    if model.sim(1)<>model_n.sim(1) then  //function name has changed
	      needcompile=4
	    end
	  end
	end
      else //implicit block
	//force compilation if an implicit block has been edited
	modified=or(model_n<>model)
	eq=model.equations;eqn=model_n.equations;
	if or(eq.model<>eqn.model)|or(eq.inputs<>eqn.inputs)|..
				      or(eq.outputs<>eqn.outputs) then  
	  needcompile=4
	end
      end
      
      // parameter or states changed
      arg1(spath)=xxn// Update
      newpar(size(newpar)+1)=path// Notify modification
    end
  end
  x=arg1
  y=needcompile
  typ=newpar
case 'define' then
  model = mlist(..
  ['model','sim','in','in2','intyp','out','out2','outtyp','evtin','evtout','state','dstate',..
  'odstate','rpar','ipar','opar','blocktype','firing','dep_ut','label','nzcross','nmode','equations'],..
  'csuper',[],[],1,-1,[],1,[],[],[],[],list(),..
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
  ['model','sim','in','in2','intyp','out','out2','outtyp','evtin','evtout','state','dstate',..
  'odstate','rpar','ipar','opar','blocktype','firing','dep_ut','label','nzcross','nmode','equations'],..
  list('step_func',4),[],[],1,1,[],1,1,1,[],..
  [],list(),[0;1],[],list(),'c',1,[%f,%t],emptystr(),0,0,list()),'STEP',list()),..
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
  ['model','sim','in','in2','intyp','out','out2','outtyp','evtin','evtout','state','dstate',..
  'odstate','rpar','ipar','opar','blocktype','firing','dep_ut','label','nzcross','nmode','equations'],..
  'output',-1,[],1,[],[],1,[],[],[],[],list(),[],1,list(),..
  'c',[],[%f,%f],emptystr(),0,0,list()),'OUT_f',list()),..
  mlist(['Link','xx','yy','id','thick','ct','from','to'],[130.80203;150.80203],..
  [672.6813;672.6813],'drawlink',[0,0],[1,1],[1,1,0],[3,1,1]))),[],list(),'h',[],[%f,%f],emptystr(),..
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
endfunction
