<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version ="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:import href="$SCI/modules/preferences/src/xslt/XCommon.xsl"/>

  <xsl:template match="toolbox_skeleton" mode="tooltip">Environment</xsl:template>
  <xsl:template match="toolbox_skeleton">
    <VSpace height="25"/>
    <Title text="Environment">
      <HBox>
        Foo:
        <xsl:call-template name="Select">
          <xsl:with-param name="among">
            <option foo="bar"/>
            <option foo="barbar"/>
            <option foo="barbarbar"/>
          </xsl:with-param>
        </xsl:call-template>
      </HBox>
    </Title>
  </xsl:template>

  <xsl:template match="save">
    <VSpace height="25"/>
    <Title text="Save">
      <HBox>
        <Checkbox listener="ActionListener">
          <xsl:if test="@bar='true'">
            <xsl:attribute name="checked">checked</xsl:attribute>
          </xsl:if>
          <actionPerformed set="bar" value="true">
            <xsl:call-template name="context"/>
          </actionPerformed>
        </Checkbox>
        Save:
      </HBox>
    </Title>
  </xsl:template>
</xsl:stylesheet>