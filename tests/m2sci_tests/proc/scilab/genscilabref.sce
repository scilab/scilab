mode(-1)
// Copyright INRIA
// This batch file generates reference files for M2SCI tests

// Get the absolute path
[units,typs,nams]=file();
clear units typs
for k=size(nams,"*"):-1:1
  l=strindex(nams(k),"genscilabref.sce");
  if l<>[] then
    DIR=part(nams(k),1:l($)-1);
    break
  end
end

// Init test
exec(pathconvert(DIR+"../../initm2scitests.sce",%F))

// Make reference files for Scilab
testgenfiles("all","ref")

