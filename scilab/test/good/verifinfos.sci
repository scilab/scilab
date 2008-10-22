function [val,logfiletxt]=verifinfos(teststr,sciinfos,minfos,inferinfos)
// Copyright INRIA
// Compare type, dimensions...of results to validate Scilab reference file
// M2SCI infered data is also compared to Matlab result
val=%T
logfiletxt=[]
ksci=strindex(sciinfos,"|")
scisize=msscanf(part(sciinfos,ksci(1)+1:ksci(2)-1),"%d %d")
scitype=part(sciinfos,ksci(2)+1:ksci(3)-1)
sciprop=part(sciinfos,ksci(3)+1:length(sciinfos))
resname=part(sciinfos,6:ksci(1)-1)

km=strindex(minfos,"|")
msize=msscanf(part(minfos,km(1)+1:km(2)-1),"%d %d")
mtype=part(minfos,km(2)+1:km(3)-1)
mprop=part(minfos,km(3)+1:length(minfos))

ki=strindex(inferinfos,"|")
infersize=msscanf(part(inferinfos,ki(1)+1:ki(2)-1),"%d %d")
infertype=part(inferinfos,ki(2)+1:ki(3)-1)
inferprop=part(inferinfos,ki(3)+1:length(inferinfos))

// Verify size between Matlab and Scilab
for k=1:size(scisize,"*")
  if scisize(k)<>msize(k) & scitype<>"String" then
    logfiletxt=[logfiletxt;teststr+": Scilab and Matlab size do not match for "+resname]
    logfiletxt=[logfiletxt;"Matlab: "+part(minfos,km(1)+1:km(2)-1)]
    logfiletxt=[logfiletxt;"Scilab: "+part(sciinfos,ksci(1)+1:ksci(2)-1)]
    val=%F
  elseif scitype=="String" & scisize(k)<>msize(k) then
    if scisize(msize<>scisize)<>1 then
      logfiletxt=[logfiletxt;teststr+": Scilab and Matlab size do not match for "+resname]
      logfiletxt=[logfiletxt;"Matlab: "+part(minfos,km(1)+1:km(2)-1)]
      logfiletxt=[logfiletxt;"Scilab: "+part(sciinfos,ksci(1)+1:ksci(2)-1)]
      val=%F
    else
      logfiletxt=[logfiletxt;teststr+": Scilab and Matlab size do not match but result is OK because is a String for "+resname]
    end
  end
end

// Verify size between M2SCI and Matlab
for k=1:size(msize,"*")
  if msize(k)<>infersize(k) & infersize(k)<>-1 then
    logfiletxt=[logfiletxt;teststr+": Infered size do not match for "+resname]
    logfiletxt=[logfiletxt;"M2SCI: "+part(inferinfos,ki(1)+1:ki(2)-1)]
    logfiletxt=[logfiletxt;"Matlab: "+part(minfos,km(1)+1:km(2)-1)]
    val=%F
    break
  end
end

// Verify type between Scilab and Matlab
if scitype<>mtype then
  if scitype<>"Double" & mtype<>"Boolean" then
    logfiletxt=[logfiletxt;teststr+": Scilab and Matlab type do not match for "+resname]
    logfiletxt=[logfiletxt;"Matlab: "+mtype]
    logfiletxt=[logfiletxt;"Scilab: "+scitype]
    val=%F
  else
    val=and([%T,val])
    logfiletxt=[logfiletxt;teststr+": Scilab returns "+scitype+" and Matlab returns "+mtype+" for "+resname]
  end
end
  
// Verify type between Matlab and M2SCI
if mtype<>infertype & infertype<>"Unknown" then
  logfiletxt=[logfiletxt;teststr+": Infered type do not match for "+resname]
  logfiletxt=[logfiletxt;"M2SCI: "+infertype]
  logfiletxt=[logfiletxt;"Matlab: "+mtype]
  val=%F
end
  
// Verify prop between Scilab and Matlab
if sciprop<>mprop then
  logfiletxt=[logfiletxt;teststr+": Scilab and Matlab property do not match for "+resname]
  logfiletxt=[logfiletxt;"Matlab: "+mprop]
  logfiletxt=[logfiletxt;"Scilab: "+sciprop]
  val=%F 
end

// Verify prop between Matlab and M2SCI
if mprop<>inferprop & inferprop<>"Unknown" then
  logfiletxt=[logfiletxt;teststr+": Infered property do not match for "+resname]
  logfiletxt=[logfiletxt;"M2SCI: "+inferprop]
  logfiletxt=[logfiletxt;"Matlab: "+mprop]
  val=%F 
end

endfunction


