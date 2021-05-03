// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright 2018 - 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [helppart, txt, batch] = m2sci_syntax(txt)
    // Make minor changes on M-file data syntax to have it readable by Scilab
    // Input arguments:
    //  - txt: the contents of an M-file
    // Output:
    //  - helppart: Matlab help contained in M-file
    //  - txt: input txt modified (If M-file contains only comments returned txt is[])
    //  - batch: boolean flag indicates if it is a batch file

    // m2sci kernel functions called :
    //  - isacomment
    //  - isinstring
    //  - replace_brackets
    //  - replace_end_dollar

    if ~isdef("octave_mode"), octave_mode = %f, end

    // Pre-process block-comments:
    // --------------------------
    // Converts block-comments tags
    k = grep(txt, "/^\%\{\s*$/", "r"); if k <> [] then, txt(k) = "/*", end
    k = grep(txt, "/^\%\}\s*$/", "r"); if k <> [] then, txt(k) = "*/", end
    // Comments the block-comments
    // (otherwise their content is cleared by the compiler)
    ks = find(txt=="/*")
    ke = find(txt=="*/")
    for k = matrix(ks,1,-1)
        i = find(ke>k,1)
        if i==[]
            j = size(txt,1)
        else
            j = ke(i)
            ke(i) = []
        end
        txt(k:j) = ["///*" ; "// " + txt(k+1:j-1) ; "//*/"]
    end
    //

    sciparam();
    quote = "''"
    dquote = """"
    ctm = "." + "." + "." // Continuation mark
    batch = %t

    k = 0
    first_ncl = []
    while k<size(txt,"r")
        k = k+1
        tk = txt(k)
        if part(stripblanks(tk),1:9) == "function " | part(stripblanks(tk),1:9) == "function[" then
            eolind = strindex(tk,";")
            if eolind<>[] then
                kc = isacomment(tk)
                if kc<>0 then // Current line has or is a comment
                    // If function prototype immediately followed by a comment on same line
                    if stripblanks(part(tk,eolind(1):kc))<>"" then
                        txt = [txt(1:k-1);part(tk,1:eolind(1)-1);part(tk,eolind(1)+1:length(tk));txt(k+1:size(txt,"*"))]
                        tk = part(tk,1:eolind(1)-1)
                    end
                elseif stripblanks(part(tk,eolind(1)+1:length(tk)))<>"" then
                    txt = [txt(1:k-1);part(tk,1:eolind(1)-1);part(tk,eolind(1)+1:length(tk));txt(k+1:size(txt,"*"))]
                    tk = part(tk,1:eolind(1)-1)
                end
            end
        end
    end

    // Number of lines in txt (i.e. in M-file)
    n = size(txt,"r")
    eoltoinsert = 0
    firstctm = []
    k = 0
    while k<size(txt,"r")
        k = k+1
        kc = strindex(txt(k),ctm)
        isacontline = %f

        for kck = kc,
            kc1 = isacomment(txt(k))
            if ~isinstring(txt(k),kck) then
                if kc1<>0 then // Line has a comment
                    if kc1<kck then // Continuation mark in a comment
                    else // Comment follow continuation mark
                        com = part(txt(k),kc1(1):length(txt(k)))
                        txt(k) = part(txt(k),1:kck-1)+txt(k+1)+com
                        txt(k+1) = []
                        k = k-1
                        break
                    end
                else // Not a comment line
                    if isempty(firstctm) then firstctm = k;end
                    txt(k) = part(txt(k),1:kck-1)+txt(k+1)
                    txt(k+1) = []
                    eoltoinsert = eoltoinsert+1
                    if isempty(strindex(txt(k),ctm)) then // If no ctm in txt(k), insert all necessary EOL to keep nblines unchanged
                        for l = 0:eoltoinsert-1
                            txt = [txt(1:firstctm-1+l);"";txt(firstctm-1+l+1:$)]
                        end
                        eoltoinsert = 0
                        firstctm = []
                    end
                    k = k-1
                    break
                end
            end
        end
        if k<>0 then
            if part(stripblanks(txt(k)),1)=="}" then
                txt(k-1) = txt(k-1)+txt(k);
                txt(k) = "";
            end
        end
    end

    // Change comments and get help part in input txt
    n = size(txt,"r")
    first = %t
    helppart = [],endofhelp = %f

    for k = 1:n
        tk = txt(k)

        // ifthenelse expression like if (x==1)t=2 becomes if (x==1) t=2
        // Add a blank between parenthesize expression and the first instruction
        kif = strindex(tk,"if")
        if kif<>[] then
            kcom = isacomment(tk)
            for i = 1:size(kif,"*")
                if kif(i)>kcom & kcom<>0 then
                    break
                else
                    if (kif(i)>1 & or(stripblanks(part(tk,kif(i)-1:kif(i)+2))==["if(","if"])) | (kif(i)==1 & or(stripblanks(part(tk,kif(i):kif(i)+2))==["if(","if"]))
                        m = min(strindex(tk,"("))
                        if m<>[] then
                            for l = 1:size(m,"*")
                                if m(l)>=kif(i)+2
                                    if stripblanks(part(tk,kif(i)+2:m(l)))=="(" then
                                        openpar = 1
                                        mtemp = m(l)+1
                                        while openpar<>0
                                            if or(part(tk,mtemp)=="(") then
                                                openpar = openpar+1
                                            elseif or(part(tk,mtemp)==")") then
                                                openpar = openpar-1
                                            end
                                            mtemp = mtemp+1
                                        end
                                        tk = part(tk,1:mtemp-1)+" "+part(tk,mtemp:length(tk))
                                        break
                                    end
                                end
                            end
                        end
                    end
                end
            end
        end

        // Modify struct like x.(fieldname) which become x(fieldname)
        tk = strsubst(tk,".(","(")

        // Parenthesize calls to pause when pause on or pause off
        kpause = strindex(tk,"pause")
        kpsav = length(tk) // kpsave is kp value for l-1 index
        for l = size(kpause,"*"):-1:1
            kp = kpause(l)
            kon = strindex(tk,"on")
            kon = kon(find((kon>kp)&(kon<kpsav)))
            if kon<>[] then
                for l = kp+5:kon-1
                    if part(tk,l)<>" " then
                        break
                    end
                end
                tk = part(tk,1:kp+4)+"(''on'')"+part(tk,kon+2:length(tk))
            end
            koff = strindex(tk,"off")
            koff = koff(find((koff>kp)&(koff<kpsav)))
            if koff<>[] then
                for l = kp+5:koff-1
                    if part(tk,l)<>" " then
                        break
                    end
                end
                tk = part(tk,1:kp+4)+"(''off'')"+part(tk,koff+3:length(tk))
            end
            kpsav = kp
        end

        // Convert @fhandle to fhandle
        // ---------------------------
        //  For test:
        //tk = "a=@sin, t=''legend(txt,''''abc@gdef@ghi'''')''; // See @ the link"
        kat = strindex(tk, "@");
        if kat <> []
            for m = 1:length(kat)
                if isinstring(tk, kat(m)) // We should also leave @ in comments
                    kat(m) = -kat(m);
                end
            end
            kat(kat<0) = [];
            // Remaining @ out of strings replaced with "£" (compilable)
            //  to tag them as function handles:
            if kat <> []
                // tk = strsplit(tk);
                // tk(kat) = []
                // tk = strcat(tk)
                tmp = part(tk,1:kat(1)-1);
                kat = [kat length(tk)+1];
                for m = 1:length(kat)-1
                    tmp = tmp + "£" + part(tk,kat(m)+1:kat(m+1)-1);
                end
                tk = tmp;
            end
        end

        // Looking for comments
        // --------------------
        kc = isacomment(tk)
        if kc<>0 then // Current line has or is a comment
            // If function prototype immediately followed by a comment on same line
            protoline = or(part(stripblanks(tk),1:9) == ["function " "function["]);
            if protoline then
                first_ncl = k
            end
            com = part(tk,kc+1:length(tk))
            if stripblanks(part(tk,1:kc-1))<>"" & ~protoline
                endofhelp = %t
            end
            if ~endofhelp & part(tk,1:9) ~= "function " then
                helppart = [helppart;com];
            end // Get help part placed at the beginning of the file
            if length(com)==0 then com = " ",end
            com = strsubst(com, quote, quote+quote)
            com = strsubst(com, dquote, dquote+dquote)
            if part(com,1:12)=="m2sciassume " | part(com,1:13)=="m2scideclare " then
                // User has given a clue to help translation
                if part(com,1:12)=="m2sciassume " then
                    warning(gettext("m2sciassume is obsolete, used m2scideclare instead."));
                end
                com = "m2scideclare("+quote+part(com,13:$)+quote+")"
                if kc>1
                    com = ";" + com
                end
            else
                if protoline
                    com = ";//" + com
                else
                    com = " //" + com
                end
            end
            tkbeg = part(tk,1:kc-1)

            // varargout replaced by %varargout so that code can be compiled with varargout considered as a Cell
            if isempty(strindex(tkbeg,"function")) then
                tkbeg = strsubst(tkbeg,"varargout","%varargout")
            end

            txt(k) = tkbeg+com
        else // Current line has not and is not a comment line
            if first then // Function keyword not yet found
                tk = stripblanks(tk)
                if tk<>"" then // Current line is not a blank line
                    if ~(part(tk,1:9) == "function "| part(tk,1:9) == "function[") then
                        endofhelp = %t;
                        txt(k) = tk; // VC 01/04/2003
                    else
                        first_ncl = k
                        first = %f
                    end
                else
                    if ~endofhelp then helppart = [helppart;" "],end
                    txt(k) = "// "
                end
            else // Current line is a line after function keyword
                endofhelp = %t
                txt(k) = tk
            end

            // varargout replaced by %varargout so that code can be compiled with varargout considered as a Cell
            if isempty(strindex(tk,"function")) then
                tk = strsubst(tk,"varargout","%varargout")
            end

            txt(k) = tk
        end
    end

    // When there is just help line in txt
    if ~endofhelp then
        txt = []
        return
    end

    // ===================
    // Syntax modification
    // ===================

    // Complex variable
    txt = i_notation(txt)

    // Processes double-quotes
    txt = process_doublequotes(txt)

    // Translate Flow control elements
    txt = strsubst(txt,"switch ","select ")
    txt = strsubst(txt,"switch(","select (")
    txt = strsubst(txt,"otherwise","else")
    if octave_mode then
        txt = strsubst(txt,"endif","end")
        txt = strsubst(txt,"endfor","end")
        txt = strsubst(txt,"endwhile","end")
        txt = strsubst(txt,"endswitch","end")
        txt = strsubst(txt,"end_try_catch","end")
        // txt = replace_do_until(txt)
    end

    // Replace double quotes
    txt = strsubst(txt,dquote,dquote+dquote)

    // Replace {..} by (..) or [..] : useful for cells translation
    txt = replace_brackets(txt)

    // Replace end with $ where it is relevant
    txt = replace_end_dollar(txt)

    // Place function definition line at first line
    kc = strindex(txt(first_ncl),"function")
    if kc==[] then
        // Batch file
        txt = ["function []="+fnam+"()";txt] // fnam is defined in mfile2sci()
        batch = %t
    else
        kc = kc(1)
        batch = %f
        if first_ncl<>1 then
            while strindex(txt(first_ncl($)+1),ctm)<>[] then
                first_ncl = [first_ncl,first_ncl($)+1]
            end
            txt = [txt(first_ncl);txt(1:first_ncl(1)-1);txt(first_ncl($)+1:$)]
        end
        // Beginning of BUG 2341 fix: function prototype with no comma between output parameters names
        begb = strindex(txt(1),"[");
        endb = strindex(txt(1),"]");
        if ~isempty(begb) & ~isempty(endb)
            outputparams = stripblanks(part(txt(1),(begb+1):(endb-1)))+"   ";
            k = 0;
            while k<length(outputparams)
                k = k+1;
                while (and(part(outputparams,k)<>[","," "])) & (k<length(outputparams)) // skip identifier
                    k = k+1;
                end
                while (part(outputparams,k)==" ") & (k<length(outputparams)) // skip spaces before comma (or next identifier)
                    k = k+1;
                end
                if (part(outputparams,k)<>",") & (k<length(outputparams))
                    outputparams = part(outputparams,1:(k-1))+","+part(outputparams,k:length(outputparams));
                    k = k+1;
                else
                    k = k+1;
                    while (part(outputparams,k)==" ") & (k<length(outputparams)) // skip spaces after comma
                        k = k+1;
                    end
                end
            end
            txt(1) = stripblanks(part(txt(1),1:begb)+outputparams+part(txt(1),endb:length(txt(1))));
        end
        // END of BUG 2341 fix: function prototype with no comma between output parameters names
    end
endfunction

// ---------------------------------------------------------------------------

function txt = replace_brackets(txt)

    symbs = [",",";","=",")","]","("]
    // This file will be use to deal with cells...
    for k = 1:size(txt,"r")

        // select-case
        if strindex(txt(k),"case")<>[] then
            txt(k) = strsubst(strsubst(txt(k),"{","makecell("),"}",")")
        else
            tk = strsubst(txt(k)," ","")

            ko = strindex(tk,"{")
            if ko<>[] then
                teq = strindex(tk,"=")

                ///
                if ko(1)==1 then
                    txt(k) = strsubst(txt(k),"{}","makecell()")
                    txt(k) = strsubst(strsubst(txt(k),"{","(makecell([cell(),"),"}","]))")
                elseif or(part(tk,ko(1)-1)==symbs) then
                    txt(k) = strsubst(txt(k),"{}","makecell()")
                    txt(k) = strsubst(strsubst(txt(k),"{","(makecell([cell(),"),"}","]))")
                else // Cell index
                    txt(k) = strsubst(strsubst(txt(k),"{","("),"}",").entries")
                end

                ////

                for kk = 2:size(ko,"*")
                    if or(part(tk,ko(kk)-1)==symbs) then // Cell creation
                        txt(k) = strsubst(txt(k),"{}","makecell()")
                        txt(k) = strsubst(strsubst(txt(k),"{","(makecell([cell(),"),"}","]))")
                    else // Cell index
                        txt(k) = strsubst(strsubst(txt(k),"{","("),"}",").entries")
                    end
                end
            elseif ~isempty(strindex(txt(k),"}")) then
                txt(k) = strsubst(txt(k),"}","]))")
            end
        end
    end
endfunction

// ---------------------------------------------------------------------------

function txt = replace_end_dollar(txt)

    patterns = ["/(?:\(|\-|\+|\*|\:|\,)\s*end\s*(\)|(\-|\+|\*|\/|\:|\,).*?\))/"
                "/(?:\{|\-|\+|\*|\:|\,)\s*end\s*(\}|(\-|\+|\*|\/|\:|\,).*?\})/"
                ]'
    for pattern = patterns
        rows = grep(txt, pattern, "r");
        for i = rows
            t = txt(i);
            [d, f, M] = regexp(t, pattern);
            Mr = strsubst(M, "end", "$");
            for j = 1:size(M,1)
                t = strsubst(t, M(j), Mr(j));
            end
            txt(i) = t;
        end
    end
endfunction

// ---------------------------------------------------------------------------
function txt = process_doublequotes(txt)
    k = grep(txt, """");
    if k==[] then
        return
    end
    if octave_mode then
        k = grep(txt, "|\$|", "r");
        if k <> []
        end
    end
endfunction

// ---------------------------------------------------------------------------

function txt = replace_do_until(txt)
    // do .. 
    // Before do:
    //  - can be ^,;\s
    //  - can't be (
    // After do:
    //  - can be [$\s,;("] //
    //  - can't be [a-zA-Z0-9_|&]

    // Before comments translation
    k = grep(txt, "_(^|,|;|\s)do($|\s|,|;|\(|%)_", "r")
    // After comments translation
    k = grep(txt, "_(^|,|;|\s)do($|\s|,|;|\(|//)_", "r")

    // until (condition)

endfunction

// ---------------------------------------------------------------------------

function txt = i_notation(txt)
    // This function changes 1i ,... by 1*i,...

    // M2SCI kernel functions called :
    //  - isinstring

    // To succeed in this work, we successively suppress occurences which can be proved not to be complex notation
    // Until we are 'sure' to have a complex notation

    n = size(txt,"r")

    I = "i";J = "j"
    matches = [string(0:9)+I(ones(1,10)),".i",string(0:9)+J(ones(1,10)),".j"]
    symbs = ["+","-","*","/","\","(","["," ","^"," ",",",";","=","{"]
    s1 = ["+","-","*","/","\",",",";"," ","^",".","&","|","''","]",")","}"]
    s2 = [string(0:9),"d","e","D","E","."]

    for k = 1:n
        // Isolate a possible appended comment
        st = strindex(txt(k),[";//","//"])
        endComment = "";
        tk = txt(k) + " "
        if st<> [] then
            for stk = 1:size(st,"*")
                if ~isinstring(txt(k),st(stk)) then
                    endComment = part(txt(k), st(stk):$);
                    tk = part(txt(k), 1:st(stk)-1)
                    break
                end
            end
        end

        // Find possible occurence of complex notation
        kc = strindex(tk,matches)

        // Kill indexes which point to non complex values (e.g. : a1item...)
        for kk = size(kc,"*"):-1:1
            km = kc(kk)+2
            if find(part(tk,km)==s1)==[] then kc(kk) = [],end
        end

        kc = [0 kc]

        for kk = size(kc,"*"):-1:2
            km = kc(kk)
            num = %T
            // Reads numeric value leading complex variable
            while or(part(tk,km)==s2)
                km = km-1
                if km<=kc(kk-1)+1 then
                    km = kc(kk-1);
                    num = %F;
                    break
                end
            end

            tokill = %F
            num = part(tk,km+1:kc(kk)-1)
            ke = strindex(convstr(num),["e","d"])
            kd = strindex(convstr(num),".")

            // Searching for invalid numeric values (more than one dot...)
            if size(ke,2)>1|size(kd,2)>1 then
                tokill = %T
            elseif size(ke,2)==1&size(kd,2)==1 then
                if ke<kd then tokill = %T,end
            end

            if ~tokill then
                // If char which follows supposed complex notation is not an operation symbol
                if km<>kc(kk-1) then
                    if and(part(tk,km)<>symbs) then tokill = %T,end
                end
            end

            if ~tokill then
                km = kc(kk)
                // If supposed complex notation is not in a string
                if ~isinstring(tk,km) then
                    tk = part(tk,1:km)+"*%"+part(tk,km+1:length(tk))
                end
            end
        end
        txt(k) = tk + endComment
    end
endfunction
