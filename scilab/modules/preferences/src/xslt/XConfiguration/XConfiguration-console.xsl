<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version ="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
><!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
  ::
  ::     M A I N   P A N E L :   C O N S O L E

  ::
  -->

	<xsl:template match="display">
		<Title text="Display">
			<Grid>
				<Panel gridx="1" gridy="1" gridwidth="2">
					<xsl:call-template name="BooleanCheckBox">
						<xsl:with-param name="text" select="'Wrap lines'"/>
						<xsl:with-param name="attr-name" select="'wrap-lines'"/>
					</xsl:call-template>
				</Panel>
				<Label gridx="1" gridy="2" text="Lines to display:"/>
				<Panel gridx="2" gridy="2">
					<xsl:call-template name="Select">
						<xsl:with-param name="among">
							<option lines-to-display="25"/>
							<option lines-to-display="50"/>
							<option lines-to-display="100"/>
							<option lines-to-display="200"/>
							<option lines-to-display="400"/>
						</xsl:with-param>
					</xsl:call-template>
				</Panel>
				<Label gridx="1" gridy="3" text="Columns to display:"/>
				<Panel gridx="2" gridy="3">
					<xsl:call-template name="Select">
						<xsl:with-param name="among">
							<option columns-to-display="25"/>
							<option columns-to-display="50"/>
							<option columns-to-display="100"/>
							<option columns-to-display="200"/>
							<option columns-to-display="400"/>
						</xsl:with-param>
					</xsl:call-template>
				</Panel>
				<Label gridx="1" gridy="4" text="Tab size:"/>
				<Panel gridx="2" gridy="4">
					<xsl:call-template name="Select">
						<xsl:with-param name="among">
							<option tab-size="2"/>
							<option tab-size="4"/>
							<option tab-size="8"/>
						</xsl:with-param>
					</xsl:call-template>
				</Panel>
				<Label gridx="1" gridy="5" text="Number of lines in console scroll buffer:"/>
				<Panel gridx="2" gridy="5">
					<xsl:call-template name="Select">
						<xsl:with-param name="among">
							<option scroll-size="750"/>
							<option scroll-size="1500"/>
							<option scroll-size="3000"/>
							<option scroll-size="6000"/>
							<option scroll-size="12000"/>
						</xsl:with-param>
					</xsl:call-template>
				</Panel>
			</Grid>
		</Title>
	</xsl:template>

	<xsl:template match="accessibility">
		<Title text="Display">
			<VBox>
				<HBox>
					To enable keyboard navigation with arrow keys, 
					assign shotcuts to the Cursor Up and
					<Glue/>
				</HBox>
				<HBox>&lt;HTML&gt;Cursor Down actions in the 
					&lt;U&gt;Shortcuts Panel&lt;/U&gt; (for Console)
					&lt;/HTML&gt;<Glue/>
				</HBox>
				<VSpace height="25"/>
				<HBox>&lt;HTML&gt;To enable or disable the completion, 
					assign a shortcut action in the &lt;U&gt;Shortcuts 
					panel&lt;/U&gt; (for &lt;/HTML&gt;<Glue/>
				</HBox>
				<HBox>
					Console). Default shortcut for completion is Tab.
					<Glue/>
				</HBox>
			</VBox>
		</Title>
	</xsl:template>

</xsl:stylesheet>

