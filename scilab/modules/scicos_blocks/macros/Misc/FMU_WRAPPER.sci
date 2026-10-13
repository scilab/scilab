//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2020 - ESI Group - Clément David
// Copyright (C) - 2026 - Dassault Systèmes - Clément David
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
function [x, y, typ] = FMU_WRAPPER(job,arg1,arg2)
    x = []; y = []; typ = [];
    errmsg = [];

    select job
    case 'set' then
        x = arg1;
        model = x.model;
        exprs = x.graphics.exprs;

        if size(exprs, '*') < 2 then
            // a new load has to be done
            fname = uigetfile(["*.fmu"], "", "Load FMU for Simulation");
            [xml, exprs, opar, rpar, ipar, fmuImpl] = load_fmu(fname, exprs, [], []);
        else
            filepath = resolve_fmu_filepath(exprs(1));
            // legacy layout without the FMU implementation, exprs(3) is the logging flag
            if size(exprs, '*') >= 3 && or(exprs(3) == ["%f" "%t"]) then
                exprs = [exprs(1:2) ; "" ; exprs(3:$)];
            end
            // legacy layout without the event iteration limit, exprs(10) is the first parameter valueReference
            if size(exprs, '*') >= 12 && or(exprs(11) == ["Real" "Integer" "Boolean" "String" "Float64" "Float32" "Int64" "Int32" "Int16" "Int8" "UInt64" "UInt32" "UInt16" "UInt8" "Enumeration" "Binary"]) then
                exprs = [exprs(1:9) ; "100" ; exprs(10:$)];
            elseif size(exprs, '*') == 9 then
                exprs(10) = "100";
            end
            // resolve the FMU implementation if previously set
            if size(exprs, '*') >= 3 && exprs(3) <> "" then
                fmuImpl = exprs(3);
            else
                fmuImpl = [];
            end

            if isdir(exprs(2)) then
                // it looks already or partially loaded, get the information back
                [xml, exprs, opar, rpar, ipar, fmuImpl] = load_fmu(filepath, exprs, exprs(2), fmuImpl);
            elseif isfile(filepath) then
                // a reload has to be done
                [xml, exprs, opar, rpar, ipar, fmuImpl] = load_fmu(filepath, exprs, [], fmuImpl);
            elseif isdir(fileparts(filepath)) then
                // reload the fmu from the same existing directory
                fname = uigetfile(["*.fmu"], fileparts(filepath), "Load FMU for Simulation");
                [xml, exprs, opar, rpar, ipar, fmuImpl] = load_fmu(fname, exprs, [], fmuImpl);
            else
                // none of the previously stored information looks correct, a new load has to be done
                fname = uigetfile(["*.fmu"], "", "Load FMU for Simulation");
                [xml, exprs, opar, rpar, ipar, fmuImpl] = load_fmu(fname, exprs, [], fmuImpl);
            end
        end

        if xml == [] then
            return;
        end

        // default parameters are empty
        realRefs = [];
        realNegatedAlias = [];
        intRefs = [];
        intNegatedAlias = [];
        boolRefs = [];
        boolNegatedAlias = [];

        ok = %f;
        while ~ok
            modelName = xmlXPath(xml, "//@modelName")(1).content;
            desc = "Configuration of " + modelName

            // first ask for the common parameters
            labels = ["FMU file" ; "FMU directory" ; "FMU implementation" ; "Enable logging ?" ; "Relative tolerance" ; "Integer as Real" ; "Boolean as Real" ; "String maximum length" ; "Enumeration as Real" ; "Event iteration limit"]
            typ = list("str", 1, "str", 1, "str", 1, "str", -1, "mat", [1 1], "str", -1, "str", -1, "mat", [1 1], "str", -1, "mat", [1 1]);
            ini = exprs(1:10);
            [ok, fmuFile, fmuWorkdir, fmuImpl, loggingOn, relativeTolerance, integerToDouble, booleanToDouble, stringLength, enumToDouble, eventIterationLimit, next] = scicos_getvalue(desc, labels, typ, ini);
            if ~ok then
                return;
            end
            next(1) = portable_fmu_filepath(next(1));

            // the event iteration limit must be positive
            if eventIterationLimit < 1 then
                messagebox([_("FMU_WRAPPER: the event iteration limit must be greater or equal to 1.")], "modal", "error");
                ok = %f;
                continue;
            end

            // reload in case of FMU parameters changed, will display an error messagebox on failure
            if or(ini(1:3) <> next(1:3)) then
                exprs(1:3) = next(1:3);

                filepath = resolve_fmu_filepath(exprs(1));
                [xml, exprs, opar, rpar, ipar, fmuImpl] = load_fmu(filepath, exprs, exprs(2), fmuImpl);
                if xml == [] then
                    return;
                end
                exprs(10) = next(10);
            else
                exprs(1:10) = next;
            end

            // set simulation function
            select fmuImpl
            case "me 1.0" then
                model.sim=list("fmu_simulate_me",4);
            case "me 2.0" then
                model.sim=list("fmu2_simulate_me",4);
            case "me 3.0" then
                model.sim=list("fmu3_simulate_me",4);
            case "cs 1.0" then
                model.sim=list("fmu_simulate_cs",4);
            case "cs 2.0" then
                model.sim=list("fmu2_simulate_cs",4);
            case "cs 3.0" then
                model.sim=list("fmu3_simulate_cs",4);
            else
                messagebox(["Invalid FMU version / mode."],"modal","error");
                ok = %f
                return
            end

            // hasEventMode is used to trigger Event Mode handling
            if fmuImpl == "cs 3.0" then
                r = xmlXPath(xml, "//CoSimulation/@hasEventMode");
                hasEventMode = length(r) == 1 && r(1).content == "true";
            else
                hasEventMode = %f;
            end

            opar(13) = int8([evstr(loggingOn) hasEventMode]);
            opar(16) = relativeTolerance;
            opar(17) = int32(eventIterationLimit);
            if evstr(integerToDouble) then
                integerToDouble = ones(3,2); // all integers to double
            else
                integerToDouble = matrix(3:8,3,2) // mapped to integers
            end
            if evstr(booleanToDouble) then
                booleanToDouble = 1; // double
            else
                booleanToDouble = 8; // uint8
            end
            if evstr(enumToDouble) then
                enumToDouble = 1; // double
            else
                enumToDouble = 6; // uint32
            end

            // second ask for the parameters
            select fmuImpl
            case "me 1.0" then
                xmlParameters = xmlXPath(xml, "//ScalarVariable[@variability = ""parameter""]");
            case "me 2.0" then
                // See Functional Mock-up Interface 2.0.3, Page 51 of 131, table variability \ causality
                //  - causality = parameter (A with default initial)
                //  - initial = exact (C with preset value)
                xmlParameters = xmlXPath(xml, "//ScalarVariable[@causality = ""parameter""]|//ScalarVariable[@initial = ""exact""]");
            case "me 3.0" then
                // See Functional Mock-up Interface 3.0, https://fmi-standard.org/docs/3.0/#HowToTreatTunable "How to treat tunable variables"
                xmlParameters = xmlXPath(xml, "//ModelVariables/*[@causality = ""parameter""]|//ModelVariables/*[@initial = ""exact""]");
            case "cs 1.0" then
                xmlParameters = xmlXPath(xml, "//ScalarVariable[@variability = ""parameter""]");
            case "cs 2.0" then
                // See Functional Mock-up Interface 2.0.3, Page 51 of 131, table variability \ causality
                //  - causality = parameter (A with default initial)
                //  - initial = exact (C with preset value)
                xmlParameters = xmlXPath(xml, "//ScalarVariable[@causality = ""parameter""]|//ScalarVariable[@initial = ""exact""]");
            case "cs 3.0" then
                // See Functional Mock-up Interface 3.0, https://fmi-standard.org/docs/3.0/#HowToTreatTunable "How to treat tunable variables"
                xmlParameters = xmlXPath(xml, "//ModelVariables/*[@causality = ""parameter""]|//ModelVariables/*[@initial = ""exact""]");
            end
            if xmlParameters.size == 0 then
                ok = %t
                break,
            end

            PARAMS_PER_POPUP = 10; // limit the number of entry per popup
            ranges = 1:PARAMS_PER_POPUP:xmlParameters.size;
            ranges(2, :) = ranges+PARAMS_PER_POPUP-1;
            ranges($) = xmlParameters.size;
            for r=ranges
                N = r(2)-r(1)+1
                outString = "[ok, " + strcat("param" + string(1:N), ", ") + " , next]";

                labels = emptystr(N,1);
                typ = list();
                ini = emptystr(N,1);
                for i=1:N
                    node = xmlParameters(r(1) + i - 1);
                    if length(node.children) > 0 then
                        child = node.children(1);
                    else
                        child = node;
                    end

                    if node.attributes.name <> [] then
                        labels(i) = labels(i) + node.attributes.name + " : ";
                    end
                    if child.attributes.declaredType <> [] then
                        labels(i) = labels(i) + "(" + child.attributes.declaredType + ") ";
                    end
                    if child.attributes.min <> [] then
                        labels(i) = labels(i) + "min=" + child.attributes.min + " ";
                    end
                    if child.attributes.max <> [] then
                        labels(i) = labels(i) + "max=" + child.attributes.max + " ";
                    end
                    if node.attributes.description <> [] then
                        labels(i) = "<HTML><BODY>" + labels(i) + "<BR>" + node.attributes.description + ". "
                    end
                    typ($+1) = "str";
                    typ($+1) = -1;

                    if node.attributes.start <> [] then
                        ini(i) = node.attributes.start;
                    elseif child.attributes.start <> [] then
                        ini(i) = child.attributes.start;
                    else
                        ini(i) = "0";
                    end
                    if child.name == "Boolean" then
                        ini(i) = strsubst(strsubst(ini(i), "true", "1"), "false", "0");
                    end

                    // each exprs entry is a triplet ["valueReference" ; "type" ; "value"] ; match the preset valueReference and type, set ini value
                    refIdx = find(exprs(11:3:$-2) == node.attributes.valueReference & exprs(12:3:$-1) == child.name, 1);
                    if refIdx <> [] then
                        ini(i) = exprs(10 + 3*refIdx);
                    end
                end

                execstr(outString + " = scicos_getvalue(desc, labels, typ, ini);");
                if ~ok then
                    xmlDelete(xml);
                    return;
                end

                // update parameters and the opar encoded values
                for i=1:size(next, '*')
                    node = xmlParameters(r(1) + i - 1);
                    if node.name == "ScalarVariable" && length(node.children) > 0 then
                        child = node.children(1);
                    else
                        child = node;
                    end

                    // each exprs entry is a triplet ["valueReference" ; "type" ; "value"] ; set them
                    refIdx = find(exprs(11:3:$-2) == node.attributes.valueReference & exprs(12:3:$-1) == child.name);
                    if refIdx == [] then
                        exprs($+1) = node.attributes.valueReference;
                        refIdx = size(exprs, 1);
                        exprs($+1) = child.name;
                        exprs($+1) = next(i);
                    else
                        // update the value
                        exprs(10 + 3*refIdx(1)) = next(i);

                        // buggy blocks, clear multiple entries
                        if size(refIdx, "*") > 1 then
                            toBeRemoved = 10 + 3 * refIdx(2:$);
                            exprs([toBeRemoved toBeRemoved+1 toBeRemoved+2]) = [];
                        end
                    end

                    // References and values update
                    ref = uint32(strtod(node.attributes.valueReference));
                    hasNegatedAlias = uint32(node.attributes.alias == "negatedAlias");
                    select child.name
                    case "Boolean" then
                        boolRefs = [boolRefs ref];
                        boolNegatedAlias = [boolNegatedAlias hasNegatedAlias];
                        execstr(msprintf("opar(9)($+1) = evstr(param%d) ;", i));
                    case "Float64" then
                        realRefs = [realRefs ref];
                        realNegatedAlias = [realNegatedAlias hasNegatedAlias];
                        execstr(msprintf("rpar($+1) = strtod(param%d) ;", i));
                    case "Int32" then
                        intRefs = [intRefs ref];
                        intNegatedAlias = [intNegatedAlias hasNegatedAlias];
                        // FIXME: int32 is not supported on ModelAdapter.cpp / struct ipar in Scilab 2024.1 ; use double for now
                        // FIXME:
                        // execstr(msprintf("ipar($+1) = int32(strtod(param%d)) ;", i));
                        execstr(msprintf("ipar($+1) = strtod(param%d) ;", i));
                    case "Integer" then
                        intRefs = [intRefs ref];
                        intNegatedAlias = [intNegatedAlias hasNegatedAlias];
                        // FIXME: int32 is not supported on ModelAdapter.cpp / struct ipar in Scilab 2024.1 ; use double for now
                        // FIXME:
                        // execstr(msprintf("ipar($+1) = int32(strtod(param%d)) ;", i));
                        execstr(msprintf("ipar($+1) = strtod(param%d) ;", i));
                    case "Enumeration" then
                        // FMI 1.0 and 2.0 enumerations are set as Integer
                        intRefs = [intRefs ref];
                        intNegatedAlias = [intNegatedAlias hasNegatedAlias];
                        execstr(msprintf("ipar($+1) = strtod(param%d) ;", i));
                    case "Real" then
                        realRefs = [realRefs ref];
                        realNegatedAlias = [realNegatedAlias hasNegatedAlias];
                        execstr(msprintf("rpar($+1) = strtod(param%d) ;", i));
                    else
                        msg = "unsupported %s type for %s\n";
                        messagebox(msprintf(msg, child.name, node.attributes.name),"modal","error");
                        ok = %f
                        return
                    end
                end
            end
        end

        // concatenate refs and negatedAlias
        opar(6) = uint32([realRefs realNegatedAlias]);
        opar(7) = uint32([intRefs  intNegatedAlias]);
        opar(8) = uint32([boolRefs boolNegatedAlias]);

        // inputs
        if part(fmuImpl, $-2:$) == "3.0" then
            xmlInputs = xmlXPath(xml, "//ModelVariables/*[@causality = ""input""]");
        else
            xmlInputs = xmlXPath(xml, "//ScalarVariable[@causality=""input""]");
        end
        [model.in, model.in2, model.intyp, labelInputs, opar(2), opar(3), errmsg] = configure_input_or_output(xmlInputs, errmsg);

        // outputs
        if part(fmuImpl, $-2:$) == "3.0" then
            xmlOutputs = xmlXPath(xml, "//ModelVariables/*[@causality = ""output""]");
        else
            xmlOutputs = xmlXPath(xml, "//ScalarVariable[@causality=""output""]");
        end
        [model.out, model.out2, model.outtyp, labelOutputs, opar(4), opar(5), errmsg] = configure_input_or_output(xmlOutputs, errmsg);

        // continuous states
        if or(fmuImpl == ["me 1.0" "cs 1.0"]) then
            // initial values are read from the FMU at initialization ;
            // co-simulation states are not integrated but change the solver steps as in FMInterface
            numberOfContinuousStates = xmlXPath(xml, "//@numberOfContinuousStates");
            opar(14) = uint32(strtod(numberOfContinuousStates(1).content));
            state = zeros(double(opar(14)), 1);
        else
            // in FMU 2.0 and 3.0 unknown have a unique derivative index
            if fmuImpl == "me 2.0" then
                derivative = xmlXPath(xml, "//ScalarVariable/Real[@derivative]");
                derivativeIndex = xmlXPath(xml, "//ScalarVariable/Real/@derivative").content;
            elseif fmuImpl == "me 3.0" then
                derivative = xmlXPath(xml, "//ModelVariables/Float64[@derivative]");
                derivativeIndex = xmlXPath(xml, "//ModelVariables/Float64/@derivative").content;
            else
                derivative = list();
                derivativeIndex = [];
            end
            [_, _, ku, _] = unique(strtod(derivativeIndex));

            numberOfContinuousStates = size(ku, "*");
            state = zeros(numberOfContinuousStates, 1);
            opar(14) = uint32(numberOfContinuousStates);

            // fill default values, the last start value win
            for i=1:length(derivative)
                node = derivative(i);

                x0 = strtod(node.attributes.start);
                if x0 <> [] then
                    state(ku(i)) = x0;
                end
            end
        end

        // define the block interface
        model.state = state;
        if state <> [] then
            model.dep_ut = [%f,%t];
        elseif fmuImpl == "cs 1.0" then
            // FMI 1.0 co-simulation outputs are delayed by one communication step
            model.dep_ut = [%f,%t];
        else
            model.dep_ut = [%t,%t];
            if xmlXPath(xml, "//DirectDependency").size == xmlOutputs.size then
                model.dep_ut = [%t,%f];
            end
        end

        model.rpar = rpar;
        model.ipar = ipar;
        model.opar = opar;

        if fmuImpl == "me 1.0" then
            // FMI 1.0 state events are detected by fmu_simulate_me on output update
            model.nzcross = 0;
        elseif part(fmuImpl, 1:2) == "me" then
            numberOfEventIndicators = xmlXPath(xml, "//@numberOfEventIndicators");
            if length(numberOfEventIndicators) > 0 then
                // number of event indicators in v1 or v2
                model.nzcross = strtod(numberOfEventIndicators.content);
            else
                // EventIndicator in the structure in v3
                numberOfEventIndicators = xmlXPath(xml, "/fmiModelDescription/ModelStructure/EventIndicator");
                model.nzcross = length(numberOfEventIndicators);
            end
        else
            model.nzcross = 0;
        end
        if errmsg == [] then
            x.model = model;

            x.graphics.style = ["blockWithLabel;displayedLabel="+modelName];
            x.graphics.pin = zeros(model.in);
            x.graphics.in_label = labelInputs;
            x.graphics.pout = zeros(model.out);
            x.graphics.out_label = labelOutputs;
            x.graphics.exprs = exprs;
        else
            x = [];
            msg = _("%s: unable to load the FMU model\n");
            if getscilabmode() == "STD" then
                messagebox(errmsg, msprintf(msg, "FMU_WRAPPER"), "modal", "error");
            else
                mprintf(strcat(errmsg, "\n"));
                mprintf(msg, "FMU_WRAPPER");
            end
            printf(msg, "FMU_WRAPPER");
        end
        xmlDelete(xml);

    case 'define' then
        exprs = "";
        model=scicos_model();
        model.sim=list("fmu_simulate_me",4);
        model.opar = list();
        model.rpar=[];
        model.ipar = [];
        model.out = [];
        model.in = [];
        model.state = [];
        model.dstate = [];
        model.evtin = [];
        label = "FMU file not defined";
        x = standard_define([9 5],model,exprs,[]);
        x.graphics.style = ["blockWithLabel;displayedLabel="+label];
    end
endfunction

// helper function to resole a FMU filepath
function filepath = resolve_fmu_filepath(fname)
    // if the file does not exist, resolve to :
    //  - PWD/resources for local files
    //  - TMPDIR/resources for SSP support
    //  - fmigetPath() for the fmu module demos and tests
    //  - fmigetPath() for a trailing part of a path saved elsewhere (e.g. from the fmu-wrapper toolbox)

    fname = strsubst(fname, "\", "/"); // portable path from other platforms
    filepath = fname;
    if ~isfile(filepath) then
        filepath = fullfile(PWD, "resources", fname);
    end
    if ~isfile(filepath) then
        filepath = fullfile(TMPDIR, "resources", fname);
    end
    if ~isfile(filepath) then
        filepath = fullfile(fmigetPath(), fname);
    end
    if ~isfile(filepath) then
        parts = tokens(fname, ["/" "\"]);
        for i=2:size(parts, "*")
            filepath = fullfile(fmigetPath(), strcat(parts(i:$), filesep()));
            if isfile(filepath) then
                break
            end
        end
    end
    if ~isfile(filepath) then
        msg = gettext("FMU_WRAPPER error: Unable to load ""%s""");
        messagebox(msprintf(msg, fname), "modal", "error");
        filepath = "";
    end
endfunction

// helper function to store a FMU filepath with portable separators
function filepath = portable_fmu_filepath(fname)
    filepath = strsubst(fname, "\", "/");
    fmuPath = strsubst(fmigetPath(), "\", "/");
    if strindex(filepath, fmuPath) == 1 then
        // relative to fmigetPath(), resolved on reload
        filepath = part(filepath, length(fmuPath)+1:$);
    end
endfunction

// helper function to load an FMU
function [xml, exprs, opar, rpar, ipar, fmuImpl] = load_fmu(filepath, exprs, workdir, fmuImpl)
    xml = [];
    opar = list();
    rpar = [];
    ipar = [];

    if filepath == "" then
        return
    end

    // make the path relative to fmigetPath() or PWD
    fmuroot = fullpath(fmigetPath());
    if strindex(fullpath(filepath), fmuroot) == 1 then
        filepath = part(fullpath(filepath), (length(fmuroot)+1):$);
    elseif strindex(filepath, PWD + filesep()) == 1 then
        filepath = part(filepath, (length(PWD)+2):$);
    end

    [path, file_name, extension] = fileparts(filepath);
    if getscilabmode() == "STD"
        winId = progressionbar('Loading of '+file_name);
    else
        winId = [];
    end
    try
        [workdir, xml, libname, modelIdentifier, fmuImpl] = loadFMU(filepath, [], fmuImpl);

        // sync some properties to make them accessible at simulation time
        exprs(1) = portable_fmu_filepath(filepath);
        exprs(2) = workdir;
        exprs(3) = fmuImpl($); // FMI implementation
        if size(exprs, '*') < 10 then
            exprs(4) = "%f"; // debug logging expr
            exprs(5) = "0"; // relative tolerance expr, not set
            exprs(6) = "%t"; // map integers to double
            exprs(7) = "%t"; // map boolean to double
            exprs(8) = "24"; // string max length
            exprs(9) = "%t"; // map enumeration to double
            exprs(10) = "100"; // event iteration limit
        end
        // following exprs are sett'ed value references, empty by default
        fmiVersion = xmlXPath(xml, "/fmiModelDescription/@fmiVersion")(1).content;

        opar(1) = int8([ascii(fullpath(libname)) 0]);
        opar(2) = []; // inputs references + negated alias
        opar(3) = []; // inputs types
        opar(4) = []; // outputs references + negated alias
        opar(5) = []; // outputs types
        opar(6) = []; // Real references + negated alias
        opar(7) = []; // Integer references + negated alias
        opar(8) = []; // Boolean references + negated aliasex
        opar(9) = []; // Boolean values
        select fmiVersion
        case "3.0" then
            opar(10) = int8([ascii(fullfile(fullpath(workdir), "resources") + filesep()) 0]); // absolute file path (with a trailing file separator) of the resources directory of the extracted FMU archive.
        case "2.0" then
            opar(10) = int8([ascii("file:///" + strsubst(fullfile(fullpath(workdir), "resources"), filesep(), "/")) 0]); // URL path to the resource dir
        case "1.0" then
            opar(10) = int8([ascii(fullpath(libname)) 0]); // FMI 1.0 simulation functions load the library from opar(10)
        end
        opar(11) = int8([ascii(xmlXPath(xml, "//@modelName")(1).content) 0]);  // model name
        if fmiVersion == "3.0" then
            opar(12) = int8([ascii(xmlXPath(xml, "//@instantiationToken")(1).content) 0]); // model instantiationToken
        else
            opar(12) = int8([ascii(xmlXPath(xml, "//@guid")(1).content) 0]); // model guid
        end
        opar(13) = int8([0 0]); // [debug logging, hasEventMode]
        opar(14) = [] // number of states. Computed in FMI 2.0 or uint32(xml.root.attributes.numberOfContinuousStates);
        numberOfEventIndicators = xmlXPath(xml, "//@numberOfEventIndicators");
        if length(numberOfEventIndicators) > 0 then
            // number of event indicators in v1 or v2
            opar(15) = uint32(strtod(numberOfEventIndicators(1).content));
        else
            // EventIndicator in the structure in v3
            opar(15) = uint32(length(xmlXPath(xml, "/fmiModelDescription/ModelStructure/EventIndicator")));
        end
        opar(16) = 0.; // relative tolerance not set
        opar(17) = int32(100); // event iteration limit
    catch
        [str,n]=lasterror();
        msg = ["FMU model hasn''t been loaded." ;
                msprintf("Error %d : ""%s""", n, str)];
        if getscilabmode() == "STD"
            close(winId);
            winId = [];
            messagebox(msg,"modal","error");
        else
            disp(msg)
        end
    end
    if getscilabmode() == "STD" && winId <> [] then
        close(winId);
    end
endfunction

// helper function to encode FMI variables on block inputs and outputs
function [d, d2, dtyp, labels, refsAndModifiers, types, errmsg] = configure_input_or_output(xmlVariables, errmsg)
    d = ones(xmlVariables.size, 1);
    d2 = ones(xmlVariables.size, 1);
    dtyp = ones(xmlVariables.size, 1);
    labels = emptystr(xmlVariables.size, 1);
    refs = uint32(-ones(xmlVariables.size, 1));
    modifiers = uint32(zeros(xmlVariables.size, 1));
    refsAndModifiers = [refs ; modifiers];
    types = ones(xmlVariables.size, 1);

    for i=1:xmlVariables.size
        node = xmlVariables(i);
        if length(node.children) > 0 then
            child = node.children(1);
        else
            child = node;
        end
        labels(i) = node.attributes.name;
        refs(i) = strtod(node.attributes.valueReference);

        if node.name == "ScalarVariable" && length(node.children) > 0 then
            datatype = child.name;
        else
            datatype = node.name;
        end

        // FMI 3.0 type definitions
        // values are [internal type, scicos_model type, n_rows, n_columns]
        typeDefinition = struct();
        typeDefinition.Float64 = [108 1 1 1];
        typeDefinition.Float32 = [104 1 1 1];
        typeDefinition.Int64 = [808 1 1 1];
        typeDefinition.Int32 = [804 integerToDouble(1,1) 1 1];
        typeDefinition.Int16 = [802 integerToDouble(2,1) 1 1];
        typeDefinition.Int8 =  [801 integerToDouble(3,1) 1 1];
        typeDefinition.UInt64 = [818 1 1 1];
        typeDefinition.UInt32 = [814 integerToDouble(1,2) 1 1];
        typeDefinition.UInt16 = [812 integerToDouble(2,2) 1 1];
        typeDefinition.UInt8 =  [811 integerToDouble(3,2) 1 1];
        typeDefinition.Boolean = [4 booleanToDouble 1 1];
        typeDefinition.String = [10 5 1 stringLength];
        typeDefinition.Binary = [0 5 1 1];
        typeDefinition.Enumeration = [800 enumToDouble 1 1];
        // FMI 2.0 type definitions
        typeDefinition.Real = [108 1 1 1];
        typeDefinition.Integer = [804 integerToDouble(1,1) 1 1];
        // FMI 1.0 type definitions are already handled

        idx = find(datatype == fieldnames(typeDefinition), 1);
        if idx == [] then
            ok = %f;
            msg = _("unsupported %s type for input #%d\n");
            errmsg($+1) = sprintf(msg, datatype, i);
            continue;
        end

        def = typeDefinition(datatype);
        types(i) = def(1);
        dtyp(i) = def(2);
        d(i) = def(3);
        d2(i) = def(4);

        select node.attributes("alias")
        case "alias" then
            // valueReference are the same, do not care
            modifiers(i) = 0;
        case "negatedAlias" then
            modifiers(i) = 1;
        end
        select node.attributes("variability")
        case "discrete" then
            modifiers(i) = modifiers(i) + 2;
        end
        // some values have multi dimensions, handle them
        if dtyp(i) == 10 && datatype == "String" then
            // String maximum length, up to the configured length
            if child.attributes.maxLength <> [] then
                d2(i) = max(strtod(child.attributes.maxLength), stringLength);
            else
                d2(i) = stringLength;
            end
        end
        if dtyp(i) == 5 && datatype == "Binary" then
            // Binary maximum size
            if child.attributes.maxSize <> [] then
                d2(i) = strtod(child.attributes.maxSize);
            else
                d2(i) = stringLength;
            end
        end

        // printf("Port #%-2d ref %4d %-11s types: %3d   dtyp: %3d   d: %3d   d2: %3d\n", i, refs(i), datatype, types(i), dtyp(i), d(i), d2(i));
    end
    refsAndModifiers = [refs ; modifiers];
    types = int32(types);
endfunction
