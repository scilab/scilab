<?xml version='1.0'?>
<xsl:stylesheet exclude-result-prefixes="d"
                 xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:d="http://docbook.org/ns/docbook"
xmlns:fo="http://www.w3.org/1999/XSL/Format"
                version='1.0'>

<xsl:template match="*" mode="fop1.outline">
  <xsl:apply-templates select="*" mode="fop1.outline"/>
</xsl:template>

<xsl:template match="d:set|d:book|d:part|d:reference|
                     d:preface|d:chapter|d:appendix|d:article
                     |d:glossary|d:bibliography|d:index|d:setindex
                     |d:refentry
                     |d:sect1|d:sect2|d:sect3|d:sect4|d:sect5|d:section"
              mode="fop1.outline">

  <xsl:variable name="id">
    <xsl:call-template name="object.id"/>
  </xsl:variable>
  <xsl:variable name="bookmark-label">
    <xsl:apply-templates select="." mode="object.title.markup"/>
  </xsl:variable>

  <!-- Put the root element bookmark at the same level as its children -->
  <!-- If the object is a set or book, generate a bookmark for the toc -->

  <xsl:choose>
    <xsl:when test="parent::*">
      <fo:bookmark internal-destination="{$id}">
        <fo:bookmark-title>
          <xsl:value-of select="normalize-space(translate($bookmark-label, $a-dia, $a-asc))"/>
        </fo:bookmark-title>
        <xsl:apply-templates select="*" mode="fop1.outline"/>
      </fo:bookmark>
    </xsl:when>
    <xsl:otherwise>
      <fo:bookmark internal-destination="{$id}">
        <fo:bookmark-title>
          <xsl:value-of select="normalize-space(translate($bookmark-label, $a-dia, $a-asc))"/>
        </fo:bookmark-title>
      </fo:bookmark>

      <xsl:variable name="toc.params">
        <xsl:call-template name="find.path.params">
          <xsl:with-param name="table" select="normalize-space($generate.toc)"/>
        </xsl:call-template>
      </xsl:variable>

      <xsl:if test="contains($toc.params, 'toc')
                    and (d:book|d:part|d:reference|d:preface|d:chapter|d:appendix|d:article
                         |d:glossary|d:bibliography|d:index|d:setindex
                         |d:refentry
                         |d:sect1|d:sect2|d:sect3|d:sect4|d:sect5|d:section)">
        <fo:bookmark internal-destination="toc...{$id}">
          <fo:bookmark-title>
            <xsl:call-template name="gentext">
              <xsl:with-param name="key" select="'TableofContents'"/>
            </xsl:call-template>
          </fo:bookmark-title>
        </fo:bookmark>
      </xsl:if>
      <xsl:apply-templates select="*" mode="fop1.outline"/>
    </xsl:otherwise>
  </xsl:choose>
<!--
  <fo:bookmark internal-destination="{$id}"/>
-->
</xsl:template>


</xsl:stylesheet>
