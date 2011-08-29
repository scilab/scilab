<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version ="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
> <!--
		||
		||   I M P O R T S
		||
		-->
	<xsl:import href="XCommon.xsl"/>
	<xsl:import href="XWizard/XWizard-any.xsl"/>
  <xsl:import href="XWizard/XWizard-function.xsl"/>
	<xsl:import href="XWizard/XWizard-expression.xsl"/>
	<xsl:import href="XWizard/XWizard-xcos.xsl"/>

	<!--
		||
		||   T E M P O R A R Y   T R E E S
		||   | wrox book workaround, p.237
		||
		-->
	<xsl:variable name="imports"
		select="document('')/xsl:stylesheet/xsl:import/@import"/>
	<xsl:variable name="variables"
		select="document($imports)/xsl:stylesheet/descendant::xsl-variable"/>

	<!--
		||
		||   N A V I G A T I O N   M A N A G E M E N T
		||
		-->
	<xsl:variable name="current" select="/*/*[local-name()=/*/@step]"/>
	<xsl:variable name="next"    select="$current/following-sibling::*[1]"/>
	<xsl:variable name="prev"    select="$current/preceding-sibling::*[1]"/>

	<!--
		||
		||   T O P - L E V E L
		||
		-->
	<xsl:template match="/">
		<ISML layout="border">
			<VBox background="#ffffff" border-side="West" scroll="vertical">
				<xsl:apply-templates select="*" mode="left-panel"/>
			</VBox>
			<Panel border-side="Center" width="640" height="420">
				<VBox>
					<xsl:apply-templates mode="main-panel"/>
				</VBox>
			</Panel>
			<HBox border-side="South">
				<Button text="Previous"    listener="ActionListener">
					<xsl:choose>
						<xsl:when test="$prev">
							<actionPerformed   set="step" context="/">
								<xsl:attribute name="value">
									<xsl:for-each select="$prev">
										<xsl:value-of select="local-name()"/>
									</xsl:for-each>
								</xsl:attribute>
							</actionPerformed>
						</xsl:when>
						<xsl:otherwise>
							<xsl:attribute name="status">
								<xsl:text>disabled</xsl:text>
							</xsl:attribute>
						</xsl:otherwise>
					</xsl:choose>
				</Button>
				<HSpace width="5"/>
				<Button text="Next"    listener="ActionListener">
					<xsl:choose>
						<xsl:when test="$next">
							<actionPerformed   set="step" context="/">
								<xsl:attribute name="value">
									<xsl:for-each select="$next">
										<xsl:value-of select="local-name()"/>
									</xsl:for-each>
								</xsl:attribute>
							</actionPerformed>
						</xsl:when>
						<xsl:otherwise>
							<xsl:attribute name="status">
								<xsl:text>disabled</xsl:text>
							</xsl:attribute>
						</xsl:otherwise>
					</xsl:choose>
				</Button>
				<Glue/>
				<xsl:choose>
					<xsl:when test="user-block-wizard">
						<Button text="Open"  listener="ActionListener">
							<actionPerformed   callback="Open"/>
						</Button>
					</xsl:when>
					<xsl:otherwise>
						<Button text="New"   listener="ActionListener">
							<actionPerformed   callback="New"/>
						</Button>
						<HSpace width="5"/>
						<Button text="Save"  listener="ActionListener">
							<actionPerformed   callback="Save"/>
						</Button>
						<HSpace width="5"/>
						<Button text="Save as" listener="ActionListener">
							<actionPerformed   callback="SaveAs"/>
						</Button>
						<HSpace width="5"/>
						<Button text="Ok"    listener="ActionListener">
							<actionPerformed   callback="Ok"/>
						</Button>
					</xsl:otherwise>
				</xsl:choose>
				<HSpace width="5"/>
				<Button text="Quit"  listener="ActionListener">
					<actionPerformed   callback="Quit"/>
				</Button>
			</HBox>
		</ISML>
	</xsl:template>

	<!--
		||
		||   L E F T   P A N E L   M A N A G E M E N T
		||
		-->
	<xsl:template match="user-block-wizard" mode="left-panel">
		<xsl:apply-templates mode="inner-left-panel"/>
	</xsl:template>

	<xsl:template match="kind" mode="inner-left-panel">
		<HBox>
			<Icon src="media-playback-start.png"/>
			<xsl:choose>
				<xsl:when test="ancestor::user-block-wizard/@select=@name">
					<Label
							text="{@title}"
							background="#0000ff"
							foreground="#ffffff"
							listener="MouseListener">
						<mouseClicked callback="load" name="{@name}"/>
					</Label>
				</xsl:when>
				<xsl:otherwise>
					<Label text="{@title}">
						<xsl:choose>
							<xsl:when test="@name">
								<xsl:attribute name="listener">MouseListener</xsl:attribute>
								<mouseClicked set="select" value="{@name}" context="/"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:attribute name="foreground">#dddddd</xsl:attribute>
							</xsl:otherwise>
						</xsl:choose>
					</Label>
				</xsl:otherwise>
			</xsl:choose>
			<Glue/>
			<HSpace width="25"/>
		</HBox>
	</xsl:template>

	<xsl:template match="folder" mode="inner-left-panel">
		<HBox>
			<Icon src="media-playback-start.png"/>&lt;HTML&gt;&lt;B&gt;
			<xsl:value-of select="@title"/>
			<Glue/>
			<HSpace width="25"/>
		</HBox>
		<HBox>
			<HSpace width="25"/>
			<VBox>
				<xsl:apply-templates mode="inner-left-panel"/>
			</VBox>
		</HBox>
	</xsl:template>

	<!--
		||
		||   M E A N   P A N E L   M A N A G E M E N T
		||
		-->
	<xsl:template match="kind" mode="main-panel">
		<xsl:if test="@name=ancestor::user-block-wizard/@select">
			<Title text="{@title}">
				<Label listener="MouseListener">
					<xsl:attribute name="text"><xsl:apply-templates/></xsl:attribute>
					<mouseClicked callback="load" name="{@name}"/>
				</Label>
			</Title>
		</xsl:if>
	</xsl:template>

</xsl:stylesheet>

