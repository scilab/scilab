<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="html" indent="no" encoding="ISO-8859-1" />
<xsl:template match="MAN">
<html>
<head>
<title><xsl:for-each select="./SHORT_DESCRIPTION">
<xsl:value-of select="@name"/>
</xsl:for-each>
</title>
</head>

<body bgcolor="FFFFFF">
  <CENTER><xsl:value-of select="./TYPE"/></CENTER>
<xsl:for-each select="./SHORT_DESCRIPTION">
  <p><H2><xsl:value-of select="@name"/> - <xsl:value-of select="text()"/></H2></p>
</xsl:for-each>

<xsl:if test="./CALLING_SEQUENCE">
	<H3><font color="blue">Calling Sequence</font></H3>
	<xsl:apply-templates select="CALLING_SEQUENCE"/>
</xsl:if>

<xsl:if test="./PARAM">
	<H3><font color="blue">Parameters</font></H3>
	<xsl:apply-templates select="PARAM"/>
</xsl:if>

<xsl:if test="./DESCRIPTION">
	<H3><font color="blue">Description</font></H3>
	<xsl:apply-templates select="DESCRIPTION"/>
</xsl:if>

<xsl:if test="./ITEMIZE">
	<xsl:apply-templates select="ITEMIZE"/>
</xsl:if>


<!-- A corriger 
<xsl:if test="./EXAMPLE">
	<H3><font color="blue"><a><xsl:attribute name="href">toscilab.html</xsl:attribute>
	<xsl:attribute name="title"><xsl:value-of select="./EXAMPLE/self::node()" /></xsl:attribute>Examples</a>
	</font></H3>
	<dl><table bgcolor="lightblue"><tr><td><pre><xsl:value-of select="./EXAMPLE/self::node()"/></pre></td></tr></table></dl>
</xsl:if>
--> 


<xsl:if test="./EXAMPLE">
	<H3><font color="blue">Examples</font></H3>
	<dl><table bgcolor="lightblue"><tr><td><pre><xsl:value-of select="./EXAMPLE/self::node()"/></pre></td></tr></table></dl>
</xsl:if>

<xsl:if test="./SEE_ALSO/SEE_ALSO_ITEM">
	<H3><font color="blue">See Also</font></H3>
	<p><ul><xsl:for-each  select="./SEE_ALSO/SEE_ALSO_ITEM"><xsl:apply-templates select="A"/><xsl:text disable-output-escaping="yes">&amp;nbsp;</xsl:text><xsl:text disable-output-escaping="yes">&amp;nbsp;</xsl:text></xsl:for-each></ul></p>
</xsl:if>

<xsl:if test="./AUTHOR">
	<H3><font color="blue">Author</font></H3>
	<p><ul><xsl:value-of select="./AUTHOR"/></ul></p>
</xsl:if>

<xsl:if test="./AUTHORS">
	<H3><font color="blue">Authors</font></H3>
	<p><ul><xsl:value-of select="./AUTHORS"/></ul></p>
</xsl:if>

<xsl:if test="./USED_FUNCTION">
	<H3><font color="blue">Used Function</font></H3>
	<p><xsl:value-of select="./USED_FUNCTION"/></p>
</xsl:if>

<xsl:if test="./BIBLIO">
	<H3><font color="blue">Bibliography</font></H3>
	<p><xsl:value-of select="./BIBLIO"/></p>
</xsl:if>

</body>
</html>
</xsl:template>

<xsl:template match="PARAM">
	<ul><xsl:apply-templates select="PARAM_ITEM|PARAM_INDENT|ITEMIZE|P|TABLE|VERBATIM"/></ul>
</xsl:template>

<xsl:template match="PARAM[@target='in']">
	<ul><li><H3><font color="blue">Input Parameters</font></H3>
	<ul><xsl:apply-templates select="PARAM_ITEM|PARAM_INDENT|ITEMIZE|P|TABLE|VERBATIM"/></ul>
	</li></ul>
</xsl:template>

<xsl:template match="PARAM[@target='out']">
	<ul><li><H3><font color="blue">Output Parameters</font></H3>
		<ul><xsl:apply-templates select="PARAM_ITEM|PARAM_INDENT|ITEMIZE|P|TABLE|VERBATIM"/></ul>
	</li>
	</ul>
</xsl:template>

<xsl:template match="PARAM_ITEM">
	<li><xsl:apply-templates select="PARAM_NAME|PARAM_ITEM|PARAM_INDENT|PARAM_DESCRIPTION"/></li>
</xsl:template>

<xsl:template match="PARAM_DESCRIPTION">
	<xsl:apply-templates select="P|VERBATIM|TABLE|VERB|EM|BD|TT|PARAM_INDENT|text()"/>
</xsl:template>

<xsl:template match="PARAM_INDENT">
	<ul><xsl:apply-templates select="PARAM_ITEM|PARAM_INDENT|P|VERBATIM|TABLE|VERB|EM|BD|TT|text()"/></ul>
</xsl:template>


<xsl:template match="PARAM_NAME">
	<tt><b><xsl:value-of select="text()"/></b></tt>
</xsl:template>


<xsl:template match="CALLING_SEQUENCE">
	<dl><xsl:apply-templates select="CALLING_SEQUENCE_ITEM"/></dl>
</xsl:template>

<xsl:template match="CALLING_SEQUENCE_ITEM">
	<dd><tt><xsl:value-of select="text()"/></tt></dd>
</xsl:template>

<xsl:template match="ITEM">
	<li><b><font color="maroon"><xsl:value-of select="@label"/></font></b><xsl:apply-templates select="ITEMIZE|TABLE|P|VERBATIM|VERB|EM|BD|TT|A|text()"/></li>
</xsl:template>

<xsl:template match="ITEMIZE">
	<H3><font color="blue"><xsl:value-of select="@label"/></font></H3>
	<ul><xsl:apply-templates select="ITEM|ITEMIZE|TABLE|P|VERBATIM|VERB|EM|BD|TT|A"/></ul>
</xsl:template>

<xsl:template match="TABLE">
        <div align="center">
	<table border="2">
	<xsl:for-each select="./TABLE_NEW_ROW">
	<tr align="center">
		<xsl:for-each select="./TABLE_NEW_COLUMN">
			<td>
			<xsl:value-of select="./text()"/>
			</td>	
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

<xsl:template match="BD">
	<b><xsl:value-of select="./text()"/></b>
</xsl:template>

<xsl:template match="A">
	<a><xsl:attribute name="href"><xsl:value-of select='@href'/></xsl:attribute><xsl:apply-templates select="VERB|text()"/></a>
</xsl:template>

<xsl:template match="DESCRIPTION">
	<dl><xsl:apply-templates select="DESCRIPTION_ITEM|DESCRIPTION_INDENT|P|VERBATIM|VERB|EM|BD|TT|TABLE|A|text()"/></dl>
</xsl:template>

<xsl:template match="DESCRIPTION_ITEM">
	<li><b><xsl:value-of select="@label"/></b><xsl:apply-templates select="DESCRIPTION_INDENT|DESCRIPTION_ITEM|P|VERBATIM|VERB|EM|BD|TT|TABLE|ITEMIZE|A|text()"/></li>
</xsl:template>

<xsl:template match="P">
	<p><xsl:apply-templates select="VERB|EM|BD|TT|A|text()"/></p>
</xsl:template>

<xsl:template match="VERBATIM">
	<pre><xsl:value-of select="self::node()"/></pre>
</xsl:template>

<xsl:template match="AUTHORS">
<ul>
<xsl:apply-templates select="AUTHORS_ITEM|P|VERBATIM|TABLE|A|text()"/>
</ul>
</xsl:template>

<xsl:template match="AUTHORS_ITEM">
<li>
<b><xsl:value-of select="@label"/>  </b>,
<xsl:apply-templates select="AUTHORS_ITEM|P|VERBATIM|VERB|EM|BD|TT|TABLE|ITEMIZE|A|text()"/>
</li>
</xsl:template>

</xsl:stylesheet>
