function state=scicos_state(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11)
// Copyright INRIA
//initialisation de state mlist
  if exists('x','local')==0 then x=[],end
  if exists('z','local')==0 then z=[],end
  if exists('oz','local')==0 then oz=list,end
  if exists('iz','local')==0 then iz=[],end
  if exists('tevts','local')==0 then tevts=[],end
  if exists('evtspt','local')==0 then evtspt=[],end
  if exists('pointi','local')==0 then pointi=1,end
  if exists('outtb','local')==0 then outtb=list(),end
  state=tlist(['xcs','x','z','oz','iz','tevts','evtspt','pointi','outtb'],..
	    x,z,iz,tevts,evtspt,pointi,outtb)
endfunction
