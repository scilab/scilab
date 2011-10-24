<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet
	version ="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>


<xsl:import href="XCommon.xsl"/>
<xsl:import href="XGeneric.xsl"/>
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
  ::     T O P   L E V E L
  ::
  --> 

<xsl:template match="interface">
	<!--
  	  :: dummy call to check minimal performances
	  -->
	<ISML layout="border" style="XConfiguration.css">
		<VBox background="#ffffff" border-side="West" scroll="vertical">
			<Tinyest intention="dummy call to check minimal performances"/>
		</VBox>
		<Panel border-side="Center" width="640" height="530">
			<Tinyest intention="dummy call to check minimal performances"/>
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


<xsl:template match="interface">
	<ISML layout="border" style="XConfiguration.css">
		<VBox background="#ffffff" border-side="West" scroll="vertical">
			<!--Label text="{@path}"/-->
			<xsl:apply-templates mode="left-tree">
				<xsl:with-param name="path" select="@path"/>
				<xsl:with-param name="top-id" select="'/'"/>
			</xsl:apply-templates>
			<Glue/>
		</VBox>
		<Panel border-side="Center" width="640" height="530">
			<xsl:apply-templates mode="main-panel">
				<xsl:with-param name="path" select="@path"/>
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

<xsl:template match="*" mode="left-tree">
	<xsl:param name="path"/>
	<xsl:param name="top-id"/>
	<xsl:variable name="local-path">
		<xsl:call-template name="here"/>
	</xsl:variable>
	<VBox>
		<HBox>
			<xsl:apply-templates select="." mode="left-element">
				<xsl:with-param name="local-path" select="$local-path"/>
				<xsl:with-param name="path" select="$path"/>
				<xsl:with-param name="top-id" select="$top-id"/>
			</xsl:apply-templates>
			<Glue/>
			<HSpace width="25"/>
		</HBox>
		<xsl:if test="*[ not(name()='body')]">
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

<xsl:template match="body" mode="left-tree"/>

<xsl:template match="toolboxes" mode="left-tree">
	<xsl:param name="path"/>
	<xsl:param name="top-id"/>
	<xsl:apply-templates mode="left-tree">
		<xsl:with-param name="path" select="$path"/>
		<xsl:with-param name="top-id" select="$top-id"/>	
	</xsl:apply-templates>
</xsl:template>


<xsl:template match="*" mode="left-element">
	<xsl:param name="path"/>
	<xsl:param name="top-id"/>
	<xsl:param name="local-path"/>

	<Radiobutton text="{local-name()}" listener="ActionListener">	
		<xsl:choose>
			<!--xsl:when test="substring($local-path, 1, string-length($path))=$path and substring-after($local-path, $path)=''"-->
			<xsl:when test="$local-path=$path">
				<xsl:attribute name="checked">checked</xsl:attribute>
			</xsl:when>
			<xsl:otherwise>
				<xsl:attribute name="checked">unchecked</xsl:attribute>
			</xsl:otherwise>
		</xsl:choose>
		<!--xsl:variable name="tooltip">
			<xsl:apply-templates select="body/*" mode="tooltip"/>
		</xsl:variable>
		<xsl:if test="not($tooltip='')">
			<xsl:attribute name="tooltip"><xsl:value-of select="$tooltip"/></xsl:attribute>
		</xsl:if-->
		<actionPerformed set="path" context="{$top-id}" value="{$local-path}"/>
	</Radiobutton>
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

<xsl:template match="body" mode="main-panel">
	<xsl:param name="path"/>
	<xsl:variable name="local-path">
		<xsl:for-each select="..">
			<xsl:call-template name="here"/>
		</xsl:for-each>
	</xsl:variable>
	<xsl:if test="$local-path=$path">
	<!-- xsl:if test="substring($local-path, 1, string-length($path))=$path and substring-after($local-path, $path)=''"-->
		<Grid>
			<VBox insets="large" anchor="north" path="$path">
				<xsl:apply-templates/>
			</VBox>
		</Grid>
	</xsl:if>
</xsl:template>

</xsl:stylesheet>

