function scs_m=empty_diagram()
// Copyright INRIA
context=[];
tf=100000
tolerances=[1.d-4,1.d-6,1.d-10,tf+1];
options=default_options()
Xshift=0;Yshift=0
doc=list() //documentation structure
scs_m=list(list([600,450,Xshift,Yshift,600,450],'Untitled',tolerances,tf,..
    context,list(),options,[],[],doc))
