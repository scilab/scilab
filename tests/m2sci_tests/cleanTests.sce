mode(-1);

// Copyright INRIA
// Scilab Project - V. Couvert
// This file remove all files generated when testing M2SCI so that tests can be replayed

disp("Cleaning test directories...")

// Get the absolute path
[units,typs,nams]=file();
clear units typs
for k=size(nams,"*"):-1:1
  l=strindex(nams(k),"cleanTests.sce");
  if l<>[] then
    DIR=part(nams(k),1:l($)-1);
    break
  end
end

// Del log
mdelete(DIR+"*.log")

// Clean generated_files/ directory
disp("Cleaning generated_files/ directory...")
mdelete(pathconvert(DIR+"generated_files/*.sci",%F))
mdelete(pathconvert(DIR+"generated_files/*.dia",%F))
mdelete(pathconvert(DIR+"generated_files/*.cat",%F))
mdelete(pathconvert(DIR+"generated_files/*.log",%F))

disp("Cleaning written_files/ directory...")
mdelete(pathconvert(DIR+"written_files/*.sci",%F))
mdelete(pathconvert(DIR+"written_files/*.cat",%F))
mdelete(pathconvert(DIR+"written_files/*.log",%F))

disp("All directories clean...")

quit
