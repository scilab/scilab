// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Simon LIPP <simon.lipp@scilab.org>
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// Run the src_path+"/src/"+lang+"builder_"+lang+".sce" script if it exists

function tbx_builder_src_lang(languages,path)

    // Number of input arguments

    rhs = argn(2);

    if and(rhs <> [1 2]) then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"tbx_builder_src_lang",1,2));
    end

    // Input argument N°1
    // → languages

    if type(languages) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A string array expected.\n"),"tbx_builder_src_lang",1));
    end

    // Input argument N°2
    // → path

    if rhs < 2 then
        path = pwd();

    else
        if type(path) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"tbx_builder_src_lang",2));
        end

        if size(path,"*") <> 1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"tbx_builder_src_lang",2));
        end

        if ~isdir(path) then
            error(msprintf(gettext("%s: The directory ''%s'' doesn''t exist or is not read accessible.\n"),"tbx_builder_src_lang",path));
        end
    end

    // Check path/languages

    directories = pathconvert(path+"/"+languages,%F);

    for i=1:size(directories,"*") do
        if ~isdir(directories(i)) then
            error(msprintf(gettext("%s: The directory ''%s'' doesn''t exist or is not read accessible.\n"),"tbx_builder_src_lang",directories(i)));
        end
    end

    // Exec builders files
    //look for all files of type builder_something.sce and executes them
    builder_files = [];
    for i = 1:size(directories, "*")
        builder_file_i = findfiles(directories(i), "builder*.sce");
        if ~isempty(builder_file_i)
            builder_file_i = directories(i) + "/" + builder_file_i;
        end
        builder_files = [builder_files; builder_file_i];
    end

    if ~isempty(builder_files)
        tbx_builder(builder_files);
    else
        // No builder files found: try default building
        for i=1:size(languages, "*")
            select languages(i)
            case "c"
                tbx_builder_src_c(path)
            case "cpp"
                tbx_builder_src_cpp(path)
            case "java"
                tbx_builder_src_java(path)
            case "fortran"
                tbx_builder_src_fortran(path)
            else
                warning(_("No builder file found, nothing to be done."))
            end
        end
    end

endfunction

function tbx_builder_src_c(path)
    src_c_path = fullpath(path + "/c");
    cflags = ilib_include_flag(src_c_path);

    // default functions and name
    source_files = findfiles(src_c_path, "*.c");
    if ~isempty(source_files)
        tbx_build_src(basename(source_files), ..
        source_files, ..
        "c",                ..
        src_c_path,         ..
        "",                 ..
        "",                 ..
        cflags);
    end
endfunction

function tbx_builder_src_cpp(path)
    src_cpp_path = fullpath(path + "/cpp");
    cflags = ilib_include_flag(src_cpp_path);

    // default functions and name
    source_files = findfiles(src_cpp_path, "*.cxx");
    source_files = [source_files; findfiles(src_cpp_path, "*.cpp")];
    if ~isempty(source_files)
        tbx_build_src(basename(source_files), ..
        source_files, ..
        "cpp",                ..
        src_cpp_path,         ..
        "",                 ..
        "",                 ..
        cflags);
    end
endfunction

function tbx_builder_src_java(path)
    src_java_path = fullpath(path + "/java");

    jar_dir = fullpath(fullfile(src_java_path, "../../jar"));
    if ~isdir(jar_dir)
        mkdir(jar_dir);
    end

    curdir = pwd();
    cd(src_java_path);
    package_dirs = listfiles(src_java_path + "/org/scilab/contrib/");
    if ~isempty(package_dirs)
        // packages must be under org/scilab/contrib/packagename/
        package_name = "org.scilab.contrib." + package_dirs;
        jar_file_path = fullfile(jar_dir, package_name + ".jar");
        ilib_build_jar(jar_file_path, package_name, src_java_path);
    end
    cd(curdir);
endfunction

function tbx_builder_src_fortran(path)
    src_fortran_path = fullpath(path + "/fortran");

    // default functions and name
    source_files = findfiles(src_fortran_path, "*.f");
    if ~isempty(source_files)
        tbx_build_src(basename(source_files), ..
        source_files, ..
        "f",          ..
        src_fortran_path ..
        );
    end
endfunction
