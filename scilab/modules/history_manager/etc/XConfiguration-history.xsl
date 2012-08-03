<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version ="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                >

  <xsl:template match="history-save"/>

  <xsl:template match="history-settings">
    <Title text="_(History Management Settings)">
        <Grid>
	  <Checkbox checked="{@enable}" selected-value="true" unselected-value="false" listener="ActionListener" text="_(Enable history management)" gridx="1" gridy="1" fill="none" weightx="0" anchor="west">
	    <actionPerformed choose="enable">
	      <xsl:call-template name="context"/>
	    </actionPerformed>
	  </Checkbox>
	  <Label gridx="1" gridy="2" text="_(History file: )" enable="{@enable}"/>
	  <Panel gridx="2" gridy="2" gridheight="2" fill="both"/>
          <FileSelector gridx="3" gridy="2" weightx="1" anchor="above_baseline"
                        listener="EntryListener"
			enable="{@enable}"
                        href="{@history-file}"
			mask="*.*"
			desc="_(History files)"
                        dir-selection="false"
			check-entry="false">
	    <entryChanged choose="history-file">
              <xsl:call-template name="context"/>
            </entryChanged>
          </FileSelector>
	  <Label gridx="1" gridy="3" text="_(History lines: )" enable="{@enable}"/>
	  <NumericalSpinner gridx="3"
                          gridy="3"
                          weightx="0"
			  enable="{@enable}"
                          min-value = "0"
                          increment = "1000"
                          length = "4"
                          listener = "ActionListener"
                          value = "{@history-lines}">
          <actionPerformed choose="history-lines">
            <xsl:call-template name="context"/>
          </actionPerformed>
        </NumericalSpinner>
      </Grid>	  
    </Title>
  </xsl:template>


</xsl:stylesheet>

