function mat=excel2sci(fname,sep)
// Given an ascii  file created by Excel using "Text and comma" format
// exel2sci(fname) returns the corresponding Scilab matrix of strings.
// Copyright INRIA

if argn(2)<2 then sep=',',end
v=mgetl(fname)';
ns=length(sep);
mat=[];ki=1;
for line=v
  K=[strindex(line,sep)];
  ki=1;row=[];
  for k=K
    row=[row,part(line,ki:k-1)];
    ki=k+ns;
  end
  mat=[mat;[row,part(line,ki:length(line))]];
end

