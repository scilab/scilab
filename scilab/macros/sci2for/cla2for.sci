function [txt,vnms,vtps,nwrk]=cla2for(clause,vnms,vtps,nwrk)
// traduit une clause (if while for select)
//!
// Copyright INRIA
typ=clause(1)
//write(6,'cla2for '+typ(1))

select typ(1)
case %if then

  ncl=size(clause)
  ncas=(ncl-2)/2
  [t1,t2,ilst,vnms,vtps,nwrk]=exp2for(clause(2),1,vnms,vtps,nwrk)
  t1=t1(1);
  txt=[t2;' '+%if+'('+t1(1)+') '+%then]
  [t1,vnms,vtps,nwrk]=ins2for(clause(3),1,vnms,vtps,nwrk)
  txt=[txt;indentfor(t1)];
  for ic=2:ncas
    [t1,t2,ilst,vnms,vtps,nwrk]=exp2for(clause(2*ic),1,vnms,vtps,nwrk)
    txt=[txt;t2;' '+%elseif+'('+t1(1)(1)+') '+%then];
    [t1,vnms,vtps,nwrk]=ins2for(clause(1+2*ic),1,vnms,vtps,nwrk)
    txt=[txt;indentfor(t1)];
  end;
  [t1,vnms,vtps,nwrk]=ins2for(clause(ncl),1,vnms,vtps,nwrk)
  if t1<>[] then
    txt=[txt;' '+%else;indentfor(t1)];
  end
  txt=[txt;' endif']
case %while then
  [lbl,nwrk]=newlab(nwrk)
  tl1=part(' '+string(10*lbl)'+'   ',1:6)
  [lbl,nwrk]=newlab(nwrk)
  tl2=part(' '+string(10*lbl)'+'   ',1:6)
  [t1,t2,ilst,vnms,vtps,nwrk]=exp2for(clause(2),1,vnms,vtps,nwrk)
  t1=t1(1);
  txt=[t2;tl1+'continue';' '+%if+'(.not.'+t1(1)+') goto '+tl2]
  [t1,vnms,vtps,nwrk]=ins2for(clause(3),1,vnms,vtps,nwrk)
  txt=[txt;t1;'goto '+tl1;tl2+'continue']
case %for then
  name=typ(2)
  if find(name==vnms(:,1))==[] then
    nv=size(vtps)+1
    vnms=[vnms;[name,name]]
    vtps(nv)=list('0','1','1',0)
  end
  forexp=1
  [lbl,nwrk]=newlab(nwrk)
  tl1=part(string(10*lbl)+'   ',1:6);
  [t1,t2,ilst,vnms,vtps,nwrk]=exp2for(clause(2),1,vnms,vtps,nwrk)
  if size(t1(1)(1),2)>1 then
    
    t1=strcat(t1(1)(1),',')
    txt=[t2;' do '+tl1+' '+name+' = '+t1];
  else
    t1=t1(1)
    txt=[t2;' do '+tl1+' i_'+name+' = 0,'+t1(5)+'-1';
	    '    call dcopy('+t1(4)+','+t1(1)+'(1+i_'+name+'*'+t1(4)+'),1,'+name+',1)']   ;
  end
  forexp=0;
  [t1,vnms,vtps,nwrk]=ins2for(clause(3),1,vnms,vtps,nwrk)
  txt=[txt; indentfor(t1);tl1+'continue']
case %select then
  ncas=(size(clause)-3)/2
  [exp1,t1,ilst,vnms,vtps,nwrk]=exp2for(clause(2),1,vnms,vtps,nwrk)
  exp1=exp1(1)
  [exp2,t2,ilst,vnms,vtps,nwrk]=exp2for(clause(3),1,vnms,vtps,nwrk)
  exp2=exp2(1)
  txt=[t1;t2;' '+%if+'('+exp1(1)+'.eq.'+exp2(1)+') '+%then];
  [t1,vnms,vtps,nwrk]=ins2for(clause(4),1,vnms,vtps,nwrk)
  txt=[txt;indentfor(t1)];
  for ic=4:2:2*ncas
    [exp2,t2,ilst,vnms,vtps,nwrk]=exp2for(clause(1+ic),1,vnms,vtps,nwrk)
    exp2=exp2(1)
    txt=[txt;' '+%elseif+'('+exp1(1)+'.eq.'+exp2(1)+' '+%then]
    [t1,vnms,vtps,nwrk]=ins2for(clause(2+ic),1,vnms,vtps,nwrk)
    txt=[txt;indentfor(t1)];
  end;
  [t1,vnms,vtps,nwrk]=ins2for(clause(3+2*ncas),1,vnms,vtps,nwrk)
  if t1<>[] then
    txt=[txt;' '+%else;indentfor(t1)];
  end
  txt=[txt;' endif']
end
//write(6,txt)
endfunction
