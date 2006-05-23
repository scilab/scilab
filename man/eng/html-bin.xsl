<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="html" indent="yes"  />

<!-- 
	This xsl file is only use when scilab sources are extracted with the 
	"svn export" command ( not with the "svn checkout" command ).
	If, it's the case, we can display the "last update date".
-->

<xsl:strip-space elements="*"/>

<xsl:template match="MAN">
 <html>
 <head>
 <title><xsl:for-each select="./SHORT_DESCRIPTION"><xsl:value-of select="@name"/></xsl:for-each>
 </title>
 </head>
 <body bgcolor="#FFFFFF">
 <center><xsl:value-of select="./TYPE"/></center>
 <div align="right">Last update : <xsl:value-of select="./DATE"/></div>
 <xsl:for-each select="./SHORT_DESCRIPTION">
   <p><b><xsl:value-of select="@name"/></b> - <xsl:apply-templates/></p>
 </xsl:for-each>
 <xsl:apply-templates/>
 </body>
 </html>
</xsl:template>

<!-- ignored -->

<xsl:template match="LANGUAGE|TITLE|TYPE|DATE|NAME|KEYWORD|SHORT_DESCRIPTION"></xsl:template>

<!-- calling sequence -->

<xsl:template match="CALLING_SEQUENCE">
  <h3><font color="blue">Calling Sequence</font></h3>
  <dl>
  <xsl:for-each  select="./CALLING_SEQUENCE_ITEM">
     <dd><tt><xsl:apply-templates/></tt></dd>
  </xsl:for-each>
  </dl>
</xsl:template>

<xsl:template match="CALLING_SEQUENCE_COLOR">
  <h3><font color="blue">Calling Sequence</font></h3>
  <table width="100%" bgcolor="#eeeeee"><tr><td>
  <dl>
  <xsl:for-each  select="./CALLING_SEQUENCE_ITEM">
     <dd><tt><xsl:apply-templates/></tt></dd>
  </xsl:for-each>
  </dl>
  </td></tr></table>
</xsl:template>

<!-- parameters -->

<xsl:template match="PARAM">
   <h3><font color="blue">Parameters</font></h3>
   <xsl:apply-templates/>
</xsl:template>

<xsl:template match="PARAM[@target='in']">
   <h3><font color="blue">Input Parameters</font></h3>
   <xsl:apply-templates/>
</xsl:template>

<xsl:template match="PARAM[@target='out']">
  <h3><font color="blue">Output Parameters</font></h3>
   <xsl:apply-templates/>
</xsl:template>

<xsl:template match="PARAM_INDENT">
  <ul>
  <xsl:for-each  select="./PARAM_ITEM"><li><xsl:apply-templates/></li></xsl:for-each>
  </ul>
</xsl:template>

<xsl:template match="PARAM_ITEM"><xsl:apply-templates/></xsl:template>

<xsl:template match="PARAM_DESCRIPTION"><xsl:apply-templates/></xsl:template>

<xsl:template match="PARAM_NAME"><tt><b><xsl:value-of select="text()"/></b></tt></xsl:template>

<!-- Description -->

<xsl:template match="DESCRIPTION">
  <h3><font color="blue">Description</font></h3>
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="DESCRIPTION_INDENT">
  <dl>
  <xsl:for-each  select="./DESCRIPTION_ITEM">
     <dd><b><xsl:value-of select="@label"/></b><xsl:apply-templates/></dd>
  </xsl:for-each>
  </dl>
</xsl:template>

<!-- Other sections  -->

<xsl:template match="SECTION">
  <h3><font color="blue"><xsl:value-of select="@label"/></font></h3>
  <dl><xsl:apply-templates select="ITEMIZE|P|TABLE|A|VERBATIM|text()"/></dl>
</xsl:template>

<!-- Example  -->

<xsl:template match="EXAMPLE">
  <h3><font color="blue">Examples</font></h3>
  <pre>
  <xsl:value-of select="self::node()"/>
  </pre>
</xsl:template>

<xsl:template match="EXAMPLE_COLOR">
  <h3><font color="blue">Examples</font></h3>
  <table width="100%" bgcolor="lightblue"><tr><td><pre><xsl:value-of select="self::node()"/></pre></td></tr></table>
</xsl:template>

<!-- See also  -->

<xsl:template match="SEE_ALSO">
  <h3><font color="blue">See Also</font></h3>
  <p>
  <xsl:for-each  select="./SEE_ALSO_ITEM">
    <xsl:apply-templates select="A|LINK|text()"/>
    <xsl:text disable-output-escaping="yes">,&amp;nbsp;</xsl:text>
    <xsl:text disable-output-escaping="yes">&amp;nbsp;</xsl:text>
  </xsl:for-each>
  </p>
</xsl:template>

<!-- Authors  -->

<xsl:template match="AUTHOR">
  <h3><font color="blue">Author</font></h3>
  <p><xsl:value-of select="self::node()"/></p>
</xsl:template>

<xsl:template match="AUTHORS">
  <h3><font color="blue">Authors</font></h3>
  <dl>
  <xsl:for-each  select="./AUTHORS_ITEM">
     <dd><b><xsl:value-of select="@label"/></b><xsl:apply-templates/></dd>
  </xsl:for-each>
  </dl>
</xsl:template>

<!-- Biblio  -->

<xsl:template match="BIBLIO">
  <h3><font color="blue">Bibliography</font></h3>
  <xsl:apply-templates/>
</xsl:template>

<!-- ?  -->

<xsl:template match="USED_FUNCTIONS">
  <h3><font color="blue">Used Function</font></h3>
  <xsl:apply-templates/>
</xsl:template>

<!-- miscelanous  -->

<xsl:template match="ITEMIZE">
  <dd><xsl:apply-templates select="ITEM"/></dd>
</xsl:template>

<xsl:template match="ITEM">
  <li><b><font color="maroon"><xsl:value-of select="@label"/></font></b><xsl:apply-templates/></li>
</xsl:template>

<xsl:template match="TABLE">
  <div align="center">
  <table border="2">
  <xsl:for-each select="./TABLE_NEW_ROW">
  <tr align="center">
    <xsl:for-each select="./TABLE_NEW_COLUMN">
      <td><xsl:apply-templates/></td>
    </xsl:for-each>
        </tr>
  </xsl:for-each>
        </table>
        </div>
</xsl:template>

<xsl:template match="VERB">
  <tt><b><xsl:value-of select="./text()"/></b></tt>
</xsl:template>

<xsl:template match="EM">
  <em><xsl:value-of select="./text()"/></em>
</xsl:template>

<xsl:template match="TT">
  <tt><xsl:value-of select="./text()"/></tt>
</xsl:template>

<xsl:template match="BD"><b><xsl:value-of select="./text()"/></b></xsl:template>

<xsl:template match="A">
  <a><xsl:attribute name="href"><xsl:value-of select='@href'/></xsl:attribute>
    <xsl:apply-templates select="VERB|text()"/></a>
</xsl:template>

<xsl:template match="IMG">
  <IMG ALIGN="CENTERED"> <xsl:attribute name="src"><xsl:value-of select='@src'/></xsl:attribute>
    <xsl:apply-templates select="VERB|text()"/> </IMG>
</xsl:template>

<!-- paragraph -->
<xsl:template match="P"><p><xsl:apply-templates/></p></xsl:template>

<!-- simple paragraph -->
<xsl:template match="SP"><xsl:apply-templates/></xsl:template>

<xsl:template match="VERBATIM">
  <pre><xsl:value-of select="self::node()"/></pre>
</xsl:template>

</xsl:stylesheet>



