function [helppart,txt,batch]=m2sci_syntax(txt)
// Make minor changes on syntax to have it readable by scilab
// if m-file contains only returned txt is []
// batch boolean variable indicates if it is a batch file
quote=''''
dquote=""""
ctm='.'+'.'+'.'
batch=%t
//
k=0
first_ncl=[]
n=size(txt,'r')
while k<size(txt,'r')
  k=k+1
  kc=strindex(txt(k),ctm)
  isacontline=%f
  for kck=kc,
    kc1=isacomment(txt(k))
    if ~isinstring(txt(k),kck) then 
      if kc1<>0 then // line has a comment
	if kc1<kck then // ... in a comment
	else // comment follow continuation mark
	  com=part(txt(k),kc1(1):length(txt(k)))
	  txt(k)=part(txt(k),1:kck-1)+txt(k+1)+com
	  txt(k+1)=[]
	  k=k-1
	  break
	end
      else
	txt(k)=part(txt(k),1:kck-1)+txt(k+1)
	txt(k+1)=[]
	k=k-1
	break
      end
    end
  end
end

// change comments
n=size(txt,'r')
first=%t
helppart=[],endofhelp=%f
for k=1:n
  tk=txt(k)
  // insert blank when a digit is followed by a dotted operator
  kdot=strindex(tk,['.*','./','.\','.^','.'''])
  if kdot<>[] then
    kdgt=kdot(find(abs(str2code(part(tk,kdot-1)))<9))
    for kk=size(kdgt,'*'):-1:1
      tk=part(tk,1:kdgt(kk)-1)+' '+part(tk,kdgt(kk):length(tk));
    end
  end
  // looking at comments 
  kc=isacomment(tk)
  if kc<>0 then
    // a comment, replace it by a function call
    com=part(tk,kc+1:length(tk))
    if ~endofhelp then helppart=[helppart;com];end
    if length(com)==0 then com=' ',end
    com=strsubst(com,quote,quote+quote)
    com=strsubst(com,dquote,dquote+dquote)
    com=';comment('+quote+com+quote+')'
    txt(k)=part(tk,1:kc-1)+com
  else // current line is not a comment line
    if first then //function keyword not yet found
      tk=stripblanks(tk)
      if tk<>'' then //current line is not a blank line
	if part(tk,1:9) ~= 'function ' then
	  endofhelp=%t
	else
	  first_ncl=k,first=%f, 
	end
      else
	if ~endofhelp then helppart=[helppart;' '],end
	txt(k)='comment('+quote+' '+quote+')'
      end
    else //current line is not a comment line after function
      endofhelp=%t
    end
  end
end    
if ~endofhelp then
  txt=[]
  return
end
// replace ' by '' 
txt=strsubst(txt,dquote,dquote+dquote)

// replace switch by select
txt=strsubst(txt,'switch ','select ')
// replace otherwise by else
txt=strsubst(txt,'otherwise','else')
// 1i ,...
// replace {..} by (..) or [..]
//txt=replace_brackets(txt)

txt=i_notation(txt)

// place function definition line at first line
kc=strindex(txt(first_ncl),'function')
if kc==[] then
  //batch file
  txt=['function []='+fnam+'()';txt]
  batch=%t
else
  kc=kc(1)
  batch=%f
  if first_ncl<>1 then
    while strindex(txt(first_ncl($)+1),ctm)<>[] then
      first_ncl=[first_ncl,first_ncl($)+1]
    end
    txt=[txt(first_ncl);txt(1:first_ncl(1)-1);txt(first_ncl($)+1:$)]
  end
end



function txt=i_notation(txt)
// Change 1i ,... by 1*i,...
n=size(txt,1)
I='i';J='j'
matches=[string(0:9)+I(ones(1,10)),'.i',string(0:9)+J(ones(1,10)),'.j']
symbs=['+','-','*','/','\','(','[',' ','^',' ',',',';','=']
s1=['+','-','*','/','\',',',';',' ','^','.','&','|','''']
s2=[string(0:9),'d','e','D','E','.']
for k=1:n
  tk=txt(k)+' '
  kc=strindex(tk,matches)
  for kk=size(kc,'*'):-1:1
    km=kc(kk)+2
    if find(part(tk,km)==s1)==[] then kc(kk)=[],end
  end

  //  if kc<>[] then txt='!'+txt,end
  kc=[0 kc]

  for kk=size(kc,'*'):-1:2
    km=kc(kk)
    num=%t
    while or(part(tk,km)==s2)
      km=km-1
      if km<=kc(kk-1)+1 then km=kc(kk-1);num=%f;break,end
    end
    tokill=%f
    num=part(tk,km+1:kc(kk)-1)
    ke=strindex(convstr(num),['e','d'])
    kd=strindex(convstr(num),'.')
    if size(ke,2)>1|size(kd,2)>1 then
      tokill=%t
    elseif size(ke,2)==1&size(kd,2)==1 then
      if ke<kd then tokill=%t,end
    end
    if ~tokill then 
      if km<>kc(kk-1) then
	if and(part(tk,km)<>symbs) then tokill=%t,end
      end
    end
    if ~tokill then 
      km=kc(kk)
      if ~isinstring(tk,km ) then
	tk=part(tk,1:km)+'*%'+part(tk,km+1:length(tk))
      end
    end  
  end
  //if size(kc,2)>1 then txt='!'+txt,end
  txt(k)=tk
end

function i_n_test()
Txt=[
'x=1+k2i1'
'x=1.i'
'[1,2i,3;4.001i 0i 0.i]'
'1.i'
'x=2+1.d0i'
'x=33.4+a0i1+1.d+01i'
'c=x0i'
'x=2+d.0i'
'x=33.4+a0i1+1.d+01i'''
'x=''33.4+a0i1+1.d+01i'''
]
for k=1:size(Txt,1)
  txt=Txt(k)
  disp(txt+' --> '+i_notation(txt))
end
