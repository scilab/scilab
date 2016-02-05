// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Simon GARESTE <simon.gareste@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- CLI SHELL MODE -->


//detect scilab arch
[version, opts] = getversion();
if opts(2) == "x64" & getos() == "Windows" then
    arch = "x64/";
else
    arch = "";
end

load("SCI/modules/atoms/macros/atoms_internals/lib");

// If previous test did not end properly, restore, else backup config file
atomsRestoreConfig(%T);
atomsSaveConfig();

// Do not use the autoload system
atomsSetConfig("autoloadAddAfterInstall","False");
atomsSetConfig("Verbose" ,"False");

// Load the 1st scenario : See scene1.test.atoms.scilab.org.txt
// =============================================================================
atomsRepositorySetOfl("http://scene1.test.atoms.scilab.org");
if atomsIsInstalled("toolbox_1") then
    atomsRemove("toolbox_1");
end
if atomsIsInstalled("toolbox_2") then
    atomsRemove("toolbox_2");
end
if atomsIsInstalled("toolbox_3") then
    atomsRemove("toolbox_3");
end
if atomsIsInstalled("toolbox_4") then
    atomsRemove("toolbox_4");
end
if atomsIsInstalled("toolbox_5") then
    atomsRemove("toolbox_5");
end
if atomsIsInstalled("toolbox_6") then
    atomsRemove("toolbox_6");
end
// REMOVING PART

// REMOVING an Automatic installed toolbox deletes the toolbox it has been
// installed by
// -----------------------------------------------------------------------------
atomsInstall(["toolbox_5" "1.0"],"user");
ref = [ "toolbox_1"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_1/1.0-1"  "A" ;
"toolbox_2"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_2/1.0-1"  "A" ;
"toolbox_4"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_4/1.0-1"  "A" ;
"toolbox_5"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_5/1.0-1"  "I" ];

if getos()=="Windows" then
    ref=strsubst(ref,"/","\");
end
removed=atomsRemove(["toolbox_2" "1.0"],"user");
[a,b]=gsort(removed(:,1),"r","i");
assert_checkequal(removed(b,:),ref);

// REMOVING an Automatic toolbox deletes all the toolboxes depending on it
// -----------------------------------------------------------------------------
atomsInstall(["toolbox_5" "1.0"],"user");
atomsInstall(["toolbox_3" "1.0"],"user");
ref = [ "toolbox_1"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_1/1.0-1"  "A" ;
"toolbox_2"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_2/1.0-1"  "A" ;
"toolbox_2"  "2.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_2/2.0-1"  "A" ;
"toolbox_3"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_3/1.0-1"  "I" ;
"toolbox_4"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_4/1.0-1"  "A" ;
"toolbox_5"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_5/1.0-1"  "I" ];
if getos()=="Windows" then
    ref=strsubst(ref,"/","\");
end
removed=atomsRemove(["toolbox_1"],"user");
[a,b]=gsort(removed(:,1),"r","i");
assert_checkequal(removed(b,:),ref);
//assert_checkequal(atomsRemove(["toolbox_1"]      ,"user"),ref);

// REMOVING an Intentionnaly installed toolbox won't delete an Automatically
// installed toolbox if it is needed by another toolbox
// -----------------------------------------------------------------------------
atomsInstall(["toolbox_5" "1.0"],"user");
atomsInstall(["toolbox_3" "1.0"],"user");
ref = [ "toolbox_2"  "2.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_2/2.0-1"  "A" ;
"toolbox_3"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_3/1.0-1"  "I" ];

if getos()=="Windows" then
    ref=strsubst(ref,"/","\");
end
removed=atomsRemove(["toolbox_3" "1.0"],"user");
[a,b]=gsort(removed(:,1),"r","i");
assert_checkequal(removed(b,:),ref);

// REMOVING a toolbox in a section won't delete it in other sections
// -----------------------------------------------------------------------------
atomsInstall(["toolbox_5" "1.0"],"user");
atomsInstall(["toolbox_5" "1.0"],"allusers");
ref = [ "toolbox_1"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_1/1.0-1"  "A" ;
"toolbox_2"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_2/1.0-1"  "A" ;
"toolbox_4"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_4/1.0-1"  "A" ;
"toolbox_5"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_5/1.0-1"  "I" ];
if getos()=="Windows" then
    ref=strsubst(ref,"/","\");
end
ref_all = [ "toolbox_1"  "1.0-1"  "allusers"  "SCI/contrib/toolbox_1/1.0-1"  "A" ;
"toolbox_2"  "1.0-1"  "allusers"  "SCI/contrib/toolbox_2/1.0-1"  "A" ;
"toolbox_4"  "1.0-1"  "allusers"  "SCI/contrib/toolbox_4/1.0-1"  "A" ;
"toolbox_5"  "1.0-1"  "allusers"  "SCI/contrib/toolbox_5/1.0-1"  "I" ];
if getos()=="Windows" then
    ref_all=strsubst(ref_all,"/","\");
end
removed=atomsRemove(["toolbox_5" "1.0"],"user");
[a,b]=gsort(removed(:,1),"r","i");
assert_checkequal(removed(b,:),ref);
removed=atomsRemove(["toolbox_5" "1.0"],"allusers");
[a,b]=gsort(removed(:,1),"r","i");
assert_checkequal(removed(b,:),ref_all);

// REMOVING a toolbox with section "all" will remove this toolbox in sections
// "alluser" AND "user"
// -----------------------------------------------------------------------------
atomsInstall(["toolbox_5" "1.0"],"user");
atomsInstall(["toolbox_5" "1.0"],"allusers");
ref = [ "toolbox_1"  "1.0-1"  "allusers"  "SCI/contrib/toolbox_1/1.0-1"  "A" ;
"toolbox_1"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_1/1.0-1"  "A" ;
"toolbox_2"  "1.0-1"  "allusers"  "SCI/contrib/toolbox_2/1.0-1"  "A" ;
"toolbox_2"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_2/1.0-1"  "A" ;
"toolbox_4"  "1.0-1"  "allusers"  "SCI/contrib/toolbox_4/1.0-1"  "A" ;
"toolbox_4"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_4/1.0-1"  "A" ;
"toolbox_5"  "1.0-1"  "allusers"  "SCI/contrib/toolbox_5/1.0-1"  "I" ;
"toolbox_5"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_5/1.0-1"  "I" ];
if getos()=="Windows" then
    ref=strsubst(ref,"/","\");
end
removed=atomsRemove(["toolbox_5" "1.0"],"all");
[a,b]=gsort(removed(:,1),"r","i");
assert_checkequal(removed(b,:),ref);

// REMOVING a toolbox from a section where it doesn't exist won't remove it from
// its existing section
// REMOVING an Intentionnaly installed toolbox deletes all the Automatically
// installed toolbox that were installed for this.
// -----------------------------------------------------------------------------
atomsInstall(["toolbox_5" "1.0"],"user");
ref_empty = [];
ref = [ "toolbox_1"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_1/1.0-1"  "A" ;
"toolbox_2"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_2/1.0-1"  "A" ;
"toolbox_4"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_4/1.0-1"  "A" ;
"toolbox_5"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_5/1.0-1"  "I" ];
if getos()=="Windows" then
    ref=strsubst(ref,"/","\");
end
assert_checkequal(atomsRemove(["toolbox_5" "1.0"],"allusers"),ref_empty);
removed=atomsRemove(["toolbox_5" "1.0"],"user");
[a,b]=gsort(removed(:,1),"r","i");
assert_checkequal(removed(b,:),ref);

// ============================================================================
// DELETING PART
rmdir(atomsPath("install","user")+"archives/","s");
mkdir(atomsPath("install","user")+"archives/");
rmdir(atomsPath("install","allusers")+"archives/","s");
mkdir(atomsPath("install","allusers")+"archives/");
// ============================================================================

// REMOVING AND DELETING a toolbox will delete its archives and archives of the
// toolboxes it depends on
// -----------------------------------------------------------------------------
atomsInstall(["toolbox_5" "1.0"],"user");
ref = [ "toolbox_1"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_1/1.0-1"  "A" ;
"toolbox_2"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_2/1.0-1"  "A" ;
"toolbox_4"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_4/1.0-1"  "A" ;
"toolbox_5"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_5/1.0-1"  "I" ];
if getos()=="Windows" then
    ref=strsubst(ref,"/","\");
end
ref_empty = [];
removed=atomsRemove(["toolbox_5" "1.0"],"user",%T);
[a,b]=gsort(removed(:,1),"r","i");
assert_checkequal(removed(b,:),ref);
if getos()=="Windows" then
    assert_checkequal(ls(atomsPath("install","user")+"archives\"),ref_empty);
else
    assert_checkequal(ls(atomsPath("install","user")+"archives/"),ref_empty);
end
rmdir(atomsPath("install","user")+"archives/","s");
mkdir(atomsPath("install","user")+"archives/");
rmdir(atomsPath("install","allusers")+"archives/","s");
mkdir(atomsPath("install","allusers")+"archives/");

// REMOVING AND DELETING a toolbox will not delete other archives than the ones
// it depends on
// -----------------------------------------------------------------------------
atomsInstall(["toolbox_5" "1.0"],"user");
atomsInstall(["toolbox_6" "1.0"],"user");
ref = [ "toolbox_2"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_2/1.0-1"  "A" ;
"toolbox_4"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_4/1.0-1"  "A" ;
"toolbox_5"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_5/1.0-1"  "I" ];
[version, opts] = getversion();

if getos()=="Windows" then
    ref=strsubst(ref,"/","\");
    if opts(2) == "x86" then
        ref_ls = [ "toolbox_1_1.0-1.bin.windows.zip";
        "toolbox_2_2.0-1.bin.windows.zip";
        "toolbox_6_1.0-1.bin.windows.zip"];
    else
        ref_ls = [ "toolbox_1_1.0-1.bin.x64.windows.zip";
        "toolbox_2_2.0-1.bin.x64.windows.zip";
        "toolbox_6_1.0-1.bin.x64.windows.zip"];
    end
elseif getos()=="Linux" then
    if opts(2) == "x86" then
        ref_ls = [ "toolbox_1_1.0-1.bin.i686.linux.tar.gz";
        "toolbox_2_2.0-1.bin.i686.linux.tar.gz";
        "toolbox_6_1.0-1.bin.i686.linux.tar.gz"];
    else
        ref_ls = [ "toolbox_1_1.0-1.bin.x86_64.linux.tar.gz";
        "toolbox_2_2.0-1.bin.x86_64.linux.tar.gz";
        "toolbox_6_1.0-1.bin.x86_64.linux.tar.gz"];
    end
elseif getos()=="Darwin" then
    ref_ls = [ "toolbox_1_1.0-1.bin.x86_64.darwin.tar.gz";
    "toolbox_2_2.0-1.bin.x86_64.darwin.tar.gz";
    "toolbox_6_1.0-1.bin.x86_64.darwin.tar.gz"];
end
ref_rem_6 = [ "toolbox_1"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_1/1.0-1"  "A" ;
"toolbox_2"  "2.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_2/2.0-1"  "A" ;
"toolbox_6"  "1.0-1"  "user"  "SCIHOME/atoms/" + arch + "toolbox_6/1.0-1"  "I" ];
if getos()=="Windows" then
    ref_rem_6=strsubst(ref_rem_6,"/","\");
end
removed=atomsRemove(["toolbox_5" "1.0"],"user",%T);
[a,b]=gsort(removed(:,1),"r","i");
assert_checkequal(removed(b,:),ref);

left=ls(atomsPath("install","user")+"archives/");
[a,b]=gsort(left(:,1),"r","i");
assert_checkequal(left(b,:),ref_ls);

// cleaning
allremoved=atomsRemove(["toolbox_6" "1.0"],"user",%T);
[a,b]=gsort(allremoved(:,1),"r","i");
assert_checkequal(allremoved(b,:),ref_rem_6);

//assert_checkequal(atomsRemove(["toolbox_6" "1.0"],"user",%T),ref_rem_6);
rmdir(atomsPath("install","user")+"archives/","s");
mkdir(atomsPath("install","user")+"archives/");
rmdir(atomsPath("install","allusers")+"archives/","s");
mkdir(atomsPath("install","allusers")+"archives/");

// no module should be installed
assert_checktrue(isempty(atomsGetInstalled()));


// Restore original values
// =============================================================================
atomsRestoreConfig(%T);

atomsRepositorySetOfl(mgetl(SCI+"/modules/atoms/tests/unit_tests/repositories.orig"));