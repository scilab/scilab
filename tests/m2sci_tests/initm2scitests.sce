mode(-1);
// Copyright INRIA
// This file get all needed functions so that M2SCI tests can be done

// Get the absolute path
[units,typs,nams]=file();
clear units typs
for k=size(nams,"*"):-1:1
  l=strindex(nams(k),"initm2scitests.sce");
  if l<>[] then
    DIR=part(nams(k),1:l($)-1);
    break
  end
end

// Set variables
GEN_FILES_DIR=DIR+"generated_files/"
WRI_FILES_DIR=DIR+"written_files/"
PROC_DIR=DIR+"proc/scilab/"

// Load test functions
getf(PROC_DIR+"sci_Unknown_Dims.sci")
getf(PROC_DIR+"sci_Unknown_Type.sci")
getf(PROC_DIR+"Unknown_Dims.sci")
getf(PROC_DIR+"Unknown_Type.sci")
getf(PROC_DIR+"type_infos.sci")
getf(PROC_DIR+"validref.sci")
getf(PROC_DIR+"testgenfiles.sci")
getf(PROC_DIR+"verifinfos.sci")
getf(PROC_DIR+"rescomp.sci")

getf(PROC_DIR+"testwrittenfiles.sci")

stacksize(40000000);