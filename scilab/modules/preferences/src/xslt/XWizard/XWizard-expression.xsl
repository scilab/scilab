<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version ="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>	<!--
		||
		||   S P E C I F I C   M A I N   P A N E L S
		||
		-->
    <xsl:template match="expression" mode="preview">
        <todo/>
    </xsl:template>
	<xsl:template match="expression[parent::expression]" mode ="main-panel">
		<xsl:if test="ancestor::expression/@step='expression'">
			<Title text="Scilab expression">
				<VBox>
					<HBox>&lt;HTML&gt;&lt;B&gt;Scilab Expressions
						<Glue/>
					</HBox>
					<xsl:for-each select="outputs">
						<Table mode="cell" location="fixed" size="dynamic" listener="TableListener">
                            <tableChanged>
                                <xsl:call-template name="context"/>
                            </tableChanged>
                            <tableAdd insert="selection">
								<xsl:call-template name="context"/>
								<item value="0.0"/>
							</tableAdd>
                            <tableDel delete="selection">
								<xsl:call-template name="context"/>
                            </tableDel>
                            <tableCol title=""     width="30"       attr="label"/>
							<tableCol title="Values"       attr="value" editable="true"/>
							<xsl:for-each select="item">
								<tableRow label="{concat('y', format-number(position(), '0'))}" value="{@value}"/>
							</xsl:for-each>
						</Table>
					</xsl:for-each>
					<HBox>&lt;HTML&gt;&lt;B&gt;Input test values
						<Glue/>
						<Button text="Execute"/>
					</HBox>
					<xsl:for-each select="inputs">
						<Table mode="cell" location="fixed" size="dynamic" listener="TableListener">
                           <tableChanged>
                                <xsl:call-template name="context"/>
                            </tableChanged>
							<tableAdd>
								<xsl:call-template name="context"/>
								<item value="0.0"/>
							</tableAdd>
							<tableCol title=""     width="30"       attr="label"/>
							<tableCol title="Values"       attr="value" editable="true" />
							<xsl:for-each select="item">
								<tableRow label="{concat('u', format-number(position(), '0'))}" value="{@value}" />
							</xsl:for-each>
						</Table>
					</xsl:for-each>
					<HBox>
						<xsl:call-template name="BooleanCheckBox">
							<xsl:with-param name="attr-name">zero-crossing</xsl:with-param>
						</xsl:call-template>
						Use zero-crossing
						<Glue/>
					</HBox>
				</VBox>
			</Title>
		</xsl:if>
	</xsl:template>

</xsl:stylesheet>

