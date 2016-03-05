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

function bval=multi_fun_file(fil,res_path,Recmode,only_double,verbose_mode,prettyprintoutput)
    // This function converts M-Files containing more than one functio
    // Inputs are the same as mfile2sci()
    // Outputs :
    // - bval: boolean value, %t if file contains more than on function, %f else

    sciparam();

    // File name
    k=strindex(fil,".")
    if k<>[]
        ke=k($)-1
        base_name=part(fil,1:ke)
    else
        ke=length(fil)
        base_name=fil
    end
    // File path
    k=strindex(fil,"/")
    if k==[] then
        file_path="./"
    else
        file_path=part(fil,1:k($))
    end
    base_name=part(base_name,k($)+1:ke)

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
            if isacomment(txt(kk))==0 & ~isinstring(txt(kk),ind) & part(stripblanks(txt(kk),%T),1:8)=="function"  then // function prototype
                funcdecl=[funcdecl kk]
            end
        end

        if isempty(funcdecl) then
            // "function" only exists in comments and strings
            bval=%f
            return
        elseif size(funcdecl,"*")==1 then
            bval=%f
            return
        end

        // Verify if the directory exists
        dirnam = ls(pathconvert(TMPDIR)+base_name);

        sep = filesep();

        if or(dirnam<>"") then
            rmdir(pathconvert(TMPDIR)+base_name,"s")
        end
        mkdir(pathconvert(TMPDIR),base_name)
        write(%io(2),msprintf(gettext(" -- File %s contains more than one function -- "),fil));

        bval= %t

        // First split file into as many files as function declared
        funcdecl=[funcdecl size(txt,"*")+1]

        tmpfiles=[]
        for k=1:size(funcdecl,"*")-1
            functxt=txt(funcdecl(k):funcdecl(k+1)-1)
            str=  strindex(txt(funcdecl(k)),"(")-1
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
            mputl(functxt,pathconvert(TMPDIR)+base_name+sep+tmpfiles($)+".m");
        end

        write(%io(2),msprintf(gettext(" -- Each function converted separately: %s -- "),strcat(tmpfiles," ")));
        write(%io(2),msprintf(gettext(" -- Temporary files put in: %s -- "),pathconvert(TMPDIR)));

        // Conversion of each file

        for k=1:size(tmpfiles,"*")
            txt=mgetl(pathconvert(TMPDIR)+base_name+sep+tmpfiles(k)+".m")
            //mfile2sci(pathconvert(TMPDIR)+tmpfiles(k)+".m",res_path,Recmode,only_double,verbose_mode,prettyprintoutput)
        end

        translatepaths(pathconvert(TMPDIR)+base_name,pathconvert(TMPDIR)+base_name)

        txt=[]
        if isfile(pathconvert(TMPDIR)+base_name+sep+"log") then
            txt=mgetl(pathconvert(TMPDIR)+base_name+sep+"log")
        end
        mputl(txt,res_path+"log");
        if isfile(pathconvert(TMPDIR)+base_name+sep+"resumelog") then
            txt=mgetl(pathconvert(TMPDIR)+base_name+sep+"resumelog")
        end
        mputl(txt,res_path+"resumelog");

        // Catenation of all .sci files to have only one output file
        txt=[]
        for k=1:size(tmpfiles,"*")
            txt=[txt ;"";mgetl(pathconvert(TMPDIR)+base_name+sep+tmpfiles(k)+".sci")]
        end

        // Delete useless .sci files
        //for k=1:size(tmpfiles,"*")
        //mdelete(res_path+tmpfiles(k)+".sci")
        //end

        mputl(txt,res_path+base_name+".sci");

        // Catenation of all .log files to have only one output file
        //if exists("logfile")==0 then
        //txt=[]
        //for k=1:size(tmpfiles,"*")
        //txt=[txt ; mgetl(pathconvert(TMPDIR)+base_name+sep+"m2sci_"+tmpfiles(k)+".log")]
        //end

        // Delete useless .log files
        //for k=1:size(tmpfiles,"*")
        //mdelete(pathconvert(TMPDIR)+base_name+sep+"m2sci_"+tmpfiles(k)+".log")
        //end

        //mputl(txt,res_path+"m2sci_"+base_name+".log");
        //end

        // Catenation of all resume.log files to have only one output file
        //if exists("resume_logfile")==0 then
        //txt=[]
        //for k=1:size(tmpfiles,"*")
        //txt=[txt ; mgetl(res_path+"m2sci_"+tmpfiles(k)+"_resume.log")]
        //end

        // Delete useless _resume.log files
        //for k=1:size(tmpfiles,"*")
        //mdelete(res_path+"m2sci_"+tmpfiles(k)+"_resume.log")
        //end

        //mputl(txt,res_path+"m2sci_"+base_name+"_resume.log");
        //end

        // Delete useless .m files
        //for k=1:size(tmpfiles,"*")
        //mdelete(pathconvert(TMPDIR)+tmpfiles(k)+".m")
        //end

        rmdir(pathconvert(TMPDIR)+base_name,"s")
        for k=1:size(tmpfiles,"*")
            mdelete(pathconvert(TMPDIR)+tmpfiles(k)+".tree")
        end
    end

endfunction
