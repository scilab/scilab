function lnk=editgraph_arc(v1,v2,v3,v4,v5,v6,v7,v8)
//Copyright INRIA
//Author : Serge Steer 2002

//initialisation de link mlist
  if exists('xx','local')==0 then xx=[],end
  if exists('yy','local')==0 then yy=[],end
  if exists('thick','local')==0 then thick=[0,0],end
  if exists('ct','local')==0 then ct=[1],end
  if exists('from','local')==0 then from=[],end
  if exists('to','local')==0 then to=[],end
  lnk=mlist(['Arc','xx','yy','thick','ct','from','to'],..
	    xx,yy,thick,ct,from,to)

endfunction
