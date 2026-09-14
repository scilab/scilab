<?xml version="1.0" encoding="UTF-8"?>
<!--
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2025 - Dassault Systèmes S.E. - Antoine ELIAS

 * This file is distributed under the same license as the Scilab package.
  -->
<xsl:stylesheet version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:d="http://docbook.org/ns/docbook"
    exclude-result-prefixes="d">
  <xsl:output method="text" encoding="UTF-8"/>

  <xsl:variable name="inline-threshold" select="8"/>

  <!-- The arguments section is identified by its role. Pages not tagged yet
       fall back on the section title. Never pick any other titled section:
       the description one usually holds a variablelist too. -->
  <xsl:variable name="args-section"
                select="(//d:refsection[@role='arguments'] | //d:refsection[d:title='Arguments'])[1]"/>

  <xsl:variable name="args-entries" select="$args-section/d:variablelist/d:varlistentry"/>

  <xsl:variable name="max-inline-length">
    <xsl:choose>
      <xsl:when test="count($args-entries[string-length(normalize-space(d:term)) &lt;= $inline-threshold]) &gt; 0">
        <xsl:for-each select="$args-entries[string-length(normalize-space(d:term)) &lt;= $inline-threshold]">
          <xsl:sort select="string-length(normalize-space(d:term))" data-type="number" order="descending"/>
          <xsl:if test="position() = 1">
            <xsl:value-of select="string-length(normalize-space(d:term))"/>
          </xsl:if>
        </xsl:for-each>
      </xsl:when>
      <xsl:otherwise>0</xsl:otherwise>
    </xsl:choose>
  </xsl:variable>

  <xsl:variable name="continuation-prefix">
    <xsl:text>        </xsl:text>
    <xsl:if test="number($max-inline-length) &gt; 0">
      <xsl:call-template name="indent">
        <xsl:with-param name="count" select="number($max-inline-length)"/>
      </xsl:call-template>
    </xsl:if>
    <xsl:text>   </xsl:text>
  </xsl:variable>

  <xsl:template match="/">
    <xsl:apply-templates select="//d:refentry"/>
  </xsl:template>

  <xsl:template match="d:refentry">
    <xsl:value-of select="normalize-space(d:refnamediv/d:refname)"/>
    <xsl:text>&#10;    </xsl:text><xsl:value-of select="normalize-space(d:refnamediv/d:refpurpose)"/>
    <xsl:text>&#10;</xsl:text>
    <xsl:if test="normalize-space(d:refsynopsisdiv/d:synopsis) != ''">
      <xsl:text>&#10;    Syntax:&#10;</xsl:text>
      <xsl:call-template name="emit-lines">
        <xsl:with-param name="text" select="string(d:refsynopsisdiv/d:synopsis)"/>
      </xsl:call-template>
    </xsl:if>
    <xsl:if test="count($args-entries) &gt; 0">
      <xsl:text>&#10;    Arguments:&#10;</xsl:text>
      <xsl:apply-templates select="$args-entries"/>
    </xsl:if>
    <xsl:apply-templates select="d:refsection[@role='see also']"/>
    <xsl:text>&#10;Type "doc </xsl:text>
    <xsl:value-of select="normalize-space(@xml:id)"/>
    <xsl:text>" to open the </xsl:text>
    <xsl:value-of select="normalize-space(d:refnamediv/d:refname)"/>
    <xsl:text> reference page&#10;</xsl:text>
  </xsl:template>

  <xsl:template match="d:varlistentry">
    <xsl:variable name="term" select="normalize-space(d:term)"/>
    <xsl:variable name="content" select="d:listitem/node()[not(self::text() and normalize-space(.) = '')]"/>
    <xsl:choose>
      <xsl:when test="count($content) = 0">
        <xsl:call-template name="term-prefix">
          <xsl:with-param name="term" select="$term"/>
          <xsl:with-param name="continuation" select="string($continuation-prefix)"/>
        </xsl:call-template>
        <xsl:text>&#10;</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="render-blocks">
          <xsl:with-param name="nodes" select="$content"/>
          <xsl:with-param name="term" select="$term"/>
          <xsl:with-param name="continuation-prefix" select="string($continuation-prefix)"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
    <xsl:text>&#10;</xsl:text>
  </xsl:template>

  <xsl:template match="text()"/>

  <xsl:template name="render-blocks">
    <xsl:param name="nodes"/>
    <xsl:param name="term"/>
    <xsl:param name="continuation-prefix"/>
    <xsl:param name="first" select="true()"/>
    <xsl:if test="count($nodes) &gt; 0">
      <xsl:variable name="current" select="$nodes[1]"/>
      <xsl:choose>
        <xsl:when test="$current[self::d:para]">
          <xsl:variable name="block-prefix">
            <xsl:choose>
              <xsl:when test="$first">
                <xsl:call-template name="term-prefix">
                  <xsl:with-param name="term" select="$term"/>
                  <xsl:with-param name="continuation" select="string($continuation-prefix)"/>
                </xsl:call-template>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$continuation-prefix"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:variable>
          <xsl:call-template name="wrap-text">
            <xsl:with-param name="text" select="string($current)"/>
            <xsl:with-param name="width" select="80"/>
            <xsl:with-param name="prefix" select="string($block-prefix)"/>
            <xsl:with-param name="continuation-prefix" select="$continuation-prefix"/>
          </xsl:call-template>
          <xsl:call-template name="render-blocks">
            <xsl:with-param name="nodes" select="$nodes[position() &gt; 1]"/>
            <xsl:with-param name="term" select="$term"/>
            <xsl:with-param name="continuation-prefix" select="$continuation-prefix"/>
            <xsl:with-param name="first" select="false()"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:when test="$current[self::d:itemizedlist]">
          <xsl:if test="$first">
            <xsl:call-template name="term-prefix">
              <xsl:with-param name="term" select="$term"/>
              <xsl:with-param name="continuation" select="string($continuation-prefix)"/>
            </xsl:call-template>
            <xsl:text>&#10;</xsl:text>
          </xsl:if>
          <xsl:call-template name="render-itemizedlist">
            <xsl:with-param name="list" select="$current"/>
            <xsl:with-param name="prefix" select="$continuation-prefix"/>
          </xsl:call-template>
          <xsl:call-template name="render-blocks">
            <xsl:with-param name="nodes" select="$nodes[position() &gt; 1]"/>
            <xsl:with-param name="term" select="$term"/>
            <xsl:with-param name="continuation-prefix" select="$continuation-prefix"/>
            <xsl:with-param name="first" select="false()"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:call-template name="render-blocks">
            <xsl:with-param name="nodes" select="$nodes[position() &gt; 1]"/>
            <xsl:with-param name="term" select="$term"/>
            <xsl:with-param name="continuation-prefix" select="$continuation-prefix"/>
            <xsl:with-param name="first" select="$first"/>
          </xsl:call-template>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:if>
  </xsl:template>

  <xsl:template name="term-prefix">
    <xsl:param name="term"/>
    <xsl:param name="continuation"/>
    <xsl:text>        </xsl:text>
    <xsl:variable name="max" select="number($max-inline-length)"/>
    <xsl:variable name="len" select="string-length($term)"/>
    <xsl:variable name="threshold" select="number($inline-threshold)"/>
    <xsl:choose>
      <xsl:when test="$len &lt;= $threshold">
        <xsl:variable name="diff" select="$max - $len"/>
        <xsl:if test="$diff &gt; 0">
          <xsl:call-template name="indent">
            <xsl:with-param name="count" select="$diff"/>
          </xsl:call-template>
        </xsl:if>
        <xsl:value-of select="$term"/>
        <xsl:text>: </xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$term"/>
        <xsl:text>: </xsl:text>
        <xsl:text>&#10;</xsl:text>
        <xsl:value-of select="$continuation"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="render-itemizedlist">
    <xsl:param name="list"/>
    <xsl:param name="prefix"/>
    <xsl:for-each select="$list/d:listitem">
      <xsl:call-template name="render-listitem">
        <xsl:with-param name="item" select="."/>
        <xsl:with-param name="prefix" select="concat($prefix, '- ')"/>
        <xsl:with-param name="continuation-prefix" select="concat($prefix, '  ')"/>
      </xsl:call-template>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="render-listitem">
    <xsl:param name="item"/>
    <xsl:param name="prefix"/>
    <xsl:param name="continuation-prefix"/>
    <xsl:variable name="text">
      <xsl:apply-templates select="$item/node()" mode="inline-text"/>
    </xsl:variable>
    <xsl:if test="normalize-space($text) != ''">
      <xsl:call-template name="wrap-text">
        <xsl:with-param name="text" select="normalize-space($text)"/>
        <xsl:with-param name="width" select="80"/>
        <xsl:with-param name="prefix" select="string($prefix)"/>
        <xsl:with-param name="continuation-prefix" select="$continuation-prefix"/>
      </xsl:call-template>
    </xsl:if>
    <xsl:for-each select="$item/d:itemizedlist">
      <xsl:call-template name="render-itemizedlist">
        <xsl:with-param name="list" select="."/>
        <xsl:with-param name="prefix" select="concat($continuation-prefix, '  ')"/>
      </xsl:call-template>
    </xsl:for-each>
  </xsl:template>

  <xsl:template match="d:refsection[@role='see also']">
    <xsl:variable name="items">
      <xsl:choose>
        <xsl:when test=".//d:member[not(d:link)]">
          <xsl:for-each select=".//d:member">
            <xsl:variable name="label" select="normalize-space(string(.))"/>
            <xsl:if test="$label != ''">
              <xsl:if test="position() &gt; 1">
                <xsl:text>, </xsl:text>
              </xsl:if>
              <xsl:value-of select="$label"/>
            </xsl:if>
          </xsl:for-each>
        </xsl:when>
        <xsl:otherwise>
          <xsl:for-each select=".//d:link">
            <xsl:variable name="label" select="normalize-space(@linkend)"/>
            <xsl:if test="$label != ''">
              <xsl:if test="position() &gt; 1">
                <xsl:text>, </xsl:text>
              </xsl:if>
              <xsl:value-of select="$label"/>
            </xsl:if>
          </xsl:for-each>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="textValue" select="normalize-space(string($items))"/>
    <xsl:if test="$textValue != ''">
      <xsl:text>&#10;    See also:&#10;</xsl:text>
      <xsl:call-template name="wrap-text">
        <xsl:with-param name="text" select="$textValue"/>
        <xsl:with-param name="width" select="80"/>
        <xsl:with-param name="prefix" select="'        '"/>
        <xsl:with-param name="continuation-prefix" select="'        '"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

  <xsl:template name="emit-lines">
    <xsl:param name="text"/>
    <xsl:if test="string-length($text) &gt; 0">
      <xsl:variable name="clean" select="translate($text, '&#13;', '')"/>
      <xsl:choose>
        <xsl:when test="contains($clean, '&#10;')">
          <xsl:variable name="line" select="substring-before($clean, '&#10;')"/>
          <xsl:call-template name="emit-line">
            <xsl:with-param name="line" select="$line"/>
          </xsl:call-template>
          <xsl:call-template name="emit-lines">
            <xsl:with-param name="text" select="substring-after($clean, '&#10;')"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:call-template name="emit-line">
            <xsl:with-param name="line" select="$clean"/>
          </xsl:call-template>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:if>
  </xsl:template>

  <xsl:template name="emit-line">
    <xsl:param name="line"/>
    <xsl:variable name="trimmed">
      <xsl:call-template name="trim-leading">
        <xsl:with-param name="text" select="$line"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:if test="normalize-space(string($trimmed)) != ''">
      <xsl:text>        </xsl:text>
      <xsl:value-of select="string($trimmed)"/>
      <xsl:text>&#10;</xsl:text>
    </xsl:if>
  </xsl:template>

  <xsl:template name="wrap-text">
    <xsl:param name="text"/>
    <xsl:param name="width" select="80"/>
    <xsl:param name="prefix"/>
    <xsl:param name="continuation-prefix"/>
    <xsl:param name="current-line" select="''"/>
    <xsl:param name="current-prefix" select="$prefix"/>
    <xsl:variable name="normalized" select="normalize-space($text)"/>
    <xsl:choose>
      <xsl:when test="$normalized = ''">
        <xsl:if test="string-length($current-line) &gt; 0">
          <xsl:value-of select="$current-prefix"/>
          <xsl:value-of select="$current-line"/>
          <xsl:text>&#10;</xsl:text>
        </xsl:if>
      </xsl:when>
      <xsl:otherwise>
        <xsl:variable name="word">
          <xsl:choose>
            <xsl:when test="contains($normalized, ' ')">
              <xsl:value-of select="substring-before($normalized, ' ')"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$normalized"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        <xsl:variable name="remaining">
          <xsl:choose>
            <xsl:when test="contains($normalized, ' ')">
              <xsl:value-of select="substring-after($normalized, ' ')"/>
            </xsl:when>
            <xsl:otherwise/>
          </xsl:choose>
        </xsl:variable>
        <xsl:variable name="candidate">
          <xsl:choose>
            <xsl:when test="string-length($current-line) = 0">
              <xsl:value-of select="$word"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="concat($current-line, ' ', $word)"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        <xsl:variable name="line-length" select="string-length($current-prefix) + string-length($candidate)"/>
        <xsl:choose>
          <xsl:when test="$line-length &lt;= $width">
            <xsl:call-template name="wrap-text">
              <xsl:with-param name="text" select="$remaining"/>
              <xsl:with-param name="width" select="$width"/>
              <xsl:with-param name="prefix" select="$prefix"/>
              <xsl:with-param name="continuation-prefix" select="$continuation-prefix"/>
              <xsl:with-param name="current-line" select="$candidate"/>
              <xsl:with-param name="current-prefix" select="$current-prefix"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:choose>
              <xsl:when test="string-length($current-line) &gt; 0">
                <xsl:value-of select="$current-prefix"/>
                <xsl:value-of select="$current-line"/>
                <xsl:text>&#10;</xsl:text>
                <xsl:call-template name="wrap-text">
                  <xsl:with-param name="text">
                    <xsl:choose>
                      <xsl:when test="string-length($remaining) &gt; 0">
                        <xsl:value-of select="concat($word, ' ', $remaining)"/>
                      </xsl:when>
                      <xsl:otherwise>
                        <xsl:value-of select="$word"/>
                      </xsl:otherwise>
                    </xsl:choose>
                  </xsl:with-param>
                  <xsl:with-param name="width" select="$width"/>
                  <xsl:with-param name="prefix" select="$prefix"/>
                  <xsl:with-param name="continuation-prefix" select="$continuation-prefix"/>
                  <xsl:with-param name="current-line" select="''"/>
                  <xsl:with-param name="current-prefix" select="$continuation-prefix"/>
                </xsl:call-template>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$current-prefix"/>
                <xsl:value-of select="$word"/>
                <xsl:text>&#10;</xsl:text>
                <xsl:call-template name="wrap-text">
                  <xsl:with-param name="text" select="$remaining"/>
                  <xsl:with-param name="width" select="$width"/>
                  <xsl:with-param name="prefix" select="$prefix"/>
                  <xsl:with-param name="continuation-prefix" select="$continuation-prefix"/>
                  <xsl:with-param name="current-line" select="''"/>
                  <xsl:with-param name="current-prefix" select="$continuation-prefix"/>
                </xsl:call-template>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="trim-leading">
    <xsl:param name="text"/>
    <xsl:choose>
      <xsl:when test="starts-with($text, ' ')">
        <xsl:call-template name="trim-leading">
          <xsl:with-param name="text" select="substring($text, 2)"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="starts-with($text, '&#9;')">
        <xsl:call-template name="trim-leading">
          <xsl:with-param name="text" select="substring($text, 2)"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$text"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="indent">
    <xsl:param name="count"/>
    <xsl:if test="$count &gt; 0">
      <xsl:text> </xsl:text>
      <xsl:call-template name="indent">
        <xsl:with-param name="count" select="$count - 1"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

  <xsl:template match="d:itemizedlist" mode="inline-text"/>

  <xsl:template match="d:para" mode="inline-text">
    <xsl:apply-templates select="node()" mode="inline-text"/>
    <xsl:text> </xsl:text>
  </xsl:template>

  <xsl:template match="text()" mode="inline-text">
    <xsl:variable name="value" select="normalize-space(.)"/>
    <xsl:if test="$value != ''">
      <xsl:value-of select="$value"/>
      <xsl:text> </xsl:text>
    </xsl:if>
  </xsl:template>

  <xsl:template match="*" mode="inline-text">
    <xsl:apply-templates select="node()" mode="inline-text"/>
  </xsl:template>
</xsl:stylesheet>
