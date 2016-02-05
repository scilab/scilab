<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version ="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                >
    <!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
       ::
       ::     M A I N   P A N E L   :   W E B
       ::
  -->
    
    <xsl:template match="web">
        <Title text="_(Web browser and mailer)">
            <Grid>
                <Checkbox checked="{@default-browser}" selected-value="true" unselected-value="false" listener="ActionListener" text="_(Use default web browser)" gridx="1" gridy="1" fill="none" weightx="0" anchor="west">
                    <actionPerformed choose="default-browser">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </Checkbox>
                <Panel gridx="2" gridy="1" weightx="1"/>
                <Label gridx="1" gridy="2" weightx="0" text="_(Command for web browser: )"/>
                <FileSelector gridx="2" gridy="2" weightx="1" anchor="above_baseline"
                      listener="EntryListener"
                      href="{@command-browser}"
                      dir-selection="false"
                      check-entry="false">
                    <xsl:attribute name="enable">
                        <xsl:if test="@default-browser='false'">
                            <xsl:text>true</xsl:text>
                        </xsl:if>
                    </xsl:attribute>
                    <entryChanged choose="command-browser">
                        <xsl:call-template name="context"/>
                    </entryChanged>
                </FileSelector>
                <Label gridx="2" gridy="3" text="_((Don't forget to quote path containing white spaces))">
                    <xsl:attribute name="enable">
                        <xsl:if test="@default-browser='false'">
                            <xsl:text>true</xsl:text>
                        </xsl:if>
                    </xsl:attribute>
                </Label>
                <Panel height="5" gridx="1" gridy="4">
                    <VSpace height="5"/>
                </Panel>
                <Checkbox checked="{@default-mailer}" selected-value="true" unselected-value="false" listener="ActionListener" text="_(Use default mailer)" gridx="1" gridy="5" fill="none" weightx="0" anchor="west">
                    <actionPerformed choose="default-mailer">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </Checkbox>
                <Panel gridx="2" gridy="5" weightx="1"/>
                <Label gridx="1" gridy="6" weightx="0" text="_(Command for mailer: )"/>
                <FileSelector gridx="2" gridy="6" weightx="1" anchor="above_baseline"
                      listener="EntryListener"
                      href="{@command-mailer}"
                      dir-selection="false"
                      check-entry="false">
                    <xsl:attribute name="enable">
                        <xsl:if test="@default-mailer='false'">
                            <xsl:text>true</xsl:text>
                        </xsl:if>
                    </xsl:attribute>
                    <entryChanged choose="command-mailer">
                        <xsl:call-template name="context"/>
                    </entryChanged>
                </FileSelector>
                <Label gridx="2" gridy="7" text="_((Don't forget to quote path containing white spaces))">
                    <xsl:attribute name="enable">
                        <xsl:if test="@default-mailer='false'">
                            <xsl:text>true</xsl:text>
                        </xsl:if>
                    </xsl:attribute>
                </Label>
            </Grid>
        </Title>
        <VSpace height="10"/>
    </xsl:template>
    
    <xsl:template match="proxy">
        <Title text="_(Proxy settings)">
            <Grid>
                <Checkbox checked="{@enabled}" selected-value="true" unselected-value="false" listener="ActionListener" text="_(Enable proxy)" gridx="1" gridy="1" fill="none" weightx="0" anchor="west">
                    <actionPerformed choose="enabled">
                        <xsl:call-template name="context"/>
                    </actionPerformed>
                </Checkbox>
                <Panel gridx="2" gridy="1" weightx="1"/>
                <Label gridx="1" gridy="2" weightx="0" text="_(Host: )"/>
                <Entry gridx="2" gridy="2" weightx="1" anchor="above_baseline"
               listener="EntryListener"
               text="{@host}">
                    <xsl:attribute name="enable">
                        <xsl:if test="@enabled='true'">
                            <xsl:text>true</xsl:text>
                        </xsl:if>
                    </xsl:attribute>
                    <entryChanged choose="host">
                        <xsl:call-template name="context"/>
                    </entryChanged>
                </Entry>
                
                <Label gridx="1" gridy="3" weightx="0" text="_(Port: )"/>
                <Entry gridx="2" gridy="3" weightx="1" anchor="above_baseline"
               listener="EntryListener"
               text="{@port}">
                    <xsl:attribute name="enable">
                        <xsl:if test="@enabled='true'">
                            <xsl:text>true</xsl:text>
                        </xsl:if>
                    </xsl:attribute>
                    <entryChanged choose="port">
                        <xsl:call-template name="context"/>
                    </entryChanged>
                </Entry>
                
                <Label gridx="1" gridy="4" weightx="0" text="_(User: )"/>
                <Entry gridx="2" gridy="4" weightx="1" anchor="above_baseline"
               listener="EntryListener"
               text="{@user}">
                    <xsl:attribute name="enable">
                        <xsl:if test="@enabled='true'">
                            <xsl:text>true</xsl:text>
                        </xsl:if>
                    </xsl:attribute>
                    <entryChanged choose="user">
                        <xsl:call-template name="context"/>
                    </entryChanged>
                </Entry>
                
                <Label gridx="1" gridy="5" weightx="0" text="_(Password: )"/>
                <Entry gridx="2" gridy="5" weightx="1" password="true" anchor="above_baseline"
               listener="EntryListener"
               text="{@password}">
                    <xsl:attribute name="enable">
                        <xsl:if test="@enabled='true'">
                            <xsl:text>true</xsl:text>
                        </xsl:if>
                    </xsl:attribute>
                    <entryChanged choose="password">
                        <xsl:call-template name="context"/>
                    </entryChanged>
                </Entry>
            </Grid>
        </Title>
    </xsl:template>
    
    <xsl:template match="previous-proxy"/>
</xsl:stylesheet>
