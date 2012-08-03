<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet
    version ="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    >
  <!--
      ::
      :: Any callback must be equipped with a 'context' attribute
      :: indicating where the rewriting triggered by the event will
      :: occur
      ::
      ::::> 'here' computes context value at execution point.
      ::
      ::::> 'context' is used as an attribute of a callback:
      ::
      ::  <{event-callback} {action}="{path}">
      ::          <xsl:call-template name="context"/>
      ::     [...]
      ::  </{event-callback}>
      ::
      ::::> org.scilab.modules.gui.utils.XCommonManager.getElementByContext
      ::
  -->

  <xsl:template name="here">
    <xsl:for-each select="ancestor-or-self::*">
      <xsl:if test="not(.=/)">
        <xsl:value-of select="count(preceding-sibling::*)+1"/>
        <xsl:text>/</xsl:text>
      </xsl:if>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="context">
    <xsl:attribute name="context">
      <xsl:call-template name="here"/>
    </xsl:attribute>
  </xsl:template>

  <!--
      ::
      :: Select template allow to define values of combobox within the XSL.
      ::
      ::        <xsl:call-template name="Select">
      ::                <xsl:with-param name="among">
      ::                        <option <<attr>>="<<value 1>>"/>
      ::                        <option <<attr>>="<<value 2>>"/>
      ::                        <option <<attr>>="<<value 3>>"/>
      ::                        ...
      ::                </xsl:with-param>
      ::        </xsl:call-template>
      ::
  -->

  <xsl:template name="Select">
    <xsl:param name="among"/>
    <xsl:param name="enable" select="'true'"/>
    <xsl:variable name="list" select="nodeset($among)/*"/>
    <xsl:variable name="attr-name">
      <xsl:for-each select="$list[1]/@*[1]">
        <xsl:value-of select="local-name()"/>
      </xsl:for-each>
    </xsl:variable>
    <xsl:variable name="attr-value" select="@*[local-name()=$attr-name]"/>
    <Select listener="ActionListener" enable="{$enable}">
      <actionPerformed choose="{$attr-name}">
        <xsl:call-template name="context"/>
      </actionPerformed>
      <xsl:for-each select="$list">
        <option value="{@*}">
          <xsl:if test="@*=$attr-value">
            <xsl:attribute name="selected">selected</xsl:attribute>
          </xsl:if>
        </option>
      </xsl:for-each>
    </Select>
  </xsl:template>

  <xsl:template name="Slider">
    <xsl:param name="among"/>
    <xsl:variable name="list" select="nodeset($among)/*"/>
    <xsl:variable name="attr-name">
      <xsl:for-each select="$list[1]/@*[1]">
        <xsl:value-of select="local-name()"/>
      </xsl:for-each>
    </xsl:variable>
    <xsl:variable name="attr-value" select="@*[local-name()=$attr-name]"/>
    <Slider listener="ActionListener">
      <actionPerformed choose="{$attr-name}">
        <xsl:call-template name="context"/>
      </actionPerformed>
      <xsl:for-each select="$list">
        <option value="{@*}">
          <xsl:if test="@*=$attr-value">
            <xsl:attribute name="selected">selected</xsl:attribute>
          </xsl:if>
        </option>
      </xsl:for-each>
    </Slider>
  </xsl:template>

  <!--
      ::
      :: BooleanCheckBox template (@attr-name in {true, false})
      ::
      ::        <xsl:call-template name="SimpleCheckBox">
      ::                <xsl:with-param name="attr-name">...</xsl:with-param>
      ::        </xsl:call-template>
      ::
  -->
  <xsl:template name="BooleanCheckBox">
    <xsl:param name="attr-name"/>
    <xsl:param name="text" select="''"/>
    <xsl:choose>
      <xsl:when test="@*[local-name()=$attr-name]='true'">
        <Checkbox checked="checked" listener="ActionListener" text="{$text}">
          <actionPerformed set="{$attr-name}" value="false">
            <xsl:call-template name="context"/>
          </actionPerformed>
        </Checkbox>
      </xsl:when>
      <xsl:otherwise>
        <Checkbox checked="unchecked" listener="ActionListener" text="{$text}">
          <actionPerformed set="{$attr-name}" value="true">
            <xsl:call-template name="context"/>
          </actionPerformed>
        </Checkbox>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="InvertedCheckBox">
    <xsl:param name="attr-name"/>
    <xsl:param name="text" select="''"/>
    <xsl:choose>
      <xsl:when test="@*[local-name()=$attr-name]='false'">
        <Checkbox checked="checked" listener="ActionListener" text="{$text}">
          <actionPerformed set="{$attr-name}" value="true">
            <xsl:call-template name="context"/>
          </actionPerformed>
        </Checkbox>
      </xsl:when>
      <xsl:otherwise>
        <Checkbox checked="unchecked" listener="ActionListener" text="{$text}">
          <actionPerformed set="{$attr-name}" value="false">
            <xsl:call-template name="context"/>
          </actionPerformed>
        </Checkbox>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="BooleanRadio">
    <xsl:param name="attr-name"/>
    <xsl:param name="text" select="''"/>
    <xsl:choose>
      <xsl:when test="@*[local-name()=$attr-name]='true'">
        <Radiobutton checked="checked" listener="ActionListener" text="{$text}">
          <actionPerformed set="{$attr-name}" value="false">
            <xsl:call-template name="context"/>
          </actionPerformed>
        </Radiobutton>
      </xsl:when>
      <xsl:otherwise>
        <Radiobutton checked="unchecked" listener="ActionListener" text="{$text}">
          <actionPerformed set="{$attr-name}" value="true">
            <xsl:call-template name="context"/>
          </actionPerformed>
        </Radiobutton>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="InvertedRadio">
    <xsl:param name="attr-name"/>
    <xsl:param name="text" select="''"/>
    <xsl:choose>
      <xsl:when test="@*[local-name()=$attr-name]='false'">
        <Radiobutton checked="checked" listener="ActionListener" text="{$text}">
          <actionPerformed set="{$attr-name}" value="true">
            <xsl:call-template name="context"/>
          </actionPerformed>
        </Radiobutton>
      </xsl:when>
      <xsl:otherwise>
        <Radiobutton checked="unchecked" listener="ActionListener" text="{$text}">
          <actionPerformed set="{$attr-name}" value="false">
            <xsl:call-template name="context"/>
          </actionPerformed>
        </Radiobutton>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!--
      ::
      :: Common vocabulary
      ::
  -->

  <xsl:template match="TBD">
    <Glue/>
    <HBox>
      <Glue/>
      Implemented soon...
      <Glue/>
    </HBox>
    <Glue/>
  </xsl:template>

</xsl:stylesheet>

