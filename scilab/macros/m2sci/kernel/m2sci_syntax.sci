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
firstctm=[]

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
	if isempty(firstctm) then firstctm=k;end
	txt(k)=part(txt(k),1:kck-1)+txt(k+1)
	txt(k+1)=[]
	eoltoinsert=eoltoinsert+1
	if isempty(strindex(txt(k),ctm)) then // If no ctm in txt(k), insert all necessary EOL to keep nblines unchanged
	  for l=0:eoltoinsert-1
	    txt=[txt(1:firstctm-1+l);"";txt(firstctm-1+l+1:$)]
	  end
	  eoltoinsert=0
	  firstctm=[]
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
  kcom=isacomment(tk)
  if kcom<>0 then
    kop=kop(kop<kcom)
  end
  offset=0
  for l=1:size(kop,"*")
    if ~isinstring(tk,kop(l)) then
      ksym=kop(l)+offset+1
      while part(tk,ksym)==" "
	ksym=ksym+1
	if ksym>length(tk) then
	  break
	end
      end
      if part(tk,ksym)=="-" then
        endoftk=part(tk,ksym+1+offset:length(tk))
        m=min(strindex(endoftk,[ops(:,1)',",",";"]))
	if m<>[] then
	  tk=part(tk,1:ksym-1)+"("+part(tk,ksym+offset:ksym+m-1)+")"+part(tk,ksym+m:length(tk))
	else
	  tk=part(tk,1:ksym-1)+"("+part(tk,ksym:length(tk))+")"
	end
	offset=offset+2
      end
    end
  end

  // Modify expressions like 1++2, 1*+2... which become 1+2, 1*2...
  kop=strindex(tk,["+","-","*","/","\","^"])
  offset=0
  for l=1:size(kop,"*")
    ksym=kop(l)+offset+1
    while part(tk,ksym)==" "
      ksym=ksym+1
      if ksym>length(tk) then
	break
      end
    end
    if part(tk,ksym)=="+" then
      tk=part(tk,1:ksym-1)+part(tk,ksym+1:length(tk))
      offset=offset-1
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
    // A comment is replaced by a call to function comment() or m2scideclare()
    com=part(tk,kc+1:length(tk))
    if ~endofhelp then helppart=[helppart;com];end // Get help part placed at the beginning of the file
    if length(com)==0 then com=" ",end
    com=strsubst(com,quote,quote+quote)
    com=strsubst(com,dquote,dquote+dquote)
    if part(com,1:12)=="m2sciassume " | part(com,1:13)=="m2scideclare " then // User has given a clue to help translation
      if part(com,1:12)=="m2sciassume " then
	warning("m2sciassume is obsolete, used m2scideclare instead");
      end
      com=";m2scideclare("+quote+part(com,13:length(com))+quote+")"
    else
      com=";comment("+quote+com+quote+")"
    end
    tkbeg=part(tk,1:kc-1)
    
    // Short circuiting operators
    if ~isempty(strindex(tkbeg,"||")) then
      orexpr=tokens(tkbeg,"|")
      
      for kk=2:2:size(orexpr,"*")
	orexpr=[orexpr(1:kk);"%shortcircuit";orexpr(kk+1:size(orexpr,"*"))]
      end
      tkbeg=strcat(orexpr,"|")
    end
    if ~isempty(strindex(tkbeg,"&&")) then
      andexpr=tokens(tkbeg,"&")
      
      for kk=2:2:size(andexpr,"*")
	andexpr=[andexpr(1:kk);"%shortcircuit";andexpr(kk+1:size(andexpr,"*"))]
      end
      tkbeg=strcat(andexpr,"&")
    end
     
    // varargout replaced by %varargout so that code can be compiled with varargout considered as a Cell
    if isempty(strindex(tkbeg,"function")) then
      tkbeg=strsubst(tkbeg,"varargout","%varargout")
    end
    
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
    if ~isempty(strindex(tk,"||")) then
      orexpr=tokens(tk,"|")
      
      for kk=2:2:size(orexpr,"*")
	orexpr=[orexpr(1:kk);"%shortcircuit";orexpr(kk+1:size(orexpr,"*"))]
      end
      tk=strcat(orexpr,"|")
    end
    if ~isempty(strindex(tk,"&&")) then
      andexpr=tokens(tk,"&")
      
      for kk=2:2:size(andexpr,"*")
	andexpr=[andexpr(1:kk);"%shortcircuit";andexpr(kk+1:size(andexpr,"*"))]
      end
      tk=strcat(andexpr,"&")
    end
    
    // varargout replaced by %varargout so that code can be compiled with varargout considered as a Cell
    if isempty(strindex(tk,"function")) then
      tk=strsubst(tk,"varargout","%varargout")
    end

    txt(k)=tk
  
  end

end

// When there is just help line in txt
if ~endofhelp then
  txt=[] 
  return
end

// Syntax modification

// Complex variable
txt=i_notation(txt)

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
