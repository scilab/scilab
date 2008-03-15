<?xml version='1.0'?>
<xsl:stylesheet exclude-result-prefixes="d"
                 xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:d="http://docbook.org/ns/docbook"
xmlns:exsl="http://exslt.org/common"
                version='1.0'>

<!-- ********************************************************************
     $Id: inline.xsl 6843 2007-06-20 12:21:13Z xmldoc $
     ********************************************************************

     This file is part of the XSL DocBook Stylesheet distribution.
     See ../README or http://docbook.sf.net/release/xsl/current/ for
     copyright and other information.

     ******************************************************************** -->

<!-- ==================================================================== -->

<xsl:template match="d:replaceable|d:varname">
  <xsl:if test="$man.hyphenate.computer.inlines = 0">
    <xsl:call-template name="suppress.hyphenation"/>
  </xsl:if>
  <xsl:call-template name="italic">
    <xsl:with-param name="node" select="."/>
    <xsl:with-param name="context" select="."/>
  </xsl:call-template>
</xsl:template>

<xsl:template match="d:option|d:userinput|d:envar|d:errorcode|d:constant|d:markup">
  <xsl:if test="$man.hyphenate.computer.inlines = 0">
    <xsl:call-template name="suppress.hyphenation"/>
  </xsl:if>
  <xsl:call-template name="bold">
    <xsl:with-param name="node" select="."/>
    <xsl:with-param name="context" select="."/>
  </xsl:call-template>
</xsl:template>

<xsl:template match="d:classname">
  <xsl:if test="$man.hyphenate.computer.inlines = 0">
    <xsl:call-template name="suppress.hyphenation"/>
  </xsl:if>
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="d:command">
  <xsl:if test="$man.hyphenate.computer.inlines = 0">
    <xsl:call-template name="suppress.hyphenation"/>
  </xsl:if>
  <xsl:call-template name="bold">
    <xsl:with-param name="node" select="."/>
    <xsl:with-param name="context" select="."/>
  </xsl:call-template>
</xsl:template>

<xsl:template match="d:type[not(ancestor::d:cmdsynopsis) and
                     not(ancestor::d:funcsynopsis)]">
  <xsl:if test="$man.hyphenate.computer.inlines = 0">
    <xsl:call-template name="suppress.hyphenation"/>
  </xsl:if>
  <xsl:call-template name="bold">
    <xsl:with-param name="node" select="."/>
    <xsl:with-param name="context" select="."/>
  </xsl:call-template>
</xsl:template>

<xsl:template match="d:function[not(ancestor::d:cmdsynopsis) and
                     not(ancestor::d:funcsynopsis)]">
  <xsl:if test="$man.hyphenate.computer.inlines = 0">
    <xsl:call-template name="suppress.hyphenation"/>
  </xsl:if>
  <xsl:call-template name="bold">
    <xsl:with-param name="node" select="."/>
    <xsl:with-param name="context" select="."/>
  </xsl:call-template>
</xsl:template>

<xsl:template match="d:parameter[not(ancestor::d:cmdsynopsis) and
                     not(ancestor::d:funcsynopsis)]">
  <xsl:if test="$man.hyphenate.computer.inlines = 0">
    <xsl:call-template name="suppress.hyphenation"/>
  </xsl:if>
  <xsl:call-template name="italic">
    <xsl:with-param name="node" select="."/>
    <xsl:with-param name="context" select="."/>
  </xsl:call-template>
</xsl:template>

<xsl:template match="d:filename">
  <!-- * add hyphenation suppression in Filename output only if -->
  <!-- * break.after.slash is also non-zero -->
  <xsl:if test="$man.hyphenate.filenames = 0 and
                $man.break.after.slash = 0">
    <xsl:call-template name="suppress.hyphenation"/>
  </xsl:if>
  <xsl:call-template name="italic">
    <xsl:with-param name="node" select="."/>
    <xsl:with-param name="context" select="."/>
  </xsl:call-template>
</xsl:template>

<xsl:template match="d:emphasis">
  <xsl:choose>
    <xsl:when test="
      @role = 'bold' or
      @role = 'strong' or
      @remap = 'B'">
      <xsl:call-template name="bold">
        <xsl:with-param name="node" select="."/>
        <xsl:with-param name="context" select="."/>
      </xsl:call-template>
    </xsl:when>
    <xsl:otherwise>
      <xsl:call-template name="italic">
        <xsl:with-param name="node" select="."/>
        <xsl:with-param name="context" select="."/>
      </xsl:call-template>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template match="d:optional">
  <xsl:value-of select="$arg.choice.opt.open.str"/>
  <xsl:apply-templates/>
  <xsl:value-of select="$arg.choice.opt.close.str"/>
</xsl:template>

<xsl:template name="do-citerefentry">
  <xsl:param name="refentrytitle" select="''"/>
  <xsl:param name="manvolnum" select="''"/>
  <xsl:variable name="title">
    <xsl:value-of select="$refentrytitle"/>
  </xsl:variable>
  <xsl:call-template name="bold">
    <xsl:with-param name="node" select="exsl:node-set($title)"/>
    <xsl:with-param name="context" select="."/>
  </xsl:call-template>
  <xsl:text>(</xsl:text>
  <xsl:value-of select="$manvolnum"/>
  <xsl:text>)</xsl:text>
</xsl:template>

<xsl:template match="d:citerefentry">
  <xsl:call-template name="do-citerefentry">
    <xsl:with-param name="refentrytitle" select="d:refentrytitle"/>
    <xsl:with-param name="manvolnum" select="d:manvolnum"/>
  </xsl:call-template>
</xsl:template>

<xsl:template match="d:trademark|d:productname">
  <xsl:apply-templates/>
  <xsl:choose>
    <!-- * Just use true Unicode chars for copyright, trademark, etc., -->
    <!-- * symbols (by default, we later automatically translate them -->
    <!-- * with the apply-string-subst-map template, or with the -->
    <!-- * default character map, if man.charmap.enabled is true). -->
    <xsl:when test="@class = 'copyright'">
      <xsl:text>&#x00a9;</xsl:text>
    </xsl:when>
    <xsl:when test="@class = 'registered'">
      <xsl:text>&#x00ae;</xsl:text>
    </xsl:when>
    <xsl:when test="@class = 'service'">
      <xsl:text>&#x2120;</xsl:text>
    </xsl:when>
    <xsl:when test="@class = 'trade'">
      <xsl:text>&#x2122;</xsl:text>
    </xsl:when>
    <!-- * for Trademark element, render a trademark symbol by default -->
    <!-- * even if no "class" value is specified -->
    <xsl:when test="self::d:trademark" >
      <xsl:text>&#x2122;</xsl:text>
    </xsl:when>
    <xsl:otherwise>
      <!-- * otherwise we have a Productname with no value for the -->
      <!-- * "class" attribute, so don't render any symbol by default -->
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<!-- * span seems to sneak through into output sometimes, possibly due -->
<!-- * to failed Olink processing; so we need to catch it -->
<xsl:template match="d:span">
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="d:inlinemediaobject">
  <xsl:apply-templates/>
</xsl:template>

</xsl:stylesheet>
