//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Scilab Enterprises - Vladislav Trubkin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

//This function decompresses the FMU file at TMP Folder with the same name
//as the name of file FMU. Also this function returns "tlist" FMU.

function [fmu, objet] = importFMU(filepath)

    // Definition of the structure for the model file
    objet = struct(...
        "fmiVersion", [],...
        "fmiType", [],...
        "fmuPath", [],...
        "userPath", [],...
        "tempFolder", [],...
        "fmuFileName", [],...
        "unzippedFmu", [],...
        "dllInFMU", [],...
        "xmlPath", [],...
        "dllPath", [],...
        "isUnzipped", %f)

    // Creation the list for the future FMU
    // modelFileName - the  name of the file of model
    // xmlFile - Path for the XML file
    // simulationLibrary - Library for the simulation
    // descriptionFMU - mains attributes of XML description
    // modelVariables - description of all FMU variables
    fmu = tlist(["FMU",...
             "modelFileName",...
             "fmiType",...
             "xmlFile",...
             "libraryFile",...
             "relativePath",...
             "simulationLibrary",...
             "descriptionFMU",...
             "modelVariables",...
             "setValues",...
             "getValues"],...
             [], [], [], [], [], [], [], [],...
             struct("realNames",[],"realValues",[],...
             "integerNames",[],"integerValues",[],...
             "booleanNames",[],"booleanValues",[],...
             "stringNames",[],"stringValues",[]),...
             struct("realNames",[],...
             "integerNames",[],...
             "booleanNames",[],...
             "stringNames",[]));

    //user input string
    objet.userPath = filepath;
   //get the path
    //file_path = fullpath(filepath);
    file_path = getlongpathname(filepath);
    full_file = fullfile(filepath);
    //get the file extension
    ext = fileext(full_file);

    //creation the path of the FMU
    if isfile(file_path) then
        if (ext <> '.fmu') then
            error(msprintf(_("%s: Wrong extension '"%s'" of file: %s expected.\n"),"importFMU", ext, "fmu"));
        else
           fmu_path = file_path;
        end
    else
        error(msprintf(_("%s: The file %s doesn''t exist or is not read accessible.\n"),"importFMU", file_path));
    end

    //get the file name of the fmu
    [path, file_name, extension] = fileparts(filepath);
    objet.fmuPath = fmu_path;
    objet.fmuFileName = file_name;

    //path for a model
    //fmu.relativePath = pwd()+filesep()+"tests"+filesep()+"unit_tests"+filesep()+file_name+extension; //relative path for exemples of diagrams
    fmu.relativePath = fmu_path;
    //get a file for unzipping
    unzip_file = file_path;

    //create the temp folder
    [tmpPath, tmpFilename] = fileparts(tempname());
    tmp = fullfile(tmpPath + tmpFilename + "_fmu");
    if  isdir(tmp) then
        rmdir(tmp,'s')
    end
    mkdir(tmp);
    objet.tempFolder = tmp;

    //unzipping the fmu at temp folder
    //unzip_file = path of the fmu
    //overwrite files without prompting
    try 
        unzipped_fmu = decompress(unzip_file, tmp);
    catch
        error(10000, msprintf(_("%s: FMU was not unzipped."), "importFMU"));
    end
    //set unzipped fmu
    objet.unzippedFmu = unzipped_fmu;
    if  unzipped_fmu == [] then
        copyfile(unzip_file, tmp);
        cpFile = tmp + filesep() + file_name + extension;
        mvFile = tmp + filesep() + tmpFilename + "_fmu" + extension;
        // rename files
        movefile(cpFile, mvFile);
        unzipped_fmu = decompress(mvFile, tmp);
        deletefile(mvFile);
        if  unzipped_fmu == [] then
            error(10000, msprintf(_("%s: FMU was not unzipped."), "importFMU"));
        else
            objet.isUnzipped = %t;//FMU was unzipped successfully.
        end
    else
        objet.isUnzipped = %t;//FMU was unzipped successfully.
    end

    //Get xml file that contain a description of the fmu
    xml_description = isfile(tmp + filesep() + 'modelDescription.xml');
    xml_full_path = getlongpathname(tmp + filesep() + 'modelDescription.xml');
    if xml_description then
        objet.xmlPath = xml_full_path;
        fmu.xmlFile = objet.xmlPath;
    else
        error(msprintf(_("%s: The file %s doesn''t exist or is not read accessible.\n"),"importFMU", "modelDescription.xml"));
    end

    //Get the description of main attributes
    descriptionFMU = fmiModelDescription(fmu);
    //Get the description of model variables
    variables = fmiModelVariables(fmu);

    //check available library
    FMULibType = [isfile(fullfile(tmp+'\binaries\win32\'+ descriptionFMU.modelIdentifier +'.dll')),...
                  isfile(fullfile(tmp+'\binaries\win64\'+ descriptionFMU.modelIdentifier +'.dll')),...
                  isfile(fullfile(tmp+'\binaries\linux32\'+ descriptionFMU.modelIdentifier +'.so')),...
                  isfile(fullfile(tmp+'\binaries\linux64\'+ descriptionFMU.modelIdentifier +'.so'))];
    if ~FMULibType then
       error(10000, msprintf(_("%s: FMU has not available libraries."), "importFMU"));
    else
       libs = [];
        if FMULibType(1) then
           libs(1) = "win32";
        end
        if FMULibType(2) then
           libs(2) = "win64";
        end
        if FMULibType(3) then
           libs(3) = "linux32";
        end
        if FMULibType(4) then
           libs(4) = "linux64";
        end
    libs = [libs(find(FMULibType==%t))];
    libs = sci2exp(libs);
    //set libraries in struct
    objet.dllInFMU = libs;
    end
    if getos() == 'Windows' then
        if win64() then
            if isfile(tmp+'\binaries\win64\'+ descriptionFMU.modelIdentifier +'.dll') then
               objet.dllPath = fullpath(tmp+'\binaries\win64\' + descriptionFMU.modelIdentifier + '.dll');
            else
                mprintf("\nAvailable libraries in FMU: %s\n",libs);
                error(msprintf(_("%s: Library '"%s'" for Win64 does not exist or unsuitable XML description."), "importFMU", descriptionFMU.modelIdentifier + '.dll'));
            end
        else
            if isfile(tmp+'\binaries\win32\'+ descriptionFMU.modelIdentifier +'.dll') then
               objet.dllPath = fullpath(tmp+'\binaries\win32\' +  descriptionFMU.modelIdentifier + '.dll');
            else
               mprintf("\nAvailable libraries in FMU: %s\n",libs);
               error(10000, msprintf(_("%s: Library '"%s'" for Win32 does not exist or unsuitable XML description."), "importFMU", descriptionFMU.modelIdentifier + '.dll'));
            end
        end
    elseif getos() == 'Linux' then
           [sys,vers] = getversion();
           if vers(2) == "x64" then
             if isfile(fullfile(tmp+filesep()+'binaries'+filesep()+'linux64'+filesep()+ descriptionFMU.modelIdentifier +'.so')) then
                objet.dllPath = fullpath(tmp+filesep()+'binaries'+filesep()+'linux64'+filesep()+ descriptionFMU.modelIdentifier + '.so');
             else
                mprintf("\nAvailable libraries in FMU: %s\n",libs);
                error(msprintf(_("%s: Library '"%s'" for Linux32 does not exist or unsuitable XML description."), "importFMU", descriptionFMU.modelIdentifier + '.so'));
             end
           else
             if isfile(tmp+filesep()+'binaries'+filesep()+'linux32'+filesep()+ descriptionFMU.modelIdentifier +'.so') then
                objet.dllPath = fullpath(tmp+filesep()+'binaries'+filesep()+'linux32'+filesep() + descriptionFMU.modelIdentifier + '.so');
             else
                mprintf("\nAvailable libraries in FMU: %s\n",libs);
                error(msprintf(_("%s: Library '"%s'" for Linux64 does not exist or unsuitable XML description."), "importFMU", descriptionFMU.modelIdentifier + '.so'));
             end
           end
    end
    // using by fmu_link
    fmiType = "me";
    if descriptionFMU.fmiType == "CoSimulation" then
        fmiType = "cs";
    end
    version = int8(strtod( descriptionFMU.fmiVersion));
    // insertion in tlist
    // the implementation is explicit, some FMI 1.0 co-simulation libraries also export the model exchange functions
    simulationLibrary = fmu_link(objet.dllPath, descriptionFMU.modelIdentifier, fmiType + " " + descriptionFMU.fmiVersion);
    fmu.modelFileName = objet.fmuFileName;
    fmu.fmiType = fmiType;
    fmu.libraryFile = getlongpathname(objet.dllPath);
    fmu.simulationLibrary = simulationLibrary;
    fmu.descriptionFMU = descriptionFMU;
    fmu.modelVariables = variables;
    objet.fmiType = descriptionFMU.fmiType;
    objet.fmiVersion = descriptionFMU.fmiVersion;
    // set the input names in the structure
    fmu.setValues.inputs = [fmu.modelVariables.Real.name(find(fmu.modelVariables.Real.causality == "input")),...
                            fmu.modelVariables.Integer.name(find(fmu.modelVariables.Integer.causality == "input")),...
                            fmu.modelVariables.Boolean.name(find(fmu.modelVariables.Boolean.causality == "input")),...
                            fmu.modelVariables.String.name(find(fmu.modelVariables.String.causality == "input"))];
    for i=1:size(fmu.setValues.inputs,"*")
        execstr('fmu.setValues.input_'+string(i)+'_values = []');
    end

endfunction
