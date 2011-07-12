<?xml version='1.0' encoding='iso-8859-1'?>
<xsl:stylesheet
	version ="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>

<!-- TODO: Font chooser (Font) -->

<!-- TODO: File chooser (File) -->

<!-- TODO: Color chooser (Color) -->

<!-- TODO: Integer chooser (Spinner) -->

<!-- TODO: Free string chooser (Entry) -->

<!-- TODO: Scrolling device (Scroll) -->


<!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
  ::
  ::     T O O L B O X
  ::
  -->

<xsl:variable name="path" select="/interface/@path"/>

<xsl:variable name="types" select="document('types.xml')/TYPES"/>

<xsl:template name="context">
	<!-- 
	  :: 
	  ::  Any callback must be equipped with a 'context' attribute 
	  :: indicating where the rewriting triggered by the event will
	  :: occur.
	  :: 
	  :: 
	  ::  This template encapsulates this design constraint: 
	  :: 
	  :: <{event-callback} {action}="{path}">
	  :: 	<xsl:call-template name="context"/>
	  ::    [...]
	  :: </{event-callback}>
	  :: 
      -->
	<xsl:variable name="local-name" select="local-name()"/>
	<xsl:attribute name="context">
		<xsl:value-of select="$local-name"/>
		<xsl:text>#</xsl:text>
		<xsl:value-of select="count(preceding::*[local-name()=$local-name])"/>
	</xsl:attribute>
</xsl:template>

<xsl:template name="simple-path">
	<xsl:for-each select="ancestor-or-self::*">
		<xsl:value-of select="local-name()"/>
		<xsl:text>/</xsl:text>
	</xsl:for-each>
</xsl:template>

<xsl:template name="enum-options">
	<xsl:param name="actual-value"/>
	<xsl:param name="actual-enum"/>
	<xsl:for-each select="$actual-enum">
		<option>
			<xsl:attribute name="value">
				<xsl:value-of select="."/>
			</xsl:attribute>
			<xsl:if test=".=$actual-value">
				<xsl:attribute name="selected">selected</xsl:attribute>
			</xsl:if>
		</option>
	</xsl:for-each>
</xsl:template>

<xsl:template match="TBD">
	<Glue/>
	<HBox>
		<Glue/>
		Implemented soon...
		<Glue/>
	</HBox>
	<Glue/>
</xsl:template>

<xsl:template match="TBS">
	<Glue/>
	<HBox>
		<Glue/>
		Specification in progress...
		<Glue/>
	</HBox>
	<Glue/>
</xsl:template>

<!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
  ::
  ::     T O P   L E V E L
  ::
  --> 

<xsl:template match="interface">
	<ISML layout="border" style="XConfiguration.css">
		<VBox background="#ffffff" border-side="West" scroll="vertical">
			<xsl:apply-templates select="." mode="left-tree">
				<xsl:with-param name="path" select="$path"/>
				<xsl:with-param name="top-id" select="local-name()"/>
			</xsl:apply-templates>
			<Glue/>
		</VBox>
		<Panel border-side="Center" width="640" height="420">
			<VBox>
				<xsl:apply-templates mode="main-panel">
					<xsl:with-param name="path" select="$path"/>
				</xsl:apply-templates>
			</VBox>
		</Panel>
		<HBox border-side="South">
			<Button text="Help"    listener="ActionListener"> 
				<actionPerformed   callback="Help"/>
			</Button>
			<Glue/>
			<Button text="Default" listener="ActionListener">
				<actionPerformed   callback="Default"/>
			</Button>
			<HSpace width="5"/>
			<Button text="Cancel"  listener="ActionListener">
				<actionPerformed   callback="Cancel"/>
			</Button>
			<HSpace width="5"/>
			<Button text="OK"      listener="ActionListener">
				<actionPerformed   callback="Ok"/>
			</Button>
			<HSpace width="5"/>
			<Button text="Apply"   listener="ActionListener">
				<actionPerformed   callback="Apply"/>
			</Button>
		</HBox>
	</ISML>							
</xsl:template>


<!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
  ::
  ::     L E F T   T R E E 
  ::
  -->


<xsl:template match="interface" mode="left-tree">
	<xsl:param name="path"/>
	<xsl:param name="top-id"/>
	<VBox>
		<xsl:apply-templates mode="left-tree">
			<xsl:with-param name="path" select="$path"/>
			<xsl:with-param name="top-id" select="$top-id"/>
		</xsl:apply-templates>
	</VBox>
</xsl:template>

<xsl:template match="*" mode="left-tree">
	<xsl:param name="path"/>
	<xsl:param name="top-id"/>
	<xsl:variable name="local-path">
		<xsl:call-template name="simple-path"/>
	</xsl:variable>
	<VBox>
		<HBox>
			<Icon>
				<xsl:attribute name="src">
					<xsl:choose>
						<xsl:when test="*[not(name()='body')]">	
							<xsl:text>media-playback-start.png</xsl:text>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>media-playback-start.png</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>
			</Icon>
			<Label>	
				<xsl:attribute name="text">
					<xsl:if test="body/@maturity">&lt;HTML&gt;&lt;<xsl:value-of select="body/@maturity"/>&gt;</xsl:if>
					<xsl:value-of select="local-name()"/>
				</xsl:attribute>
				<xsl:choose>
					<xsl:when test="substring($local-path, 1, string-length($path))=$path and substring-after($local-path, $path)=''">
						<xsl:attribute name="background">#0000ff</xsl:attribute>
						<xsl:attribute name="foreground">#ffffff</xsl:attribute>
					</xsl:when>
					<xsl:otherwise>
						<xsl:attribute name="listener">MouseListener</xsl:attribute>
						<mouseClicked set="path" context="{$top-id}" value="{$local-path}"/>
					</xsl:otherwise>
				</xsl:choose>
			</Label>
			<Glue/>
			<HSpace width="25"/>
		</HBox>
		<xsl:if test="*[not(name()='body')]">
			<HBox>
				<HSpace width="25"/>
				<VBox>
					<xsl:apply-templates mode="left-tree">
						<xsl:with-param name="path" select="$path"/>
						<xsl:with-param name="top-id" select="$top-id"/>	
					</xsl:apply-templates>
				</VBox>
				<Glue/>
			</HBox>
		</xsl:if>
	</VBox>
</xsl:template>


<!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
  ::
  ::     M A I N   P A N E L :   
  ::
  ::
  ::
  ::       body tag are not displayed in 'left-tree' mode
  ::
  ::       body tag are displayed in 'main-panel' mode iff
  ::			interface/@path == $local-path
  ::
  -->

<xsl:template match="body" mode="left-tree"/>

<xsl:template match="body" mode="main-panel">
	<xsl:param name="path"/>
	<xsl:variable name="local-path">
		<xsl:for-each select="ancestor::*">
			<xsl:value-of select="local-name()"/>
			<xsl:text>/</xsl:text>
		</xsl:for-each>
	</xsl:variable>

	<xsl:if test="substring($local-path, 1, string-length($path))=$path and substring-after($local-path, $path)=''">
		<xsl:apply-templates/>
		<!-- xsl:value-of select="$local-path"/ -->
	</xsl:if>
</xsl:template>

<!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
  ::
  ::     M A I N   P A N E L :   G E N E R A L
  ::
  -->

<xsl:template match="environment">
	<Title text="Environment">
		<HBox>
			<VBox>
				<HBox>
					Floating point exception:
					<Select listener="ActionListener">
						<actionPerformed choose="floating-point-exception">
							<xsl:call-template name="context"/>
						</actionPerformed>
						<xsl:call-template name="enum-options">
							<xsl:with-param name="actual-value" select="@floating-point-exception"/>
							<xsl:with-param name="actual-enum" select="$types/floating-point-exception/enum/text()"/>
						</xsl:call-template>
					</Select>
					<Glue/>
				</HBox>
				<VSpace height="25"/>
				<HBox>
					Printing format:
					<Select listener="ActionListener">
						<actionPerformed choose="printing-format">
							<xsl:call-template name="context"/>
						</actionPerformed>
						<xsl:call-template name="enum-options">
							<xsl:with-param name="actual-value" select="@printing-format"/>
							<xsl:with-param name="actual-enum" select="$types/printing-format/enum/text()"/>
						</xsl:call-template>
					</Select>
					<Glue/>
					Width:
					<Select listener="ActionListener">
						<actionPerformed choose="printing-font-size">
							<xsl:call-template name="context"/>
						</actionPerformed>
						<xsl:call-template name="enum-options">
							<xsl:with-param name="actual-value" select="@printing-font-size"/>
							<xsl:with-param name="actual-enum" select="$types/printing-font-size/enum/text()"/>
						</xsl:call-template>
					</Select>
					<Glue/>
				</HBox>
			</VBox>
			<Glue/>
		</HBox>
	</Title>				
</xsl:template>

<xsl:template match="java-heap-memory">
	<Title text="Java Heap Memory">	
		<HBox>
			<VBox>
				<VBox>
					The Java Heap Memory is the memory available for Java Objects (default value: 128 MB).
					<VSpace height="0"/>&lt;HTML&gt;Increasing this value may 
					help you avoid "&lt;i&gt;OutOfMemoryError: Java Heap Space&lt;/I&gt;" 
					errors.
				</VBox>
				<VSpace height="5"/>
				<HBox>
					<Glue/>
					<Slider/>
					<Glue/>
					<Select listener="ActionListener">
						<actionPerformed choose="heap-size">
							<xsl:call-template name="context"/>
						</actionPerformed>
						<xsl:call-template name="enum-options">
							<xsl:with-param name="actual-value" select="@heap-size"/>
							<xsl:with-param name="actual-enum" select="$types/heap-size/enum/text()"/>
						</xsl:call-template>
					</Select>
					MB
					<Glue/>
				</HBox>	
			</VBox>	
			<Glue/>
		</HBox>
	</Title>
	<Glue/>
</xsl:template>

<xsl:template match="tools">
	<VBox>
		<HBox>
			<HSpace width="200"/>
			<VBox>&lt;HTML&gt;&lt;I&gt;The following dialog boxes 
				require user confirmation. 
				<VSpace height="0"/>&lt;HTML&gt;&lt;I&gt;Select a check 
				box if you want that dialog box to appear.
			</VBox>
		</HBox>

		<Title text="Confirmation dialogs">
			<VBox>
				<HBox>&lt;HTML&gt;&lt;B&gt;State
					<HSpace width="150"/>
					<Glue/>&lt;HTML&gt;&lt;B&gt;Dialog box description
					<HSpace width="150"/>
					<Glue/>&lt;HTML&gt;&lt;B&gt;Tool
					<HSpace width="15"/>
					<Icon listener="MouseListener">
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
				</HBox>
				<xsl:for-each select="tool">
					<xsl:sort order="{@order}" select="@name"/>
					<HBox>
						<Checkbox listener="ActionListener" checked="{@state}">
							<actionPerformed choose="state">
								<xsl:call-template name="context"/>
							</actionPerformed>
						</Checkbox>
						<HSpace width="30"/>
						<Label text="{@description}"/>
						<Glue/>
						<Label text="{@name}"/>
					</HBox>
				</xsl:for-each>
				<Glue/>
			</VBox>
		</Title>
		<Glue/>
		<HBox>
			<Glue/>
			<Button text="Select All"/>
			<Glue/>
			<Button text="Clear All"/>
			<Glue/>
		</HBox>
	</VBox>
</xsl:template>

<xsl:template match="actions">
	<xsl:variable name="filter" select="@filter"/>
	<HBox>
		Active settings:
		<Select listener="ActionListener">
			<actionPerformed choose="active">
				<xsl:call-template name="context"/>
			</actionPerformed>
			<xsl:call-template name="enum-options">
				<xsl:with-param name="actual-value" select="@active"/>
				<xsl:with-param name="actual-enum" select="$types/shortcuts-settings/enum/text()"/>
			</xsl:call-template>
		</Select>
		<Glue/>	
	</HBox>
	<xsl:choose>
		<xsl:when test="@active='browse...'">
			<HBox>
				<File/>	
				<Glue/>	
			</HBox>
		</xsl:when>
		<xsl:otherwise>
			<VSpace height="50"/>
		</xsl:otherwise>
	</xsl:choose>
	<HBox>
		Search by action name or shortcut:
		<Glue/>	
	</HBox>
	<Entry/>
	<Title width="650" height="200" background="#ffffff" text="General shortcuts preferences">
		<VBox>
			<HBox>&lt;HTML&gt;&lt;B&gt;Action name
				<HSpace width="150"/>
				<Glue/>
				<xsl:text>&lt;HTML&gt;&lt;B&gt;Shortcut</xsl:text>
			</HBox>
			<Scroll height="200" width="300">
				<VBox>
					<xsl:for-each select="action-folder[action[contains(concat(@description,@ctrl),$filter)]]">
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
							<xsl:for-each select="action[contains(concat(@description,@ctrl),$filter)]">
								<HBox>
									<HSpace width="30"/>
									<Label text="{@description}"/>
									<HSpace width="100"/>							
									<Glue/>
									<Label  text="{concat('CTRL + ', @ctrl)}"/>
								</HBox>
							</xsl:for-each>
						</xsl:if>
					</xsl:for-each>
				<Glue/>
				</VBox>				
			</Scroll>
		<Glue/>	
		</VBox>
	</Title>
	<HBox>
		<Button text="Restore defaults">
			<actionPerformed>
				<xsl:attribute name="load">
					<xsl:choose>
						<xsl:when test="@active='scilab'">
							<xsl:text>scilab-shortcuts-file</xsl:text>
						</xsl:when>
						<xsl:when test="@active='emacs'">
							<xsl:text>emacs-shortcuts-file</xsl:text>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="@file"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>
			</actionPerformed>
		</Button>
		<Glue/>
	</HBox>	
	<Glue/>
</xsl:template>

<!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
  ::
  ::     M A I N   P A N E L   :   F O N T S
  ::
  -->

<xsl:template match="desktop-font">
	<Title text="Desktop font">
		<VBox>
			<HBox>
				<Checkbox listener="ActionListener" checked="{@use-system-font}">
					<actionPerformed choose="use-system-font">
						<xsl:call-template name="context"/>
					</actionPerformed>
				</Checkbox>
				Use system font
				<Glue/>
			</HBox>
			<HBox>
				Desktop text font:
				<Font/>
			</HBox>
			<HBox>
				Sample
				<Glue/>
			</HBox>
			<Sample/>
		</VBox>
	</Title>
</xsl:template>

<xsl:template match="custom-font">
	<xsl:variable name="selected" select="@selected"/>
	<Title text="Custom font">
		<HBox>
			<List/>
			<HSpace width="30"/>
			<xsl:for-each select="font-type[ @name=$selected]">
				<VBox>
					<HBox>&lt;HTML&gt;&lt;u&gt;Fonts to use&lt;/u&gt;:
						<Glue/>	
					</HBox>
					<HBox>
						<Checkbox listener="ActionListener">
							<xsl:if test="@font-to-use='desktop'">
								<xsl:attribute name="checked">checked</xsl:attribute>
							</xsl:if>
							<actionPerformed set="font-to-use" value="desktop">
								<xsl:call-template name="context"/>
							</actionPerformed>
						</Checkbox>
						Desktop text font
						<Glue/>
					</HBox>
					<HBox>
						<Checkbox listener="ActionListener">
							<xsl:if test="@font-to-use='custom'">
								<xsl:attribute name="checked">checked</xsl:attribute>
							</xsl:if>
							<actionPerformed set="font-to-use" value="custom">
								<xsl:call-template name="context"/>
							</actionPerformed>
						</Checkbox>
						Custom:
						<Glue/>
					</HBox>

					<Font/>
					<HBox>
						Sample
						<Glue/>
					</HBox>
					<Sample/>
				</VBox>
			</xsl:for-each>
		</HBox>
	</Title>
</xsl:template>

<!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
  ::
  ::     M A I N   P A N E L   :   C O L O R S
  ::
  -->

<xsl:template match="desktop-colors">
	<Title text="Desktop color">
		<VBox>
			<HBox>
				<Checkbox listener="ActionListener" checked="{@use-system-color}">
					<actionPerformed choose="use-system-color">
						<xsl:call-template name="context"/>
					</actionPerformed>
				</Checkbox>
				Use system color
				<Glue/>
			</HBox>
			<HBox>
				Text
				<Color/>
				<HSpace width="80"/>
				Background
				<Color/>
				<HSpace width="80"/>
				Cursor
				<Color/>
			</HBox>
		</VBox>
	</Title>
</xsl:template>

<xsl:template match="syntax-highlighting-colors">
	<Title text="Syntax highlighting colors">
		<VBox>
			<HBox>
				<VBox background="#ffffff" >
					<HBox>&lt;HTML&gt;&lt;B&gt;Item
						<HSpace width="200"/>
						<Label text="&lt;HTML&gt;&lt;B&gt;Color"/>
					</HBox>
					<!-- Scroll -->
						<VBox>
							<xsl:for-each select="item">
								<HBox>
									<Label text="{@title}"/>
									<Glue/>
									<Color/>
								</HBox>
							</xsl:for-each>
						</VBox>
					<!--/Scroll -->
				</VBox>
				<HSpace width="180"/>
				<TextSample/>
			</HBox>

			<HBox>
				<VBox>
					<HBox>
						<Checkbox/> 
						Bold
						<Glue/>
					</HBox>
					<HBox>
						<Checkbox/> 
						Italic
						<Glue/>
					</HBox>
				</VBox>
				<HSpace width="25"/>
				<VBox>
					<HBox>
						<Checkbox/> 
						Underline
						<Glue/>
					</HBox>
					<HBox>
						<Checkbox/> 
						Strinke Through
						<Glue/>
					</HBox>
				</VBox>
				<Glue/>
				<Button text="Restore Default Settings"/>
			</HBox>
		</VBox>
	</Title>
</xsl:template>

<xsl:template match="display">
	<Title text="Display">
		<VBox>
			<HBox>
				<Checkbox/>
				Wrap Lines
				<Glue/>
			</HBox>
			<HBox>
				Lines to display:
				<Spinner/>
				<HSpace width="70"/>
				Columns to display:
				<Spinner/>
				<Glue/>
			</HBox>
			<HBox>
				Tab size:
				<Spinner/>
				<Glue/>
			</HBox>
			<HBox>
				Number of lines in console scroll buffer:
				<Spinner/>
				<Glue/>
			</HBox>

		</VBox>
	</Title>
</xsl:template>

<xsl:template match="accessibility">
	<Title text="Display">
		<VBox>
			<HBox>
				To enable keyboard navigation with arrow keys, 
				assign shotcuts to the Cursor Up and
				<Glue/>
			</HBox>
			<HBox>&lt;HTML&gt;Cursor Down actions in the 
				&lt;U&gt;Shortcuts Panel&lt;/U&gt; (for Console)
				&lt;/HTML&gt;<Glue/>
			</HBox>
			<VSpace height="25"/>
			<HBox>&lt;HTML&gt;To enable or disable the completion, 
				assign a shortcut action in the &lt;U&gt;Shortcuts 
				panel&lt;/U&gt; (for &lt;/HTML&gt;<Glue/>
			</HBox>
			<HBox>
				Console). Default shortcut for completion is Tab.
				<Glue/>
			</HBox>
		</VBox>
	</Title>
</xsl:template>

<!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
  ::
  ::     M A I N   P A N E L :   C O N S O L E
  ::
  -->

<xsl:template match="saving">
	<Title text="Saving">
		<VBox>
			<HBox>
				<Checkbox listener="ActionListener">
					<xsl:if test="@kind='save'">
						<xsl:attribute name="checked">checked</xsl:attribute>
					</xsl:if>
					<actionPerformed set="kind" value="save">
						<xsl:call-template name="context"/>
					</actionPerformed>
				</Checkbox>
				Save after
				<Spinner/>
				command(s)			
				<Glue/>
			</HBox>
			<HBox>
				<Checkbox listener="ActionListener">
					<xsl:if test="@kind='quit'">
						<xsl:attribute name="checked">checked</xsl:attribute>
					</xsl:if>
					<actionPerformed set="kind" value="quit">
						<xsl:call-template name="context"/>
					</actionPerformed>
				</Checkbox>
				Save history file on quit
				<Glue/>
			</HBox>
			<HBox>
				<Checkbox listener="ActionListener">
					<xsl:if test="@kind='disable'">
						<xsl:attribute name="checked">checked</xsl:attribute>
					</xsl:if>
					<actionPerformed set="kind" value="disable">
						<xsl:call-template name="context"/>
					</actionPerformed>
				</Checkbox>
				Disable history management
				<Glue/>
			</HBox>
			<VSpace height="20"/>
			<HBox>
				<Checkbox listener="ActionListener" checked="{@consecutive}">
					<actionPerformed choose="consecutive">
						<xsl:call-template name="context"/>
					</actionPerformed>
				</Checkbox>
				Save consecutive duplicate commands
				<Glue/>
			</HBox>
		</VBox>
	</Title>
</xsl:template>

<xsl:template match="settings">
	<Title text="Settings">
		<VBox>
			<HBox>
				History file:
				<File/>
			</HBox>
			<HBox>
				History lines:
				<Spinner/>
			</HBox>
		</VBox>
	</Title>
	<VSpace height="80"/>
</xsl:template>

</xsl:stylesheet>

