function [passed]=validref(funname)
// Copyright INRIA
// Function used to validate reference files for M2SCI test
// Scilab results are compared with Matlab ones and M2SCI inference is also verified
// Input: funname : name of function to validate
// V.C.
// NB : This file create a log file called <funname>_valid.log in generated_files/valid_ref_logs/

format(16)

// Directory to write log files
VALID_REF_DIR=GEN_FILES_DIR+"valid_ref_logs/"

// Read all data files
varslistfilename=GEN_FILES_DIR+funname+"_varslist.dia.ref"
scireffilename=GEN_FILES_DIR+funname+".dia.ref"
mreffilename=GEN_FILES_DIR+funname+"_ref.sci"

varslistfile=mopen(varslistfilename,"r")
scireffile=mopen(scireffilename,"r")
mreffile=mopen(mreffilename,"r")

varslist=mgetl(varslistfile)
sciref=mgetl(scireffile)
mref=mgetl(mreffile)

mclose("all")

// Return value set to %T
passed=%T
// Data to be written in log file
logfiletxt=[]
logfiletxt=[logfiletxt;"****** Beginning of validation of "+funname+".dia.ref ******"]
// Kill useless lines in sciref
lind=1
while lind<=size(sciref,"*")
  sciline=sciref(lind)
  if isempty(strindex(sciline,["TEST","res"])) then
    sciref(lind)=[]
  else
    sciref(lind)=strsubst(sciref(lind),"res","scires")
    lind=lind+1
  end
end

// Kill useless lines in mref
lind=1
while lind<=size(mref,"*")
  mline=mref(lind)
  if isempty(strindex(mline,["TEST","res"])) then
    mref(lind)=[]
  else
    if lind<size(mref,"*")-1 then
      while isempty(strindex(mref(lind+1),["TEST","res"]))
	mline=mline+";"+mref(lind+1)
	mref(lind+1)=[]
      end
    end
    mref(lind)=strsubst(mline,"res","mres")
    lind=lind+1
  end
end

// Kill useless lines in varslist
lind=1
while lind<=size(varslist,"*")
  varslistline=varslist(lind)
  if isempty(strindex(varslistline,"res")) then
    varslist(lind)=[]
  else
    lind=lind+1
  end
end

// Verify that sizes match
if size(sciref,"*")<>size(mref,"*") then
  error("Scilab and Matlab reference do not have the same size...")
end

lind=1
resind=1
while lind<size(sciref,"*") // there must be a TEST line an INFOS line and a res line after
  res_match=%T
  testnbline=sciref(lind)
  if isempty(strindex(sciref(lind),"TEST")) then
    break
  end
  lind=lind+1
  
  teststr=stripblanks(strsubst(testnbline,"/",""))
  
  while isempty(strindex(sciref(lind),"TEST"))

    if strindex(sciref(lind),"scires"+string(resind))==[] then // can not find variable name on this line
      if strindex(sciref(lind-2),"scires"+string(resind))==[] then // can not find variable name on line+2
	if strindex(sciref(lind+2),"scires"+string(resind))==[] then // can not find variable name on line-2
	  error("Could not find variable: scires"+string(resind))
	else
	  sciinfosline=sciref(lind+2)
	end
      else
	sciinfosline=sciref(lind-2)
      end
    else
      sciinfosline=sciref(lind)
    end
    //sciinfosline=sciref(lind)
    matinfosline=mref(lind)
    varslistline=varslist(1);varslist(1)=[];
    lind=lind+1;

    [infos_match,veriftxt]=verifinfos(teststr,sciinfosline,matinfosline,varslistline)
    logfiletxt=[logfiletxt;veriftxt];
    
    sciresline=sciref(lind)
    mresline=mref(lind)
    
    execstr(sciresline)
    execstr(mresline)

    if execstr("execstr(""scires=scires""+string(resind))","errcatch")<>0 then // can not find variable name
      if execstr(sciref(lind+2),"errcatch")==0 then // try to execute following sciref line
	if execstr("execstr(""scires=scires""+string(resind))","errcatch")<>0 then // can not find variable name
	  if execstr(sciref(lind-2),"errcatch")==0 then // try to execute sciref line before
	    if execstr("execstr(""scires=scires""+string(resind))","errcatch")<>0 then // can not find variable name
	      error("Could not find variable: scires"+string(resind))
	    end	  
	  end
	end
      end
    end
    lind=lind+1;
	  
    //scires=eval("scires"+string(resind))
    mres=eval("mres"+string(resind))
    clear("scires"+string(resind))
    clear("mres"+string(resind))
    // Results comparison is not done if dimensions are type... do not match
    if infos_match then
      tmp_res_match=rescomp(scires,mres);
      if ~tmp_res_match then
	mess=[teststr+": Scilab and Matlab values do not match for res"+string(resind);
	    "Scilab value: "+strcat(strsubst(sci2exp(scires),"..",""));
	    "Matlab value: "+strcat(strsubst(sci2exp(mres),"..",""))]
	logfiletxt=[logfiletxt;mess]
	res_match=%F    
      else
	res_match=%T    
      end
    else    
      logfiletxt=[logfiletxt;"Trying to compare results..."]
      ierr=execstr("res_match=rescomp(scires,mres);","errcatch")
      if ierr<>0 | ~res_match then
	mess=[teststr+": Scilab and Matlab values do not match for res"+string(resind);
	    "Scilab value: "+strcat(strsubst(sci2exp(scires),"..",""));
	    "Matlab value: "+strcat(strsubst(sci2exp(mres),"..",""))]
	logfiletxt=[logfiletxt;mess]
      end
      res_match=%F
    end
    resind=resind+1;
  
    if lind>=size(sciref,"*") then
      break
    end
  end
      
    if infos_match & res_match then
      logfiletxt=[logfiletxt;"*** "+teststr+" PASSED ***";""]
    else  
      logfiletxt=[logfiletxt;"*** "+teststr+" FAILED ***";""]
      passed=%F
    end
    

end

logfiletxt=[logfiletxt;"****** End of validation of "+funname+".dia.ref ******"]

// Write log file
logfilename=VALID_REF_DIR+funname+"_valid.log"
logfile=mopen(logfilename,"w")
mputl(logfiletxt,logfile)
mclose("all")

endfunction


