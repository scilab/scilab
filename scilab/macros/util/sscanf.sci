function [v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15,v16,..
v17,v18,v19,v20,v21,v22,v23,v24,v25,v26,v27,v28,v29,v30]=sscanf(buf,frmt)
// sscanf - Emulator of C language sscanf
// Copyright INRIA
//!
[lhs,rhs]=argn(0)
hexdigits=[string(0:9),'a','b','c','d','e','f','A','B','C','D','E','F']
hexvalues=[0:15,10,11,12,13,14,15]
kbuf=1

sbuf=length(buf)
nv=lhs
if type(frmt)<>10 then
  error('argument must be a character string')
end
if prod(size(frmt))<>1 then
  error('argument must be a character string')
end
lb=1
lfmt=length(frmt)
il=0
count=0 // argument counter
while il<=lfmt do
  [str,flags,width,typemod,conv,err]=next_scanf_format()
  if err==1 then
    error('incorrect format')
  end
  str=strcat(str)
  ns=length(str)
  if part(buf,lb:lb+ns-1)<>str then
    error('scanf : invalid conversion')
  end
  lb=lb+ns  
  if conv<>[] then
    [v,nc,err]=next_scanf_value(part(buf,lb:sbuf),str,flags,width,typemod,conv)
    if err==1 then
      error('scanf: end of input reached before final conversion')
    elseif err==2 then
      error('scanf: invalid conversion')
    end      
    if v<>[] then
      count=count+1
      execstr('v'+string(count)+'=v')
    end
    lb=lb+nc-1
  end
end
for k=count+1:lhs
   execstr('v'+string(k)+'=[]')
end  

endfunction
function [v,lb,err]=next_scanf_value(buf,str,flags,width,typemod,conv)
intf=['d','i','o','u','x','X']
floatf=['f','e','E','g','G']
strf=['c','s']
lbuf=length(buf)
if width==[]|width==0 then width=length(buf),end
err=0
cc=convstr(conv,'l')
lb=1
x=emptystr()
if cc<>'c' then //skip initial blanks
  
  while %t do
    ch=part(buf,lb)
    if ch==' '&lb<=lbuf then
      lb=lb+1
    elseif ch=='\'&part(buf,lb+1)=='n' then
      lb=lb+2
    else
      break
    end
  end
  if lb>lbuf then 
    err=1
    return
  end
end
if cc=='d'|cc=='i'|cc=='u' then
  ll=0
  ch=part(buf,lb)
  if ch=='+'|ch=='-' then
    if cc=='u'&ch=='-' then
      err=2;v=[]
      return
    else
      x=x+ch
      lb=lb+1
      ll=ll+1
    end
  end
  while isdigit(part(buf,lb))&ll<width&lb<=lbuf do
    x=x+part(buf,lb)
    lb=lb+1
    ll=ll+1
  end
  if length(x)>0 then
    v=evstr(x)
  else
    v=[]
  end
elseif cc=='o' then
  ll=0
  v=0
  while isdigit(part(buf,lb))&ll<width&lb<=lbuf do
    v=8*v+evstr(part(buf,lb))
    lb=lb+1
    ll=ll+1
  end
elseif cc=='x' then
  ll=0
  v=0
  c=part(buf,lb)
  k=find(c==hexdigits)
  while k<>[]&ll<width&lb<=lbuf do
    v=16*v+hexvalues(k)
    lb=lb+1
    ll=ll+1
    c=part(buf,lb)
    k=find(c==hexdigits)
  end
elseif cc=='f'|cc=='e'|cc=='g' then
  ll=0
  ch=part(buf,lb)
  if ch=='+'|ch=='-' then
    x=x+ch
    lb=lb+1
    ll=ll+1
  end
  if part(buf,lb:min(lb+2,lb+width-ll))=='INF' then
    v=%inf
    lb=lb+3
  elseif part(buf,lb:min(lb+2,lb+width-ll))=='NaN'
    v=%nan
    lb=lb+3
  else
    while isdigit(part(buf,lb))&ll<width&lb<=lbuf do
      x=x+part(buf,lb)
      lb=lb+1
      ll=ll+1
    end
    ch=part(buf,lb)
    if ch=='.'&ll<width&lb<=lbuf then
      x=x+'.'
      lb=lb+1
      ll=ll+1
      while isdigit(part(buf,lb))&ll<width&lb<=lbuf do
        x=x+part(buf,lb)
        lb=lb+1
        ll=ll+1
      end
    end
    ch=part(buf,lb)
    if (ch=='e'|ch=='E')&ll<width&lb<=lbuf then
      x=x+'e'
      lb=lb+1
      ll=ll+1
      ch=part(buf,lb)
      if (ch=='+'|ch=='-')&ll<width&lb<=lbuf then
        x=x+ch
        lb=lb+1
        ll=ll+1
      end
      while isdigit(part(buf,lb))&ll<width&lb<=lbuf do
        x=x+part(buf,lb)
        lb=lb+1
        ll=ll+1
      end
    end
    if length(x)>0 then
      v=evstr(x)
    else
      v=[]
    end
  end
elseif cc=='c' then
  if lb>lbuf then 
    err=1
    return
  end
  v=part(buf,lb)
elseif cc=='s' then
  ll=0
  ch=part(buf,lb)
  while ch<>' '&lb<=lbuf&ll<width do
    x=x+ch
    lb=lb+1
    ll=ll+1
    ch=part(buf,lb)
  end
  v=x
end      
if flags=='*' then  v=[],end

endfunction
function [str,flags,width,typemod,conv,err]=next_scanf_format()
//Scan frmt for % escapes and print out the arguments.
err=0
str=emptystr();kstr=1
width=[];prec=[],flags=[],typemod=[],conv=[]
il=il+1
if il>lfmt then [il,count]=resume(il,count),end

c=part(frmt,il);
while c<>'%' then
  if c=='\' then
    if part(frmt,il+1)=='n' then str=[str;emptystr()],kstr=kstr+1,end
    il=il+1
  else
    str(kstr)=str(kstr)+c
  end
  il=il+1
  if il>lfmt then break, end
  c=part(frmt,il);
end
if il>lfmt then [il,count]=resume(il,count),end
if part(frmt,il+1)=='%' then 
  str(kstr)=str(kstr)+'%',il=il+1
  [il,count]=resume(il,count)
end

//beginning of a format

//get flags
flags=[]
il=il+1;c=part(frmt,il)
if c=='*' then flags='*',il=il+1;c=part(frmt,il),end

width=[]
if isdigit(c) then
  // get width
  width=0
  while isdigit(c) do
    width=10*width+evstr(c)
    il=il+1;
    if il>lfmt then err=1;return,end
    c=part(frmt,il)
  end
end
// get type modifier
typemod=[]
if c=='l'| c=='L'|c=='h' then 
  typemod=c
  il=il+1;
  if il>lfmt then err=1;return,end
  c=part(frmt,il)
end

//get conversion
conv=c
[il,count]=resume(il,count)   


endfunction
function ok=isdigit(c)
ok=(c=='0'|c=='1'|c=='2'|c=='3'|c=='4'|c=='5'|c=='6'|c=='7'|c=='8'|c=='9')
endfunction
