function [txt,vnms,vtps,lcount]=cla2sci(clause,vnms,vtps)
// traduit une clause (if while for select)
//!
// Copyright INRIA
typ=clause(1)
//write(6,'cla2sci '+typ(1))
level;level(1)=level(1)+1
select typ(1)
case %if then
  ncl=size(clause)
  ncas=(ncl-2)/2
  tg=[]
  level(2)=1
  [t1,t2,ilst,vnms,vtps,lcount]=exp2sci(clause(2),1,vnms,vtps)
  t1=t1(1);
  tg=[tg;t2]
  txt=[%if+' '+t1(1)+' '+%then]
  vtps_n=vtps
  vnms_n=vnms
  [t1,vnms_1,vtps_1,lcount]=ins2sci(clause(3),1,vnms,vtps)
  txt=[txt;indentsci(t1)];
  [vtps_n,vnms_n]=updatevtps(size(vtps),vtps_n,vtps_1,vnms_n,vnms_1)
  // elseif parts
  for ic=2:ncas
    level(2)=ic
    [t1,t2,ilst,vnms,vtps,lcount]=exp2sci(clause(2*ic),1,vnms,vtps)
    if type(t1(1))==15 then t1=t1(1),end
    tg=[tg;t2]
    txt=[txt;%elseif+' '+t1(1)+' '+%then];
    [t1,vnms_1,vtps_1,lcount]=ins2sci(clause(1+2*ic),1,vnms,vtps)
    [vtps_n,vnms_n]=updatevtps(size(vtps),vtps_n,vtps_1,vnms_n,vnms_1)
    txt=[txt;indentsci(t1)];
  end;
  // else part
  [t1,vnms_1,vtps_1,lcount]=ins2sci(clause(ncl),1,vnms,vtps)
  if t1<>[] then
    txt=[txt;%else;indentsci(t1)];
    [vtps_n,vnms_n]=updatevtps(size(vtps),vtps_n,vtps_1,vnms_n,vnms_1)
  end
  txt=[tg;txt;%end]
  //
  vtps=vtps_n
  vnms=vnms_n
case %while then
  level(2)=1
  [t1,t2,ilst,vnms,vtps,lcount]=exp2sci(clause(2),1,vnms,vtps)
  t1=t1(1);
  if t2==[] then
    txt=[%while+' '+t1(1)+' '+%then]
  else
    txt=[%while+' %t '+%then;
        t2
        %if+' '+t1(1)+' '+%then+' break;'+%end]
  end
  vtps_n=vtps
  vnms_n=vnms
  
  [t1,vnms_1,vtps_1,lcount]=ins2sci(clause(3),1,vnms,vtps)
  txt=[txt;indentsci(t1);%end]
  [vtps,vnms]=updatevtps(size(vtps),vtps_n,vtps_1,vnms_n,vnms_1)
case %for then
  name=typ(2)
  sciexp=1
  level(2)=1
  [t1,t2,ilst,vnms,vtps,lcount]=exp2sci(clause(2),1,vnms,vtps)
  kn=find(name==vnms(:,2))
  if kn==[] then 
    if funptr(name)<>0 then 
      vnms=[vnms;['%'+name,name]];
      name='%'+name
    else
      vnms=[vnms;[name,name]];
    end
    kn=size(vnms,1),
  else
    vnms(kn($),:)=[name,name]
  end
  t1=t1(1);
  if t1(2)=='2' then
    vtps(kn)=list('1','1','1',0)
  else
    vtps(kn)=list('1','?','1',0)
  end
  txt=[t2;%for+' '+name+' = '+t1(1)];
  sciexp=0;
  vtps_n=vtps
  vnms_n=vnms
  [t1,vnms_1,vtps_1,lcount]=ins2sci(clause(3),1,vnms,vtps)
  txt=[txt; indentsci(t1);%end]
  [vtps,vnms]=updatevtps(size(vtps),vtps_n,vtps_1,vnms_n,vnms_1)
case %select then
  ncas=(size(clause)-3)/2
  tg=[]
  level(2)=1
  [exp1,t1,ilst,vnms,vtps,lcount]=exp2sci(clause(2),1,vnms,vtps)
  tg=[tg;t1]
  exp1=exp1(1)
  txt=[%select+' '+exp1(1)];
  txt=[txt;indentsci(t1)];
  vtps_n=vtps
  vnms_n=vnms
  for ic=2:2:2*ncas
    level(2)=ic/2
    [exp2,t2,ilst,vnms,vtps,lcount]=exp2sci(clause(1+ic),1,vnms,vtps)
    tg=[tg;t2]
    exp2=exp2(1)
    txt=[txt;%case+' '+exp2(1)+' '+%then]
    [t1,vnms_1,vtps_1,lcount]=ins2sci(clause(2+ic),1,vnms,vtps)
    [vtps_n,vnms_n]=updatevtps(size(vtps),vtps_n,vtps_1,vnms_n,vnms_1)
    txt=[txt;indentsci(t1)];
  end;
  [t1,vnms_1,vtps_1,lcount]=ins2sci(clause(3+2*ncas),1,vnms,vtps)
  if t1<>[] then
    txt=[txt;%else;indentsci(t1)];
    [vtps_n,vnms_n]=updatevtps(size(vtps),vtps_n,vtps_1,vnms_n,vnms_1)
  end
  txt=[tg;txt;%end]
  vtps=vtps_n
  vnms=vnms_n
end
endfunction
