function []=testwrittenfiles(test_files,opt)
// Copyright INRIA
// Function which can generate:
// - reference files for M2SCI tests
// - diary files for M2SCI tests
// Input description:
// - test_files: names of files to test ('all' for all files in written_files/ directory)
// - opt: 'ref' if we want to build reference files

ieee(1)

// Default arguments
rhs=argn(2);
// If no file WRI_FILES_DIR is given, all files of this directory are created
if rhs<1 then
  test_files=[]
else
  if test_files=="all" then
    test_files=[]
  else
    test_files=WRI_FILES_DIR+test_files
  end
end

if isempty(test_files) then
  test_files=listfiles(WRI_FILES_DIR+"*.m");
  test_files=gsort(test_files,"r","i");
end

// If option is not given, test is made not ref
if rhs<2 then
  opt=""
end

// For all files listed, a .dia or a .dia.ref file is created
for file_ind=1:size(test_files,1)
  
  // Convert test file (ex: ABS.m)
  if opt=="ref" then
    write(%io(2),"****** Beginning of generation of "+basename(test_files(file_ind))+".sci.ref file  ******")
  else
    write(%io(2),"****** Beginning of test of "+basename(test_files(file_ind))+" ******")
  end
  write(%io(2),"Conversion of "+basename(test_files(file_ind))+".m file...")
  mfile2sci(test_files(file_ind),WRI_FILES_DIR,%F,%F,3,%T)  

  funname=basename(test_files(file_ind))
  
  if opt=="ref" then
    // Now Scilab reference file is created, it has to be validated
    write(%io(2),"Validation of "+funname+".sci file...")
    scifile=mopen(WRI_FILES_DIR+funname+".sci")
    txt=mgetl(scifile)
    mclose(scifile)
    write(%io(2),txt)
    
    valid=input("Validate reference (y/n) ?","string")
    
    if valid=="y" then
      write(%io(2),"---> Validation of "+funname+".sci: OK !")
      if MSDOS then
	unix("move /Y "+WRI_FILES_DIR+funname+".sci "+WRI_FILES_DIR+funname+".sci.ref")
      else
	unix("mv -f "+WRI_FILES_DIR+funname+".sci "+WRI_FILES_DIR+funname+".sci.ref")
      end
    else
      mdelete(WRI_FILES_DIR+funname+".sci")
      write(%io(2),"---> Validation of "+funname+".sci: KO !")
    end
    mdelete(WRI_FILES_DIR+"sci_*.*")
    mdelete(WRI_FILES_DIR+"*.cat")
      
  else
    write(%io(2),"Comparison between "+funname+".sci.ref and "+funname+".sci file...")
    DIA=mopen(WRI_FILES_DIR+funname+".sci","r")
    DIATXT=mgetl(DIA,-1)
    mclose(DIA)
    
    DIAREF=mopen(WRI_FILES_DIR+funname+".sci.ref","r")
    DIAREFTXT=mgetl(DIAREF,-1)
    mclose(DIAREF)
    
    // Remove Matlab version
    for ktxt=1:size(DIATXT,"*")
      if ~isempty(strindex(DIATXT(ktxt),"version")) then
	DIATXT(ktxt:ktxt+1)=[]
	break
      end
    end
	
    // Remove Matlab version
    for ktxt=1:size(DIAREFTXT,"*")
      if ~isempty(strindex(DIAREFTXT(ktxt),"version")) then
	DIAREFTXT(ktxt:ktxt+1)=[]
	break
      end
    end
    
    if or(DIATXT<>DIAREFTXT) then
      write(%io(2),"---> Test for "+funname+": FAILED !")
    else
      write(%io(2),"---> Test for "+funname+": PASSED !")
    end
  end
  
  if opt=="ref" then
    write(%io(2),["****** End of generation of "+basename(test_files(file_ind))+".sci.ref file ******";""])
  else
    write(%io(2),["****** End of test of "+basename(test_files(file_ind))+" ******";""])
  end
end

endfunction

