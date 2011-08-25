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
					<xsl:with-param name="attr-name" select="'use-system-color'"/>
				</xsl:call-template>
				Use system color
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
			<VBox>
				<HBox>
					<Table item="{$item}" mode="select" listener="TableListener" column="name">
						<tableSelect choose="name">
							<xsl:call-template name="context"/>
						</tableSelect>
						<tableCol title="Item"        attr="name"/>

						<xsl:for-each select="item">
							<tableRow name="{@name}"/>
						</xsl:for-each>
					</Table>
					<HSpace width="20"/>
					<PreviewCode listener="ActionListener">
						<actionPerformed choose="name">
							<xsl:call-template name="context"/>
						</actionPerformed>
						<xsl:for-each select="item">
							<tableRow name="{@name}" color="{@color}"/>
						</xsl:for-each>
					</PreviewCode>
				</HBox>

				<HBox>
					<xsl:for-each select="item[ position()=$item]">
						<VBox>
							<HBox>
								<Color color="{@color}" listener="ActionListener">
									<actionPerformed choose="color">
										<xsl:call-template name="context"/>
									</actionPerformed>
								</Color>
								<HSpace width="3"/>
								Color
								<Glue/>
							</HBox>
							<HBox>
								<HSpace width="30"/>
								<xsl:call-template name="BooleanCheckBox">
									<xsl:with-param name="attr-name" select="'bold'"/>
								</xsl:call-template>
								Bold
								<Glue/>
							</HBox>
							<HBox>
								<HSpace width="30"/>
								<xsl:call-template name="BooleanCheckBox">
									<xsl:with-param name="attr-name" select="'italic'"/>
								</xsl:call-template>
								Italic
								<Glue/>
							</HBox>
						</VBox>
						<VBox>
							<HBox>
								<xsl:call-template name="BooleanCheckBox">
									<xsl:with-param name="attr-name" select="'underline'"/>
								</xsl:call-template>
								Underline
								<Glue/>
							</HBox>
							<HBox>
								<xsl:call-template name="BooleanCheckBox">
									<xsl:with-param name="attr-name" select="'strinke-through'"/>
								</xsl:call-template>
								Strinke Through
								<Glue/>
							</HBox>
							<VSpace height="30"/>
						</VBox>
					</xsl:for-each>
					<Glue/>
					<Button text="Restore Default Settings"/>
				</HBox>
			</VBox>
		</Title>
	</xsl:template>
</xsl:stylesheet>

