<?xml version='1.0' encoding='utf-8'?>

<xsl:stylesheet version ="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    
    <!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
       ::
       ::     M A I N   P A N E L   :   C O L O R S
       ::
  -->
    
    
    
    <xsl:template match="desktop-colors">
        
        <Title text="_(Desktop color)">
            
            <Grid>
                
                <xsl:choose>
                    
                    <xsl:when test="@*[local-name()='use-system-color']='true'">
                        
                        <Checkbox gridx="1" gridy="1" fill="none" weightx="0" anchor="above_baseline" checked="checked" listener="ActionListener" text="_(Use system color)">
                            
                            <actionPerformed set="use-system-color" value="false">
                                
                                <xsl:call-template name="context"/>
                                
                            </actionPerformed>
                            
                        </Checkbox>
                        
                    </xsl:when>
                    
                    <xsl:otherwise>
                        
                        <Checkbox gridx="1" gridy="1" fill="none" weightx="0" anchor="above_baseline" checked="unchecked" listener="ActionListener" text="_(Use system color)">
                            
                            <actionPerformed set="use-system-color" value="true">
                                
                                <xsl:call-template name="context"/>
                                
                            </actionPerformed>
                            
                        </Checkbox>
                        
                    </xsl:otherwise>
                    
                </xsl:choose>
                
                <xsl:variable name="enable">
                    
                    <xsl:choose>
                        
                        <xsl:when test="@use-system-color='true'">
                            
                            <xsl:text>false</xsl:text>
                            
                        </xsl:when>
                        
                        <xsl:otherwise>
                            
                            <xsl:text>true</xsl:text>
                            
                        </xsl:otherwise>
                        
                    </xsl:choose>
                    
                </xsl:variable>
                
                <Panel gridx="2" gridy="1" weightx="1"/>
                
                <Label text="_(Text )" gridx="3" gridy="1" fill="none" weightx="0" anchor="above_baseline" enable="{$enable}"/>
                
                <Color color="{@text}" gridx="4" gridy="1" fill="none" weightx="0" anchor="above_baseline" listener="ActionListener" enable="{$enable}">
                    
                    <actionPerformed choose="text">
                        
                        <xsl:call-template name="context"/>
                        
                    </actionPerformed>
                    
                </Color>
                
                <Panel gridx="5" gridy="1" weightx="1"/>
                
                <Label text="_(Background )" gridx="6" gridy="1" fill="none" weightx="0" anchor="above_baseline" enable="{$enable}"/>
                
                <Color color="{@background}" gridx="7" gridy="1" fill="none" weightx="0" anchor="above_baseline" listener="ActionListener" enable="{$enable}">
                    
                    <actionPerformed choose="background">
                        
                        <xsl:call-template name="context"/>
                        
                    </actionPerformed>
                    
                </Color>
                
                <Panel gridx="8" gridy="1" weightx="1"/>
                
                <Label text="_(Cursor )" gridx="9" gridy="1" fill="none" weightx="0" anchor="above_baseline" enable="{$enable}"/>
                
                <Color color="{@cursor}" gridx="10" gridy="1" fill="none" weightx="0" anchor="above_baseline" listener="ActionListener" enable="{$enable}">
                    
                    <actionPerformed choose="cursor">
                        
                        <xsl:call-template name="context"/>
                        
                    </actionPerformed>
                    
                </Color>
                
            </Grid>
            
        </Title>
        
        <VSpace height="10"/>
        
    </xsl:template>
    
    
    
    <xsl:template match="syntax-highlighting-colors">
        
        <Title text="_(Syntax highlighting colors)" fixed-height="false">
            
            <VBox fixed-height="false">
                
                <Grid>
                    
                    <List gridx="1" gridy="1" weightx="0" item="{@name}" nb-visible-rows="4" listener="ActionListener">
                        
                        <actionPerformed choose="name">
                            
                            <xsl:call-template name="context"/>
                            
                        </actionPerformed>
                        
                        <xsl:for-each select="item">
                            
                            <listElement name="{@name}"/>
                            
                        </xsl:for-each>
                        
                    </List>
                    
                    <Panel gridx="2" gridy="1" weightx="1"/>
                    
                    <VBox gridx="3" gridy="1" weightx="0">
                        
                        <xsl:variable name="var" select="@name"/>
                        
                        <xsl:for-each select="item[@name=$var]">
                            
                            <Grid>
                                
                                <Label text="_(Keyword color )" gridx="1" gridy="1" fill="none" weightx="0" anchor="above_baseline"/>
                                
                                <Color color="{@color}" gridx="2" gridy="1" fill="none" weightx="0" anchor="above_baseline" listener="ActionListener">
                                    
                                    <actionPerformed choose="color">
                                        
                                        <xsl:call-template name="context"/>
                                        
                                    </actionPerformed>
                                    
                                </Color>
                                
                                <Panel gridx="3" gridy="1" fill="horizontal"/>
                                
                            </Grid>
                            
                            <Glue/>
                            
                            <HBox>
                                
                                <xsl:call-template name="BooleanCheckBox">
                                    
                                    <xsl:with-param name="attr-name" select="'bold'"/>
                                    
                                    <xsl:with-param name="text" select="'_(&lt;html&gt;&lt;b&gt;Bold&lt;/b&gt;&lt;html&gt;)'"/>
                                    
                                </xsl:call-template>
                                
                            </HBox>
                            
                            <Glue/>
                            
                            <HBox>
                                
                                <xsl:call-template name="BooleanCheckBox">
                                    
                                    <xsl:with-param name="attr-name" select="'italic'"/>
                                    
                                    <xsl:with-param name="text" select="'_(&lt;html&gt;&lt;i&gt;Italic&lt;/i&gt;&lt;html&gt;)'"/>
                                    
                                </xsl:call-template>
                                
                            </HBox>
                            
                            <Glue/>
                            
                            <HBox>
                                
                                <xsl:call-template name="BooleanCheckBox">
                                    
                                    <xsl:with-param name="attr-name" select="'underline'"/>
                                    
                                    <xsl:with-param name="text" select="'_(&lt;html&gt;&lt;u&gt;Underline&lt;/u&gt;&lt;html&gt;)'"/>
                                    
                                </xsl:call-template>
                                
                            </HBox>
                            
                            <Glue/>
                            
                            <HBox>
                                
                                <xsl:call-template name="BooleanCheckBox">
                                    
                                    <xsl:with-param name="attr-name" select="'strike-through'"/>
                                    
                                    <xsl:with-param name="text" select="'_(&lt;html&gt;&lt;s&gt;Strike-through&lt;/s&gt;&lt;html&gt;)'"/>
                                    
                                </xsl:call-template>
                                
                            </HBox>
                            
                            <Glue/>
                            
                        </xsl:for-each>
                        
                    </VBox>
                    
                </Grid>
                
                <VSpace height="10"/>
                
                <PreviewCode listener="ActionListener">
                    
                    <actionPerformed choose="name">
                        
                        <xsl:call-template name="context"/>
                        
                    </actionPerformed>
                    
                    <xsl:for-each select="item">
                        
                        <tableRow name="{@name}"
		      color="{@color}"
		      underline="{@underline}"
		      strike-through="{@strike-through}"
		      bold="{@bold}"
		      italic="{@italic}" />
                        
                    </xsl:for-each>
                    
                </PreviewCode>
                
                <Glue/>
                
            </VBox>
            
        </Title>
        
    </xsl:template>
    
</xsl:stylesheet>



