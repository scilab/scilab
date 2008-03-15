<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:xlink="http://www.w3.org/1999/xlink"
  xmlns:svg="http://www.w3.org/2000/svg"
  xmlns:mml="http://www.w3.org/1998/Math/MathML"
  xmlns="http://docbook.org/ns/docbook"
  xmlns:db="http://docbook.org/ns/docbook">

  <xsl:output method="xml" version="1.0" encoding="ISO-8859-1" indent="no" 
              cdata-section-elements="programlisting" />

  <xsl:strip-space elements="CALLING_SEQUENCE" />

  <xsl:template match="MAN">
    <refentry version="5.0-subset Scilab">
      <xsl:attribute name="xml:lang">
        <xsl:call-template name="to-xml-lang">
          <xsl:with-param name="lang" select="./LANGUAGE" />
        </xsl:call-template>
      </xsl:attribute>

      <xsl:variable name="description-count" 
                    select="count(./SHORT_DESCRIPTION)" />

      <xsl:variable name="title">
        <xsl:call-template name="check-id">
          <xsl:with-param name="id" select="./TITLE" />
        </xsl:call-template>
      </xsl:variable>

      <xsl:attribute name="xml:id">
        <xsl:choose>
          <xsl:when test="$description-count = 1">
            <xsl:call-template name="check-id">
              <xsl:with-param name="id" select="./SHORT_DESCRIPTION/@name" />
            </xsl:call-template>
          </xsl:when>

          <xsl:otherwise>
            <xsl:value-of select="$title" />
          </xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>

      <xsl:if test="./DATE|./KEYWORD">
        <info>
          <xsl:if test="./DATE">
            <pubdate><xsl:apply-templates mode="info" 
                      select="./DATE" /></pubdate>
          </xsl:if>

          <xsl:if test="./KEYWORD">
            <keywordset>
              <keyword><xsl:apply-templates mode="info" 
                        select="./KEYWORD" /></keyword>
            </keywordset>
          </xsl:if>
        </info>
      </xsl:if>

      <xsl:for-each select="./SHORT_DESCRIPTION">
        <xsl:variable name="name">
          <xsl:call-template name="check-id">
            <xsl:with-param name="id" select="./@name" />
          </xsl:call-template>
        </xsl:variable>

        <refnamediv>
          <xsl:if test="$description-count &gt; 1 and $name != $title">
            <xsl:attribute name="xml:id">
              <xsl:value-of select="$name" />
            </xsl:attribute>
          </xsl:if>

          <refname><xsl:value-of select="./@name" /></refname>
          <refpurpose>
            <xsl:apply-templates select="." mode="refnamediv" />
          </refpurpose>
        </refnamediv>
      </xsl:for-each>

      <xsl:apply-templates />
    </refentry>
  </xsl:template>

  <xsl:template match="DATE" mode="info">
    <xsl:apply-templates />
  </xsl:template>

  <xsl:template match="KEYWORD" mode="info">
    <xsl:apply-templates />
  </xsl:template>

  <xsl:template match="SHORT_DESCRIPTION" mode="refnamediv">
    <xsl:apply-templates mode="flatten" />
  </xsl:template>

  <xsl:template match="P|SP|PP" mode="flatten">
    <xsl:apply-templates select="./node()" />
  </xsl:template>

  <xsl:template match="META" />
  <xsl:template match="LANGUAGE" />
  <xsl:template match="TITLE" />
  <xsl:template match="TYPE" />
  <xsl:template match="DATE" />
  <xsl:template match="KEYWORD" />
  <xsl:template match="SHORT_DESCRIPTION" />

  <!-- Section-like elements ============================================= -->

  <xsl:template match="CALLING_SEQUENCE">
    <refsynopsisdiv>
      <title>
        <xsl:call-template name="localized-message">
          <xsl:with-param name="msg" select="name(.)" />
        </xsl:call-template>
      </title>

      <synopsis>
        <xsl:apply-templates select="./CALLING_SEQUENCE_ITEM[1]" />
        <xsl:for-each select="./CALLING_SEQUENCE_ITEM[position() &gt; 1]">
          <xsl:text>&#xA;</xsl:text>
          <xsl:apply-templates select="." />
        </xsl:for-each>
      </synopsis>
    </refsynopsisdiv>
  </xsl:template>

  <xsl:template match="CALLING_SEQUENCE_ITEM">
    <xsl:value-of select="normalize-space(string(.))" />
  </xsl:template>

  <xsl:template match="PARAM">
    <xsl:if test="./*">
      <xsl:call-template name="unsupported-attribute">
        <xsl:with-param name="name" select="concat(name(.), '/@target')" />
        <xsl:with-param name="value" select="./@target" />
      </xsl:call-template>

      <refsection>
        <title>
          <xsl:call-template name="localized-message">
            <xsl:with-param name="msg" select="name(.)" />
          </xsl:call-template>
        </title>

        <xsl:apply-templates />
      </refsection>
    </xsl:if>
  </xsl:template>

  <xsl:template match="PARAM_INDENT">
    <variablelist>
      <xsl:apply-templates />
    </variablelist>
  </xsl:template>

  <xsl:template match="PARAM_ITEM">
    <varlistentry>
      <xsl:apply-templates />
    </varlistentry>
  </xsl:template>

  <xsl:template match="PARAM_NAME">
    <term><xsl:apply-templates /></term>
  </xsl:template>

  <xsl:template match="PARAM_DESCRIPTION">
    <listitem>
      <xsl:apply-templates />
    </listitem>
  </xsl:template>

  <xsl:template match="DESCRIPTION">
    <xsl:if test="./*">
      <refsection>
        <title>
          <xsl:call-template name="localized-message">
            <xsl:with-param name="msg" select="name(.)" />
          </xsl:call-template>
        </title>

        <xsl:apply-templates />
      </refsection>
    </xsl:if>
  </xsl:template>

  <xsl:template match="DESCRIPTION_INDENT">
    <xsl:if test="./DESCRIPTION_ITEM/*">
      <xsl:choose>
        <xsl:when test="./DESCRIPTION_ITEM/@label and 
                        normalize-space(./DESCRIPTION_ITEM/@label) != '*' and 
                        normalize-space(./DESCRIPTION_ITEM/@label) != '-'">
          <variablelist><xsl:apply-templates 
                         mode="variablelist" /></variablelist>
        </xsl:when>

        <xsl:otherwise>
          <itemizedlist><xsl:apply-templates /></itemizedlist>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:if>
  </xsl:template>

  <xsl:template match="DESCRIPTION_ITEM|ITEM" mode="variablelist">
    <xsl:if test="./*">
      <varlistentry>
        <term><xsl:value-of select="@label" /></term>
        <listitem><xsl:apply-templates /></listitem>
      </varlistentry>
    </xsl:if>
  </xsl:template>

  <xsl:template match="DESCRIPTION_ITEM|ITEM">
    <xsl:if test="./*">
      <listitem><xsl:apply-templates /></listitem>
    </xsl:if>
  </xsl:template>

  <xsl:template match="SECTION">
    <xsl:if test="./*">
      <refsection>
        <title><xsl:value-of select="@label" /></title>

        <xsl:apply-templates />
      </refsection>
    </xsl:if>
  </xsl:template>

  <xsl:template match="EXAMPLE">
    <refsection>
      <title>
        <xsl:call-template name="localized-message">
          <xsl:with-param name="msg" select="name(.)" />
        </xsl:call-template>
      </title>

      <programlisting role="example"><xsl:apply-templates /></programlisting>
    </refsection>
  </xsl:template>

  <xsl:template match="SEE_ALSO">
    <refsection>
      <title>
        <xsl:call-template name="localized-message">
          <xsl:with-param name="msg" select="name(.)" />
        </xsl:call-template>
      </title>

      <simplelist type="inline"><xsl:apply-templates /></simplelist>
    </refsection>
  </xsl:template>

  <xsl:template match="SEE_ALSO_ITEM">
    <member><xsl:apply-templates /></member>
  </xsl:template>

  <xsl:template match="AUTHORS">
    <refsection>
      <title>
        <xsl:call-template name="localized-message">
          <xsl:with-param name="msg" select="name(.)" />
        </xsl:call-template>
      </title>

      <xsl:choose>
        <xsl:when test="./AUTHORS_ITEM/@label">
          <variablelist><xsl:apply-templates 
                         mode="variablelist" /></variablelist>
        </xsl:when>

        <xsl:otherwise>
          <simplelist type="vert"><xsl:apply-templates /></simplelist>
        </xsl:otherwise>
      </xsl:choose>
    </refsection>
  </xsl:template>

  <xsl:template match="AUTHORS_ITEM" mode="variablelist">
    <varlistentry>
      <term><xsl:value-of select="@label" /></term>
      <listitem><para><xsl:apply-templates /></para></listitem>
    </varlistentry>
  </xsl:template>

  <xsl:template match="AUTHORS_ITEM">
    <member><xsl:apply-templates mode="flatten" /></member>
  </xsl:template>

  <xsl:template match="AUTHOR">
    <refsection>
      <title>
        <xsl:call-template name="localized-message">
          <xsl:with-param name="msg" select="name(.)" />
        </xsl:call-template>
      </title>

      <para><xsl:apply-templates /></para>
    </refsection>
  </xsl:template>

  <xsl:template match="BIBLIO|USED_FUNCTIONS">
    <xsl:if test="./*">
      <refsection>
        <title>
          <xsl:call-template name="localized-message">
            <xsl:with-param name="msg" select="name(.)" />
          </xsl:call-template>
        </title>

        <xsl:apply-templates />
      </refsection>
    </xsl:if>
  </xsl:template>

  <!-- Block-level elements ============================================== -->

  <xsl:template match="P|SP|PP">
    <para><xsl:apply-templates /></para>
  </xsl:template>

  <xsl:template match="P/text()[1]|SP/text()[1]|PP/text()[1]">
    <xsl:variable name="text" select="string(.)" />

    <xsl:choose>
      <xsl:when test="starts-with(normalize-space($text), ': ')">
        <xsl:value-of select="substring-after($text, ': ')" />
      </xsl:when>

      <xsl:otherwise>
        <xsl:value-of select="$text" />
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="VERBATIM">
    <programlisting><xsl:apply-templates /></programlisting>
  </xsl:template>

  <xsl:template match="ITEMIZE">
    <xsl:if test="./ITEM/*">
      <xsl:call-template name="unsupported-attribute">
        <xsl:with-param name="name" select="concat(name(.), '/@label')" />
        <xsl:with-param name="value" select="./@label" />
      </xsl:call-template>

      <xsl:choose>
        <xsl:when test="./ITEM/@label and 
                        normalize-space(./ITEM/@label) != '*' and 
                        normalize-space(./ITEM/@label) != '-'">
          <variablelist><xsl:apply-templates 
                         mode="variablelist" /></variablelist>
        </xsl:when>

        <xsl:otherwise>
          <itemizedlist><xsl:apply-templates /></itemizedlist>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:if>
  </xsl:template>

  <xsl:template match="TABLE">
    <informaltable border="1"><xsl:apply-templates /></informaltable>
  </xsl:template>

  <xsl:template match="TABLE_NEW_ROW">
    <tr><xsl:apply-templates /></tr>
  </xsl:template>

  <xsl:template match="TABLE_NEW_COLUMN">
    <td><xsl:apply-templates /></td>
  </xsl:template>

  <!-- Inline-level elements ============================================= -->

  <xsl:template match="A">
    <link xlink:href="{@href}"><xsl:apply-templates /></link>
  </xsl:template>

  <xsl:template match="LINK">
    <xsl:variable name="target">
      <xsl:call-template name="check-id">
        <xsl:with-param name="id" select="." />
      </xsl:call-template>
    </xsl:variable>

    <link linkend="{$target}"><xsl:apply-templates /></link>
  </xsl:template>

  <xsl:template match="IMG">
    <inlinemediaobject>
      <imageobject>
        <imagedata fileref="{@src}" />
      </imageobject>
    </inlinemediaobject>
  </xsl:template>

  <xsl:template match="VERB|TT">
    <literal><xsl:apply-templates /></literal>
  </xsl:template>

  <xsl:template match="EM|MATH">
    <emphasis><xsl:apply-templates /></emphasis>
  </xsl:template>

  <xsl:template match="BD">
    <emphasis role="bold"><xsl:apply-templates /></emphasis>
  </xsl:template>

  <!-- Unsupported elements and attributes =============================== -->

  <xsl:template match="*">
    <xsl:message terminate="no">
      <xsl:text>WARNING: unsupported element "</xsl:text>
      <xsl:value-of select="name(.)"/>
      <xsl:text>".</xsl:text>
    </xsl:message>
  </xsl:template>

  <xsl:template name="unsupported-attribute">
    <xsl:param name="name" select="''" />
    <xsl:param name="value" select="''" />

    <xsl:if test="$value != ''">
      <xsl:message terminate="no">
        <xsl:text>WARNING: unsupported attribute "</xsl:text>
        <xsl:value-of select="$name" />
        <xsl:text>" (its value is "</xsl:text>
        <xsl:value-of select="$value" />
        <xsl:text>").</xsl:text>
      </xsl:message>
    </xsl:if>
  </xsl:template>

  <!-- Helpers =========================================================== -->

  <xsl:template name="localized-message">
    <xsl:param name="msg" select="''" />

    <xsl:variable name="lang">
      <xsl:call-template name="to-xml-lang">
        <xsl:with-param name="lang" select="/MAN/LANGUAGE" />
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="$lang = 'fr'">
        <xsl:choose>
          <xsl:when 
           test="$msg = 'CALLING_SEQUENCE'">Séquence d'appel</xsl:when>
          <xsl:when test="$msg = 'PARAM'">Paramètres</xsl:when>
          <xsl:when test="$msg = 'DESCRIPTION'">Description</xsl:when>
          <xsl:when test="$msg = 'EXAMPLE'">Exemples</xsl:when>
          <xsl:when test="$msg = 'SEE_ALSO'">Voir Aussi</xsl:when>
          <xsl:when test="$msg = 'AUTHOR'">Auteurs</xsl:when>
          <xsl:when test="$msg = 'AUTHORS'">Auteurs</xsl:when>
          <xsl:when test="$msg = 'BIBLIO'">Bibliographie</xsl:when>
          <xsl:when
           test="$msg = 'USED_FUNCTIONS'">Fonctions Utilisées</xsl:when>
          <xsl:otherwise>???</xsl:otherwise>
        </xsl:choose>
      </xsl:when>

      <xsl:otherwise>
        <xsl:choose>
          <xsl:when 
           test="$msg = 'CALLING_SEQUENCE'">Calling Sequence</xsl:when>
          <xsl:when test="$msg = 'PARAM'">Parameters</xsl:when>
          <xsl:when test="$msg = 'DESCRIPTION'">Description</xsl:when>
          <xsl:when test="$msg = 'EXAMPLE'">Examples</xsl:when>
          <xsl:when test="$msg = 'SEE_ALSO'">See Also</xsl:when>
          <xsl:when test="$msg = 'AUTHOR'">Authors</xsl:when>
          <xsl:when test="$msg = 'AUTHORS'">Authors</xsl:when>
          <xsl:when test="$msg = 'BIBLIO'">Bibliography</xsl:when>
          <xsl:when test="$msg = 'USED_FUNCTIONS'">Used Functions</xsl:when>
          <xsl:otherwise>???</xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="to-xml-lang">
    <xsl:param name="lang" select="'eng'" />

    <xsl:choose>
      <xsl:when test="$lang = 'fr'">fr</xsl:when>
      <xsl:otherwise>en</xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="check-id">
    <xsl:param name="id" select="''" />

    <xsl:variable name="apos">&apos;</xsl:variable>
    <xsl:variable name="quot">&quot;</xsl:variable>

    <xsl:value-of
     select="translate(normalize-space($id), 
                       concat($quot, $apos, '% ()\/&amp;|^~[]{}!?#$=-+*;,:.'), 
                       '')" />
  </xsl:template>

</xsl:stylesheet>
