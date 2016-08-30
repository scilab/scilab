function scimihm(%_xmlfile)
    // Scicos initialization GUI  (IHM d'initialisation)
    // Masoud Najafi, Nov. 2007

    global icpr
    icpr=list();

    sciGUI_init()
    if execstr("%_filename="+%_xmlfile,"errcatch")~=0 then error(44), end

    try
        %_winId=TCL_GetVar("IHMLoc")
    catch
        %_winId="nothing";
    end

    if (%_winId <> "nothing") then
        running=TCL_EvalStr("winfo exists [sciGUIName "+%_winId+"]")
        if running=="1" then
            TCL_EvalStr("sciGUIDestroy "+%_winId);
        end
    end

    TCL_EvalStr("set IHMLoc [MIHM_Init -1]");
    %_winId=TCL_GetVar("IHMLoc");

    //if evstr(%_winId)>1 then
    //  messagebox("winId="+%_winId+" is greater than 1, check your program");
    //  return;
    //end


    TCL_SetVar("sciGUITable(win,"+%_winId+",data,Scixmlfile)",%_filename);
    TCL_SetVar("sciGUITable(win,"+%_winId+",data,SCI_TMPDIR)",string(TMPDIR));
    TCL_SetVar("sciGUITable(win,"+%_winId+",data,IERROR)","?");

    //----- launching the GUI
    TCL_EvalStr("MIHM_Create "+%_winId);

    //----- simulating a click on the OpenXML Button
    TCL_EvalStr("button_OpenXML "+%_winId);

endfunction

