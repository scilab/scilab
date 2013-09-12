// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function txt = help_skeleton(funname, path, language)

    [lhs,rhs] = argn(0);

    if rhs > 3 | rhs < 1 then
        error(39);
    end

    if type(funname) <> 10 then
        error(999,msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"),"help_skeleton",1));
    end

    if size(funname, "*") <> 1 then
        error(999,msprintf(gettext("%s: Wrong size for input argument #%d: A string expected.\n"),"help_skeleton",1));
    end

    if rhs > 1 then
        if type(path) <> 10 then
            error(55,2);
        end
    end

    previouslangage = getlanguage();

    if rhs == 3 then
        if type(language) <> 10 then
            error(55,3);
        end
        setlanguage(language);
    else
        language = getlanguage();
    end

    txt = [];
    vars = macrovar(evstr(funname));
    inputs = vars(1);
    outputs = vars(2);
    context = vars(3);
    Call = "";

    if size(outputs,"*") == 1 then
        Call = outputs + " = "
    elseif size(outputs,"*") > 1 then
        Call = "["+strcat(outputs,",")+"] = ";
    end

    Call = Call + funname;

    if size(inputs,"*") >= 1 then
        Call = Call+"("+strcat(inputs,",")+")";
    end

    args = [inputs(:);outputs(:)];

    // date management
    // --------------------------------------------------------------------------------
    w = getdate();
    xml_date = msprintf("%02d-%02d-%04d",w(6),w(2),w(1));

    //header
    // --------------------------------------------------------------------------------
    xmlns = "xmlns=""http://docbook.org/ns/docbook"""
    xlink = "xmlns:xlink=""http://www.w3.org/1999/xlink"""
    svg = "xmlns:svg=""http://www.w3.org/2000/svg"""
    mml = " xmlns:mml=""http://www.w3.org/1998/Math/MathML"""
    db = "xmlns:db=""http://docbook.org/ns/docbook"""
    scins = "xmlns:scilab=""http://www.scilab.org"""

    refs = strcat([xmlns xlink svg mml db scins]," ")

    txt=["<?xml version=""1.0"" encoding='"UTF-8""?>"
    "<!--"
    _(" * Add some comments about XML file");
    "-->"
    "<refentry " + refs + " xml:lang="""+ language + """ xml:id=""" + funname + """>"
    "  <refnamediv>"
    "    <refname>"+ funname + "</refname>"
    "    <refpurpose>" + _("Add short description here.") + "</refpurpose>"
    "  </refnamediv>"
    "  <refsynopsisdiv>"
    "    <title>" + _("Calling Sequence") + "</title>"
    "    <synopsis>" + Call + "</synopsis>"
    "  </refsynopsisdiv>"];

    //Arguments
    if size(args,"*") >0 then
        txt=[txt;
        "  <refsection>"
        "    <title>" + _("Arguments") + "</title>"
        "    <variablelist>"]
        for a=args'
            txt=[txt;
            "      <varlistentry>"
            "        <term>" + a + "</term>"
            "        <listitem>"
            "          <para>"
            _("            Add here the input/output argument description.")
            "          </para>"
            "        </listitem>"
            "      </varlistentry>"]
        end
        txt=[txt;
        "    </variablelist>"
        "  </refsection>"];
    end

    //Description
    txt=[txt;
    "  <refsection>"
    "    <title>" + _("Description") + "</title>"
    "    <para>"
    _("          Add here a paragraph of the function description.")
    _("          Other paragraph can be added")
    "    </para>"
    "    <para>With a latex expression"
    "          <latex>"
    "            \begin{eqnarray}"
    "    f(x,a,r) = \frac{1}{r^{-a}\Gamma(a)} \int_0^x t^{a-1} \exp\left(-rt\right) dt"
    "    \end{eqnarray}"
    "    </latex>"
    "    </para>"
    "  </refsection>"];

    // More information
    txt=[txt;
    "  <refsection>"
    "    <title>" + _("More information") + "</title>"
    "    <note><para>A note about foo</para></note>"
    "    <caution><para>A caution about foo</para></caution>"
    "    <warning><para>A warning about foo</para></warning>"
    "    <important><para>An important about foo</para></important>"
    "    <tip><para>A tip about foo</para></tip>"
    "  </refsection>"];

    //Example
    txt=[txt;
    "  <refsection>"
    "    <title>" + _("Examples") + "</title>"
    "    <programlisting role=""example""><![CDATA["
    _("        Add here scilab instructions and comments")
    "    ]]></programlisting>"
    "    <scilab:image><![CDATA["
    "// "+_("        Add here scilab instructions to generate a graphic")
    "    ]]></scilab:image>"
    "  </refsection>"];

    //See Also
    txt=[txt;
    "  <refsection>"
    "    <title>" + _("See Also") + "</title>"
    "    <simplelist type=""inline"">"
    "      <member>"
    "        <link linkend=""" + _("add a reference name") + """ >" + _("add a reference") + "</link>"
    "      </member>"
    "      <member>"
    "        <link linkend=""" + _("add a reference name") + """>" + _("add a reference") + "</link>"
    "      </member>"
    "    </simplelist>"
    "  </refsection>"];

    //Authors
    txt=[txt;
    "  <refsection>"
    "    <title>" + _("Authors") + "</title>"
    "    <simplelist type=""vert"">"
    "      <member>" + _("add the author name and author reference") + "</member>"
    "      <member>" + _("add another author name and his/her reference") + "</member>"
    "    </simplelist>"
    "  </refsection>"];

    //Bibliography
    txt = [txt;
    "  <refsection>"
    "     <title>" + _("Bibliography") + "</title>"
    "       <para>"
    _("         Add here the function bibliography")
    "       </para>"
    "     </refsection>"];

    // History
    txt = [txt;
    "    <refsection>"
    "       <title>" + _("History") + "</title>"
    "      <revhistory>"
    "        <revision>"
    "          <revnumber>X.Y</revnumber>"
    "          <revdescription>Function foo added</revdescription>"
    "        </revision>"
    "      </revhistory>"
    "    </refsection>"];

    //Used functions
    txt=[txt;
    "  <refsection>"
    "     <title>" + _("Used Functions") + "</title>"
    "       <para>"
    _("         Add here the Scilab, C,... used code references")
    "       </para>"
    "     </refsection>"];

    //footer
    txt=[txt;
    "</refentry>"];

    setlanguage(previouslangage);

    if rhs >= 2 then
        mputl(txt, pathconvert(path,%t,%f) + funname + ".xml");
        txt = pathconvert(path,%t,%f) + funname + ".xml";
    end

endfunction
