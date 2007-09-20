function [x,y,typ]=Extract_Activation(job,arg1,arg2)
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
 x=arg1
case 'define' then
scs_m_1=scicos_diagram()
scs_m_1.props=tlist(..
["params","wpar","title","tol","tf","context","void1","options","void2","void3",..
"doc"],[600,450,0,0,600,450],["Extract_Activation","./"],..
[0.0001;1.000E-06;1.000E-10;100001;0;0;0],30," ",[],..
tlist(["scsopt","3D","Background","Link","ID","Cmap"],list(%t,33),[8,1],[1,5],..
list([5,1],[4,1]),[0.8,0.8,0.8]),[],[],list())
scs_m_1.objs(1)=mlist(["Block","graphics","model","gui","doc"],..
                mlist(..
                ["graphics","orig","sz","flip","exprs","pin","pout","pein",..
                "peout","gr_i","id","in_implicit","out_implicit"],..
                [150.65045,143.82208],[60,60],%t,["0";"0"],6,[],[],[3;4],..
                list(..
                ["txt=[''If in>0'';'' '';'' then    else''];";
                "xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');"],8),"","E",..
                []),..
                mlist(..
                ["model","sim","in","in2","intyp","out","out2","outtyp","evtin","evtout",..
                "state","dstate","odstate","rpar","ipar","opar",..
                "blocktype","firing","dep_ut","label","nzcross",..
                "nmode","equations"],list("ifthel",-1),1,[],1,[],[],1,[],[1;1],[],[],..
                list(),[],[],list(),..
                "l",[-1,-1],[%t,%f],"",0,0,list()),"IFTHEL_f",list())
scs_m_1.objs(2)=mlist(["Block","graphics","model","gui","doc"],..
                mlist(..
                ["graphics","orig","sz","flip","exprs","pin","pout","pein",..
                "peout","gr_i","id","in_implicit","out_implicit"],..
                [169.82143,96.146231],[16.666667,16.666667],%t,[],[],[],[3;4;0],..
                8,..
                list(..
                ["rx=sz(1)*p/2;ry=sz(2)/2";
                "xsegs(orig(1)+rx*[1/2.3 1;2-1/2.3 1],orig(2)+ry*[1 2-1/2.3;1,1/2.3],0)"],..
                8),"",[],[]),..
                mlist(..
                ["model","sim","in","in2","intyp","out","out2","outtyp","evtin","evtout",..
                "state","dstate","odstate","rpar","ipar","opar",..
                "blocktype","firing","dep_ut","label","nzcross",..
                "nmode","equations"],"sum",[],[],1,[],[],1,[1;1;1],1,[],[],list(),[],[],..
                list(),"d",-1,..
                [%f,%f],"",0,0,list()),"CLKSOMV_f",list())
scs_m_1.objs(3)=mlist(["Link","xx","yy","id","thick","ct","from","to"],..
                [170.65045;170.65045;150.04302;150.04302;169.82143],..
                [138.10779;128.235;128.235;104.47956;104.47956],"drawlink",[0,0],..
                [5,-1],[1,1,0],[2,1,1])
scs_m_1.objs(4)=mlist(["Link","xx","yy","id","thick","ct","from","to"],..
                [190.65045;190.65045;178.15476],[138.10779;111.55729;112.8129],..
                "drawlink",[0,0],[5,-1],[1,2,0],[2,2,1])
scs_m_1.objs(5)=mlist(["Block","graphics","model","gui","doc"],..
                mlist(..
                ["graphics","orig","sz","flip","exprs","pin","pout","pein",..
                "peout","gr_i","id","in_implicit","out_implicit"],..
                [102.07902,163.82208],[20,20],%t,"1",[],6,[],[],list(" ",8),"",..
                [],"E"),..
                mlist(..
                ["model","sim","in","in2","intyp","out","out2","outtyp","evtin","evtout",..
                "state","dstate","odstate","rpar","ipar","opar",..
                "blocktype","firing","dep_ut","label","nzcross",..
                "nmode","equations"],"input",[],[],1,-1,[],1,[],[],[],[],list(),[],1,list(),..
                "c",[],..
                [%f,%f],"",0,0,list()),"IN_f",list())
scs_m_1.objs(6)=mlist(["Link","xx","yy","id","thick","ct","from","to"],..
                [122.07902;142.07902],[173.82208;173.82208],"drawlink",[0,0],..
                [1,1],[5,1,0],[1,1,1])
scs_m_1.objs(7)=mlist(["Block","graphics","model","gui","doc"],..
                mlist(..
                ["graphics","orig","sz","flip","exprs","pin","pout","pein",..
                "peout","gr_i","id","in_implicit","out_implicit"],..
                [168.15476,38.527183],[20,30],%t,"1",[],[],8,[],list(" ",8),"",..
                [],[]),..
                mlist(..
                ["model","sim","in","in2","intyp","out","out2","outtyp","evtin","evtout",..
                "state","dstate","odstate","rpar","ipar","opar","blocktype","firing",..
                "dep_ut","label","nzcross",..
                "nmode","equations"],"output",[],[],1,[],[],1,1,[],[],[],list(),[],1,list(),"d",[],..
                [%f,%f],"",0,0,list()),"CLKOUTV_f",list())
scs_m_1.objs(8)=mlist(["Link","xx","yy","id","thick","ct","from","to"],..
                [178.15476;178.15476],[98.527183;68.527183],"drawlink",[0,0],..
                [5,-1],[2,1,0],[7,1,1])
model = mlist(..
["model","sim","in","in2","intyp","out","out2","outtyp","evtin","evtout",..
"state","dstate","odstate","rpar","ipar","opar","blocktype",..
"firing","dep_ut","label","nzcross","nmode","equations"],"csuper",-1,[],1,[],[],1,[],1,[],[],list(),..
scs_m_1,[],list(),"h",[],[%f,%f],"",0,0,list())
  gr_i='xstringb(orig(1),orig(2),[''Extract'';''Activation''],sz(1),sz(2),''fill'')';
  x=standard_define([3 2],model,[],gr_i)
end
endfunction