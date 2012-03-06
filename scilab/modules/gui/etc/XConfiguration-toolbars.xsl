<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version ="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:template match="toolbars">
    <xsl:variable name="select" select="@select"/>
    <Title text="Choose toolbar">
      <Select listener="ActionListener">
        <actionPerformed choose="select">
          <xsl:call-template name="context"/>
        </actionPerformed>
        <xsl:for-each select="$toolbars/toolbar">
          <option value="{@title}">
            <xsl:if test="@title=$select">
              <xsl:attribute name="selected">selected</xsl:attribute>
            </xsl:if>
          </option>
        </xsl:for-each>
      </Select>
    </Title>
    <xsl:for-each select="toolbar[@title=$select]">
      <xsl:variable name="context"><xsl:call-template name="here"/></xsl:variable>
      <xsl:variable name="reference" select="$toolbars/toolbar[ @title=$select]"/>
      <xsl:variable name="sequence"  select="*"/>
      <xsl:variable name="count"     select="count(*)"/>

      <!-- reverse position to preserve position vs insertion
      -->
      <xsl:variable name="reversed-position">
        <xsl:choose>
          <xsl:when test="@position &gt; $count">
            <xsl:text>1</xsl:text>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="1 + $count - @position"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <!-- Tip to compare position to attribute value
      -->
      <xsl:variable name="comparable-position"><xsl:value-of select="$reversed-position"/></xsl:variable>
      <xsl:variable name="comparable-selection"><xsl:value-of select="@selection"/></xsl:variable>
      <xsl:variable name="selection" select="$reference/item[ format-number(position(),0)=$comparable-selection]"/>
      <xsl:variable name="position" select="*[ format-number(position(),0)=$comparable-position]"/>
      <!--/
      -->

      <Title refresh="{@title}" text="Proposed layout">
        <HBox>
          <Glue/>
          <xsl:for-each select="*">
            <VBox>
              <xsl:choose>
                <xsl:when test="format-number(position(),0)=$comparable-position">
                  <xsl:attribute name="background">#aaaaff</xsl:attribute>
                </xsl:when>
              </xsl:choose>
              <Label text=" " listener="MouseListener">
                <mouseClicked set="position" value="{1 + $count - position()}" context="{$context}"/>
              </Label>
              <xsl:apply-templates select="." mode="button">
                <xsl:with-param name="reference" select="$reference"/>
                <xsl:with-param name="context"   select="$context"/>
                <xsl:with-param name="position"  select="1 + $count - position()"/>
              </xsl:apply-templates>
            </VBox>
          </xsl:for-each>
          <VBox>
            <xsl:choose>
              <xsl:when test="format-number($count + 1,0)=$comparable-position">
                <xsl:attribute name="background">#aaaaff</xsl:attribute>
              </xsl:when>
            </xsl:choose>
            <Label text="    " listener="MouseListener">
              <mouseClicked set="position" value="0" context="{$context}"/>
            </Label>
            <Label text="::::" listener="MouseListener">
              <mouseClicked set="position" value="0" context="{$context}"/>
            </Label>
          </VBox>
          <Glue/>
          <Label text=""/> <!-- Workaround for proper deletion of glue -->
        </HBox>
      </Title>
      <Title text="Use these buttons to move, delete or add separator.">
        <Grid ipadx="5">
          <Panel  gridx="1" gridheight="1" weightx="10"/>
          <Button gridx="2" text="&lt;" enable="{@position &lt; $count}" listener="ActionListener">
            <actionPerformed set="position" value="{@position + 1}" context="{$context}"/>
          </Button>
          <Button gridx="3" text="&gt;" enable="{@position &gt; 0}" listener="ActionListener">
            <actionPerformed set="position" value="{@position - 1}" context="{$context}"/>
          </Button>
          <Button gridx="4" text="X" listener="ActionListener">
            <xsl:if test="format-number($count + 1,0)=$comparable-position">
              <xsl:attribute name="enable">false</xsl:attribute>
            </xsl:if>
            <actionPerformed delete="{$comparable-position}" context="{$context}"/>
          </Button>
          <Button gridx="5" text="|+" listener="ActionListener">
            <actionPerformed insert="{$comparable-position}" context="{$context}">
              <separator/>
            </actionPerformed>
          </Button>
          <Panel gridx="6" gridheight="3" weightx="10"/>
        </Grid>
      </Title>
      <Title refresh="{@title}" text="Action set for chosen toolbar">
        <xsl:variable name="item" select="@item"/>
        <Scroll>
          <Grid>
            <xsl:for-each select="$reference/*">
              <xsl:variable name="checked" select="@title=$sequence/@title"/>
              <Checkbox gridy="{position() + 1}" text="{@title}" listener="ActionListener">
                <xsl:choose>
                  <xsl:when test="$checked">
                    <xsl:attribute name="checked">checked</xsl:attribute>
                    <actionPerformed context="{$context}" delete="{1 + count($sequence[@title = current()/@title]/preceding-sibling::*)}"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:attribute name="checked">unchecked</xsl:attribute>
                    <actionPerformed insert="{$comparable-position}" context="{$context}">
                      <item title="{@title}"/>
                    </actionPerformed>
                  </xsl:otherwise>
                </xsl:choose>
              </Checkbox>
              <Icon gridy="{position() + 1}" gridx="2" src="{@icon}"/>
            </xsl:for-each>
          </Grid>
        </Scroll>
      </Title>
    </xsl:for-each>
  </xsl:template>

  <xsl:template match="item" mode="button">
    <xsl:param name="reference"/>
    <xsl:param name="position"/>
    <xsl:param name="context"/>
    <Icon src="{$reference/item[@title = current()/@title]/@icon}" listener="MouseListener">
      <mouseClicked set="position" value="{$position}" context="{$context}"/>
    </Icon>
  </xsl:template>

  <xsl:template match="separator" mode="button">
    <xsl:param name="count"/>
    <xsl:param name="position"/>
    <xsl:param name="context"/>
    <Label text="|" listener="MouseListener">
      <mouseClicked set="position" value="{$position}" context="{$context}"/>
    </Label>
  </xsl:template>

</xsl:stylesheet>

