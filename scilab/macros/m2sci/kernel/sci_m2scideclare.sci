function []=sci_m2scideclare(def)
// Copyright INRIA
// This function translate calls to m2scideclare
// which can be used by the user to influence translation
// by adding a comment which begins by m2scideclare

// Global variable for M2SCI
global("varslist")

//variable_name|dimensions|datatype|property

//dimensions: variable dimensions separated by blanks (Unknown dimension is ?)

//datatype:
// - double: scalar/vector/matrix/hypermatrix containing double values
// - string: scalar/vector/matrix/hypermatrix containing character strings
// - boolean: scalar/vector/matrix/hypermatrix containing boolean values
// - int: scalar/vector/matrix/hypermatrix containing integer values
// - handle: matrix of graphics handles or graphics handle
// - sparse: sparse scalar/vector/matrix/hypermatrix
// - cell: cell array
// - struct: struct array
// - ? if unknown

//property:
// - real/complex/? for double and int datatype
// - real for string and boolean datatype (ignored if not)
// - NOT USED for struct/cell/handle datatype

// def is the comment added by the user
userdata=def.rhs(1).value
// Remove all multiple blanks
while strindex(userdata,"  ")<>[]
  userdata=strsubst(userdat,"  "," ")
end

seppos=strindex(userdata,"|")

seppos=[seppos,length(userdata)+1]

nbsep=size(seppos,"*")

if nbsep<3 then
  error("m2scideclare: not enough data, you should give at least variable_name|dimensions|datatype");
elseif nbsep>4 then
  error("m2scideclare: too much data");
end

name=stripblanks(part(userdata,1:seppos(1)-1))

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
if or(datatype==["double","boolean","string","int","handle","sparse","cell","struct","unknown"]) then
  datatype=convstr(part(datatype,1),"u")+convstr(part(datatype,2:length(datatype)),"l")
  vtype=evstr(datatype)
else
  error("m2scideclare: Unknown datatype "+datatypetxt);
end

// Property
proptxt=strsubst(proptxt,"?","Unknown")
prop=convstr(proptxt,"l")
if or(prop==["real","complex","homogen","unknown"]) then
  prop=convstr(part(prop,1),"u")+part(prop,2:length(prop))
  property=evstr(prop)
else
  error("m2scideclare: Unknown property "+proptxt);
end
// Property correction
if or(vtype==[Boolean,String]) then
  property="Real"
end

if strindex(name,".")<>[] then // Cell or Struct m2scideclare
  // Get varname
  endofname=min([strindex(name,[".","("])])-1
  varname=part(name,1:endofname)
  
  // First field name (if is 'entries' then a cell else a struct)
  firstpoint=min(strindex(name,"."))
  secpoint=min(strindex(part(name,firstpoint+1:length(name)),"."))
  par=min(strindex(part(name,firstpoint+1:length(name)),"("))
  if isempty(secpoint) & isempty(par) then //x.fieldname
    firstfield=part(name,firstpoint:length(name))
  elseif isempty(secpoint) then //x.fieldname(p...)
    firstfield=part(name,firstpoint:par-1)
  elseif isempty(par) then //x.fieldname.fieldname2 
    firstfield=part(name,firstpoint:secpoint-1)
  else //x.fieldname(p...).fieldname2 
    firstfield=part(name,firstpoint:min([secpoint par])-1)
  end
    
  if firstfield=="entries" then // Cell
    vartype=Cell
  else // Struct
    vartype=Struct
  end
  
  // Indexes for varname ? myvar(1,2).field....
  if or(strindex(name,"(")<strindex(name,".")) then
    vardims=evstr("list"+part(name,min(strindex(name,"(")):min(strindex(name,")"))))
  else
    vardims=list(1,1)
  end
  
  [isvar,index]=isdefinedvar(Variable(varname,Infer()))
    
  if ~isvar then // If variable does not exist it is added to varslist
    contents=struct()
    evstr("contents"+part(name,length(name))+"=1"); // should be removed is no more Scilab bug for it
    evstr("contents"+part(name,length(name))+"=Infer(dims,Type(vtype,property))");
    varslist($+1)=M2scivar(varname,varname,Infer(vardims,Type(vartype,Unknown,contents)))
  else
    inferreddims=varslist(index).dims
    
    err=%F
    for kd=1:min([lstsize(vardims) lstsize(infereddims)])
      if infereddims(kd)~=vardims(kd) & infereddims(kd)<>Unknown then
	err=%T
	break
      end
    end
    
    // Update dimensions
    if err then
      set_infos(["Dimensions current value and m2scideclare statements conflict for: "+varname
	  "   m2scideclare given dimension: "+dims2str(vardims)
	  "   Current dimension: "+dims2str(infereddims)
	  "   m2scideclare IGNORED"],2)
    else
      varslist(index).dims=dims
    end
    
    // Update vtype
    if varslist(index).type.vtype==Unknown then
      varslist(index).type.vtype=vartype
    elseif varslist(index).type.vtype~=vartype then
      set_infos(["Type current value and m2scideclare statements conflict for: "+varname
	  "   m2scideclare given type: "+tp2str(vartype)
	  "   current type: "+tp2str(varslist(index).type.vtype)
	  "   m2scideclare IGNORED"],2)
    end

    // Update property
    if varslist(index).type.property==Unknown then
      varslist(index).type.property=Unknown
    else
      set_infos(["Property current value and m2scideclare statements conflict for: "+name
	  "   m2scideclare given type: "+prop2str(Unknown)
	  "   current type: "+prop2str(varslist(index).type.property)
	  "   m2scideclare IGNORED"],2)
    end
    
    // Update contents (no verification made...too complex)
    contents=varslist(index).contents
    if vartype==Cell & typeof(contents)<>"ce" then
      contents=makecell()
      evstr("contents"+part(name,length(name))+"=1"); // should be removed is no more Scilab bug for it
    elseif vartype==Struct & typeof(contents)<>"st" then
      contents=struct()
      evstr("contents"+part(name,length(name))+"=1"); // should be removed is no more Scilab bug for it
    end
    evstr("contents"+part(name,length(name))+"=Infer(dims,Type(vtype,property))");
    varslist(index).contents=M2scivar(varname,varname,Infer(vardims,Type(vartype,Unknown,contents)))
    
    
  end
else // Variable m2scideclare 
  
  // Special part for %graphicswindow
  if name=="%graphicswindow" then
    global %graphicswindow
    if and(vtype<>[Handle,Double]) then
      set_infos("%graphicswindow set to default value Handle",2);
    else
      %graphicswindow=vtype
    end
    return
  end
  
  [isvar,index]=isdefinedvar(Variable(name,Infer()))

  if ~isvar then // If variable does not exist it is added to varslist
    varslist($+1)=M2scivar(name,name,Infer(dims,Type(vtype,property)))
  else // Try to compare with already infered data
    // Update dims
    infereddims=varslist(index).dims
    
    err=%F
    for kd=1:min([lstsize(dims) lstsize(infereddims)])
      if infereddims(kd)~=dims(kd) & infereddims(kd)<>Unknown then
	err=%T
	break
      end
    end

    if err then
      set_infos(["Dimensions current value and m2scideclare statements conflict for: "+name
	  "   m2scideclare given dimension: "+dims2str(dims)
	  "   Current dimension: "+dims2str(infereddims)
	  "   m2scideclare IGNORED"],2)
    else
      varslist(index)=M2scivar(varslist(index).matname,varslist(index).sciname,Infer(dims,varslist(index).type))
    end
    
    // Update vtype
    if varslist(index).type.vtype==Unknown then
      varslist(index)=M2scivar(varslist(index).matname,varslist(index).sciname,Infer(varslist(index).dims,Type(vtype,varslist(index).type.property)))
    elseif varslist(index).type.vtype~=vtype then
      set_infos(["Type current value and m2scideclare statements conflict for: "+name
	  "   m2scideclare given type: "+tp2str(vtype)
	  "   current type: "+tp2str(varslist(index).type.vtype)
	  "   m2scideclare IGNORED"],2)
    end
    
    // Update property
    if varslist(index).type.property==Unknown then
      varslist(index)=M2scivar(varslist(index).matname,varslist(index).sciname,Infer(varslist(index).dims,Type(varslist(index).type.vtype,property)))
    elseif varslist(index).type.property~=property then
      set_infos(["Property current value and m2scideclare statements conflict for: "+name
	  "   m2scideclare given type: "+prop2str(property)
	  "   current type: "+prop2str(varslist(index).type.property)
	  "   m2scideclare IGNORED"],2)
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
elseif tp==9 then
  str="Handle"
elseif tp==10 then
  str="String"
elseif tp==16 then
  str="Struct"
elseif tp==17 then
  str="Cell"
elseif tp==-1 then
  str="Unknown"
else
  error("m2scideclare: type "+string(tp)+" is not implemented")
end
endfunction

function str=prop2str(prop)
if type(prop)==10 then
  str=prop
elseif prop==-1 then
  str="Unknown"
else
  error("m2scideclare: property "+string(prop)+" is not implemented")
end
endfunction

function str=dims2str(dims)
str=""
for kd=1:lstsize(dims)
  str=str+string(dims(kd))+" "
end
endfunction
