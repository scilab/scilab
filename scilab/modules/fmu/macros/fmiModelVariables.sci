//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Scilab Enterprises - Vladislav Trubkin
// Copyright (C) - 2013 - Scilab Enterprises - Clement David
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

//This function returns all info about ModelVariables. 

function [vars] = fmiModelVariables(model)
    [lhs,rhs]=argn()
    if rhs <> 1 then
        error(999, msprintf(_("%s: Wrong number of input arguments: %d expected.\n"), "fmiModelVariables", 1));
    elseif typeof(model) <> "FMU" then
        if ~isfile(string(model)) then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiModelVariables", 1, "FMU"));
        elseif part(string(model),$-2:$) <> "xml" then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiModelVariables", 1, "FMU"));
        end
    end
    isXml = %f;
    if typeof(model) == "FMU" then
        if ~isempty(model.modelVariables) then
            vars = model.modelVariables;
            return; 
        end
        modelDescription =  xmlRead(model.xmlFile);
    else
        modelDescription =  xmlRead(model);
        isXml = %t;
    end
    if typeof(modelDescription) <> "XMLDoc" then
       error(msprintf(_("%s: The file %s doesn''t exist or is not read accessible.\n"),"fmiModelVariables", "modelDescription.xml"));
    end

//helper function to find an Offset and a Gain of units 
function [gain, offset] = getModelBaseGainOffset(unitName)
posUnitDef = find(modelDescription.root.children.name == "UnitDefinitions");
gain = 1;
offset = 0;
unit = [];
if ~isempty(posUnitDef) then
    for i=1:modelDescription.root.children(posUnitDef).children.size
           unit(i) = modelDescription.root.children(posUnitDef).children(i).attributes.unit; 
        if unit(i) == unitName & modelDescription.root.children(posUnitDef).children(i).children.size then
            if ~isempty(modelDescription.root.children(posUnitDef).children(i).children(1).attributes.gain) then
                gain = strtod(modelDescription.root.children(posUnitDef).children(i).children(1).attributes.gain)
            end
            if ~isempty(modelDescription.root.children(posUnitDef).children(i).children(1).attributes.offset) then
                offset = strtod(modelDescription.root.children(posUnitDef).children(i).children(1).attributes.offset)
            end
        end
    end
end
endfunction

    //find TypeDefinitions first
    posTypeDef = find(modelDescription.root.children.name == "TypeDefinitions");

    if ~isempty(posTypeDef) then
        TypeDef = emptystr(modelDescription.root.children(posTypeDef).children.size, 1);
        for i=1:modelDescription.root.children(posTypeDef).children.size 
            //Types of Variable
            if ~isempty(modelDescription.root.children(posTypeDef).children(i).attributes.name) then
                TypeDef(i) = modelDescription.root.children(posTypeDef).children(i).attributes.name;
            end   
        end
        
        // Type initialisation from the specification
        TypeName = emptystr(TypeDef);
        TypeDescription = emptystr(TypeDef);
        TypeQuantity = emptystr(TypeDef);
        TypeUnit = emptystr(TypeDef);
        TypeDisplayUnit = emptystr(TypeDef);
        TypeRelativeQuantity = 1 == zeros(TypeDef);
        TypeMin = -%inf + zeros(TypeDef);
        TypeMax = %inf + zeros(TypeDef);
        TypeNominal = ones(TypeDef); 
        Items = zeros(TypeDef); // index inside the ItemsList
        ItemsList = list();
        TypeGain = ones(TypeDef);
        TypeOffset = zeros(TypeDef);
        
        for i=1:size(TypeDef, "*")
            //RealType BooleanType IntegerType
            if ~isempty(modelDescription.root.children(posTypeDef).children(i).attributes.name) then
                TypeName(i) = modelDescription.root.children(posTypeDef).children(i).attributes.name
            end
            if ~isempty(modelDescription.root.children(posTypeDef).children(i).attributes.description) then
                TypeDescription(i) = modelDescription.root.children(posTypeDef).children(i).attributes.description 
            end
            if ~isempty(modelDescription.root.children(posTypeDef).children(i).children(1).attributes.quantity) then
                TypeQuantity(i) = modelDescription.root.children(posTypeDef).children(i).children(1).attributes.quantity
            end    
            if ~isempty(modelDescription.root.children(posTypeDef).children(i).children(1).attributes.unit) then
                TypeUnit(i) = modelDescription.root.children(posTypeDef).children(i).children(1).attributes.unit   
            end
            if ~isempty(modelDescription.root.children(posTypeDef).children(i).children(1).attributes.displayUnit) then
                TypeDisplayUnit(i) = modelDescription.root.children(posTypeDef).children(i).children(1).attributes.displayUnit
            end
            [TypeGain(i), TypeOffset(i)] = getModelBaseGainOffset(TypeUnit(i));
            TypeRelativeQuantity(i) = modelDescription.root.children(posTypeDef).children(i).children(1).attributes.relativeQuantitiy == "true"
            if ~isempty(modelDescription.root.children(posTypeDef).children(i).children(1).attributes.min) then
                TypeMin(i) = strtod(modelDescription.root.children(posTypeDef).children(i).children(1).attributes.min)
            end    
            if ~isempty(modelDescription.root.children(posTypeDef).children(i).children(1).attributes.max) then
                TypeMax(i) = strtod(modelDescription.root.children(posTypeDef).children(i).children(1).attributes.max)
            end    
            if ~isempty(modelDescription.root.children(posTypeDef).children(i).children(1).attributes.nominal) then
                TypeNominal(i) = strtod(modelDescription.root.children(posTypeDef).children(i).children(1).attributes.nominal)
            end
            //Enumeration type
            if modelDescription.root.children(posTypeDef).children(i).children(1).name == "EnumerationType" then
                if ~isempty(modelDescription.root.children(posTypeDef).children(i).children(1).attributes.min) then
                    TypeMin(i) = strtod(modelDescription.root.children(posTypeDef).children(i).children(1).attributes.min)
                end
                if ~isempty(modelDescription.root.children(posTypeDef).children(i).children(1).attributes.max) then
                    TypeMax(i) = strtod(modelDescription.root.children(posTypeDef).children(i).children(1).attributes.max)
                end

                enumLen = modelDescription.root.children(posTypeDef).children(i).children(1).children.size;
                //All items for current name
                ItemData = emptystr(enumLen, 2);
                for j = 1:enumLen
                    if ~isempty(modelDescription.root.children(posTypeDef).children(i).children(1).children(j).attributes.name) then 
                        ItemData(j,1) = modelDescription.root.children(posTypeDef).children(i).children(1).children(j).attributes.name
                    end
                    if ~isempty(modelDescription.root.children(posTypeDef).children(i).children(1).children(j).attributes.description) then 
                        ItemData(j,2) = modelDescription.root.children(posTypeDef).children(i).children(1).children(j).attributes.description 
                    end
                end
                ItemsList($+1) = ItemData;
                Items(i) = size(ItemsList($),"r");
            end
        end
    else
        // Type initialisation from the specification
        TypeName = "";
        TypeDescription = "";
        TypeQuantity = "";
        TypeUnit = "";
        TypeDisplayUnit = "";
        TypeRelativeQuantity = 0;
        TypeMin = -%inf;
        TypeMax = %inf;
        TypeNominal = 1; 
        Items = 0; // index inside the ItemsList
        ItemsList = list();
        TypeGain = 1;
        TypeOffset = 0;
    end
   
    listType = mlist(...
        ["TypeFMU", "name",...
            "description",...
            "quantity",...
            "unit",...
            "displayUnit",...
            "gain",...
            "offset",...
            "relativeQuantity",...
            "min",...
            "max",...
            "nominal",...
            "items",...
            "itemList"],... 
        TypeName, TypeDescription,...
        TypeQuantity, TypeUnit,...
        TypeDisplayUnit,TypeGain, TypeOffset,...
        TypeRelativeQuantity, TypeMin, TypeMax,...
        TypeNominal,Items,ItemsList);                         

    //find ModelVariables common attributes
    posModelVar = find(modelDescription.root.children.name == "ModelVariables");
    //Do not use .children: XML comments count as children too, and Dymola
    //(for example) writes one before every ScalarVariable.
    //*[1] is the type element whatever else the variable carries after it.
    xp = xmlXPath(modelDescription, "/fmiModelDescription/ModelVariables/ScalarVariable/*[1]");
    VarType = emptystr(xp.size, 1);
    for i=1:xp.size
        VarType(i) = xp(i).name;
    end

    //A variable's rank is not its position among the children, so
    //every access below goes through this map.
    svPos = find(modelDescription.root.children(posModelVar).children.name == "ScalarVariable");
    
    // Common shared attributes with default values handling
    xp = xmlXPath(modelDescription, "/fmiModelDescription/ModelVariables/ScalarVariable/@name");
    commonNames = xp.content;
    xp = xmlXPath(modelDescription, "/fmiModelDescription/ModelVariables/ScalarVariable/@valueReference");
    commonReferences = strtod(xp.content);
    xp = xmlXPath(modelDescription, "/fmiModelDescription/ModelVariables/ScalarVariable[not(@description)] | /fmiModelDescription/ModelVariables/ScalarVariable/@description");
    commonDescription = xp.content;
    xp = xmlXPath(modelDescription, "/fmiModelDescription/ModelVariables/ScalarVariable[not(@variability)] | /fmiModelDescription/ModelVariables/ScalarVariable/@variability");
    commonVariability = xp.content;
    commonVariability(find(commonVariability == "")) = "continuous";
    xp = xmlXPath(modelDescription, "/fmiModelDescription/ModelVariables/ScalarVariable[not(@causality)] | /fmiModelDescription/ModelVariables/ScalarVariable/@causality");
    commonCausality = xp.content;
    commonCausality(find(commonCausality == "")) = "internal";
    xp = xmlXPath(modelDescription, "/fmiModelDescription/ModelVariables/ScalarVariable[not(@alias)] | /fmiModelDescription/ModelVariables/ScalarVariable/@alias");
    commonAlias = xp.content;
    commonAlias(find(commonAlias == "")) = "noAlias";

    //RealType
    typeReal = find(VarType == "Real");

    // Real default initialisation from the spec
    VarName = commonNames(typeReal);
    VarRef = commonReferences(typeReal);
    VarDescription = commonDescription(typeReal);
    VarVariability = commonVariability(typeReal);
    VarCausality = commonCausality(typeReal);
    VarAlias = commonAlias(typeReal);
    
    xp = xmlXPath(modelDescription, "/fmiModelDescription/ModelVariables/ScalarVariable/Real[not(@declaredType)] | /fmiModelDescription/ModelVariables/ScalarVariable/Real/@declaredType");
    VarDeclaredType = xp.content;
    VarQuantity = emptystr(typeReal);
    VarUnit = emptystr(typeReal);
    VarDisplayUnit = emptystr(typeReal);
    VarRelativeQuantity = 1 == zeros(typeReal);
    VarMin = -%inf + zeros(typeReal);
    VarMax = %inf + zeros(typeReal);
    VarNominal = ones(typeReal);
    VarStart =  %nan + zeros(typeReal);
    VarFixed =  0 == zeros(typeReal);
    VarDependency = list();
    
    // Loop to decode all Real entries
    for i=1:size(typeReal, "*")
        //Declared type of Variable is performed through xpath for performance reasons
        
        //quantitity of Variable
        if ~isempty(VarDeclaredType(i)) then
            VarQuantity(i) = listType.quantity(find(listType.name == VarDeclaredType(i)))
        end
        if ~isempty(modelDescription.root.children(posModelVar).children(svPos(typeReal(i))).children(1).attributes.quantity) then
            VarQuantity(i) = modelDescription.root.children(posModelVar).children(svPos(typeReal(i))).children(1).attributes.quantity;
        end
        //unit of Variable
        if ~isempty(VarDeclaredType(i)) then
            VarUnit(i) = listType.unit(find(listType.name == VarDeclaredType(i)))
        end
        if ~isempty(modelDescription.root.children(posModelVar).children(svPos(typeReal(i))).children(1).attributes.unit) then
            VarUnit(i) =  modelDescription.root.children(posModelVar).children(svPos(typeReal(i))).children(1).attributes.unit;
        end
        //displayUnit of Variable
        if ~isempty(VarDeclaredType(i)) then
            VarDisplayUnit(i) = listType.displayUnit(find(listType.name == VarDeclaredType(i)))
        end
        if ~isempty(modelDescription.root.children(posModelVar).children(svPos(typeReal(i))).children(1).attributes.displayUnit) then
            VarDisplayUnit(i) =  modelDescription.root.children(posModelVar).children(svPos(typeReal(i))).children(1).attributes.displayUnit;
        end
        //relativeQuantity of Variable
        if ~isempty(VarDeclaredType(i)) then
            VarRelativeQuantity(i) = listType.relativeQuantity(find(listType.name == VarDeclaredType(i)));
        end
        if ~isempty(modelDescription.root.children(posModelVar).children(svPos(typeReal(i))).children(1).attributes.relativeQuantity) then
            VarRelativeQuantity(i) = modelDescription.root.children(posModelVar).children(svPos(typeReal(i))).children(1).attributes.relativeQuantity == "true"; 
        end
        //min value of Variable
        if ~isempty(VarDeclaredType(i)) then
            VarMin(i) = listType.min(find(listType.name == VarDeclaredType(i)))
        end
        if ~isempty(modelDescription.root.children(posModelVar).children(svPos(typeReal(i))).children(1).attributes.min) then
            VarMin(i) = strtod(modelDescription.root.children(posModelVar).children(svPos(typeReal(i))).children(1).attributes.min);
        end 
        //max value of Variable
        if ~isempty(VarDeclaredType(i)) then
            VarMax(i) = listType.max(find(listType.name == VarDeclaredType(i)))
        end
        if ~isempty(modelDescription.root.children(posModelVar).children(svPos(typeReal(i))).children(1).attributes.max) then
            VarMax(i) = strtod(modelDescription.root.children(posModelVar).children(svPos(typeReal(i))).children(1).attributes.max);
        end 
        //nominal value of Variable
        if ~isempty(VarDeclaredType(i)) then
            VarNominal(i) = listType.nominal(find(listType.name == VarDeclaredType(i)))
        end
        if ~isempty(modelDescription.root.children(posModelVar).children(svPos(typeReal(i))).children(1).attributes.nominal) then
            VarNominal(i) = strtod(modelDescription.root.children(posModelVar).children(svPos(typeReal(i))).children(1).attributes.nominal);
        end 
        //start value of Variable
        if ~isempty(modelDescription.root.children(posModelVar).children(svPos(typeReal(i))).children(1).attributes.start) then
            VarStart(i) = strtod(modelDescription.root.children(posModelVar).children(svPos(typeReal(i))).children(1).attributes.start);
        elseif VarCausality(i) == 'input' & isempty(modelDescription.root.children(posModelVar).children(svPos(typeReal(i))).children(1).attributes.start) then
            VarStart(i) = 0; 
        end
        //is fixed value of Variable
        VarFixed(i) = modelDescription.root.children(posModelVar).children(svPos(typeReal(i))).children(1).attributes.fixed <> "false";
        // Direct Dependency
        if VarCausality(i) == "output" then
            xp = xmlXPath(modelDescription, "/fmiModelDescription/ModelVariables/ScalarVariable["+string(i)+"]/DirectDependency/Name");
            VarDependency(i) = "";
            if xp.size <> 0 then
                VarDependency(i) = xp.content;
            end
        else
            VarDependency(i) = "";
        end
    end

    listReal = mlist(...
        ["RealFMU", "name",...
            "valueReference",...
            "description",...
            "variability",...
            "causality",...
            "alias",...
            "declaredType",...
            "quantity",...
            "unit",...
            "displayUnit",...
            "relativeQuantity",...
            "min",...
            "max",...
            "nominal",...
            "start",...
            "fixed",...
            "DirectDependency"], ...
        VarName, VarRef,...
        VarDescription, VarVariability,...
        VarCausality, VarAlias,...
        VarDeclaredType, VarQuantity,...
        VarUnit, VarDisplayUnit,...
        VarRelativeQuantity,VarMin,...
        VarMax,VarNominal,...
        VarStart, VarFixed,...
        VarDependency);

    //integer Type
    typeInteger = find(VarType == "Integer");

    if ~isempty(typeInteger) then
        // Integer default initialisation from the spec
        VarName = commonNames(typeInteger);
        VarRef = commonReferences(typeInteger);
        VarDescription = commonDescription(typeInteger);
        VarVariability = commonVariability(typeInteger);
        VarVariability(find(VarVariability == "continuous")) = "discrete"; // issue #1251
        VarCausality = commonCausality(typeInteger);
        VarAlias = commonAlias(typeInteger);
        
        xp = xmlXPath(modelDescription, "/fmiModelDescription/ModelVariables/ScalarVariable/Integer[not(@declaredType)] | /fmiModelDescription/ModelVariables/ScalarVariable/Integer/@declaredType");
        VarDeclaredType = xp.content;
        VarQuantity = emptystr(typeInteger);
        VarMin = -2147483648 + zeros(typeInteger);
        VarMax = 2147483647 + zeros(typeInteger);
        VarStart = %nan + zeros(typeInteger);
        VarFixed =  0 == zeros(typeInteger);

        for i=1:size(typeInteger, "*")
            //Declared type of Variable is performed through xpath for performance reasons
            
            //quantitity of Variable
            if ~isempty(VarDeclaredType(i)) then
                VarQuantity(i) = listType.quantity(find(listType.name == VarDeclaredType(i)))
            end
            if ~isempty(modelDescription.root.children(posModelVar).children(svPos(typeInteger(i))).children(1).attributes.quantity) then
                VarQuantity(i) = modelDescription.root.children(posModelVar).children(svPos(typeInteger(i))).children(1).attributes.quantity;
            end
            //min value of Variable
            if ~isempty(VarDeclaredType(i)) then
                VarMin(i) = listType.min(find(listType.name == VarDeclaredType(i)));
            end
            if ~isempty(modelDescription.root.children(posModelVar).children(svPos(typeInteger(i))).children(1).attributes.min) then
                VarMin(i) = strtod(modelDescription.root.children(posModelVar).children(svPos(typeInteger(i))).children(1).attributes.min);
            end
            //max value of Variable
            if ~isempty(VarDeclaredType(i)) then
                VarMax(i) = listType.max(find(listType.name == VarDeclaredType(i)))
            end
            if ~isempty(modelDescription.root.children(posModelVar).children(svPos(typeInteger(i))).children(1).attributes.max) then
                VarMax(i) = strtod(modelDescription.root.children(posModelVar).children(svPos(typeInteger(i))).children(1).attributes.max);
            end
            //start value of Variable
            if ~isempty(modelDescription.root.children(posModelVar).children(svPos(typeInteger(i))).children(1).attributes.start) then
                VarStart(i) = strtod(modelDescription.root.children(posModelVar).children(svPos(typeInteger(i))).children(1).attributes.start);
            elseif VarCausality(i) == 'input' & isempty(modelDescription.root.children(posModelVar).children(svPos(typeInteger(i))).children(1).attributes.start) then
                VarStart(i) = 0;
            end
            //is fixed value of Variable
            VarFixed(i) = modelDescription.root.children(posModelVar).children(svPos(typeInteger(i))).children(1).attributes.fixed <> "false";
        end
    else
        VarName = [];
        VarRef = [];
        VarDescription = [];
        VarVariability = [];
        VarCausality = [];
        VarAlias = [];
        
        VarDeclaredType = [];
        VarQuantity = "";
        VarMin = -2147483648;
        VarMax = 2147483647;
        VarStart = %nan;
        VarFixed = %f;
    end
    
    //list of Integer variables
    listInteger = mlist(...
        ["IntFMU", "name",...
            "valueReference",...
            "description",...
            "variability",...
            "causality",...
            "alias",...
            "declaredType",...
            "quantity",...
            "min",...
            "max",...
            "start",...
            "fixed"], ...
        VarName, VarRef,...
        VarDescription, VarVariability,...
        VarCausality, VarAlias,...
        VarDeclaredType, VarQuantity,...
        VarMin, VarMax,...
        VarStart, VarFixed);

    //boolean Type
    typeBoolean = find(VarType == "Boolean");

    if ~isempty(typeBoolean) then
        // Boolean default initialisation from the spec
        VarName = commonNames(typeBoolean);
        VarRef = commonReferences(typeBoolean);
        VarDescription = commonDescription(typeBoolean);
        VarVariability = commonVariability(typeBoolean);
        VarVariability(find(VarVariability == "continuous")) = "discrete"; // issue #1251
        VarCausality = commonCausality(typeBoolean);
        VarAlias = commonAlias(typeBoolean);
        
        xp = xmlXPath(modelDescription, "/fmiModelDescription/ModelVariables/ScalarVariable/Boolean[not(@declaredType)] | /fmiModelDescription/ModelVariables/ScalarVariable/Boolean/@declaredType");
        VarDeclaredType = xp.content;
        VarStart = 1 == zeros(typeBoolean);
        VarFixed = 0 == zeros(typeBoolean);

        for i=1:size(typeBoolean, "*")
            //Declared type of Variable is performed through xpath for performance reasons
            
            //start value of Variable
            VarStart(i) = modelDescription.root.children(posModelVar).children(svPos(typeBoolean(i))).children(1).attributes.start == "true";
            //is fixed value of Variable
            VarFixed(i) = modelDescription.root.children(posModelVar).children(svPos(typeBoolean(i))).children(1).attributes.fixed <> "false";
        end
    else
        // Boolean default initialisation from the spec
        VarName = [];
        VarRef = [];
        VarDescription = [];
        VarVariability = [];
        VarCausality = [];
        VarAlias = [];
        
        VarDeclaredType = [];
        VarStart = %f;
        VarFixed = %f;
    end
    
    listBoolean = mlist(...
        ["BoolFMU", "name",...
            "valueReference",...
            "description",...
            "variability",...
            "causality",...
            "alias",...
            "declaredType",...
            "start",...
            "fixed"], ...
        VarName, VarRef,...
        VarDescription, VarVariability,...
        VarCausality, VarAlias,...
        VarDeclaredType, VarStart,...
        VarFixed);

    //string Type
    typeString = find(VarType == "String");

    if ~isempty(typeString) then
        // String default initialisation from the spec
        VarName = commonNames(typeString);
        VarRef = commonReferences(typeString);
        VarDescription = commonDescription(typeString);
        VarVariability = commonVariability(typeString);
        VarVariability(find(VarVariability == "continuous")) = "discrete"; // issue #1251
        VarCausality = commonCausality(typeString);
        VarAlias = commonAlias(typeString);
        
        xp = xmlXPath(modelDescription, "/fmiModelDescription/ModelVariables/ScalarVariable/String[not(@declaredType)] | /fmiModelDescription/ModelVariables/ScalarVariable/String/@declaredType");
        VarDeclaredType = xp.content;
        VarStart = emptystr(typeString);
        VarFixed = 0 == zeros(typeString);
        
        for i=1:size(typeString, "*")
            //Declared type of Variable is performed through xpath for performance reasons
            
            //Declared type of Variable
            if ~isempty(modelDescription.root.children(posModelVar).children(svPos(typeString(i))).children(1).attributes.declaredType) then
                VarDeclaredType(i) =  modelDescription.root.children(posModelVar).children(svPos(typeString(i))).children(1).attributes.declaredType;
            end
            //start value of Variable
            if ~isempty(modelDescription.root.children(posModelVar).children(svPos(typeString(i))).children(1).attributes.start) then
                VarStart(i) =  modelDescription.root.children(posModelVar).children(svPos(typeString(i))).children(1).attributes.start;
            end
            //is fixed value of Variable
            VarFixed(i) = modelDescription.root.children(posModelVar).children(svPos(typeString(i))).children(1).attributes.fixed <> "false";
        end
    else
        VarName = [];
        VarRef = [];
        VarDescription = [];
        VarVariability = [];

        VarCausality = [];
        VarAlias = [];
        
        VarDeclaredType = "";
        VarStart = "";
        VarFixed = %f;
    end
    
    listString = mlist(...
        ["StrFMU", "name",...
            "valueReference",...
            "description",...
            "variability",...
            "causality",...
            "alias",...
            "declaredType",...
            "start",...
            "fixed"], ...
        VarName, VarRef,...
        VarDescription, VarVariability,...
        VarCausality, VarAlias,...
        VarDeclaredType, VarStart,... 
        VarFixed);

    //enumeration Type
    typeEnumeration = find(VarType == "Enumeration");

    if ~isempty(typeEnumeration) then
        // Enumeration default initialisation from the spec
        VarName = commonNames(typeEnumeration);
        VarRef = commonReferences(typeEnumeration);
        VarDescription = commonDescription(typeEnumeration);
        VarVariability = commonVariability(typeEnumeration);
        VarVariability(find(VarVariability == "continuous")) = "discrete"; // issue #1251
        VarCausality = commonCausality(typeEnumeration);
        VarAlias = commonAlias(typeEnumeration);
        
        xp = xmlXPath(modelDescription, "/fmiModelDescription/ModelVariables/ScalarVariable/Enumeration[not(@declaredType)] | /fmiModelDescription/ModelVariables/ScalarVariable/Enumeration/@declaredType");
        VarDeclaredType = xp.content;
        VarQuantity = emptystr(typeEnumeration);
        VarMin = -%inf + zeros(typeEnumeration);
        VarMax = %inf + zeros(typeEnumeration);
        VarStart =  %nan + zeros(typeEnumeration);
        VarFixed =  0 == zeros(typeEnumeration);

        for i=1:size(typeEnumeration, "*")
            //Declared type of Variable is performed through xpath for performance reasons
            
            //quantitity of Variable
            if ~isempty(VarDeclaredType(i)) then
                VarQuantity(i) = listType.quantity(find(listType.name == VarDeclaredType(i)))
            end
            if ~isempty(modelDescription.root.children(posModelVar).children(svPos(typeEnumeration(i))).children(1).attributes.quantity) then
                VarQuantity(i) = modelDescription.root.children(posModelVar).children(svPos(typeEnumeration(i))).children(1).attributes.quantity;
            end
            //min value of Variable
            if ~isempty(VarDeclaredType(i)) then
                VarMin(i) = listType.min(find(listType.name == VarDeclaredType(i)));
            end
            if ~isempty(modelDescription.root.children(posModelVar).children(svPos(typeEnumeration(i))).children(1).attributes.min) then
                VarMin(i) = modelDescription.root.children(posModelVar).children(svPos(typeEnumeration(i))).children(1).attributes.min;
            end
            //max value of Variable
            if ~isempty(VarDeclaredType(i)) then
                VarMax(i) = listType.max(find(listType.name == VarDeclaredType(i)));
            end
            if ~isempty(modelDescription.root.children(posModelVar).children(svPos(typeEnumeration(i))).children(1).attributes.max) then
                VarMax(i) = modelDescription.root.children(posModelVar).children(svPos(typeEnumeration(i))).children(1).attributes.max;
            end
            //start value of Variable
            if ~isempty(modelDescription.root.children(posModelVar).children(svPos(typeEnumeration(i))).children(1).attributes.start) then
                VarStart(i) = strtod(modelDescription.root.children(posModelVar).children(svPos(typeEnumeration(i))).children(1).attributes.start);
            elseif VarCausality(i) == 'input' & isempty(modelDescription.root.children(posModelVar).children(svPos(typeEnumeration(i))).children(1).attributes.start) then
                VarStart(i) = 0;
            end
            //is fixed value of Variable
            VarFixed(i) = modelDescription.root.children(posModelVar).children(svPos(typeEnumeration(i))).children(1).attributes.fixed <> "false";
        end
    else
        // Enumeration default initialisation from the spec
        VarName = [];
        VarRef = [];
        VarDescription = [];
        VarVariability = [];
        VarCausality = [];
        VarAlias = [];
        
        VarDeclaredType = "";
        VarQuantity = "";
        VarMin = -%inf;
        VarMax = %inf;
        VarStart =  %nan;
        VarFixed =  %f;
    end

    listEnumeration = mlist(...
        ["EnumFMU", "name",...
            "valueReference",...
            "description",...
            "variability",...
            "causality",...
            "alias",...
            "declaredType",...
            "quantity",...
            "min",...
            "max",...
            "start",...
            "fixed"], ...
        VarName, VarRef,...
        VarDescription, VarVariability,...
        VarCausality, VarAlias,...
        VarDeclaredType, VarQuantity,...
        VarMin, VarMax,...
        VarStart, VarFixed);

    vars = mlist(["InfoFMU", "Real",...
        "Integer", "Boolean", "String",...
        "Enumeration", "Type"],...
        listReal, listInteger,...
        listBoolean, listString,...
        listEnumeration, listType);
    
    // delete xml description from memory
    xmlDelete(modelDescription); 
endfunction
