function [stk,nwrk,txt,top]=%m2for(nwrk)
// multiplications 
//!
// Copyright INRIA

s2=stk(top);s1=stk(top-1);
txt1=[]
if s1(3)<>s2(3) then
  if s1(3)=='0' then 
    [s1,nwrk,txt1]=typconv(s1,nwrk,'1'),
  else
    [s2,nwrk,txt1]=typconv(s2,nwrk,'1'),
  end
end
[s1,te1,t1,m1,n1]=s1(1:5);
[s2,te2,t2,m2,n2]=s2(1:5);
it1=prod(size(s1))-1;it2=prod(size(s2))-1
if t1<>t2 then
  if t2=='0' then t2='1',end
end
//
if m1=='1'&n1=='1'&m2=='1'&n2=='1' then // cas scalaire
  select it1+2*it2
  case 0 then
     if te2=='2' then s2='('+s2+')',end
     if te1=='2' then s1='('+s1+')',end
     stk=list(mulf(s1,s2),'1',t2,m1,n1)
  case 1 then
     if te1=='2' then s1(1)='('+s1(1)+')',s1(2)='('+s1(2)+')',   end
     if te2=='2' then s2='('+s2+')',end
     stk=list([mulf(s1(1),s2(1)),mulf(s1(2),s2(1))],'1',t2,m1,n1)
  case 2 then
     if te2=='2' then s2(1)='('+s2(1)+')',s2(2)='('+s2(2)+')',   end
     if te1=='2' then s1='('+s1+')',end
     stk=list([mulf(s1(1),s2(1)),mulf(s1(1),s2(2))],'1',t2,m1,n1)
  case 3 then
     if te1=='2' then s1(1)='('+s1(1)+')',s1(2)='('+s1(2)+')',   end
     if te2=='2' then s2(1)='('+s2(1)+')',s2(2)='('+s2(2)+')',   end
     stk=list([subf(mulf(s1(1),s2(1)),mulf(s1(2),s2(2))),..
               addf(mulf(s1(1),s2(2)),mulf(s1(2),s2(1))) ],'1',t2,m1,n1)
  end
  txt=[txt1;txt]
  top=top-1
  return
end 
if m1=='1'&n1=='1' then
  mn2=mulf(m2,n2)
  select it1+2*it2
  case 0 then
  [out,nwrk,txt]=outname(nwrk,t2,m2,n2)
  if out<>s2 then
    txt=[txt;gencall(['dcopy',mn2,s2,'1',out,'1'])]
  end
  txt=[txt;gencall(['dscal',mn2,s1,out,'1'])]
  if ~isnum(s1)&op(2)==s1 then
    txt=[txt;gencall(['dcopy',mn2,out,'1',s1,'1'])]
    out=s1
  end
  stk=list(out,'-1',t2,m2,n2)
  case 1 then
    [outr,nwrk,txt]=outname(nwrk,t2,m2,n2)
    if part(outr,1:4)<>'work' then
      outi=outr+'_i'
      outr=outr+'_r'
    else
      [outi,nwrk,txt]=outname(nwrk,t2,m2,n2)
    end
    txt=[txt;
         gencall(['dcopy',mn2,s2,'1',outr,'1']);
         gencall(['dcopy',mn2,s2,'1',outi,'1']);
         gencall(['dscal',mn2,s1(1),outr,'1']);
         gencall(['dscal',mn2,s1(2),outi,'1'])];
    stk=list([outr,outi],'-1',t2,m2,n2)
  case 2 then
    [outr,nwrk,txt]=outname(nwrk,t2,m2,n2)
    if part(outr,1:4)<>'work' then
      outi=outr+'_i'
      outr=outr+'_r'
    else
     [outi,nwrk,txt]=outname(nwrk,t2,m2,n2)
    end
    txt=[txt;
         gencall(['dcopy',mn2,s2(1),'1',outr,'1']);
         gencall(['dcopy',mn2,s2(2),'1',outi,'1']);
         gencall(['dscal',mn2,s1,outr,'1']);
         gencall(['dscal',mn2,s1,outi,'1'])];
    stk=list([outr,outi],'-1',t2,m2,n2)
  case 3 then
    [outr,nwrk,txt]=outname(nwrk,t2,m2,n2)
    if part(outr,1:4)<>'work' then
      outi=outr+'_i'
      outr=outr+'_r'
    else
      [outi,nwrk,txt]=outname(nwrk,t2,m2,n2)
    end
    txt=[txt;
         gencall(['dcopy',mn2,s2(1),'1',outr,'1']);
         gencall(['dcopy',mn2,s2(2),'1',outi,'1']);
         gencall(['wscal',mn2,s1(1),s1(2),outr,outi,'1'])];
  stk=list([outr,outi],'-1',t2,m2,n2)
  end
  txt=[txt1;txt]
  top=top-1
  return
end 
if m2=='1'&n2=='1' then
  mn1=mulf(m1,n1)
  select it1*2*it2
  case 0 then
    [out,nwrk,txt]=outname(nwrk,t2,m1,n1)
    if out<>s1 then txt=[txt;gencall(['dcopy',mn1,s1,'1',out,'1'])], end
    txt=[txt;gencall(['dscal',mn1,s2,out,'1'])]
    if ~isnum(s2)&op(2)==s2 then
      txt=[txt;gencall(['dcopy',mn1,out,'1',s2,'1'])]
      out=s2
    end
    stk=list(out,'-1',t2,m1,n1)
  case 1 then
    [outr,nwrk,txt]=outname(nwrk,t1,m1,n1)
    if part(outr,1:4)<>'work' then
      outi=outr+'_i'
      outr=outr+'_r'
    else
      [outi,nwrk,txt]=outname(nwrk,t2,m2,n2)
    end
    txt=[txt;
         gencall(['dcopy',mn1,s1(1),'1',outr,'1']);
         gencall(['dcopy',mn1,s1(2),'1',outi,'1']);
         gencall(['dscal',mn1,s2,outr,'1']);
         gencall(['dscal',mn1,s2,outi,'1'])];
    stk=list([outr,outi],'-1',t2,m1,n1)
  case 2 then
    [outr,nwrk,txt]=outname(nwrk,t1,m1,n1)
    if part(outr,1:4)<>'work' then
      outi=outr+'_i'
      outr=outr+'_r'
    else
      [outi,nwrk,txt]=outname(nwrk,t2,m2,n2)
    end
    txt=[txt;
         gencall(['dcopy',mn1,s1,'1',outr,'1']);
         gencall(['dcopy',mn1,s1,'1',outi,'1']);
         gencall(['dscal',mn1,s2(1),outr,'1']);
         gencall(['dscal',mn1,s2(2),outi,'1'])];
    stk=list([outr,outi],'-1',t2,m1,n1)
  case 3 then
    [outr,nwrk,txt]=outname(nwrk,t1,m1,n1)
    if part(outr,1:4)<>'work' then
      outi=outr+'_i'
      outr=outr+'_r'
    else
      [outi,nwrk,txt]=outname(nwrk,t2,m2,n2)
    end
    txt=[txt;
         gencall(['dcopy',mn1,s1(1),'1',outr,'1']);
         gencall(['dcopy',mn1,s1(2),'1',outi,'1']);
         gencall(['wscal',mn1,s2(1),s2(2),outr,outi,'1'])]
    stk=list([outr,outi],'-1',t2,m1,n1)
  end
  txt=[txt1;txt]
  top=top-1
  return
end 
  select it1+2*it2
  case 0 then
    [out,nwrk,txt]=outname(nwrk,t2,m1,n2,[s1,s2])
    txt=[txt;gencall(['dmmul',s1,m1,s2,m2,out,m1,m1,n1,n2])]
    if ~isnum(s1)&op(2)==s1 then
      txt=[txt;gencall(['dcopy',mulf(m1,n2),out,'1',s1,'1'])]
      out=s1
    elseif ~isnum(s2)&op(2)==s2 then
      txt=[txt;gencall(['dcopy',mulf(m1,n2),out,'1',s2,'1'])]
      out=s2
    end
    stk=list(out,'-1',t2,m1,n2)
  case 1 then
    [outr,nwrk,txt]=outname(nwrk,t2,m1,n2,[s1,s2])
    [outi,nwrk,txt]=outname(nwrk,t2,m1,n2,[s1,s2])
    txt=[txt;gencall(['dmmul',s1(1),m1,s2(1),m2,outr,m1,m1,n1,n2]);
             gencall(['dmmul',s1(2),m1,s2(1),m2,outi,m1,m1,n1,n2])];
    stk=list([outr,outi],'-1',t2,m1,n2)
  case 2 then
    [outr,nwrk,txt]=outname(nwrk,t2,m1,n2,[s1,s2])
    [outi,nwrk,txt]=outname(nwrk,t2,m1,n2,[s1,s2])
    txt=[txt;gencall(['dmmul',s1(1),m1,s2(1),m2,outr,m1,m1,n1,n2]);
             gencall(['dmmul',s1(1),m1,s2(2),m2,outi,m1,m1,n1,n2])];
    stk=list([outr,outi],'-1',t2,m1,n2)
  case 3 then
    [outr,nwrk,txt]=outname(nwrk,t2,m1,n2,[s1,s2])
    [outi,nwrk,txt]=outname(nwrk,t2,m1,n2,[s1,s2])
    txt=[txt;
         gencall(['wmmul',s1(1),s1(2),m1,s2(1),s2(2),m2,outr,outi,..
                                                            m1,m1,n1,n2])]
    stk=list([outr,outi],'-1',t2,m1,n2)
  end
  txt=[txt1;txt]
  top=top-1
  return
endfunction
