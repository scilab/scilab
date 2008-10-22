function [tevts,evtspt,pointi]=init_agenda(initexe,clkptr)

// Copyright INRIA
// order initial firing events in chronological order.
nblk=size(clkptr,1)-1
timevec=initexe(:,3)
if timevec<>[] then
  [timevec,indtime]=sort(-timevec)
  initexe=initexe(indtime,:)
else
  initexe=[]
end
timevec=[]
//initialize agenda
ninit=size(initexe,1)
pointi=0
nevts=clkptr(nblk+1)-1 //time events agenda size
tevts=0*ones(nevts,1)
if initexe<>[] then
  tevts(clkptr(initexe(:,1))+initexe(:,2)-1)=initexe(:,3)
end
evtspt=-ones(nevts,1)

if ninit>0 then 
  pointi=clkptr(initexe(1,1))+initexe(1,2)-1;
  evtspt(pointi)=0
end
if ninit>1 then
  evtspt(clkptr(initexe(1:ninit-1,1))+initexe(1:ninit-1,2)-1)=..
      clkptr(initexe(2:ninit,1))+initexe(2:ninit,2)-1;
  evtspt(clkptr(initexe(ninit,1))+initexe(ninit,2)-1)=0;
end
endfunction
