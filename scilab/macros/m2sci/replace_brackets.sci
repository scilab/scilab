function txt=replace_brackets(txt)

// Copyright INRIA
for k=1:size(txt,'r')
  tk=txt(k)
  ko=strindex(tk,'{')
  if ko<>[] then
    kf=strindex(tk,'}')
    //create matching pairs
    kw=[ko,kf]
    for kk=1:size(ko,'*')
      pchar=abs(str2code(part(tk,ko(kk)-1)))
      if pchar<36 then // extraction
  end
end
