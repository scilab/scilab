<?xml version='1.0'?>
<xsl:stylesheet exclude-result-prefixes="d"
                 xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:d="http://docbook.org/ns/docbook"
xmlns:fo="http://www.w3.org/1999/XSL/Format"
                version='1.0'>

<!-- ********************************************************************
     $Id: toc.xsl 6910 2007-06-28 23:23:30Z xmldoc $
     ********************************************************************

     This file is part of the XSL DocBook Stylesheet distribution.
     See ../README or http://docbook.sf.net/release/xsl/current/ for
     copyright and other information.

     ******************************************************************** -->

<!-- ==================================================================== -->

<!-- FIXME: in the contexts where <toc> can occur, I think it's always
     the case that a page-sequence is required. Is that true? -->

<xsl:template match="d:toc">
  <xsl:variable name="master-reference">
    <xsl:call-template name="select.pagemaster"/>
  </xsl:variable>

  <xsl:choose>
    <xsl:when test="*">
      <xsl:if test="$process.source.toc != 0">
        <!-- if the toc isn't empty, process it -->
        <fo:page-sequence hyphenate="{$hyphenate}"
                          master-reference="{$master-reference}">
          <xsl:attribute name="language">
            <xsl:call-template name="l10n.language"/>
          </xsl:attribute>
          <xsl:attribute name="format">
            <xsl:call-template name="page.number.format">
              <xsl:with-param name="element" select="'toc'"/>
              <xsl:with-param name="master-reference" 
                              select="$master-reference"/>
            </xsl:call-template>
          </xsl:attribute>

          <xsl:attribute name="initial-page-number">
            <xsl:call-template name="initial.page.number">
              <xsl:with-param name="element" select="'toc'"/>
              <xsl:with-param name="master-reference" 
                              select="$master-reference"/>
            </xsl:call-template>
          </xsl:attribute>

          <xsl:attribute name="force-page-count">
            <xsl:call-template name="force.page.count">
              <xsl:with-param name="master-reference" 
	                      select="$master-reference"/>
            </xsl:call-template>
          </xsl:attribute>

          <xsl:attribute name="hyphenation-character">
            <xsl:call-template name="gentext">
              <xsl:with-param name="key" select="'hyphenation-character'"/>
            </xsl:call-template>
          </xsl:attribute>
          <xsl:attribute name="hyphenation-push-character-count">
            <xsl:call-template name="gentext">
              <xsl:with-param name="key" select="'hyphenation-push-character-count'"/>
            </xsl:call-template>
          </xsl:attribute>
          <xsl:attribute name="hyphenation-remain-character-count">
            <xsl:call-template name="gentext">
              <xsl:with-param name="key" select="'hyphenation-remain-character-count'"/>
            </xsl:call-template>
          </xsl:attribute>

          <xsl:apply-templates select="." mode="running.head.mode">
            <xsl:with-param name="master-reference" select="$master-reference"/>
          </xsl:apply-templates>
          <xsl:apply-templates select="." mode="running.foot.mode">
            <xsl:with-param name="master-reference" select="$master-reference"/>
          </xsl:apply-templates>

          <fo:flow flow-name="xsl-region-body">
            <xsl:call-template name="set.flow.properties">
              <xsl:with-param name="element" select="local-name(.)"/>
              <xsl:with-param name="master-reference" 
                              select="$master-reference"/>
            </xsl:call-template>

            <fo:block xsl:use-attribute-sets="toc.margin.properties">
              <xsl:call-template name="table.of.contents.titlepage"/>
              <xsl:apply-templates/>
            </fo:block>
          </fo:flow>
        </fo:page-sequence>
      </xsl:if>
    </xsl:when>
    <xsl:otherwise>
      <xsl:if test="$process.empty.source.toc != 0">
        <fo:page-sequence hyphenate="{$hyphenate}"
                          master-reference="{$master-reference}">
          <xsl:attribute name="language">
            <xsl:call-template name="l10n.language"/>
          </xsl:attribute>
          <xsl:attribute name="format">
            <xsl:call-template name="page.number.format">
              <xsl:with-param name="element" select="'toc'"/>
              <xsl:with-param name="master-reference" 
                              select="$master-reference"/>
            </xsl:call-template>
          </xsl:attribute>

          <xsl:attribute name="initial-page-number">
            <xsl:call-template name="initial.page.number">
              <xsl:with-param name="element" select="'toc'"/>
              <xsl:with-param name="master-reference" 
                              select="$master-reference"/>
            </xsl:call-template>
          </xsl:attribute>

          <xsl:attribute name="hyphenation-character">
            <xsl:call-template name="gentext">
              <xsl:with-param name="key" select="'hyphenation-character'"/>
            </xsl:call-template>
          </xsl:attribute>
          <xsl:attribute name="hyphenation-push-character-count">
            <xsl:call-template name="gentext">
              <xsl:with-param name="key" select="'hyphenation-push-character-count'"/>
            </xsl:call-template>
          </xsl:attribute>
          <xsl:attribute name="hyphenation-remain-character-count">
            <xsl:call-template name="gentext">
              <xsl:with-param name="key" select="'hyphenation-remain-character-count'"/>
            </xsl:call-template>
          </xsl:attribute>

          <xsl:apply-templates select="." mode="running.head.mode">
            <xsl:with-param name="master-reference" select="$master-reference"/>
          </xsl:apply-templates>
          <xsl:apply-templates select="." mode="running.foot.mode">
            <xsl:with-param name="master-reference" select="$master-reference"/>
          </xsl:apply-templates>

          <fo:flow flow-name="xsl-region-body">
            <xsl:choose>
              <xsl:when test="parent::d:section
                              or parent::d:sect1
                              or parent::d:sect2
                              or parent::d:sect3
                              or parent::d:sect4
                              or parent::d:sect5">
                <xsl:apply-templates select="parent::*"
                                     mode="toc.for.section"/>
              </xsl:when>
              <xsl:when test="parent::d:article">
                <xsl:apply-templates select="parent::*"
                                     mode="toc.for.component"/>
              </xsl:when>
              <xsl:when test="parent::d:book
                              or parent::d:part">
                <xsl:apply-templates select="parent::*"
                                     mode="toc.for.division"/>
              </xsl:when>
              <xsl:when test="parent::d:set">
                <xsl:apply-templates select="parent::*"
                                     mode="toc.for.set"/>
              </xsl:when>
              <!-- there aren't any other contexts that allow toc -->
              <xsl:otherwise>
                <xsl:message>
                  <xsl:text>I don't know how to make a TOC in this context!</xsl:text>
                </xsl:message>
              </xsl:otherwise>
            </xsl:choose>
          </fo:flow>
        </fo:page-sequence>
      </xsl:if>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template match="d:tocpart|d:tocchap
                     |d:toclevel1|d:toclevel2|d:toclevel3|d:toclevel4|d:toclevel5">
  <xsl:apply-templates select="d:tocentry"/>
  <xsl:if test="d:tocchap|d:toclevel1|d:toclevel2|d:toclevel3|d:toclevel4|d:toclevel5">
    <fo:block start-indent="{count(ancestor::*)*2}pc">
      <xsl:apply-templates select="d:tocchap|d:toclevel1|d:toclevel2|d:toclevel3|d:toclevel4|d:toclevel5"/>
    </fo:block>
  </xsl:if>
</xsl:template>

<xsl:template match="d:tocentry|d:tocfront|d:tocback">
  <fo:block text-align-last="justify"
            end-indent="2pc"
            last-line-end-indent="-2pc">
    <fo:inline keep-with-next.within-line="always">
      <xsl:choose>
        <xsl:when test="@linkend">
          <fo:basic-link internal-destination="{@linkend}">
            <xsl:apply-templates/>
          </fo:basic-link>
        </xsl:when>
        <xsl:otherwise>
          <xsl:apply-templates/>
        </xsl:otherwise>
      </xsl:choose>
    </fo:inline>

    <xsl:choose>
      <xsl:when test="@linkend">
        <fo:inline keep-together.within-line="always">
          <xsl:text> </xsl:text>
          <fo:leader leader-pattern="dots"
                     keep-with-next.within-line="always"/>
          <xsl:text> </xsl:text>
          <fo:basic-link internal-destination="{@linkend}">
            <xsl:choose>
              <xsl:when test="@pagenum">
                <xsl:value-of select="@pagenum"/>
              </xsl:when>
              <xsl:otherwise>
                <fo:page-number-citation ref-id="{@linkend}"/>
              </xsl:otherwise>
            </xsl:choose>
          </fo:basic-link>
        </fo:inline>
      </xsl:when>
      <xsl:when test="@pagenum">
        <fo:inline keep-together.within-line="always">
          <xsl:text> </xsl:text>
          <fo:leader leader-pattern="dots"
                     keep-with-next.within-line="always"/>
          <xsl:text> </xsl:text>
          <xsl:value-of select="@pagenum"/>
        </fo:inline>
      </xsl:when>
      <xsl:otherwise>
        <!-- just the leaders, what else can I do? -->
        <fo:inline keep-together.within-line="always">
          <xsl:text> </xsl:text>
          <fo:leader leader-pattern="space"
                     keep-with-next.within-line="always"/>
        </fo:inline>
      </xsl:otherwise>
    </xsl:choose>
  </fo:block>
</xsl:template>

<!-- ==================================================================== -->

<xsl:template match="*" mode="toc.for.section">
<!--
  <xsl:call-template name="section.toc"/>
-->
</xsl:template>

<xsl:template match="*" mode="toc.for.component">
  <xsl:call-template name="component.toc"/>
</xsl:template>

<xsl:template match="*" mode="toc.for.section">
<!--
  <xsl:call-template name="section.toc"/>
-->
</xsl:template>

<xsl:template match="*" mode="toc.for.division">
  <xsl:call-template name="division.toc"/>
</xsl:template>

<xsl:template match="*" mode="toc.for.set">
<!--
  <xsl:call-template name="set.toc"/>
-->
</xsl:template>

<!-- ==================================================================== -->

<xsl:template match="d:lot|d:lotentry">
</xsl:template>

</xsl:stylesheet>
