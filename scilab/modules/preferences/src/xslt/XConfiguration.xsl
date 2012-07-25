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
      <VBox border-side="North">
        <VSpace height="5"/>
      </VBox>
      <HBox border-side="West">
        <Glue/>
        <Panel>
          <HSpace width="15"/>
        </Panel>
        <Tree listener="ActionListener">
          <xsl:apply-templates mode="left-tree"/>
          <actionPerformed choose="path" context="/"/>
        </Tree>
        <Panel>
          <HSpace width="5"/>
        </Panel>
        <Glue/>
      </HBox>
      <HBox border-side="Center">
	<Scroll>
        <Panel border-side="Center">
          <xsl:apply-templates mode="main-panel">
            <xsl:with-param name="path" select="@path"/>
          </xsl:apply-templates>
        </Panel>
	</Scroll>
      </HBox>
      <HBox border-side="East">
        <Glue/>
        <Panel>
          <HSpace width="15"/>
        </Panel>
      </HBox>
      <HBox border-side="South">
        <VBox>
          <Grid>
            <Panel gridx="1" gridy="1" weightx="1"/>
            <Button gridx="2" gridy="1" weightx="0" text="_(Default)" listener="ActionListener">
              <actionPerformed callback="Default"/>
            </Button>
            <Panel gridx="3" gridy="1" weightx="0">
              <HSpace width="10"/>
            </Panel>
            <Button gridx="4" gridy="1" weightx="0" text="_(Cancel)" listener="ActionListener">
              <actionPerformed callback="Cancel"/>
            </Button>
            <Panel gridx="5" gridy="1" weightx="0">
              <HSpace width="10"/>
            </Panel>
            <Button gridx="6" gridy="1" weightx="0" text="_(OK)" listener="ActionListener">
              <actionPerformed callback="Ok"/>
            </Button>
            <Panel gridx="7" gridy="1" weightx="0">
              <HSpace width="10"/>
            </Panel>
            <Button gridx="8" gridy="1" weightx="0" text="_(Apply)" listener="ActionListener">
              <actionPerformed callback="Apply"/>
            </Button>
            <Panel gridx="9" gridy="1" weightx="0">
              <HSpace width="15"/>
            </Panel>
          </Grid>
          <VSpace height="10"/>
        </VBox>
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

