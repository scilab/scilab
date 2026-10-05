// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// Runs the JUnit tests of a module from a .tst file:
//   exec(SCI + "/modules/development_tools/tests/unit_tests/javaHelpers.sce", -1);
//   run_junit_tests(SCI + "/modules/<module>/tests/java/<path>/<Class>.java");

function jars = junit_scilab_classpath()
    // the jars Scilab itself loads, with paths valid in a source tree or an installed version
    doc = xmlRead(SCI + "/etc/classpath.xml");
    paths = xmlXPath(doc, "//path");
    jars = [];
    for i = 1:size(paths, "*")
        jars($+1) = paths(i).attributes.value;
    end
    xmlDelete(doc);
    jars = strsubst(jars, "$SCILAB", SCI);
    jars = jars(isfile(jars));
endfunction

function opts = junit_jvm_options()
    // the options Scilab starts its JVM with (class loader, add-opens...), except the paths set by run_junit_tests
    os = convstr(getos());
    if os == "darwin" then
        os = "macosx";
    end
    doc = xmlRead(SCI + "/etc/jvm_options.xml");
    options = xmlXPath(doc, "//option");
    opts = [];
    for i = 1:size(options, "*")
        a = options(i).attributes;
        if a.os == [] | a.os == os then
            opts($+1) = a.value;
        end
    end
    xmlDelete(doc);
    opts(grep(opts, ["-Djava.library.path=", "-Djava.class.path="])) = [];
    opts = strsubst(strsubst(opts, "$SCILAB", SCI), "\", "/");
endfunction

function jar = junit_jar(ecj)
    // not in classpath.xml: shipped in thirdparty, next to ecj
    candidates = [SCI + "/thirdparty/"; SCI + "/../../thirdparty/"; fileparts(ecj, "path")] + "junit-4.10.jar";
    jar = candidates(isfile(candidates));
    if jar == [] then
        error(["run_junit_tests: junit-4.10.jar not found in:"; candidates]);
    end
    jar = jar(1);
endfunction

function files = junit_sources(paths)
    // .java files given directly, or found recursively in the given directories
    files = [];
    for p = paths(:)'
        if isdir(p) then
            java = findfiles(p, "*.java");
            if java <> [] then
                files = [files; p + "/" + java];
            end
            for d = findfiles(p)'
                if isdir(p + "/" + d) then
                    files = [files; junit_sources(p + "/" + d)];
                end
            end
        else
            files($+1) = p;
        end
    end
endfunction

function names = junit_class_names(srcFiles)
    names = [];
    for f = srcFiles'
        txt = mgetl(f);
        [s, e, m, pkg] = regexp(txt(grep(txt, "/^\s*package\s/", "r")), "/^\s*package\s+([\w.]+)\s*;/");
        if pkg == [] then
            names($+1) = fileparts(f, "fname");
        else
            names($+1) = pkg(1) + "." + fileparts(f, "fname");
        end
    end
endfunction

function env = junit_preload()
    // the JVM loads native libraries with RTLD_LOCAL, so the Scilab symbols they need must be preloaded
    env = "";
    if getos() <> "Linux" then
        return
    end
    candidates = SCI + ["/../../lib/scilab/libscilab.so"; "/modules/.libs/libscilab.so"];
    lib = candidates(isfile(candidates));
    if lib == [] then
        error(["run_junit_tests: libscilab.so not found in:"; candidates]);
    end
    env = "LD_PRELOAD=""" + lib(1) + """ ";
endfunction

function [status, out] = junit_java(java, args, env)
    // arguments go through an @argfile: the classpath exceeds the Windows command line length
    argfile = tempname() + ".args";
    mputl(args, argfile);
    if ~isdef("env", "l") then
        env = "";
    end
    [status, stdout, stderr] = host(env + """" + java + """ ""@" + argfile + """");
    out = [stdout; stderr];
    deletefile(argfile);
endfunction

function run_junit_tests(srcFiles)
    // compile srcFiles (files or directories) with ecj, then run them with JUnitCore in a separate JVM, as `ant test` does
    srcFiles = junit_sources(srcFiles);
    if srcFiles == [] then
        error("run_junit_tests: no .java file found");
    end
    srcFiles = strsubst(fullpath(srcFiles), "\", "/");
    q = """";
    sep = pathsep();

    cp = strsubst(junit_scilab_classpath(), "\", "/");
    ecj = cp(grep(cp, "/\/ecj[^\/]*\.jar$/", "r"));
    if ecj == [] then
        error("run_junit_tests: ecj not found in " + SCI + "/etc/classpath.xml");
    end
    ecj = ecj(1);
    cp = [strsubst(junit_jar(ecj), "\", "/"); cp];

    java = system_getproperty("java.home") + "/bin/java";
    if getos() == "Windows" then
        java = java + ".exe";
    end

    classes = strsubst(tempname(), "\", "/");
    deletefile(classes);
    mkdir(classes);

    // ecj is launched as a jar, so its arguments follow -jar on the command line
    [status, out] = junit_java(java, ["-jar"; q + ecj + q; "-17"; "-nowarn"; "-encoding"; "UTF-8"; ..
    "-cp"; q + strcat(cp, sep) + q; "-d"; q + classes + q; q + srcFiles + q]);
    if status <> 0 then
        error(["run_junit_tests: compilation failed (exit status " + string(status) + ")"; out]);
    end

    libpath = strsubst(system_getproperty("java.library.path"), "\", "/");
    [status, out] = junit_java(java, [junit_jvm_options(); "-Djava.library.path=" + q + libpath + q; ..
    "-cp"; q + strcat([classes; cp], sep) + q; ..
    "org.junit.runner.JUnitCore"; junit_class_names(srcFiles)], junit_preload());
    rmdir(classes, "s");
    if status <> 0 then
        out(grep(out, "/^\s+at org\.junit\./", "r")) = [];
        error(["run_junit_tests: JUnit tests failed (exit status " + string(status) + ")"; out]);
    end
endfunction
