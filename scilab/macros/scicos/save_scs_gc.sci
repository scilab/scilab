function scs_gc=save_scs_gc()
// save the current scicos graphic context
// and the curent scale ( xsetech)
// Copyright INRIA
vals=["alufunction";"pattern";"thickness";
    "pixmap";"background";"foreground";"dashes"];
[r1,r2]=xgetech()
scs_gc=list(xget('window'),list(r1,r2),driver());
for i=1:prod(size(vals)),
  scs_gc($+1)=list(vals(i),xget(vals(i)));
end
endfunction
