//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

// Callback function for M2SCI GUI (See m2sci_gui.sci)

function cb_m2sci_gui

//
// --- Switch to file conversion mode ---
//
if get(gcbo,"tag")=="fileradio" then

    set(gcbo, "value", 1);
    set(findobj("tag", "dirradio"), "value", 0);
    
    // Make dir objects not-visible
    set(findobj("tag", "dirtitle"),"visible","off");
    set(findobj("tag", "dirlabel"),"visible","off");
    set(findobj("tag", "diredit"),"visible","off");
    set(findobj("tag", "dirbtn"),"visible","off");

    // Make file objects visible
    set(findobj("tag", "filetitle"),"visible","on");
    set(findobj("tag", "filelabel"),"visible","on");
    set(findobj("tag", "fileedit"),"visible","on");
    set(findobj("tag", "filebtn"),"visible","on");
  
//
// --- Switch to directory conversion mode ---
//
elseif get(gcbo,"tag")=="dirradio" then
    
    set(gcbo, "value", 1);
    set(findobj("tag", "fileradio"), "value", 0);
    
    // Make dir objects visible
    set(findobj("tag", "dirtitle"),"visible","on");
    set(findobj("tag", "dirlabel"),"visible","on");
    set(findobj("tag", "diredit"),"visible","on");
    set(findobj("tag", "dirbtn"),"visible","on");

    // Make file objects not-visible
    set(findobj("tag", "filetitle"),"visible","off");
    set(findobj("tag", "filelabel"),"visible","off");
    set(findobj("tag", "fileedit"),"visible","off");
    set(findobj("tag", "filebtn"),"visible","off");
    
//
// --- Directory to convert ---
//
elseif get(gcbo,"tag")=="dirbtn" then
  
  directoryname = uigetdir(pwd(), gettext("Select the directory to convert"));
  set(findobj("tag", "diredit"), "string", directoryname);
  
//
// --- File to convert ---
//
elseif get(gcbo,"tag")=="filebtn" then
  
  filename = uigetfile("*.m", pwd(), gettext("Select the file to convert"));
  set(findobj("tag", "fileedit"), "string", filename);
  
//
// --- Output directory ---
//
elseif get(gcbo,"tag")=="outbtn" then
  
  directoryname = uigetdir(pwd(), gettext("Select the directory for generated files"));
  set(findobj("tag", "outedit"), "string", directoryname);
  
//
// --- Recmode option ---
//
elseif or(get(gcbo, "tag")==["recradioyes","recradiono"]) then
  
  set(findobj("tag", "recradioyes"), "value", 0);
  set(findobj("tag", "recradiono"), "value", 0);
  
  set(gcbo, "value", 1);
  
//
// --- Double option ---
//
elseif or(get(gcbo, "tag")==["doubradioyes","doubradiono"]) then
  
  set(findobj("tag", "doubradioyes"), "value", 0);
  set(findobj("tag", "doubradiono"), "value", 0);
  
  set(gcbo, "value", 1);
  
//
// --- Verbose option ---
//
elseif or(get(gcbo, "tag")==["verbradio0","verbradio1","verbradio2","verbradio3"]) then
  
  set(findobj("tag", "verbradio0"), "value", 0);
  set(findobj("tag", "verbradio1"), "value", 0);
  set(findobj("tag", "verbradio2"), "value", 0);
  set(findobj("tag", "verbradio3"), "value", 0);
  
  set(gcbo, "value", 1);
  
//
// --- Pretty print option ---
//
elseif or(get(gcbo, "tag")==["ppradioyes","ppradiono"]) then
  
  set(findobj("tag", "ppradioyes"), "value", 0);
  set(findobj("tag", "ppradiono"), "value", 0);
  
  set(gcbo, "value", 1);
  
//
// --- Cancel conversion ---
//
elseif get(gcbo, "tag")=="cancelbtn" | get(gcbo, "tag")=="close_menu" then
  //delete(findobj("tag", "m2scifig"));
  delete(gcf())

//
// --- Launch conversion ---
//
elseif get(gcbo, "tag")=="convertbtn" then
  outputdir = get(findobj("tag", "outedit"), "string");
  
  rec = get(findobj("tag", "recradioyes"), "value") == 1;
  
  doub = get(findobj("tag", "doubradioyes"), "value") == 1;
  
  if get(findobj("tag", "verbradio0"), "value") == 1 then
    verb = 0;
  elseif get(findobj("tag", "verbradio1"), "value") == 1 then
    verb = 1;
  elseif get(findobj("tag", "verbradio2"), "value") == 1 then
    verb = 2;
  else
    verb = 3;
  end
  
  pp = get(findobj("tag", "ppradioyes"), "value") == 1;
  
  if get(findobj("tag", "fileradio"), "value") == 1 then // Single file conversion
    inputfile = get(findobj("tag", "fileedit"), "string");
    //delete(findobj("tag", "m2scifig"));
    delete(gcf());
    mfile2sci(inputfile, outputdir, rec, doub, verb, pp);
  else // Directory conversion
    inputdir = get(findobj("tag", "diredit"), "string");
    //delete(findobj("tag", "m2scifig"));
    delete(gcf());
    translatepaths(inputdir, outputdir);
  end

//
// --- mfile2sci help ---
//
elseif get(gcbo, "tag")=="mfile2sci_help_menu" then
  help mfile2sci

//
// --- translatepaths help ---
//
elseif get(gcbo, "tag")=="translatepaths_help_menu" then
  help translatepaths
  
//
// --- About M2SCI ---
//
elseif get(gcbo, "tag")=="about_m2sci_menu" then
  help(gettext("About_M2SCI_tools"))
end

endfunction
