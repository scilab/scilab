<?xml version='1.0' encoding='iso-8859-1'?>
<xsl:stylesheet version ="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>

	<xsl:template match="variable-browser">
		<Title text="Variable size">
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
		<VSpace height="350"/>
	</xsl:template>

	<xsl:template match="variable-editor">
		<Title text="Variable size">
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
		</Title>
		<Title text="Variable size">
			<VBox>
				<HBox>
					<xsl:call-template name="BooleanCheckBox">
						<xsl:with-param name="attr-name" select="'move-selection-after-enter'"/>
					</xsl:call-template>
					Move selection after enter
					<Glue/>
				</HBox>
				<xsl:choose>
					<xsl:when test="@move-selection-after-enter='true'">
						<HBox>
							<Label text="Direction: "/> 
							<xsl:call-template name="Select">
								<xsl:with-param name="among">
									<option direction="Down"/>
									<option direction="Up"/>
									<option direction="Left"/>
									<option direction="Right"/>
								</xsl:with-param>
							</xsl:call-template>
							<Glue/>
						</HBox>
					</xsl:when>
					<xsl:otherwise>
						<VSpace height="40"/>
					</xsl:otherwise>
				</xsl:choose>
			</VBox>
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

