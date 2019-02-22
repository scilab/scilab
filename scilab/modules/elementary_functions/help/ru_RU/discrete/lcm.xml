<?xml version="1.0" encoding="UTF-8"?>
<!--
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA
 * Copyright (C) 2018 - Samuel GOUGEON
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 -->
<!--  module: polynomials  -->
<refentry xmlns="http://docbook.org/ns/docbook" xmlns:xlink="http://www.w3.org/1999/xlink"
         xmlns:svg="http://www.w3.org/2000/svg" xmlns:mml="http://www.w3.org/1998/Math/MathML"
         xmlns:db="http://docbook.org/ns/docbook" xmlns:scilab="http://www.scilab.org"
         xml:lang="ru" xml:id="lcm">
   <refnamediv>
       <refname>lcm</refname>
       <refpurpose>наименьшее общее кратное (положительное) целых чисел или полиномов</refpurpose>
   </refnamediv>
   <refsynopsisdiv>
       <title>Синтаксис</title>
       <synopsis>
          pp = lcm(p)
         [pp, fact] = lcm(p)
       </synopsis>
   </refsynopsisdiv>
   <refsection>
       <title>Аргументы</title>
       <variablelist>
           <varlistentry>
               <term>p</term>
               <listitem>
                   <para>
                       матрица полиномов (тип 2), или десятичных чисел или кодированных целых
                       чисел (тип 1 или 8).
                   </para>
               </listitem>
           </varlistentry>
           <varlistentry>
               <term>pp</term>
               <listitem>
                   <para>
                       полином или десятичное целое число: положительное нименьшее общее кратное у
                       составляющих <varname>p</varname>.
                   </para>
               </listitem>
           </varlistentry>
           <varlistentry>
               <term>fact</term>
               <listitem>
                   <para>
                       матрица полиномов или десятичных целых чисел (тип 1) с размером, как у
                       <varname>p</varname>, такая, что <literal>fact(i)= pp./p(i)</literal>.
                   </para>
               </listitem>
           </varlistentry>
       </variablelist>
   </refsection>
   <refsection>
       <title>Описание</title>
       <para>
           <literal>pp=lcm(p)</literal>  вычисляет НОК <varname>pp</varname> составляющих <varname>p</varname>.
       </para>
       <para>
         Если <literal>p</literal> являются полиномами, то <literal>pp</literal> является полиномом и
         <varname>fact</varname> также является матрицей полиномов.
       </para>
       <para>
         Если <literal>p</literal> является массивом целых числел, то
           <itemizedlist>
               <listitem>
                   если они являются кодированными целыми числами, то тогда они преобразуются
                   в десятичные целые числи перед обработкой.
               </listitem>
               <listitem>
                   Любые входные целые числа типа int64 или uint64 по модулю больше 2^53
                   будут усечены и <literal>lcm()</literal> вернёт неверный результат.
               </listitem>
               <listitem>
                   Если некоторые из них отрицательные, то возвращаемое значение <literal>pp</literal>
                   (их НОК) будет всегда положительным.
               </listitem>
           </itemizedlist>
       </para>
       <para>
           Намиеньшее общее кратное массива <literal>p</literal> вещественных чисел может быть
           получено конвертированием его в полином перед вызовом <literal>lcm</literal>,
           через <literal>p = inv_coeff(p, 0)</literal>.
       </para>
   </refsection>
   <refsection>
       <title>Примеры</title>
       <para>С полиномами:</para>
       <programlisting role="example"><![CDATA[
s = %s;
p = [s , s*(s+1) , s^2-1]
[pp, fact] = lcm(p)
p .* fact == pp
]]></programlisting>
   <screen><![CDATA[
--> p = [s , s*(s+1) , s^2-1]
 p  =
           2       2
   s   s +s   -1 +s

--> [pp, fact] = lcm(p)
 fact  =
       2
  -1 +s   -1 +s   s

 pp  =
       3
  -s +s

--> p .* fact == pp
 ans  =
  T T T
]]></screen>
   <para>С кодированными целыми числами:</para>
   <programlisting role="example"><![CDATA[
// Простые числа: 2  3  5  7  11  13  17  19  23  29  31  37  41  43  47
V = int16([2*3 3*7 ; 7*5  3*5])
[pp, fact] = lcm(V)
 ]]></programlisting>
    <screen><![CDATA[
--> V = int16([2*3 3*7 ; 7*5  3*5])
 V  =
   6  21
  35  15

--> [pp, fact] = lcm(V)
 pp  =
   210.

 fact  =
   35.   10.
   6.    14.
]]></screen>
    <para>С десятичнми целыми числами:</para>
    <programlisting role="example"><![CDATA[
V = [2*3 3*7 ; 7*5  3*5]
[pp, fact] = lcm(V)
 ]]></programlisting>
    <para>С большими целыми числами:</para>
    <programlisting role="example"><![CDATA[
V = [3*2^51 , 3*5]
[pp, fact] = lcm(V)    // OK
 ]]></programlisting>
    <screen><![CDATA[
--> V = [3*2^51 , 3*5]
 V  =
   6.755D+15   15.

--> [pp, fact] = lcm(V)
 fact  =
   5.   2.252D+15

 pp  =
   3.378D+16
]]></screen>
    <para>
        Когда числовое кодирование переполняет разрядную сетку, происходит усечение
        и результаты возвращаются ошибочные:
    </para>
    <programlisting role="example"><![CDATA[
V = [3*2^52 , 3*5]
[pp, fact] = lcm(V)
 ]]></programlisting>
    <screen><![CDATA[
--> V = [3*2^52 , 3*5]
 V  =
   1.351D+16   15.

--> [pp, fact] = lcm(V)
 fact  =
   15.   1.351D+16

 pp  =
   2.027D+17
]]></screen>
   </refsection>
   <refsection role="see also">
       <title>Смотрите также</title>
       <simplelist type="inline">
           <member>
               <link linkend="gcd">gcd</link>
           </member>
           <member>
               <link linkend="bezout">bezout</link>
           </member>
           <member>
               <link linkend="factor">factor</link>
           </member>
       </simplelist>
   </refsection>
   <refsection role="history">
       <title>История</title>
       <revhistory>
           <revision>
               <revnumber>6.0.1</revnumber>
               <revdescription>
                  Для входных целых чисел, возможно отрицательных, возвращаемое МОК теперь
                  всегда положительно.
               </revdescription>
           </revision>
           <revision>
               <revnumber>6.0.2</revnumber>
               <revdescription>
                   <itemizedlist>
                       <listitem>
                           Теперь поддерживаются входные кодированные целые числа типа int64 и uint64.
                       </listitem>
                       <listitem>
                           Теперь <varname>p</varname> может быть матрицей любой формы, необязательно строка.
                       </listitem>
                       <listitem>
                           Когда <varname>p</varname> является матрицей кодированных или десятичных целых
                           чисел, то вычисления теперь выполняются в числах с плавающей запятой вместо
                           формата int32.
                           <varname>pp</varname> и <varname>fact</varname> теперь десятичные целые числа
                           (вместо целых чисел типа int32).
                       </listitem>
                   </itemizedlist>
               </revdescription>
           </revision>
       </revhistory>
   </refsection>
</refentry>
