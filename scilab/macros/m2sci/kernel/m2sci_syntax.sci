function [helppart,txt,batch]=m2sci_syntax(txt)
// Copyright INRIA
// Scilab Project - V. Couvert
// Make minor changes on M-file data syntax to have it readable by Scilab
// Input arguments:
//  - txt: the contents of an M-file
// Output:
//  - helppart: Matlab help contained in M-file
//  - txt: input txt modified (If M-file contains only comments returned txt is[])
//  - batch: boolean flag indicates if it is a batch file

// m2sci kernel functions called :
//  - isacomment
//  - isinstring
//  - replace_brackets

sciparam();

quote=''''
dquote=""""
ctm='.'+'.'+'.' // Continuation mark
batch=%t

k=0
first_ncl=[]

// Number of lines in txt (i.e. in M-file)
n=size(txt,'r')

eoltoinsert=0

while k<size(txt,'r')
  k=k+1
  kc=strindex(txt(k),ctm)
  isacontline=%f
  
  for kck=kc,
    kc1=isacomment(txt(k))
    if ~isinstring(txt(k),kck) then
      if kc1<>0 then // Line has a comment
	if kc1<kck then // Continuation mark in a comment
	else // Comment follow continuation mark
	  com=part(txt(k),kc1(1):length(txt(k)))
	  txt(k)=part(txt(k),1:kck-1)+txt(k+1)+com
	  txt(k+1)=[]
	  k=k-1
	  break
	end
      else // Not a comment line
	txt(k)=part(txt(k),1:kck-1)+txt(k+1)
	txt(k+1)=[]
	eoltoinsert=eoltoinsert+1
	if isempty(strindex(txt(k),ctm)) then // If no ctm in txt(k), insert all necessary EOL to keep nblines unchanged
	  for l=0:eoltoinsert-1
	    txt=[txt(1:k+l);"";txt(k+l+1:$)]
	  end
	  eoltoinsert=0
	end
	k=k-1
	break
      end
    end
  end
end

// Change comments and get help part in input txt
n=size(txt,'r')
first=%t
helppart=[],endofhelp=%f

for k=1:n
  tk=txt(k)
  // Parenthesize expressions like 1+-2, 1*-2... which becomes 1+(-2), 1*(-2)...
  // Parentheses are deleted by comp() and will be added one more time by %?2sci function
  kop=strindex(tk,["+","-","*","/","\","^"])
  kminus=strindex(tk,"-")
  kcom=isacomment(tk)
  if kcom<>0 then
    kminus=kminus(kminus<kcom)
  end
  offset=0
  for l=1:size(kminus,"*")
    if find(kop==kminus(l)-1)<>[] then
      endoftk=part(tk,kminus(l)+1+offset:length(tk))
      m=min(strindex(endoftk,[ops(:,1)',",",";"]))
      if m<>[] then
	tk=part(tk,1:kminus(l)+offset-1)+"("+part(tk,kminus(l)+offset:kminus(l)+offset+m-1)+")"+part(tk,kminus(l)+offset+m:length(tk))
      else
	tk=part(tk,1:kminus(l)+offset-1)+"("+part(tk,kminus(l)+offset:length(tk))+")"
      end
      offset=offset+2
    end
  end

  // Modify expressions like 1++2, 1*+2... which become 1+2, 1*2...
  kop=strindex(tk,["+","-","*","/","\","^"])
  kplus=strindex(tk,"+")
  for l=size(kplus,"*"):-1:1
    if find(kop==kplus(l)-1)<>[] then
      tk=part(tk,1:kplus(l)+offset-1)+part(tk,kplus(l)+1+offset:length(tk))
    end
  end

  
  // Insert a blank when a digit is followed by a dotted operator
  // So that point is associated to operator and not to digit
  // Because if it is associated to digit, dot is suppressed by comp()
  kdot=strindex(tk,[".*","./",".\",".^",".''"])
  if kdot<>[] then
    kdgt=kdot(find(abs(str2code(part(tk,kdot-1)))<9))
    for kk=size(kdgt,"*"):-1:1
      tk=part(tk,1:kdgt(kk)-1)+" "+part(tk,kdgt(kk):length(tk));
    end
  end
  
  // Parenthesize calls to pause when pause on or pause off
  kpause=strindex(tk,"pause") 
  kpsav=length(tk) // kpsave is kp value for l-1 index
  for l=size(kpause,"*"):-1:1
    kp=kpause(l)
    kon=strindex(tk,"on")
    kon=kon(find((kon>kp)&(kon<kpsav)))
    if kon<>[] then
      for l=kp+5:kon-1
	if part(tk,l)<>" " then
	  break
	end
      end
      tk=part(tk,1:kp+4)+"(''on'')"+part(tk,kon+2:length(tk))
    end
    koff=strindex(tk,"off")
    koff=koff(find((koff>kp)&(koff<kpsav)))
    if koff<>[] then
      for l=kp+5:koff-1
	if part(tk,l)<>" " then
	  break
	end
      end
      tk=part(tk,1:kp+4)+"(''off'')"+part(tk,koff+3:length(tk))
    end
    kpsav=kp
  end
  
  // Convert @fhandle to 'fhandle' (cf feval)
  symbs=[" ",",",";","=",")","]"]
  kpunct=strindex(tk,"@")
  kcom=isacomment(tk)
  if kcom<>0 then
    kpunct=kpunct(kpunct<kcom)
  end
  if kpunct<>[] then
    for l=size(kpunct,"*"):-1:1
      kk=gsort(strindex(tk,symbs),"r","i")
      kk=kk(find(kk>kpunct(l)))
      if kk==[] then 
	kk=length(tk)
	tk=part(tk,1:kpunct(l)-1)+quote+part(tk,kpunct(l)+1:kk)+quote
      else
	kk=kk(1)
	tk=part(tk,1:kpunct(l)-1)+quote+part(tk,kpunct(l)+1:kk-1)+quote+part(tk,kk:length(tk))
      end
    end
  end
  
  // Looking for comments
  kc=isacomment(tk)
  if kc<>0 then // Current line has or is a comment
    // A comment is replaced by a call to function comment() or m2sciassume()
    com=part(tk,kc+1:length(tk))
    if ~endofhelp then helppart=[helppart;com];end // Get help part placed at the beginning of the file
    if length(com)==0 then com=' ',end
    com=strsubst(com,quote,quote+quote)
    com=strsubst(com,dquote,dquote+dquote)
    if part(com,1:12)=='m2sciassume ' then // User has given a clue to help translation
      com=';m2sciassume('+quote+part(com,13:length(com))+quote+')'
    else
      com=';comment('+quote+com+quote+')'
    end
    tkbeg=part(tk,1:kc-1)
    
    // Short circuiting operators
    tkbeg=strsubst(tkbeg,"  ","")
    tkbeg=strsubst(tkbeg," |","|")
    tkbeg=strsubst(tkbeg,"| ","|")
    tkbeg=strsubst(tkbeg," &","&")
    tkbeg=strsubst(tkbeg,"& ","&")
    symbs=[" ",",",";","=",")","]"]
    ksymbs=gsort(strindex(tkbeg,symbs),"r","i")
    ksymbs=[ksymbs length(tkbeg)+1]
    tmptkbeg=part(tkbeg,1:ksymbs(1)-1)
    for kk=1:size(ksymbs,"*")-1
      kop1=strindex(part(tkbeg,ksymbs(kk):ksymbs(kk+1)-1),"||")
      kop2=strindex(part(tkbeg,ksymbs(kk):ksymbs(kk+1)-1),"&&")
      tmptkbeg=tmptkbeg+strsubst(strsubst(part(tkbeg,ksymbs(kk):ksymbs(kk+1)-1),"&&","&"),"||","|")
      if kop1<>[] then
	tmptkbeg=tmptkbeg+"|%shortcircuit"
      elseif kop2<>[] then
	tmptkbeg=tmptkbeg+"&%shortcircuit"
      end
    end
    tkbeg=tmptkbeg

    txt(k)=tkbeg+com
  
  else // Current line has not and is not a comment line
    if first then // Function keyword not yet found
      tk=stripblanks(tk)
      if tk<>'' then // Current line is not a blank line
	if part(tk,1:9) ~= 'function ' then
	  endofhelp=%t;
	  txt(k)=tk; // VC 01/04/2003
	else
	  first_ncl=k
	  first=%f
	end
      else
	if ~endofhelp then helppart=[helppart;' '],end
	txt(k)='comment('+quote+' '+quote+')'
      end
    else // Current line is a line after function keyword
      endofhelp=%t
      txt(k)=tk
    end
    
    // Short circuiting operators
    tk=strsubst(tk,"  ","")
    tk=strsubst(tk," |","|")
    tk=strsubst(tk,"| ","|")
    tk=strsubst(tk," &","&")
    tk=strsubst(tk,"& ","&")
    symbs=[" ",",",";","=",")","]"]
    ksymbs=gsort(strindex(tk,symbs),"r","i")
    ksymbs=[ksymbs length(tk)+1]
    tmptk=part(tk,1:ksymbs(1)-1)
    for kk=1:size(ksymbs,"*")-1
      kop1=strindex(part(tk,ksymbs(kk):ksymbs(kk+1)-1),"||")
      kop2=strindex(part(tk,ksymbs(kk):ksymbs(kk+1)-1),"&&")
      tmptk=tmptk+strsubst(strsubst(part(tk,ksymbs(kk):ksymbs(kk+1)-1),"&&","&"),"||","|")
      if kop1<>[] then
	tmptk=tmptk+"|%shortcircuit"
      elseif kop2<>[] then
	tmptk=tmptk+"&%shortcircuit"
      end
    end
    tk=tmptk

    txt(k)=tk
  
  end

end

// When there is just help line in txt
if ~endofhelp then
  txt=[] 
  return
end

// Syntax modification

// Replace double quotes
txt=strsubst(txt,dquote,dquote+dquote)

// Replace switch by select
txt=strsubst(txt,"switch ","select ")
txt=strsubst(txt,"switch(","select (")
    
// Replace otherwise by else
txt=strsubst(txt,"otherwise","else")

// Replace {..} by (..) or [..] : useful for cells translation
txt=replace_brackets(txt)
  
// Place function definition line at first line
kc=strindex(txt(first_ncl),"function")
if kc==[] then
  // Batch file
  txt=["function []="+fnam+"()";txt] // fnam is defined in mfile2sci() 
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
endfunction
