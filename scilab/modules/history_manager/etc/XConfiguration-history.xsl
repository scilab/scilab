<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version ="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                >

  <xsl:template match="history-save">
    <Title text="Saving">
      <VBox>
        <Grid>
          <Radiobutton gridx="1" gridy="1" weightx="0" anchor="baseline" listener="ActionListener" text="Save after ">
            <xsl:if test="@kind='save'">
              <xsl:attribute name="checked">checked</xsl:attribute>
            </xsl:if>
            <actionPerformed set="kind" value="save">
              <xsl:call-template name="context"/>
            </actionPerformed>
          </Radiobutton>
	  <xsl:variable name="enable">
	      <xsl:choose>
                <xsl:when test="@kind='save'">
                  <xsl:text>true</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:text>false</xsl:text>
                </xsl:otherwise>
              </xsl:choose>
	  </xsl:variable>
	  <NumericalSpinner gridx="2"
			    gridy="1"
			    weightx="0"
			    anchor="baseline"
			    min-value="0"
			    increment="100"
			    length="5"
			    listener="ActionListener"
			    value="{@after}"
			    enable="{$enable}">
	    <actionPerformed choose="after">
	      <xsl:call-template name="context"/>
	    </actionPerformed>
	  </NumericalSpinner>
	  <Label gridx="3" gridy="1" weightx="0" anchor="baseline" text=" command(s)" enable="{$enable}"/>
	  <Panel gridx="4" gridy="1"/>
        </Grid>
        <HBox>
          <Radiobutton listener="ActionListener" text="Save history file on quit">
            <xsl:if test="@kind='quit'">
              <xsl:attribute name="checked">checked</xsl:attribute>
            </xsl:if>
            <actionPerformed set="kind" value="quit">
              <xsl:call-template name="context"/>
            </actionPerformed>
          </Radiobutton>
          <Glue/>
        </HBox>
        <HBox>
          <Radiobutton listener="ActionListener" text="Disable history management">
            <xsl:if test="@kind='disable'">
              <xsl:attribute name="checked">checked</xsl:attribute>
            </xsl:if>
            <actionPerformed set="kind" value="disable">
              <xsl:call-template name="context"/>
            </actionPerformed>
          </Radiobutton>
          <Glue/>
        </HBox>
        <VSpace height="20"/>
        <HBox>
          <Checkbox listener="ActionListener" checked="{@consecutive}" text="Save consecutive duplicate commands">
            <actionPerformed choose="consecutive">
              <xsl:call-template name="context"/>
            </actionPerformed>
          </Checkbox>
          <Glue/>
        </HBox>
      </VBox>
    </Title>
  </xsl:template>

  <xsl:template match="history-settings">
    <Title text="Settings">
      <Grid>
        <Label gridx="1" gridy="1" text="History file:"/>
        <File gridx="2" gridy="1" href="{@history-file}" mask="*.scilab" desc="Choose a history file" listener="ActionListener">
          <actionPerformed choose="history-file">
            <xsl:call-template name="context"/>
          </actionPerformed>
        </File>
        <Label gridx="1" gridy="2" text="History lines:"/>
        <Panel gridx="2" gridy="2" >
          <xsl:call-template name="Select">
            <xsl:with-param name="among">
              <option history-line="250"/>
              <option history-line="500"/>
              <option history-line="1000"/>
              <option history-line="2000"/>
              <option history-line="4000"/>
            </xsl:with-param>
          </xsl:call-template>
        </Panel>
      </Grid>
    </Title>
    <VSpace height="120"/>
  </xsl:template>


</xsl:stylesheet>

