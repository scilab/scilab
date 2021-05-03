// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
// Copyright (C) ???? - INRIA - Serge STEER
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function res = mfile2sci(fil, results_path, Recmode, only_double, verbose_mode, prettyprintoutput)
    // This function performs translation of a single M-file
    // - fil: file name
    // - results_path: path to write translated file in (default value is fil path)
    // - Recmode: recursive mode (default value is false)

    // Get default arguments
    [lhs,rhs] = argn(0)
    if ~isdef("prettyprintoutput","l"), prettyprintoutput = %F, end
    if ~isdef("verbose_mode","l"),      verbose_mode = 3,       end
    if ~isdef("only_double","l"),       only_double = %F,       end
    if ~isdef("Recmode","l"),           Recmode = %F,           end
    if ~isdef("results_path","l"),      results_path = ".",     end
    if rhs<1 then
        m2sci_gui();
        res = [];
        return
    end
    fil = strsubst(fil,filesep(),"/")
    results_path = getshortpathname(pathconvert(results_path));
    results_path = strsubst(results_path,"\","/")
    res_path = results_path  // res_path: environment variable in some called functions

    // Loads libraries related to m2sci
    if exists("m2scikernellib")==0 then load("SCI/modules/m2sci/macros/kernel/lib"),end
    if exists("m2scipercentlib")==0 then load("SCI/modules/m2sci/macros/percent/lib"),end
    if exists("m2scisci_fileslib")==0 then load("SCI/modules/m2sci/macros/sci_files/lib"),end

    if multi_fun_file(fil,results_path,Recmode,only_double,verbose_mode,prettyprintoutput) then
        res = 1
        return
    end

    // Get context
    global("m2sci_infos")
    global("mtlbref_fun")
    global("mtlbtool_fun")
    global("not_mtlb_fun")
    [l,mac] = where()
    Reclevel = size(find(mac=="mfile2sci"),"*")
    tpcallpos = min(find(mac=="translatepaths"));
    guicallpos = min(find(mac=="m2sci_gui"));
    if size(find(mac=="m2sci_gui"),"*")==1 & tpcallpos<guicallpos then // Bug 679
        Reclevel = Reclevel-1
    end
    if size(find(mac=="multi_fun_file"),"*")==1 then
        Reclevel = Reclevel-1
    end

    if Reclevel==1 then
        nametbl = []
    else
        m2sci_infos_save = m2sci_infos
    end
    m2sci_infos = [%f %f]

    margin = part(" ",ones(1,3*(Reclevel-1)))
    margin = "  "
    rec = gettext("OFF");
    dble = gettext("NO");
    pretty = gettext("NO");
    if prettyprintoutput then pretty = gettext("YES");end
    if Recmode then rec = gettext("ON");end
    if only_double then dble = gettext("YES");end

    res = []

    // Handle file path
    // File name
    k = strindex(fil,".")
    if k<>[]
        ke = k($)-1
        base_name = part(fil,1:ke)
    else
        ke = length(fil)
        base_name = fil
    end
    // File path
    k = strindex(fil,"/")
    if k==[] then
        file_path = "./"
    else
        file_path = part(fil,1:k($))
    end
    // Others M-files in directory
    if exists("Paths")==0 then
        Paths = file_path,
        if getos() == "Windows" then
            Paths = strsubst(Paths,"/","\")
            mfiles = listfiles(Paths+"*.m")
            sep = filesep()
        else
            mfiles = listfiles(Paths+"*.m")
            sep = filesep()
        end
    end

    // Function name
    if k~=[] then
        fnam = part(base_name,k($)+1:ke) // File name without extension
    else
        fnam = base_name
    end

    // logfile initialisation
    if exists("logfile")==0 then
        File = getshortpathname(pathconvert(TMPDIR))+"logfile.dat"
        [tempfd1,ierr1] = file("open",File,"old")
        if ierr1==0 then
            load(File)
            file("close",tempfd1)
            file("close",logfile)
            mdelete(File)
        end
        logfile = file("open",results_path+"m2sci_"+fnam+".log","unknown")
        save(File, "logfile")
    end

    // Output beginning message
    mss = [gettext("****** Beginning of mfile2sci() session ******");
    gettext("File to convert:")+" "+fil;
    gettext("Result file path:")+" "+results_path;
    gettext("Recursive mode:")+" "+rec;
    gettext("Only double values used in M-file:")+" "+dble;
    gettext("Verbose mode:")+" "+string(verbose_mode);
    gettext("Generate formatted code:")+" "+pretty]
    m2sci_info(mss,-1);

    // Read in the file as text
    m2sci_info(gettext("M-file reading..."),-1);
    txt = mgetl(fil);
    m2sci_info(gettext("M-file reading: Done"),-1);

    //Replace TAB by SPACE
    txt = strsubst(txt, ascii(9), "");

    if txt==[] then
        m2sci_infos(msprintf(gettext("File %s is an empty file.\n"),fil),-1);
        return
    end
    tmptxt = txt

    // Setting octave_mode
    // -------------------
    if ~isdef("octave","l") then
        if ~isdef("octave_mode")
            octave_mode = %F    // To replace with an optional argin
        else
            octave_mode = octave_mode
        end
    else
        octave_mode = octave
    end


    // Make changes on syntax, to make the file scilab-compilable
    // ==========================================================
    m2sci_info(gettext("Syntax modification..."),-1);
    ierr = execstr("load(''"+pathconvert(TMPDIR)+fnam+ ".tree'',''txt'',''helppart'',''batch'')","errcatch","n")
    if ierr<>0 | exists("txt")==0 | exists("batch")==0 & ..
                 strindex(results_path,getshortpathname(TMPDIR))==[] then
        [helppart, txt, batch] = m2sci_syntax(txt)
    elseif ierr==0 & newest(fil,pathconvert(TMPDIR)+fnam+ ".tree")==1 then
        [helppart, txt, batch] = m2sci_syntax(tmptxt)
    end

    m2sci_info(gettext("Syntax modification: Done"),-1);

    // Write .cat file and update whatis
    // ---------------------------------
    if helppart<>[] then
        catfil = results_path + fnam+".cat"
        whsfil = results_path + "whatis"
        mputl(helppart,catfil);
        if exists("whsfil_unit")==1 then
            write(whsfil_unit,stripblanks(helppart(1))+" |"+fnam,"(a)")
        end
    end

    if txt~=[] then
        quote = "''";
        dquote = """";
        kc = strindex(txt(1),"function");
        kc = kc(1);

        // Define Scilab function
        fprot = funcprot();
        funcprot(0);

        // Blanks in file name are replaced by _ for batch
        // kc+9 because 'function '
        ksc = min(strindex(txt(1),";")) // searching for a comment on first line after function prototype
        if isempty(ksc) then
            ksc = length(txt(1))+1;
            firstline = []
        else
            firstline = part(txt(1),ksc+1:length(txt(1)));
        end

        // Extraction of the macro's name
        func_proto = part(txt(1),kc+9:ksc-1)
        keq = min(strindex(func_proto,"="))
        kpar = min(strindex(func_proto,"("))
        if isempty(keq) then
            keq = 1
        end
        if isempty(kpar) then
            kpar = length(func_proto)+1
        end

        func_proto = part(func_proto,1:keq)+strsubst(stripblanks(part(func_proto,keq+1:kpar-1))," ","_")+part(func_proto,kpar:length(func_proto))

        mname = getMacroNameFromPrototype(func_proto)
        if mname=="" | mname==[]
            msg = _("%s: ""%s"" does not exist or is not the macro name.\n");
            error(msprintf(msg,"mfile2sci", mname))
        end
        w = mname;
        nametbl = [nametbl;mname]
        if fnam<>mname & ~batch then // warning is not displayed for a batch file
            mss = msprintf(gettext("Warning: file %s defines function %s instead of %s\n         %s.sci, %s.cat and sci_%s.sci will be generated !"),fil,mname,fnam,mname,mname,mname);
            m2sci_info(mss,-1);
        end

        txt($+1) = "endfunction"
        // Compilation
        txt = [part(txt(1),kc:ksc-1);firstline;txt(2:$)]
        mputl(txt, TMPDIR+"/"+mname+".sci");
        err = exec(TMPDIR+"/"+mname+".sci", "errcatch", -1);
        if err~=0
            // Maybe a final {end} closing {function} => try without it
            txt($) = []   // removing "endfunction"
            while size(txt,1)>0 & stripblanks(txt($))==""
                txt($) = []
            end
            if size(txt,1)>0 & stripblanks(txt($))=="end" then
                txt($) = []
                txt = [txt ; "endfunction"]
                mputl(txt, TMPDIR+"/"+mname+".sci");
            end
            exec(TMPDIR+"/"+mname+".sci", -1);
        end
        funcprot(fprot);
        mdelete(TMPDIR+"/"+mname+".sci");

        // Get Scilab pseudo code of the function
        m2sci_info(gettext("Macro to tree conversion..."),-1);
        macr = evstr(mname)
        mtlbtree = macr2tree(macr);
        if ~batch then
            mtlbtree.name = mname;
        else
            mtlbtree.name = "";
        end

        //Transfom a equal instructions(if lhs are multi_operation insert and expression is a funcall) in the matlab tree to sup_equal instructions
        global("tmpvarnb")
        tmpvarnb = 0;
        level = [0,0];
        ninstr = 1;
        while ninstr<=size(mtlbtree.statements)-3
            mtlbtree.statements(ninstr) = transformtree(mtlbtree.statements(ninstr))
            ninstr = ninstr+1
        end

        // Perform the translation
        [scitree,trad,hdr,crp] = m2sci(mtlbtree,w(1),Recmode,prettyprintoutput)

        //Creation of fname_resume.log file
        // if mtlbref_fun<>[]|not_mtlb_fun<>[]|mtlbtool_fun<>[] then
        //resume_logfile initialisation
        if exists("resume_logfile")==0 then
            File = getshortpathname(pathconvert(TMPDIR))+gettext("resumelogfile.dat")
            [tempfd2,ierr2] = file("open",File,"old")
            if ierr2==0 then
                load(pathconvert(TMPDIR)+gettext("resumelogfile.dat"))
                file("close", tempfd2)
                file("close", resume_logfile)
                mdelete(pathconvert(TMPDIR)+gettext("resumelogfile.dat"))
            end
            resume_logfile = results_path+gettext("resume")+"_m2sci_"+fnam+".log"
            resume_logfile = file("open", resume_logfile, "unknown")
            save(pathconvert(TMPDIR)+gettext("resumelogfile.dat"), "resume_logfile")
        end

        //number of matlab reference functions, matlab toolboxes functions, not matlab functions
        size1=size(mtlbref_fun,1)
        size2=size(mtlbtool_fun,1)
        size3=size(not_mtlb_fun,1)

        if size(mtlbref_fun,"*")<>0 then
            mtlbref_fun(:,1) = """"+mtlbref_fun(:,1)+"""";
            mtlbref_fun12 = mtlbref_fun(:,1) + mtlbref_fun(:,2);
        else
            mtlbref_fun12 = [];
        end
        if size(mtlbtool_fun,"*")<>0 then
            mtlbtool_fun(:,1) = """"+mtlbtool_fun(:,1)+"""";
            mtlbtool_fun12 = mtlbtool_fun(:,1) + mtlbtool_fun(:,2);
        else
            mtlbtool_fun12 = [];
        end
        if size(not_mtlb_fun,"*")<>0 then
            not_mtlb_fun(:,1) = """"+not_mtlb_fun(:,1)+"""";
            not_mtlb_fun12 = not_mtlb_fun(:,1) + not_mtlb_fun(:,2);
        else
            not_mtlb_fun12 = [];
        end

        info_resume = [msprintf(gettext("****** %s: Functions of mfile2sci() session ******"),fnam);
        "*";
        msprintf(gettext("%d Matlab Function(s) not yet converted, original calling sequence used:"),size1);
        mtlbref_fun12;
        "*";
        msprintf(gettext("%d Matlab Toolbox(es) Functions, original calling sequence used :"),size2);
        mtlbtool_fun12;
        "*";
        msprintf(gettext("%d Unknown Function(s), original calling sequence used :"),size3);
        not_mtlb_fun12;
        "*"]

        write(resume_logfile,margin+info_resume)
        file("close",resume_logfile)
        mdelete(pathconvert(TMPDIR)+gettext("resumelogfile.dat"))
        //end

        m2sci_info(gettext("Macro to tree conversion: Done"),-1);

        crp(1) = ""; // Delete function prototype
        if isempty(firstline) then
            res = [hdr;crp]
        else
            hdr(1) = hdr(1)+" "+crp(2);
            crp(2) = [];
            res = [hdr;crp];
        end

        // Strip last return and blank lines
        n = size(res,1)
        while res(n)==part(" ",1:length(res(n))) then
            n = n-1
        end
        res = res(1:n)

        // Write sci-file
        ext = ".sci"
        scifil = results_path + fnam + ext
        mputl(res, scifil);

        // Write sci_<mname>.sci translation file
        if trad<>[] then
            sci_fil = results_path + "sci_" + mname + ".sci"
            mputl(trad,sci_fil);
            res = 1
        else
            res = 0
        end

        // Output summary information
        infos = []
        if m2sci_infos(1) then
            infos = gettext("Translation may be improved: see the //! comments and for all mtlb_<funname> function call\n  Type help mtlb_<funname> in Scilab command window to get information about improvements.");
        end
        if m2sci_infos(2) then
            infos = [infos ; gettext("Translation may be wrong (see the //!! comments).")]
        end

        nametbl($) = []

    else
        infos = gettext("File contains no instruction, no translation made...");
    end

    // End of translation messages
    mss = gettext("****** End of mfile2sci() session ******");

    m2sci_info([infos;mss],-1);

    if Reclevel>1 then
        m2sci_infos = m2sci_infos_save
    end

    file("close", logfile)
    clearglobal m2sci_infos  mtlbref_fun  mtlbtool_fun  not_mtlb_fun
    clearglobal m2sci_Mtoolboxes_funs_db
    
    // For execution called by translatepaths()
    //nametbl = resume(nametbl)
    mdelete(pathconvert(TMPDIR)+fnam+ ".tree")
    mdelete(pathconvert(TMPDIR)+"logfile.dat")

endfunction

// ---------------------------------------------------------------------------

function funcname = getMacroNameFromPrototype(proto)
    // PRIVATE UTILITY FUNCTION called only by mfile2sci()
    //
    // Extraction of the macro's name
    tmp = tokens(proto,["(" "=" ")"]);
    if size(tmp,1)>1
        [?,?,?,funcname] = regexp(proto, "/(?:.*?=(.*?)\(|(.*?)\(|[^(]+?=\s*([^(]+))/","o");
        funcname = stripblanks(funcname);
        funcname(funcname=="") = [];
    else
        funcname = proto;
    end
    // proto = "[b,hy] = fun2(bar=3)";
    // proto = "fun3 (bar=3)";
    // proto = "a = fun4 ()";
    // proto = "fun5";
    // proto = "a = fun6"; // from bug_2341 use case
endfunction

// ---------------------------------------------------------------------------

function instr = transformtree(instr)
    // PRIVATE UTILITY FUNCTION called only by mfile2sci()
    //
    // Copyright (C) 2004-2006 - INRIA - Farid BELAHCENE
    //
    //TRANSFORMTREE function
    //This function research and transform the equal instructions(if the lhs are a multi_operation and expression is a funcall)
    //of the matlab tree to a sup_equal instructions
    //sup_equal is a tlist : tlist([sup_equal,sup_instr,nb_op],sup_instr,nb_op)
    //i.e : the equal instruction [a(1),b(2:3)] = f() is replaced by
    //sup_equal, whith sup_intr list is composed to :
    //[%v1,%v2] = f()
    //a(1) = %v1
    //b(2:3) = %v2
    //and nb_op is: the number of insert operation (in this case 2)
    //Input
    //instr : instruction of matlab tree before tranformation
    //Output
    //instr : instruction of matlab tree after transformation

    Unknown = -1;
    // Browse all the instrucions of the matlab tree:
    if typeof(instr)=="ifthenelse" then
        for i = 1:size(instr.then)
            instr.then(i) = transformtree((instr.then(i)))
        end
        for i = 1:size(instr.elseifs)
            for k = 1:size(instr.elseifs(i).then)
                instr.elseifs(i).then(k) = transformtree((instr.elseifs(i).then(k)))
            end
        end
        for i = 1:size(instr.else)
        instr.else(i) = transformtree((instr.else(i)))
        end
    elseif typeof(instr)=="selectcase" then
        for i = 1:size(instr.cases)
            for j = 1:size(instr.cases(i).then)
                instr.cases(i).then(j) = transformtree((instr.cases(i).then(j)))
            end
        end
        for i = 1:size(instr.else)
        instr.else(i) = transformtree(instr.else(i))
        end
    elseif typeof(instr)=="while" then
        for i = 1:size(instr.statements)
            instr.statements(i) = transformtree(instr.statements(i))
        end
    elseif typeof(instr)=="for" then
        for i = 1:size(instr.statements)
            instr.statements(i) = transformtree(instr.statements(i))
        end
        //instruction is an equal instruction
    elseif typeof(instr)=="equal" then
        if typeof(instr.expression)=="funcall" then //expression is a funcall
            nb_opr = 0;
            for ind = 1:size(instr.lhs)
                if typeof(instr.lhs(ind))=="operation" then
                    nb_opr = nb_opr+1
                end
            end
            if nb_opr>1 then //more than one lhs insert operation
                sup_instr = list("");
                lhstemp = list();
                for j = 1:size(instr.lhs)
                    if typeof(instr.lhs(j))=="operation" then
                        x = gettempvar();
                        sup_instr($+1) = Equal(list(instr.lhs(j)),x);
                        lhstemp(j) = x;
                    else
                        lhstemp(j) = instr.lhs(j)
                    end
                end
                sup_instr(1) = Equal(lhstemp,instr.expression)
                //creation of the sup_equal
                instr = tlist(["sup_equal","sup_instr","nb_opr"],sup_instr,nb_opr)
            end
        end
    end
endfunction

// ---------------------------------------------------------------------------

function bval = multi_fun_file(fil,res_path,Recmode,only_double,verbose_mode,prettyprintoutput)
    // PRIVATE UTILITY FUNCTION called only by mfile2sci()
    //
    // This function converts M-Files containing more than one function
    // Inputs are the same as mfile2sci()
    // Outputs :
    // - bval: boolean value, %t if file contains more than on function, %f else

    sciparam();

    // File name
    k = strindex(fil,".")
    if k<>[]
        ke = k($)-1
        base_name = part(fil,1:ke)
    else
        ke = length(fil)
        base_name = fil
    end
    // File path
    k = strindex(fil,"/")
    if k==[] then
        file_path = "./"
    else
        file_path = part(fil,1:k($))
        base_name = part(base_name,k($)+1:ke)
    end

    txt = mgetl(fil);

    kf = grep(txt,["function[","function "])

    if isempty(kf) then
        // Batch file
        bval = %f
    elseif size(kf,"*")==1 then
        // Only one function defined
        bval = %f
    else
        funcdecl = []
        for kk = kf
            ind = strindex(txt(kk),["function[";"function "])
            if isacomment(txt(kk))==0 & ~isinstring(txt(kk),ind) & part(stripblanks(txt(kk),%T),1:8)=="function"  then // function prototype
                funcdecl = [funcdecl kk]
            end
        end

        if isempty(funcdecl) then
            // "function" only exists in comments and strings
            bval = %f
            return
        elseif size(funcdecl,"*")==1 then
            bval = %f
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
        funcdecl = [funcdecl size(txt,"*")+1]

        tmpfiles = []
        for k = 1:size(funcdecl,"*")-1
            functxt = txt(funcdecl(k):funcdecl(k+1)-1)
            str=  strindex(txt(funcdecl(k)),"(")
            if str==[] then
                funcname = stripblanks(part(txt(funcdecl(k)),strindex(txt(funcdecl(k)),["function[","function "])+8:length(txt(funcdecl(k)))))
            else
                funcname = stripblanks(part(txt(funcdecl(k)),strindex(txt(funcdecl(k)),["function[","function "])+8:str(1)-1))
            end

            keq = strindex(funcname,"=")
            if ~isempty(keq) then
                funcname = stripblanks(part(funcname,keq+1:length(funcname)))
            end
            tmpfiles = [tmpfiles;funcname]
            mputl(functxt,pathconvert(TMPDIR)+base_name+sep+tmpfiles($)+".m");
        end

        write(%io(2),msprintf(gettext(" -- Each function converted separately: %s -- "),strcat(tmpfiles," ")));
        write(%io(2),msprintf(gettext(" -- Temporary files put in: %s -- "),pathconvert(TMPDIR)));

        // Conversion of each file

        for k = 1:size(tmpfiles,"*")
            txt = mgetl(pathconvert(TMPDIR)+base_name+sep+tmpfiles(k)+".m")
            //mfile2sci(pathconvert(TMPDIR)+tmpfiles(k)+".m",res_path,Recmode,only_double,verbose_mode,prettyprintoutput)
        end

        translatepaths(pathconvert(TMPDIR)+base_name,pathconvert(TMPDIR)+base_name)

        txt = []
        if isfile(pathconvert(TMPDIR)+base_name+sep+"log") then
            txt = mgetl(pathconvert(TMPDIR)+base_name+sep+"log")
        end
        mputl(txt,res_path+"log");
        if isfile(pathconvert(TMPDIR)+base_name+sep+"resumelog") then
            txt = mgetl(pathconvert(TMPDIR)+base_name+sep+"resumelog")
        end
        mputl(txt,res_path+"resumelog");

        // Catenation of all .sci files to have only one output file
        txt = []
        for k = 1:size(tmpfiles,"*")
            txt = [txt ;"";mgetl(pathconvert(TMPDIR)+base_name+sep+tmpfiles(k)+".sci")]
        end

        // Delete useless .sci files
        //for k = 1:size(tmpfiles,"*")
        //mdelete(res_path+tmpfiles(k)+".sci")
        //end

        mputl(txt,res_path+base_name+".sci");

        // Catenation of all .log files to have only one output file
        //if exists("logfile")==0 then
        //txt = []
        //for k = 1:size(tmpfiles,"*")
        //txt = [txt ; mgetl(pathconvert(TMPDIR)+base_name+sep+"m2sci_"+tmpfiles(k)+".log")]
        //end

        // Delete useless .log files
        //for k = 1:size(tmpfiles,"*")
        //mdelete(pathconvert(TMPDIR)+base_name+sep+"m2sci_"+tmpfiles(k)+".log")
        //end

        //mputl(txt,res_path+"m2sci_"+base_name+".log");
        //end

        // Catenation of all resume.log files to have only one output file
        //if exists("resume_logfile")==0 then
        //txt = []
        //for k = 1:size(tmpfiles,"*")
        //txt = [txt ; mgetl(res_path+"m2sci_"+tmpfiles(k)+"_resume.log")]
        //end

        // Delete useless _resume.log files
        //for k = 1:size(tmpfiles,"*")
        //mdelete(res_path+"m2sci_"+tmpfiles(k)+"_resume.log")
        //end

        //mputl(txt,res_path+"m2sci_"+base_name+"_resume.log");
        //end

        // Delete useless .m files
        //for k = 1:size(tmpfiles,"*")
        //mdelete(pathconvert(TMPDIR)+tmpfiles(k)+".m")
        //end

        rmdir(pathconvert(TMPDIR)+base_name,"s")
        for k = 1:size(tmpfiles,"*")
            mdelete(pathconvert(TMPDIR)+tmpfiles(k)+".tree")
        end
    end

endfunction
