<?xml version="1.0" encoding="UTF-8"?>
<!--
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2016 - Serge Steer - INRIA
*
* This file is hereby licensed under the terms of the GNU GPL v2.0,
* pursuant to article 5.3.4 of the CeCILL v.2.1.
* For more information, see the COPYING file which you should have received
* along with this program.
*
-->
<refentry xmlns="http://docbook.org/ns/docbook" xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:svg="http://www.w3.org/2000/svg" xmlns:mml="http://www.w3.org/1998/Math/MathML" xmlns:db="http://docbook.org/ns/docbook" xmlns:scilab="http://www.scilab.org" xml:lang="en" xml:id="zpk2tf.sci">
<refnamediv>
<refname>zpk2tf</refname>
<refpurpose>Zero pole gain to transfer function</refpurpose>
    </refnamediv>
    <refsynopsisdiv>
    <title>Syntax</title>
    <synopsis>S=zpk2tf(sys)</synopsis>
    </refsynopsisdiv>
    <refsection>
    <title>Arguments</title>
    <variablelist>

    <varlistentry>
    <term>z</term>
    <listitem>
    <para>
    a real or complex matrix, the ith column
    contains the transmission zeros relative to
    the ith output. If some zeros are complex,
    they must appear in complex conjugate pairs.
    </para>
    </listitem>
    </varlistentry>
    <varlistentry>
    <term>p</term>
    <listitem>
    <para>
    a real or complex vector, the poles of the
    system. If some poles are complex, they must
    appear in complex conjugate pairs.
    </para>
    </listitem>
    </varlistentry>
    <varlistentry>
    <term>k</term>
    <listitem>
    <para>
    a real scalar or a real vector, the ith
    element contains the gain relative to the ith
    output.
    </para>
    </listitem>
    </varlistentry>
    <varlistentry>
    <term>dom</term>
    <listitem>
    <para>
    a character string with possible values "c" or
    "d", [] or a real positive scalar, the system
    time domain (see <link
    linkend="syslin">syslin</link>).
    </para>
    </listitem>
    </varlistentry>
    <varlistentry>
    <term>S</term>
    <listitem>
    <para>
    a single input transfer function.
        </para>
        </listitem>
        </varlistentry>

        </variablelist>
        </refsection>
        <refsection>
        <title>Description</title>
        <para>
        Converts a  zero pole gain representation to  SIMO state space
        </para>

        </refsection>
        <refsection>
        <title>Examples</title>
        <programlisting role="example"><![CDATA[
        Z=[2;1];P=[0;-3-%i;-3+%i];K=2.5;
        S=zpk2tf.sci(Z,P,K,"c")
        roots(S.num)
        ]]></programlisting>
        </refsection>
        <refsection role="see also">
        <title>See Also</title>
        <simplelist type="inline">
        <member>
        <link linkend="zp2ss">zp2ss</link>
        </member>
        </simplelist>
        </refsection>
        <refsection role="history">
        <title>History</title>
        <revhistory>
        <revision>
        <revnumber>6.0</revnumber>
        <revdescription>
        <para>Function added.</para>
        </revdescription>
        </revision>
        </revhistory>
        </refsection>

        </refentry>
