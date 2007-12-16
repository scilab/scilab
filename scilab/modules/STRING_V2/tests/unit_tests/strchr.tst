//===============================
if strchr('Scilab','l') <> 'lab' then pause,end
//===============================
if strchr('Scilab',' ')<> '' then pause,end
//===============================
STR = 'Scilab string module';
STR_MAT = [STR,STR;STR,STR];
if strchr(STR_MAT,'l') <> ['lab string module','lab string module';'lab string module','lab string module'] then pause,end
//===============================
REF = ['le','string module';'ring module','ab string module'];
if strchr(STR_MAT,['l','s';'r','a']) <> REF then pause,end
//===============================
STR_MAT = ["STR1","STR2";"STR3","STR4"];
if strchr(STR_MAT,["1","2";"3","4"]) <> ["1","2";"3","4"] then pause,end
//===============================

