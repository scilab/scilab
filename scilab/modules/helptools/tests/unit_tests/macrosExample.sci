function [helptxt,demotxt]=macrosExample(funname,helpdir,demodir)
    // Generate help files and demo files from the head comments section of a .sci source file.
    //
    // Syntax
    //  help_from_sci() // generate an empty function template
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
    //  help_from_sci is a revised version of the help_skeleton function.
    //  Its objective is to generate .xml help files based on the head comments section
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
    //    <listitem><literal>Syntax</literal> - one example pr. line.</listitem>
    //    <listitem><literal>Parameters</literal> - separate parameter name and
    //    description by a ":". Keep the description of each parameter on the same line.</listitem>
    //    <listitem><literal>Description</literal> - formatting of the text can be done
    //    using XML commands. Compare the output of head_comments('help_from_sci') with help('help_from_sci')
    //    to get some hints.
    //    Adding an empty comment line in the Description section is interpreted as the
    //    start of a new paragraph.</listitem>
    //    <listitem><literal>See also</literal> - list one function name pr line.</listitem>
    //    <listitem><literal>Authors</literal> - write one author on each line following
    //    the Authors headline. Use ";" to separate the authors name
    //    from any add additional information.</listitem>
    //    <listitem><literal>Bibliography</literal> - write one reference pr line
    //    following the References headline.</listitem>
    //  </itemizedlist>
    //
    // Examples
    //  help_from_sci()   // Open an empty source code template in the scinotes editor.
    //  // Save this template as test_fun.sci in the current directory before running
    //  // the next example commands.
    //
    //  help_from_sci('test_fun')        // return the xml skeleton as a text string
    //
    //  help_from_sci('test_fun','.')    // create the xml help file in the current directory.
    //
    //  // create both a xml help file and a demo file in the current directory.
    //  help_from_sci('test_fun','.','.')
    //
    //  // From a toolbox root directory a typical syntax would be:
    //  // help_from_sci('macros','help\en_US','demos')
    //  // This command would process all .sci files in the macros directory
    //  // and use the head comments section to update all .xml help files in the
    //  // help\en_US directory an rebuild the .dem.sce files in the demos\ directory.
    //
    // See also
    //  help
    //  help_skeleton
    //  head_comments
    // Authors
    //  T. Pettersen ; top@tpett.com

    a=1+2
    helptxt="retour"
endfunction
