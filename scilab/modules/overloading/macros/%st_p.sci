8// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT <vincent.couvert@scilab.org>
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - Stéphane MOTTELET
// Copyright (C) 2019-2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function %st_p(s)
    recursive = 0
    tmp = xmlGetValues("//general/body/environment","container_disp_max_depth")
    maxDisplayDepth = evstr(tmp)
    consoleWidth = lines()(1)
    //st_p_compacity = "";         // "" vspacing of blocks
    st_p_compacity = [];      // for compact display
    t = %st_p_inc(s, "st")
    mprintf("  %s\n", t);
endfunction

// -------------------------------------------------------------------

function t = %st_p_inc(s, parentType)
    // Can be called with s = struct | Tlist | list

    ll    = lines()
    t     = []
    indentFields = "    "
    if parentType=="list" then
        eq = "= "
    else
        eq = " = "
    end

    if  typeof(s)=="st"
        // No fields
        if isempty(fieldnames(s)) then
            t = msprintf(_("%s struct with no field"), ..
                         strcat(msprintf("%d\n",size(s)'), "x"));
            return
        end

        multi = size(s,"*")

        // 0x0 struct with fields
        if multi == 0 then
            t = _("0x0 struct with fields:")
            for field = fieldnames(s)'
                t = [t ; "   "+field]
            end
            return
        end

        // axb struct where a<>0 & b<>0
        if multi > 1 | recursive > maxDisplayDepth then
            if ~recursive
                t = msprintf(_("%s struct with fields:"), ..
                     strcat(msprintf("%d\n", size(s)'), "x"));
            end
            tmp = sci2exp(fieldnames(s)', consoleWidth-10)
            tmp = strsubst(strsubst(tmp, """""", """"), ",", ", ")
            t = [t ; tmp]
            return
        end
    end

    recursive0 = recursive
    if type(s)==15 then
        Fields = 1:length(s)
    else
        Fields = fieldnames(s)'
    end

    // MAIN LOOP
    // ---------
    for field = Fields
        sep = ": "      //   field_name<sep> ...
        if type(s)==15 then
            fieldn = msprintf("(%d) ", field)
        else
            fieldn = field
        end
        clear value
        value = s(field)
        if isdef("value","l") then
            tp = typeof(value)
        else
            tp = "void"
        end

        if tp=="void" then
            str = _("(void)")
            sep = eq

        elseif tp == "st" then
            recursive = recursive + 1
            str = %st_p_inc(value, "st")

        elseif tp == "implicitlist"
            str = sci2exp(value)
            sep = eq

        elseif tp == "function"
            [out,inp,?] = string(value)
            if inp==[], inp = "", end
            if out==[], out = "", end
            p = macr2tree(value)
            str = p.name+"("+strcat(inp,",")+") => ["+strcat(out,",")+"] ";
            str = str + msprintf(_("(%d lines)"),p.nblines)
            txt = fieldn + ": " + str

        elseif tp == "rational"
            str = strcat(msprintf("%d\n", size(value)'), "x")
            tmp = _("rational")
            if ~isreal(value,0)
                tmp = _("complex rational")
            end
            str = msprintf(_("[%s %s] of %s"), str, tmp, varn(value));

        elseif tp == "ce"
            if length(value)==0 then
                str = "{}"
                txt = fieldn + eq + str
            else
                str = strcat(msprintf("%d\n", size(value)'), "x")
                str = msprintf("[%s %s]", str, _("cell"));
                txt = fieldn + ": " + str
            end

        elseif type(value)==15
            if length(value)==0 then
                str = _(" empty list()")
            else
                if recursive < maxDisplayDepth
                    recursive = recursive + 1
                    tmp = %st_p_inc(value, "list")
                    str = ["list:" ; tmp]
                else
                    str = msprintf("list with %d elements.\n", length(value))
                end
            end

        elseif or(type(value)==[16 17])
            // Tlists or Mlists
            Tfields = fieldnames(value);
            sz = []
            try
                sz = size(value)
            end
            signature = tp
            if length(sz) > 1
                signature = strcat(msprintf("%d\n", sz'), "x") + " " + tp
            end
            listType = "tlist"
            if type(value)==17
                listType = "mlist"
            end
            if Tfields==[]
                str = msprintf(_("[%s] %s without field."), signature, listType);
            else
                str = msprintf(_("[%s] %s with fields:"), signature, listType);
                if recursive < maxDisplayDepth
                    recursive = recursive + 1
                    tmp = %st_p_inc(value, "mtlist")
                    str = [str ; tmp]
                else
                    tmp = sci2exp(Tfields', consoleWidth-10)
                    tmp = strsubst(strsubst(tmp, """", ""), ",", ", ")
                    str = [str ; tmp]
                end
            end

        elseif type(value)==14  // Library
            tmp = string(value)
            p = tmp(1)
            libname = xmlGetValues("/scilablib","name",p + "lib")
            str = msprintf("%s library with %d functions @ %s", ..
                           libname, size(tmp,1)-1, p)

        elseif type(value)> 10 then
            str = tp

        else
            sz = size(value)
            // If number of elements in value is greater than ll(1) (current
            // page width) then we do not call sci2exp because the returned
            // value will be ignored at line 68: size(str,"*")==1
            // Note that ll(1)/2 elements could be the max because of colon,
            //  semi-colon and brackets added between elements by sci2exp
            if sz(1) <= 1 & type(value) <> 9 & prod(sz) < ll(1) then
                // This line can avoid some memory issues when
                //  field contains a big matrix
                str = sci2exp(value, ll(1))
                sep = eq
            else
                str = "[" + strcat(msprintf("%d\n",sz'), "x") + " " + tp + "]"
            end
        end
        // ---------------------------
        if size(str,"*") == 1 & ..
               and(tp <> ["st" "function" "rational" "ce"])
            txt = fieldn + sep + str

        elseif and(tp <> ["function" "ce"])
            if recursive
                txt0 = indentFields    // indentation for fields list
                //txt0 = field + "."  // to display the chain of parent fields
            else
                txt0 = fieldn
            end
            if tp == "st"
                txt = fieldn + ": "
                if size(value,"*")==0
                    txt = txt + _("[0x0 struct] with no field")
                else
                    sv = size(value)'
                    if prod(sv)==1 then
                        txt = txt + "struct with fields:"
                    else
                        txt = txt + msprintf(_("[%s struct] with fields:"), ..
                                  strcat(msprintf("%d\n", sv), "x"));
                    end
                    txt = [txt ; txt0 + str]
                    if stripblanks(t($)) <> ""
                        txt = [st_p_compacity ; txt]
                    end
                end

            elseif or(type(value)==[15 16 17])
                txt = fieldn + ": " + str(1)
                if size(str,1)>1
                    txt = [txt ; indentFields + str(2:$)]
                end
                if stripblanks(t($)) <> ""
                    txt = [st_p_compacity ; txt]
                end

            else
                txt = txt0 + ..
                  ": " + "[" + strcat(msprintf("%d\n",size(value)'), "x") ..
                       + " " + tp + "]"
            end
        end
        t = [t ; txt]
        recursive = recursive0
    end
    // Display a blank line after each field that is a non-empty
    // struct or tlist:
    if recursive & stripblanks(t($)) <> ""
        t = [t ; st_p_compacity]
    end
endfunction
