function k=isacomment(txt)
// find if txt contains a matlab comment 
// if no return 0 if yes return the position of the begining of the comment

// Copyright INRIA
kc=strindex(txt,'%')
k=0
if kc<>[] then
  kq=strindex(txt,quote)
  while %t then
    qc=size(find(kq<kc(1)),2)
    if modulo(qc,2)==0 then 
      k=kc(1)
      break,
    else //there is a single quote before %
      //check for the beginning of the string
      while qc>2 then
	if (kq(qc-1)==kq(qc)-1) then 
	  qc=qc-2,
	else
	  break
	end
      end
      kk=kq(qc)
      prev= part(txt,kk-1)
      if prev==' '|prev==','|prev==';'|prev=='='|prev=='['|prev=='(' then
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
