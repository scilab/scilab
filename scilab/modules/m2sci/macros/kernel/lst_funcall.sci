// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function   funcallname = lst_funcall(fil, fnamvect)
    //  Internal function called only by translatepaths()
    //
    //  Creates a list of vectors.
    //  The first component of each vector is the name of a M-file (found in the Paths to translate),
    //  followed by the called functions by this file
    //  Output
    //  -funcallname : a list of vectors
    //  Input
    //  -fil : vector which contains all M-files names (path+name) found in the Paths
    //  -fnamvect : vector which contains all M-files names (just the name) found in the Paths

    quote="''";
    dquote="""";

    k=strindex(fil,".")
    ke=k($)-1
    // Function name
    ksep=strindex(fil,sep)
    fnam=part(fil,ksep($)+1:ke) // File name without extension
    txt=mgetl(fil);

    kf=grep(txt,["function[","function "])

    if isempty(kf) then
        // Batch file
        bval=%f
    elseif size(kf,"*")==1 then
        // Only one function defined
        bval=%f
    else
        funcdecl=[]
        for kk=kf
            ind=strindex(txt(kk),["function[";"function "])
            if (ind<isacomment(txt(kk)) | isacomment(txt(kk))==0) & ~isinstring(txt(kk),ind) & part(stripblanks(txt(kk),%T),1:8)=="function"  then // function prototype
                funcdecl=[funcdecl kk]
            end
        end
        if isempty(funcdecl) then
            // "function" only exists in comments and strings
            bval=%f
        elseif size(funcdecl,"*")==1 then
            bval=%f
        else
            bval= %t
        end
    end

    // file contains more than one function declaration
    if bval then
        // Verify if the directory exists
        sep = filesep();
        dirnam = ls(pathconvert(TMPDIR));
        if or(fnam==dirnam) then
            rmdir(pathconvert(TMPDIR)+fnam,"s")
        end
        mkdir(pathconvert(TMPDIR),fnam)

        write(%io(2),msprintf(gettext(" -- File %s contains more than one function -- "),fil));
        // First split file into as many files as function declared
        funcdecl=[funcdecl size(txt,"*")+1]
        tmpfiles=[]
        for k=1:size(funcdecl,"*")-1
            tmp = strindex(txt(funcdecl(k)),["function[","function "])
            if k==1 then
                functxt=txt(funcdecl(k):funcdecl(k+1)-1)
                str =  strindex(txt(funcdecl(k)),"(")
                if str==[] then
                    funcname=stripblanks(part(txt(funcdecl(k)),tmp+8:length(txt(funcdecl(k)))))
                else
                    funcname=stripblanks(part(txt(funcdecl(k)),tmp+8:str(1)-1))
                end
                keq=strindex(funcname,"=")
                if ~isempty(keq) then
                    funcname=stripblanks(part(funcname,keq+1:length(funcname)))
                end
                mputl(functxt,pathconvert(TMPDIR)+fnam+".m");
            else
                functxt=txt(funcdecl(k):funcdecl(k+1)-1)
                str=strindex(txt(funcdecl(k)),"(")
                if str==[] then
                    funcname=stripblanks(part(txt(funcdecl(k)),tmp+8:length(txt(funcdecl(k)))))
                else
                    funcname=stripblanks(part(txt(funcdecl(k)),tmp+8:str(1)-1))
                end
                keq=strindex(funcname,"=")
                if ~isempty(keq) then
                    funcname=stripblanks(part(funcname,keq+1:length(funcname)))
                end
                tmpfiles=[tmpfiles;funcname]
                mputl(functxt,pathconvert(TMPDIR)+pathconvert(fnam)+tmpfiles($)+".m");
            end
        end
        write(%io(2),msprintf(gettext(" -- Each function converted separately: %s -- "),strcat(tmpfiles," ")));
        write(%io(2),msprintf(gettext(" -- Temporary files put in: %s -- "),pathconvert(TMPDIR)));

        // Conversion of each file
        //for k=1:size(tmpfiles,"*")
        //mfile2sci(pathconvert(TMPDIR)+pathconvert(fnam)+tmpfiles(k)+".m",pathconvert(TMPDIR)+pathconvert(fnam))
        // Delete useless .m files
        //mdelete(pathconvert(TMPDIR)+pathconvert(fnam)+tmpfiles(k)+".m")
        //end

        translatepaths(pathconvert(TMPDIR)+pathconvert(fnam),pathconvert(TMPDIR)+pathconvert(fnam))
        // Catenation of all .sci files to have only one output file
        txt=[]

        for k=1:size(tmpfiles,"*")
            txt=[txt ;" ";mgetl(pathconvert(TMPDIR)+pathconvert(fnam)+tmpfiles(k)+".sci")]
            mdelete(pathconvert(TMPDIR)+pathconvert(fnam)+tmpfiles(k)+".sci")
        end
        mputl(txt,pathconvert(TMPDIR)+"tmp_"+fnam+".sci");
        // End of catenation of all .sci files to have only one output file
        txt=mgetl(pathconvert(TMPDIR)+pathconvert(fnam)+sep+"log")
        mputl(txt,pathconvert(TMPDIR)+"tmp_m2sci_"+fnam+".log");
        //
        txt=mgetl(pathconvert(TMPDIR)+pathconvert(fnam)+sep+"resumelog")
        mputl(txt,pathconvert(TMPDIR)+"tmp_resume_m2sci_"+fnam+".log");

        // Catenation of all .log files to have only one output file
        // txt=[]
        // for k=1:size(tmpfiles,"*")
        // txt=[txt ; mgetl(pathconvert(TMPDIR)+pathconvert(fnam)+"m2sci_"+tmpfiles(k)+".log")]
        // Delete useless .log files
        // mdelete(pathconvert(TMPDIR)+pathconvert(fnam)+"m2sci_"+tmpfiles(k)+".log")
        //end
        //mputl(txt,pathconvert(TMPDIR)+"tmp_m2sci_"+fnam+".log");
        //End of catenation of all .log files to have only one output file

        // Catenation of all resume.log files to have only one output file
        //txt=[]
        //for k=1:size(tmpfiles,"*")
        //txt=[txt ; mgetl(pathconvert(TMPDIR)+pathconvert(fnam)+"resume_m2sci_"+tmpfiles(k)+".log")]
        // Delete useless resume.log files
        // mdelete(pathconvert(TMPDIR)+pathconvert(fnam)+"resume_m2sci_"+tmpfiles(k)+".log")
        //end
        //mputl(txt,pathconvert(TMPDIR)+"tmp_resume_m2sci_"+fnam+".log");
        //End of catenation of all resume.log files to have only one output file

        txt=mgetl(pathconvert(TMPDIR)+fnam+".m")
    end

    txt = strsubst(txt, ascii(9), "")
    [helppart,txt,batch]=m2sci_syntax(txt)
    // save txt vector, helpart and batch after the syntax modification
    if strindex(fil,TMPDIR)==[] then
        save(pathconvert(TMPDIR)+fnam+".tree", "txt", "helppart", "batch")
    end

    funcallname=[]
    if txt~=[] then
        kc=strindex(txt(1),"function");
        kc=kc(1);

        // Define Scilab function
        fprot=funcprot();
        funcprot(0);

        // Blanks in file name are replaced by _ for batch
        // kc+9 because 'function '

        ksc=min(strindex(txt(1),";")) // searching for a comment on first line after function prototype

        if isempty(ksc) then
            ksc=length(txt(1))+1;
            firstline=[]
        else
            firstline=part(txt(1),ksc+1:length(txt(1)));
        end

        func_proto=part(txt(1),kc+9:ksc-1)
        keq=min(strindex(func_proto,"="))
        kpar=min(strindex(func_proto,"("))

        if isempty(keq) then
            keq=0
        end
        if isempty(kpar) then
            kpar=length(func_proto)+1
        end

        mname=strsubst(stripblanks(part(func_proto,keq+1:kpar-1))," ","_")
        func_proto=part(func_proto,1:keq)+..
        strsubst(stripblanks(part(func_proto,keq+1:kpar-1))," ","_")+..
        part(func_proto,kpar:length(func_proto))

        deff(func_proto,[firstline;txt(2:$)])
        // w=who("get");
        // mname=w(1);


        funcprot(fprot)

        // Get Scilab pseudo code of the function
        macr=evstr(mname)

        mtlbtree=macr2tree(macr);

        if ~batch then
            mtlbtree.name=mname;
        else
            mtlbtree.name="";
        end

        ninstr=1
        // variablevect is a vector which contains all variables (excluded functions)
        variablevect=[]
        for i=1:size(mtlbtree.inputs)
            variablevect=[variablevect;mtlbtree.inputs(i).name]
        end

        // search the declared variables in mtlbtree
        while ninstr<=size(mtlbtree.statements)-3
            [variablevect]=variablesearch(mtlbtree.statements(ninstr),variablevect)
            ninstr=ninstr+1
        end

        ninstr=1
        // search the called functions in the mtlbtree
        // funcallname contains the name of the M-file, followed by the called functions

        while ninstr<=size(mtlbtree.statements)-3
            [funcallname,variablevect]=funcallsearch(mtlbtree.statements(ninstr),funcallname,fnamvect,variablevect)
            ninstr=ninstr+1
        end

    end

    // add the M-file name in funcallname vector (at the first index)
    funcallname=[fnam;funcallname]

endfunction

// ---------------------------------------------------------------------------

function  variablename = variablesearch(instr,variablename)
    //  PRIVATE INTERNAL function called only by lst_funcall() hereabove and itself (recursive)
    //
    //  Searches names of declared variables for each instruction of mtlbtree
    //  Output
    //  -variablename : a vector which contains the names of declared variables
    //  -instr : mtlbtree instruction

    // case : ifthenelse instruction
    if typeof(instr) == "ifthenelse" then
        for i=1:size(instr.then)
            [variablename]=variablesearch((instr.then(i)),variablename)
        end
        for i=1:size(instr.elseifs)
            for k=1:size(instr.elseifs(i).then)
                [variablename]=variablesearch((instr.elseifs(i).then(k)),variablename)
            end
        end
        for i=1:size(instr.else)
        [variablename]=variablesearch((instr.else(i)),variablename)
        end
        // case : selectcase instruction
    elseif typeof(instr) == "selectcase" then
        for i=1:size(instr.cases)
            [variablename]=variablesearch(instr.cases(i).expression,variablename)
            for j=1:size(instr.cases(i).then)
                [variablename]=variablesearch((instr.cases(i).then(j)),variablename)
            end
        end
        for i=1:size(instr.else)
        [variablename]=variablesearch(instr.else(i),variablename)
        end
        // case : while instruction
    elseif typeof(instr) == "while" then
        for i=1:size(instr.statements)
            [variablename]=variablesearch(instr.statements(i),variablename)
        end
        // case : for instruction
    elseif typeof(instr) == "for" then
        [variablename]=variablesearch(instr.expression,variablename)
        for i=1:size(instr.statements)
            [variablename]=variablesearch(instr.statements(i),variablename)
        end
        // case : equal instruction
    elseif typeof(instr) == "equal" then
        for i=1:size(instr.lhs)
            [variablename]=variablesearch(instr.lhs(i),variablename)
        end
        // case : operation instruction
    elseif typeof(instr) == "operation" then
        if instr.operator=="ins" then
            if find(instr.operands(1).name==variablename)==[] then
                variablename($+1)=instr.operands(1).name
            end
        end
        // case : variable instruction
    elseif typeof(instr) == "variable" then
        if find(instr.name==variablename)==[] & instr.name<>"ans" then
            variablename($+1)=instr.name
        end
    end

endfunction

// ---------------------------------------------------------------------------

function   [funcallname,variablename] = funcallsearch(instr,funcallname,fnamvect,variablename)
    //  PRIVATE INTERNAL function called only by lst_funcall() hereabove and itself (recursive)
    //
    //  Searches the functions names in each instruction of mtlbtree
    //  Input-Output
    //  -funcallname : a vector which contains the names of called functions if it exists a M-file having the same name in the Paths
    //  -variablename : a vector which contains the names of declared variables
    //  Input
    //  -instr : mtlbtree instruction
    //  -fnamvect : vector which contains all M-files names found in the Paths

    // case : ifthenelse instruction
    if typeof(instr) == "ifthenelse" then
        [funcallname,variablename]=funcallsearch(instr.expression,funcallname,fnamvect,variablename)
        for i=1:size(instr.then)
            [funcallname,variablename]=funcallsearch((instr.then(i)),funcallname,fnamvect,variablename)
        end
        for i=1:size(instr.elseifs)
            for k=1:size(instr.elseifs(i).then)
                [funcallname,variablename]=funcallsearch((instr.elseifs(i).then(k)),funcallname,fnamvect,variablename)
            end
        end
        for i=1:size(instr.else)
        [funcallname,variablename]=funcallsearch((instr.else(i)),funcallname,fnamvect,variablename)
        end
        // case : selectcase instruction
    elseif typeof(instr) == "selectcase" then
        [funcallname,variablename]=funcallsearch(instr.expression,funcallname,fnamvect,variablename)

        for i=1:size(instr.cases)
            [funcallname,variablename]=funcallsearch((instr.cases(i).expression),funcallname,fnamvect,variablename)
            for j=1:size(instr.cases(i).then)
                [funcallname,variablename]=funcallsearch((instr.cases(i).then(j)),funcallname,fnamvect,variablename)
            end
        end
        for i=1:size(instr.else)
        [funcallname,variablename]=funcallsearch(instr.else(i),funcallname,fnamvect,variablename)
        end
        // case : while instruction
    elseif typeof(instr) == "while" then
        [funcallname,variablename]=funcallsearch(instr.expression,funcallname,fnamvect,variablename)
        for i=1:size(instr.statements)
            [funcallname,variablename]=funcallsearch(instr.statements(i),funcallname,fnamvect,variablename)
        end
        // case : for instruction
    elseif typeof(instr) == "for" then
        [funcallname,variablename]=funcallsearch(instr.expression,funcallname,fnamvect,variablename)
        for i=1:size(instr.statements)
            [funcallname,variablename]=funcallsearch(instr.statements(i),funcallname,fnamvect,variablename)
        end
        // case : cste instruction
    elseif  typeof(instr)== "cste" then
        return
        // case : variable instruction
    elseif typeof(instr)=="variable"
        if find(instr.name==variablename)==[] & find(instr.name==stripblanks(part(fnamvect,1:24)))<>[] & find(instr.name==funcallname)==[] then
            funcallname=[funcallname;fnamvect(find(instr.name==stripblanks(part(fnamvect,1:24))))]
        else
            return
        end
        // case : equal instruction
    elseif typeof(instr) == "equal" then
        [funcallname,variablename]=funcallsearch(instr.expression,funcallname,fnamvect,variablename)
        // case : expression is a funcall
    elseif typeof(instr) == "funcall" then
        if find(funcallname==instr.name) == [] & find(instr.name==stripblanks(part(fnamvect,1:24)))<>[]  then
            if size(find(instr.name==stripblanks(part(fnamvect,1:24))),2)==1 then
                funcallname=[funcallname;fnamvect(find(instr.name==stripblanks(part(fnamvect,1:24))))]
            else
                findvect=find(instr.name==stripblanks(part(fnamvect,1:24)))
                funcallname=[funcallname;fnamvect(findvect(2))]
                st = " " + mtlbtree.name + ": " + fnamvect(findvect(1))
                for i=2:size(findvect,2)
                    st = st+ " <-> " + fnamvect(findvect(i))
                end
                st = st + gettext(": The 24 first characters of the files names are equal: ");
                warning(st)
            end
        end
        // case : expression is cste
        if typeof(instr.rhs)== "constant" then
            return
        else
            for ind=1:size(instr.rhs)
                [funcallname,variablename]=funcallsearch(instr.rhs(ind),funcallname,fnamvect,variablename)
            end
        end
        // case : operation instruction
    elseif typeof(instr) == "operation" then
        for ind=1:size(instr.operands)
            [funcallname,variablename]=funcallsearch(instr.operands(ind),funcallname,fnamvect,variablename)
        end
    end

endfunction
