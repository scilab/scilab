<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:d="http://docbook.org/ns/docbook"
    xmlns:xlink="http://www.w3.org/1999/xlink"
    exclude-result-prefixes="d xlink">
  <xsl:output method="text" encoding="UTF-8"/>

  <xsl:template match="/">
    <xsl:apply-templates select="//d:refentry"/>
  </xsl:template>

  <xsl:template match="d:refentry">
    <xsl:variable name="name" select="normalize-space(d:refnamediv/d:refname)"/>
    <xsl:variable name="purpose" select="normalize-space(d:refnamediv/d:refpurpose)"/>
    <xsl:variable name="synopsis" select="string(d:refsynopsisdiv/d:synopsis)"/>
    <xsl:variable name="entry-id" select="normalize-space(@xml:id)"/>
    <xsl:text>## </xsl:text>
    <xsl:call-template name="escape-markdown-tags">
      <xsl:with-param name="text" select="$name"/>
    </xsl:call-template>
    <xsl:text>&#10;&#10;</xsl:text>
    <xsl:if test="$purpose != ''">
      <xsl:text>_</xsl:text>
      <xsl:call-template name="escape-markdown-tags">
        <xsl:with-param name="text" select="$purpose"/>
      </xsl:call-template>
      <xsl:text>_&#10;&#10;</xsl:text>
    </xsl:if>
    <xsl:call-template name="emit-syntax">
      <xsl:with-param name="text" select="$synopsis"/>
    </xsl:call-template>
    <xsl:variable name="arguments" select="d:refsection[d:title]/d:variablelist/d:varlistentry"/>
    <xsl:if test="count($arguments) &gt; 0">
      <xsl:text>### Arguments&#10;&#10;</xsl:text>
      <xsl:apply-templates select="$arguments"/>
      <xsl:text>&#10;</xsl:text>
    </xsl:if>
    <xsl:text>&#10;</xsl:text>
    <xsl:variable name="doc-id">
      <xsl:choose>
        <xsl:when test="$entry-id != ''">
          <xsl:value-of select="$entry-id"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$name"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
  </xsl:template>

  <xsl:template match="d:varlistentry">
    <xsl:variable name="term">
      <xsl:call-template name="escape-markdown-tags">
        <xsl:with-param name="text" select="normalize-space(d:term)"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="content" select="d:listitem/node()[not(self::text() and normalize-space(.) = '')]"/>
    <xsl:choose>
      <xsl:when test="count($content) = 0">
        <xsl:text>- **</xsl:text>
        <xsl:value-of select="$term"/>
        <xsl:text>**&#10;</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="render-md-blocks">
          <xsl:with-param name="nodes" select="$content"/>
          <xsl:with-param name="term" select="$term"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="render-md-blocks">
    <xsl:param name="nodes"/>
    <xsl:param name="term"/>
    <xsl:param name="first" select="true()"/>
    <xsl:if test="count($nodes) &gt; 0">
      <xsl:variable name="current" select="$nodes[1]"/>
      <xsl:choose>
        <xsl:when test="$current[self::d:para]">
          <xsl:if test="not($first)">
            <xsl:text>&#10;</xsl:text>
          </xsl:if>
          <xsl:variable name="line-prefix">
            <xsl:choose>
              <xsl:when test="$first">
                <xsl:value-of select="concat('- **', $term, '**: ')"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>  </xsl:text>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:variable>
          <xsl:variable name="para-text">
            <xsl:apply-templates select="$current/node()" mode="inline-text"/>
          </xsl:variable>
          <xsl:call-template name="wrap-text">
            <xsl:with-param name="text" select="normalize-space($para-text)"/>
            <xsl:with-param name="width" select="100"/>
            <xsl:with-param name="prefix" select="string($line-prefix)"/>
            <xsl:with-param name="continuation-prefix" select="'  '"/>
          </xsl:call-template>
          <xsl:call-template name="render-md-blocks">
            <xsl:with-param name="nodes" select="$nodes[position() &gt; 1]"/>
            <xsl:with-param name="term" select="$term"/>
            <xsl:with-param name="first" select="false()"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:when test="$current[self::d:itemizedlist]">
          <xsl:if test="$first">
            <xsl:text>- **</xsl:text>
            <xsl:value-of select="$term"/>
            <xsl:text>**&#10;</xsl:text>
          </xsl:if>
          <xsl:call-template name="render-md-itemizedlist">
            <xsl:with-param name="list" select="$current"/>
            <xsl:with-param name="prefix" select="'  '"/>
          </xsl:call-template>
          <xsl:call-template name="render-md-blocks">
            <xsl:with-param name="nodes" select="$nodes[position() &gt; 1]"/>
            <xsl:with-param name="term" select="$term"/>
            <xsl:with-param name="first" select="false()"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:call-template name="render-md-blocks">
            <xsl:with-param name="nodes" select="$nodes[position() &gt; 1]"/>
            <xsl:with-param name="term" select="$term"/>
            <xsl:with-param name="first" select="$first"/>
          </xsl:call-template>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:if>
  </xsl:template>

  <xsl:template name="render-md-itemizedlist">
    <xsl:param name="list"/>
    <xsl:param name="prefix"/>
    <xsl:for-each select="$list/d:listitem">
      <xsl:call-template name="render-md-listitem">
        <xsl:with-param name="item" select="."/>
        <xsl:with-param name="bullet-prefix" select="concat($prefix, '- ')"/>
        <xsl:with-param name="continuation-prefix" select="concat($prefix, '  ')"/>
      </xsl:call-template>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="render-md-listitem">
    <xsl:param name="item"/>
    <xsl:param name="bullet-prefix"/>
    <xsl:param name="continuation-prefix"/>
    <xsl:variable name="text">
      <xsl:apply-templates select="$item/node()" mode="inline-text"/>
    </xsl:variable>
    <xsl:if test="normalize-space($text) != ''">
      <xsl:call-template name="wrap-text">
        <xsl:with-param name="text" select="normalize-space($text)"/>
        <xsl:with-param name="width" select="100"/>
        <xsl:with-param name="prefix" select="string($bullet-prefix)"/>
        <xsl:with-param name="continuation-prefix" select="string($continuation-prefix)"/>
      </xsl:call-template>
    </xsl:if>
    <xsl:for-each select="$item/d:itemizedlist">
      <xsl:call-template name="render-md-itemizedlist">
        <xsl:with-param name="list" select="."/>
        <xsl:with-param name="prefix" select="concat($continuation-prefix, '  ')"/>
      </xsl:call-template>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="emit-syntax">
    <xsl:param name="text"/>
    <xsl:variable name="clean" select="translate($text, '&#13;', '')"/>
    <xsl:if test="normalize-space($clean) != ''">
      <xsl:text>### Syntax&#10;&#10;</xsl:text>
      <xsl:text>```scilab&#10;</xsl:text>
      <xsl:call-template name="emit-raw-lines">
        <xsl:with-param name="text" select="$clean"/>
      </xsl:call-template>
      <xsl:text>```&#10;&#10;</xsl:text>
    </xsl:if>
  </xsl:template>

  <xsl:template name="emit-raw-lines">
    <xsl:param name="text"/>
    <xsl:if test="string-length($text) &gt; 0">
      <xsl:variable name="clean" select="translate($text, '&#13;', '')"/>
      <xsl:choose>
        <xsl:when test="contains($clean, '&#10;')">
          <xsl:variable name="line" select="substring-before($clean, '&#10;')"/>
          <xsl:call-template name="emit-raw-line">
            <xsl:with-param name="line" select="$line"/>
          </xsl:call-template>
          <xsl:call-template name="emit-raw-lines">
            <xsl:with-param name="text" select="substring-after($clean, '&#10;')"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:call-template name="emit-raw-line">
            <xsl:with-param name="line" select="$clean"/>
          </xsl:call-template>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:if>
  </xsl:template>

  <xsl:template name="emit-raw-line">
    <xsl:param name="line"/>
    <xsl:variable name="trimmed">
      <xsl:call-template name="trim-leading">
        <xsl:with-param name="text" select="$line"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:value-of select="$trimmed"/>
    <xsl:text>&#10;</xsl:text>
  </xsl:template>

  <xsl:template name="wrap-text">
    <xsl:param name="text"/>
    <xsl:param name="width" select="100"/>
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

  <xsl:template match="d:itemizedlist" mode="inline-text"/>

  <xsl:template match="d:para" mode="inline-text">
    <xsl:apply-templates select="node()" mode="inline-text"/>
    <xsl:text> </xsl:text>
  </xsl:template>

  <xsl:template match="d:varname" mode="inline-text">
    <xsl:variable name="content">
      <xsl:apply-templates select="node()" mode="inline-text"/>
    </xsl:variable>
    <xsl:text>_**</xsl:text>
    <xsl:value-of select="normalize-space($content)"/>
    <xsl:text>**_</xsl:text>
    <xsl:text> </xsl:text>
  </xsl:template>

  <xsl:template match="d:emphasis" mode="inline-text">
    <xsl:variable name="content">
      <xsl:apply-templates select="node()" mode="inline-text"/>
    </xsl:variable>
    <xsl:text>**</xsl:text>
    <xsl:value-of select="normalize-space($content)"/>
    <xsl:text>**</xsl:text>
    <xsl:text> </xsl:text>
  </xsl:template>

  <xsl:template match="d:literal" mode="inline-text">
    <xsl:variable name="content">
      <xsl:apply-templates select="node()" mode="inline-text"/>
    </xsl:variable>
    <xsl:text>_</xsl:text>
    <xsl:value-of select="normalize-space($content)"/>
    <xsl:text>_</xsl:text>
    <xsl:text> </xsl:text>
  </xsl:template>

  <xsl:template match="d:link" mode="inline-text">
    <xsl:variable name="label">
      <xsl:apply-templates select="node()" mode="inline-text"/>
    </xsl:variable>
    <xsl:variable name="linkend" select="normalize-space(@linkend)"/>
    <xsl:variable name="href">
      <xsl:choose>
        <xsl:when test="$linkend != ''">
          <xsl:value-of select="concat('scilab-help://', $linkend)"/>
        </xsl:when>
        <xsl:when test="normalize-space(@xlink:href) != ''">
          <xsl:value-of select="normalize-space(@xlink:href)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="'scilab-help://index'"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="display-label">
      <xsl:choose>
        <xsl:when test="normalize-space($label) != ''">
          <xsl:value-of select="normalize-space($label)"/>
        </xsl:when>
        <xsl:when test="$linkend != ''">
          <xsl:value-of select="$linkend"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="'link'"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:value-of select="concat('[', $display-label, '](', $href, ')')"/>
    <xsl:text> </xsl:text>
  </xsl:template>

  <xsl:template match="text()" mode="inline-text">
    <xsl:variable name="value" select="normalize-space(.)"/>
    <xsl:if test="$value != ''">
      <xsl:call-template name="escape-markdown-tags">
        <xsl:with-param name="text" select="$value"/>
      </xsl:call-template>
      <xsl:text> </xsl:text>
    </xsl:if>
  </xsl:template>

  <!--
    The help writes its placeholders between angle brackets, as in
    <nom_de_la_fonction>. Left as they are, a markdown reader takes them for
    HTML tags and drops them, leaving an empty term. This escaping is for the
    prose and the terms only: the syntax block is fenced code, where a reader
    would show the escape itself.
  -->
  <xsl:template name="escape-markdown-tags">
    <xsl:param name="text"/>
    <xsl:choose>
      <xsl:when test="contains($text, '&lt;')">
        <xsl:call-template name="escape-markdown-tags">
          <xsl:with-param name="text" select="substring-before($text, '&lt;')"/>
        </xsl:call-template>
        <xsl:text>&amp;lt;</xsl:text>
        <xsl:call-template name="escape-markdown-tags">
          <xsl:with-param name="text" select="substring-after($text, '&lt;')"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="contains($text, '&gt;')">
        <xsl:value-of select="substring-before($text, '&gt;')"/>
        <xsl:text>&amp;gt;</xsl:text>
        <xsl:call-template name="escape-markdown-tags">
          <xsl:with-param name="text" select="substring-after($text, '&gt;')"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$text"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="*" mode="inline-text">
    <xsl:apply-templates select="node()" mode="inline-text"/>
  </xsl:template>

  <xsl:template name="trim-leading">
    <xsl:param name="text"/>
    <xsl:choose>
      <xsl:when test="starts-with($text, ' ') or starts-with($text, '&#9;')">
        <xsl:call-template name="trim-leading">
          <xsl:with-param name="text" select="substring($text, 2)"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$text"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
</xsl:stylesheet>
