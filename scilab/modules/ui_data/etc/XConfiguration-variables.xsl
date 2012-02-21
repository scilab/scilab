<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version ="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>

	<xsl:template match="variable-browser">
		<Title text="Variable browser">
			<HBox>
				<Label text="Maximum array size: "/>
				<xsl:call-template name="Select">
					<xsl:with-param name="among">
						<option maximum-array-size="125000"/>
						<option maximum-array-size="250000"/>
						<option maximum-array-size="500000"/>
						<option maximum-array-size="1000000"/>
						<option maximum-array-size="2000000"/>
						<option maximum-array-size="4000000"/>
					</xsl:with-param>
				</xsl:call-template>
				<Glue/>
			</HBox>
		</Title>
	</xsl:template>

	<xsl:template match="variable-editor">
		<Title text="Variable editor">
			<HBox>
				<Label text="Default array format"/>
				<HSpace width="1"/>
				<xsl:call-template name="Select">
					<xsl:with-param name="among">
						<option default-array-format="short"/>
						<option default-array-format="long"/>
						<option default-array-format="short e"/>
						<option default-array-format="long e"/>
						<option default-array-format="short g"/>
						<option default-array-format="long g"/>
						<option default-array-format="variable format"/>
					</xsl:with-param>
				</xsl:call-template>
				<Glue/>
			</HBox>
			<HBox>
				<xsl:call-template name="BooleanCheckBox">
					<xsl:with-param name="attr-name" select="'move-selection-after-enter'"/>
					<xsl:with-param name="text" select="'Move selection after enter direction '"/>
				</xsl:call-template>
				<xsl:call-template name="Select">
					<xsl:with-param name="enable" select="@move-selection-after-enter"/>							
					<xsl:with-param name="among">
						<option direction="Down"/>
						<option direction="Up"/>
						<option direction="Left"/>
						<option direction="Right"/>
					</xsl:with-param>
				</xsl:call-template>
				<Glue/>
			</HBox>
		</Title>

		<Title text="International number handling">
			<HBox>
				Default array format
				<Entry text="{@decimal-separator}" listener="ActionListener">
					<actionPerformed choose="decimal-separator">
						<xsl:call-template name="context"/>
					</actionPerformed>							
				</Entry>
				<Glue/>
			</HBox>		
		</Title>
		<VSpace height="150"/>
	</xsl:template>

</xsl:stylesheet>

