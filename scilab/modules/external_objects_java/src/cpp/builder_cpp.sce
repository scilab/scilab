// Copyright (C) 2010 - 2011 - Calixte DENIZET <calixte@contrib.scilab.org>
// Copyright (C) 2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
function build_cpp()
    LDFLAGS = "";
    CFLAGS = "";
    if getos() == "Windows" then
        CFLAGS = "-D_MSC_VER";
    end
    here = get_absolute_file_path("builder_cpp.sce");

    src_cpp = ["WrapAsDirectBuffer.cpp"; ..
    "ScilabObjectsCPP.cpp"; ..
    "NoMoreScilabMemoryException.cpp"; ..
    "OptionsHelper.cpp"];

    src_jni = ["ScilabJavaObject.cpp"; ..
    "ScilabJavaCompiler.cpp"; ..
    "ScilabJavaClass.cpp"; ..
    "ScilabJavaArray.cpp"; ..
    "ScilabClassLoader.cpp"; ..
    "GiwsException.cpp"];

    if (getos() == "Windows") then
        src_cpp = [src_cpp; ..
        "dllMain.cpp"];
    end

    jdk_home = getenv("JAVA_HOME", "");
    if (getos() <> "Windows") then
        if jdk_home <> "" then
            if ~isdir(fullpath(jdk_home + "/include")) then
                jdk_home = jdk_home + "/../";
            end
        end
    end

    if jdk_home == "" then
        if getos() <> "Darwin" then
            jdk_home = fullpath(system_getproperty("java.home") + "/../");
        else
            jdk_home = fullpath(system_getproperty("java.home"));
        end
    end


    if ~isdir(fullpath(jdk_home + "/include")) then
        error("JDK include directory not found.")
    end

    if (getos() == "Windows") then
        CFLAGS = "-I" + jdk_home + "\include" + " -I" + jdk_home + "\include\win32";
    elseif (getos() == "Linux") then
        CFLAGS = "-I" + jdk_home + "/include" + " -I" + jdk_home + "/include/linux";
    else
        CFLAGS = "-I" + jdk_home + "/include";
    end

    if getos() <> "Windows" then
        LDFLAGS = "-L" + fullpath(here + "../c/") + " " + ..
        "-L" + fullpath(here + "../../sci_gateway/c");
    end
    CFLAGS = CFLAGS + " -I" + fullpath(here) + " -I" + fullpath(here + "../jni");
    CFLAGS = CFLAGS + " -I" + fullpath(here + "../include/") + " -I"  + fullpath(jdk_home + "/include") + " -I" + fullpath(SCI + "/modules/jvm/includes");

    tbx_build_src("jims_cpp", [src_cpp; src_jni], "cpp", here, "", LDFLAGS, CFLAGS);
endfunction

build_cpp();
clear build_cpp
