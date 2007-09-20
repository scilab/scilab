function [x,y,typ]=EDGE_TRIGGER(job,arg1,arg2)
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
  y=0;
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
	  needcompile=1
	end
	if or(model.firing<>model_n.firing)  then 
	  needcompile=2
	end
	if (size(model.in,'*')<>size(model_n.in,'*'))|..
        (size(model.out,'*')<>size(model_n.out,'*')) then  
         needcompile=4  
       end  
	if model.sim=='input'|model.sim=='output' then
	  if model.ipar<>model_n.ipar then
	    needcompile=4
	  end
	end
	if or(model.blocktype<>model_n.blocktype)|..
	      or(model.dep_ut<>model_n.dep_ut)  then 
	  needcompile=4
	end
	if (model.nzcross<>model_n.nzcross)|(model.nmode<>model_n.nmode) then 
	  needcompile=4
	end
	if prod(size(model_n.sim))>1 then
	  if model_n.sim(2)>1000 then 
	    if model.sim(1)<>model_n.sim(1) then
	      needcompile=4
	    end
	  end
	end
      else 
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
      y=max(y,needcompile)
    end
  end
  x=arg1
  typ=newpar
case 'define' then
scs_m_1=scicos_diagram()
scs_m_1.props=tlist(..
["params","wpar","title","tol","tf","context","void1","options","void2","void3",..
"doc"],[600,450,0,0,600,450],["EDGE_TRIGGER","./"],..
[0.0001;1.000E-06;1.000E-10;100001;0;0;0],30," ",[],..
tlist(["scsopt","3D","Background","Link","ID","Cmap"],list(%t,33),[8,1],[1,5],..
list([5,1],[4,1]),[0.8,0.8,0.8]),[],[],list())
scs_m_1.objs(1)=mlist(["Block","graphics","model","gui","doc"],..
                mlist(..
                ["graphics","orig","sz","flip","exprs","pin","pout","pein",..
                "peout","gr_i","id","in_implicit","out_implicit"],..
                [288.58631,257.1131],[60,40],%t,"0",5,3,[],[],..
                list(..
                "xstringb(orig(1),orig(2),[''Edge'';''trigger''],sz(1),sz(2),''fill'');",..
                8),"","E","E"),..
                mlist(..
                ["model","sim","in","in2","intyp","out","out2","outtyp","evtin","evtout",..
                "state","dstate","odstate",..
                "rpar","ipar","opar","blocktype","firing","dep_ut","label","nzcross",..
                "nmode","equations"],list("edgetrig",4),1,[],1,1,[],1,[],[],[],0,list(),..
                [],0,list(),"c",..
                [],[%t,%f],"",1,0,list()),"EDGETRIGGER",list())
scs_m_1.objs(2)=mlist(["Block","graphics","model","gui","doc"],..
                mlist(..
                ["graphics","orig","sz","flip","exprs","pin","pout","pein",..
                "peout","gr_i","id","in_implicit","out_implicit"],..
                [388.28869,247.1131],[60,60],%t,["0";"0"],3,[],[],[7;0],..
                list(..
                ["txt=[''If in>0'';'' '';'' then    else''];";
                "xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');"],8),"","E",..
                []),..
                mlist(..
                ["model","sim","in","in2","intyp","out","out2","outtyp","evtin","evtout",..
                "state","dstate","odstate",..
                "rpar","ipar","opar","blocktype","firing","dep_ut","label","nzcross",..
                "nmode","equations"],list("ifthel",-1),1,[],1,[],1,[],[],[1;1],[],[],list(),..
                [],[],list(),"l",[-1,-1],[%t,%f],"",0,0,list()),"IFTHEL_f",list())
scs_m_1.objs(3)=mlist(["Link","xx","yy","id","thick","ct","from","to"],..
                [357.15774;362.99107;379.71726],[277.1131;277.1131;277.1131],..
                "drawlink",[0,0],[1,1],[1,1,0],[2,1,1])
scs_m_1.objs(4)=mlist(["Block","graphics","model","gui","doc"],..
                mlist(..
                ["graphics","orig","sz","flip","exprs","pin","pout","pein",..
                "peout","gr_i","id","in_implicit","out_implicit"],..
                [240.01488,267.1131],[20,20],%t,"1",[],5,[],[],list(" ",8),"",[],..
                "E"),..
                mlist(..
                ["model","sim","in","in2","intyp","out","out2","outtyp","evtin","evtout",..
                "state","dstate","odstate",..
                "rpar","ipar","opar","blocktype","firing","dep_ut","label","nzcross",..
                "nmode","equations"],"input",[],[],1,-1,[],1,[],[],[],[],list(),[],1,list(),"c",[],..
                [%f,%f],"",0,0,list()),"IN_f",list())
scs_m_1.objs(5)=mlist(["Link","xx","yy","id","thick","ct","from","to"],..
                [260.01488;280.01488],[277.1131;277.1131],"drawlink",[0,0],[1,1],..
                [4,1,0],[1,1,1])
scs_m_1.objs(6)=mlist(["Block","graphics","model","gui","doc"],..
                mlist(..
                ["graphics","orig","sz","flip","exprs","pin","pout","pein",..
                "peout","gr_i","id","in_implicit","out_implicit"],..
                [398.28869,181.39881],[20,30],%t,"1",[],[],7,[],list(" ",8),"",..
                [],[]),..
                mlist(..
                ["model","sim","in","in2","intyp","out","out2","outtyp","evtin","evtout",..
                "state","dstate","odstate",..
                "rpar","ipar","opar","blocktype","firing","dep_ut","label","nzcross",..
                "nmode","equations"],"output",[],[],1,[],[],1,1,[],[],[],list(),[],1,list(),"d",[],..
                [%f,%f],"",0,0,list()),"CLKOUTV_f",list())
scs_m_1.objs(7)=mlist(["Link","xx","yy","id","thick","ct","from","to"],..
                [408.28869;408.28869],[241.39881;211.39881],"drawlink",[0,0],..
                [5,-1],[2,1,0],[6,1,1])
model = mlist(..
["model","sim","in","in2","intyp","out","out2","outtyp","evtin","evtout",..
"state","dstate","odstate","rpar","ipar","opar","blocktype",..
"firing","dep_ut","label","nzcross","nmode","equations"],"csuper",-1,[],1,[],[],1,[],1,[],[],list(),..
scs_m_1,[],list(),"h",[],[%f,%f],"",0,0,list())
  gr_i='xstringb(orig(1),orig(2),[''EDGE'';''TRIGGER''],sz(1),sz(2),''fill'')';
  x=standard_define([3 2],model,[],gr_i)
end
endfunction