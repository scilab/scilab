<?xml version="1.0" encoding="UTF-8"?>
<!--
  Help page to the short markdown of a hover: name and purpose, the syntax,
  and one line per argument.
-->
<xsl:stylesheet version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:d="http://docbook.org/ns/docbook"
    xmlns:xlink="http://www.w3.org/1999/xlink"
    exclude-result-prefixes="d xlink">
  <xsl:output method="text" encoding="UTF-8"/>

  <!-- Syntax forms shown before the block is cut, by a line "...". -->
  <xsl:variable name="max-syntax-lines" select="6"/>

  <!-- What separates names in a syntax form or an argument term, and as many blanks. -->
  <xsl:variable name="separators">()[]{},;:=.'"|+-*/\&lt;&gt;!~^&amp;`@#&#9;&#10;&#13;</xsl:variable>
  <xsl:variable name="blanks" select="'                                  '"/>

  <xsl:template match="/">
    <xsl:apply-templates select="//d:refentry[1]"/>
  </xsl:template>

  <xsl:template match="d:refentry">
    <xsl:variable name="name" select="normalize-space(d:refnamediv/d:refname)"/>
    <xsl:variable name="purpose" select="normalize-space(d:refnamediv/d:refpurpose)"/>

    <xsl:text>**</xsl:text>
    <xsl:call-template name="escape-markdown-tags">
      <xsl:with-param name="text" select="$name"/>
    </xsl:call-template>
    <xsl:text>**</xsl:text>
    <xsl:if test="$purpose != ''">
      <xsl:text> — </xsl:text>
      <xsl:call-template name="escape-markdown-tags">
        <xsl:with-param name="text" select="$purpose"/>
      </xsl:call-template>
    </xsl:if>
    <xsl:text>&#10;&#10;</xsl:text>

    <xsl:variable name="syntax">
      <xsl:call-template name="emit-syntax">
        <xsl:with-param name="text" select="translate(string(d:refsynopsisdiv/d:synopsis), '&#13;', '')"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:value-of select="$syntax"/>
    <!-- Only the arguments of the forms shown, when some are left out. -->
    <xsl:variable name="shown-words">
      <xsl:if test="contains($syntax, '&#10;...&#10;```')">
        <xsl:value-of select="concat(' ', normalize-space(translate($syntax, $separators, $blanks)), ' ')"/>
      </xsl:if>
    </xsl:variable>

    <!--
      The arguments section only: the description often holds variable lists
      too, which are not arguments. Most pages give it a role; otherwise it is
      the first section with a variable list.
    -->
    <xsl:variable name="by-role" select="d:refsection[@role = 'arguments' or @role = 'parameters'][1]"/>
    <xsl:variable name="section" select="$by-role | d:refsection[d:variablelist][not($by-role)][1]"/>
    <xsl:variable name="arguments" select="$section/d:variablelist/d:varlistentry"/>
    <xsl:if test="count($arguments) &gt; 0">
      <xsl:apply-templates select="$arguments">
        <xsl:with-param name="shown-words" select="string($shown-words)"/>
      </xsl:apply-templates>
      <xsl:text>&#10;</xsl:text>
    </xsl:if>
  </xsl:template>

  <!-- ================================================================ -->
  <!-- syntax                                                           -->
  <!-- ================================================================ -->

  <xsl:template name="emit-syntax">
    <xsl:param name="text"/>
    <xsl:if test="normalize-space($text) != ''">
      <!-- The indentation of the first form is the XML one: removed from all. -->
      <xsl:variable name="first-line">
        <xsl:call-template name="first-non-blank-line">
          <xsl:with-param name="text" select="$text"/>
        </xsl:call-template>
      </xsl:variable>
      <xsl:variable name="first-trimmed">
        <xsl:call-template name="trim-leading">
          <xsl:with-param name="text" select="$first-line"/>
        </xsl:call-template>
      </xsl:variable>
      <xsl:text>```scilab&#10;</xsl:text>
      <xsl:call-template name="emit-syntax-lines">
        <xsl:with-param name="text" select="$text"/>
        <xsl:with-param name="left" select="$max-syntax-lines"/>
        <xsl:with-param name="indent" select="string-length($first-line) - string-length($first-trimmed)"/>
      </xsl:call-template>
      <xsl:text>```&#10;&#10;</xsl:text>
    </xsl:if>
  </xsl:template>

  <!-- One form per line, blank lines dropped, cut after $left forms. -->
  <xsl:template name="first-non-blank-line">
    <xsl:param name="text"/>
    <xsl:variable name="line" select="substring-before(concat($text, '&#10;'), '&#10;')"/>
    <xsl:choose>
      <xsl:when test="normalize-space($line) != '' or not(contains($text, '&#10;'))">
        <xsl:value-of select="$line"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="first-non-blank-line">
          <xsl:with-param name="text" select="substring-after($text, '&#10;')"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="emit-syntax-lines">
    <xsl:param name="text"/>
    <xsl:param name="left"/>
    <xsl:param name="indent"/>
    <xsl:if test="normalize-space($text) != ''">
      <xsl:variable name="line">
        <xsl:choose>
          <xsl:when test="contains($text, '&#10;')">
            <xsl:value-of select="substring-before($text, '&#10;')"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$text"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <xsl:variable name="rest" select="substring-after($text, '&#10;')"/>
      <xsl:choose>
        <xsl:when test="normalize-space($line) = ''">
          <xsl:call-template name="emit-syntax-lines">
            <xsl:with-param name="text" select="$rest"/>
            <xsl:with-param name="left" select="$left"/>
            <xsl:with-param name="indent" select="$indent"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:when test="$left = 0">
          <xsl:text>...&#10;</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:call-template name="trim-leading">
            <xsl:with-param name="text" select="$line"/>
            <xsl:with-param name="max" select="$indent"/>
          </xsl:call-template>
          <xsl:text>&#10;</xsl:text>
          <xsl:call-template name="emit-syntax-lines">
            <xsl:with-param name="text" select="$rest"/>
            <xsl:with-param name="left" select="$left - 1"/>
            <xsl:with-param name="indent" select="$indent"/>
          </xsl:call-template>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:if>
  </xsl:template>

  <!-- ================================================================ -->
  <!-- arguments: `name` — first sentence of the description            -->
  <!-- ================================================================ -->

  <xsl:template match="d:varlistentry">
    <xsl:param name="shown-words" select="''"/>
    <xsl:variable name="shown">
      <xsl:choose>
        <xsl:when test="$shown-words = ''">yes</xsl:when>
        <xsl:otherwise>
          <xsl:call-template name="any-word-in">
            <xsl:with-param name="words" select="normalize-space(translate(d:term, $separators, $blanks))"/>
            <xsl:with-param name="text" select="$shown-words"/>
          </xsl:call-template>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:if test="$shown = 'yes'">
      <xsl:call-template name="emit-argument"/>
    </xsl:if>
  </xsl:template>

  <!-- "yes" when one of the blank-separated words is a whole word of $text (blank padded). -->
  <xsl:template name="any-word-in">
    <xsl:param name="words"/>
    <xsl:param name="text"/>
    <xsl:variable name="word" select="substring-before(concat($words, ' '), ' ')"/>
    <xsl:choose>
      <xsl:when test="$word = ''"/>
      <xsl:when test="contains($text, concat(' ', $word, ' '))">yes</xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="any-word-in">
          <xsl:with-param name="words" select="substring-after($words, ' ')"/>
          <xsl:with-param name="text" select="$text"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- One line each, no bullet: the trailing two blanks break the line. -->
  <xsl:template name="emit-argument">
    <xsl:text>`</xsl:text>
    <xsl:value-of select="normalize-space(d:term)"/>
    <xsl:text>`</xsl:text>
    <xsl:variable name="text">
      <xsl:apply-templates select="(d:listitem/d:para | d:listitem/d:itemizedlist/d:listitem)[1]/node()" mode="inline-text"/>
    </xsl:variable>
    <xsl:variable name="sentence">
      <xsl:call-template name="first-sentence">
        <xsl:with-param name="text" select="normalize-space($text)"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:if test="$sentence != ''">
      <xsl:text> — </xsl:text>
      <xsl:value-of select="$sentence"/>
    </xsl:if>
    <xsl:text>  &#10;</xsl:text>
  </xsl:template>

  <!--
    Up to the first ". " that ends a sentence of some length, so that
    "e.g. " or "i.e. " early in the text do not cut it.
  -->
  <xsl:template name="first-sentence">
    <xsl:param name="text"/>
    <xsl:param name="done" select="''"/>
    <xsl:choose>
      <xsl:when test="contains($text, '. ')">
        <xsl:variable name="head" select="concat($done, substring-before($text, '. '), '.')"/>
        <xsl:choose>
          <xsl:when test="string-length($head) &gt;= 25">
            <xsl:value-of select="$head"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="first-sentence">
              <xsl:with-param name="text" select="substring-after($text, '. ')"/>
              <xsl:with-param name="done" select="concat($head, ' ')"/>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="concat($done, $text)"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- ================================================================ -->
  <!-- inline markup                                                    -->
  <!-- ================================================================ -->

  <!-- Spaces come from the text nodes themselves: none is added around markup. -->
  <xsl:template match="text()" mode="inline-text">
    <xsl:call-template name="escape-markdown-tags">
      <xsl:with-param name="text" select="translate(., '&#9;&#10;&#13;', '   ')"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="d:literal | d:varname | d:function | d:code | d:constant | d:option | d:command"
                mode="inline-text">
    <xsl:text>`</xsl:text>
    <xsl:value-of select="normalize-space(.)"/>
    <xsl:text>`</xsl:text>
  </xsl:template>

  <xsl:template match="d:emphasis" mode="inline-text">
    <xsl:text>*</xsl:text>
    <xsl:value-of select="normalize-space(.)"/>
    <xsl:text>*</xsl:text>
  </xsl:template>

  <xsl:template match="d:link" mode="inline-text">
    <xsl:variable name="linkend" select="normalize-space(@linkend)"/>
    <xsl:variable name="label">
      <xsl:choose>
        <xsl:when test="normalize-space(.) != ''">
          <xsl:value-of select="normalize-space(.)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$linkend"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="href">
      <xsl:choose>
        <xsl:when test="$linkend != ''">
          <xsl:value-of select="concat('scilab-help://', $linkend)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="normalize-space(@xlink:href)"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="$href != ''">
        <xsl:value-of select="concat('[', $label, '](', $href, ')')"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$label"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- Nested lists and code blocks do not fit on one line. -->
  <xsl:template match="d:itemizedlist | d:orderedlist | d:programlisting | d:screen | d:table | d:informaltable"
                mode="inline-text"/>

  <xsl:template match="*" mode="inline-text">
    <xsl:apply-templates select="node()" mode="inline-text"/>
  </xsl:template>

  <!--
    The help writes its placeholders between angle brackets, as in
    <nom_de_la_fonction>. Left as they are, a markdown reader takes them for
    HTML tags and drops them. Not for code spans or the syntax block, where a
    reader would show the escape itself.
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

  <!-- At most $max leading blanks; all of them by default. -->
  <xsl:template name="trim-leading">
    <xsl:param name="text"/>
    <xsl:param name="max" select="-1"/>
    <xsl:choose>
      <xsl:when test="$max != 0 and (starts-with($text, ' ') or starts-with($text, '&#9;'))">
        <xsl:call-template name="trim-leading">
          <xsl:with-param name="text" select="substring($text, 2)"/>
          <xsl:with-param name="max" select="$max - 1"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$text"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
</xsl:stylesheet>
