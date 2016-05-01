// Copyright (C) DIGITEO - 2010-2011 - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//=============================================================================
function Makename = dlwGenerateMakefile(name, ..
    tables, ..
    files, ..
    libs, ..
    libname, ..
    with_gateway, ..
    ldflags, ..
    cflags, ..
    fflags, ..
    cc)

    [lhs,rhs] = argn(0);
    if rhs < 4 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): at least %d expected.\n"), "dlwGenerateMakefile", 4));
    end

    if argn(2) < 6 then
        with_gateway = %t;
        ldflags = "";
        cflags = "";
        fflags = "";
        cc = "";
    end

    if ~isdef("makename") then
        makename = "";
    end

    // change table if necessary
    if tables <> [] then
        if typeof(tables)<>"list" then
            tables = list(tables)
        end
        L = length(tables);

        for it = 1:L
            table = tables(it);
            [mt, nt] = size(table);

            if nt == 2 then
                col= "csci";
                table = [table, col(ones(mt,1))];
                nt=3;
            end

            if nt<>3 then
                error(msprintf(gettext("%s: Wrong size for input argument #%d.\n") ,"dlwGenerateMakefile", 2));
            end
            tables(it) = table;
        end
    end

    if isempty(makename) then
        Makename = dlwGetDefltMakefileName() + dlwGetMakefileExt();
    else
        Makename = makename + dlwGetMakefileExt();
    end

    if length(libname) > 0  & strncpy(libname, 3) <> "lib" then
        libname = "lib" + libname;
    end

    if getenv("__USE_DEPRECATED_STACK_FUNCTIONS__", "NO") == "YES" then
        cflags = cflags + " -D__USE_DEPRECATED_STACK_FUNCTIONS__";
    end

    ilib_gen_Make_win32(name, tables, files, libs, libname, Makename, with_gateway, ldflags, cflags, fflags)

endfunction
//=============================================================================
function ilib_gen_Make_win32(name, ..
    table, ..
    files, ..
    libs, ..
    libname, ..
    Makename, ..
    with_gateway, ..
    ldflags, ..
    cflags, ..
    fflags)

    managed_ext = [".cxx", ".cpp", ".c", ".f90", ".f"];

    SCIDIR = SCI;
    LIBRARY = name;
    FILES_SRC = "";
    OBJS = "";
    OBJS_WITH_PATH = "";
    FORTRAN_RUNTIME = "";
    OTHERLIBS = "";
    CC = "";
    CFLAGS = cflags;
    MEXCFLAGS = "";
    FFLAGS = fflags;
    MEXFFLAGS = "";
    LDFLAGS = ldflags;

    SCILAB_INCLUDES = dlwGetScilabIncludes();
    SCILAB_INCLUDES = "-I""" + SCILAB_INCLUDES + """";
    SCILAB_INCLUDES = [SCILAB_INCLUDES(1:$-1) + " \"; SCILAB_INCLUDES($)];
    SCILAB_INCLUDES = strcat(SCILAB_INCLUDES, ascii(10));

    SCILAB_LIBS = dlwGetScilabLibraries();
    SCILAB_LIBS = """$(SCIDIR)/bin/" + SCILAB_LIBS + """";
    SCILAB_LIBS = [SCILAB_LIBS(1:$-1) + " \"; SCILAB_LIBS($)];
    SCILAB_LIBS = strcat(SCILAB_LIBS, ascii(10));

    if isempty(libname) then
        LIBRARY = name;
    else
        LIBRARY = libname;
    end

    FILES_SRC_MATRIX = [];

    [path_Make, file_Make, ext_Make] = fileparts(Makename);

    for i=1:size(files,"*")
        [path_f, file_f, ext_f] = fileparts(files(i));
        FILENAME = [];
        FILE_FOUNDED = %f;
        for y = managed_ext(:)'
            if (FILE_FOUNDED == %f) then
                if (fileinfo(path_f + file_f + y) <> []) | (fileinfo(path_Make + file_f + y) <> []) then
                    FILENAME = path_f + file_f + y;
                    FILE_FOUNDED = %t;
                end
            end
        end
        FILES_SRC_MATRIX = [FILES_SRC_MATRIX , FILENAME];
    end

    if typeof(tables) <> "list" then
        tables = list(tables);
    end
    L = length(tables);

    if with_gateway then
        if L == 1 then
            FILES_SRC_MATRIX = [FILES_SRC_MATRIX , name + ".cpp"];
        else
            for i=1:L
                FILES_SRC_MATRIX = [FILES_SRC_MATRIX , name + string(i) + ".cpp"];
            end
        end
    end

    for it=1:L
        table = tables(it);
        [mt,nt] = size(table);

        for i=1:mt
            if table(i,3)=="cmex" | table(i,3)=="fmex" | table(i,3)=="Fmex" then
                MEXCFLAGS = "-Dmexfunction_=mex" + table(i,2) + "_ -DmexFunction=mex_" + table(i,2);
                MEXFFLAGS = "-Dmexfunction=mex" + table(i,2);
                if table(i,3)=="cmex" then
                    if find(listfiles() == table(i,2)+".cpp") then
                        filenameMex = table(i,2) + ".cpp";
                    else
                        filenameMex = table(i,2) + ".c";
                    end
                else
                    filenameMex = table(i,2) + ".f";
                end
                if grep(FILES_SRC_MATRIX,filenameMex) == [] then
                    FILES_SRC_MATRIX = [FILES_SRC_MATRIX , filenameMex];
                end
            end
        end
    end

    if isempty(FILES_SRC_MATRIX) | ~and(isfile(FILES_SRC_MATRIX)) then
        error(999, msprintf(_("%s: Wrong value for input argument #%d: existing file(s) expected.\n"), "ilib_gen_Make", 3));
    end

    //update DEBUG_SCILAB_DYNAMIC_LINK to map with Scilab compilation mode
    val = getenv("DEBUG_SCILAB_DYNAMIC_LINK","");
    if val <> "YES" & val <> "NO" & isDebug() then
        setenv("DEBUG_SCILAB_DYNAMIC_LINK","YES");
        CFLAGS = CFLAGS + " -D_DEBUG";
    else
        setenv("DEBUG_SCILAB_DYNAMIC_LINK","");
        CFLAGS = CFLAGS + " -DNDEBUG";
    end

    // remove duplicated files
    FILES_SRC_MATRIX = unique(FILES_SRC_MATRIX);

    FILES_SRC = strcat(FILES_SRC_MATRIX," ");

    OBJ_DEST_PATH = "";
    if (getenv("DEBUG_SCILAB_DYNAMIC_LINK","") == "YES") then
        OBJ_DEST_PATH = "Debug/";
    else
        OBJ_DEST_PATH = "Release/";
    end

    OBJS_MATRIX = [];
    OBJS_WITH_PATH_MATRIX = [];

    for y = 1:size(FILES_SRC_MATRIX,"*")
        [path_f, file_f, ext_f] = fileparts(FILES_SRC_MATRIX(y));
        OBJS_MATRIX = [OBJS_MATRIX, path_f + file_f + ".obj"];
        OBJS_WITH_PATH_MATRIX = [OBJS_WITH_PATH_MATRIX, OBJ_DEST_PATH + path_f + file_f + ".obj"];
    end

    OBJS = strcat(OBJS_MATRIX, " ");
    OBJS_WITH_PATH =  strcat(OBJS_WITH_PATH_MATRIX, " ");

    if ( or(fileext(FILES_SRC_MATRIX) == ".f90") | or(fileext(FILES_SRC_MATRIX) == ".f") ) then
        if findmsifortcompiler() <> "unknown" then
            if (getenv("DEBUG_SCILAB_DYNAMIC_LINK","NO") == "NO") then
                FORTRAN_RUNTIME = "libifcoremd.lib libmmd.lib";
            else
                FORTRAN_RUNTIME = "libifcoremdd.lib libmmdd.lib";
            end
        end
    end

    for x=libs(:)'
        if (x <> [] & x <> "") then
            if OTHERLIBS <> "" then
                OTHERLIBS = OTHERLIBS + " " + x + ".lib";
            else
                OTHERLIBS = x + ".lib";
            end
        end
    end

    if isfile(SCI+"/modules/dynamic_link/src/scripts/TEMPLATE_MAKEFILE.VC") then
        MAKEFILE_VC = mgetl(SCI+"/modules/dynamic_link/src/scripts/TEMPLATE_MAKEFILE.VC");
    else
        MAKEFILE_VC = "";
    end

    MAKEFILE_VC = strsubst(MAKEFILE_VC, "__SCI__", SCIDIR);
    MAKEFILE_VC = strsubst(MAKEFILE_VC, "__SCILAB_INCLUDES__", SCILAB_INCLUDES);
    MAKEFILE_VC = strsubst(MAKEFILE_VC, "__SCILAB_LIBS__",SCILAB_LIBS);
    MAKEFILE_VC = strsubst(MAKEFILE_VC, "__LIBNAME__", LIBRARY);
    MAKEFILE_VC = strsubst(MAKEFILE_VC, "__FILES_SRC__", FILES_SRC);
    MAKEFILE_VC = strsubst(MAKEFILE_VC, "__OBJS__", OBJS);
    MAKEFILE_VC = strsubst(MAKEFILE_VC, "__OBJS_WITH_PATH__", OBJS_WITH_PATH);
    MAKEFILE_VC = strsubst(MAKEFILE_VC, "__FORTRAN_RUNTIME__", FORTRAN_RUNTIME);
    MAKEFILE_VC = strsubst(MAKEFILE_VC, "__OTHERSLIBS__", OTHERLIBS);

    if CC <> "" then
        MAKEFILE_VC = strsubst(MAKEFILE_VC, "__CC__",CC);
    else
        MAKEFILE_VC = strsubst(MAKEFILE_VC, "CC = __CC__","#CC = ");
    end

    CFLAGS = CFLAGS + " -D" + convstr(name, "u") + "_GW_EXPORTS";
    MAKEFILE_VC = strsubst(MAKEFILE_VC, "__CFLAGS__", CFLAGS);
    MAKEFILE_VC = strsubst(MAKEFILE_VC, "__MEXCFLAGS__", MEXCFLAGS);
    MAKEFILE_VC = strsubst(MAKEFILE_VC, "__FFLAGS__", FFLAGS);
    MAKEFILE_VC = strsubst(MAKEFILE_VC, "__MEXFFLAGS__", MEXFFLAGS);
    MAKEFILE_VC = strsubst(MAKEFILE_VC, "__LDFLAGS__", LDFLAGS);

    if ( MAKEFILE_VC <> "") then
        mputl(MAKEFILE_VC, Makename);

        if ilib_verbose() > 1 then
            disp(MAKEFILE_VC);
        end

    else
        // TEMPLATE_MAKEFILE.VC not found
        warning(SCI+"/modules/dynamic_link/src/scripts/TEMPLATE_MAKEFILE.VC"+ _("not found.") );
    end

    //restore DEBUG_SCILAB_DYNAMIC_LINK
    setenv("DEBUG_SCILAB_DYNAMIC_LINK", val);

endfunction
//=============================================================================
