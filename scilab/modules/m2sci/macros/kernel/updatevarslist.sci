// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

function []=updatevarslist(instr_lhs)
    // (2 functions in this file: merge_vars() at the end)
    // Update list of M2SCI variables with converted instruction lhs
    // Input:
    // - instr_lhs: list of lhs of current instruction
    // - in_a_clause: boolean value
    //   Set to 1 if instruction is in a clause
    //   In this case, type and dimensions are set to unknown if differ from those already stored in varslist
    //   (Default value is %F)

    // Global variable for M2SCI
    global("varslist")
    if isempty(varslist)
        varslist = list()
    end
    // level is declared in m2sci.sci and modified in clause2sci.sci
    level;

    rhs=argn(2)
    if rhs==2 then
        in_a_clause=%F
    end

    // Merge infered data from the last two parts of clause which are above the current part
    // if we are in the third part of clause (current), then : merge the first and second part of clause
    // when end of conversion of a clause : merge infered data from the last two parts of clause
    levelsize=size(level,1)
    changepartclause=%F

    for i=size(varslist):-1:1
        if size(varslist(i).level,1)==levelsize then
            varlevel=varslist(i).level
            if varlevel($)<>level($)
                changepartclause=%T
            else
                changepartclause=%F
                break
            end
        end
    end
    if changepartclause | instr_lhs=="END OF CLAUSE" then
        index=[] // Search variables from two part above current part clause
        for k=size(varslist):-1:1
            if size(varslist(k).level,1)==levelsize then
                varlevel=varslist(k).level
                if and(varlevel(1:$-1)==level(1:$-1)) & varlevel($)==level($)-2 then
                    index=[index;k]
                end
            end
        end
        if index<>[] then  // Found variables from the second part above current part of a clause
            for k=1:size(index,1)
                boolmerge =%F
                for i=size(varslist):-1:1 // Search variables from the first part above current part of a clause, and which have the same name than variables from the second part above current part of a clause
                    varlevel=varslist(i).level
                    if varslist(i).matname==varslist(index(k)).matname & and(varlevel(1:$-1)==level(1:$-1)) &  varlevel($)==level($)-1 then
                        boolmerge =%T // Found the same variable name from the last two parts above the current part : Merge
                        merge_vars(index(k),varslist(i))
                        varslist(i)=null()
                        break
                    end
                end
                if ~boolmerge then
                    varslist(index(k)).level=[level(1:$-1);level($)-1]
                end
            end
        end
    end

    // Special case when end of conversion of a clause
    // Merge infered data from clause and those from level-1
    if instr_lhs=="END OF CLAUSE" then // Search variables in the last part of a clause (above end of conversion of a clause)
        index=[] //
        for k=size(varslist):-1:1 // Search variables from level-1 which have the same name than variables from the last part of current level
            varlevel=varslist(k).level
            if varlevel==[level(1:$-1);level($)-1] then
                index=[index;k]
            end
        end
        if index<>[] then
            for j=1:size(index,1)
                boolmerge=%F
                for k=size(varslist):-1:1 //
                    varlevel=varslist(k).level
                    if varslist(k).matname==varslist(index(j)).matname  & and(varlevel==level(1:$-1)) then
                        boolmerge=%T // Found variables from level-1 which have the same name than variables from the last part of current level : Merge
                        index_lower_level=k
                        merge_vars(index_lower_level,varslist(index(j)))
                        varslist(k).level=level(1:$-1)
                        varslist(index(j))=null()
                        break
                    end
                end
                if boolmerge==%F then
                    varslist(index(j)).level=level(1:$-1)
                end
            end
        end
        return
    end

    // Expression: lhs name is empty => nothing to do
    if instr_lhs==list() then
        return
    end

    // Remove lhs which are not variables
    k=1
    while k<=size(instr_lhs)
        // Insertion operation
        if typeof(instr_lhs(k))=="operation" then
            instr_lhs(k)=null()
        else
            k=k+1
        end
    end

    if size(instr_lhs)==0 then
        return
    end

    // Update varslist
    for k=1:size(instr_lhs)
        [bval,index]=isdefinedvar(instr_lhs(k))
        ierr=execstr("zz=instr_lhs(k).contents.index","errcatch")
        if ierr<>0 then pause;end
        // Remove multiple entries from contents
        for kcont=lstsize(instr_lhs(k).contents.index):-1:1
            [infertlist,pos]=get_contents_infer(instr_lhs(k),instr_lhs(k).contents.index(kcont))
            if pos<>0 & pos<>kcont then
                instr_lhs(k).contents.index(pos)=null()
                instr_lhs(k).contents.data(pos)=null()
            end
        end
        // If variable exists for the current level in the same part of clause then update exixting variable
        if bval
            boolupdate=%F
            for l=1:size(varslist)
                if varslist(l).matname==instr_lhs(k).name & varslist(l).level==level then
                    varslist(l)=M2scivar(varslist(l).sciname,..
                    varslist(l).matname,..
                    Infer(instr_lhs(k).infer.dims,instr_lhs(k).infer.type,instr_lhs(k).infer.contents),..
                    varslist(l).level)
                    boolupdate=%T
                    break
                end
            end
            // If variable exists, but not for the current level or not in the same part of clause then Update variable then create new variable
            if ~boolupdate then
                varslist($+1)=M2scivar(varslist(index).sciname,..
                varslist(index).matname,..
                instr_lhs(k).infer,..
                level)
            end
        else
            // Variable added to varslist if as a name (not done for expressions
            if execstr("f=instr_lhs(k).name","errcatch")<>0 then pause;end;errclear();
            if instr_lhs(k).name<>"ans" then
                varslist($+1)=M2scivar(instr_lhs(k).name,..
                instr_lhs(k).name,..
                instr_lhs(k).infer,..
                level)
            end
        end
    end
endfunction

function []=merge_vars(oldvarindex,newvar)
    // M2SCI function
    // Merge two variables inference properties, if different then set to Unknown
    // Input:
    // - oldvarindex: index of old variable in varslist
    // - newvar: new variable to take in account to update oldvar properties

    // Global variable for M2SCI
    global("varslist")
    oldvar=varslist(oldvarindex)

    olddims=oldvar.dims
    oldvtype=oldvar.vtype
    oldprop=oldvar.property

    newdims=newvar.dims
    newvtype=newvar.vtype
    newprop=newvar.property

    // Verify dims
    for l=1:min(size(newdims),size(olddims))
        if newdims(l)<>olddims(l) then
            newdims(l)=Unknown
        end
    end
    if size(newdims)>size(olddims) then
        for l=size(olddims):size(newdims)
            newdims(l)=null()
        end
    end

    // Verify vtype
    if newvtype<>oldvtype then
        newvtype=Unknown
    end

    // Verify property
    if newprop<>oldprop then
        newprop=Unknown
    end

    // Verify contents
    for k=1:lstsize(newvar.contents.index)
        olddata=get_contents_infer(oldvar,newvar.contents.index(k))
        newdata=newvar.contents.data(k)

        if or(olddata<>newdata) then
            newvar.infer.contents.data(k)=Infer()
        end
    end

    // Write result in varslist
    varslist(oldvarindex)=M2scivar(oldvar.sciname,..
    oldvar.matname,..
    Infer(newdims,Type(newvtype,newprop),newvar.contents),..
    newvar.level)
endfunction
