function r=linetype(str)
// Copyright INRIA
mltyp=['.','o','x','+','-','*',':','-.','--']
sltyp=[0  ,9  ,2,   1,  -1  3   -1  -1   -1]
k=find(str==mltyp)
if k<>[] then
  r=sltyp(k)
else
  r=-1
end
