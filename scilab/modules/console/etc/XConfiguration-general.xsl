<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version ="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                >
    <!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
       ::
       ::     M A I N   P A N E L :   G E N E R A L
       ::
  -->
    <xsl:template match="environment" mode="tooltip">Settings environment</xsl:template>
    <xsl:template match="environment">
        <Title text="_(Environment)">
            <Grid>
                <Label gridx="1" gridy="1" weightx="0" text="_(Floating point exception (ieee): )"/>
                <Panel gridx="2" gridy="1" weightx="1"/>
                <Select gridx="3" gridy="1" listener="ActionListener">
                    <xsl:variable name="fpe" select="@fpe"/>
                    <actionPerformed choose="fpe">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                    <xsl:for-each select="fpe">
                        <option value="{@floating-point-exception}" key="{@code}">
                            <xsl:if test="@code=$fpe">
                                <xsl:attribute name="selected">selected</xsl:attribute>
                            </xsl:if>
                        </option>
                    </xsl:for-each>
                </Select>
                
                <Label gridx="1" gridy="2" weightx="0" text="_(Printing format: )"/>
                <Select gridx="3" gridy="2" listener="ActionListener">
                    <xsl:variable name="pf" select="@printing-format"/>
                    <actionPerformed choose="printing-format">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                    <xsl:for-each select="printing-format">
                        <option value="{@format}" key="{@code}">
                            <xsl:if test="@code=$pf">
                                <xsl:attribute name="selected">selected</xsl:attribute>
                            </xsl:if>
                        </option>
                    </xsl:for-each>
                </Select>
                <Label gridx="1" gridy="3" weightx="0" text="_(Width: )"/>
                <NumericalSpinner gridx="3"
                          gridy="3"
                          weightx="0"
                          min-value="2"
                          max-value="25"
                          increment="1"
                          length="3"
                          listener="ActionListener"
                          value="{@width}">
                    <actionPerformed choose="width">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </NumericalSpinner>
                <Label gridx="1" gridy="4" weightx="0" text="_(Recursion limit: )"/>
                <Panel gridx="2" gridy="1" weightx="1"/>
                <NumericalSpinner gridx="3" gridy="4" weightx="0" length="3"
                    increment="1"
                    min-value="10"
                    listener="ActionListener"
                    value="{@recursion-limit}">
                    <actionPerformed choose="recursion-limit">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </NumericalSpinner>
            </Grid>
        </Title>
    </xsl:template>
    
    <xsl:template match="languages">
        <xsl:if test="$OS='Windows'">
            <VSpace height="10"/>
            <Title text="_(Language setting)">
                <Grid>
                    <Label text="_(Default language: )" gridx="1" gridy="1" anchor="baseline" weightx="0"/>
                    <Panel gridx="2" gridy="1" weightx="1"/>
                    <Select gridx="3" gridy="1" listener="ActionListener">
                        <actionPerformed choose="lang">
                            <xsl:call-template name="context"/>
                        </actionPerformed>
                        <xsl:variable name="code" select="@lang"/>
                        <xsl:for-each select="language">
                            <option value="{@desc}" key="{@code}">
                                <xsl:if test="@code=$SCILAB_LANGUAGE or @code=$code">
                                    <xsl:attribute name="selected">selected</xsl:attribute>
                                </xsl:if>
                            </option>
                        </xsl:for-each>
                    </Select>
                    <Label text="_((This requires a restart of Scilab))" font-face="bold" gridx="1" gridy="2" anchor="west" weightx="0"/>
                    <Panel gridx="2" gridy="2" weightx="1" fill="both"/>
                </Grid>
            </Title>
        </xsl:if>
    </xsl:template>
    
    <xsl:template match="java-heap-memory" mode="tooltip"> and java heap size.</xsl:template>
    <xsl:template match="java-heap-memory">
        <VSpace height="10"/>
        <Title text="_(Java Heap Memory)">
            <Grid>
                <Label text="_(Select the memory (in MB) available in Java: )" gridx="1" gridy="1" anchor="baseline" weightx="0"/>
                <Panel gridx="2" gridy="1" weightx="1"/>
                <NumericalSpinner min-value="0"
                          increment="128"
                          length="6"
                          listener="ActionListener"
                          value="{@heap-size}"
                          gridx="3" gridy="1" anchor="baseline" weightx="0">
                    <actionPerformed choose="heap-size">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </NumericalSpinner>
                <Label text="_((This requires a restart of Scilab))" font-face="bold" gridx="1" gridy="2" anchor="west" weightx="0"/>
                <Panel gridx="2" gridy="2" weightx="1" fill="both"/>
            </Grid>
        </Title>
    </xsl:template>

    <xsl:template match="startup">
        <VSpace height="10"/>
        <Title text="_(Start-up directory)">
            <Grid>
                <Radiobutton value="{@use}" expected-value="current" listener="ActionListener" text="_(Use current working directory)" gridx="1" gridy="1" fill="none" weightx="0" anchor="west">
                    <actionPerformed choose="use">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </Radiobutton>
                
                <Radiobutton value="{@use}" expected-value="previous" listener="ActionListener" text="_(Use previous working directory)" gridx="1" gridy="2" fill="none" weightx="0" anchor="west">
                    <actionPerformed choose="use">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </Radiobutton>
                
                <Radiobutton value="{@use}" expected-value="default" listener="ActionListener" text="_(Use default directory)" gridx="1" gridy="3" fill="none" weightx="0" anchor="west">
                    <actionPerformed choose="use">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </Radiobutton>
                
                <FileSelector gridx="2" gridy="3" weightx="1" anchor="above_baseline"
                              listener="EntryListener"
                              href="{@default}"
                              desc="_(Default startup directory)"
                              dir-selection="true"
                              check-entry="true">
                    <xsl:attribute name="enable">
                        <xsl:if test="@use='default'">
                            <xsl:text>true</xsl:text>
                        </xsl:if>
                    </xsl:attribute>
                    <entryChanged choose="default">
                        <xsl:call-template name="context"/>
                    </entryChanged>
                </FileSelector>
            </Grid>
        </Title>
    </xsl:template>
    
    <xsl:template match="tools">
        <Title text="_(Confirmation dialogs)">
            <Grid>
                <xsl:for-each select="tool">
                    <Checkbox
              gridy="{position() + 1}"
              gridx="1"
              listener="ActionListener"
              checked="{@state}"
              text="{@description}">
                        <actionPerformed choose="state">
                            <xsl:call-template name="context"/>
                        </actionPerformed>
                    </Checkbox>
                </xsl:for-each>
            </Grid>
        </Title>
    </xsl:template>
    
    <xsl:template match="layouts">
        <xsl:variable name="id" select="@id"/>
        <Title text="_(Desktop Layout)">
            <Grid>
                <Label gridx="1" gridy="1" weightx="0" text="_(Select a layout)"/>
                <Panel gridx="2" gridy="1" gridheight="1" fill="both"/>
                <Select gridx="3" gridy="1" listener="ActionListener">
                    <actionPerformed choose="id">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                    <xsl:for-each select="layout">
                        <option value="{@name}" key="{@id}">
                            <xsl:if test="@id=$id">
                                <xsl:attribute name="selected">selected</xsl:attribute>
                            </xsl:if>
                        </option>
                    </xsl:for-each>
                </Select>
                <Label text="_((Modify the layout requires to restart Scilab))" font-face="bold" gridx="1" gridy="2" anchor="west" weightx="0"/>
                <Panel gridx="1" gridy="3">
                    <VSpace height="10"/>
                </Panel>
                <Image gridx="1" gridy="4" gridwidth="3">
                    <xsl:attribute name="url">
                        <xsl:value-of select="layout[@id=$id]/@image"/>
                    </xsl:attribute>
                </Image>
                <Panel gridx="1" gridy="5">
                    <VSpace height="10"/>
                </Panel>
                <Checkbox gridx="1" gridy="6" weightx="0" text="_(Save layout on exiting)" checked="{@save-desktop}" selected-value="true" unselected-value="false" listener="ActionListener">
                    <actionPerformed choose="save-desktop">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </Checkbox>
                <Button gridx="3" gridy="6" weightx="0" text="_(Reset layout)" listener="ActionListener">
                    <actionPerformed callback="Reset layout"/>
                </Button>
            </Grid>
        </Title>
    </xsl:template>
    
    <xsl:template match="actions">
        <xsl:variable name="name" select="@name"/>
        <xsl:variable name="current-item" select="action-folder[@name=$name]/action[number(@item)]"/>
        <Title text="_(Keys binding)" fixed-height="false">
            <VBox fixed-height="false">
                <Grid>
                    <Label gridy="1" gridx="1" gridwidth="2" text="_(Filter on action name: )"/>
                    <Entry gridy="2" gridx="1" gridwidth="2" text="{@filter}" listener="EntryListener">
                        <entryChanged choose="filter">
                            <xsl:call-template name="context"/>
                        </entryChanged>
                    </Entry>
                </Grid>
                <VSpace height="10"/>
                <Grid>
                    <VBox gridx="1" gridy="1" weightx="0" anchor="north">
                        <Label text="_(Component)"/>
                        <List item="{@name}" nb-visible-rows="4" listener="ActionListener">
                            <actionPerformed choose="name">
                                <xsl:call-template name="context"/>
                            </actionPerformed>
                            <xsl:for-each select="action-folder">
                                <listElement name="{@name}"/>
                            </xsl:for-each>
                        </List>
                    </VBox>
                    <Panel gridx="2" gridy="1" weightx="0" width="30" height="1" fixed-width="true"/>
                    <Grid gridx="3" gridy="1" weightx="1" anchor="north">
                        <Label gridx="1" gridy="1" weightx="0" weighty="0" anchor="north" text="_(Name: )"/>
                        <Label gridx="2" gridy="1" weightx="0" weighty="0" anchor="west">
                            <xsl:attribute name="text">
                                <xsl:value-of select="$current-item/@name"/>
                            </xsl:attribute>
                        </Label>
                        <Label gridx="1" gridy="2" weightx="0" weighty="0" anchor="north" text="_(Description: )"/>
                        <TextArea gridx="2" gridy="2" weightx="1" weighty="0" anchor="west" editable="false" rows="4">
                            <xsl:attribute name="text">
                                <xsl:value-of select="$current-item/@description"/>
                            </xsl:attribute>
                        </TextArea>
                        <Label gridx="1" gridy="3" weightx="0" weighty="0" anchor="baseline" text="_(Binding: )"/>
                        <Entry gridx="2" gridy="3" weightx="0" weighty="0" anchor="baseline" listener="EntryListener">
                            <xsl:attribute name="text">
                                <xsl:value-of select="$current-item/@key"/>
                            </xsl:attribute>
                            <entryChanged choose="key">
                                <xsl:attribute name="context">
                                    <xsl:for-each select="$current-item/ancestor-or-self::*">
                                        <xsl:if test="not(.=/)">
                                            <xsl:value-of select="count(preceding-sibling::*)+1"/>
                                            <xsl:text>/</xsl:text>
                                        </xsl:if>
                                    </xsl:for-each>
                                </xsl:attribute>
                            </entryChanged>
                        </Entry>
                    </Grid>
                </Grid>
                <VSpace height="10"/>
                <Table mode="select" listener="TableListener" fixed-height="false">
                    <!-- we change this useless attribute (which is not an actuator) to force the table reload) -->
                    <xsl:attribute name="name">
                        <xsl:value-of select="concat($name,@filter)"/>
                    </xsl:attribute>
                    <tableSelect choose="item">
                        <xsl:call-template name="context"/>
                    </tableSelect>
                    <xsl:for-each select="action-folder[@name=$name]/action[contains(
                                translate(@name,'abcdefghijklmnopqrstuvwxyz','ABCDEFGHIJKLMNOPQRSTUVWXYZ'),
                                translate(current()/@filter,'abcdefghijklmnopqrstuvwxyz','ABCDEFGHIJKLMNOPQRSTUVWXYZ'))]">
                        <tableRow binding="{@key}" command="{@name}"/>
                    </xsl:for-each>
                    <tableCol title="_(Action name)" attr="command"/>
                    <tableCol title="_(Key binding)" attr="binding"/>
                </Table>
                <Glue/>
            </VBox>
        </Title>
    </xsl:template>
</xsl:stylesheet>
