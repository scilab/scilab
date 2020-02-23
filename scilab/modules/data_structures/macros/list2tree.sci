// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// Internal function (for the time being..)

function tree = list2tree(x, titleRoot, styles, arrayByFields)
    // x : list, cells or structs array, tlist, mlist, Xcos block
    // titleRoot: single string: label of the tree root, for instance
    //     the name of x in the calling environment
    // styles: single string = HTML.4 tag embedding the address
    //   of each data (fields names, lists indices..).
    //   Example: "<b>$</b>"  where $ will be replaced with the address.
    // arrayByFields: single boolean (when x is an array of structs).
    //   %T => For each field: the array of its values is displayed.
    //   %F => For each array component, all its fields are displayed.

    // CHECKING INPUT ARGUMENTS
    // ------------------------
    if argn(2) < 1 then
        msg = _("%s: Wrong number of input argument(s): %d to %d expected.\n")
        error(msprintf(msg, "list2tree", 1, 4));
    end

    if and(type(x)<>[15 16 17 128]) then
        msg = _("%s: Argument #%d: Container or Xcos object expected.\n")
        error(msprintf(msg, "list2tree", 1))
    end

    if ~isdef("titleRoot","l")
        titleRoot = "";
    end

    if ~isdef("arrayByFields","l") then
        arrayByFields = %f
    elseif type(arrayByFields) <> 4
        msg = _("%s: Argument #%d: Boolean(s) expected.\n")
        error(msprintf(msg, "tree_show", 3))
    else
        arrayByFields = arrayByFields(1)
    end

    if ~isdef("styles","l") then
        styles = ""
    elseif type(styles) <> 10
        msg = _("%s: Argument #%d: Text(s) expected.\n")
        error(msprintf(msg, "tree_show", 4))
    else
        styles = styles(1)
    end

    // BUILDING THE TREE
    // -----------------
    tree = uiCreateNode(titleRoot);
    tree = list2tree_inc(x, "root", tree, styles, arrayByFields);
endfunction

// ===========================================================================

function tree = list2tree_inc(x, Path, tree, styles, arrayByFields)

    if type(x) <> 15 then
        list_field_content = %f
    end
    if type(x)==15 then
        if length(x)==0 then
            tree($).label = tree($).label + " list()"
            return
        end
        II = 1:size(x);
        if isdef("list_field_content") & list_field_content
            if sum(list_field_content>1)>1
                // Generating all "(i,j,..)" headers
                v = container_get_ijk_labels(list_field_content)
            else
                v = msprintf("%d\n",II')';
                if prod(list_field_content)==1  // parent struct is scalar
                    tree($).label = tree($).label + " [list]"
                end
            end
        else
            tree($).label = tree($).label + " [list]"
            v = msprintf("%d\n",II')';
        end

    elseif typeof(x)=="ce"
        nb = length(x)
        if nb==0
            tree($).label = tree($).label + " { }"
            return
        else
            if nb==1 then
                tmp = " [cell]"
            else
                tmp = msprintf(" [%s cell]", ..
                        strcat(msprintf("%d\n",size(x)'),"x"));
            end
            tree($).label = tree($).label + tmp
            II = 1:nb
            // Generating all "(i,j,..)" headers
            v = container_get_ijk_labels(size(x))
        end

    elseif (type(x)==17 & typeof(x)=="st") then
        // Structures are displayed first by k indices, then
        // for each x(k), by field. In this way, each x(k) is
        // displayed as a whole object/record, instead of being split.
        if length(x)==0 then
            tree($).label = tree($).label + " struct()"
            return
        else
            if arrayByFields | length(x)==1 then
                if length(x)==1 then
                    if isdef("struct_array") & struct_array then
                        tmp = "↴"
                    else
                        tmp = " [struct]"
                    end
                else
                    tmp = msprintf(" [%s struct]", ..
                            strcat(msprintf("%d\n",size(x)'),"x"));
                end
                struct_array = %f
                v = ["" ; fieldnames(x)]
                II = 2:size(v,"*")  // fields indices
                list_field_content = size(x)
            else
                tmp = msprintf(" [%s struct]", ..
                        strcat(msprintf("%d\n",size(x)'),"x"));
                II = 1:size(x,"*")
                v = container_get_ijk_labels(size(x))
                struct_array = %t
            end
            tree($).label = tree($).label + tmp
        end

    elseif typeof(x)=="rational"
        v = getfield(1,x);
        II = 2:4
        if x.dt == []
            II = 2:3 // dt field skipped
        end
        if size(x,"*")<=1 then
            tmp = " [rational]"
        else
            tmp = msprintf(" [%s rational]", ..
                    strcat(msprintf("%d\n",size(x)'),"x"));
        end
        tree($).label = tree($).label + tmp

    elseif type(x)==128 & or(typeof(x)==["diagram" "Block"])
        tmp = _(" [Xcos diagram]")
        if typeof(x)=="Block" then
            tmp = _(" [Xcos block]")
        end
        struct_array = %f
        v = ["" ; fieldnames(x)]
        II = 2:size(v,"*")  // fields indices
        //list_field_content = size(x)
        tree($).label = tree($).label + tmp

    else // or(type(x)==[16 17])
        v = getfield(1,x);
        II = 2:size(v,"*")
        if II==[] // No field
            tree($).label = tree($).label + " " + sci2exp(x)
            return
        end
        // Header:
        tpof = typeof(x)
        sz = []
        if or(tpof==["st" "ce"]) | isdef("%"+tpof+"_size") then
            sz = size(x)
        end
        tmp = "tlist"
        if type(x)==17
            tmp = "mlist"
        end
        if sz == [] | prod(sz)<2
            tmp = "[" + tpof + " " + tmp + "]"
        else
            tmp = msprintf(" [%s %s %s]", ..
                    strcat(msprintf("%d\n",size(x)'),"x"), tpof, tmp);
        end
        tree($).label = tree($).label + tmp
    end

    // Loop over container components or fields
    // ----------------------------------------
    for i = II
        path = Path+"," + msprintf("%d",i)
        titre = v(i) + ": ";
        if styles <> ""
            titre = strsubst("<html>"+styles, "$", titre)
        end

        // Reads the object
        clear o
        if typeof(x)=="st" | (type(x)==128 & or(typeof(x)==["diagram" "Block"])) then
            if ~struct_array
                o = x(v(i))
            else
                o = x(i)
            end
        elseif typeof(x)=="ce"
            o = x{i}
        elseif or(type(x)==[15 16 17])
            try // there may be more field names than data
                o = getfield(i,x);
            catch
                o = list(,2)(1)
            end
        else
            o = x
        end
        //
        if ~isdef("o","l")  // void element in lists
            currentNode = uiCreateNode(titre);
            tree = uiConcatTree(tree, currentNode);

        elseif or(type(o)==[15 16 17 128])
            // Recursive call:
            currentNode = uiCreateNode(titre);
            currentNode = list2tree_inc(o, path, currentNode, styles, arrayByFields)
            tree = uiConcatTree(tree, currentNode);

        elseif typeof(o)=="implicitlist"
            tree = uiConcatTree(tree, uiCreateNode(titre + sci2exp(o)));

        elseif type(o)== 13    // function
            p = macr2tree(o)
            tmp = msprintf(_("%s() (%d lines)"), p.name, p.nblines)
            tree = uiConcatTree(tree, uiCreateNode(titre + tmp));

        elseif type(o)== 14    // library
            tmp = string(o)
            p = strsubst(tmp(1), "\", "/"); // not escaped chars for mprintf()
            libname = xmlGetValues("/scilablib","name",p + "lib")
            tmp = msprintf(_("%s library of %d functions @ %s"), ..
                           libname, size(tmp,1)-1, p)
            tree = uiConcatTree(tree, uiCreateNode(titre + tmp));

        else
            tp = typeof(o)
            try
                if size(o, "*") > 40 then
                    tts = tp + " of size " + sci2exp(size(o))
                else
                    tts = sci2exp(o)
                    if or(type(o)==(1:8))
                        tts = strsubst(tts, ",", ", ");
                        tts = strsubst(tts, ";", " ; ");
                    elseif type(o)==10
                        tts = strsubst(tts, """,""", """, """);
                        tts = strsubst(tts, """;""", """ ; """);
                    end
                    if type(o)==2 | typeof(o)=="rational"
                        vn = varn(o)
                        if or(vn==["s" "z"])
                            tts = strsubst(tts, "%"+vn, vn)
                            tts = strsubst(tts, "*", "")
                        end
                    end
                    if (or(type(o)==[1 2 5]) | typeof(o)=="rational")
                        tts = strsubst(tts, "%inf","Inf")
                        tts = strsubst(tts, "%nan","Nan")
                        tts = strsubst(tts, "%i","i")
                    end
                    if or(type(o)==[4 6])
                        tts = strsubst(tts, "%t", "T")
                        tts = strsubst(tts, "%f", "F")
                        tts = strsubst(tts, "],", "]]")
                        tts = strsubst(tts, ",", "")
                        tts = strsubst(tts, "]]", "],")
                    end
                end
            catch
                tts = tp;
            end
            titre2 = titre + tts  ;
            tree = uiConcatTree(tree, uiCreateNode(titre2));
        end
    end

endfunction

// -------------------------------------------------------------------

function v = container_get_ijk_labels(sz)
    if sum(sz>1) < 2 then
        // if it's a vector, we display linearized indices
        v = msprintf("%d\n", (1:prod(sz))')
    else
        // Otherwise we display (i,j,.) indices
        L = list()
        nd = length(sz);
        in = "=ndgrid(" + strcat(msprintf("1:%d\n",sz'),",") + ")";
        out = "[" + strcat(msprintf("L(%d)\n",(1:nd)'),",") + "]";
        execstr(out + in);
        v = []
        for i = 1:nd
            v = [v, msprintf("%d\n",L(i)(:))];
        end
        v = "(" + strcat(v,",","c") + ") ";
    end
endfunction
