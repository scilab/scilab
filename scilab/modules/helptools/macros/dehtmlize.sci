function formatted=dehtmlize(filename)
//author: Enrico Segre
//no rocket science
//no table formatting yet
   [x,ierr]=fileinfo(filename)
   fsize=x(1)
   fd=mopen(filename,"r")
   htmltext=mgetstr(fsize,fd)
   mclose(fd)
   if MSDOS then
    CR=strcat(ascii([13,10]))
   else
    CR=ascii(10)
   end
   CR1=strcat(ascii([10]))
//collapses lines (which is wrong for <pre>)   
   asciitext=strsubst(htmltext,CR,"")
//space paragraphs
   asciitext=strsubst(asciitext,"<p>",CR1)
   asciitext=strsubst(asciitext,"<h3>",CR1)
   asciitext=strsubst(asciitext,"<dd>"," "+CR1)
   asciitext=strsubst(asciitext,"<li>",CR1)
   asciitext=strsubst(asciitext,"<pre>"," "+CR1) 
   asciitext=strsubst(asciitext,"<tr",CR1+"<tr")
   asciitext=strsubst(asciitext,"<td>"," | ")
//   asciitext=strsubst(asciitext,"</td>"," |")
   asciitext=strsubst(asciitext,"<center>"," ")
   asciitext=strsubst(asciitext,"</center>"," ")
//remove all tags
   otag=strindex(asciitext,"<")
   ctag=strindex(asciitext,">")
   dtag=[1:otag(1)-1]
   for i=1:length(otag)-1
     dtag=[dtag,(ctag(i)+1):(otag(i+1)-1)]
   end
   asciitext=part(asciitext,dtag)
//multiple space
   asciitext=strsubst(asciitext,ascii(9)," ")
   s2=length(asciitext); s1=s2-1
   while s2>s1
    asciitext=strsubst(asciitext,"  "," ")
    s2=s1; s1=length(asciitext);
   end
// ascii characters   
   asciitext=strsubst(asciitext,"&lt;","<")
   asciitext=strsubst(asciitext,"&gt;",">")
   asciitext=strsubst(asciitext,"&nbsp;"," ")
//split paragraphs
   asciitext=tokens(asciitext,CR1)
//reformat paragraphs adding an indentation
   formatted=[]; ncl=lines()
   for i=1:size(asciitext,1)
     lindent=""; a=asciitext(i)
     t=tokenpos(a," ")
     if t(1)>1 then s=part(a,1:t(1)-1); else s=""; end
     formatted=[formatted;linesplit(s+stripblanks(a),lindent,ncl(1));""]
   end
endfunction

//this comes from my msgdiff.sci (to reformat scipad message files)
function s=linesplit(longstring,lindent,maxline)
  n=length(longstring)
  d=length(lindent)
  if ~exists("maxline","local") then maxline=80; end
  if n+2<maxline then 
    s=longstring;
    return;
  else
    p=tokenpos(longstring," ");
    i=1;j=0;m=1
    while j<n
      if j==0 then 
        k=find((p(:,2)+2-j+1)<maxline & p(:,1)>j)
      else
        k=find((p(:,2)+d+2-j+1)<maxline & p(:,1)>j)
      end
      if k==[] then
        m=m+1
        if m<size(p,1) then q=p(m+1,1)-2; else q=p(m,2); end
        s(i)=part(longstring,j:q-1) 
      else
        if k($)<size(p,1) then q=p(k($)+1,1)-2; else q=p(k($),2); end
        s(i)=part(longstring,(j+1):q)
        m=k($)
      end
      if i>1 then s(i)=lindent+s(i); end
      j=q
      if j<n then s(i)=s(i)+""; end
      i=i+1;
    end
  end
endfunction
