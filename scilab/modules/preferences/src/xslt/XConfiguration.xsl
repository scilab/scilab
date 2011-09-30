<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet
	version ="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>


<xsl:import href="XCommon.xsl"/>
<xsl:import href="XConfiguration/XConfiguration-general.xsl"/>
<xsl:import href="XConfiguration/XConfiguration-font.xsl"/>
<xsl:import href="XConfiguration/XConfiguration-colors.xsl"/>
<xsl:import href="XConfiguration/XConfiguration-console.xsl"/>
<xsl:import href="XConfiguration/XConfiguration-history.xsl"/>
<xsl:import href="XConfiguration/XConfiguration-scinotes.xsl"/>
<xsl:import href="XConfiguration/XConfiguration-variables.xsl"/>
<xsl:import href="XConfiguration/XConfiguration-xcos.xsl"/>
<xsl:import href="XConfiguration/XConfiguration-toolbars.xsl"/>


<!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
  ::
  ::     T O O L B O X
  ::
  -->

<xsl:variable name="path" select="/interface/@path"/>

<xsl:template name="simple-path">
	<xsl:for-each select="ancestor-or-self::*">
		<xsl:value-of select="local-name()"/>
		<xsl:text>/</xsl:text>
	</xsl:for-each>
</xsl:template>

<!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
  ::
  ::     T O P   L E V E L
  ::
  --> 

<xsl:template match="interface">
	<ISML layout="border" style="XConfiguration.css">
		<VBox background="#ffffff" border-side="West" scroll="vertical">
			<xsl:apply-templates select="." mode="left-tree">
				<xsl:with-param name="path" select="$path"/>
				<xsl:with-param name="top-id" select="'/'"/>
			</xsl:apply-templates>
			<Glue/>
		</VBox>
		<Panel border-side="Center" width="640" height="530">
			<xsl:apply-templates mode="main-panel">
				<xsl:with-param name="path" select="$path"/>
			</xsl:apply-templates>
		</Panel>
		<HBox border-side="South">
			<!--Button text="Help"    listener="ActionListener"> 
				<actionPerformed   callback="Help"/>
			</Button -->
			<Glue/>
			<Button text="Default" listener="ActionListener">
				<actionPerformed   callback="Default"/>
			</Button>
			<HSpace width="5"/>
			<Button text="Cancel"  listener="ActionListener">
				<actionPerformed   callback="Cancel"/>
			</Button>
			<HSpace width="5"/>
			<Button text="OK"      listener="ActionListener">
				<actionPerformed   callback="Ok"/>
			</Button>
			<HSpace width="5"/>
			<Button text="Apply"   listener="ActionListener">
				<actionPerformed   callback="Apply"/>
			</Button>
		</HBox>
	</ISML>							
</xsl:template>


<!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
  ::
  ::     L E F T   T R E E 
  ::
  -->


<xsl:template match="interface" mode="left-tree">
	<xsl:param name="path"/>
	<xsl:param name="top-id"/>
	<VBox>
		<xsl:apply-templates mode="left-tree">
			<xsl:with-param name="path" select="$path"/>
			<xsl:with-param name="top-id" select="$top-id"/>
		</xsl:apply-templates>
	</VBox>
</xsl:template>

<xsl:template match="*" mode="left-tree">
	<xsl:param name="path"/>
	<xsl:param name="top-id"/>
	<xsl:variable name="local-path">
		<xsl:call-template name="simple-path"/>
	</xsl:variable>
	<VBox>
		<HBox>
			<Icon>
				<xsl:attribute name="src">
					<xsl:choose>
						<xsl:when test="*[not(name()='body')]">	
							<xsl:text>media-playback-start.png</xsl:text>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>media-playback-start.png</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>
			</Icon>
			<Label listener="MouseListener">	
				<xsl:attribute name="text">
					<xsl:if test="body/@maturity">&lt;HTML&gt;&lt;<xsl:value-of select="body/@maturity"/>&gt;</xsl:if>
					<xsl:value-of select="local-name()"/>
				</xsl:attribute>
				<xsl:choose>
					<xsl:when test="substring($local-path, 1, string-length($path))=$path and substring-after($local-path, $path)=''">
						<xsl:attribute name="background">#0000ff</xsl:attribute>
						<xsl:attribute name="foreground">#ffffff</xsl:attribute>
					</xsl:when>
					<xsl:otherwise>
						<xsl:variable name="tooltip">
							<xsl:apply-templates select="body/*" mode="tooltip"/>
						</xsl:variable>
						<xsl:if test="not($tooltip='')">
							<xsl:attribute name="tooltip"><xsl:value-of select="$tooltip"/></xsl:attribute>
						</xsl:if>
						<mouseClicked set="path" context="{$top-id}" value="{$local-path}"/>
					</xsl:otherwise>
				</xsl:choose>
			</Label>
			<Glue/>
			<HSpace width="25"/>
		</HBox>
		<xsl:if test="*[not(name()='body')]">
			<HBox>
				<HSpace width="25"/>
				<VBox>
					<xsl:apply-templates mode="left-tree">
						<xsl:with-param name="path" select="$path"/>
						<xsl:with-param name="top-id" select="$top-id"/>	
					</xsl:apply-templates>
				</VBox>
				<Glue/>
			</HBox>
		</xsl:if>
	</VBox>
</xsl:template>


<!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
  ::
  ::     M A I N   P A N E L :   
  ::
  ::
  ::
  ::       body tag are not displayed in 'left-tree' mode
  ::
  ::       body tag are displayed in 'main-panel' mode iff
  ::			interface/@path == $local-path
  ::
  -->

<xsl:template match="body" mode="left-tree"/>

<xsl:template match="body" mode="main-panel">
	<xsl:param name="path"/>
	<xsl:variable name="local-path">
		<xsl:for-each select="ancestor::*">
			<xsl:value-of select="local-name()"/>
			<xsl:text>/</xsl:text>
		</xsl:for-each>
	</xsl:variable>

	<xsl:if test="substring($local-path, 1, string-length($path))=$path and substring-after($local-path, $path)=''">
			<Grid>
				<VBox insets="large" anchor="north" path="$path">
					<xsl:apply-templates/>
				</VBox>
			</Grid>
	</xsl:if>
</xsl:template>

</xsl:stylesheet>

