function txt=stripblanks(txt,tabs)
// stripblanks - strips leading and trailing blanks of strings
//%SYNTAX
// txt=stripblanks(txt)
//%PARAMETERS
// txt : string or matrix of strings
//%DESCRIPTION
// stripblanks strips leading and trailing blanks of strings
//!
// Copyright INRIA

if argn(2)==1 then // Compatibility with old version
  tabs=0;
end

if tabs then
  symbs=[" ",ascii(9)];
else
  symbs=" ";
end

[m,n]=size(txt)
for l=1:m
  for k=1:n
    t=txt(l,k)
    l2=length(t)
    while l2>0 then
      if or(part(t,l2)==symbs) then l2=l2-1,else break,end //trailing blanks
    end
    l1=1
    while l1<l2 then
      if or(part(t,l1)==symbs) then l1=l1+1,else break,end //leading blanks
    end
    txt(l,k)=part(t,l1:l2)
  end
end
endfunction
