// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - T. Pettersen
// Copyright (C) 2010 - DIGITEO - Allan CORNET
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//==============================================================================

function [helptxt,demotxt]=help_from_sci(funname,helpdir,demodir)
    // Generate help files and demo files from the head comments section of a .sci source file.
    //
    // Syntax
    //  help_from_sci() // generate an empty function template
    //  helptxt = help_from_sci() // generate an empty function template
    //  help_from_sci(funname,helpdir) // generate helpdir/funname.xml from funname.sci.
    //  help_from_sci(dirname,helpdir) // process dirname/*.sci and create helpdir/*.xml help files.
    //  help_from_sci(dirname,helpdir,demodir) // as above but also creating demodir/*.dem.sce demo files.
    //  [helptxt,demotxt]=help_from_sci(funname) // return funname.xml and funname.dem.sce code as two text matrices.
    // Parameters
    //  funname: the name of a single .sci source file to be processed.
    //  dirname: directory name where all .sci files will be processed.
    //  helpdir: optional path where the .xml help file will be created.
    //  demodir: optional path where .dem.sce demo files will be created based on code from the Examples section.
    //  helptxt: returns the XML help code if helpdir is empty, or the path to the .xml file.
    //  demotxt: returns the demo code if demodir is empty, or the path to the .dem.sce file.
    //
    // Description
    //  The help_from_sci function generates .xml help files based on the head comments section
    //  of .sci source files. Optionally .dem.sce demo files can be generated based on
    //  code from the Examples section in the head comments section of .sci files.
    //
    //  In order for help_from_sci to format the .xml file properly the
    //  head comments section should comply with some simple formatting rules.
    //
    //  The first comment line following the function definition should contain a short description
    //  of the function.
    //
    //  The remaining comments are formatted according to the following (optional) headlines:
    //  "Syntax", "Parameters", "Description", "Examples", "See also", "Used functions",
    //  "Authors" and "Bibliography".
    //
    //  The following guidelines should be used when writing the source code comments:
    //  <itemizedlist>
    //    <listitem><para><literal>Syntax</literal> - one example pr. line.</para></listitem>
    //    <listitem><para><literal>Parameters</literal> - separate parameter name and
    //    description by a ":". Keep the description of each parameter on the same line.</para></listitem>
    //    <listitem><para><literal>Description</literal> - formatting of the text can be done
    //    using XML commands. Compare the output of head_comments("help_from_sci") with help("help_from_sci")
    //    to get some hints.
    //    Adding an empty comment line in the Description section is interpreted as the
    //    start of a new paragraph.</para></listitem>
    //    <listitem><para><literal>See also</literal> - list one function name pr line.</para></listitem>
    //    <listitem><para><literal>Authors</literal> - write one author on each line following
    //    the Authors headline. Use ";" to separate the authors name
    //    from any add additional information.</para></listitem>
    //    <listitem><para><literal>Bibliography</literal> - write one reference pr line
    //    following the References headline.</para></listitem>
    //  </itemizedlist>
    //
    // Examples
    //  help_from_sci()   // Open an empty source code template in the scinotes editor.
    //  // Save this template as test_fun.sci in the current directory before running
    //  // the next example commands.
    //
    //  help_from_sci("test_fun")        // return the xml skeleton as a text string
    //
    //  help_from_sci("test_fun",".")    // create the xml help file in the current directory.
    //
    //  // create both a xml help file and a demo file in the current directory.
    //  help_from_sci("test_fun",".",".")
    //
    //  // From a toolbox root directory a typical syntax would be:
    //  // help_from_sci("macros","help\en_US","demos")
    //  // This command would process all .sci files in the macros directory
    //  // and use the head comments section to update all .xml help files in the
    //  // help\en_US directory an rebuild the .dem.sce files in the demos\ directory.
    //
    // See also
    //  help
    //  help_skeleton
    //  head_comments
    //
    // Authors
    // Copyright (C) 2008-2009 - T. Pettersen
    // Copyright (C) 2010 - DIGITEO - Allan CORNET
    // Copyright (C) 2011 - DIGITEO - Michael Baudin

    if argn(2) == 0 then
        helptxt = [..
        "function [z] = function_template(x,y)"
        "// Short description on the first line following the function header."
        "//"
        "// Syntax"
        "//   [z] = template_function(x,y) // calling examples, one pr. line"
        "//"
        "// Parameters"
        "// x: the x parameter   // single line description of each parameter."
        "// y: the y parameter   // parameter name and description must be "
        "// z: the z parameter   // separated by "":""."
        "//"
        "// Description"
        "// Here is a description of the function."
        "// Add an empty comment line to format the text into separate paragraphs."
        "//"
        "// XML format commands may also be used directly in the text, "
        "// as in the following examples."
        "//"
        "// This is an example of a itemized list:"
        "// <itemizedlist>"
        "// <listitem><para>An itemized list is shown here</para></listitem>"
        "// </itemizedlist>"
        "// The help text for help_from_sci was generated from the head comments section of help_from_sci.sci"
        "// Compare the output from head_comments(""help_from_sci"") and help(""help_from_sci"")"
        "// to see more examples on how to write the head comments section."
        "//"
        "// This is an example of the programlisting tag:"
        "//   <programlisting>z = test_fun(x, y)</programlisting>"
        "//"
        "// This is an example of a latex equation:"
        "//   <latex>"
        "//   \begin{eqnarray}"
        "//   m = |M| \beta^{1-p},"
        "//   \end{eqnarray}"
        "//   </latex>"
        "//"
        "// Examples"
        "// [z] = test_fun(1, 2) // examples of use"
        "//"
        "// // An empty comment line in the Examples section will add a halt() statement"
        "// // in the demo file test_fun.dem.sce generated by help_from_sci."
        "//"
        "// See also"
        "//  help_from_sci"
        "//  help_skeleton"
        "//"
        "// Authors"
        "//  Author name ; should be listed one pr line. Use "";"" to separate names from additional information "
        "//"
        "// Bibliography"
        "//   Literature references one pr. line"
        ""
        "// start of coding after on empty line to mark the end of the head_comments section"
        "z = sin(x).*cos(x + y);"
        "endfunction"
        ];

        mputl(helptxt, TMPDIR + filesep() + "function_template.sci");
        if (isdef("editor") | (funptr("editor") <> 0)) then
            editor(TMPDIR + filesep() + "function_template.sci");
        end
        [helptxt, demotxt] = help_from_sci("TMPDIR/function_template");
        return;
    end

    if argn(2) < 3 then demodir = []; end
    if argn(2) < 2 then helpdir = []; end

    if ~isempty(demodir) & ~isdir(demodir) then
        error(sprintf(gettext("%s: Wrong value for input argument #%d: A valid existing directory is expected.\n"), "help_from_sci", 3));
    end

    if isdir(funname) then
        printf(gettext("%s: Reading from directory %s\n"), "help_from_sci", funname);
        files = findfiles(funname, "*.sci");   // read *.sci files.
        for i = 1:size(files, "r")
            [tmp, out] = fileparts(files(i));
            if isempty(helpdir) then
                help_from_sci(funname + filesep() + files(i), ".", demodir);
                printf(gettext("%s: Processing of file: %s to %s\n"), "help_from_sci", funname + "/" + out, out);
            else
                help_from_sci(funname + filesep() + files(i), helpdir, demodir);
                printf(gettext("%s: Processing of file: %s to %s\n"), "help_from_sci", funname + "/" + out, helpdir + "/" + out);
            end
            if ~isempty(demodir) then
                printf(gettext("%s: Processing of file: %s\n"), "help_from_sci" , demodir + "/" + out + ".dem.sce");
            else
                printf("\n");
            end
        end
        printf(gettext("%s: processed %i files.\n"), "help_from_sci", i);
        helptxt = "";
        return;
    end

    out = tokens(pathconvert(funname), filesep());
    out = out($);
    out = tokens(out,".");
    out = out(1);      // remove .sci (...wont work for fil.name.sci)

    demotxt = ["mode(1)"
    "//"
    "// Demo of "+out+".sci"
    "//"
    ""];

    verno = ver();
    verno = verno(1,2);
    helptxt = [
    "<?xml version=""1.0"" encoding=""UTF-8""?>"
    ""
    "<!--"
    " *"
    " * This help file was generated from "+out+".sci using help_from_sci()."
    " *"
    " -->"
    ""
    "<refentry version=""5.0-subset Scilab"" xml:id="""+out+""" xml:lang=""en"""
    "          xmlns=""http://docbook.org/ns/docbook"""
    "          xmlns:xlink=""http://www.w3.org/1999/xlink"""
    "          xmlns:svg=""http://www.w3.org/2000/svg"""
    "          xmlns:ns3=""http://www.w3.org/1999/xhtml"""
    "          xmlns:mml=""http://www.w3.org/1998/Math/MathML"""
    "          xmlns:scilab=""http://www.scilab.org"""
    "          xmlns:db=""http://docbook.org/ns/docbook"">"
    ""
    ];

    if isempty(strindex(funname, ".sci")) then funname = funname + ".sci"; end;
    if isempty(fileinfo(funname)) then
        error(sprintf(gettext("%s: The file %s does not exist.\n"),"help_from_sci",funname));
    end;
    f = mopen(funname, "rt");
    if isempty(f) then
        error(sprintf(gettext("%s: Cannot open file %s.\n"), "help_from_sci", funname + ".sci"));
    end
    line = " ";
    doc = [];

    while isempty(strindex(line, "function ")) & ~meof(f), line = mgetl(f, 1); end

    line = mgetl(f,1);
    line = replaceTabBySpace(line);
    short_descr = stripblanks(strsubst(line, "//", ""), %T);
    helptxt = [helptxt;
    "  <refnamediv>"
    "    <refname>"+out+"</refname>"
    "    <refpurpose>"+short_descr+"</refpurpose>"
    "  </refnamediv>"
    ];

    cmds = ["SYNTAX", "PARAMETERS", "DESCRIPTION", "EXAMPLES", "SEE ALSO", ..
    "AUTHORS", "BIBLIOGRAPHY", "USED FUNCTIONS"];

    doing = "search";
    i = strindex(line, "//");
    line = mgetl(f, 1);
    line = replaceTabBySpace(line);
    // Continue until empty line or end of file or a scilab command line (Bug#5487)
    while (~isempty(stripblanks(line)) & ~meof(f)) & ~isempty(regexp(stripblanks(line),"/^\/\/*/"))
        if stripblanks(line) == "//" then
            if doing == "Description" then
                in = "new_descr_param";
            else
                in = "";
            end
        else

            in = strsplit(line, i(1) + 1);
            in = stripblanks(in(2));
            code = in;  // store original line for the demos.
            if (doing ~= "Examples") then // Replacing characters like <, > or & should not be done in the Examples
                in = strsubst(in, "&", "&amp;"); // remove elements that make xml crash.
                in = strsubst(in, "< ", "&lt; ");
                if strindex(in ,"<") then
                    if ~helpfromsci_isxmlstr(in) then
                        in = strsubst(in, "<", "&lt;");
                    end;
                end
                in = strsubst(in, " >", " &gt;");
                if strindex(in, ">") then
                    if ~helpfromsci_isxmlstr(in) then
                        in = strsubst(in, ">", "&gt;");
                    end;
                end
            end
        end

        IN = convstr(in, "u");
        if find(cmds == IN) then
            [add_txt, doing] = change_activity(doing, in);
            helptxt = [helptxt; add_txt];
        else
            if doing == "Syntax" then
                helptxt = [helptxt;"   " + in];
            elseif doing == "Parameters" then
                i = strindex(in, ":");
                if ~isempty(i) then
                    if length(in) > i(1) then
                        in = strsplit(in,i(1));
                        par_name = in(1);
                        par_descr = in(2);
                    else
                        par_name = in;
                        par_descr = " ";
                    end
                    helptxt = [helptxt; "   <varlistentry><term>" + par_name + "</term>"];
                    helptxt = [helptxt;"      <listitem><para>" + par_descr + "</para></listitem></varlistentry>"];
                end
            elseif doing == "Description" & in == "new_descr_param" then
                helptxt = [helptxt;"   </para>";"   <para>"];
            elseif doing == "Description" then
                helptxt = [helptxt; in];
            elseif doing == "Examples" & convstr(in, "u") ~= "EXAMPLES" then
                if isempty(stripblanks(in)) then
                    demotxt = [demotxt; "halt()   // Press return to continue"; " "];
                else
                    demotxt = [demotxt; code];
                end
                helptxt = [helptxt; in];
            elseif doing == "See also" & convstr(in, "u") ~= "SEE ALSO" & ~isempty(stripblanks(in)) then
                str = stripblanks(in);
                i = strindex(str, " ");
                if i <> [] then
                    str = stripblanks(strsplit(str, i(1)));
                else
                    str = [str str];
                end
                helptxt = [helptxt; "   <member><link linkend=""" + str(1) + """>" + str(2) + "</link></member>"];
            elseif doing == "Authors" & convstr(in, "u") ~= "AUTHORS" & ~isempty(stripblanks(in)) then
                [name, ref] = chop(in, ";");
                if isempty(ref) then
                    helptxt = [helptxt; "   <member>" + name + "</member>"];
                else
                    helptxt = [helptxt; "   <member>" + name + "</member><listitem><para>" + ref + "</para></listitem>"];
                end
            elseif doing == "Bibliography" & convstr(in, "u") ~= "BIBLIOGRAPHY" & ~isempty(stripblanks(in)) then
                helptxt = [helptxt;"   <para>" + in + "</para>"];
            elseif doing == "Used functions" & convstr(in, "u") ~= "USED FUNCTIONS" & ~isempty(stripblanks(in)) then
                helptxt = [helptxt;"   <para>" + in + "</para>"];
            end
        end
        line = mgetl(f,1);
        line = replaceTabBySpace(line);
        i = strindex(line, "//");
    end

    helptxt = [helptxt; change_activity(doing, "FINISHED")];
    mclose(f);

    if ~isempty(helpdir) then
        fnme = pathconvert(helpdir, %t, %f) + out + ".xml";
        answ = 1;
        if isfile(fnme) then  // file exists...
            answ = messagebox(fnme + " exists!", "Warning - help_from_sci", "warning", ["Create anyway" "Skip file"], "modal");
        end
        if answ == 1 then
            mputl(helptxt, fnme);
            helptxt = fnme;
        else
            printf(gettext("%s: File skipped %s."), "help_from_sci", out + ".xml");
            helptxt = "";
        end
    end

    demotxt = [demotxt; "//========= E N D === O F === D E M O =========//"];
    if ~isempty(demodir) then
        fnme = demodir + filesep() + out + ".dem.sce";
        answ = 1;
        if isfile(fnme) then
            answ = messagebox(fnme + " exists!", "Warning - help_from_sci", "warning", ["Create anyway" "Skip file"], "modal");
        end
        if answ == 1 then
            mputl(demotxt, fnme);
            demotxt = fnme;
        else
            printf(gettext("%s: File skipped %s."), "help_from_sci", out + ".demo.sce");
            demotxt = "";
        end
    end
endfunction
//==============================================================================
function tf = helpfromsci_isxmlstr(str)
    // Returns %t if the current string is a xml line
    if ( ~isempty(regexp(str, "/\<*[a-z]\>/")) ) then
        tf=%t
    elseif ( ~isempty(regexp(str, "/\<(.*)\/\>/")) ) then
        tf=%t
    else
        tf=%f
    end
endfunction
//==============================================================================
function [head, tail] = chop(str, tok)
    i = regexp(str, "/" + tok + "/", "o");
    if isempty(i) then
        head = str;
        tail = [];
    else
        head = part(str, 1:i - 1);
        tail = part(str, i + 1:length(str));
    end
endfunction
//==============================================================================
function strOut = replaceTabBySpace(strIn)
    strOut = strsubst(strIn, ascii(9), part(" ",1:4));
endfunction
//==============================================================================
function [txt, doing] = change_activity(currently_doing, start_doing)
    doing = start_doing;
    select convstr(currently_doing,"u")
    case "SYNTAX" then
        txt = ["   </synopsis>"; "</refsynopsisdiv>"];
    case "PARAMETERS" then
        txt = ["   </variablelist>"; "</refsection>"];
    case "DESCRIPTION" then
        txt = ["</para>"; "</refsection>"];
    case "EXAMPLES" then
        txt = ["   ]]></programlisting>"; "</refsection>"];
    case "SEE ALSO" then
        txt = ["   </simplelist>"; "</refsection>"];
    case "AUTHORS" then
        txt = ["   </simplelist>"; "</refsection>"];
    case "BIBLIOGRAPHY" then
        txt = ["</refsection>"];
    case "USED FUNCTIONS" then
        txt = ["</refsection>"];
    else
        txt = "";
    end

    select convstr(start_doing, "u"),
    case "SYNTAX"
        txt = [txt; ""; "<refsynopsisdiv>"; "   <title>Syntax</title>"; "   <synopsis>"];
    case "PARAMETERS"
        txt = [txt; ""; "<refsection>"; "   <title>Parameters</title>"; "   <variablelist>"];
    case "DESCRIPTION"
        txt = [txt; ""; "<refsection>"; "   <title>Description</title>"; "   <para>"];
    case "EXAMPLES"
        txt = [txt; ""; "<refsection>"; "   <title>Examples</title>"; "   <programlisting role=""example""><![CDATA["];
    case "SEE ALSO"
        txt = [txt; ""; "<refsection>"; "   <title>See also</title>"; "   <simplelist type=""inline"">"];
    case "AUTHORS"
        txt = [txt; ""; "<refsection>"; "   <title>Authors</title>"; "   <simplelist type=""vert"">"];
    case "BIBLIOGRAPHY"
        txt = [txt; ""; "<refsection>"; "   <title>Bibliography</title>"];
    case "USED FUNCTIONS"
        txt = [txt; ""; "<refsection>"; "   <title>Used functions</title>"];
    case "FINISHED"
        txt = [txt; "</refentry>"];
    end
endfunction
//==============================================================================
