function [x,y,typ]=freq_div(job,arg1,arg2)
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
 newpar=list();
 y=0;
 spath=list()
 spath($+1)='model'
 spath($+1)='rpar'
 spath($+1)='objs'
 spath($+1)=1

 xx=arg1(spath)// get the block
	       //execstr('xxn='+xx.gui+'(''set'',xx)')
 xxn=xx;
 graphics=xx.graphics;exprs=graphics.exprs
 model=xx.model;
 while %t do
   [ok,%ph,%df,exprs]=getvalue...
       ('Set frequency division block parameters',...
	['Phase (0 to division factor -1)';'Division factor'],...
	list('vec',1,'vec',1),exprs)
   if ~ok then break,end
   if ok then
     if %df<1 then %df=1,end
     %ph=abs(%ph)
     if %ph>%df-1 then %ph=%df-1,end
     graphics.exprs=exprs
     model.ipar=%df;
     model.dstate=%ph;
     xxn.graphics=graphics;xxn.model=model
     break
   end
 end
 
 
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
   newpar(size(newpar)+1)=1// Notify modification
   y=max(y,needcompile)
 end
 x=arg1
 typ=newpar
 case 'define' then
  scs_m_1=scicos_diagram()
  scs_m_1.props=tlist(..
["params","wpar","title","tol","tf","context","void1","options","void2","void3",..
"doc"],[600,450,0,0,600,450],"freq_div",[0.0001,1.000E-06,1.000E-10,100001,0,0],100000," ",[],..
tlist(["scsopt","3D","Background","Link","ID","Cmap"],list(%t,33),[8,1],[1,5],..
list([5,1],[4,1]),[0.8,0.8,0.8]),[],[],list())
scs_m_1.objs(1)=mlist(["Block","graphics","model","gui","doc"],..
mlist(["graphics","orig","sz","flip","exprs","pin","pout","pein",..
"peout","gr_i","id","in_implicit","out_implicit"],..
[60.518363,178.33333],[60,40],%t,["0";"3"],[],7,10,[],..
             list(..
        "xstringb(orig(1),orig(2),[''  Counter'';''Modulo ''+string(base)],sz(1),sz(2),''fill'');",..
                8),"",[],"E"),..
                mlist(..
                ["model","sim","in","in2","intyp","out","out2","outtyp","evtin","evtout",..
                "state","dstate","odstate",..
                "rpar","ipar","opar","blocktype","firing","dep_ut","label","nzcross",..
                "nmode","equations"],list("modulo_count",4),[],[],1,1,[],1,1,[],[],0,list(),..
                [],3,list(),..
                "c",[],[%f,%f],"",0,0,list()),"Modulo_Count",list())
scs_m_1.objs(2)=mlist(["Block","graphics","model","gui","doc"],..
                mlist(..
                ["graphics","orig","sz","flip","exprs","pin","pout","pein",..
                "peout","gr_i","id","in_implicit","out_implicit"],..
                [215.37648,299.81481],[20,30],%t,"1",[],[],[],6,..
                list(..
                ["xo=orig(1);yo=orig(2)+sz(2)/3";
                "xstringb(xo,yo,string(prt),sz(1),sz(2)/1.5)"],8),"",[],[]),..
                mlist(..
                ["model","sim","in","in2","intyp","out","out2","outtyp","evtin","evtout",..
                "state","dstate","odstate",..
                "rpar","ipar","opar","blocktype","firing","dep_ut","label","nzcross",..
                "nmode","equations"],"input",[],[],1,[],[],1,[],1,[],[],list(),[],1,..
                list(),"d",-1,..
                [%f,%f],"",0,0,list()),"CLKINV_f",list())
scs_m_1.objs(3)=mlist(["Block","graphics","model","gui","doc"],..
                mlist(..
                ["graphics","orig","sz","flip","exprs","pin","pout","pein",..
                "peout","gr_i","id","in_implicit","out_implicit"],..
                [221.30407,86.481481],[20,30],%t,"1",[],[],5,[],list(" ",8),"",..
                [],[]),..
                mlist(..
                ["model","sim","in","in2","intyp","out","out2","outtyp","evtin","evtout",..
                "state","dstate","odstate",..
                "rpar","ipar","opar","blocktype","firing","dep_ut","label","nzcross",..
                "nmode","equations"],"output",[],[],1,[],[],1,1,[],[],[],list(),[],1,list(),"d",[],..
                [%f,%f],"",0,0,list()),"CLKOUTV_f",list())
scs_m_1.objs(4)=mlist(["Block","graphics","model","gui","doc"],..
                mlist(..
                ["graphics","orig","sz","flip","exprs","pin","pout","pein",..
                "peout","gr_i","id","in_implicit","out_implicit"],..
                [193.14804,168.7037],[60,60],%t,["1";"0"],7,[],9,[0;5],..
                list(..
                ["txt=[''If in>0'';'' '';'' then    else''];";
                "xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');"],8),"","E",..
                []),..
                mlist(..
                ["model","sim","in","in2","intyp","out","out2","outtyp","evtin","evtout",..
                "state","dstate","odstate",..
                "rpar","ipar","opar","blocktype","firing","dep_ut","label","nzcross",..
                "nmode","equations"],list("ifthel",-1),1,[],1,[],[],1,1,[1;1],[],[],list(),..
                [],[],list(),..
                "l",[-1,-1],[%t,%f],"",0,0,list()),"IFTHEL_f",list())
scs_m_1.objs(5)=mlist(["Link","xx","yy","id","thick","ct","from","to"],..
                [233.14804;231.30407],[162.98942;116.48148],"drawlink",[0,0],..
                [5,-1],[4,2,0],[3,1,1])
scs_m_1.objs(6)=mlist(["Link","xx","yy","id","thick","ct","from","to"],..
                [225.37648;224.29194],[299.81481;267.98739],"drawlink",[0,0],..
                [5,-1],[2,1,0],[8,1,1])
scs_m_1.objs(7)=mlist(["Link","xx","yy","id","thick","ct","from","to"],..
                [129.08979;184.57662],[198.33333;198.7037],"drawlink",[0,0],..
                [1,1],[1,1,0],[4,1,1])
scs_m_1.objs(8)=mlist(["Block","graphics","model","gui","doc"],..
                mlist(..
                ["graphics","orig","sz","flip","exprs","pin","pout","pein",..
                "peout","gr_i","id","in_implicit","out_implicit"],..
                [224.29194;267.98739],[0.3333333,0.3333333],%t,[],[],[],6,[9;10],..
                list([],8),"",[],[]),..
                mlist(..
                ["model","sim","in","in2","intyp","out","out2","outtyp","evtin","evtout",..
                "state","dstate","odstate",..
                "rpar","ipar","opar","blocktype","firing","dep_ut","label","nzcross",..
                "nmode","equations"],"split",[],[],1,[],[],1,1,[1;1],[],[],list(),..
                [],[],list(),"d",..
                [%f,%f,%f],[%f,%f],"",0,0,list()),"CLKSPLIT_f",list())
scs_m_1.objs(9)=mlist(["Link","xx","yy","id","thick","ct","from","to"],..
                [224.29194;223.14804],[267.98739;234.41799],"drawlink",[0,0],..
                [5,-1],[8,1,0],[4,1,1])
scs_m_1.objs(10)=mlist(["Link","xx","yy","id","thick","ct","from","to"],..
                 [224.29194;90.518363;90.518363],[267.98739;267.98739;224.04762],..
                 "drawlink",[0,0],[5,-1],[8,2,0],[1,1,1])
model = mlist(..
["model","sim","in","in2","intyp","out","out2","outtyp","evtin","evtout",..
"state","dstate","odstate","rpar","ipar","opar","blocktype",..
"firing","dep_ut","label","nzcross","nmode","equations"],"csuper",[],[],1,[],[],1,1,1,[],[],list(),..
scs_m_1,[],list(),"h",[],[%f,%f],"",0,0,list())
  gr_i='xstringb(orig(1),orig(2),''freq_div'',sz(1),sz(2),''fill'')';
  x=standard_define([2 2],model,[],gr_i)
end
endfunction