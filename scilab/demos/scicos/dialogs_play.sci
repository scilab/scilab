function rep=x_message(comment,btns)
// Copyright INRIA
  rep=[]
  [lhs,rhs]=argn(0)
  comment=matrix(comment,prod(size(comment)),1)
  if rhs==1 then
  else
    if size(btns,'*')==1
    else
      str=readline()	
      rep=find(str==btns)
    end
  end
endfunction

function str=x_dialog(comment,default)
// Copyright INRIA
  if argn(2)==1 then default=' ',end
  default=default(1) // for lmitool
  str=[]
  while %t do
    s=readline()	
    if part(s,1)=='o'|part(s,1)=='c' then break,end
    str=[str;s]
  end
  if str==' ' then str=default,end
  rep=s
  if part(rep,1)=='c' then str=[],end
endfunction

function str=x_mdialog(description,labels,valuesini)
// Copyright INRIA
  n=prod(size(labels))
  if argn(2)==2 then valuesini(n)=' ',end
  for k=1:n
    str(k,1)=readline()
    if str(k)==' ' then str(k)=valuesini(k),end
  end
  rep=readline()
  if part(rep,1)=='c' then str=[],end
endfunction


function num=x_choose(tochoose,comment,button)
// Copyright INRIA
  num=-1
  while num<0|num>size(tochoose,'*') then
    num=evstr(readline())
  end
endfunction


function [btn,xc,yc,win,Cmenu]=xclick();
// Copyright INRIA  
  str=readline()
  rep=evstr('list('+str+')')
  btn=rep(1)
  xc=rep(2)
  yc=rep(3)
  mrk=xget('mark')
  xset('mark',0,5)
  plot2d(xc,yc,-1,'000')
  plot2d(xc,yc,-1,'000')
  xset('mark',mrk(1),mrk(2))
  win=0
  if size(rep)>=4 then 
    win=rep(4),
  end
  if size(rep)>=5 then 
    Cmenu=rep(5)
  else
    Cmenu=[]
  end
endfunction

function rep=xgetmouse(flag);
// Copyright INRIA 
  str=readline()
  rep=evstr('['+str+']')
  // next line for animation
  xpolys(rep(1),rep(2),-1);xpause(10000);xpolys(rep(1),rep(2),-1);
endfunction


function str=readline()
// Copyright INRIA 
  global LineCount
  LineCount=LineCount+1
  comm='//'
  blank=' '
  while %t do
    rep=mgetl(%I,1)
    k=strindex(rep,comm)
    if k<>[] then
      k=k(1)
      if part(rep,1:k-1)<>part(blank,1:k-1) then 
	str=stripblanks(part(rep,1:k-1))
	com=part(rep,k+1:length(rep))
	if part(str,1:3)=='-->' then
	  execstr(part(str,4:length(str)))
	else
	  break
	end
      end
    else
      n=length(rep)
      str=stripblanks(rep)
      com=emptystr()
      if part(str,1:3)=='-->' then
	execstr(part(str,4:length(str)))
      else
	break
      end
    end
  end
  bl='=';txt=com+':'+str;txt=part(bl,ones(1,60-length(txt)))+txt
endfunction

function p=xgetfile(file_mask,dir,title)
  p=readline()
endfunction


function c=getcolor(title,cini)
// Copyright INRIA  
  str=readline()
  c=evstr('['+str+']')
endfunction

function rep=message(comment,btns)
// Copyright INRIA
  if argn(2)==1 then
    rep=x_message(comment)
  else
    rep=x_message(comment,btns)
  end
endfunction

function rep=dialog(labels,valueini) 
// Copyright INRIA
  if argn(2)==1 then
    rep=x_dialog(labels) 
  else
    rep=x_dialog(labels,valueini) 
  end
endfunction
