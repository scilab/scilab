<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version ="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
><!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
  ::
  ::     M A I N   P A N E L :   G E N E R A L
  ::
  -->
  <xsl:template match="environment" mode="tooltip">Settings environment</xsl:template>
	<xsl:template match="environment">
		<VSpace height="25"/>
		<Title text="Environment">
			<Grid>
				<Label gridx="1" gridy="1" text="Floating point exception:"/>
				<Panel gridx="2" gridy="1">
					<xsl:call-template name="Select">
						<xsl:with-param name="among">
							<option floating-point-exception="Produces an error"/>
							<option floating-point-exception="Produces a warning"/>
							<option floating-point-exception="Produces Inf or Nan"/>
						</xsl:with-param>
					</xsl:call-template>
				</Panel>
				<Label gridx="1" gridy="2" text="Printing format:"/>
				<Panel gridx="2" gridy="2">
					<xsl:call-template name="Select">
						<xsl:with-param name="among">
							<option printing-format="short"/>
							<option printing-format="long"/>
							<option printing-format="short e"/>
							<option printing-format="long e"/>
							<option printing-format="short g"/>
							<option printing-format="long g"/>
							<option printing-format="variable format"/>
						</xsl:with-param>
					</xsl:call-template>
				</Panel>
				<Label gridx="1" gridy="3" text="Width:"/>
				<Panel gridx="2" gridy="3">
					<xsl:call-template name="Select">
						<xsl:with-param name="among">
							<option printing-font-size="8"/>
							<option printing-font-size="9"/>
							<option printing-font-size="10"/>
							<option printing-font-size="11"/>
							<option printing-font-size="12"/>
							<option printing-font-size="14"/>
							<option printing-font-size="16"/>
							<option printing-font-size="18"/>
							<option printing-font-size="20"/>
							<option printing-font-size="22"/>
							<option printing-font-size="24"/>
							<option printing-font-size="28"/>
							<option printing-font-size="32"/>
							<option printing-font-size="36"/>
							<option printing-font-size="40"/>
							<option printing-font-size="44"/>
							<option printing-font-size="48"/>
							<option printing-font-size="96"/>
						</xsl:with-param>
					</xsl:call-template>
				</Panel>
			</Grid>
		</Title>
	</xsl:template>

  <xsl:template match="java-heap-memory" mode="tooltip"> and java heap size.</xsl:template>
	<xsl:template match="java-heap-memory">
		<xsl:variable name="heap-size">
				<option heap-size="128"/>
				<option heap-size="256"/>
				<option heap-size="512"/>
				<option heap-size="768"/>
				<option heap-size="1024"/>
		</xsl:variable>
		<VSpace height="25"/>
		<Title text="Java Heap Memory">	
			<Grid>
				<Label gridx="1" gridy="1" gridwidth="3" text="&lt;HTML&gt;The Java Heap Memory is the memory available for Java Objects (default value: 128 MB). Increasing this value may help you avoid '&lt;i&gt;OutOfMemoryError: Java Heap Space&lt;/I&gt;' errors."/>
				<Panel gridx="1" gridy="2">
					<xsl:call-template name="Slider">
						<xsl:with-param name="among" select="$heap-size">
						</xsl:with-param>
					</xsl:call-template>
				</Panel>
				<Panel gridx="2" gridy="2">
					<xsl:call-template name="Select">
						<xsl:with-param name="among" select="$heap-size"/>
					</xsl:call-template>
				</Panel>
				<Label gridx="3" gridy="2" text="MB"/>
			</Grid>
		</Title>
		<Glue/>
		<VSpace height="25"/>
	</xsl:template>

	<xsl:template match="tools">
		<VBox>
			<VSpace height="1"/>
			<HBox>
				<HSpace width="200"/>
				<VBox>
					<HBox>&lt;HTML&gt;&lt;I&gt;The following dialog boxes 
					require user confirmation. 
					</HBox>
					<HBox>&lt;HTML&gt;&lt;I&gt;Select a check 
					box if you want that dialog box to appear.
					</HBox>
				</VBox>
			</HBox>

			<Title text="Confirmation dialogs">
				<Grid>
					<Label gridy="1" gridx="1" text="&lt;HTML&gt;&lt;B&gt;Dialog box description"/>
					<Label gridy="1" gridx="2" text="&lt;HTML&gt;&lt;B&gt;Tool"/>
					<Icon  gridy="1" gridx="3" listener="MouseListener">
						<xsl:attribute name="src">
							<xsl:choose>
								<xsl:when test="@order='ascending'">
									<xsl:text>go-up.png</xsl:text>
								</xsl:when>
								<xsl:otherwise>
									<xsl:text>go-down.png</xsl:text>
								</xsl:otherwise>
							</xsl:choose>		
						</xsl:attribute>					
						<mouseClicked set="order">
							<xsl:attribute name="value">
								<xsl:choose>
									<xsl:when test="@order='ascending'">
										<xsl:text>descending</xsl:text>
									</xsl:when>
									<xsl:otherwise>
										<xsl:text>ascending</xsl:text>
									</xsl:otherwise>
								</xsl:choose>				
							</xsl:attribute>			
							<xsl:call-template name="context"/>								
						</mouseClicked>
					</Icon>
					<xsl:for-each select="tool">
						<xsl:sort order="{@order}" select="@name"/>
						<Checkbox 
							gridy    = "{position() + 1}"
							gridx    = "1"
							listener = "ActionListener" 
							checked  = "{@state}" 
							text     = "{@description}"
							>
							<actionPerformed choose="state">
								<xsl:call-template name="context"/>
							</actionPerformed>
						</Checkbox>
						<Label
							gridy    = "{position() + 1}"
							gridx    = "2"
							text     = "{@name}"/>
					</xsl:for-each>
				</Grid>
			</Title>
			<Glue/>
			<HBox>
				<Glue/>
				<Button text="Select All" listener="ActionListener">
					<xsl:for-each select="tool">
						<actionPerformed set="state" value="checked">
							<xsl:call-template name="context"/>
						</actionPerformed>
					</xsl:for-each>
				</Button>
				<Glue/>
				<Button text="Clear All" listener="ActionListener">
					<xsl:for-each select="tool">
						<actionPerformed set="state" value="unchecked">
							<xsl:call-template name="context"/>
						</actionPerformed>
					</xsl:for-each>
				</Button>
				<Glue/>
			</HBox>
		</VBox>
	</xsl:template>

	<xsl:template match="actions">
		<Grid>
			<Label gridy="1" gridx="1" text="Active settings: "/>
			<Panel gridy="1" gridx="2">
				<xsl:call-template name="Select">
					<xsl:with-param name="among">
						<option active="scilab"/>
						<option active="emacs"/>
						<option active="browse..."/>
					</xsl:with-param>
				</xsl:call-template>
			</Panel>
			<File gridy="2" gridx="1" gridwidth="2" href="{@browse}" mask="*.xml" desc="Choose a shortcut description file" listener="ActionListener">
				<xsl:if test="not(@active='browse...')">
					<xsl:attribute name="enable">false</xsl:attribute>
				</xsl:if>
				<actionPerformed choose="browse">
					<xsl:call-template name="context"/>
				</actionPerformed>
			</File>	
			<Label gridy="3" gridx="1" gridwidth="2" text="Strike return to search by action name or shortcut:"/>
			<Entry gridy="4" gridx="1" gridwidth="2" text="{@filter}" listener="ActionListener">
				<actionPerformed choose="filter">
					<xsl:call-template name="context"/>
				</actionPerformed>							
			</Entry>
		</Grid>
		<Title background="#ffffff" text="General shortcuts preferences">
			<VBox>
				<xsl:variable name="filtered-actions" 
					select="action-folder/action[contains(
						translate(
							concat(@description,@ctrl), 
							'abcdefghijklmnopqrstuvwxyz', 
							'ABCDEFGHIJKLMNOPQRSTUVWXYZ'),
						translate(current()/@filter,
							'abcdefghijklmnopqrstuvwxyz', 
							'ABCDEFGHIJKLMNOPQRSTUVWXYZ')
						)]"/>
				<xsl:variable name="filtered-folder" 
					select="action-folder[action [@description=$filtered-actions/@description][@ctrl=$filtered-actions/@ctrl]]"/>
				<HBox>&lt;HTML&gt;&lt;B&gt;Action name 
					<Glue/>
					<Label halign="right" text="&lt;HTML&gt;&lt;B&gt;Shortcut"/>
				</HBox>
				<VBox width="200" height="300">
				<Scroll>
					<Grid >
						<VBox anchor="north">
							<xsl:for-each select="$filtered-folder">
								<HBox>
									<Icon listener="MouseListener">
										<xsl:attribute name="src">
											<xsl:choose>
												<xsl:when test="@state='close'">
													<xsl:text>list-add.png</xsl:text>
												</xsl:when>
												<xsl:otherwise>
													<xsl:text>list-remove.png</xsl:text>
												</xsl:otherwise>
											</xsl:choose>
										</xsl:attribute>
										<mouseClicked set="state">
											<xsl:attribute name="value">
												<xsl:choose>
													<xsl:when test="@state='close'">
														<xsl:text>open</xsl:text>
													</xsl:when>
													<xsl:otherwise>
														<xsl:text>close</xsl:text>
													</xsl:otherwise>
												</xsl:choose>				
											</xsl:attribute>											
											<xsl:call-template name="context"/>
										</mouseClicked>
									</Icon>				
									<Label text="  {@name}"/>
									<Glue/>	
								</HBox>
								<xsl:if test="@state='open'">
									<xsl:for-each select="action[@description=$filtered-actions/@description][@ctrl=$filtered-actions/@ctrl]">
										<HBox>
											<HSpace width="30"/>
											<Label text="{@description}"/>
											<Glue/>
											<Label font-family="Courier 10 Pitch" text="{concat('CTRL + ', @ctrl)}"/>
										</HBox>
									</xsl:for-each>
								</xsl:if>
							</xsl:for-each>
						</VBox>				
					</Grid>			</Scroll>
				</VBox>

			</VBox>
		</Title>
		<HBox>
			<Button text="Restore defaults"/>
			<Glue/>
		</HBox>	
		<Glue/>
	</xsl:template>

</xsl:stylesheet>

