function []=sci_m2sciassume(def)
// Copyright INRIA
// This function translate calls to m2sciassume
// which can be used by the user to influence translation
// by adding a comment which begins by m2sciassume

// Global variable for M2SCI
global("varslist")

txt=[]

//variable_name|dimensions|datatype|property

//dimensions: variable dimensions separated by blanks (Unknown dimension is ?)

//datatype:
// - double: scalar/vector/matrix/hypermatrix containing double values
// - string: scalar/vector/matrix/hypermatrix containing character strings
// - boolean: scalar/vector/matrix/hypermatrix containing boolean values
// - int: scalar/vector/matrix/hypermatrix containing integer values
// - sparse: sparse scalar/vector/matrix/hypermatrix
// - cell: cell array
// - struct: struct array
// - FOS structname datatype: Field Of Struct
// - COC cell contents: Contents Of Cell (in this case name is ignored, set it to ? for example)
// - ? if unknown

//property:
// - real/complex/? for double and int datatype
// - real for string and boolean datatype (ignored if not)
// - NOT USED for struct datatype

// def is the comment added by the user
//userdata=def.rhs(1).value
userdata=def
// Remove all multiple blanks
while strindex(userdata,"  ")<>[]
  userdata=strsubst(userdat,"  "," ")
end

seppos=strindex(userdata,"|")

seppos=[seppos,length(userdata)+1]

nbsep=size(seppos,"*")

if nbsep<3 then
  error("m2sciassume: not enough data, you should give at least variable_name|dimensions|datatype");
elseif nbsep>4 then
  error("m2sciassume: too much data");
end

name=part(userdata,1:seppos(1)-1)

dimstxt=part(userdata,seppos(1)+1:seppos(2)-1)

datatypetxt=part(userdata,seppos(2)+1:seppos(3)-1)

if nbsep==4 then
  proptxt=part(userdata,seppos(3)+1:seppos(4)-1)
else
  proptxt="?"
end

// Dimensions
dimstxt=strsubst(dimstxt,"?","-1")
blpos=strindex(dimstxt," ")
nbblanks=size(blpos)
blpos=[1,blpos,length(dimstxt)+1]
fmt="%d"
dims=list()
for kbl=1:size(blpos,"*")-1
  dims($+1)=evstr(part(dimstxt,blpos(kbl):blpos(kbl+1)))
end

// Datatype
datatypetxt=strsubst(datatypetxt,"?","Unknown")
datatype=convstr(datatypetxt,"l")
if part(datatype,1:3)=="fos" then
  // Nothing to do (parsing done below)
elseif part(datatype,1:3)=="coc" then
  // Nothing to do (parsing done below)
elseif or(datatype==["double","boolean","string","int","sparse","cell","struct","unknown"]) then
  datatype=convstr(part(datatype,1),"u")+part(datatype,2:length(datatype))
  vtype=evstr(datatype)
else
  error("m2sciassume: Unknown datatype "+datatypetxt);
end

// Property
proptxt=strsubst(proptxt,"?","Unknown")
prop=convstr(proptxt,"l")
if or(prop==["real","complex","homogen","unknown"]) then
  prop=convstr(part(prop,1),"u")+part(prop,2:length(prop))
  property=evstr(prop)
else
  error("m2sciassume: Unknown property "+proptxt);
end
// Property correction
if or(vtype==[Boolean,String]) then
  property="Real"
end

if part(datatype,1:3)=="fos" then // Struct field m2sciassume
  blpos=strindex(datatype," ")
  nbbl=size(blpos,"*")
  if nbbl<>2 then
    error("m2sciassume: wrong datatype for field of struct: "+datatype);
  end
  stname=part(datatype,blpos(1)+1:blpos(2)-1)
  fdtype=part(datatype,blpos(2)+1:length(dadatype))
  
  [isvar,index]=isdefinedvar(Variable(stname,Infer()))
  if ~isvar then
    error("m2sciassume: undefined struct """+stname+""" in varslist")
  else
    varslist(index).infer.contents(name)=Infer(dims,Type(fdtype,property))
  end
elseif part(datatype,1:3)=="coc" then // Cell contents m2sciassume  
  blpos=strindex(datatype," ")
  nbbl=size(blpos,"*")
  if nbbl<>2 then
    error("m2sciassume: wrong datatype for field of struct: "+datatype);
  end
  cename=part(datatype,blpos(1)+1:blpos(2)-1)
  fdtype=part(datatype,blpos(2)+1:length(dadatype))
  
  [isvar,index]=isdefinedvar(Variable(cename,Infer()))
  if ~isvar then
    error("m2sciassume: undefined cell """+cename+""" in varslist")
  else
    varslist(index).infer.contents.entries=Infer(dims,Type(fdtype,property))
  end
else // Variable m2sciassume  
  [isvar,index]=isdefinedvar(Variable(name,Infer()))

  if ~isvar then // If variable does not exist it is added to varslist
    varslist($+1)=M2scivar(name,name,Infer(dims,Type(vtype,property)))
  else // Try to compare with already infered data
    // Update dims
    infereddims=varslist(index).dims
    err=%F
    if lstsize(dims)<>lstsize(infereddims) then
      err=%T
    else
      for kd=1:lstsize(dims)
	if infereddims(kd)~=dims(kd) then
	  err=%T
	  break
	end
      end
    end
    if err then
      set_infos(["Dimensions current value and m2sciassume statements conflict for: "+name
	  "   m2sciassume given dimension: "+dims2str(dims)
	  "   Current dimension: "+dims2str(infereddims)
	  "   m2sciassume IGNORED"],2)
    else
      varslist(index).dims=dims
    end
    
    // Update vtype
    if varslist(index).type.vtype==Unknown then
      varslist(index).type.vtype=tp
    elseif varslist(index).type.vtype~=vtype then
      set_infos(["Type current value and m2sciassume statements conflict for: "+name
	  "   m2sciassume given type: "+tp2str(vtype)
	  "   current type: "+tp2str(varslist(index).type.vtype)
	  "   m2sciassume IGNORED"],2)
    end
    
    // Update property
    if varslist(index).type.property==Unknown then
      varslist(index).type.property=property
    elseif varslist(index).type.property~=property then
      set_infos(["Property current value and m2sciassume statements conflict for: "+name
	  "   m2sciassume given type: "+prop2str(property)
	  "   current type: "+prop2str(varslist(index).type.property)
	  "   m2sciassume IGNORED"],2)
    end
  end
end
endfunction


function str=tp2str(tp)
// Copyright INRIA
// Scilab Project - V. Couvert
// Returns equivalent of typeof() from data coming from type()
if tp==1 then
  str="Double"
elseif tp==4 then
  str="Boolean"
elseif tp==6 then
  str="Sparse"
elseif tp==8 then
  str="Int"
elseif tp==10 then
  str="String"
elseif tp==16 then
  str="Struct"
elseif tp==17 then
  str="Cell"
elseif tp==-1 then
  str="Unknown"
else
  error("m2sciassume: type "+string(tp)+" is not implemented")
end
endfunction

function str=prop2str(prop)
if type(prop)==10 then
  str=prop
elseif prop==-1 then
  str="Unknown"
else
  error("m2sciassume: property "+string(prop)+" is not implemented")
end
endfunction

function str=dims2str(dims)
str=""
for kd=1:lstsize(dims)
  str=str+string(dims(kd))+" "
end
endfunction
