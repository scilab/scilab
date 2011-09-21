<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version ="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>

<xsl:template match="history-save">
	<Title text="Saving">
		<VBox>
				<HBox>
				<Checkbox listener="ActionListener">
					<xsl:if test="@kind='save'">
						<xsl:attribute name="checked">checked</xsl:attribute>
					</xsl:if>
					<actionPerformed set="kind" value="save">
						<xsl:call-template name="context"/>
					</actionPerformed>
				</Checkbox>
				Save after
				<xsl:variable name="enable">
				</xsl:variable>
				<xsl:call-template name="Select">
					<xsl:with-param name="enable">
						<xsl:choose>
							<xsl:when test="@kind='save'">
								<xsl:text>true</xsl:text>
							</xsl:when>
							<xsl:otherwise>
								<xsl:text>false</xsl:text>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:with-param>
					<xsl:with-param name="among">
						<option after="1"/>
						<option after="2"/>
						<option after="4"/>
						<option after="8"/>
						<option after="16"/>
						<option after="32"/>
						<option after="64"/>
						<option after="128"/>
						<option after="256"/>
					</xsl:with-param>
				</xsl:call-template>
				command(s)			
				<Glue/>
			</HBox>
			<HBox>
				<Checkbox listener="ActionListener">
					<xsl:if test="@kind='quit'">
						<xsl:attribute name="checked">checked</xsl:attribute>
					</xsl:if>
					<actionPerformed set="kind" value="quit">
						<xsl:call-template name="context"/>
					</actionPerformed>
				</Checkbox>
				Save history file on quit
				<Glue/>
			</HBox>
			<HBox>
				<Checkbox listener="ActionListener">
					<xsl:if test="@kind='disable'">
						<xsl:attribute name="checked">checked</xsl:attribute>
					</xsl:if>
					<actionPerformed set="kind" value="disable">
						<xsl:call-template name="context"/>
					</actionPerformed>
				</Checkbox>
				Disable history management
				<Glue/>
			</HBox>
			<VSpace height="20"/>
			<HBox>
				<Checkbox listener="ActionListener" checked="{@consecutive}">
					<actionPerformed choose="consecutive">
						<xsl:call-template name="context"/>
					</actionPerformed>
				</Checkbox>
				Save consecutive duplicate commands
				<Glue/>
			</HBox>
		</VBox>
	</Title>
</xsl:template>

<xsl:template match="history-settings">
	<Title text="Settings">
		<VBox>
			<HBox>
				History file:
				<File href="{@history-file}" mask="*.scilab" desc="Choose a history file" listener="ActionListener">
					<actionPerformed choose="history-file">
						<xsl:call-template name="context"/>
					</actionPerformed>
				</File>	
			</HBox>
			<VSpace height="30"/>
			<HBox>
				History lines:
				<xsl:call-template name="Select">
					<xsl:with-param name="among">
						<option history-line="250"/>
						<option history-line="500"/>
						<option history-line="1000"/>
						<option history-line="2000"/>
						<option history-line="4000"/>
					</xsl:with-param>
				</xsl:call-template>
			</HBox>
		</VBox>
	</Title>
	<VSpace height="120"/>
</xsl:template>


</xsl:stylesheet>

