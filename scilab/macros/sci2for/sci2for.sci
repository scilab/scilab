function txt=sci2for(fun,nam,vtps,lvtps)
//!
// Copyright INRIA
[lhs,rhs]=argn()
if type(fun)==11 then comp(fun),end
if type(fun)<>13 then 
  error('sci2for: first argument must be a scilab function'),
end
lst=modlst(macr2lst(fun))
lst(1)=nam;
count=0;level=0;nwrk=list(0,[],[],0,[],[],0,list(),1,[],[],[],[],[],[])
forparam()
//

inputs=lst(3)
macrhs=size(vtps)
vnms=[],for k=1:macrhs,vnms=[vnms;[inputs(k),inputs(k)]],end,

na=prod(size(vnms))/2
 
maclhs=prod(size(lst(2)))
kl=1
for k=lst(2),
  if find(vnms(:,1)==k)==[] then
    vnms=[vnms;[k,k]],
    na=na+1;
    if rhs==4 then // output variable type given
      vtps(na)=lvtps(kl);kl=kl+1
    else
      vtps(na)=list('?','0','0',0)
    end
  end
end
 
//stack of variables
k=1
while k<=na
  w=vtps(k);
  nw=size(w)
  if nw==3 then w(4)=0;vtps(k)=w;nw=4;end
  for iw=2:nw-1
      if find(vnms(:,1)==w(iw))==[] then
        ch=part(w(iw),1)
        if find(ch==string(0:9))==[]
          na=na+1
          vtps(na)=list('0','1','1',0)
          vnms=[vnms;[w(iw),w(iw)]]
        end
      end
  end
  k=k+1
end
 
bot=na

//
forexp=0
[crp,vnms,vtps,nwrk]=ins2for(lst,4,vnms,vtps,nwrk)
if strindex(strcat(crp),'WARNING local variable')<>[] then
  nwrk=list(0,[],[],0,[],[],0,list(),1,[],[],[],[],[],[])
//  pause
  [crp,vnms,vtps,nwrk]=ins2for(lst,4,vnms,vtps,nwrk)
end

na=size(vtps)
if find(vnms(:,1)=='%eps')<>[] then
  crp=[' %eps = d1mach(4)';'c';crp]
  vnms=[vnms;['d1mach','d1mach']]
  vtps(na+1)=list('1','1','1',0)
  na=na+1
end
 
//
//header
if nwrk(10)<>[]|nwrk(12)<>[] then 
  pntrs='c      adress of local variables',
else
  pntrs=[];
end
v1=vnms(1:bot,1)
// 
if nwrk(3)<>[]|nwrk(10)<>[] then
  if prod(size(nwrk(3)))==1 then
    used=nwrk(3)
  else
    used='max('+makeargs(nwrk(3))+')'
  end
  if nwrk(10)<>[] then
    nams=nwrk(10);loc=nwrk(11);nl=prod(size(loc))/2
    pntrs=[pntrs;' iw'+nams(1)+' = 1']
    for k=2:nl
      pntrs=[pntrs;' iw'+nams(k)+' = '+addf('iw'+nams(k-1),loc(1,k-1))];
      used=addf(used,loc(1,k-1))
    end
    pntrs=[pntrs;'c';' iw0 = '+addf('iw'+nams(nl),loc(1,nl))]
    used=addf(used,loc(1,nl))
  else
    pntrs=[pntrs;' iw0 = 1']
  end
  if ~isnum(used) then
    v1=[v1;'work'];
  end
end
// tableau de travail entier
if nwrk(6)<>[]|nwrk(12)<>[] then
  if prod(size(nwrk(6)))==1 then
    iused=nwrk(6)
  else
    iused='max('+makeargs(nwrk(6))+')'
  end
  if nwrk(12)<>[] then
    nams=nwrk(12);loc=nwrk(13);nl=prod(size(loc))/2
    pntrs=[pntrs;' iiw'+nams(1)+' = 1']
    for k=2:nl
      pntrs=[pntrs;' iiw'+nams(k)+' = '+addf('iiw'+nams(k-1),loc(1,k-1))];
     iused=addf(iused,loc(1,k-1))
    end
    pntrs=[pntrs;'c';' iiw0 = '+addf('iiw'+nams(nl),loc(1,nl))]
    iused=addf(iused,loc(1,nl))
  else
    pntrs=[pntrs;' iiw0 = 1']
  end
  if ~isnum(iused) then
    v1=[v1;'iwork'];
  end
end
//if nwrk(10)<>[]|nwrk(12)<>[] then   pntrs=[pntrs;'c'],end
// variable d'erreur
if nwrk(7)>0 then
  v1=[v1;'ierr']
end
//
kv1=0;nvc=na
for k=1:bot
  kv1=kv1+1
  vk=vtps(k);
  if vk(4)==1 then
    v1=[v1(1:k-1);v1(k)+'_r';v1(k)+'_i';v1(k+1:nvc)]
    kv1=kv1+1
    nvc=nvc+2
  end
end
 
hdr=[' subroutine '+lst(1)+'('+makeargs(v1)+')';
     'c!';
     'c automatic translation';
     'c';
     'c!     calling sequence';
     'c      ----------------';
     'c' ]
//
//declarations
argld=[];//double precision argument list
argli=[];// integer argument list
argls=[];// character argument list
locals=[]
for iv=1:na
 // var=vnms(iv,1);
  var=vnms(iv,2);
  vartyp=vtps(iv)
  if part(var,1:4)<>'work'&part(var,1:5)<>'iwork' then
    nl=vartyp(2);nc=vartyp(3);it=vartyp(4)

    if type(nl)==1 then 
      nl=string(maxi(1,nl)),
    else
      if isnum(nl) then 
	nl=string(maxi(evstr(nl),1))
      end
    end
    if type(nc)==1 then 
      nc=string(maxi(1,nc)),
    else
      if isnum(nc) then 
	nc=string(maxi(evstr(nc),1))
      end
    end
    if nl+nc=='11' then
      if it==0 then
         nm=var
      else
         nm=nm+'_r'+','+nm+'_i',
        var=var+'(_r,_i)'
      end
      com='variable '
    elseif nl=='1' then
      if it==0 then
        nm=var+'('+nc+')'
      else
        nm=var+'_r('+nc+')'+','+var+'_i('+nc+')'
        var=var+'(_r,_i)'
      end
      com='vector of size '+nc
    elseif nc=='1' then
      if it==0 then
        nm=var+'('+nl+')'
      else
        nm=var+'_r('+nl+')'+','+var+'_i('+nl+')'
        var=var+'(_r,_i)'
      end
      com='vector of size '+nl
    else
      if it==0 then
        nm=var+'('+makeargs([nl;nc])+')'
      else
        nm=var+'_r('+makeargs([nl,nc])+')'+','+var+'_i('+makeargs([nl,nc])+')'
        var=var+'(_r,_i)'
      end  
     
      com='vector of size '+nl+','+nc
    end
    if vartyp(1)=='1' then
      argld=[argld;nm]
      com='c      '+part(var,1:10)+' : double precision '+com
    elseif vartyp(1)=='0' then
      argli=[argli;nm]
      com='c      '+ part(var,1:10)+' : integer '+com
    elseif vartyp(1)=='10' then
      argls=[argls;nm]
      com='c      '+ part(var,1:10)+' : string '+com      
    end
    if iv<=bot then
      hdr=[hdr;com],
    end
  else
    locals=[locals;'c        '+ vnms(iv,2)+' : size ('+..
            vartyp(2)+','+vartyp(3)+'), implemented in '+vnms(iv,1)]
  end
end
if nwrk(3)<>[]|nwrk(10)<>[] then
  if ~isnum(used) then
    nm='work(*)';argld=[argld;nm]
    hdr=[hdr;'c      work       : working array :';
	'c                 '+used]
  else
    nm='work('+used+')';argld=[argld;nm]
    hdr=[hdr;'c      work       : working array :'+used]
  end
end
if nwrk(6)<>[]|nwrk(12)<>[] then
  if ~isnum(iused) then
    nm='iwork(*)';argli=[argli;nm]
    hdr=[hdr;'c      iwork      : working array :';
	'c                 '+iused]
  else
    nm='iwork('+iused+')';argli=[argli;nm]
    hdr=[hdr;'c      iwork      : working array :'+iused]
  end
end
if nwrk(7)>0 then
  msgs=nwrk(8)
  argli($+1)='ierr'
  hdr=[hdr;'c      ierr      : error indicator']
  hdr=[hdr;'c             0 :  correct run']
  for k=1:nwrk(7)
    msgk=msgs(k)
    for l=1:prod(size(msgk))
      hdr=[hdr;'c             '+string(k)+' : '+msgk(l)]
    end
  end
end
if nwrk(10)<>[]|nwrk(12)<>[] then
  hdr=[hdr;'c';..
           'c      dimension of local variables ';..
           'c      -----------------------------';
           locals]
end
hdr=[hdr;'c!']
//
dcl=[]
if argli<>[] then  dcl=[dcl;' integer '+makeargs(argli)],end
if argld<>[] then  dcl=[dcl;' double precision '+makeargs(argld)],end
if argls<>[] then  dcl=[dcl;' character*(*) '+makeargs(argls)],end
if nwrk(14)<>[]|nwrk(15)<>[] then dcl=[dcl;'c     external functions'],end
if nwrk(14)<>[] then  dcl=[dcl;' integer '+makeargs(nwrk(14))],end
if nwrk(15)<>[] then  dcl=[dcl;' double precision '+makeargs(nwrk(15))],end

dcl=[dcl;'c']
 
txt=[hdr;dcl;pntrs;crp;' end']
k=find(part(txt,1)==' ')
txt(k)='     '+txt(k)
 
kk=1
while kk<>[] then
  kk=find(length(txt)>72)
  nk=prod(size(kk))
  for k=nk:-1:1
    k1=kk(k)
    t1=txt(k1),
    if part(t1,1)=='c' then
      t1=[part(t1,1:72);
         'c              '+part(t1,73:length(t1))];
   else
     ksplit=max(strindex(part(t1,1:71),[',','+','-','*','/',' ']))
     if ksplit==[] then ksplit=72,end
      t1=[part(t1,1:ksplit);
         '     & '+part(t1,ksplit+1:length(t1))];
    end
    txt=[txt(1:k1-1);t1;txt(k1+1:prod(size(txt)))]
  end
end
endfunction
