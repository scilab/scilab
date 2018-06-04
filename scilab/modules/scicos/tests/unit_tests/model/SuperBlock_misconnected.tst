// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - ESI Group - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

loadXcosLibs();

scs_m=scicos_diagram(..
version="scicos4.3",..
props=scicos_params(..
wpar=[600,450,0,0,600,450],..
Title="Test",..
tol=[0.000001;0.000001;1.000D-10;100001;0;1;0],..
tf=20,..
context=[],..
void1=[],..
options=tlist(["scsopt","3D","Background","Link","ID","Cmap"],list(%t,33),[8,1],[1,5],..
list([5,1],[4,1]),[0.8,0.8,0.8]),..
void2=[],..
void3=[],..
doc=list()))
scs_m_1=scicos_diagram(..
version="scicos4.3",..
props=scicos_params(..
wpar=[600,450,0,0,600,450],..
Title="Untitled",..
tol=[0.000001;0.000001;1.000D-10;100001;0;0;0],..
tf=100000,..
context="",..
void1=[],..
options=tlist(["scsopt","3D","Background","Link","ID","Cmap"],list(%t,33),[8,1],[1,5],..
list([5,1],[4,1]),[0.8,0.8,0.8]),..
void2=[],..
void3=[],..
doc=list()))
scs_m_1.objs(1)=scicos_block(..
gui="CLKOUT_f",..
graphics=scicos_graphics(..
orig=[99,-136],..
sz=[20,20],..
flip=%t,..
theta=0,..
exprs="1",..
pin=[],..
pout=[],..
pein=5,..
peout=[],..
gr_i=list("xstringb(orig(1),orig(2),""CLKOUT_f"",sz(1),sz(2));",8),..
id="",..
in_implicit=[],..
out_implicit=[],..
in_style=[],..
out_style=[],..
in_label=[],..
out_label=[],..
style="CLKOUT_f"),..
model=scicos_model(..
sim="output",..
in=[],..
in2=[],..
intyp=[],..
out=[],..
out2=[],..
outtyp=[],..
evtin=-1,..
evtout=[],..
state=[],..
dstate=[],..
odstate=list(),..
rpar=[],..
ipar=1,..
opar=list(),..
blocktype="d",..
firing=[],..
dep_ut=[%f,%f],..
label="",..
nzcross=0,..
nmode=0,..
equations=list(),..
uid="4512295e:12c22db13e9:-6683"),..
doc=list("4512295e:12c22db13e9:-6683"))
scs_m_1.objs(2)=scicos_block(..
gui="EVTDLY_c",..
graphics=scicos_graphics(..
orig=[0,-46],..
sz=[40,40],..
flip=%t,..
theta=0,..
exprs=["0.1";"0.1"],..
pin=[],..
pout=[],..
pein=4,..
peout=6,..
gr_i=list("xstringb(orig(1),orig(2),""EVTDLY_c"",sz(1),sz(2));",8),..
id="",..
in_implicit=[],..
out_implicit=[],..
in_style=[],..
out_style=[],..
in_label=[],..
out_label=[],..
style="EVTDLY_c"),..
model=scicos_model(..
sim=list("evtdly4",4),..
in=[],..
in2=[],..
intyp=[],..
out=[],..
out2=[],..
outtyp=[],..
evtin=-1,..
evtout=-1,..
state=[],..
dstate=[],..
odstate=list(),..
rpar=[0.1;0.1],..
ipar=[],..
opar=list(),..
blocktype="d",..
firing=0.1,..
dep_ut=[%f,%f],..
label="",..
nzcross=0,..
nmode=0,..
equations=list(),..
uid="4512295e:12c22db13e9:-667f"),..
doc=list("4512295e:12c22db13e9:-667f"))
scs_m_1.objs(3)=scicos_block(..
gui="CLKSPLIT_f",..
graphics=scicos_graphics(..
orig=[92.71066,-138],..
sz=[8,8],..
flip=%t,..
theta=0,..
exprs=[],..
pin=[],..
pout=[],..
pein=5,..
peout=[4;6],..
gr_i=list("xstringb(orig(1),orig(2),""CLKSPLIT_f"",sz(1),sz(2));",8),..
id="",..
in_implicit=[],..
out_implicit=[],..
in_style=[],..
out_style=[],..
in_label=[],..
out_label=[],..
style="CLKSPLIT_f"),..
model=scicos_model(..
sim="split",..
in=[],..
in2=[],..
intyp=[],..
out=[],..
out2=[],..
outtyp=[],..
evtin=-1,..
evtout=[-1;-1],..
state=[],..
dstate=[],..
odstate=list(),..
rpar=[],..
ipar=[],..
opar=list(),..
blocktype="d",..
firing=[-1;-1],..
dep_ut=[%f,%f],..
label="",..
nzcross=0,..
nmode=0,..
equations=list(),..
uid="4512295e:12c22db13e9:-667a"),..
doc=list("4512295e:12c22db13e9:-667a"))
scs_m_1.objs, 1;
scs_m_1.objs(4)=scicos_link(..
xx=[60;78;103.37733],..
yy=[-2;-202;-142],..
id="drawlink",..
thick=[0,0],..
ct=[5,-1],..
from=[2,1,1],..
to=[3,1,0])
scs_m_1.objs, 1;
scs_m_1.objs(5)=scicos_link(..
xx=[104.71066;129],..
yy=[-126;-112],..
id="drawlink",..
thick=[0,0],..
ct=[5,-1],..
from=[3,1,1],..
to=[1,1,1])
scs_m_1.objs, 1;
scs_m_1.objs(6)=scicos_link(..
xx=[106.04399;133.71;93;60],..
yy=[-142;-40;-40;-50],..
id="drawlink",..
thick=[0,0],..
ct=[5,-1],..
from=[3,2,0],..
to=[2,1,0])
scs_m_1.objs, 1;
scs_m.objs(1)=scicos_block(..
gui="CLOCK_c",..
graphics=scicos_graphics(..
orig=[110,-130],..
sz=[40,40],..
flip=%t,..
theta=0,..
exprs=[],..
pin=[],..
pout=[],..
pein=[],..
peout=0,..
gr_i=list("xstringb(orig(1),orig(2),""CLOCK_c"",sz(1),sz(2));",8),..
id="",..
in_implicit=[],..
out_implicit=[],..
in_style=[],..
out_style=[],..
in_label=[],..
out_label=[],..
style="mirror=false;CLOCK_c;flip=false"),..
model=scicos_model(..
sim="csuper",..
in=[],..
in2=[],..
intyp=[],..
out=[],..
out2=[],..
outtyp=[],..
evtin=[],..
evtout=-1,..
state=[],..
dstate=[],..
odstate=[],..
rpar=scs_m_1,..
ipar=[],..
opar=list(),..
blocktype="h",..
firing=-1,..
dep_ut=[%f,%f],..
label="",..
nzcross=0,..
nmode=0,..
equations=list(),..
uid="-38f07e57:12bd41b596e:-7f20"),..
doc=list("-38f07e57:12bd41b596e:-7f20"))

clear scs_m_1
scs_m_1 = scs_m.objs(1).model.rpar, 1;
scs_m_1.objs, 1;

assert_checkequal(scs_m_1.objs(4).from, [2,1,1])
assert_checkequal(scs_m_1.objs(4).to,   [3,1,0])

assert_checkequal(scs_m_1.objs(5).from, [3,1,1])
assert_checkequal(scs_m_1.objs(5).to,   [1,1,1])

assert_checkequal(scs_m_1.objs(6).from, [3,2,0])
assert_checkequal(scs_m_1.objs(6).to,   [2,1,0])

clear

