<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:d="http://docbook.org/ns/docbook"
    xmlns:xlink="http://www.w3.org/1999/xlink"
    exclude-result-prefixes="d xlink">
  <xsl:output method="text" encoding="UTF-8"/>

  <xsl:variable name="alphabet-lower" select="'abcdefghijklmnopqrstuvwxyz'"/>
  <xsl:variable name="alphabet-upper" select="'ABCDEFGHIJKLMNOPQRSTUVWXYZ'"/>

  <xsl:template match="/">
    <xsl:text>[</xsl:text>
    <xsl:for-each select="//d:refentry">
      <xsl:if test="position() &gt; 1">
        <xsl:text>,</xsl:text>
      </xsl:if>
      <xsl:text>&#10;</xsl:text>
      <xsl:call-template name="emit-entry">
        <xsl:with-param name="entry" select="."/>
      </xsl:call-template>
    </xsl:for-each>
    <xsl:text>&#10;]</xsl:text>
  </xsl:template>

  <xsl:template name="emit-entry">
    <xsl:param name="entry"/>
    <xsl:variable name="name" select="normalize-space($entry/d:refnamediv/d:refname)"/>
    <xsl:variable name="purpose" select="normalize-space($entry/d:refnamediv/d:refpurpose)"/>
    <xsl:variable name="id-attr" select="normalize-space($entry/@xml:id)"/>
    <xsl:variable name="id">
      <xsl:choose>
        <xsl:when test="$id-attr != ''">
          <xsl:value-of select="$id-attr"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$name"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="synopsis-raw">
      <xsl:for-each select="$entry/d:refsynopsisdiv/d:synopsis">
        <xsl:value-of select="."/>
        <xsl:text>&#10;</xsl:text>
      </xsl:for-each>
    </xsl:variable>
    <xsl:variable name="arguments" select="d:refsection[d:title]/d:variablelist/d:varlistentry"/>
    <xsl:text>  {&#10;</xsl:text>
    <xsl:text>    "id":"</xsl:text>
    <xsl:call-template name="escape-json-string">
      <xsl:with-param name="text" select="string($id)"/>
    </xsl:call-template>
    <xsl:text>",&#10;</xsl:text>
    <xsl:text>    "name":"</xsl:text>
    <xsl:call-template name="escape-json-string">
      <xsl:with-param name="text" select="string($name)"/>
    </xsl:call-template>
    <xsl:text>",&#10;</xsl:text>
    <xsl:text>    "purpose":</xsl:text>
    <xsl:choose>
      <xsl:when test="$purpose != ''">
        <xsl:text>"</xsl:text>
        <xsl:call-template name="escape-json-string">
          <xsl:with-param name="text" select="string($purpose)"/>
        </xsl:call-template>
        <xsl:text>",&#10;</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:text>null,&#10;</xsl:text>
      </xsl:otherwise>
    </xsl:choose>
    <xsl:text>    "docUrl":"https://help.scilab.org/</xsl:text>
    <xsl:call-template name="escape-json-string">
      <xsl:with-param name="text" select="string($id)"/>
    </xsl:call-template>
    <xsl:text>",&#10;</xsl:text>
    <xsl:text>    "synopsis":</xsl:text>
    <xsl:call-template name="emit-string-array-wrapper">
      <xsl:with-param name="text" select="translate(string($synopsis-raw), '&#13;', '')"/>
      <xsl:with-param name="base-indent" select="'    '"/>
      <xsl:with-param name="item-indent" select="'      '"/>
    </xsl:call-template>
    <xsl:text>,&#10;</xsl:text>
    <xsl:text>    "parameters":[</xsl:text>
    <xsl:choose>
      <xsl:when test="count($arguments) &gt; 0">
        <xsl:text>&#10;</xsl:text>
        <xsl:for-each select="$arguments">
          <xsl:if test="position() &gt; 1">
            <xsl:text>,&#10;</xsl:text>
          </xsl:if>
          <xsl:text>      {"name":"</xsl:text>
          <xsl:call-template name="escape-json-string">
            <xsl:with-param name="text" select="normalize-space(string(d:term))"/>
          </xsl:call-template>
          <xsl:text>","description":"</xsl:text>
          <xsl:variable name="description-fragment">
            <xsl:apply-templates select="d:listitem/node()" mode="plain-text"/>
          </xsl:variable>
          <xsl:variable name="description" select="normalize-space(string($description-fragment))"/>
          <xsl:call-template name="escape-json-string">
            <xsl:with-param name="text" select="$description"/>
          </xsl:call-template>
          <xsl:text>"}</xsl:text>
        </xsl:for-each>
        <xsl:text>&#10;    ]</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:text>]</xsl:text>
      </xsl:otherwise>
    </xsl:choose>
    <xsl:text>&#10;  }</xsl:text>
  </xsl:template>

  <xsl:template name="emit-string-array-wrapper">
    <xsl:param name="text"/>
    <xsl:param name="base-indent"/>
    <xsl:param name="item-indent"/>
    <xsl:variable name="normalized" select="normalize-space($text)"/>
    <xsl:choose>
      <xsl:when test="$normalized = ''">
        <xsl:text>[]</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:text>[</xsl:text>
        <xsl:call-template name="emit-string-array">
          <xsl:with-param name="text" select="$text"/>
          <xsl:with-param name="indent" select="$item-indent"/>
          <xsl:with-param name="first" select="true()"/>
        </xsl:call-template>
        <xsl:text>&#10;</xsl:text>
        <xsl:value-of select="$base-indent"/>
        <xsl:text>]</xsl:text>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="emit-string-array">
    <xsl:param name="text"/>
    <xsl:param name="indent"/>
    <xsl:param name="first" select="true()"/>
    <xsl:variable name="clean" select="translate($text, '&#13;', '')"/>
    <xsl:choose>
      <xsl:when test="contains($clean, '&#10;')">
        <xsl:variable name="line" select="substring-before($clean, '&#10;')"/>
        <xsl:variable name="rest" select="substring-after($clean, '&#10;')"/>
        <xsl:variable name="trimmed">
          <xsl:call-template name="trim-both">
            <xsl:with-param name="text" select="$line"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="line-value" select="string($trimmed)"/>
        <xsl:choose>
          <xsl:when test="$line-value != ''">
            <xsl:if test="$first">
              <xsl:text>&#10;</xsl:text>
            </xsl:if>
            <xsl:if test="not($first)">
              <xsl:text>,&#10;</xsl:text>
            </xsl:if>
            <xsl:value-of select="$indent"/>
            <xsl:text>"</xsl:text>
            <xsl:call-template name="escape-json-string">
              <xsl:with-param name="text" select="$line-value"/>
            </xsl:call-template>
            <xsl:text>"</xsl:text>
            <xsl:call-template name="emit-string-array">
              <xsl:with-param name="text" select="$rest"/>
              <xsl:with-param name="indent" select="$indent"/>
              <xsl:with-param name="first" select="false()"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="emit-string-array">
              <xsl:with-param name="text" select="$rest"/>
              <xsl:with-param name="indent" select="$indent"/>
              <xsl:with-param name="first" select="$first"/>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>
        <xsl:variable name="trimmed-final">
          <xsl:call-template name="trim-both">
            <xsl:with-param name="text" select="$clean"/>
          </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="line-value" select="string($trimmed-final)"/>
        <xsl:if test="$line-value != ''">
          <xsl:if test="$first">
            <xsl:text>&#10;</xsl:text>
          </xsl:if>
          <xsl:if test="not($first)">
            <xsl:text>,&#10;</xsl:text>
          </xsl:if>
          <xsl:value-of select="$indent"/>
          <xsl:text>"</xsl:text>
          <xsl:call-template name="escape-json-string">
            <xsl:with-param name="text" select="$line-value"/>
          </xsl:call-template>
          <xsl:text>"</xsl:text>
        </xsl:if>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="escape-json-string">
    <xsl:param name="text"/>
    <xsl:if test="string-length($text) &gt; 0">
      <xsl:variable name="char" select="substring($text, 1, 1)"/>
      <xsl:choose>
        <xsl:when test="$char = '&quot;'">
          <xsl:text>\&quot;</xsl:text>
        </xsl:when>
        <xsl:when test="$char = '\'">
          <xsl:text>\\</xsl:text>
        </xsl:when>
        <xsl:when test="$char = '&#09;'">
          <xsl:text>\t</xsl:text>
        </xsl:when>
        <xsl:when test="$char = '&#10;'">
          <xsl:text>\n</xsl:text>
        </xsl:when>
        <xsl:when test="$char = '&#13;'">
          <xsl:text>\r</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$char"/>
        </xsl:otherwise>
      </xsl:choose>
      <xsl:call-template name="escape-json-string">
        <xsl:with-param name="text" select="substring($text, 2)"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

  <xsl:template name="trim-both">
    <xsl:param name="text"/>
    <xsl:variable name="lead">
      <xsl:call-template name="trim-leading">
        <xsl:with-param name="text" select="$text"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:call-template name="trim-trailing">
      <xsl:with-param name="text" select="string($lead)"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="trim-leading">
    <xsl:param name="text"/>
    <xsl:choose>
      <xsl:when test="starts-with($text, ' ') or starts-with($text, '&#9;') or starts-with($text, '&#10;') or starts-with($text, '&#13;')">
        <xsl:call-template name="trim-leading">
          <xsl:with-param name="text" select="substring($text, 2)"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$text"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="trim-trailing">
    <xsl:param name="text"/>
    <xsl:variable name="len" select="string-length($text)"/>
    <xsl:choose>
      <xsl:when test="$len = 0">
        <xsl:text></xsl:text>
      </xsl:when>
      <xsl:when test="substring($text, $len, 1) = ' ' or substring($text, $len, 1) = '&#9;' or substring($text, $len, 1) = '&#10;' or substring($text, $len, 1) = '&#13;'">
        <xsl:call-template name="trim-trailing">
          <xsl:with-param name="text" select="substring($text, 1, $len - 1)"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$text"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="d:itemizedlist" mode="plain-text">
    <xsl:for-each select="d:listitem">
      <xsl:apply-templates select="node()" mode="plain-text"/>
      <xsl:if test="position() != last()">
        <xsl:text>; </xsl:text>
      </xsl:if>
    </xsl:for-each>
    <xsl:text> </xsl:text>
  </xsl:template>

  <xsl:template match="d:orderedlist" mode="plain-text">
    <xsl:for-each select="d:listitem">
      <xsl:apply-templates select="node()" mode="plain-text"/>
      <xsl:if test="position() != last()">
        <xsl:text>; </xsl:text>
      </xsl:if>
    </xsl:for-each>
    <xsl:text> </xsl:text>
  </xsl:template>

  <xsl:template match="d:listitem" mode="plain-text">
    <xsl:apply-templates select="node()" mode="plain-text"/>
  </xsl:template>

  <xsl:template match="d:para|d:simpara" mode="plain-text">
    <xsl:apply-templates select="node()" mode="plain-text"/>
    <xsl:text> </xsl:text>
  </xsl:template>

  <xsl:template match="d:link|d:literal|d:varname|d:emphasis|d:code|d:function|d:command|d:filename|d:replaceable" mode="plain-text">
    <xsl:apply-templates select="node()" mode="plain-text"/>
  </xsl:template>

  <xsl:template match="text()" mode="plain-text">
    <xsl:variable name="value" select="normalize-space(.)"/>
    <xsl:if test="$value != ''">
      <xsl:value-of select="$value"/>
      <xsl:text> </xsl:text>
    </xsl:if>
  </xsl:template>

  <xsl:template match="*" mode="plain-text">
    <xsl:apply-templates select="node()" mode="plain-text"/>
  </xsl:template>

</xsl:stylesheet>
