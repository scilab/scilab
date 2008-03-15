<?xml version="1.0"?>
<xsl:stylesheet exclude-result-prefixes="d"
                 xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:d="http://docbook.org/ns/docbook"
version="1.0">

<!-- ********************************************************************
     $Id: htmltbl.xsl 6910 2007-06-28 23:23:30Z xmldoc $
     ********************************************************************

     This file is part of the XSL DocBook Stylesheet distribution.
     See ../README or http://docbook.sf.net/release/xsl/current/ for
     copyright and other information.

     ******************************************************************** -->

<!-- ==================================================================== -->

<xsl:template match="d:colgroup" mode="htmlTable">
  <xsl:copy>
    <xsl:copy-of select="@*"/>
    <xsl:apply-templates mode="htmlTable"/>
  </xsl:copy>
</xsl:template>

<xsl:template match="d:col" mode="htmlTable">
  <xsl:copy>
    <xsl:copy-of select="@*"/>
  </xsl:copy>
</xsl:template>

<xsl:template match="d:caption" mode="htmlTable">
  <xsl:copy>
    <xsl:copy-of select="@*"/>

    <xsl:apply-templates select=".." mode="object.title.markup">
      <xsl:with-param name="allow-anchors" select="1"/>
    </xsl:apply-templates>

  </xsl:copy>
</xsl:template>

<xsl:template match="d:thead|d:tbody|d:tgroup|d:tr" mode="htmlTable">
  <xsl:copy>
    <xsl:copy-of select="@*"/>
    <xsl:apply-templates mode="htmlTable"/>
  </xsl:copy>
</xsl:template>

<xsl:template match="d:th|d:td" mode="htmlTable">
  <xsl:copy>
    <xsl:copy-of select="@*"/>
    <xsl:apply-templates/> <!-- *not* mode=htmlTable -->
  </xsl:copy>
</xsl:template>

</xsl:stylesheet>
