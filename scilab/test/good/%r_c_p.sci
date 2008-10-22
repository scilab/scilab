function f=%r_c_p(f,p)
// f=%r_c_p(f,p) <=> f=[f,p]  [rational, polynomial]
// Copyright INRIA
f=rlist([f('num'),p],[f('den'),ones(p)],f('dt'))
endfunction
