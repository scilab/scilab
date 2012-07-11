<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version ="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:import href="XCommon.xsl"/>
  <xsl:import href="XCommon-toolbars.xsl"/>
  <xsl:import href="XGeneric.xsl"/>

  <!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
       ::
       ::     T O P   L E V E L
       ::
  -->

  <xsl:template match="interface">
    <ISML layout="border" height="{@height}" width="{@width}">
      <Tree border-side="West" listener="ActionListener">
        <xsl:apply-templates mode="left-tree"/>
        <actionPerformed choose="path" context="/"/>
      </Tree>
      <Panel border-side="Center">
        <xsl:apply-templates mode="main-panel">
          <xsl:with-param name="path" select="@path"/>
        </xsl:apply-templates>
      </Panel>
      <HBox border-side="South">
        <Glue/>
        <Button text="Default" listener="ActionListener">
          <actionPerformed callback="Default"/>
        </Button>
        <HSpace width="5"/>
        <Button text="Cancel" listener="ActionListener">
          <actionPerformed callback="Cancel"/>
        </Button>
        <HSpace width="5"/>
        <Button text="OK" listener="ActionListener">
          <actionPerformed callback="Ok"/>
        </Button>
        <HSpace width="5"/>
        <Button text="Apply" listener="ActionListener">
          <actionPerformed callback="Apply"/>
        </Button>
      </HBox>
    </ISML>
  </xsl:template>

  <!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
       ::
       ::     L E F T   T R E E
       ::
       ::       body tag are not displayed in 'left-tree' mode
       ::
  -->


  <xsl:template match="*" mode="left-tree">
    <tableNode>
      <xsl:attribute name="name">
	<xsl:value-of select="./@title"/> 
      </xsl:attribute>
      <xsl:apply-templates mode="left-tree"/>
    </tableNode>
  </xsl:template>

  <xsl:template match="toolboxes" mode="left-tree">
    <xsl:if test="*[not(self::body)]">
      <tableNode name="{local-name()}">
	<xsl:apply-templates mode="left-tree"/>
      </tableNode>
    </xsl:if>
  </xsl:template>

  <xsl:template match="body" mode="left-tree"/>

  <!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
       ::
       ::     M A I N   P A N E L :
       ::
       ::       body tag are displayed in 'main-panel' mode iff
       ::                       interface/@path == $local-path
       ::
  -->

  <xsl:template match="body" mode="main-panel">
    <xsl:param name="path"/>
    <xsl:variable name="local-path">
      <xsl:for-each select="..">
        <xsl:call-template name="here"/>
      </xsl:for-each>
    </xsl:variable>
    <!--xsl:message>
      <xsl:value-of select="$local-path"/>::::::::<xsl:value-of select="$path"/>
    </xsl:message-->
    <xsl:if test="$local-path=$path">
      <VBox path="{$path}" fill="both">
        <xsl:apply-templates/>
      </VBox>
    </xsl:if>
  </xsl:template>
</xsl:stylesheet>

