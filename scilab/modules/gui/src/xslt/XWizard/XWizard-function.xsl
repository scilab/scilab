<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version ="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>	<!--
		||
		||   S P E C I F I C   M A I N   P A N E L S 
		||
		-->
	<xsl:template match="function[parent::function]" mode ="main-panel">
		<xsl:if test="ancestor::function/@step='function'">
			<Title text="Scilab function">
				<VBox>
					<HBox>&lt;HTML&gt;&lt;B&gt;Discrete State
						<Entry text="{@discrete-state}" listener="ActionListener">
							<actionPerformed choose="context">
								<xsl:call-template name="context"/>
							</actionPerformed>							
						</Entry>
					</HBox>
					<VSpace height="25"/>
					<HBox>&lt;HTML&gt;&lt;B&gt;Continuous State
						<Entry text="{@discrete-state}" listener="ActionListener">
							<actionPerformed choose="discrete-state">
								<xsl:call-template name="context"/>
							</actionPerformed>							
						</Entry>
					</HBox>
					<VSpace height="25"/>
					<HBox>
						<xsl:call-template name="BooleanCheckBox">
							<xsl:with-param name="attr-name">always-active</xsl:with-param>
						</xsl:call-template>&lt;HTML&gt;&lt;B&gt;Always active
					</HBox>
				</VBox>
			</Title>
			<VSpace height="300"/>
		</xsl:if>
	</xsl:template>

</xsl:stylesheet>

