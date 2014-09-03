<?xml version='1.0' encoding='utf-8'?>

<xsl:stylesheet version ="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:template match="scinotes-preferences">
        <Title text="_(Editor)">
            <VBox>
                <HBox>
                    <Checkbox checked="{@scinotes}" selected-value="true" unselected-value="false" listener="ActionListener" text="_(Use Scinotes)">
                        <actionPerformed choose="scinotes">
                            <xsl:call-template name="context"/>
                        </actionPerformed>
                    </Checkbox>
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
                    <Radiobutton value="{@external-cmd}" expected-value="true" listener="ActionListener" text="_(External editor: )" gridx="1" gridy="1" fill="none" weightx="0" anchor="west" enable="{$enable}">
                        <actionPerformed choose="external-cmd">
                            <xsl:call-template name="context"/>
                        </actionPerformed>
                    </Radiobutton>
                    <FileSelector gridx="2" gridy="1" weightx="1" anchor="above_baseline"
                        listener="EntryListener"
                        href="{@cmd}"
                        dir-selection="false"
                        check-entry="false">
                        <xsl:attribute name="enable">
                            <xsl:if test="@scinotes='false' and @external-cmd='true'">
                                <xsl:text>true</xsl:text>
                            </xsl:if>
                        </xsl:attribute>
                        <entryChanged choose="cmd">
                            <xsl:call-template name="context"/>
                        </entryChanged>
                    </FileSelector>
                    <Label gridx="2" gridy="2" text="_((Don't forget to quote path containing white spaces))">
                        <xsl:attribute name="enable">
                            <xsl:if test="@scinotes='false' and @external-cmd='true'">
                                <xsl:text>true</xsl:text>
                            </xsl:if>
                        </xsl:attribute>
                    </Label>
                    <Panel height="5" gridx="1" gridy="3">
                        <VSpace height="5"/>
                    </Panel>
                    <Radiobutton value="{@external-cmd}" expected-value="false" listener="ActionListener" text="_(Scilab command: )" gridx="1" gridy="4" fill="none" weightx="0" anchor="west" enable="{$enable}">
                        <actionPerformed choose="external-cmd">
                            <xsl:call-template name="context"/>
                        </actionPerformed>
                    </Radiobutton>
                    <Entry gridx="2" gridy="4" weightx="1" anchor="above_baseline"
                 listener="EntryListener"
                 text="{@macro}">
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
        <Title text="_(Scinotes General settings)">
            <Grid>
                <Panel gridx="1" gridy="1" gridwidth="3">
                    <xsl:call-template name="BooleanCheckBox">
                        <xsl:with-param name="attr-name" select="'restart-reopen'"/>
                        <xsl:with-param name="text" select="'_(Restore previous session on start-up)'"/>
                    </xsl:call-template>
                </Panel>
                
                <Panel gridx="1" gridy="2" gridwidth="3">
                    <xsl:call-template name="BooleanCheckBox">
                        <xsl:with-param name="attr-name" select="'add-line-termination'"/>
                        <xsl:with-param name="text" select="'_(Add carriage return at the end of the file.)'"/>
                    </xsl:call-template>
                </Panel>
                
                <Label gridx="1" gridy="3" weightx="0" text="_(Number of recently opened files to display: )"/>
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
                
                <Label gridx="1" gridy="4" weightx="0" text="_(Default file encoding: )"/>
                
                <Panel gridx="3" gridy="4">
                    <xsl:call-template name="Select">
                        <xsl:with-param name="among">
                            <option encoding="utf-8"/>
                            <option encoding="iso-8859-1"/>
                            <option encoding="windows-1252"/>
                        </xsl:with-param>
                    </xsl:call-template>
                </Panel>
                
                <Label gridx="1" gridy="5" weightx="0" text="_(Default End-Of-Line: )"/>
                
                <Select gridx="3" gridy="5" listener="ActionListener">
                    <actionPerformed choose="eol">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                    <xsl:variable name="eol">
                        <xsl:choose>
                            <xsl:when test="@eol=''">
                                <xsl:choose>
                                    <xsl:when test="$OS='Windows'">
                                        <xsl:text>Windows (CR+LF)</xsl:text>
                                    </xsl:when>
                                    <xsl:otherwise>
                                        <xsl:text>Unix (LF)</xsl:text>
                                    </xsl:otherwise>
                                </xsl:choose>
                            </xsl:when>
                            <xsl:otherwise>
                                <xsl:value-of select="@eol"/>
                            </xsl:otherwise>
                        </xsl:choose>
                    </xsl:variable>
                    <option value="Unix (LF)">
                        <xsl:if test="$eol='Unix (LF)'">
                            <xsl:attribute name="selected">selected</xsl:attribute>
                        </xsl:if>
                    </option>
                    <option value="Windows (CR+LF)">
                        <xsl:if test="$eol='Windows (CR+LF)'">
                            <xsl:attribute name="selected">selected</xsl:attribute>
                        </xsl:if>
                    </option>
                </Select>
            </Grid>
        </Title>
	<VSpace height="10"/>
	<Title text="_(Completion)">
	    <Checkbox checked="{@complete-at-eol}" selected-value="true" unselected-value="false" listener="ActionListener" text="_(Auto-complete brackets when cursor is at the end of a line)">
              <actionPerformed choose="complete-at-eol">
		<xsl:call-template name="context"/>
              </actionPerformed>
            </Checkbox>
	</Title>
    </xsl:template>
    
    
    <xsl:template match="scinotes-display">
        <Title text="_(General display options)">
            <Grid>
                <Checkbox checked="{@highlight-current-line}" selected-value="true" unselected-value="false" listener="ActionListener" text="_(Highlight current line: )" gridx="1" gridy="1" fill="none" weightx="0" anchor="west">
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
                
                <Checkbox checked="{@show-line-numbers}" selected-value="true" unselected-value="false" listener="ActionListener" text="_(Show line numbers: )" gridx="1" gridy="2" fill="none" weightx="0" anchor="west">
                    <actionPerformed choose="show-line-numbers">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </Checkbox>
                
                <Radiobutton value="{@whereami}" expected-value="true" listener="ActionListener" text="_(Local numbering)" gridx="2" gridy="2" fill="none" weightx="0" anchor="west" enable="{@show-line-numbers}">
                    <actionPerformed choose="whereami">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </Radiobutton>
                
                <Radiobutton value="{@whereami}" expected-value="false" listener="ActionListener" text="_(Normal)" gridx="3" gridy="2" gridwidth="2" fill="none" weightx="0" anchor="west" enable="{@show-line-numbers}">
                    <actionPerformed choose="whereami">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </Radiobutton>
                
                <Checkbox checked="{@wrap-lines}" selected-value="true" unselected-value="false" listener="ActionListener" text="_(Wrap lines)" gridx="1" gridy="3" fill="none" weightx="0" anchor="west">
                    <actionPerformed choose="wrap-lines">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </Checkbox>
                
                <Label text="_(Background color: )" gridx="1" gridy="4" weightx="0"/>
                <Color color="{@background-color}" listener="ActionListener" gridx="2" gridy="4" fill="none" weightx="0" anchor="west">
                    <actionPerformed choose="background-color">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </Color>
                
                <Label text="_(Cursor color: )" gridx="3" gridy="4" weightx="0"/>
                <Color color="{@caret-color}" listener="ActionListener" gridx="4" gridy="4" weightx="0" fill="none" anchor="east">
                    <actionPerformed choose="caret-color">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </Color>
            </Grid>
        </Title>
        
        <VSpace height="10"/>
        
        <Title text="_(Keywords)">
            <Grid>
                <Checkbox checked="{@keywords-colorization}" selected-value="true" unselected-value="false" listener="ActionListener" text="_(Enable keywords colorization)" gridx="1" gridy="1" fill="none" weightx="0" anchor="west">
                    <actionPerformed choose="keywords-colorization">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </Checkbox>
                
                <Panel gridx="2" gridy="1" weightx="1"/>
                <Link gridx="3" gridy="1" weightx="0" listener="MouseListener" text="_(Set syntax Colors)" enable="{@keywords-colorization}">
                    <mouseClicked set="path" value="colors" context="/" enable="{@keywords-colorization}"/>
                </Link>
                <Checkbox checked="{@highlight-brackets}" selected-value="true" unselected-value="false" listener="ActionListener" text="_(Highlight corresponding brackets)" gridx="1" gridy="2" fill="none" weightx="0" anchor="west">
                    <actionPerformed choose="highlight-brackets">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </Checkbox>
                <Grid gridx="1" gridy="3" gridwidth="3" ipadx="10">
                    <Label gridx="1" gridy="1" weightx="0" text="_(Color: )" enable="{@highlight-brackets}"/>
                    <Color color="{@brackets-color}" listener="ActionListener" gridx="2" gridy="1" weightx="0" enable="{@highlight-brackets}">
                        <actionPerformed choose="brackets-color">
                            <xsl:call-template name="context"/>
                        </actionPerformed>
                    </Color>
                    <Panel gridx="3" gridy="1" weightx="1"/>
                    <Checkbox checked="{@brackets-onmouseover}" selected-value="true" unselected-value="false" listener="ActionListener" text="_(Highlight on mouse over)" gridx="4" gridy="1" fill="none" weightx="0" anchor="west" enable="{@highlight-brackets}">
                        <actionPerformed choose="brackets-onmouseover">
                            <xsl:call-template name="context"/>
                        </actionPerformed>
                    </Checkbox>
                    <Panel gridx="5" gridy="1" weightx="1"/>
                    <Label gridx="6" gridy="1" weightx="0" text="_(Style: )" enable="{@highlight-brackets}"/>
                    <Select gridx="7" gridy="1" listener="ActionListener" enable="{@highlight-brackets}">
                        <xsl:variable name="bh" select="@brackets-highlightment"/>
                        <actionPerformed choose="brackets-highlightment">
                            <xsl:call-template name="context"/>
                        </actionPerformed>
                        <option value="_(Filled)" key="Filled">
                            <xsl:if test="'Filled'=$bh">
                                <xsl:attribute name="selected">selected</xsl:attribute>
                            </xsl:if>
                        </option>
                        <option value="_(Framed)" key="Framed">
                            <xsl:if test="'Framed'=$bh">
                                <xsl:attribute name="selected">selected</xsl:attribute>
                            </xsl:if>
                        </option>
                        <option value="_(Underlined)" key="Underlined">
                            <xsl:if test="'Underlined'=$bh">
                                <xsl:attribute name="selected">selected</xsl:attribute>
                            </xsl:if>
                        </option>
                    </Select>
                </Grid>
                
                <Checkbox checked="{@highlight-keywords}" selected-value="true" unselected-value="false" listener="ActionListener" text="_(Highlight corresponding keywords (e.g. if ... end))" gridx="1" gridy="4" fill="none" weightx="0" anchor="west">
                    <actionPerformed choose="highlight-keywords">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </Checkbox>
                
                <Grid gridx="1" gridy="5" gridwidth="3" ipadx="10">
                    <Label gridx="1" gridy="1" weightx="0" text="_(Color: )" enable="{@highlight-keywords}"/>
                    <Color color="{@keywords-color}" listener="ActionListener" gridx="2" gridy="1" weightx="0" enable="{@highlight-keywords}">
                        <actionPerformed choose="keywords-color">
                            <xsl:call-template name="context"/>
                        </actionPerformed>
                    </Color>
                    <Panel gridx="3" gridy="1" weightx="1"/>
                    <Checkbox checked="{@keywords-onmouseover}" selected-value="true" unselected-value="false" listener="ActionListener" text="_(Highlight on mouse over)" gridx="4" gridy="1" fill="none" weightx="0" anchor="west" enable="{@highlight-keywords}">
                        <actionPerformed choose="keywords-onmouseover">
                            <xsl:call-template name="context"/>
                        </actionPerformed>
                    </Checkbox>
                    <Panel gridx="5" gridy="1" weightx="1"/>
                    <Label gridx="6" gridy="1" weightx="0" text="_(Style: )" enable="{@highlight-keywords}"/>
                    <Select gridx="7" gridy="1" listener="ActionListener" enable="{@highlight-keywords}">
                        <xsl:variable name="kh" select="@keywords-highlightment"/>
                        <actionPerformed choose="keywords-highlightment">
                            <xsl:call-template name="context"/>
                        </actionPerformed>
                        <option value="_(Filled)" key="Filled">
                            <xsl:if test="'Filled'=$kh">
                                <xsl:attribute name="selected">selected</xsl:attribute>
                            </xsl:if>
                        </option>
                        <option value="_(Framed)" key="Framed">
                            <xsl:if test="'Framed'=$kh">
                                <xsl:attribute name="selected">selected</xsl:attribute>
                            </xsl:if>
                        </option>
                    </Select>
                </Grid>
            </Grid>
        </Title>
        
        <VSpace height="10"/>
        
        <Title text="_(Tabulation and Indentation)">
            <Grid>
                <Label text="_(Tabulation size: )" gridx="1" gridy="1" weightx="0"/>
                <NumericalSpinner gridx="2"
                          gridy="1"
                          weightx="0"
                          min-value = "0"
                          increment = "1"
                          length = "3"
                          listener = "ActionListener"
                          value = "{@tab-size}">
                    <actionPerformed choose="tab-size">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </NumericalSpinner>
                
                <Panel gridx="3" gridy="1" weightx="1"/>
                
                <Checkbox checked="{@use-spaces}" selected-value="true" unselected-value="false" listener="ActionListener" text="_(Use space)" gridx="4" gridy="1" fill="none" weightx="0" anchor="west">
                    <actionPerformed choose="use-spaces">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </Checkbox>
                
                <xsl:variable name="enable">
                    <xsl:choose>
                        <xsl:when test="@use-spaces='true'">
                            <xsl:text>false</xsl:text>
                        </xsl:when>
                        <xsl:otherwise>
                            <xsl:text>true</xsl:text>
                        </xsl:otherwise>
                    </xsl:choose>
                </xsl:variable>
                
                <Label text="_(Representation: )" gridx="1" gridy="2" weightx="0" enable="{$enable}"/>
                
                <Radiobutton value="{@tab-representation}" expected-value="chevrons" listener="ActionListener" text="_(Chevrons)" gridx="2" gridy="2" fill="none" weightx="0" anchor="west" enable="{$enable}">
                    <actionPerformed choose="tab-representation">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </Radiobutton>
                
                <Radiobutton value="{@tab-representation}" expected-value="hrule" listener="ActionListener" text="_(Horizontal rule)" gridx="3" gridy="2" fill="none" weightx="0" anchor="center" enable="{$enable}">
                    <actionPerformed choose="tab-representation">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </Radiobutton>
                
                <Radiobutton value="{@tab-representation}" expected-value="vrule" listener="ActionListener" text="_(Vertical rule)" gridx="4" gridy="2" fill="none" weightx="0" anchor="west" enable="{$enable}">
                    <actionPerformed choose="tab-representation">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </Radiobutton>
                
                <Checkbox checked="{@automatic-indent}" selected-value="true" unselected-value="false" listener="ActionListener" text="_(Enable auto indentation)" gridx="1" gridy="3" fill="none" weightx="0" anchor="west">
                    <actionPerformed choose="automatic-indent">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </Checkbox>
                
                <Label gridy="4" text="_(Indent size: )" enable="{@automatic-indent}"/>
                
                <NumericalSpinner gridx="2"
                          gridy="4"
                          weightx="0"
                          min-value = "0"
                          increment = "1"
                          length = "3"
                          enable="{@automatic-indent}"
                          listener = "ActionListener"
                          value = "{@indent-size}">
                    <actionPerformed choose="indent-size">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </NumericalSpinner>
            </Grid>
        </Title>
    </xsl:template>
    
    
    
    <xsl:template match="scinotes-autosave">
        <Checkbox checked="{@enable}" selected-value="true" unselected-value="false" listener="ActionListener" text="_(Enable autosave in Scinotes)">
            <actionPerformed choose="enable">
                <xsl:call-template name="context"/>
            </actionPerformed>
        </Checkbox>
        
        <VSpace height="10"/>
        
        <Title text="_(Save options)">
            <Grid>
                <Label gridx="1" gridy="1" weightx="0" anchor="west" text="_(Save every )" enable="{@enable}"/>
                <NumericalSpinner gridx="2"
                          gridy="1"
                          weightx="0"
                          min-value = "1"
                          increment = "1"
                          length = "4"
                          listener = "ActionListener"
                          enable = "{@enable}"
                          value = "{@save-every}">
                    <actionPerformed choose="save-every">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </NumericalSpinner>
                
                <Label gridx="3" gridy="1" weightx="0" anchor="west" text="_( minutes )" enable="{@enable}"/>
                
                <Panel gridx="4" gridy="1" gridheight="1" fill="both"/>
            </Grid>
        </Title>
        
        <VSpace height="10"/>
        
        <Title text="_(Close options)">
            <Checkbox checked="{@automatic-delete}" selected-value="true" unselected-value="false" listener="ActionListener" text="_(Delete automatically saved files)" enable="{@enable}">
                
                <actionPerformed choose="automatic-delete">
                    <xsl:call-template name="context"/>
                </actionPerformed>
            </Checkbox>
        </Title>
        
        <VSpace height="10"/>
        
        <Title text="_(Filename)">
            <Grid>
                <Radiobutton value="{@append-filename}" expected-value="true" listener="ActionListener" text="_(Append filename with: )" gridx="1" gridy="1" fill="none" weightx="0" anchor="west" enable="{@enable}">
                    <actionPerformed choose="append-filename">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </Radiobutton>
                <Panel gridx="2" gridy="1" gridheight="1" fill="both"/>
                <Entry gridx="3" gridy="1" weightx="1" anchor="above_baseline"
               listener="EntryListener"
               text="{@append-with}">
                    <xsl:attribute name="enable">
                        <xsl:if test="@enable='true' and @append-filename='true'">
                            <xsl:text>true</xsl:text>
                        </xsl:if>
                    </xsl:attribute>
                    <entryChanged choose="append-with">
                        <xsl:call-template name="context"/>
                    </entryChanged>
                </Entry>
                <Label gridx="3" gridy="2" text="_((%date can be used to be replaced by the current date))">
                    <xsl:attribute name="enable">
                        <xsl:if test="@enable='true' and @append-filename='true'">
                            <xsl:text>true</xsl:text>
                        </xsl:if>
                    </xsl:attribute>
                </Label>
                
                <Radiobutton value="{@append-filename}" expected-value="false" listener="ActionListener" text="_(Replace extension with: )" gridx="1" gridy="3" fill="none" weightx="0" anchor="west" enable="{@enable}">
                    <actionPerformed choose="append-filename">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </Radiobutton>
                
                <Entry gridx="3" gridy="3" weightx="1" anchor="above_baseline"
               listener="EntryListener"
               text="{@replace-with}">
                    <xsl:attribute name="enable">
                        <xsl:if test="@enable='true' and @append-filename='false'">
                            <xsl:text>true</xsl:text>
                        </xsl:if>
                    </xsl:attribute>
                    <entryChanged choose="replace-with">
                        <xsl:call-template name="context"/>
                    </entryChanged>
                </Entry>
            </Grid>
        </Title>
        
        <VSpace height="10"/>
        
        <Title text="_(Location)">
            <Grid>
                <Radiobutton value="{@source-flag}" expected-value="true" listener="ActionListener" text="_(Source file directory)" gridx="1" gridy="1" fill="none" weightx="0" anchor="west" enable="{@enable}">
                    <actionPerformed choose="source-flag">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </Radiobutton>
                
                <Radiobutton value="{@source-flag}" expected-value="false" listener="ActionListener" text="_(Single directory)" gridx="1" gridy="2" fill="none" weightx="0" anchor="west" enable="{@enable}">
                    <actionPerformed choose="source-flag">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </Radiobutton>
                
                <Panel gridx="2" gridy="2" gridheight="1" fill="both"/>
                
                <FileSelector gridx="3" gridy="2" href="{@single-directory}" mask="*"
                      desc="_(Choose an autosave directory)"
                      dir-selection = "true"
                      check-entry = "false"
                      listener="EntryListener">
                    <xsl:attribute name="enable">
                        <xsl:if test="@enable='true' and @source-flag='false'">
                            <xsl:text>true</xsl:text>
                        </xsl:if>
                    </xsl:attribute>
                    <entryChanged choose="single-directory">
                        <xsl:call-template name="context"/>
                    </entryChanged>
                </FileSelector>
            </Grid>
        </Title>
    </xsl:template>
    
    <xsl:template match="scinotes-header">
        <Title text="_(Default header)">
            <Grid>
                <Checkbox checked="{@enable}" selected-value="true" unselected-value="false" listener="ActionListener" text="_(Add a default header to new file)" gridx="1" gridy="1" fill="none" weightx="0" anchor="west">
                    <actionPerformed choose="enable">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </Checkbox>
                
                <Panel gridx="2" gridy="1" gridheight="1" fill="both"/>
                
                <TextArea gridx="1" gridy="2" weightx="1" gridwidth="2" weighty="0" anchor="west"
                  editable="true"
                  rows="15"
                  scroll="true"
                  listener="EntryListener"
                  enable="{@enable}">
                    <xsl:attribute name="text">
                        <xsl:value-of select="string(child::node()[1])" disable-output-escaping="yes"/>
                    </xsl:attribute>
                    <entryChanged choose-child="1">
                        <xsl:call-template name="context"/>
                    </entryChanged>
                </TextArea>
            </Grid>
        </Title>
    </xsl:template>
</xsl:stylesheet>
