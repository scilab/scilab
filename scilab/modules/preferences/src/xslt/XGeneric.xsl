<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet
	version ="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>

<xsl:template match="Form">
	<Title text="{@text}">
		<Grid>
			<xsl:apply-templates mode="form"/>
		</Grid>
	</Title>
</xsl:template>

<xsl:template match="Entry" mode="form">
	<xsl:variable name="position" select="count(preceding-sibling::*) + 1"/>
	<Label gridx="1" gridy="{$position}" text="{@text}"/>
	<Entry gridx="2" gridy="{$position}" text="{@value}" listener="ActionListener">
		<actionPerformed choose="value">
			<xsl:call-template name="context"/>
		</actionPerformed>
	</Entry>
</xsl:template>

<xsl:template match="Select" mode="form">
	<xsl:variable name="position" select="count(preceding-sibling::*) + 1"/>
	<Label  gridx="1" gridy="{$position}" text="{@text}"/>
	<Select gridx="2" gridy="{$position}" listener="ActionListener">
		<actionPerformed choose="value">
			<xsl:call-template name="context"/>
		</actionPerformed>
		<xsl:apply-templates/>
	</Select>
</xsl:template>

<xsl:template match="Option">
	<option value="{@value}">
		<xsl:if test="@value=../@value">
			<xsl:attribute name="selected">selected</xsl:attribute>
		</xsl:if>
	</option>
</xsl:template>

<xsl:template match="Color" mode="form">
	<xsl:variable name="position" select="count(preceding-sibling::*) + 1"/>
	<Label gridx="1" gridy="{$position}" text="{@text}"/>
	<Color gridx="2" gridy="{$position}" color="{@value}" listener="ActionListener">
		<actionPerformed choose="value">
			<xsl:call-template name="context"/>
		</actionPerformed>
	</Color>
</xsl:template>

<xsl:template match="Checkbox[ @value='no']" mode="form">
	<xsl:variable name="position" select="count(preceding-sibling::*) + 1"/>
	<Checkbox gridx="1" gridy="{$position}" gridwidth="2" checked="unchecked" text="{@text}" listener="ActionListener">
		<actionPerformed set="value" value="yes">
			<xsl:call-template name="context"/>
		</actionPerformed>
	</Checkbox>
</xsl:template>

<xsl:template match="Checkbox[ @value='yes']" mode="form">
	<xsl:variable name="position" select="count(preceding-sibling::*) + 1"/>
	<Checkbox gridx="1" gridy="{$position}" gridwidth="2" checked="checked" text="{@text}" listener="ActionListener">
		<actionPerformed set="value" value="no">
			<xsl:call-template name="context"/>
		</actionPerformed>
	</Checkbox>
</xsl:template>

<!--   /////////////////////////////////////////////// -->
<!--  ///  //  ///  ///  ///  ///  ///  ///  ///  /// -->
<!-- ///////////////////////////////////////////////   -->

<xsl:template match="*">
	<Glue/>
	<HBox>
		<Title text="{local-name()}">
			<Label text="will be implemented soon..." foreground="#0000ff"/>
		</Title>
	</HBox>
	<Glue/>
</xsl:template>

</xsl:stylesheet>

