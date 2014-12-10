// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

loadXcosLibs();
scicos_log("TRACE");

link = scicos_link(from=[2,1,0],to=[3,1,1]);
subDiag = scicos_diagram( objs=list(link) );

Super1 = scicos_block(,..
model=scicos_model(sim="super",in=[],in2=[],intyp=1,out=[],out2=[],outtyp=[],evtin=[],evtout=[],state=[],dstate=[],odstate=list(),..
rpar=subDiag,..
ipar=[],opar=list(),blocktype="h",firing=[],dep_ut=[%f,%f],label="Quat_Conj",nzcross=0,nmode=0,equations=list())..
);
diag = scicos_diagram();
diag.objs(1) = Super1;
diag.objs(1)
diag.objs(1).model
diag.objs(1).model.rpar.objs(1)

Super2 = scicos_block(,..
model=scicos_model(sim="super",in=[],in2=[],intyp=1,out=[],out2=[],outtyp=[],evtin=[],evtout=[],state=[],dstate=[],odstate=list(),..
rpar=diag,..
ipar=[],opar=list(),blocktype="h",firing=[],dep_ut=[%f,%f],label="Quat_Conj",nzcross=0,nmode=0,equations=list())..
);

Super2.model
Super2.model.rpar
Super2.model.rpar.objs(1)
Super2.model.rpar.objs(1).model
Super2.model.rpar.objs(1).model.rpar
Super2.model.rpar.objs(1).model.rpar.objs(1)
assert_checkequal(Super2.model.rpar.objs(1).model.rpar.objs(1).from, [2 1 0]);
assert_checkequal(Super2.model.rpar.objs(1).model.rpar.objs(1).to,   [3 1 1]);


// Check that all the model items are freed
clear
