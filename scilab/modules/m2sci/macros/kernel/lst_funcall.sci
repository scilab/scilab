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

function   funcallname=lst_funcall(fil,fnamvect)
    //  LST_FUNCALL function (used by "translatepaths" function) Creates a list of vectors. The first component of each vector is the name of a M-file (found in the Paths to translate), followed by the called functions by this file
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
            if k==1 then
                functxt=txt(funcdecl(k):funcdecl(k+1)-1)
                str=  strindex(txt(funcdecl(k)),"(")-1
                funcname=stripblanks(part(txt(funcdecl(k)),strindex(txt(funcdecl(k)),["function[","function "])+8:str(1)))
                keq=strindex(funcname,"=")
                if ~isempty(keq) then
                    funcname=stripblanks(part(funcname,keq+1:length(funcname)))
                end
                mputl(functxt,pathconvert(TMPDIR)+fnam+".m");
            else
                functxt=txt(funcdecl(k):funcdecl(k+1)-1)
                str=strindex(txt(funcdecl(k)),"(")-1
                if str==-1 then
                    funcname=stripblanks(part(txt(funcdecl(k)),strindex(txt(funcdecl(k)),["function[","function "])+8:length(txt(funcdecl(k)))))
                else
                    funcname=stripblanks(part(txt(funcdecl(k)),strindex(txt(funcdecl(k)),["function[","function "])+8:str(1)))
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
        save(pathconvert(TMPDIR)+fnam+".tree",txt,helppart,batch)
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
        w=who("get");
        mname=w(1);

        // Compilation
        execstr("comp("+mname+",1)")

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
