mode(-1)
// Copyright INRIA
// This file launch all tests for M2SCI

// Get the absolute path
[units,typs,nams]=file();
clear units typs
for k=size(nams,"*"):-1:1
  l=strindex(nams(k),"m2sci.sce");
  if l<>[] then
    DIR=part(nams(k),1:l($)-1);
    break
  end
end
disp(getdate())
// Init tests
write(%io(2),["**********";
              "INIT TESTS";
	      "**********"])
exec(DIR+"initm2scitests.sce")
write(%io(2),["*********************";
              "CLEAN ALL DIRECTORIES";
	      "*********************"])
exec(DIR+"clean.sce")

// Exec all generated tests
write(%io(2),["****************************";
              "BEGINNING OF GENERATED TESTS";
	      "****************************"])
testgenfiles()
write(%io(2),["****************************";
              "   END OF GENERATED TESTS   ";
	      "****************************"])
disp(getdate())

// Exec all written tests
write(%io(2),["**************************";
              "BEGINNING OF WRITTEN TESTS";
	      "**************************"])
testwrittenfiles()
write(%io(2),["**************************";
              "   END OF WRITTEN TESTS   ";
	      "**************************"])

disp(getdate())
