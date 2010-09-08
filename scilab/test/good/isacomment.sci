// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function k=isacomment(txt)
// Find if txt contains a Matlab comment
// If no return 0
// If yes return the position of the beginning of the comment

kc=strindex(txt,"%")
k=0
if kc<>[] then
  kq=strindex(txt,quote)
  while %t then
    qc=size(find(kq<kc(1)),2) // qc = nb of quote placed before the first %
    // Even number of quote before first %
    if modulo(qc,2)==0 then
      k=kc(1)
      break
    else // There is a single quote before %, have to check for the beginning of the string
      while qc>2 then
	if (kq(qc-1)==kq(qc)-1) then // Found a '' sequence
	  qc=qc-2
	else
	  break
	end
      end
      kk=kq(qc)
      if execstr("prev = part(txt,kk-1)","errcatch")<>0 then pause,end;errclear();
      if or(prev==[" ",",",";","=","[","("]) then // qc points on a string beginning
	kc(1)=[]
	if kc==[] then break,end
      else
	k=kc(1)
	break
      end
    end
  end
end
endfunction
