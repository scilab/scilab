function [txt,trad]=m2sci(lst,nam,Imode,Recmode)
// translate matlab syntax to scilab
//%Parameters
// lst   : list, represents the interpreted code of the matlab function given by macr2lst
// nam   : nam of the scilab function to generate
// txt   : character string column vector: the text of resulting scilab function
//!
//%Main variables
// vnms  : nx2 matrix of strings, each row contains the matlab and scilab name of 
//         a refered matlab variable 
// vtps  : a list, each entry is a list(?,?,?,?)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==1 then nam=lst(1),end
if rhs<=2 then Imode=%t,end
if rhs<=3 then Recmode=%f,end
//sci_min=sci_mini
//sci_max=sci_maxi
lst=mmodlst(lst)
lcount=1;level=[0,0];
quote=''''
//
sciparam()

//stack of named  variables

// add input variable in the defined variables
inputs=lst(3)
macrhs=size(inputs,2) 
vnms=[],
vtps=list()
for k=1:macrhs
  if or(inputs(k)==killed(1)) then
    vnms=[vnms;['%'+inputs(k),inputs(k)]],
  else
    if funptr(inputs(k))<>0 then 
      vnms=[vnms;['%'+inputs(k),inputs(k)]],
      inputs(k)='%'+inputs(k),
    else
      vnms=[vnms;[inputs(k),inputs(k)]],
    end
  end
  if Imode then
    r=askfortype(inputs(k))
    if r<>[] then
      vtps($+1)=r
    else
      vtps($+1)=list('?','?','?',0)
    end
  else
    vtps($+1)=list('?','?','?',0)
  end
end,

// add predefined variable in the defined variables
if find(vnms(:,2)=='i')==[] then
  vnms=[vnms;['%i','i']],
  vtps($+1)=list('1','1','1',0)
end
if find(vnms(:,2)=='j')==[] then
  vnms=[vnms;['%i','j']],
  vtps($+1)=list('1','1','1',0)
end
vnms=[vnms;['%nan','NaN']],
vtps($+1)=list('1','1','1',0)
vnms=[vnms;['%nan','nan']],
vtps($+1)=list('1','1','1',0)
vnms=[vnms;['%inf','Inf']],
vtps($+1)=list('1','1','1',0)
vnms=[vnms;['%inf','inf']],
vtps($+1)=list('1','1','1',0)

if find(vnms(:,2)=='pi')==[] then
  vnms=[vnms;['%pi','pi']],
  vtps($+1)=list('1','1','1',0)
end
if find(vnms(:,2)=='eps')==[] then
  vnms=[vnms;['%eps','eps']],
  vtps($+1)=list('1','1','1',0)
end


outputs=lst(2)
maclhs=size(outputs,2)
for k=1:maclhs
  if funptr(outputs(k))<>0 then
    vnms=[vnms;['%'+outputs(k),outputs(k)]]
    vtps($+1)=list('?','?','?',0)
    outputs(k)='%'+outputs(k)
  end
end
bot=size(vtps)


// translate
sciexp=0
[crp,vnms,vtps]=ins2sci(lst,4,vnms,vtps)
dcl=[]

//add special code
// nargin & nargout
if find(vnms(:,1)=='nargin'|vnms(:,1)=='nargout') then
  dcl='[nargout,nargin] = argn(0)'
end
//Initial value of lhs arguments
ini=emptystr()
for k=outputs
  if find(inputs==k)==[] then ini=ini+k+'=[];',end
end
if ini==emptystr() then ini=[],end


//info on macros variables
n=size(vtps)
info=[]
for k=1:n
  m=string(vtps(k)(2))
  n=string(vtps(k)(3))
  tp=string(vtps(k)(1))
  info=[info;
      '// '+part(vnms(k,2),1:24)+' size :'+part(m+' x '+n,1:12)+' type: '+tp];
end
//write(%io(2),info,'(a)')
  

//add the function header
hdr='function '+lhsargs(outputs)+'='+nam+rhsargs(vnms(1:macrhs,1));
txt=[hdr;ini;dcl;crp(1:$-1)]


// generate associated translation function
//if nam=='script' then
//  f=fnam+'.sce'
//  trad=[
//      'function [stk,txt,top]=sci_'+fnam+'()'
//      'stk=list(''exec('''+sci2exp(f)+''')'',''0'',''?'',''?'',''?'')']
if batch then
 
  trad=[
      'function [stk,txt,top]=sci_'+fnam+'()'
      'stk=list(''exec('+fnam+')'',''0'',''?'',''?'',''?'')']
else
  trad=[
      'function [stk,txt,top]=sci_'+nam+'()'
      'RHS=[]'
      'for k=1:rhs'
      '  RHS=[stk(top)(1),RHS]'
      '  top=top-1'
      'end'
      'top=top+1']
  if maclhs==0 then
    trad=[trad;
        'stk=list('+sci2exp(nam)+'+rhsargs(RHS),''0'',''?'',''?'',''?'')']
  elseif maclhs==1 then
    k1=find(outputs(1)==vnms(:,2));
    if k1<>[] then
      k1=k1(1);
      w=strcat([sci2exp(vtps(k1)(2)),sci2exp(vtps(k1)(3)),sci2exp(vtps(k1)(1))],',')
    else
      w='''?'',''?'',''?'''
    end
    trad=[trad;
        'stk=list('+sci2exp(nam)+'+rhsargs(RHS),''0'','+w+')']
  else  
    w=[]
    for k=1:maclhs
      k1=find(outputs(k)==vnms(:,2));
      if k1<>[] then
	k1=k1(1);
	w=[w;strcat([sci2exp(vtps(k1)(2)),sci2exp(vtps(k1)(3)),sci2exp(vtps(k1)(1))],',') ] ;
      else
	w=[w;'''?'',''?'',''?''']
      end
    end  
    w(1)='w=['+w(1);w($)=w($)+']';
    trad=[trad;
        '//  w(i,1) is the ith output argument type'
        '//  w(i,2) is the ith output argument row dimension'
        '//  w(i,2) is the ith output argument column dimension'
        w]
//        sci2exp(w,'w')]
    trad=[trad;
        'if lhs==1 then'
        '  stk=list('+sci2exp(nam)+'+rhsargs(RHS),''0'',w(1,1),w(1,2),w(1,3))'
        'else'
        '  stk=list()'
        '  for k=1:lhs'
        '    stk(k)=list('+sci2exp(nam)+'+rhsargs(RHS),''-1'',w(k,1),w(k,2),w(k,3))'
        '  end'
        'end']
  end
end



