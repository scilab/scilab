<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:fo="http://www.w3.org/1999/XSL/Format"
                xmlns:d="http://docbook.org/ns/docbook"
                xmlns:db="http://docbook.org/ns/docbook"
                xmlns:ah="http://www.antennahouse.com/"
                version='1.0'>

<xsl:import href="../../docbook_xsl/fo/docbook.xsl"/>

<!-- =========================================================================
     Customize the look of the output
========================================================================== -->

<xsl:attribute-set name="xref.properties">
  <xsl:attribute name="color">navy</xsl:attribute>
  <xsl:attribute name="text-decoration">underline</xsl:attribute>
</xsl:attribute-set>

<!-- =========================================================================
     Fix bugs in the rendering of the HTML tables
========================================================================== -->

<!--

Copyright Antenna House, Inc. (http://www.antennahouse.com) 2001, 2002.

Since this stylesheet is originally developed by Antenna House to be used with XSL Formatter, it may not be compatible with another XSL-FO processors.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, provided that the above copyright notice(s) and this permission notice appear in all copies of the Software and that both the above copyright notice(s) and this permission notice appear in supporting documentation.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

-->

  <!--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
       Table
  =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-->

  <xsl:attribute-set name="ah:table">
    <xsl:attribute name="border-collapse">separate</xsl:attribute>
    <xsl:attribute name="border">1px</xsl:attribute>
  </xsl:attribute-set>

  <xsl:attribute-set name="ah:table-column">
  </xsl:attribute-set>

  <xsl:attribute-set name="ah:inside-table">
    <!-- prevent unwanted inheritance -->
    <xsl:attribute name="start-indent">0pt</xsl:attribute>
    <xsl:attribute name="end-indent">0pt</xsl:attribute>
    <xsl:attribute name="text-indent">0pt</xsl:attribute>
    <xsl:attribute name="last-line-end-indent">0pt</xsl:attribute>
    <xsl:attribute name="text-align">start</xsl:attribute>
    <xsl:attribute name="text-align-last">relative</xsl:attribute>
  </xsl:attribute-set>

  <xsl:attribute-set name="ah:thead" 
                     use-attribute-sets="ah:inside-table">
  </xsl:attribute-set>

  <xsl:attribute-set name="ah:tfoot" 
                     use-attribute-sets="ah:inside-table">
  </xsl:attribute-set>

  <xsl:attribute-set name="ah:tbody" 
                     use-attribute-sets="ah:inside-table">
  </xsl:attribute-set>

  <xsl:attribute-set name="ah:tr">
  </xsl:attribute-set>

  <xsl:attribute-set name="ah:th">
    <xsl:attribute name="font-weight">bold</xsl:attribute>
    <xsl:attribute name="text-align">center</xsl:attribute>
    <xsl:attribute name="border">1px</xsl:attribute>
    <xsl:attribute name="padding">0.25em</xsl:attribute>
  </xsl:attribute-set>

  <xsl:attribute-set name="ah:td">
    <xsl:attribute name="border">1px</xsl:attribute>
    <xsl:attribute name="padding">0.25em</xsl:attribute>
  </xsl:attribute-set>

  <!--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
       Table
  =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-->

  <xsl:template match="d:table|d:informaltable" mode="htmlTable">
    <fo:table xsl:use-attribute-sets="ah:table">
      <xsl:call-template name="ah:process-table"/>
    </fo:table>
  </xsl:template>

  <xsl:template name="ah:process-table">
    <xsl:if test="@width">
      <xsl:attribute name="inline-progression-dimension">
        <xsl:choose>
          <xsl:when test="contains(@width, '%')">
            <xsl:value-of select="@width"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="@width"/>px</xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
    </xsl:if>
    <xsl:if test="@border or @frame">
      <xsl:choose>
        <xsl:when test="@border &gt; 0">
          <xsl:attribute name="border">
            <xsl:value-of select="@border"/>px</xsl:attribute>
        </xsl:when>
      </xsl:choose>
      <xsl:choose>
        <xsl:when test="@border = '0' or @frame = 'void'">
          <xsl:attribute name="border-style">hidden</xsl:attribute>
        </xsl:when>
        <xsl:when test="@frame = 'above'">
          <xsl:attribute name="border-style">outset hidden hidden hidden</xsl:attribute>
        </xsl:when>
        <xsl:when test="@frame = 'below'">
          <xsl:attribute name="border-style">hidden hidden outset hidden</xsl:attribute>
        </xsl:when>
        <xsl:when test="@frame = 'hsides'">
          <xsl:attribute name="border-style">outset hidden</xsl:attribute>
        </xsl:when>
        <xsl:when test="@frame = 'vsides'">
          <xsl:attribute name="border-style">hidden outset</xsl:attribute>
        </xsl:when>
        <xsl:when test="@frame = 'lhs'">
          <xsl:attribute name="border-style">hidden hidden hidden outset</xsl:attribute>
        </xsl:when>
        <xsl:when test="@frame = 'rhs'">
          <xsl:attribute name="border-style">hidden outset hidden hidden</xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="border-style">outset</xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:if>
    <xsl:if test="@cellspacing">
      <xsl:attribute name="border-spacing">
        <xsl:value-of select="@cellspacing"/>px</xsl:attribute>
      <xsl:attribute name="border-collapse">separate</xsl:attribute>
    </xsl:if>
    <xsl:if test="@rules and (@rules = 'groups' or
                      @rules = 'rows' or
                      @rules = 'cols' or
                      @rules = 'all' and (not(@border or @frame) or
                          @border = '0' or @frame and
                          not(@frame = 'box' or @frame = 'border')))">
      <xsl:attribute name="border-collapse">collapse</xsl:attribute>
      <xsl:if test="not(@border or @frame)">
        <xsl:attribute name="border-style">hidden</xsl:attribute>
      </xsl:if>
    </xsl:if>
    <xsl:call-template name="ah:process-common-attributes"/>
    <xsl:apply-templates select="db:col | db:colgroup" mode="htmlTable"/>
    <xsl:apply-templates select="db:thead" mode="htmlTable"/>
    <xsl:apply-templates select="db:tfoot" mode="htmlTable"/>
    <xsl:choose>
      <xsl:when test="db:tbody">
        <xsl:apply-templates select="db:tbody" mode="htmlTable"/>
      </xsl:when>
      <xsl:otherwise>
        <fo:table-body xsl:use-attribute-sets="ah:tbody">
          <xsl:apply-templates select="db:tr" mode="htmlTable"/>
        </fo:table-body>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="db:thead" mode="htmlTable">
    <fo:table-header xsl:use-attribute-sets="ah:thead">
      <xsl:call-template name="ah:process-table-rowgroup"/>
    </fo:table-header>
  </xsl:template>

  <xsl:template match="db:tfoot" mode="htmlTable">
    <fo:table-footer xsl:use-attribute-sets="ah:tfoot">
      <xsl:call-template name="ah:process-table-rowgroup"/>
    </fo:table-footer>
  </xsl:template>

  <xsl:template match="db:tbody" mode="htmlTable">
    <fo:table-body xsl:use-attribute-sets="ah:tbody">
      <xsl:call-template name="ah:process-table-rowgroup"/>
    </fo:table-body>
  </xsl:template>

  <xsl:template name="ah:process-table-rowgroup">
    <xsl:if test="ancestor::db:table[1]/@rules = 'groups' or
                  ancestor::db:informaltable[1]/@rules = 'groups'">
      <xsl:attribute name="border">1px solid</xsl:attribute>
    </xsl:if>
    <xsl:call-template name="ah:process-common-attributes-and-children"/>
  </xsl:template>

  <xsl:template match="db:colgroup" mode="htmlTable">
    <fo:table-column xsl:use-attribute-sets="ah:table-column">
      <xsl:call-template name="ah:process-table-column"/>
    </fo:table-column>
  </xsl:template>

  <xsl:template match="db:colgroup[db:col]" mode="htmlTable">
    <xsl:apply-templates mode="htmlTable"/>
  </xsl:template>

  <xsl:template match="db:col" mode="htmlTable">
    <fo:table-column xsl:use-attribute-sets="ah:table-column">
      <xsl:call-template name="ah:process-table-column"/>
    </fo:table-column>
  </xsl:template>

  <xsl:template name="ah:process-table-column">
    <xsl:if test="parent::db:colgroup">
      <xsl:call-template name="ah:process-col-width">
        <xsl:with-param name="width" select="../@width"/>
      </xsl:call-template>
      <xsl:call-template name="ah:process-cell-align">
        <xsl:with-param name="align" select="../@align"/>
      </xsl:call-template>
      <xsl:call-template name="ah:process-cell-valign">
        <xsl:with-param name="valign" select="../@valign"/>
      </xsl:call-template>
    </xsl:if>
    <xsl:if test="@span">
      <xsl:attribute name="number-columns-repeated">
        <xsl:value-of select="@span"/>
      </xsl:attribute>
    </xsl:if>
    <xsl:call-template name="ah:process-col-width">
      <xsl:with-param name="width" select="@width"/>
      <!-- it may override parent colgroup's width -->
    </xsl:call-template>
    <xsl:if test="ancestor::db:table[1]/@rules = 'cols' or
                  ancestor::db:informaltable[1]/@rules = 'cols'">
      <xsl:attribute name="border">1px solid</xsl:attribute>
    </xsl:if>
    <xsl:call-template name="ah:process-common-attributes"/>
    <!-- this processes also align and valign -->
  </xsl:template>

  <xsl:template match="db:tr" mode="htmlTable">
    <fo:table-row xsl:use-attribute-sets="ah:tr">
      <xsl:call-template name="ah:process-table-row"/>
    </fo:table-row>
  </xsl:template>

  <xsl:template match="db:tr[(parent::db:table or parent::db:informaltable) and
                             db:th and not(db:td)]" 
                mode="htmlTable">
    <fo:table-row xsl:use-attribute-sets="ah:tr" keep-with-next="always">
      <xsl:call-template name="ah:process-table-row"/>
    </fo:table-row>
  </xsl:template>

  <xsl:template name="ah:process-table-row">
    <xsl:if test="ancestor::db:table[1]/@rules = 'rows' or
                  ancestor::db:informaltable[1]/@rules = 'rows'">
      <xsl:attribute name="border">1px solid</xsl:attribute>
    </xsl:if>
    <xsl:call-template name="ah:process-common-attributes-and-children"/>
  </xsl:template>

  <xsl:template match="db:th" mode="htmlTable">
    <fo:table-cell xsl:use-attribute-sets="ah:th">
      <xsl:call-template name="ah:process-table-cell"/>
    </fo:table-cell>
  </xsl:template>

  <xsl:template match="db:td" mode="htmlTable">
    <fo:table-cell xsl:use-attribute-sets="ah:td">
      <xsl:call-template name="ah:process-table-cell"/>
    </fo:table-cell>
  </xsl:template>

  <xsl:template name="ah:process-table-cell">
    <xsl:if test="@colspan">
      <xsl:attribute name="number-columns-spanned">
        <xsl:value-of select="@colspan"/>
      </xsl:attribute>
    </xsl:if>
    <xsl:if test="@rowspan">
      <xsl:attribute name="number-rows-spanned">
        <xsl:value-of select="@rowspan"/>
      </xsl:attribute>
    </xsl:if>
    <xsl:for-each select="ancestor::db:table[1]|ancestor::db:informaltable[1]">
      <xsl:if test="(@border or @rules) and (@rules = 'all' or
                    not(@rules) and not(@border = '0'))">
        <xsl:attribute name="border-style">inset</xsl:attribute>
      </xsl:if>
      <xsl:if test="@cellpadding">
        <xsl:attribute name="padding">
          <xsl:choose>
            <xsl:when test="contains(@cellpadding, '%')">
              <xsl:value-of select="@cellpadding"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="@cellpadding"/>px</xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
      </xsl:if>
    </xsl:for-each>
    <xsl:if test="not(@align or 
                      ../@align or
                      ../parent::*[self::db:thead or self::db:tfoot or self::db:tbody]/@align) and
                      (ancestor::db:table[1]/*[self::db:col or self::db:colgroup]/descendant-or-self::*/@align or 
                      ancestor::db:informaltable[1]/*[self::db:col or self::db:colgroup]/descendant-or-self::*/@align)">
      <xsl:attribute name="text-align">from-table-column()</xsl:attribute>
    </xsl:if>
    <xsl:if test="not(@valign or 
                      ../@valign or
                      ../parent::*[self::db:thead or self::db:tfoot or self::db:tbody]/@valign) and
                      (ancestor::db:table[1]/*[self::db:col or self::db:colgroup]/descendant-or-self::*/@valign or
                       ancestor::db:informaltable[1]/*[self::db:col or self::db:colgroup]/descendant-or-self::*/@valign)">
      <xsl:attribute name="display-align">from-table-column()</xsl:attribute>
      <xsl:attribute name="relative-align">from-table-column()</xsl:attribute>
    </xsl:if>
    <xsl:call-template name="ah:process-common-attributes"/>
    <fo:block>
      <xsl:apply-templates/>
    </fo:block>
  </xsl:template>

  <xsl:template name="ah:process-col-width">
    <xsl:param name="width"/>
    <xsl:if test="$width and $width != '0*'">
      <xsl:attribute name="column-width">
        <xsl:choose>
          <xsl:when test="contains($width, '*')">
            <xsl:text>proportional-column-width(</xsl:text>
            <xsl:value-of select="substring-before($width, '*')"/>
            <xsl:text>)</xsl:text>
          </xsl:when>
          <xsl:when test="contains($width, '%')">
            <xsl:value-of select="$width"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$width"/>px</xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
    </xsl:if>
  </xsl:template>

  <xsl:template name="ah:process-cell-align">
    <xsl:param name="align"/>
    <xsl:if test="$align">
      <xsl:attribute name="text-align">
        <xsl:choose>
          <xsl:when test="$align = 'char'">
            <xsl:choose>
              <xsl:when test="$align/../@char">
                <xsl:value-of select="$align/../@char"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="'.'"/>
                <!-- todo: it should depend on xml:lang ... -->
              </xsl:otherwise>
            </xsl:choose>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$align"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
    </xsl:if>
  </xsl:template>

  <xsl:template name="ah:process-cell-valign">
    <xsl:param name="valign"/>
    <xsl:if test="$valign">
      <xsl:attribute name="display-align">
        <xsl:choose>
          <xsl:when test="$valign = 'middle'">center</xsl:when>
          <xsl:when test="$valign = 'bottom'">after</xsl:when>
          <xsl:when test="$valign = 'baseline'">auto</xsl:when>
          <xsl:otherwise>before</xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
      <xsl:if test="$valign = 'baseline'">
        <xsl:attribute name="relative-align">baseline</xsl:attribute>
      </xsl:if>
    </xsl:if>
  </xsl:template>

  <!--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
   process common attributes and children
  =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-->

  <xsl:template name="ah:process-common-attributes-and-children">
    <xsl:call-template name="ah:process-common-attributes"/>
    <xsl:apply-templates mode="htmlTable" />
  </xsl:template>

  <xsl:template name="ah:process-common-attributes">
    <xsl:if test="@align">
      <xsl:call-template name="ah:process-cell-align">
        <xsl:with-param name="align" select="@align"/>
      </xsl:call-template>
    </xsl:if>

    <xsl:if test="@valign">
      <xsl:call-template name="ah:process-cell-valign">
        <xsl:with-param name="valign" select="@valign"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

</xsl:stylesheet>
