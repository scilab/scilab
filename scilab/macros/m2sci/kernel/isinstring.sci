function r=isinstring(str,pos)
// Copyright INRIA

// Finds if the character pointed by pos is in a string (return %T) or not (return %F)

str=part(str,1:pos-1)
quote="''"

// If no quote in before pos
if strindex(str,quote)==[] then
  r=%F
  return
end


ksym=0
strcnt=0
qcount=0 // Quote counter
bcount=0 // Bracket counter
pcount=0 // Paranthesis counter
sym=' '

while %T ,
  while %T then // Looking for next non white character
    if ksym>=pos then
      r=strcnt<>0;
      return
    end
    ksym=ksym+1;
    psym=sym; // psym = previous symbol
    sym=part(str,ksym);
    if sym<>' ' then break,end
  end
  
  if strcnt<>0 then // If in a string
    if sym==quote then
      qcount=1
      while part(str,ksym+1)<>quote&ksym+1<pos then // Searching for quote or end of string
	ksym=ksym+1
      end
      
      // 5 following lines added 25/02/2003 (it seemed to work before...)
//      if ksym+1<pos then // Added by VC 17/03/2003 because didn't work for following string 'a'.^[1+1i] (pb in i_notation())
//	qcount=qcount+1
//      end // Added by VC 17/03/2003
      while part(str,ksym+1)==quote&ksym+1<pos then // Searching for a sequence of quotes
	ksym=ksym+1
	qcount=qcount+1
      end
      // End of part added 25/02/2003

      if 2*int(qcount/2)<>qcount then 
	strcnt=0
	sym=part(str,ksym)
      end
    end
  elseif sym==quote then
    // Check if transpose or beginning of a string 
    if abs(str2code(psym))>=36&psym<>')'&psym<>']'&psym<>'.'&psym<>quote then // Not a transpose
      strcnt=1
    elseif bcount<>0 then // Inside a matrix definition
      if part(str,ksym-1)==' ' then strcnt=1,end
    end
  elseif sym=='[' then
    bcount=bcount+1
  elseif sym==']' then
    bcount=bcount-1
  end
end
endfunction
