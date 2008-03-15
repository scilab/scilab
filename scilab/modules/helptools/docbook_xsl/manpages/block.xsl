<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:d="http://docbook.org/ns/docbook"
xmlns:exsl="http://exslt.org/common"
                exclude-result-prefixes="exsl d"
                version='1.0'>

<!-- ********************************************************************
     $Id: block.xsl 6843 2007-06-20 12:21:13Z xmldoc $
     ********************************************************************

     This file is part of the XSL DocBook Stylesheet distribution.
     See ../README or http://docbook.sf.net/release/xsl/current/ for
     copyright and other information.

     ******************************************************************** -->

<!-- ==================================================================== -->

<xsl:template match="d:caution|d:important|d:note|d:tip|d:warning">
  <xsl:call-template name="nested-section-title"/>
  <xsl:apply-templates/>
  <xsl:text>&#10;</xsl:text>
</xsl:template> 

<xsl:template match="d:formalpara">
  <xsl:variable name="title.wrapper">
    <xsl:value-of select="normalize-space(d:title[1])"/>
  </xsl:variable>
  <xsl:text>.PP&#10;</xsl:text>
  <!-- * don't put linebreak after head; instead render it as a "run in" -->
  <!-- * head, that is, inline, with a period and space following it -->
  <xsl:call-template name="bold">
    <xsl:with-param name="node" select="exsl:node-set($title.wrapper)"/>
    <xsl:with-param name="context" select="."/>
  </xsl:call-template>
  <xsl:text>. </xsl:text>
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="d:formalpara/d:para">
  <xsl:call-template name="mixed-block"/>
  <xsl:text>&#10;</xsl:text>
</xsl:template>

<xsl:template match="d:para">
  <!-- * FIXME: Need to extract the ancestor::footnote, etc. checking and -->
  <!-- * move to named template so that we can call it from templates for -->
  <!-- * other block elements also -->
  <xsl:choose>
    <!-- * If a para is a descendant of a footnote, etc., then indent it -->
    <!-- * (unless it is the first child, in which case don't generate -->
    <!-- * anything at all to mark its start). -->
    <!-- * FIXME: *blurb checking should not be munged in here the way -->
    <!-- * it currently is; this probably breaks blurb indenting. -->
    <xsl:when test="ancestor::d:footnote or
                    ancestor::d:annotation or
                    ancestor::d:authorblurb or
                    ancestor::d:personblurb">
      <xsl:if test="preceding-sibling::*[not(name() ='')]">
        <xsl:text>.sp</xsl:text>
        <xsl:text>&#10;</xsl:text>
        <xsl:text>.RS 4n</xsl:text>
        <xsl:text>&#10;</xsl:text>
      </xsl:if>
    </xsl:when>
    <xsl:otherwise>
      <xsl:text>.PP</xsl:text>
      <xsl:text>&#10;</xsl:text>
    </xsl:otherwise>
  </xsl:choose>
  <xsl:call-template name="mixed-block"/>
    <xsl:if test="ancestor::d:footnote or
                  ancestor::d:annotation or
                  ancestor::d:authorblurb or
                  ancestor::d:personblurb">
      <xsl:if test="preceding-sibling::*[not(name() ='')]">
        <xsl:text>&#10;</xsl:text>
        <xsl:text>.RE</xsl:text>
        <xsl:text>&#10;</xsl:text>
      </xsl:if>
    </xsl:if>
  <xsl:text>&#10;</xsl:text>
</xsl:template>

<xsl:template match="d:simpara">
  <xsl:variable name="content">
    <xsl:apply-templates/>
  </xsl:variable>
  <xsl:value-of select="normalize-space($content)"/>
  <xsl:text>&#10;</xsl:text>
  <xsl:if test="not(ancestor::d:authorblurb) and
                not(ancestor::d:personblurb)">
    <xsl:text>.sp&#10;</xsl:text>
  </xsl:if>
</xsl:template>

<!-- ==================================================================== -->

<!-- * Yes, address, synopsis, and funcsynopsisinfo are verbatim environments. -->
<xsl:template match="d:literallayout|d:programlisting|d:screen|
                     d:address|d:synopsis|d:funcsynopsisinfo">
  <xsl:param name="indent">
    <!-- * Only indent this verbatim if $man.indent.verbatims is -->
    <!-- * non-zero and it is not a child of a *synopsis element -->
    <xsl:if test="not($man.indent.verbatims = 0) and
                  not(substring(local-name(..),
                  string-length(local-name(..))-7) = 'synopsis')">
      <xsl:text>Yes</xsl:text>
    </xsl:if>
  </xsl:param>

  <xsl:choose>
    <!-- * Check to see if this verbatim item is within a parent element that -->
    <!-- * allows mixed content. -->
    <!-- * -->
    <!-- * If it is within a mixed-content parent, then a line space is -->
    <!-- * already added before it by the mixed-block template, so we don't -->
    <!-- * need to add one here. -->
    <!-- * -->
    <!-- * If it is not within a mixed-content parent, then we need to add a -->
    <!-- * line space before it. -->
    <xsl:when test="parent::d:caption|parent::d:entry|parent::d:para|
                    parent::d:td|parent::d:th" /> <!-- do nothing -->
    <xsl:otherwise>
      <xsl:text>&#10;</xsl:text>
      <xsl:text>.sp&#10;</xsl:text>
    </xsl:otherwise>
  </xsl:choose>
  <xsl:if test="$indent = 'Yes'">
    <!-- * start indented section -->
    <xsl:text>.RS</xsl:text> 
    <xsl:if test="not($man.indent.width = '')">
      <xsl:text> </xsl:text>
      <xsl:value-of select="$man.indent.width"/>
    </xsl:if>
    <xsl:text>&#10;</xsl:text>
  </xsl:if>
  <xsl:choose>
    <xsl:when test="self::d:funcsynopsisinfo">
      <!-- * All Funcsynopsisinfo content is by default rendered in bold, -->
      <!-- * because the man(7) man page says this: -->
      <!-- * -->
      <!-- *   For functions, the arguments are always specified using -->
      <!-- *   italics, even in the SYNOPSIS section, where the rest of -->
      <!-- *   the function is specified in bold -->
      <!-- * -->
      <!-- * Look through the contents of the man/man2 and man3 directories -->
      <!-- * on your system, and you'll see that most existing pages do follow -->
      <!-- * this "bold everything in function synopsis" rule. -->
      <!-- * -->
      <!-- * Users who don't want the bold output can choose to adjust the -->
      <!-- * man.font.funcsynopsisinfo parameter on their own. So even if you -->
      <!-- * don't personally like the way it looks, please don't change the -->
      <!-- * default to be non-bold - because it's a convention that's -->
      <!-- * followed is the vast majority of existing man pages that document -->
      <!-- * functions, and we need to follow it by default, like it or no. -->
      <xsl:text>.ft </xsl:text>
      <xsl:value-of select="$man.font.funcsynopsisinfo"/>
      <xsl:text>&#10;</xsl:text>
      <xsl:text>.nf&#10;</xsl:text>
      <xsl:apply-templates/>
      <xsl:text>&#10;</xsl:text>
      <xsl:text>.fi&#10;</xsl:text>
      <xsl:text>.ft&#10;</xsl:text>
    </xsl:when>
    <xsl:otherwise>
      <!-- * Other verbatims do not need to get bolded -->
      <xsl:text>.nf&#10;</xsl:text>
      <xsl:apply-templates/>
      <xsl:text>&#10;</xsl:text>
      <xsl:text>.fi&#10;</xsl:text>
    </xsl:otherwise>
  </xsl:choose>
  <xsl:if test="$indent = 'Yes'">
    <!-- * end indented section -->
    <xsl:text>.RE&#10;</xsl:text> 
  </xsl:if>
  <!-- * if first following sibling node of this verbatim -->
  <!-- * environment is a text node, output a line of space before it -->
  <xsl:if test="following-sibling::node()[1][name(.) = '']">
    <xsl:text>.sp&#10;</xsl:text>
  </xsl:if>
</xsl:template>

<!-- ==================================================================== -->

<xsl:template match="d:table|d:informaltable">
  <xsl:apply-templates select="." mode="to.tbl">
    <!--* we call the to.tbl mode with the "source" param so that we can -->
    <!--* preserve the context information and pass it down to the -->
    <!--* named templates that do the actual table processing -->
    <xsl:with-param name="source" select="ancestor::d:refentry/d:refnamediv[1]/d:refname[1]"/>
  </xsl:apply-templates>
</xsl:template>

<!-- ==================================================================== -->

<xsl:template match="d:informalexample">
  <xsl:apply-templates/>
</xsl:template>

<!-- ==================================================================== -->

<xsl:template match="d:figure|d:example">
  <xsl:variable name="param.placement"
                select="substring-after(normalize-space($formal.title.placement),
                        concat(local-name(.), ' '))"/>

  <xsl:variable name="placement">
    <xsl:choose>
      <xsl:when test="contains($param.placement, ' ')">
        <xsl:value-of select="substring-before($param.placement, ' ')"/>
      </xsl:when>
      <xsl:when test="$param.placement = ''">before</xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$param.placement"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:variable>

  <xsl:text>.PP&#10;</xsl:text>
  <xsl:call-template name="formal.object">
    <xsl:with-param name="placement" select="$placement"/>
  </xsl:call-template>
  <xsl:text>&#10;</xsl:text>
</xsl:template>

<!-- ==================================================================== -->

<xsl:template match="d:mediaobject">
  <xsl:text>.sp</xsl:text>
  <xsl:text>&#10;</xsl:text>
  <xsl:text>.RS</xsl:text> 
  <xsl:if test="not($list-indent = '')">
    <xsl:text> </xsl:text>
    <xsl:value-of select="$list-indent"/>
  </xsl:if>
  <xsl:text>&#10;</xsl:text>
  <xsl:apply-templates/>
  <xsl:text>&#10;</xsl:text>
  <xsl:text>.RE&#10;</xsl:text>
</xsl:template>

<xsl:template match="d:imageobject">
  <xsl:text>[IMAGE]</xsl:text>
  <xsl:apply-templates/>
  <xsl:text>&#10;</xsl:text>
</xsl:template>

<xsl:template match="d:textobject[parent::d:inlinemediaobject]">
  <xsl:text>[</xsl:text>
  <xsl:value-of select="."/>
  <xsl:text>]</xsl:text>
</xsl:template>

<xsl:template match="d:textobject">
  <xsl:apply-templates/>
</xsl:template>

<!-- ==================================================================== -->

<xsl:template name="formal.object">
  <xsl:param name="placement" select="'before'"/>
  <xsl:param name="class" select="local-name(.)"/>

  <xsl:choose>
    <xsl:when test="$placement = 'before'">
      <xsl:call-template name="formal.object.heading"/>
      <xsl:apply-templates/>
    </xsl:when>
    <xsl:otherwise>
      <xsl:apply-templates/>
      <xsl:call-template name="formal.object.heading"/>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template name="formal.object.heading">
  <xsl:param name="object" select="."/>
  <xsl:param name="title">
    <xsl:apply-templates select="$object" mode="object.title.markup.textonly"/>
  </xsl:param>
  <xsl:call-template name="bold">
    <xsl:with-param name="node" select="exsl:node-set($title)"/>
    <xsl:with-param name="context" select="."/>
  </xsl:call-template>

  <xsl:text>&#10;</xsl:text>
</xsl:template>

<!-- ==================================================================== -->

<!-- * suppress abstract -->
<xsl:template match="d:abstract"/>

</xsl:stylesheet>
