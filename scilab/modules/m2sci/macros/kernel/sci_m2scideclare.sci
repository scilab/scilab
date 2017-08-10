// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function []=sci_m2scideclare(def)
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
        error(gettext("not enough data, you should give at least variable_name|dimensions|datatype."));
    elseif nbsep>4 then
        error(gettext("too much data."));
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
    dimstxt=strsubst(dimstxt,"*","-2")
    dimstxt=strsubst(dimstxt,"#","-3")
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
        error(msprintf(gettext("Unknown datatype %s."),datatypetxt));
    end

    // Property
    proptxt=strsubst(proptxt,"?","Unknown")
    prop=convstr(proptxt,"l")
    if or(prop==["real","complex","homogen","unknown"]) then
        prop=convstr(part(prop,1),"u")+part(prop,2:length(prop))
        property=evstr(prop)
    else
        error(msprintf(gettext("Unknown property %s."),proptxt));
    end
    // Property correction
    if or(vtype==[Boolean,String]) then
        property=Real
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
        if firstfield==".entries" then // Cell
            vartype=Cell
        else // Struct
            vartype=Struct
        end

        // Indexes for varname ? myvar(1,2).field....
        if or(strindex(name,"(")<strindex(name,".")) | (~isempty(strindex(name,"("))&isempty(strindex(name,"."))) then
            ierr=execstr("vardims=list"+part(name,min(strindex(name,"(")):min(strindex(name,")"))),"errcatch")
            if ierr then
                if ~isempty(strindex(part(name,min(strindex(name,"(")):min(strindex(name,")"))),"*")) then // Generic command *
                    vardims="generic"
                else
                    error(gettext("Wrong dimensions user data."));
                end
            end
        else
            vardims=list(1,1)
        end

        [isvar,index]=isdefinedvar(Variable(varname,Infer()))

        if ~isvar then // If variable does not exist it is added to varslist
            if vardims=="generic" then
                vardims=list(Unknown,Unknown)
            end
            contents=Contents()
            name=strsubst(name,"*","%genericm2sciindex")
            deff("m2scitmp",name)
            t=macr2tree(m2scitmp);
            if isempty(firstpoint) then
                contents.index($+1)=t.statements(2).expression.rhs;
            else
                contents.index($+1)=t.statements(2).expression.rhs(1);
            end
            clear m2scitmp
            for k=1:lstsize(contents.index($))
                if typeof(contents.index($)(k))=="variable" & contents.index($)(k).name=="%genericm2sciindex" then
                    contents.index($)(k)=Cste("*")
                elseif typeof(contents.index($)(k))=="cste" then
                    contents.index($)(k)=Cste(contents.index($)(k).value)
                elseif typeof(contents.index($)(k))=="list" then
                    for kk=1:lstsize(contents.index($)(k))
                        if typeof(contents.index($)(k)(kk))=="variable" & contents.index($)(k)(kk).name=="%genericm2sciindex" then
                            contents.index($)(k)(kk)=Cste("*")
                        elseif typeof(contents.index($)(k)(kk))=="cste" then
                            contents.index($)(k)(kk)=Cste(contents.index($)(k)(kk).value)
                        end
                    end
                end
            end
            contents.data($+1)=Infer(dims,Type(vtype,property))
            varslist($+1)=M2scivar(varname,varname,Infer(vardims,Type(vartype,Unknown),contents))
        else
            if vardims=="generic" then
                vardims=varslist(index).dims
            else
                vardims=dims
            end

            infereddims=varslist(index).dims

            err=%F
            for kd=1:min([lstsize(vardims) lstsize(infereddims)])
                if infereddims(kd)~=vardims(kd) & infereddims(kd)<>Unknown then
                    err=%T
                    break
                end
            end

            // Update dimensions
            if err then
                set_infos(msprintf(gettext("Dimensions current value and m2scideclare statements conflict for: %s\n   m2scideclare given dimension: %s\n   Current dimension: %s\n   m2scideclare IGNORED"),varname,dims2str(vardims),dims2str(infereddims)),2)
            else
                varslist(index)=M2scivar(varslist(index).matname,varslist(index).matname,Infer(vardims,Type(varslist(index).type.vtype,property)))
            end

            // Update vtype
            if varslist(index).type.vtype==Unknown then
                varslist(index)=M2scivar(varslist(index).matname,varslist(index).matname,Infer(vardims,Type(vartype,varslist(index).property)))
            elseif varslist(index).type.vtype~=vartype then
                set_infos(msprintf(gettext("Type current value and m2scideclare statements conflict for: %s\n   m2scideclare given type: %s\n   current type: %s\n   m2scideclare IGNORED"),varname,tp2str(vartype),tp2str(varslist(index).type.vtype)),2)
            end

            // Update property
            if varslist(index).property==Unknown then
                varslist(index).infer.type.property=property
            elseif property==Unknown then
                varslist(index).type.property=Unknown
            elseif varslist(index).type.property~=property then
                set_infos(msprintf(gettext("Property current value and m2scideclare statements conflict for: %s\n   m2scideclare given type: %s\n   current type: %s\n   m2scideclare IGNORED"),name,prop2str(Unknown),prop2str(varslist(index).type.property)),2)
            end

            // Update contents (no verification made...too complex)
            contents=varslist(index).contents
            name=strsubst(name,"*","%genericm2sciindex")
            deff("m2scitmp",name)
            t=macr2tree(m2scitmp);
            if isempty(firstpoint) then
                contents.index($+1)=t.statements(2).expression.rhs;
            else
                contents.index($+1)=t.statements(2).expression.rhs(1);
            end
            clear m2scitmp
            for k=1:lstsize(contents.index($))
                if typeof(contents.index($)(k))=="variable" & contents.index($)(k).name=="%genericm2sciindex" then
                    contents.index($)(k)=Cste("*")
                elseif typeof(contents.index($)(k))=="cste" then
                    contents.index($)(k)=Cste(contents.index($)(k).value)
                elseif typeof(contents.index($)(k))=="list" then
                    for kk=1:lstsize(contents.index($)(k))
                        if typeof(contents.index($)(k)(kk))=="variable" & contents.index($)(k)(kk).name=="%genericm2sciindex" then
                            contents.index($)(k)(kk)=Cste("*")
                        elseif typeof(contents.index($)(k)(kk))=="cste" then
                            contents.index($)(k)(kk)=Cste(contents.index($)(k)(kk).value)
                        end
                    end
                end
            end
            contents.data($+1)=Infer(dims,Type(vtype,property))
            varslist(index)=M2scivar(varname,varname,Infer(vardims,Type(vartype,Unknown),contents))

        end
    else // Variable m2scideclare

        // Special part for %graphicswindow
        if name=="%graphicswindow" then
            global %graphicswindow
            if and(vtype<>[Handle,Double]) then
                set_infos(gettext("%graphicswindow set to default value Handle."),2);
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
                set_infos(msprintf(gettext("Dimensions current value and m2scideclare statements conflict for: %s\n   m2scideclare given dimension: %s\n   Current dimension: %s\n   m2scideclare IGNORED"),name,dims2str(dims),dims2str(infereddims)),2)
            else
                varslist(index)=M2scivar(varslist(index).matname,varslist(index).sciname,Infer(dims,varslist(index).type))
            end

            // Update vtype
            if varslist(index).type.vtype==Unknown then
                varslist(index)=M2scivar(varslist(index).matname,varslist(index).sciname,Infer(varslist(index).dims,Type(vtype,varslist(index).type.property)))
            elseif varslist(index).type.vtype~=vtype then
                set_infos(msprintf(gettext("Type current value and m2scideclare statements conflict for: %s\n   m2scideclare given type: %s\n   current type: %s\n   m2scideclare IGNORED"),name,tp2str(vtype),tp2str(varslist(index).type.vtype)),2)
            end

            // Update property
            if varslist(index).type.property==Unknown then
                varslist(index)=M2scivar(varslist(index).matname,varslist(index).sciname,Infer(varslist(index).dims,Type(varslist(index).type.vtype,property)))
            elseif varslist(index).type.property~=property then
                set_infos(msprintf(gettext("Property current value and m2scideclare statements conflict for: %s\n   m2scideclare given type: %s\n   current type: %s\n   m2scideclare IGNORED"),name,prop2str(property),prop2str(varslist(index).type.property)),2)
            end
        end
    end
endfunction


function str=tp2str(tp)
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
        error(msprintf(gettext("type %d is not implemented."),tp))
    end
endfunction

function str=prop2str(prop)
    if type(prop)==10 then
        str=prop
    elseif prop==-1 then
        str="Unknown"
    else
        error(msprintf(gettext("type %d is not implemented."),prop))
    end
endfunction

function str=dims2str(dims)
    str=""
    for kd=1:lstsize(dims)
        str=str+string(dims(kd))+" "
    end
endfunction
