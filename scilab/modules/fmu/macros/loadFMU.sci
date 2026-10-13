function [workdir, xml, libname, modelIdentifier, fmuImpl] = loadFMU(fname, workdir, fmuImpl)
    [lhs, rhs] = argn()
    if rhs < 1 then
        msg = gettext("%s: Wrong number of input arguments: %d to %d expected.\n");
        error(msprintf(msg, "loadFMU", 1, 3));
    end

    // if the file does not exist, resolve to :
    //  - ./resources for local files
    //  - TMPDIR/resources for SSP support
    //  - fmigetPath() for the fmu module demos and tests
    filepath = fname;
    if ~isfile(filepath) then
        filepath = fullfile("resources", fname);
    end
    if ~isfile(filepath) then
        filepath = fullfile(TMPDIR, "resources", fname);
    end
    if ~isfile(filepath) then
        filepath = fullfile(fmigetPath(), fname);
    end
    if ~isfile(filepath) then
        msg = gettext("%s: The file %s does not exist.\n");
        error(msprintf(msg, "loadFMU", filepath));
    end

    // create the temp folder if it does not exist
    if rhs < 2 | workdir == [] then
        workdir = tempname("fmu");
        deletefile(workdir);
        mkdir(workdir);
    end
    if ~isdir(workdir) then
        msg = gettext("%s: The directory %s does not exist.\n");
        error(msprintf(msg, "loadFMU", workdir));
    end
    // Use TMPDIR to avoid sharing username on Windows
    if getos() == "Windows" then
        workdir = getlongpathname(workdir);
    end
    workdir = strsubst(workdir, TMPDIR, "TMPDIR");

    // auto-detect the FMU implementation if not specified
    if rhs < 3 then
        fmuImpl = [];
    elseif fmuImpl <> [] then
        if typeof(fmuImpl ) <> "string" then
            msg = gettext("%s: Wrong type for input argument #%d: string expected.\n")
            error(msprintf(msg, "loadFMU", 3));
        end
        if or(size(fmuImpl ) <> [1 1]) then
            msg = gettext("%s: Wrong size for input argument #%d: string expected.\n")
            error(msprintf(msg, "loadFMU", 3));
        end
    end

    if ~isfile(fullfile(workdir, "modelDescription.xml")) then
        decompress(filepath, workdir);
    end
    if ~isfile(fullfile(workdir, "modelDescription.xml")) then
        msg = gettext("%s: Invalid FMU %s, %s does not exist.\n");
        error(msprintf(msg, "loadFMU", filepath, "modelDescription.xml"));
    end
    if lhs < 2 then
        return
    end

    xml = xmlRead(fullfile(workdir, "modelDescription.xml"));
    if lhs < 3 then
        return
    end

    fmiVersion = xmlXPath(xml, "/fmiModelDescription/@fmiVersion")(1).content;

    [version, opts] = getversion();
    select [getos() opts(2) fmiVersion]
    case ["Windows" "x64" "1.0"] then
        arch = "win64"
        so = ".dll"
    case ["Windows" "x64" "2.0"] then
        arch = "win64"
        so = ".dll"
    case ["Windows" "x64" "3.0"] then
        arch = "x86_64-windows"
        so = ".dll"
    case ["Linux" "x64" "1.0"] then
        arch = "linux64"
        so = ".so"
    case ["Linux" "x64" "2.0"] then
        arch = "linux64"
        so = ".so"
    case ["Linux" "x64" "3.0"] then
        arch = "x86_64-linux"
        so = ".so"
    else
        msg = gettext("%s: FMU not supported.\n");
        error(msprintf(msg, "loadFMU"));
    end

    l = xmlXPath(xml, "/fmiModelDescription/@modelIdentifier|/fmiModelDescription/*/@modelIdentifier");
    modelIdentifier = l(1).content;
    if fmiVersion == "1.0" then
        // on FMI v1.0, Implementation is only available in CS
        if size(xmlXPath(xml, "/fmiModelDescription/Implementation")) <> 0 then
            allFmuImpl = "cs 1.0";
        else
            allFmuImpl = "me 1.0";
        end
    else
        // on FMI v2 or later, the implementation is defined as the parent element
        fmiType = convstr(strsubst(l(1).parent.name, "/[a-z]/", "", "r"), "l");
        allFmuImpl = fmiType + " " + fmiVersion;
        for i=2:length(l)
            fmiType = convstr(strsubst(l(i).parent.name, "/[a-z]/", "", "r"), "l");
            allFmuImpl = [allFmuImpl ; fmiType + " " + fmiVersion];
        end
    end

    if fmuImpl <> [] && ~or(allFmuImpl == fmuImpl) then
        msg = gettext("%s: fmuImpl ""%s"" must be in the set {""%s""} defined in %s.\n");
        error(msprintf(msg, "loadFMU", fmuImpl, strcat(allFmuImpl, """, """), fullfile(workdir, "modelDescription.xml")));
    end
    if fmuImpl == [] then
        // if not specified, try to load all the declared simulation type ; fmu_link() will select the "best" one
        fmuImpl = allFmuImpl;
    end

    libname = fullfile(workdir, "binaries", arch, modelIdentifier + so);
    if ~isfile(libname) then
        msg = gettext("%s: Invalid FMU for this platform %s.\n%s does not exist.\n");
        error(msprintf(msg, "loadFMU", filepath, fullfile(workdir, "binaries", arch, modelIdentifier + so)));
    end
endfunction
