// This file is released under the 3-clause BSD license. See COPYING-BSD.

// This macro compiles JAR from Java files

function builder_java()
    src_java_dir = get_absolute_file_path("builder_java.sce");

    curdir = pwd();
    cd(src_java_dir);

    jar_dir = fullpath(fullfile(src_java_dir, "../../jar"));
    if ~isdir(jar_dir)
        mkdir(jar_dir);
    end

    package_name = "org.scilab.contrib.toolboxskeleton";
    jar_file_path = fullfile(jar_dir, package_name + ".jar");
    ilib_build_jar(jar_file_path, package_name, src_java_dir);

    cd(curdir);
endfunction

builder_java();
clear builder_java;

