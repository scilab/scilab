// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Calixte DENIZET 
// 
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// <-- INTERACTIVE TEST -->

tmpdir = TMPDIR + "/testlinks";
mkdir(tmpdir);
tmpdir = tmpdir + "/help";
mkdir(tmpdir);
tmpdir = tmpdir + "/en_US";
mkdir(tmpdir);
tmpfile = tmpdir + "/mytbxhelp.xml";
mputl(["<?xml version=""1.0"" encoding=""UTF-8""?>"
       "<refentry version=""5.0-subset Scilab"""
       "xmlns=""http://docbook.org/ns/docbook"""
       "xmlns:html=""http://www.w3.org/1999/xhtml"""
       "xmlns:db=""http://docbook.org/ns/docbook"">"
       "<refnamediv xml:id=""mytbx_foo"">"
       "<refname>Mytbx_foo</refname>"
       "<refpurpose>calculates foo</refpurpose>"
       "</refnamediv>"
       "<refsection>"
       "<para>Firstly a link on <link type=""scilab"" linkend=""scilab.help/bvode"">bvode</link> in the Scilab Doc</para>"
       "<para>Secondly a link on <link type=""scilab"" linkend=""mytbx.demos/mytbxdemo"">mytbxdemo</link> </para>"
       "<para>Thirdly a link on <link type=""scilab"" linkend=""mytbx.exec/macros/fooexec.sce"">macros/fooexec.sce</link> (just an about) </para>"
       "<para>Fourthly a link on <link type=""scilab"" linkend=""module_lycee.help/enlever"">enlever</link> in module_lycee toolbox (must be loaded before)</para>"
       "<para>Fifthly a link on <link type=""scilab"" linkend=""scilab.help/invalidlink"">invalid link</link></para>"
       "</refsection>"
       "<programlisting role=""example""><![CDATA[" 
       "plot3d() // should link on plot3d"
       "]]></programlisting>"
       "</refentry>"], tmpfile);
jar = xmltojar(tmpdir, 'mytbx', 'en_US');
tmpdir = TMPDIR + "/testlinks/demos";
mkdir(tmpdir);
tmpfile = tmpdir + "/mytbxdemo.sce";
mputl("messagebox(""A demo from mytbxdemo.sce"");", tmpfile);
tmpdir = TMPDIR + "/testlinks/macros";
mkdir(tmpdir);
tmpfile = tmpdir + "/fooexec.sce";
mputl("about;", tmpfile);
add_help_chapter('mytbx', TMPDIR + "/testlinks/jar")
help('mytbx_foo')