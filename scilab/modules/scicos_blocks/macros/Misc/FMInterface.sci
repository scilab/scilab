//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2017-2020 - ESI Group - Clement David
// Copyright (C) - 2013 - Scilab Enterprises - Vladislav Trubkin
// Copyright (C) - 2013-2017 - Scilab Enterprises - Clement David
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
// helper functions for getters/setters
// negated alias
// setters
function refsNew = negAliasReal(refs)
    sizeRefs = size(refs,"*");
    refsNew = zeros(sizeRefs,1);
    negAliasValues = fmu.modelVariables.Real.valueReference(find(fmu.modelVariables.Real.alias == "negatedAlias" &...
    ~isnan(fmu.modelVariables.Real.start)));
    valuePosition = zeros(size(negAliasValues,"*"));
    for i=1:size(negAliasValues,"*")
        valuePosition(i) = find(refs == negAliasValues(i));
        refsNew(valuePosition(i)) = 1;
    end
endfunction

function refsNew = negAliasInt(refs)
    sizeRefs = size(refs,"*");
    refsNew = zeros(sizeRefs,1);
    var = [fmu.modelVariables.Integer.valueReference,...
    fmu.modelVariables.Enumeration.valueReference];
    var = var(find([fmu.modelVariables.Integer.alias,...
    fmu.modelVariables.Enumeration.alias] == "negatedAlias"));
    negAliasValues = var(find(~isnan([fmu.modelVariables.Integer.start,...
    fmu.modelVariables.Enumeration.start])));
    valuePosition = zeros(size(negAliasValues,"*"));
    for i=1:size(negAliasValues,"*")
        valuePosition(i) = find(refs == negAliasValues(i));
        refsNew(valuePosition(i)) = 1;
    end
endfunction

function refsNew = negAliasBool(refs)
    sizeRefs = size(refs,"*");
    refsNew = zeros(sizeRefs,1);
    negAliasValues = fmu.modelVariables.Boolean.valueReference(find(fmu.modelVariables.Boolean.alias == "negatedAlias" &...
    fmu.modelVariables.Boolean.variability == "discrete"));
    valuePosition = zeros(size(negAliasValues,"*"));
    for i=1:size(negAliasValues,"*")
        valuePosition(i) = find(refs == negAliasValues(i));
        refsNew(valuePosition(i)) = 1;
    end
endfunction


// Interface function

function [x, y, typ] = FMInterface(job,arg1,arg2)
    x = []; y = []; typ = [];
    select job
    case 'set' then
        x = arg1;
        model=arg1.model;
        opar = model.opar;
        graphics=arg1.graphics;
        exprs=graphics.exprs;
        value = list(graphics.exprs.setReal,...
                    graphics.exprs.setInteger,...
                    graphics.exprs.setBoolean,...
                    graphics.exprs.getValue,...
                    graphics.exprs.debugLogging,...
                    graphics.exprs.relativeTolerance);

        // helper function to load an FMU
        function [fmu, opar] = load_fmu(fname, opar)
            fmu = [];
            if typeof(opar) <> "list" then
                opar = list();
            end

            if ~isfile(fname) then
                fmu = [];
                return
            end

            [path, file_name, extension] = fileparts(fname);
            if getscilabmode() == "STD" then
                winId = progressionbar('Loading of '+file_name);
            else
                winId = [];
            end
            try
                fmu = importFMU(fname);//Loading of fmu

                fmu_ulink(fmu.simulationLibrary);
                fmu.simulationLibrary = ""; // pointer is not supported in opar

                // sync some properties to be accessible at simulation time
                opar(10) = int8([ascii(fmu.libraryFile)';0]);
                opar(11) = int8([ascii(fmu.descriptionFMU.modelIdentifier)';0]);
                opar(12) = int8([ascii(fmu.descriptionFMU.guid)';0]);
                opar(14) = uint32(fmu.descriptionFMU.numberOfContinuousStates);
                opar(15) = uint32(fmu.descriptionFMU.numberOfEventIndicators);
            catch
                [msg,n]=lasterror();
                messagebox(["FMU model hasn''t been loaded." + ...
                "Error: " + string(n) + " """ + msg + """"], "modal", "error");
                error(msg);
            end
            if getscilabmode() == "STD" && winId <> [] then
                close(winId);
            end
        endfunction

        if isempty(opar) then
            // a new load has to be done
            Load = uigetfile(["*.fmu"], "", "Load FMU for Simulation");
        elseif typeof(opar(1)) <> "FMU" then
            // invalid opar type, for a reload
            Load = uigetfile(["*.fmu"], "", "Load FMU for Simulation");
        else
            // the file name is stored in a field
            Load = opar(1).relativePath;
        
            if ~isfile(Load) then
                // try to relocate tests/unit_tests and tests/nonreg_tests files
                sp = strsplit(Load, "/(\\|\/)tests(\\|\/)/");
                if size(sp, 'r') > 2 then
                    Load = fullfile(fmigetPath(), "tests", sp($));
                end
            end
            if ~isfile(Load) then
                // try to locate an existing file on tests/unit_tests
                // this is greedy but will work on all existing tests
                [?, fname, ext] = fileparts(Load);
                Load = fullfile(fmigetPath(), "tests", "unit_tests", fname+ext);
            end

            if ~isfile(Load) then    
                if isdir(fileparts(Load)) then
                    // reload the fmu from the same existing directory
                    Load = uigetfile(["*.fmu"], fileparts(Load),"Load FMU for Simulation");    
                else
                    Load = uigetfile(["*.fmu"], "", "Load FMU for Simulation");
                end
            end
        end

        [fmu, opar] = load_fmu(Load, opar);

        if typeof(fmu) <> "FMU" then
            x.model = model;
            x.graphics = graphics;
            return;
        end

        fmiType = fmu.fmiType;
        fmiVersion = fmu.descriptionFMU.fmiVersion; 

        // set simulation function
        select [fmiType fmiVersion]
        case ["me" "1.0"] then
            model.sim=list("fmu_simulate_me",4);
        case ["cs" "1.0"] then
            model.sim=list("fmu_simulate_cs",4);
        else
            messagebox(["Invalid FMU version / mode."],"modal","error");
            x = [];
            close(winId);
            return
        end

        // inputs
        inputs = ones(size(fmu.setValues.inputs,"c"),1);
        graphics.exprs.in = fmu.setValues.inputs;
        // get user inputs
        inputTypes = ones(1:size(graphics.exprs.in,"*"));

        refsInputs = [];
        if ~isempty(graphics.exprs.in) then
            refsInputs = inputTypes;
            for i=1:size(graphics.exprs.in,"*")
                if find(fmu.modelVariables.Real.name == graphics.exprs.in(i)) then
                    refsInputs(i) = fmu.modelVariables.Real.valueReference(find(fmu.modelVariables.Real.name == graphics.exprs.in(i)));
                    inputTypes(i) = 1; //'real'
                elseif find(fmu.modelVariables.Integer.name == graphics.exprs.in(i)) then
                    refsInputs(i) = fmu.modelVariables.Integer.valueReference(find(fmu.modelVariables.Integer.name == graphics.exprs.in(i)));
                    inputTypes(i) = 2; //'integer'
                    model.intyp(i) = 3;//int32
                elseif find(fmu.modelVariables.Boolean.name == graphics.exprs.in(i)) then
                    refsInputs(i) = fmu.modelVariables.Boolean.valueReference(find(fmu.modelVariables.Boolean.name == graphics.exprs.in(i)));
                    inputTypes(i) = 3; //'boolean'
                elseif find(fmu.modelVariables.String.name == graphics.exprs.in(i)) then
                    refsInputs(i) = fmu.modelVariables.String.valueReference(find(fmu.modelVariables.String.name == graphics.exprs.in(i)));
                    inputTypes(i) = 4; //'string'
                end
            end
        end
        graphics.exprs.inRefs = refsInputs;
        graphics.exprs.inTypes = inputTypes;
        opar(2) = refsInputs;
        opar(3) = int32([108 804 4 10](inputTypes'));

        // inputs negatedAlias
        negatedAlias = [];
        for i=1:size(opar(2),"*")
            if inputTypes(i) == 1 then
                negatedAlias(i) = negAliasReal(opar(2)(i));
            elseif inputTypes(i) == 2
                negatedAlias(i) = negAliasInt(opar(2)(i));
            elseif inputTypes(i) == 3
                negatedAlias(i) = negAliasBool(opar(2)(i));
            end
        end
        opar(2) = uint32([matrix(opar(2),-1,1);matrix(negatedAlias,-1,1)]);
        graphics.in_label = graphics.exprs.in;

        // outputs
        names = [fmu.modelVariables.Real.name, fmu.modelVariables.Integer.name,...
        fmu.modelVariables.Boolean.name, fmu.modelVariables.String.name];
        outputs = names(find([fmu.modelVariables.Real.causality,...
        fmu.modelVariables.Integer.causality,...
        fmu.modelVariables.Enumeration.causality,...
        fmu.modelVariables.Boolean.causality,...
        fmu.modelVariables.String.causality] == "output"));
        graphics.exprs.out = outputs;
        outputs = ones(size(outputs,"c"),1);
        model.out = outputs;
        model.out2 = outputs;
        model.outtyp = outputs;

        // get user outputs
        outputTypes = ones(1:size(graphics.exprs.out,"*"));
        refsOutputs = []
        if ~isempty(graphics.exprs.out) then
            refsOutputs = outputTypes;
            for i=1:size(graphics.exprs.out,"*")
                if find(fmu.modelVariables.Real.name == graphics.exprs.out(i)) then
                    refsOutputs(i) = fmu.modelVariables.Real.valueReference(find(fmu.modelVariables.Real.name == graphics.exprs.out(i)));
                    outputTypes(i) = 1; //'real'
                elseif find(fmu.modelVariables.Integer.name == graphics.exprs.out(i)) then
                    refsOutputs(i) = fmu.modelVariables.Integer.valueReference(find(fmu.modelVariables.Integer.name == graphics.exprs.out(i)));
                    outputTypes(i) = 2; //'integer'
                    model.outtyp(i) = 3;//int32
                elseif find(fmu.modelVariables.Boolean.name == graphics.exprs.out(i)) then
                    refsOutputs(i) = fmu.modelVariables.Boolean.valueReference(find(fmu.modelVariables.Boolean.name == graphics.exprs.out(i)));
                    outputTypes(i) = 3; //'boolean'
                end
            end
        end

        graphics.exprs.outRefs = refsOutputs;
        graphics.exprs.outTypes = outputTypes;
        opar(4) = refsOutputs;
        opar(5) = int32([108 804 4 10](outputTypes'));

        // outputs negatedAlias
        negatedAlias = [];
        for i=1:size(opar(4),"*")
            if outputTypes(i) == 1 then
                negatedAlias(i) = negAliasReal(opar(4)(i));
            elseif outputTypes(i) == 2
                negatedAlias(i) = negAliasInt(opar(4)(i));
            elseif outputTypes(i) == 3
                negatedAlias(i) = negAliasBool(opar(4)(i));
            end
        end
        opar(4) = uint32([matrix(opar(4),-1,1);matrix(negatedAlias,-1,1)]);
        graphics.out_label = outputs';

        // set real, integer, boolean
        opar(6) = [];
        opar(7) = [];
        opar(8) = [];
        opar(9) = [];
        opar(10) = int8([ascii(fmu.libraryFile)';0]);
        opar(11) = int8([ascii(fmu.descriptionFMU.modelIdentifier)';0]);
        opar(12) = int8([ascii(fmu.descriptionFMU.guid)';0]);
        opar(13) = int8([0]); // debuglogging
        // number of states and events
        opar(14) = uint32(fmu.descriptionFMU.numberOfContinuousStates);
        opar(15) = uint32(fmu.descriptionFMU.numberOfEventIndicators);
        opar(16) = [0];
        if fmu.descriptionFMU.numberOfContinuousStates <> 0 then
            model.state = zeros(1,fmu.descriptionFMU.numberOfContinuousStates)';
            model.dep_ut = [%f,%t];
        else
            //model.dstate = [0];
            model.dep_ut = [%f,%f];
            model.evtin = 1; // activation port
            if ~isempty(fmu.modelVariables.Real.DirectDependency) then
                model.dep_ut = [%t,%f];
                model.state = [0];
            end
        end

        //Block with the name of FMU
        graphics.style = ["blockWithLabel;displayedLabel="""+fmu.modelFileName+""""];

        ok = %f;
        while ~ok
            [ok, TSReal, TSInt, TSBool, TGValue, TDebug, TToler, value]=...
            scicos_getvalue("Configuration of "+fmu.modelFileName,...
            ['Set Real:',...
            'Set Integer:',...
            'Set Boolean:',...
            'Get Values:',...
            'Debug Logging ?',...
            'Relative Tolerance ?'],...
            list('str',-1,'str',-1,'str',-1,'str',-1,...
            'str',-1,'str',-1),value);

            if ~ok then
                break;
            end
            // fmi type
            fmiType = fmu.fmiType;
            fmiVersion = fmu.descriptionFMU.fmiVersion;
            // labels of inputs/outputs
            graphics.in_label = graphics.exprs.in;
            graphics.out_label = graphics.exprs.out';
            // number of states and events
            opar(14) = uint32(fmu.descriptionFMU.numberOfContinuousStates);
            opar(15) = uint32(fmu.descriptionFMU.numberOfEventIndicators);
            //check errors
            NamesValues = ["TSReal", "TSInt", "TSBool", "TGValue",...
            """TDebug""", """TToler"""];
            for i=1:size(NamesValues, "*")
                err(i) = execstr("nameVP=stripblanks(evstr("+NamesValues(i)+"))", "errcatch");
            end
            if err(1) == 999 & stripblanks(TSReal) <> "all" then
                messagebox(["Answer given for ""Set Real"" is incorrect:"...
                "String type expected."],"modal","error");
                ok = %f;
            elseif err(1) == 4 & stripblanks(TSReal) <> "all" then
                messagebox(["Answer given for ""Set Real"" is incorrect:"...
                "Undefined variables."],"modal","error");
                ok = %f;
            elseif err(1) <> 0 & stripblanks(TSReal) <> "all" then
                messagebox(["Answer given for ""Set Real"" is incorrect:"...
                "Error in values evolution."],"modal","error");
                ok = %f;
            elseif err(2) == 999 & stripblanks(TSInt) <> "all" then
                messagebox(["Answer given for ""Set Integer"" is incorrect:"...
                "String type expected."],"modal","error");
                ok = %f;
            elseif err(2) == 4 & stripblanks(TSInt) <> "all" then
                messagebox(["Answer given for ""Set Integer"" is incorrect:"...
                "Undefined variables."],"modal","error");
                ok = %f;
            elseif err(2) <> 0 & stripblanks(TSInt) <> "all" then
                messagebox(["Answer given for ""Set Integer"" is incorrect:"...
                "Error in values evolution."],"modal","error");
                ok = %f;
            elseif err(3) == 999 & stripblanks(TSBool) <> "all" then
                messagebox(["Answer given for ""Set Boolean"" is incorrect:"...
                "String type expected."],"modal","error");
                ok = %f;
            elseif err(3) == 4 & stripblanks(TSBool) <> "all" then
                messagebox(["Answer given for ""Set Boolean"" is incorrect:"...
                "Undefined variables."],"modal","error");
                ok = %f;
            elseif err(3) <> 0 & stripblanks(TSBool) <> "all" then
                messagebox(["Answer given for ""Set Boolean"" is incorrect:"...
                "Error in values evolution."],"modal","error");
                ok = %f;
            elseif err(4) == 999 then
                messagebox(["Answer given for ""Get Values"" is incorrect:"...
                "String type expected."],"modal","error");
                ok = %f;
            elseif err(4) == 4 then
                messagebox(["Answer given for ""Get Values"" is incorrect:"...
                "Undefined variables."],"modal","error");
                ok = %f;
            elseif err(4) <> 0 then
                messagebox(["Answer given for ""Get Values"" is incorrect:"...
                "Error in values evolution."],"modal","error");
                ok = %f;
            elseif err(5) <> 0 | and(TDebug<>(["%t","%f"])) then
                messagebox(["Answer given for ""Debug Logging"" is incorrect:"...
                "%t or %f expected."],"modal","error");
                ok = %f;
            elseif err(6) <> 0 | and(TToler<>(["%t","%f"])) then
                messagebox(["Answer given for ""Relative Tolerance"" is incorrect:"...
                "%t or %f expected."],"modal","error");
                ok = %f;
            end

            if ok then
                // debug logging
                graphics.exprs.debugLogging = TDebug;
                if evstr(TDebug) == %t then
                    opar(13) = int8(1);
                else
                    opar(13) = int8(0);
                end
                // realtive tolerance
                graphics.exprs.relativeTolerance = TToler;
                if evstr(TToler) == %t then
                    opar(16) = 1;
                else
                    opar(16) = 0;
                end
            end
            
            //*****************************Set Real****************************//
            if ok then
                TSRealTmp = TSReal;
                tmpGraphicsSetReal = evstr(graphics.exprs.setReal);
                if fmiType == "me" then
                    realNames = fmu.modelVariables.Real.name(find(~isnan(fmu.modelVariables.Real.start) &...
                    fmu.modelVariables.Real.causality <> "input" & fmu.modelVariables.Real.causality <> "output"));
                else
                    realNames = fmu.modelVariables.Real.name(find(~isnan(fmu.modelVariables.Real.start) &...
                    fmu.modelVariables.Real.variability == "parameter"));
                end
                if TSReal == "all" then
                    TSReal = realNames;
                    if size(TSReal, "*") == 0 then
                        messagebox([sci2exp(fmu.modelFileName)+" has not real parameters for setting values."], "Set Real");
                    elseif size(TSReal, "*") < 29 then
                        TSReal = TSReal(1:$);
                    else
                        TSReal = TSReal(1:28);
                    end
                    SReals = stripblanks(TSReal);
                    graphics.exprs.setReal = sci2exp(TSReal);
                else
                    SReals = stripblanks(evstr(TSReal));
                    graphics.exprs.setReal = TSReal;
                end
                SReals = matrix(SReals, size(SReals, "*"), 1);
                tmpModelRpar = model.rpar;
                // number of names
                if size(SReals, "*") > 29 then
                    messagebox(["Answer given for ""Set Real"" is incorrect:"...
                    "Is too many names "+string(size(SReals, "*"))+": Must be less than 30."],"modal","error");
                    ok = %f;
                end
                
                // valid names for fmiSetReal
                if ok & ~isempty(SReals) then
                    for i=1:size(SReals,"*")
                        if find(realNames==SReals(i)) == [] then
                            messagebox(["Answer given for ""Set Real"" is incorrect:"...
                            "Wrong name "+""""+SReals(i)+""" for setting real values."],"modal","error");
                            ok = %f;
                            break;
                        elseif size(find(SReals==SReals(i)), "*") > 1 then
                            messagebox(["Answer given for ""Set Real"" is incorrect:"...
                            """"+SReals(i)+""" already exist for setting real values."],"modal","error");
                            ok = %f;
                            break;
                        end
                    end
                end
                if ok & ~isempty(SReals) then
                    value2 = [];// values
                    value_2 = []; // names
                    refs = []; // references
                    if ~isempty(model.rpar) & TSRealTmp <> "all" then
                        for i=1:size(SReals,"*")
                            value_2(i) = find(fmu.modelVariables.Real.name == SReals(i));
                            if size(SReals,"*") == size(model.rpar,"*") & tmpGraphicsSetReal(i) == SReals(i) then
                                value2(i) = string(model.rpar(i));
                                refs(i) = fmu.modelVariables.Real.valueReference(value_2(i));
                            else
                                if find(tmpGraphicsSetReal==SReals(i)) == [] then
                                    value2(i) = string(fmu.modelVariables.Real.start(value_2(i)));// default value for a new real
                                    refs(i) = fmu.modelVariables.Real.valueReference(value_2(i));
                                else
                                    value2(i) = string(model.rpar(find(tmpGraphicsSetReal==SReals(i))));// old values for existing reals
                                    refs(i) = fmu.modelVariables.Real.valueReference(value_2(i));
                                end
                            end
                        end
                    else
                        for i=1:size(SReals,"*") //start values for all params
                            value_2(i) = find(fmu.modelVariables.Real.name==SReals(i));
                            value2(i) = string(fmu.modelVariables.Real.start(value_2(i)));
                            refs(i) = fmu.modelVariables.Real.valueReference(value_2(i));
                        end
                    end
                    
                    // creation the second dialog box to Set Real parameters.
                    lhs_txt="" //lhs txt
                    lab_txt="" //label txt
                    rhs_txt="" //rhs txt
                    for i=1:size(SReals,"*")
                        f = find(fmu.modelVariables.Real.name==SReals(i));
                        lhs_txt=lhs_txt+'%v'+string(i)+',';
                        // name of parameter with displayUnit
                        lab_txt=lab_txt+''''+SReals(i)+" "+"("+fmu.modelVariables.Real.unit(f)+") "+"(min:"+string(fmu.modelVariables.Real.min(f))+", max:"+string(fmu.modelVariables.Real.max(f))+")"+''';';
                        rhs_txt=rhs_txt+'''vec'',-1,';
                    end
                    lhs_txt=part(lhs_txt,1:length(lhs_txt)-1);
                    lab_txt=part(lab_txt,1:length(lab_txt)-1);
                    rhs_txt=part(rhs_txt,1:length(rhs_txt)-1);
                    // getvalue
                    getvalue_txt = '[ok,'+lhs_txt+',value2]=scicos_getvalue(''Set Real'',['+..
                    lab_txt+'],'+..
                    'list('+rhs_txt+'),value2)';

                    // second dialog box
                    execstr(getvalue_txt);
                    value2 = strtod(value2);
                    value2(find(isnan(value2))) = 0;
                    model.rpar = value2;// insertion in model
                    opar(6) = refs;
                    opar(6) = uint32([matrix(refs,-1,1);matrix(negAliasReal(opar(6)),-1,1)]);
                elseif ok & isempty(SReals)
                    model.rpar = [];
                    opar(6) = [];
                    graphics.exprs.setReal = sci2exp("");
                end
                // if "cancel" return the previous values
                if ~ok then
                    graphics.exprs.setReal = exprs.setReal;
                    model.rpar = tmpModelRpar;
                end
            end

            //***************************Set Integer***************************//
            if ok then
                TSIntTmp = TSInt;
                tmpGraphicsSetInt = evstr(graphics.exprs.setInteger);
                varInd = find(~isnan([fmu.modelVariables.Integer.start, fmu.modelVariables.Enumeration.start]));
                var = [fmu.modelVariables.Integer.name, fmu.modelVariables.Enumeration.name];
                var = var(varInd);
                varRef = [fmu.modelVariables.Integer.valueReference, fmu.modelVariables.Enumeration.valueReference];
                varRef = varRef(varInd);
                varCas = [fmu.modelVariables.Integer.causality, fmu.modelVariables.Enumeration.causality];
                varCas = varCas(varInd);
                varVariab = [fmu.modelVariables.Integer.variability, fmu.modelVariables.Enumeration.variability];
                varVariab = varVariab(varInd);
                varSt = [fmu.modelVariables.Integer.start, fmu.modelVariables.Enumeration.start];
                varSt = varSt(varInd);
                if fmiType == "me" then
                    intNames = var(find(varCas <> "input" & varCas <> "output"));
                else
                    intNames = var(find(varVariab == "parameter"));
                end
                if TSInt == "all" then
                    TSInt = intNames;
                    if size(TSInt, "*") == 0 then
                        messagebox([sci2exp(fmu.modelFileName)+" has not integer parameters for setting values."], "Set Integer");
                    elseif size(TSInt, "*") < 29 then
                        TSInt = TSInt(1:$);
                    else
                        TSInt = TSInt(1:28);
                    end
                    SInts = stripblanks(TSInt);
                    graphics.exprs.setInteger = sci2exp(TSInt);
                else
                    SInts = stripblanks(evstr(TSInt));
                    graphics.exprs.setInteger = TSInt;
                end
                SInts = matrix(SInts, size(SInts, "*"), 1);
                tmpModelIpar = model.ipar;
                if size(SInts, "*") > 29 then
                    messagebox(["Answer given for ""Set Integer"" is incorrect:"...
                    "Is too many names "+string(size(SInts, "*"))+": Must be less than 30."],"modal","error");
                    ok = %f;
                end
                if ok & ~isempty(SInts) then
                    for i=1:size(SInts,"*")
                        if find(intNames==SInts(i)) == [] then
                            messagebox(["Answer given for ""Set Integer"" is incorrect:"...
                            "Wrong name "+""""+SInts(i)+""" for setting integer values."],"modal","error");
                            ok = %f;
                            break;
                        elseif size(find(SInts==SInts(i)), "*") > 1 then
                            messagebox(["Answer given for ""Set Integer"" is incorrect:"...
                            """"+SInts(i)+""" already exist for setting integer values."],"modal","error");
                            ok = %f;

                            break;
                        end
                    end
                end
                if ok & ~isempty(SInts) then
                    value2 = [];
                    value_2 = [];
                    refs = []; // references
                    if ~isempty(model.ipar) & TSIntTmp <> "all" then
                        for i=1:size(SInts,"*")
                            value_2(i) = find(var == SInts(i));
                            if size(SInts,"*") == size(model.ipar,"*") & tmpGraphicsSetInt(i) == SInts(i) then
                                value2(i) = string(model.ipar(i));
                                refs(i) = varRef(value_2(i));
                            else
                                if find(tmpGraphicsSetInt==SInts(i)) == [] then
                                    value2(i) = string(varSt(value_2(i)));
                                    refs(i) = varRef(value_2(i));
                                else
                                    value2(i) = string(model.ipar(find(tmpGraphicsSetInt==SInts(i))));
                                    refs(i) = varRef(value_2(i));
                                end
                            end
                        end
                    else
                        for i=1:size(SInts,"*")
                            value_2(i) = find(var==SInts(i));
                            value2(i) = string(varSt(value_2(i)));
                            refs(i) = varRef(value_2(i));
                        end
                    end
                    lhs_txt=""
                    lab_txt=""
                    rhs_txt=""
                    varMin = [fmu.modelVariables.Integer.min, fmu.modelVariables.Enumeration.min];
                    varMax = [fmu.modelVariables.Integer.max, fmu.modelVariables.Enumeration.max];
                    for i=1:size(SInts,"*")
                        f = find(var==SInts(i));
                        lhs_txt=lhs_txt+'%v'+string(i)+',';
                        lab_txt=lab_txt+''''+SInts(i)+" "+"(min:"+string(varMin(f))+", max:"+string(varMax(f))+")"+''';';
                        rhs_txt=rhs_txt+'''vec'',-1,';
                    end
                    lhs_txt=part(lhs_txt,1:length(lhs_txt)-1);
                    lab_txt=part(lab_txt,1:length(lab_txt)-1);
                    rhs_txt=part(rhs_txt,1:length(rhs_txt)-1);
                    getvalue_txt = '[ok,'+lhs_txt+',value2]=scicos_getvalue(''Set Integer'',['+..
                    lab_txt+'],'+..
                    'list('+rhs_txt+'),value2)';
                    execstr(getvalue_txt);
                    value2 = strtod(value2);
                    value2(find(isnan(value2))) = 0;
                    model.ipar = value2;
                    opar(7) = refs;
                    opar(7) = uint32([matrix(refs,-1,1);matrix(negAliasInt(opar(7)),-1,1)]);
                elseif ok & isempty(SInts)
                    model.ipar = [];
                    opar(7) = [];
                    graphics.exprs.setInteger = sci2exp("");
                end
                if ~ok then
                    graphics.exprs.setInteger = exprs.setInteger;
                    model.ipar = tmpModelIpar;
                end
            end

            //***************************Set Boolean***************************//
            if ok then
                TSBoolTmp = TSBool;
                tmpGraphicsSetBool = evstr(graphics.exprs.setBoolean);
                if fmiType == "me" then
                    boolNames = fmu.modelVariables.Boolean.name(find((fmu.modelVariables.Boolean.variability == "parameter" |...
                    fmu.modelVariables.Boolean.variability == "discrete") & fmu.modelVariables.Boolean.alias == "noAlias" &...
                    fmu.modelVariables.Boolean.causality <> "input" & fmu.modelVariables.Boolean.causality <> "output"));
                else
                    boolNames = fmu.modelVariables.Boolean.name(find((fmu.modelVariables.Boolean.variability == "parameter") &...
                    fmu.modelVariables.Boolean.alias == "noAlias" &...
                    fmu.modelVariables.Boolean.causality <> "input" & fmu.modelVariables.Boolean.causality <> "output"));
                end
                if TSBool == "all" then
                    TSBool = boolNames;
                    if size(TSBool, "*") == 0 then
                        messagebox([sci2exp(fmu.modelFileName)+" has not boolean parameters for setting values."], "Set Boolean");
                    elseif size(TSBool, "*") < 29 then
                        TSBool = TSBool(1:$);
                    else
                        TSBool = TSBool(1:28);
                    end
                    SBools = stripblanks(TSBool);
                    graphics.exprs.setBoolean = sci2exp(TSBool);
                else
                    SBools = stripblanks(evstr(TSBool));
                    graphics.exprs.setBoolean = TSBool;
                end
                SBools = matrix(SBools, size(SBools, "*"), 1);
                tmpModelOparTwo = opar(9);
                if size(SBools, "*") > 29 then
                    messagebox(["Answer given for ""Set Boolean"" is incorrect:"...
                    "Is too many names "+string(size(SBools, "*"))+": Must be less than 30."],"modal","error");
                    ok = %f;
                end
                if ok & ~isempty(SBools) then
                    for i=1:size(SBools,"*")
                        if find(boolNames==SBools(i)) == [] then
                            messagebox(["Answer given for ""Set Boolean"" is incorrect:"...
                            "Wrong name "+""""+SBools(i)+""" for setting boolean values."],"modal","error");
                            ok = %f;
                            break;
                        elseif size(find(SBools==SBools(i)), "*") > 1 then
                            messagebox(["Answer given for ""Set Boolean"" is incorrect:"...
                            """"+SBools(i)+""" already exist for setting boolean values."],"modal","error");
                            ok = %f;
                            break;
                        end
                    end
                end
                if ok & ~isempty(SBools) then
                    value2 = [];
                    value_2 = [];
                    refs = [];
                    if ~isempty(opar(9)) & TSBoolTmp <> "all" then

                        for i=1:size(SBools,"*")
                            value_2(i) = find(fmu.modelVariables.Boolean.name == SBools(i));
                            if size(SBools,"*") == size(opar(9),"*") & tmpGraphicsSetBool(i) == SBools(i) then
                                value2(i) = string(double(opar(9)(i)));
                                refs(i) = fmu.modelVariables.Boolean.valueReference(value_2(i));
                            else
                                if find(tmpGraphicsSetBool==SBools(i)) == [] then
                                    value2(i) = string(double(fmu.modelVariables.Boolean.start(value_2(i))));
                                    refs(i) = fmu.modelVariables.Boolean.valueReference(value_2(i));
                                else
                                    value2(i) = string(double(opar(9)(find(tmpGraphicsSetBool==SBools(i)))));
                                    refs(i) = fmu.modelVariables.Boolean.valueReference(value_2(i));
                                end
                            end
                        end
                    else
                        for i=1:size(SBools,"*")
                            value_2(i) = find(fmu.modelVariables.Boolean.name==SBools(i));
                            value2(i) = string(double(fmu.modelVariables.Boolean.start(value_2(i))));
                            refs(i) = fmu.modelVariables.Boolean.valueReference(value_2(i));
                        end
                    end
                    lhs_txt=""
                    lab_txt=""
                    rhs_txt=""
                    for i=1:size(SBools,"*")
                        lhs_txt=lhs_txt+'%v'+string(i)+',';
                        lab_txt=lab_txt+''''+SBools(i)+" "+''';';
                        rhs_txt=rhs_txt+'''vec'',-1,';

                    end
                    lhs_txt=part(lhs_txt,1:length(lhs_txt)-1);
                    lab_txt=part(lab_txt,1:length(lab_txt)-1);
                    rhs_txt=part(rhs_txt,1:length(rhs_txt)-1);
                    getvalue_txt = '[ok,'+lhs_txt+',value2]=scicos_getvalue(''Set Boolean (true = 1, false = 0)'',['+..
                    lab_txt+'],'+..
                    'list('+rhs_txt+'),value2)';
                    execstr(getvalue_txt);
                    value2 = strtod(value2)
                    // correct values 0 or 1
                    if ~and(value2==0 | value2==1) then
                        messagebox(["Answer given for ""Set Boolean"" is incorrect:"...
                        "Wrong values. Must be 1 or 0."],"modal","error");
                        ok = %f;
                    end
                    opar(9) = value2;
                    opar(8) = refs;
                    opar(8) = uint32([matrix(refs,-1,1);matrix(negAliasBool(opar(8)),-1,1)]);
                elseif ok & isempty(SBools)
                    opar(9) = [];
                    opar(8) = [];
                    graphics.exprs.setBoolean = sci2exp("");
                end
                if ~ok then
                    graphics.exprs.setBoolean = exprs.setBoolean;
                    opar(9) = tmpModelOparTwo;
                end
            end

            //***************************Get Values*****************************//
            // temp for output ports
            nameOut = graphics.exprs.out;
            outRef = graphics.exprs.outRefs;
            outType = graphics.exprs.outTypes;
            outLabel = nameOut;
            if ok then;
                GetValues = stripblanks(evstr(TGValue));
                GetValues = matrix(GetValues, size(GetValues, "*"), 1);
                tmpGraphicsGetReal = evstr(graphics.exprs.getValue);
                tmpModelOut = model.out;
                if ok & ~isempty(GetValues) then
                    allNames = [fmu.modelVariables.Real.name,...
                    fmu.modelVariables.Integer.name,...
                    fmu.modelVariables.Enumeration.name,...
                    fmu.modelVariables.Boolean.name];
                    for i=1:size(GetValues,"*")
                        if find(allNames==GetValues(i)) == [] then
                            messagebox(["Answer given for ""Get Values"" is incorrect:"...
                            "Wrong name "+""""+GetValues(i)+""" for getting values."],"modal","error");
                            ok = %f;
                            break;
                        end
                    end
                end

                if (~isempty(GetValues) & or(GetValues <> tmpGraphicsGetReal) | size(tmpGraphicsGetReal, "*") == 0) | (size(tmpGraphicsGetReal, "*") <> size(GetValues, "*")) then
                    refs = [];
                    types = [];
                    names = [];
                    var = [fmu.modelVariables.Integer.name, fmu.modelVariables.Enumeration.name];
                    varRef = [fmu.modelVariables.Integer.valueReference, fmu.modelVariables.Enumeration.valueReference]
                    for i=1:size(GetValues,"*")
                        if find(fmu.modelVariables.Real.name == GetValues(i)) then
                            refs(i) = fmu.modelVariables.Real.valueReference(find(fmu.modelVariables.Real.name == GetValues(i)))
                            types(i) = 1;
                            names(i) = GetValues(i);
                        elseif find(var == GetValues(i))
                            refs(i) = varRef(find(var == GetValues(i)))
                            types(i) = 2;
                            names(i) = GetValues(i);
                        elseif find(fmu.modelVariables.Boolean.name == GetValues(i))
                            refs(i) = fmu.modelVariables.Boolean.valueReference(find(fmu.modelVariables.Boolean.name == GetValues(i)))
                            types(i) = 3;
                            names(i) = GetValues(i);
                        end
                    end
                    outTypeAll = [outType';types];
                    opar(4) = [outRef,refs'];
                    opar(5) = int32([108 804 4 10](outTypeAll));
                    for i=1:size(outTypeAll, "*")
                        if outTypeAll(i) == 1 | outTypeAll(i) == 3 then
                            model.outtyp(i) = 1;
                        elseif outTypeAll(i) == 2 then
                            model.outtyp(i) = 3;
                        end
                    end
                    //output ports for Get values
                    outRealNames = ones(size([outRef,refs'],"*"),1);
                    model.out = outRealNames;
                    graphics.exprs.getValue = TGValue;
                    graphics.out_label = [outLabel';names];
                    // outputs negatedAlias
                    negatedAlias = [];
                    for i=1:size(opar(4),"c")
                        if outTypeAll(i) == 1 then
                            negatedAlias(i) = negAliasReal(opar(4)(i));
                        elseif outTypeAll(i) == 2
                            negatedAlias(i) = negAliasInt(opar(4)(i));
                        elseif outTypeAll(i) == 3
                            negatedAlias(i) = negAliasBool(opar(4)(i));
                        end
                    end
                    opar(4) = uint32([matrix(opar(4),-1,1);matrix(negatedAlias,-1,1)]);
                elseif isempty(GetValues)
                    graphics.exprs.getValue = sci2exp("");
                    model.out = ones(size(graphics.exprs.out,"c"),1);
                    opar(5) = int32([108 804 4 10](outType'));
                    negatedAlias = [];
                    for i=1:size(outRef,"c")
                        if outType(i) == 1 then
                            negatedAlias(i) = negAliasReal(outRef(i));
                        elseif outType(i) == 2
                            negatedAlias(i) = negAliasInt(outRef(i));
                        elseif outType(i) == 3
                            negatedAlias(i) = negAliasBool(outRef(i));
                        end
                    end
                    opar(4) = uint32([matrix(outRef,-1,1);matrix(negatedAlias,-1,1)]);
                else
                    graphics.out_label = [outLabel';evstr(graphics.exprs.getValue)];
                end
                if ~ok then
                    graphics.exprs.getValue = exprs.getValue;
                    model.out = tmpModelOut;
                end
            end
            // inputs negatedAlias
            if size(opar(2),"r") == 1 then
                negatedAlias = [];
                for i=1:size(opar(2),"c")
                    if inputTypes(i) == 1 then
                        negatedAlias(i) = negAliasReal(opar(2)(i));
                    elseif inputTypes(i) == 2
                        negatedAlias(i) = negAliasInt(opar(2)(i));
                    elseif inputTypes(i) == 3
                        negatedAlias(i) = negAliasBool(opar(2)(i));
                    end
                end
                opar(2) = uint32([matrix(opar(2),-1,1);matrix(negatedAlias,-1,1)]);
            end

            model.opar = opar;
            x.model = model;
            x.graphics = graphics;
        end
    case 'define' then
        //inputs
        in = [];
        inRefs = [];
        inTypes = [];
        //outputs
        out = [];
        outRefs = [];
        outTypes = [];
        //set Values
        setReal = "";
        setInteger = "";
        setBoolean = "";
        setString = "";

        //get Values
        getValue = emptystr();
        debugLogging = %f;
        relativeTolerance = %f;
        exprs = tlist(["FMInterface", "in", "inRefs", "inTypes", "out",...
        "outRefs", "outTypes", "setReal", "setInteger",...
        "setBoolean", "getValue", "debugLogging",...
        "relativeTolerance"],...
        sci2exp(in(:)),...
        inRefs,...
        inTypes,...
        sci2exp(out(:)),...
        outRefs,...
        outTypes,...
        sci2exp(setReal(:)),...
        sci2exp(setInteger(:)),...
        sci2exp(setBoolean(:)),...
        sci2exp(getValue(:)),...
        sci2exp(debugLogging(:)),...
        sci2exp(relativeTolerance(:)));

        model=scicos_model();
        model.sim=list("fmu_simulate_me",4);
        opar = list();
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
        x.graphics.in_label = in;
        x.graphics.out_label = out;
    end
endfunction
