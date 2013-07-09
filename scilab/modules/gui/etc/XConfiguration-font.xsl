<?xml version='1.0' encoding='utf-8'?>

<xsl:stylesheet version ="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                >
    
    <!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
       ::
       ::     M A I N   P A N E L   :   F O N T S
       ::
  -->
    
    
    
    <xsl:template match="fonts">
        
        <xsl:variable name="core-desktop-font">
            
            <xsl:choose>
                
                <xsl:when test="@system='true'">
                    
                    <font font-name="Monospaced" font-face="plain" font-size="11"/>
                    
                </xsl:when>
                
                <xsl:otherwise>
                    
                    <font font-name="{@font-name}" font-face="{@font-face}" font-size="{@font-size}"/>
                    
                </xsl:otherwise>
                
            </xsl:choose>
            
        </xsl:variable>
        
        <xsl:variable name="desktop-font" select="nodeset($core-desktop-font)/font"/>
        
        
        
        <Title text="_(Desktop font)">
            
            <VBox>
                
                <HBox>
                    
                    <xsl:call-template name="BooleanCheckBox">
                        
                        <xsl:with-param name="text" select="'_(Use system font)'"/>
                        
                        <xsl:with-param name="attr-name" select="'system'"/>
                        
                    </xsl:call-template>
                    
                    <Glue/>
                    
                </HBox>
                
                <xsl:variable name="enable">
                    
                    <xsl:choose>
                        
                        <xsl:when test="@system='true'">
                            
                            <xsl:text>false</xsl:text>
                            
                        </xsl:when>
                        
                        <xsl:otherwise>
                            
                            <xsl:text>true</xsl:text>
                            
                        </xsl:otherwise>
                        
                    </xsl:choose>
                    
                </xsl:variable>
                
                <Grid>
                    
                    <Label text="_(Desktop text font: )" gridx="1" gridy="1" fill="none" weightx="0" anchor="above_baseline" enable="{$enable}"/>
                    
                    <Panel gridx="2" gridy="1" weightx="1"/>
                    
                    <FontSelector gridx="3" gridy="1" weightx="0" anchor="above_baseline"
                        font-name="{@font-name}"
                        font-face="{@font-face}"
                        font-size="{@font-size}"
                        listener="ActionListener"
                        enable="{$enable}">
                        
                        <actionPerformed choose="font-name, font-face, font-size">
                            
                            <xsl:call-template name="context"/>
                            
                        </actionPerformed>
                        
                    </FontSelector>
                    
                </Grid>
                
            </VBox>
            
        </Title>
        
        
        
        <VSpace height="10"/>
        
        
        
        <Title text="_(Custom font)">
            
            <Grid>
                
                <List gridx="1" gridy="1" weightx="0" item="{@item}" nb-visible-rows="5" listener="ActionListener">
                    
                    <actionPerformed choose="item">
                        
                        <xsl:call-template name="context"/>
                        
                    </actionPerformed>
                    
                    <xsl:for-each select="item">
                        
                        <listElement name="{@name}"/>
                        
                    </xsl:for-each>
                    
                </List>
                
                <Panel gridx="2" gridy="1" weightx="1"/>
                
                <xsl:variable name="var" select="@item"/>
                
                <xsl:for-each select="item[@name=$var]">
                    
                    <VBox gridx="3" gridy="1" weightx="0" anchor="North">
                        
                        <Label text="_(Fonts to use:)"/>
                        
                        <HBox>
                            
                            <xsl:call-template name="BooleanRadio">
                                
                                <xsl:with-param name="text" select="'_(Desktop text font)'"/>
                                
                                <xsl:with-param name="attr-name" select="'desktop'"/>
                                
                            </xsl:call-template>
                            
                        </HBox>
                        
                        <HBox>
                            
                            <xsl:call-template name="InvertedRadio">
                                
                                <xsl:with-param name="text" select="'_(Custom:)'"/>
                                
                                <xsl:with-param name="attr-name" select="'desktop'"/>
                                
                            </xsl:call-template>
                            
                        </HBox>
                        
                        <HBox>
                            
                            <FontSelector font-name="{@font-name}"
                            font-face="{@font-face}"
                            font-size="{@font-size}"
                            listener="ActionListener">
                                
                                <xsl:attribute name="enable">
                                    
                                    <xsl:choose>
                                        
                                        <xsl:when test="@desktop='true'">
                                            
                                            <xsl:text>false</xsl:text>
                                            
                                        </xsl:when>
                                        
                                        <xsl:otherwise>
                                            
                                            <xsl:text>true</xsl:text>
                                            
                                        </xsl:otherwise>
                                        
                                    </xsl:choose>
                                    
                                </xsl:attribute>
                                
                                <actionPerformed choose="font-name, font-face, font-size">
                                    
                                    <xsl:call-template name="context"/>
                                    
                                </actionPerformed>
                                
                            </FontSelector>
                            
                        </HBox>
                        
                    </VBox>
                    
                </xsl:for-each>
                
            </Grid>
            
        </Title>
        
        
        
        <VSpace height="10"/>
        
        
        
        <Title text="_(LaTeX preview)">
            
            <Grid>
                
                <Label gridx="1" gridy="1" fill="none" weightx="1" anchor="west" text="_(LaTeX font size:)"/>
                	
                
                <NumericalSpinner gridx="2"
                          gridy="1"
                          weightx="0"
                          min-value="1"
                          max-value="100"
                          increment="1"
                          length="2"
                          listener="ActionListener"
                          value="{@latex}">
                    
                    <actionPerformed choose="latex">
                        
                        <xsl:call-template name="context"/>
                        
                    </actionPerformed>
                    
                </NumericalSpinner>
                	
                
            </Grid>
            
        </Title>
        
    </xsl:template>
    
</xsl:stylesheet>



