<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version ="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>
<!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
  ::
  ::     M A I N   P A N E L   :   C O L O R S
  ::
  -->

	<xsl:template match="desktop-colors">
		<Title text="Desktop color">
			<HBox>
				<xsl:call-template name="BooleanCheckBox">
					<xsl:with-param name="text" select="'Use system color'"/>
					<xsl:with-param name="attr-name" select="'use-system-color'"/>
				</xsl:call-template>
				<Glue/>
				Text
				<Color color="{@text}" listener="ActionListener">
					<actionPerformed choose="text">
						<xsl:call-template name="context"/>
					</actionPerformed>
				</Color>
				<HSpace width="80"/>
				Background
				<Color color="{@background}" listener="ActionListener">
					<actionPerformed choose="background">
						<xsl:call-template name="context"/>
					</actionPerformed>
				</Color>
				<HSpace width="80"/>
				Cursor
				<Color color="{@cursor}" listener="ActionListener">
					<actionPerformed choose="cursor">
						<xsl:call-template name="context"/>
					</actionPerformed>
				</Color>
			</HBox>
		</Title>
	</xsl:template>

	<xsl:template match="syntax-highlighting-colors">
		<xsl:variable name="item">
			<xsl:for-each select="item">
				<xsl:if test="@name = ../@name">
					<xsl:value-of select="position()"/>
				</xsl:if>
			</xsl:for-each>
		</xsl:variable> 
		<Title text="Syntax highlighting colors">
			<Grid>
					<Panel weightx="2" gridy="1" gridx="1" gridwidth="2" height="300" width="360">
						<Table item="{$item}" mode="select" listener="TableListener" column="name">
							<tableSelect choose="name">
								<xsl:call-template name="context"/>
							</tableSelect>
							<tableCol title="Item"        attr="name"/>
							<xsl:for-each select="item">
								<tableRow name="{@name}"/>
							</xsl:for-each>
						</Table>
					</Panel>
					<PreviewCode gridy="1" gridx="3" gridheight="4" listener="ActionListener">
						<actionPerformed choose="name">
							<xsl:call-template name="context"/>
						</actionPerformed>
						<xsl:for-each select="item">
							<tableRow name="{@name}" color="{@color}"/>
						</xsl:for-each>
					</PreviewCode>
					<xsl:for-each select="item[ position()=$item]">
						<HBox gridy="2" gridx="1">
							<Color color="{@color}" listener="ActionListener">
								<actionPerformed choose="color">
									<xsl:call-template name="context"/>
								</actionPerformed>
							</Color>
							<HSpace width="3"/>
							Color
							<Glue/>
						</HBox>
						<HBox gridy="3" gridx="1">
							<HSpace width="30"/>
							<xsl:call-template name="BooleanCheckBox">
								<xsl:with-param name="attr-name" select="'bold'"/>
							</xsl:call-template>
							Bold
							<Glue/>
						</HBox>
						<HBox gridy="3" gridx="2">

							<xsl:call-template name="BooleanCheckBox">
								<xsl:with-param name="attr-name" select="'italic'"/>
							</xsl:call-template>
							Italic
							<Glue/>
						</HBox>
						<HBox gridy="2" gridx="2">
							<xsl:call-template name="BooleanCheckBox">
								<xsl:with-param name="attr-name" select="'underline'"/>
							</xsl:call-template>
							Underline
							<Glue/>
						</HBox>
						<HBox gridy="4" gridx="1">
							<HSpace width="30"/>
							<xsl:call-template name="BooleanCheckBox">
								<xsl:with-param name="attr-name" select="'strike-through'"/>
							</xsl:call-template>
							Strike Through
							<Glue/>
						</HBox>
					</xsl:for-each>
					<Button gridy="4" gridx="2" text="Restore Default Settings"/>
			</Grid>
		</Title>
	</xsl:template>
</xsl:stylesheet>

