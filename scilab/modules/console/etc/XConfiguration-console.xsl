<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version ="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                ><!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
                      ::
                      ::     M A I N   P A N E L :   C O N S O L E

::
                 -->

  <xsl:template match="display">
    <Title text="_(Display)">
      <xsl:variable name="enable">
	<xsl:choose>
	  <xsl:when test="@adapt-to-display='true'">
              <xsl:text>false</xsl:text>
	  </xsl:when>
	  <xsl:otherwise>
              <xsl:text>true</xsl:text>
	  </xsl:otherwise>
	</xsl:choose>
      </xsl:variable>
      <Grid>
	<Checkbox checked="{@adapt-to-display}" selected-value="true" unselected-value="false" listener="ActionListener" text="_(Automatically adapt the display to the console)" gridx="1" gridy="2" fill="none" weightx="0" anchor="west">
          <actionPerformed choose="adapt-to-display">
            <xsl:call-template name="context"/>
          </actionPerformed>
        </Checkbox>
        <Label gridx="1" gridy="3" weightx="0" text="_(Lines to display:)" enable="{$enable}"/>
	<Panel gridx="2" gridy="3" gridheight="4" fill="both"/>
	<NumericalSpinner gridx="3"
                          gridy="3"
			  weightx="0"
                          min-value = "0"
                          increment = "10"
                          length = "4"
                          listener = "ActionListener"
                          value = "{@lines-to-display}"
			  enable="{$enable}">
          <actionPerformed choose="lines-to-display">
            <xsl:call-template name="context"/>
          </actionPerformed>
        </NumericalSpinner>
        <Label gridx="1" gridy="4" weightx="0" text="_(Columns to display:)" enable="{$enable}"/>
	<NumericalSpinner gridx="3"
                          gridy="4"
			  weightx="0"
                          min-value = "0"
                          increment = "10"
                          length = "4"
                          listener = "ActionListener"
                          value = "{@columns-to-display}"
			  enable="{$enable}">
          <actionPerformed choose="columns-to-display">
            <xsl:call-template name="context"/>
          </actionPerformed>
        </NumericalSpinner>
        <Label gridx="1" gridy="5" weightx="0" text="_(Number of lines in console scroll buffer:)"/>
	<NumericalSpinner gridx="3"
                          gridy="5"
			  weightx="0"
                          min-value = "100"
                          increment = "1000"
                          length = "5"
                          listener = "ActionListener"
                          value = "{@scroll-size}">
          <actionPerformed choose="scroll-size">
            <xsl:call-template name="context"/>
          </actionPerformed>
        </NumericalSpinner>
      </Grid>
    </Title>
  </xsl:template>
</xsl:stylesheet>

