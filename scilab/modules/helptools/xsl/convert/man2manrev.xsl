<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:output method="xml" version="1.0" encoding="ISO-8859-1" indent="no" 
              cdata-section-elements="VERBATIM EXAMPLE" />

  <xsl:template match="USED_FUNCTIONS | 
                       PARAM_DESCRIPTION |
                       DESCRIPTION |
                       DESCRIPTION_ITEM |
                       BIBLIO |
                       ITEM">
    <xsl:element name="{ name(.) }">
      <xsl:apply-templates select="@*" />

      <xsl:call-template name="wrap-mixed-content">
        <xsl:with-param name="nodes" select="node()" />
      </xsl:call-template>
    </xsl:element>
  </xsl:template>

  <xsl:template name="wrap-mixed-content">
    <xsl:param name="nodes" />

    <xsl:if test="count($nodes) &gt; 0">
      <xsl:variable name="last">
        <xsl:call-template name="find-last-mixed">
          <xsl:with-param name="nodes" select="$nodes" />
          <xsl:with-param name="index" select="0" />
        </xsl:call-template>
      </xsl:variable>

      <xsl:choose>
        <xsl:when test="$last &gt;= 1">
          <xsl:variable name="range" select="$nodes[position() &lt;= $last]" />

          <xsl:if test="count($range) &gt; 1 or 
                        $range[1]/node() or 
                        normalize-space($range[1])">
            <P><xsl:apply-templates select="$range" /></P>
          </xsl:if>

          <xsl:call-template name="wrap-mixed-content">
            <xsl:with-param name="nodes" 
                            select="$nodes[position() &gt; $last]" />
          </xsl:call-template>
        </xsl:when>

        <xsl:otherwise>
         <xsl:apply-templates select="$nodes[1]" />

          <xsl:call-template name="wrap-mixed-content">
            <xsl:with-param name="nodes" select="$nodes[position() &gt; 1]" />
          </xsl:call-template>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:if>
  </xsl:template>

  <xsl:template name="find-last-mixed">
    <xsl:param name="nodes" />
    <xsl:param name="index" />

    <xsl:variable name="name" select="name($nodes[1])" />

    <xsl:choose>
      <xsl:when test="count($nodes) &gt; 0 and
                      ($name = '' or 
                       $name = 'VERB' or
                       $name = 'EM' or
                       $name = 'BD' or
                       $name = 'TT' or
                       $name = 'A' or
                       $name = 'LINK')">
        <xsl:call-template name="find-last-mixed">
          <xsl:with-param name="nodes" select="$nodes[position() &gt; 1]" />
          <xsl:with-param name="index" select="$index + 1" />
        </xsl:call-template>
      </xsl:when>

      <xsl:otherwise>
        <xsl:value-of select="$index" />
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- Flatten these elements. -->

  <xsl:template match="CALLING_SEQUENCE/PP |
                       SEE_ALSO_ITEM/P">
    <xsl:value-of select="string(.)" />
  </xsl:template>

  <!-- Check these elements. -->

  <xsl:template match="ITEMIZE">
    <xsl:if test="./*[name(.) != 'ITEM'] or 
                  ./text()[normalize-space(.)]">
      <xsl:call-template name="unsupported-child-element">
        <xsl:with-param name="parent" select="." />
        <xsl:with-param name="supported" select="'ITEM'" />
      </xsl:call-template>
    </xsl:if>

    <xsl:element name="{ name(.) }">
      <xsl:apply-templates select="@*" />

      <xsl:apply-templates select="./ITEM" />
    </xsl:element>
  </xsl:template>

  <xsl:template match="AUTHORS">
    <xsl:if test="./*[name(.) != 'AUTHORS_ITEM'] or 
                  ./text()[normalize-space(.)]">
      <xsl:call-template name="unsupported-child-element">
        <xsl:with-param name="parent" select="." />
        <xsl:with-param name="supported" select="'AUTHORS_ITEM'" />
      </xsl:call-template>
    </xsl:if>

    <xsl:element name="{ name(.) }">
      <xsl:apply-templates select="@*" />

      <xsl:apply-templates select="./AUTHORS_ITEM" />
    </xsl:element>
  </xsl:template>

  <xsl:template match="DESCRIPTION_INDENT">
    <xsl:if test="./*[name(.) != 'DESCRIPTION_ITEM'] or 
                  ./text()[normalize-space(.)]">
      <xsl:call-template name="unsupported-child-element">
        <xsl:with-param name="parent" select="." />
        <xsl:with-param name="supported" select="'DESCRIPTION_ITEM'" />
      </xsl:call-template>
    </xsl:if>

    <xsl:element name="{ name(.) }">
      <xsl:apply-templates select="@*" />

      <xsl:apply-templates select="./DESCRIPTION_ITEM" />
    </xsl:element>
  </xsl:template>

  <xsl:template match="PARAM_ITEM">
    <xsl:if test="./*[name(.) != 'PARAM_NAME' and 
                      name(.) != 'PARAM_DESCRIPTION']">
      <xsl:call-template name="unsupported-child-element">
        <xsl:with-param name="parent" select="." />
        <xsl:with-param name="supported" 
         select="'PARAM_NAME and PARAM_DESCRIPTION'" />
      </xsl:call-template>
    </xsl:if>

    <xsl:element name="{ name(.) }">
      <xsl:apply-templates select="@*" />

      <xsl:apply-templates select="./*[name(.) = 'PARAM_NAME' or
                                       name(.) = 'PARAM_DESCRIPTION']" />
    </xsl:element>
  </xsl:template>

  <xsl:template match="PARAM_INDENT">
    <xsl:if test="./*[name(.) != 'PARAM_ITEM'] or 
                  ./text()[normalize-space(.)]">
      <xsl:call-template name="unsupported-child-element">
        <xsl:with-param name="parent" select="." />
        <xsl:with-param name="supported" select="'PARAM_ITEM'" />
      </xsl:call-template>
    </xsl:if>

    <xsl:element name="{ name(.) }">
      <xsl:apply-templates select="@*" />

      <xsl:apply-templates select="./PARAM_ITEM" />
    </xsl:element>
  </xsl:template>

  <xsl:template match="PARAM">
    <xsl:if test="./*[name(.) != 'PARAM_INDENT'] or 
                  ./text()[normalize-space(.)]">
      <xsl:call-template name="unsupported-child-element">
        <xsl:with-param name="parent" select="." />
        <xsl:with-param name="supported" select="'PARAM_INDENT'" />
      </xsl:call-template>
    </xsl:if>

    <xsl:element name="{ name(.) }">
      <xsl:apply-templates select="@*" />

      <xsl:apply-templates select="./PARAM_INDENT" />
    </xsl:element>
  </xsl:template>

  <!-- By default, copy everything. -->

  <xsl:template match="@*|node()">
    <xsl:copy>
      <xsl:apply-templates select="@*|node()" />
    </xsl:copy>
  </xsl:template>

  <!-- Helpers =========================================================== -->

  <xsl:template name="unsupported-child-element">
    <xsl:param name="parent" />
    <xsl:param name="supported" />

    <xsl:message terminate="no">
      <xsl:text>WARNING: element </xsl:text>
      <xsl:value-of select="name($parent)" />
      <xsl:text> contains some text or child elements other than </xsl:text>
      <xsl:value-of select="$supported" />
      <xsl:text>.</xsl:text>
    </xsl:message>
  </xsl:template>

</xsl:stylesheet>
