<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version ="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                ><!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
                      ::
                      ::     M A I N   P A N E L :   C O N S O L E

::
                 -->

  <xsl:template match="display">
    <Title text="Display">
      <Grid>
        <Panel gridx="1" gridy="1" gridwidth="3">
          <xsl:call-template name="BooleanCheckBox">
            <xsl:with-param name="text" select="'Wrap lines'"/>
            <xsl:with-param name="attr-name" select="'wrap-lines'"/>
          </xsl:call-template>
        </Panel>
        <Label gridx="1" gridy="2" weightx="0" text="Lines to display: "/>
	<Panel gridx="2" gridy="2" gridheight="4" fill="both"/>
	<NumericalSpinner gridx="3"
                          gridy="2"
			  weightx="0"
                          min-value = "25"
                          increment = "25"
                          length = "4"
                          listener = "ActionListener"
                          value = "{@lines-to-display}">
          <actionPerformed choose="lines-to-display">
            <xsl:call-template name="context"/>
          </actionPerformed>
        </NumericalSpinner>
        <Label gridx="1" gridy="3" weightx="0" text="Columns to display: "/>
	<NumericalSpinner gridx="3"
                          gridy="3"
			  weightx="0"
                          min-value = "25"
                          increment = "25"
                          length = "4"
                          listener = "ActionListener"
                          value = "{@columns-to-display}">
          <actionPerformed choose="columns-to-display">
            <xsl:call-template name="context"/>
          </actionPerformed>
        </NumericalSpinner>
        <Label gridx="1" gridy="4" weightx="0" text="Tab size: "/>
	<NumericalSpinner gridx="3"
                          gridy="4"
			  weightx="0"
                          min-value = "1"
                          increment = "1"
                          length = "2"
                          listener = "ActionListener"
                          value = "{@tab-size}">
          <actionPerformed choose="tab-size">
            <xsl:call-template name="context"/>
          </actionPerformed>
        </NumericalSpinner>
        <Label gridx="1" gridy="5" weightx="0" text="Number of lines in console scroll buffer: "/>
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
    <VSpace height="10"/>
  </xsl:template>

  <xsl:template match="accessibility">
    <Title text="Display">
      <VBox>
	  <HTMLTextArea>
          <html>
            To enable keyboard navigation with arrow keys,
            assign shotcuts to the Cursor Up and Cursor Down actions in the
            <U>Shortcuts Panel</U> (for Console).
            <br/>
            To enable or disable the completion,
            assign a shortcut action in the <U>Shortcuts
            panel</U> (for Console). Default shortcut for completion is Tab.
          </html>
        </HTMLTextArea>
        <Glue/>
      </VBox>
    </Title>
  </xsl:template>

</xsl:stylesheet>

