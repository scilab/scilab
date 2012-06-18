<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version ="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:template match="scinotes-preferences">
    <Title text="Editor">
      <VBox>
        <HBox>
          <xsl:call-template name="BooleanCheckBox">
            <xsl:with-param name="text" select="'Use SciNotes'"/>
            <xsl:with-param name="attr-name" select="'scinotes'"/>
          </xsl:call-template>
          <Glue/>
        </HBox>
        <xsl:variable name="enable">
          <xsl:choose>
            <xsl:when test="@scinotes='true'">
              <xsl:text>false</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>true</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        <Grid>
	  <Radiobutton value="{@external-cmd}" expected-value="true" listener="ActionListener" text="External editor: " gridx="1" gridy="1" fill="none" weightx="0" anchor="west" enable="{$enable}">
	    <actionPerformed choose="external-cmd">
	      <xsl:call-template name="context"/>
	    </actionPerformed>
	  </Radiobutton>
          <FileSelector gridx="2" gridy="1" weightx="1" anchor="above_baseline"
                        listener="EntryListener"
                        href="{@cmd}"
                        dir-selection="false">
	    <xsl:attribute name="enable">
	      <xsl:if test="@scinotes='false' and @external-cmd='true'">
		<xsl:text>true</xsl:text>
	      </xsl:if>
	    </xsl:attribute>
	    <entryChanged choose="cmd">
              <xsl:call-template name="context"/>
            </entryChanged>
          </FileSelector>
	  <Radiobutton value="{@external-cmd}" expected-value="false" listener="ActionListener" text="Scilab command: " gridx="1" gridy="2" fill="none" weightx="0" anchor="west" enable="{$enable}">
	    <actionPerformed choose="external-cmd">
	      <xsl:call-template name="context"/>
	    </actionPerformed>
	  </Radiobutton>
          <Entry gridx="2" gridy="2" weightx="1" anchor="above_baseline"
                        listener="EntryListener"
                        text="{@cmd}">
	    <xsl:attribute name="enable">
	      <xsl:if test="@scinotes='false' and @external-cmd='false'">
		<xsl:text>true</xsl:text>
	      </xsl:if>
	    </xsl:attribute>
            <entryChanged choose="macro">
              <xsl:call-template name="context"/>
            </entryChanged>
          </Entry>
       </Grid>
      </VBox>
    </Title>
    <VSpace height="10"/>
    <Title text="SciNotes General settings">
      <Grid>
        <Panel gridx="1" gridy="1" gridwidth="3">
          <xsl:call-template name="BooleanCheckBox">
            <xsl:with-param name="attr-name" select="'restart-reopen'"/>
            <xsl:with-param name="text" select="'Restore previous session on start-up'"/>
          </xsl:call-template>
        </Panel>
        <Panel gridx="1" gridy="2" gridwidth="3">
          <xsl:call-template name="BooleanCheckBox">
            <xsl:with-param name="attr-name" select="'add-line-termination'"/>
            <xsl:with-param name="text" select="'Add carriage return at the end of the file.'"/>
          </xsl:call-template>
        </Panel>
        <Label gridx="1" gridy="3" weightx="0" text="Number of recently opened files to display: "/>
        <Panel gridx="2" gridy="3" gridheight="2" fill="both"/>
        <NumericalSpinner gridx="3"
                          gridy="3"
                          weightx="0"
                          min-value = "0"
                          increment = "1"
                          length = "4"
                          listener = "ActionListener"
                          value = "{@number-of-recently-open}">
          <actionPerformed choose="number-of-recently-open">
            <xsl:call-template name="context"/>
          </actionPerformed>
        </NumericalSpinner>
        <Label gridx="1" gridy="4" weightx="0" text="Default file encoding: "/>
        <Panel gridx="3" gridy="4">
          <xsl:call-template name="Select">
            <xsl:with-param name="among">
              <option encoding="utf-8"/>
              <option encoding="iso-8859-1"/>
              <option encoding="windows-1252"/>
            </xsl:with-param>
          </xsl:call-template>
        </Panel>
        <Label gridx="1" gridy="5" weightx="0" text="Default End-Of-Line: "/>
        <Panel gridx="3" gridy="5">
          <xsl:call-template name="Select">
            <xsl:with-param name="among">
              <option eol="Unix (LF)"/>
              <option eol="Windows (CR+LF)"/>
              <option eol="Mac OS (CR)"/>
            </xsl:with-param>
          </xsl:call-template>
        </Panel>
      </Grid>
    </Title>
  </xsl:template>

  <xsl:template match="scinotes-display">
    <Title text="General display options">
      <Grid>
	<Checkbox checked="{@highlight-current-line}" selected-value="true" unselected-value="false" listener="ActionListener" text="Highlight current line: " gridx="1" gridy="1" fill="none" weightx="0" anchor="west">
	  <actionPerformed choose="highlight-current-line">
	    <xsl:call-template name="context"/>
	  </actionPerformed>
	</Checkbox>
	<Panel gridx="2" gridy="1" weightx="1"/>
	<Panel gridx="3" gridy="1" weightx="1"/>
	<Color color="{@current-line-color}" listener="ActionListener" gridx="4" gridy="1" weightx="0" enable="{@highlight-current-line}">
	  <actionPerformed choose="current-line-color">
	    <xsl:call-template name="context"/>
	  </actionPerformed>
	</Color>
	<Checkbox checked="{@show-line-numbers}" selected-value="true" unselected-value="false" listener="ActionListener" text="Show line numbers: " gridx="1" gridy="2" fill="none" weightx="0" anchor="west">
	  <actionPerformed choose="show-line-numbers">
	    <xsl:call-template name="context"/>
	  </actionPerformed>
	</Checkbox>
	<Radiobutton value="{@whereami}" expected-value="true" listener="ActionListener" text="Whereami compatible" gridx="2" gridy="2" fill="none" weightx="0" anchor="west" enable="{@show-line-numbers}">
	  <actionPerformed choose="whereami">
	    <xsl:call-template name="context"/>
	  </actionPerformed>
	</Radiobutton>
	<Radiobutton value="{@whereami}" expected-value="false" listener="ActionListener" text="Normal" gridx="3" gridy="2" gridwidth="2" fill="none" weightx="0" anchor="west" enable="{@show-line-numbers}">
	  <actionPerformed choose="whereami">
	    <xsl:call-template name="context"/>
	  </actionPerformed>
	</Radiobutton>
	<Checkbox checked="{@wrap-lines}" selected-value="true" unselected-value="false" listener="ActionListener" text="Wrap lines" gridx="1" gridy="3" fill="none" weightx="0" anchor="west">
	  <actionPerformed choose="wrap-lines">
	    <xsl:call-template name="context"/>
	  </actionPerformed>
	</Checkbox>
      </Grid>
    </Title>
    <VSpace height="10"/>
    <Title text="Keywords">
        <Grid>
	  <Checkbox checked="{@keywords-colorization}" selected-value="true" unselected-value="false" listener="ActionListener" text="Enable keywords colorization" gridx="1" gridy="1" fill="none" weightx="0" anchor="west">
	    <actionPerformed choose="keywords-colorization">
	      <xsl:call-template name="context"/>
	    </actionPerformed>
	  </Checkbox>
	  <Panel gridx="2" gridy="1" weightx="1"/>
	  <Link gridx="3" gridy="1" weightx="0" listener="MouseListener" text="Set syntax Colors" enable="{@keywords-colorization}">
	    <mouseClicked set="path" value="3/" context="/" enable="{@keywords-colorization}"/>
	  </Link>
	  <Checkbox checked="{@highlight-brackets}" selected-value="true" unselected-value="false" listener="ActionListener" text="Highlight corresponding brackets" gridx="1" gridy="2" fill="none" weightx="0" anchor="west">
	    <actionPerformed choose="highlight-brackets">
	      <xsl:call-template name="context"/>
	    </actionPerformed>
	  </Checkbox>
	  <Grid gridx="1" gridy="3" gridwidth="3" ipadx="10">
	    <Label gridx="1" gridy="1" weightx="0" text="Color: " enable="{@highlight-brackets}"/>	  
	    <Color color="{@brackets-color}" listener="ActionListener" gridx="2" gridy="1" weightx="0" enable="{@highlight-brackets}">
	      <actionPerformed choose="brackets-color">
		<xsl:call-template name="context"/>
	      </actionPerformed>
	    </Color>
	    <Panel gridx="3" gridy="1" weightx="1"/>
	    <Checkbox checked="{@brackets-onmouseover}" selected-value="true" unselected-value="false" listener="ActionListener" text="Highlight on mouse over" gridx="4" gridy="1" fill="none" weightx="0" anchor="west" enable="{@highlight-brackets}">
	      <actionPerformed choose="brackets-onmouseover">
		<xsl:call-template name="context"/>
	      </actionPerformed>
	    </Checkbox>
	    <Panel gridx="5" gridy="1" weightx="1"/>
	    <Label gridx="6" gridy="1" weightx="0" text="Style: " enable="{@highlight-brackets}"/>	  
	    <Panel gridx="7" gridy="1" weightx="0">
	      <xsl:call-template name="Select">
		<xsl:with-param name="among">
		  <option brackets-highlightment="Filled"/>
		  <option brackets-highlightment="Framed"/>
		  <option brackets-highlightment="Underlined"/>
		</xsl:with-param>
		<xsl:with-param name="enable" select="@highlight-brackets" />
	      </xsl:call-template>
	    </Panel>
	  </Grid>
	  <Checkbox checked="{@highlight-keywords}" selected-value="true" unselected-value="false" listener="ActionListener" text="Highlight corresponding keywords (e.g. if ... end)" gridx="1" gridy="4" fill="none" weightx="0" anchor="west">
	    <actionPerformed choose="highlight-keywords">
	      <xsl:call-template name="context"/>
	    </actionPerformed>
	  </Checkbox>
	  <Grid gridx="1" gridy="5" gridwidth="3" ipadx="10">
	    <Label gridx="1" gridy="1" weightx="0" text="Color: " enable="{@highlight-keywords}"/>	  
	    <Color color="{@keywords-color}" listener="ActionListener" gridx="2" gridy="1" weightx="0" enable="{@highlight-keywords}">
	      <actionPerformed choose="keywords-color">
		<xsl:call-template name="context"/>
	      </actionPerformed>
	    </Color>
	    <Panel gridx="3" gridy="1" weightx="1"/>
	    <Checkbox checked="{@keywords-onmouseover}" selected-value="true" unselected-value="false" listener="ActionListener" text="Highlight on mouse over" gridx="4" gridy="1" fill="none" weightx="0" anchor="west" enable="{@highlight-keywords}">
	      <actionPerformed choose="keywords-onmouseover">
		<xsl:call-template name="context"/>
	      </actionPerformed>
	    </Checkbox>
	    <Panel gridx="5" gridy="1" weightx="1"/>
	    <Label gridx="6" gridy="1" weightx="0" text="Style: " enable="{@highlight-keywords}"/>	  
	    <Panel gridx="7" gridy="1" weightx="0">
	      <xsl:call-template name="Select">
		<xsl:with-param name="among">
		  <option keywords-highlightment="Filled"/>
		  <option keywords-highlightment="Framed"/>
		</xsl:with-param>
		<xsl:with-param name="enable" select="@highlight-keywords" />
	      </xsl:call-template>
	    </Panel>
	  </Grid>
	</Grid>
    </Title>
    <VSpace height="10"/>
    <Title text="Tabulation &amp; Indentation">
      <Grid>
        <Label text="Tabulation size:" gridx="1" gridy="1" weightx="0"/>
	<NumericalSpinner gridx="2"
                          gridy="1"
			  weightx="0"
                          min-value = "1"
                          increment = "1"
                          length = "3"
                          listener = "ActionListener"
                          value = "{@tab-size}">
          <actionPerformed choose="tab-size">
            <xsl:call-template name="context"/>
          </actionPerformed>
        </NumericalSpinner>
        <Panel gridx="3" gridy="1" weightx="1"/>
	<Checkbox checked="{@use-spaces}" selected-value="true" unselected-value="false" listener="ActionListener" text="Use space" gridx="4" gridy="1" fill="none" weightx="0" anchor="west">
	  <actionPerformed choose="use-spaces">
	    <xsl:call-template name="context"/>
	  </actionPerformed>
	</Checkbox>

	<Label text="Representation:" gridx="1" gridy="2" weightx="0"/>
	<Radiobutton value="{@tab-representation}" expected-value="chevrons" listener="ActionListener" text="Chevrons" gridx="2" gridy="2" fill="none" weightx="0" anchor="west" enable="true">
	  <actionPerformed choose="tab-representation">
	    <xsl:call-template name="context"/>
	  </actionPerformed>
	</Radiobutton>
	<Radiobutton value="{@tab-representation}" expected-value="hrule" listener="ActionListener" text="Horizontal rule" gridx="3" gridy="2" fill="none" weightx="0" anchor="west" enable="true">
	  <actionPerformed choose="tab-representation">
	    <xsl:call-template name="context"/>
	  </actionPerformed>
	</Radiobutton>
	<Radiobutton value="{@tab-representation}" expected-value="vrule" listener="ActionListener" text="Vertical rule" gridx="4" gridy="2" fill="none" weightx="0" anchor="west" enable="true">
	  <actionPerformed choose="tab-representation">
	    <xsl:call-template name="context"/>
	  </actionPerformed>
	</Radiobutton>

	<Checkbox checked="{@automatic-indent}" selected-value="true" unselected-value="false" listener="ActionListener" text="Enable auto indentation" gridx="1" gridy="3" fill="none" weightx="0" anchor="west">
	  <actionPerformed choose="use-spaces">
	    <xsl:call-template name="context"/>
	  </actionPerformed>
	</Checkbox>
        <Label gridy="4" text="Indent size:"/>
        <Panel gridy="4" gridx="2" >
          <xsl:call-template name="Select">
            <xsl:with-param name="among">
              <option indent-size="2"/>
              <option indent-size="4"/>
              <option indent-size="8"/>
            </xsl:with-param>
          </xsl:call-template>
        </Panel>
      </Grid>
    </Title>
  </xsl:template>

  <!--xsl:template match="scinotes-syntax">
    <VSpace height="20"/>
    <Title text="Tabs &amp; Indents">
      <Grid ipady="30" insets="large">
        <Label text="Tab size:"/>
        <Panel gridx="2" >
          <xsl:call-template name="Select">
            <xsl:with-param name="among">
              <option tab-size="2"/>
              <option tab-size="4"/>
              <option tab-size="8"/>
            </xsl:with-param>
          </xsl:call-template>
        </Panel>
        <Panel gridx="3">
          <xsl:call-template name="BooleanCheckBox">
            <xsl:with-param name="attr-name" select="'use-spaces'"/>
            <xsl:with-param name="text" select="'Tab key insert spaces'"/>
          </xsl:call-template>
        </Panel>
        <HBox gridy="2" gridwidth="3">
          View
          <HSpace width="2"/>
          <Link listener="MouseListener" text="Scinotes tab keys preferences">
            <mouseClicked callback="customScinotesColors"/>
          </Link>
        </HBox>
        <Label gridy="3" text="Indent size:"/>
        <Panel gridy="3" gridx="2" >
          <xsl:call-template name="Select">
            <xsl:with-param name="among">
              <option indent-size="2"/>
              <option indent-size="4"/>
              <option indent-size="8"/>
            </xsl:with-param>
          </xsl:call-template>
        </Panel>
        <HBox gridy="4" gridwidth="3">
          <xsl:call-template name="BooleanCheckBox">
            <xsl:with-param name="attr-name" select="'automatic-indent'"/>
            <xsl:with-param name="text" select="' Enable automatic indenting'"/>
          </xsl:call-template>
          <Glue/>
        </HBox>
      </Grid>
    </Title>
  </xsl:template-->

  <xsl:template match="scinotes-autosave">
    <HBox>
      <xsl:call-template name="BooleanCheckBox">
        <xsl:with-param name="attr-name" select="'enable'"/>
      </xsl:call-template>
      Enable autosave in Scinotes
      <Glue/>
    </HBox>
    <xsl:choose>
      <xsl:when test="@enable='true'">
        <Title text="Save options">
          <HBox>
            Save every
            <xsl:call-template name="Select">
              <xsl:with-param name="among">
                <option save-every="2"/>
                <option save-every="3"/>
                <option save-every="5"/>
                <option save-every="7"/>
                <option save-every="11"/>
                <option save-every="13"/>
              </xsl:with-param>
            </xsl:call-template>
            minutes
            <Glue/>
            <xsl:call-template name="BooleanCheckBox">
              <xsl:with-param name="attr-name" select="'save-untitled'"/>
            </xsl:call-template>
            Save untitled files
          </HBox>
        </Title>
        <Title text="Close options">
          <HBox>
            <xsl:call-template name="BooleanCheckBox">
              <xsl:with-param name="attr-name" select="'automatic-delete'"/>
            </xsl:call-template>
            Delete automatically saved files
            <Glue/>
          </HBox>
        </Title>
        <Title text="Filename">
          <Grid>
            <Panel>
              <xsl:call-template name="BooleanRadio">
                <xsl:with-param name="attr-name" select="'append-flag'"/>
                <xsl:with-param name="text" select="'Append filename with'"/>
              </xsl:call-template>
            </Panel>
            <Entry gridx="2" enable="{@append-flag='true'}" text="{@append-with}" listener="ActionListener">
              <actionPerformed choose="append-with">
                <xsl:call-template name="context"/>
              </actionPerformed>
            </Entry>
            <Panel gridy="2">
              <xsl:call-template name="InvertedRadio">
                <xsl:with-param name="attr-name" select="'append-flag'"/>
                <xsl:with-param name="text" select="'Replace extension with'"/>
              </xsl:call-template>
            </Panel>
            <Entry gridy="2" gridx="2" enable="{@append-flag='false'}" text="{@replace-with}" listener="ActionListener">
              <actionPerformed choose="replace-with">
                <xsl:call-template name="context"/>
              </actionPerformed>
            </Entry>
          </Grid>
        </Title>
        <Title text="Location">
          <Grid>
            <Panel>
              <xsl:call-template name="BooleanRadio">
                <xsl:with-param name="attr-name" select="'source-flag'"/>
                <xsl:with-param name="text" select="'Source file directory'"/>
              </xsl:call-template>
            </Panel>
            <Panel gridy="2" >
              <xsl:call-template name="InvertedRadio">
                <xsl:with-param name="attr-name" select="'source-flag'"/>
                <xsl:with-param name="text" select="'Single directory:'"/>
              </xsl:call-template>
            </Panel>
            <FileSelector gridx="2" gridy="2" enable="{@source-flag='false'}" href="{@single-directory}" mask="*"
                          desc="Choose an autosave directory"
                          dir-selection = "true"
                          listener="DocumentListener">
              <actionPerformed choose="single-directory">
                <xsl:call-template name="context"/>
              </actionPerformed>
            </FileSelector>
          </Grid>
        </Title>
      </xsl:when>
      <xsl:otherwise>
        <VSpace height="350"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
</xsl:stylesheet>

