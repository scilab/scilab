<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="html" encoding="ISO-8859-1" />
<xsl:template match="MAN">
<html>
<body bgcolor="FFFFFF">
        <CENTER><xsl:value-of select="./TYPE"/></CENTER>
        <P></P>
	<H3>NOM </H3>

<xsl:for-each select="./SHORT_DESCRIPTION">
	<p><ul><xsl:value-of select="@name"/> - <xsl:value-of select="text()"/></ul></p>
</xsl:for-each>
<xsl:if test="./CALLING_SEQUENCE">
	<H3>SEQUENCE D'APPEL</H3>

	<xsl:apply-templates select="CALLING_SEQUENCE"/>

</xsl:if>
<xsl:if test="./PARAM">
	<H3>PARAMETRES</H3>
	<xsl:apply-templates select="PARAM"/>
</xsl:if>
<xsl:if test="./DESCRIPTION">
	<H3>DESCRIPTION</H3>

	<xsl:apply-templates select="DESCRIPTION"/>
</xsl:if>

<xsl:if test="./ITEMIZE">
	<xsl:apply-templates select="ITEMIZE"/>
</xsl:if>

<xsl:if test="./EXAMPLE">
	<H3>EXEMPLES</H3>

	<ul>
		<pre><xsl:value-of select="./EXAMPLE/self::node()"/>
		</pre>
	</ul>

</xsl:if>


<xsl:if test="./SEE_ALSO/SEE_ALSO_ITEM">
	<H3>VOIR AUSSI</H3>
	<p><ul>
	<xsl:for-each select="./SEE_ALSO/SEE_ALSO_ITEM">
		<xsl:apply-templates select="A"/>
	</xsl:for-each>
	</ul></p>
</xsl:if>

<xsl:if test="./AUTHOR">
	<H3>AUTEUR</H3>
	<p><ul>
		<xsl:value-of select="./AUTHOR"/>
	</ul></p>
</xsl:if>

<xsl:if test="./USED_FUNCTION">
	<H3>FONCTION UTILISEE</H3>
	<p><xsl:value-of select="./USED_FUNCTION"/></p>
</xsl:if>

<xsl:if test="./BIBLIO">
	<H3>BIBLIOGRAPHIE</H3>
	<p><xsl:value-of select="./BIBLIO"/></p>
</xsl:if>

</body>
</html>
</xsl:template>

<xsl:template match="PARAM">
	<ul>
		<xsl:apply-templates select="PARAM_ITEM|PARAM_INDENT|ITEMIZE|P|TABLE|VERBATIM"/>
	</ul>
</xsl:template>

<xsl:template match="PARAM[@target='in']">
	<ul>
		<li><H3>PARAMETRES D'ENTREE</H3>
			<ul>
			<xsl:apply-templates select="PARAM_ITEM|PARAM_INDENT|ITEMIZE|P|TABLE|VERBATIM"/>
			</ul>
		</li>
	</ul>
</xsl:template>

<xsl:template match="PARAM[@target='out']">
	<ul>
		<li><H3>PARAMETRES DE SORTIE</H3>
			<ul>
			<xsl:apply-templates select="PARAM_ITEM|PARAM_INDENT|ITEMIZE|P|TABLE|VERBATIM"/>
			</ul>
		</li>
	</ul>
</xsl:template>

<xsl:template match="PARAM_ITEM">
		<li><xsl:apply-templates select="PARAM_NAME|PARAM_ITEM|PARAM_INDENT|PARAM_DESCRIPTION"/></li>
</xsl:template>

<xsl:template match="PARAM_DESCRIPTION">
	<xsl:apply-templates select="P|VERBATIM|TABLE|VERB|PARAM_INDENT|text()"/>
</xsl:template>

<xsl:template match="PARAM_INDENT">
	<ul><xsl:apply-templates select="PARAM_ITEM|PARAM_INDENT|P|VERBATIM|TABLE|VERB|text()"/></ul>
</xsl:template>


<xsl:template match="PARAM_NAME">
	<tt><b><xsl:value-of select="text()"/></b></tt>
</xsl:template>


<xsl:template match="CALLING_SEQUENCE">
		<dl><xsl:apply-templates select="CALLING_SEQUENCE_ITEM"/></dl>
</xsl:template>


<xsl:template match="CALLING_SEQUENCE_ITEM">
	<dd><xsl:value-of select="text()"/></dd>
</xsl:template>


<xsl:template match="ITEM">
	<li>
		<xsl:value-of select="@label"/> 
		<xsl:apply-templates select="ITEMIZE|TABLE|P|VERBATIM|text()"/>
	</li>
</xsl:template>

<xsl:template match="ITEMIZE">
	<H3><xsl:value-of select="@label"/></H3>
	<ul>		
		<xsl:apply-templates select="ITEM|ITEMIZE|TABLE|P|VERBATIM"/>
	</ul>
</xsl:template>

<xsl:template match="TABLE">
	<table border="1">
	<xsl:for-each select="./TABLE_NEW_ROW">
            <tr>
		<xsl:for-each select="./TABLE_NEW_COLUMN">
                	<td>
			<xsl:value-of select="./text()"/>
			</td>	
		</xsl:for-each>
            </tr>
	</xsl:for-each>
        </table>
</xsl:template>

<xsl:template match="VERB">
	<tt><b><xsl:value-of select="./text()"/></b></tt>
</xsl:template>

<xsl:template match="A">
	<a>
	<xsl:attribute name="href"><xsl:value-of select='@href'/></xsl:attribute>
	<xsl:apply-templates select="VERB|text()"/>
	</a>
</xsl:template>

<xsl:template match="DESCRIPTION">
	<ul>
		<xsl:apply-templates select="DESCRIPTION_ITEM|P|VERBATIM|TABLE|A|text()"/>
	</ul>
</xsl:template>

<xsl:template match="DESCRIPTION_ITEM">
	<li>
		<xsl:value-of select="@label"/>  
		<xsl:apply-templates select="P|VERBATIM|TABLE|ITEMIZE|A|text()"/>
	</li>
</xsl:template>

<xsl:template match="P">
	<p><xsl:apply-templates select="VERB|A|text()"/></p>
</xsl:template>

<xsl:template match="VERBATIM">
	<pre>
		<xsl:value-of select="self::node()"/>
	</pre>
</xsl:template>

</xsl:stylesheet>
