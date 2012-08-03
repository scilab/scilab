<?xml version='1.0' encoding='utf-8'?>

<xsl:stylesheet version ="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    
    
    
    <!-- Default settings for Xcos -->
    
    <xsl:template match="xcos-preferences">
        
        <HBox>
            
            Xcos Preferences window.
            
            <Glue />
            
        </HBox>
        
    </xsl:template>
    
    <!-- Customize Xcos editing behavior-->
    
    <!-- deactivate on beta2
	<xsl:template match="xcos-edition">
		<HBox>
			Xcos edition window.
			<Glue />
		</HBox>
	</xsl:template>
-->
    
    <!-- Simulation settings for Xcos -->
    
    <xsl:template match="xcos-simulation">
        
        <Title text="_(Default simulation settings)">
            
            <Grid>
                
                <Label gridx="1" gridy="1" text="_(Final integration time)"/>
                
                <ScilabDoubleEntry gridx="2" gridy="1" value="{@final-integration-time}" listener="PropertyChangeListener">
                    
                    <propertyChange name="value" choose="final-integration-time">
                        
                        <xsl:call-template name="context"/>
                        
                    </propertyChange>
                    
                </ScilabDoubleEntry>
                
                <Label gridx="1" gridy="2" text="_(Real time scaling)"/>
                
                <ScilabDoubleEntry gridx="2" gridy="2" value="{@real-time-scaling}" listener="PropertyChangeListener">
                    
                    <propertyChange name="value" choose="real-time-scaling">
                        
                        <xsl:call-template name="context"/>
                        
                    </propertyChange>
                    
                </ScilabDoubleEntry>
                
                <Label gridx="1" gridy="3" text="_(Integrator absolute tolerance)"/>
                
                <ScilabDoubleEntry gridx="2" gridy="3" value="{@integrator-absolute-tolerance}" listener="PropertyChangeListener">
                    
                    <propertyChange name="value" choose="integrator-absolute-tolerance">
                        
                        <xsl:call-template name="context"/>
                        
                    </propertyChange>
                    
                </ScilabDoubleEntry>
                
                <Label gridx="1" gridy="4" text="_(Integrator relative tolerance)"/>
                
                <ScilabDoubleEntry gridx="2" gridy="4" value="{@integrator-relative-tolerance}" listener="PropertyChangeListener">
                    
                    <propertyChange name="value" choose="integrator-relative-tolerance">
                        
                        <xsl:call-template name="context"/>
                        
                    </propertyChange>
                    
                </ScilabDoubleEntry>
                
                <Label gridx="1" gridy="5" text="_(Tolerance on time)"/>
                
                <ScilabDoubleEntry gridx="2" gridy="5" value="{@tolerance-on-time}" listener="PropertyChangeListener">
                    
                    <propertyChange name="value" choose="tolerance-on-time">
                        
                        <xsl:call-template name="context"/>
                        
                    </propertyChange>
                    
                </ScilabDoubleEntry>
                
                <Label gridx="1" gridy="6" text="_(Max integration time interval)"/>
                
                <ScilabDoubleEntry gridx="2" gridy="6" value="{@max-integration-interval}" listener="PropertyChangeListener">
                    
                    <propertyChange name="value" choose="max-integration-interval">
                        
                        <xsl:call-template name="context"/>
                        
                    </propertyChange>
                    
                </ScilabDoubleEntry>
                
                <Label gridx="1" gridy="7" text="_(Solver kind)"/>
                
                <Select gridx="2" gridy="7" listener="ActionListener">
                    
                    <xsl:variable name="solver" select="@solver"/>
                    
                    <actionPerformed choose="solver">
                        
                        <xsl:call-template name="context"/>
                        
                    </actionPerformed>
                    
                    <xsl:for-each select="solver">
                        
                        <option value="{@description}" key="{@code}">
                            
                            <xsl:if test="@code=$solver">
                                
                                <xsl:attribute name="selected">selected</xsl:attribute>
                                
                            </xsl:if>
                            
                        </option>
                        
                    </xsl:for-each>
                    
                </Select>
                
                <Label gridx="1" gridy="8" text="_(Maximum step size (0 means no limit))"/>
                
                <NumericalSpinner gridx="2" gridy="8"
                          min-value = "0"
                          increment = "1"
                          length = "4"
                          listener = "ActionListener"
                          value = "{@max-step-size}">
                    
                    <actionPerformed choose="max-step-size">
                        
                        <xsl:call-template name="context"/>
                        
                    </actionPerformed>
                    
                </NumericalSpinner>
                
            </Grid>
            
        </Title>
        
        <VSpace height="10"/>
        
        <Title text="_(Default trace settings)">
            
            <Select listener="ActionListener">
                
                <xsl:variable name="trace" select="@trace"/>
                
                <actionPerformed choose="trace">
                    
                    <xsl:call-template name="context"/>
                    
                </actionPerformed>
                
                <xsl:for-each select="trace">
                    
                    <option value="{@description}" key="{@code}">
                        
                        <xsl:if test="@code=$trace">
                            
                            <xsl:attribute name="selected">selected</xsl:attribute>
                            
                        </xsl:if>
                        
                    </option>
                    
                </xsl:for-each>
                
            </Select>
            
        </Title>
        
    </xsl:template>
    
    
    
</xsl:stylesheet>

