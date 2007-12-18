//===============================
if strrchr('Scilab','l') <> 'lab' then pause,end
//===============================
if strrchr('Scilab',' ')<> '' then pause,end
//===============================
STR = 'Scilab string module';
STR_MAT = [STR,STR;STR,STR];
if strrchr(STR_MAT,'l') <> ['le','le';'le','le'] then pause,end
//===============================
REF = ['le','string module';'ring module','ab string module'];
if strrchr(STR_MAT,['l','s';'r','a']) <> REF then pause,end
//===============================
STR_MAT = ["STR1","STR2";"STR3","STR4"];
if strrchr(STR_MAT,["1","2";"3","4"]) <> ["1","2";"3","4"] then pause,end
//===============================

