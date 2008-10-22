function [passed]=valid_dia(funname)
// Copyright INRIA
// Function used to validate reference files for M2SCI test
// The result of Scilab file .dia are compared with Scilab file dia.ref
// Input: funname : name of function to validate
// F.B.

format(16)

// Read all data files

scireffilename=GEN_FILES_DIR+funname+".dia"
mreffilename=GEN_FILES_DIR+funname+".dia.ref"

scireffile=mopen(scireffilename,"r")
mreffile=mopen(mreffilename,"r")

sciref=mgetl(scireffile)
mref=mgetl(mreffile)

mclose("all")

// Return value set to %T
passed=%T
// Data to be written in log file
logfiletxt=[]
logfiletxt=[logfiletxt;"****** Beginning of validation of "+funname+".dia ******"]
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

// Verify that sizes match
if size(sciref,"*")<>size(mref,"*") then
  error("Scilab and Matlab reference do not have the same size...")
end

lind=1
resind=1
passed=%T
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
    sciinfosline=sciref(lind)
    matinfosline=mref(lind)
    lind=lind+1;

    [infos_match,veriftxt]=verif_dia(teststr,sciinfosline,matinfosline)
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
      
    res_match=rescomp(scires,mres);

    resind=resind+1;

    if ~res_match | ~infos_match then
      passed=%F
    end
    if lind>=size(sciref,"*") then
      break
    end
  end
  
end

mclose("all")

endfunction


