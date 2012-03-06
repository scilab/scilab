<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version ="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>	<!--
		||
		||   S P E C I F I C   M A I N   P A N E L S
		||
		-->
  <xsl:template match="xcos" mode="preview">
    <xsl:for-each select="interface/regular-inputs/item">
      <xsl:value-of select="@columns"/>
    </xsl:for-each>
  </xsl:template>
</xsl:stylesheet>

