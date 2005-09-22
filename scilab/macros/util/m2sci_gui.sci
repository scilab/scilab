function res=m2sci_gui()
// Copyright INRIA
// Scilab Project - V. Couvert
// Call m2sci.tcl to get file or path to convert using M2SCI and user defined options

if ~with_tk() then
  error('Tcl/Tk interface not defined')
end

// Default Options and Values
TCL_SetVar("ePathToConvert",getcwd());
TCL_SetVar("eFileToConvert","<enter a file name>");
TCL_SetVar("eResDirName",getcwd());
TCL_SetVar("rRecMode","NO");
TCL_SetVar("rOnlyDouble","NO");
TCL_SetVar("rVerbMode","3");
TCL_SetVar("rPrettyPrint","NO");
TCL_SetVar("rFlagWhatToConvert","0");
TCL_SetVar("rToDo","0");
TCL_SetVar("rWindowKilled","0");

TCL_EvalFile(SCI+"/tcl/m2sci.tcl");

rWindowKilled=TCL_GetVar("rWindowKilled");
rToDo=TCL_GetVar("rToDo");
while rToDo=="0" & rWindowKilled=="0"
  rWindowKilled=TCL_GetVar("rWindowKilled");
  rToDo=TCL_GetVar("rToDo");
  sleep(1);
end

if rToDo=="1" then
  DirName=TCL_GetVar("ePathToConvert");
  FileName=TCL_GetVar("eFileToConvert");
  ResDirName=TCL_GetVar("eResDirName");
  RecMode=TCL_GetVar("rRecMode");
  OnlyDouble=TCL_GetVar("rOnlyDouble");
  VerbMode=TCL_GetVar("rVerbMode");
  PrettyPrint=TCL_GetVar("rPrettyPrint");
  FlagWhatToConvert=TCL_GetVar("rFlagWhatToConvert");
  
  // Modify Tcl/Tk values
  if RecMode=="YES" then
    RecMode=%T
  else  
    RecMode=%F
  end    
  if OnlyDouble=="YES" then
    OnlyDouble=%T
  else
    OnlyDouble=%F
  end    
  if PrettyPrint=="YES" then
    PrettyPrint=%T
  else
    PrettyPrint=%F
  end    
  VerbMode=evstr(VerbMode);
  
  if FlagWhatToConvert=="0" then // Convert a whole directory
    translatepaths(DirName,ResDirName);
    res=0
  else
    res=mfile2sci(FileName,ResDirName,RecMode,OnlyDouble,VerbMode,PrettyPrint);
  end
elseif rToDo=="-1" then
  // Cancel button
  res=0
else
  // Window killed
  res=0
end
endfunction
