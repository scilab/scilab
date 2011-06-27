<?xml version='1.0' encoding='iso-8859-1'?>
<xsl:stylesheet
	version ="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>

<!-- TODO: Font chooser (FONT) -->

<!-- TODO: File chooser (FILE) -->

<!-- TODO: Color chooser (COLOR) -->

<!-- TODO: Integer chooser (SPINNER) -->

<!-- TODO: Free string chooser (ENTRY) -->

<!-- TODO: Scrolling device (SCROLL) -->


<!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
  ::
  ::     T O O L B O X
  ::
  -->

<xsl:variable name="path" select="/INTERFACE/@path"/>

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
	<GLUE/>
	<HBOX>
		<GLUE/>
		Implemented soon...
		<GLUE/>
	</HBOX>
	<GLUE/>
</xsl:template>

<xsl:template match="TBS">
	<GLUE/>
	<HBOX>
		<GLUE/>
		Specification in progress...
		<GLUE/>
	</HBOX>
	<GLUE/>
</xsl:template>

<!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
  ::
  ::     T O P   L E V E L
  ::
  -->

<xsl:template match="INTERFACE">
	<ISML layout="border" style="XConfiguration.css">
		<VBOX background="#ffffff" border-side="West" scroll="vertical">
			<xsl:apply-templates select="." mode="left-tree">
				<xsl:with-param name="path" select="$path"/>
				<xsl:with-param name="top-id" select="local-name()"/>
			</xsl:apply-templates>
			<GLUE/>
		</VBOX>
		<PANEL border-side="Center" >
			<VBOX>
				<xsl:apply-templates mode="main-panel">
					<xsl:with-param name="path" select="$path"/>
				</xsl:apply-templates>
			</VBOX>
		</PANEL>
		<HBOX border-side="South">
			<BUTTON text="Help"    listener="ActionListener"> 
				<actionPerformed   callback="Help"/>
			</BUTTON>
			<GLUE/>
			<BUTTON text="Default" listener="ActionListener">
				<actionPerformed   callback="Default"/>
			</BUTTON>
			<BUTTON text="Cancel"  listener="ActionListener">
				<actionPerformed   callback="Cancel"/>
			</BUTTON>
			<BUTTON text="OK"      listener="ActionListener">
				<actionPerformed   callback="Ok"/>
			</BUTTON>
			<BUTTON text="Apply"   listener="ActionListener">
				<actionPerformed   callback="Apply"/>
			</BUTTON>
		</HBOX>
	</ISML>							
</xsl:template>


<!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
  ::
  ::     L E F T   T R E E 
  ::
  -->


<xsl:template match="INTERFACE" mode="left-tree">
	<xsl:param name="path"/>
	<xsl:param name="top-id"/>
	<VBOX>
		<xsl:apply-templates mode="left-tree">
			<xsl:with-param name="path" select="$path"/>
			<xsl:with-param name="top-id" select="$top-id"/>
		</xsl:apply-templates>
	</VBOX>
</xsl:template>

<xsl:template match="*" mode="left-tree">
	<xsl:param name="path"/>
	<xsl:param name="top-id"/>
	<xsl:variable name="local-path">
		<xsl:call-template name="simple-path"/>
	</xsl:variable>
	<VBOX>
		<HBOX>
			<ICON>
				<xsl:attribute name="src">
					<xsl:choose>
						<xsl:when test="*[not(name()='BODY')]">	
							<xsl:text>tree-node-closed.png</xsl:text>
						</xsl:when>
						<xsl:otherwise>
							<xsl:text>tree-item.png</xsl:text>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>
			</ICON>
			<LABEL>	
				<xsl:attribute name="text">
					<xsl:if test="BODY/@maturity">&lt;HTML&gt;&lt;<xsl:value-of select="BODY/@maturity"/>&gt;</xsl:if>
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
			</LABEL>
			<GLUE/>
			<HSPACE width="25"/>
		</HBOX>
		<xsl:if test="*[not(name()='BODY')]">
			<HBOX>
				<HSPACE width="25"/>
				<VBOX>
					<xsl:apply-templates mode="left-tree">
						<xsl:with-param name="path" select="$path"/>
						<xsl:with-param name="top-id" select="$top-id"/>	
					</xsl:apply-templates>
				</VBOX>
				<GLUE/>
			</HBOX>
		</xsl:if>
	</VBOX>
</xsl:template>


<!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
  ::
  ::     M A I N   P A N E L :   
  ::
  ::
  ::
  ::       BODY tag are not displayed in 'left-tree' mode
  ::
  ::       BODY tag are displayed in 'main-panel' mode iff
  ::			INTERFACE/@path == $local-path
  ::
  -->

<xsl:template match="BODY" mode="left-tree"/>

<xsl:template match="BODY" mode="main-panel">
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
	<TITLE text="Environment">
		<HBOX>
			<VBOX>
				<HBOX>
					Floating point exception:
					<SELECT listener="ActionListener">
						<actionPerformed choose="floating-point-exception">
							<xsl:call-template name="context"/>
						</actionPerformed>
						<xsl:call-template name="enum-options">
							<xsl:with-param name="actual-value" select="@floating-point-exception"/>
							<xsl:with-param name="actual-enum" select="$types/floating-point-exception/enum/text()"/>
						</xsl:call-template>
					</SELECT>
					<GLUE/>
				</HBOX>
				<VSPACE height="25"/>
				<HBOX>
					Printing format:
					<SELECT listener="ActionListener">
						<actionPerformed choose="printing-format">
							<xsl:call-template name="context"/>
						</actionPerformed>
						<xsl:call-template name="enum-options">
							<xsl:with-param name="actual-value" select="@printing-format"/>
							<xsl:with-param name="actual-enum" select="$types/printing-format/enum/text()"/>
						</xsl:call-template>
					</SELECT>
					<GLUE/>
					Width:
					<SELECT listener="ActionListener">
						<actionPerformed choose="printing-font-size">
							<xsl:call-template name="context"/>
						</actionPerformed>
						<xsl:call-template name="enum-options">
							<xsl:with-param name="actual-value" select="@printing-font-size"/>
							<xsl:with-param name="actual-enum" select="$types/printing-font-size/enum/text()"/>
						</xsl:call-template>
					</SELECT>
					<GLUE/>
				</HBOX>
			</VBOX>
			<GLUE/>
		</HBOX>
	</TITLE>				
</xsl:template>

<xsl:template match="java-heap-memory">
	<TITLE text="Java Heap Memory">	
		<HBOX>
			<VBOX>
				<VBOX>
					The Java Heap Memory is the memory available for Java Objects (default value: 128 MB).
					<VSPACE height="0"/>&lt;HTML&gt;Increasing this value may 
					help you avoid "&lt;i&gt;OutOfMemoryError: Java Heap Space&lt;/I&gt;" 
					errors.
				</VBOX>
				<VSPACE height="5"/>
				<HBOX>
					<GLUE/>
					<SLIDER/>
					<GLUE/>
					<SELECT listener="ActionListener">
						<actionPerformed choose="heap-size">
							<xsl:call-template name="context"/>
						</actionPerformed>
						<xsl:call-template name="enum-options">
							<xsl:with-param name="actual-value" select="@heap-size"/>
							<xsl:with-param name="actual-enum" select="$types/heap-size/enum/text()"/>
						</xsl:call-template>
					</SELECT>
					MB
					<GLUE/>
				</HBOX>	
			</VBOX>	
			<GLUE/>
		</HBOX>
	</TITLE>
	<GLUE/>
</xsl:template>

<xsl:template match="tools">
	<VBOX>
		<HBOX>
			<HSPACE width="300"/>
			<VBOX>&lt;HTML&gt;&lt;I&gt;The following dialog boxes 
				require user confirmation. 
				<VSPACE height="0"/>&lt;HTML&gt;&lt;I&gt;Select a check 
				box if you want that dialog box to appear.
			</VBOX>
		</HBOX>

		<TITLE text="Confirmation dialogs">
			<VBOX>
				<HBOX>&lt;HTML&gt;&lt;B&gt;State
					<HSPACE width="150"/>
					<GLUE/>&lt;HTML&gt;&lt;B&gt;Dialog box description
					<HSPACE width="150"/>
					<GLUE/>&lt;HTML&gt;&lt;B&gt;Tool
					<HSPACE width="15"/>
					<ICON listener="MouseListener">
						<xsl:attribute name="src">
							<xsl:choose>
								<xsl:when test="@order='ascending'">
									<xsl:text>U.png</xsl:text>
								</xsl:when>
								<xsl:otherwise>
									<xsl:text>D.png</xsl:text>
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
										<xsl:text>ascending</xsl:text>									</xsl:otherwise>

								</xsl:choose>				
							</xsl:attribute>			
							<xsl:call-template name="context"/>								
						</mouseClicked>
					</ICON>
				</HBOX>
				<xsl:for-each select="tool">
					<xsl:sort order="{@order}" select="@name"/>
					<HBOX>
						<CHECKBOX listener="ActionListener" checked="{@state}">
							<actionPerformed choose="state">
								<xsl:call-template name="context"/>
							</actionPerformed>
						</CHECKBOX>
						<HSPACE width="30"/>
						<LABEL text="{@description}"/>
						<GLUE/>
						<LABEL text="{@name}"/>
					</HBOX>
				</xsl:for-each>
				<GLUE/>
			</VBOX>
		</TITLE>
		<VGLUE/>
		<HBOX>
			<GLUE/>
			<BUTTON text="Select All"/>
			<GLUE/>
			<BUTTON text="Clear All"/>
			<GLUE/>
		</HBOX>
	</VBOX>
</xsl:template>

<xsl:template match="actions">
	<xsl:variable name="filter" select="@filter"/>
	<HBOX>
		Active settings:
		<SELECT listener="ActionListener">
			<actionPerformed choose="active">
				<xsl:call-template name="context"/>
			</actionPerformed>
			<xsl:call-template name="enum-options">
				<xsl:with-param name="actual-value" select="@active"/>
				<xsl:with-param name="actual-enum" select="$types/shortcuts-settings/enum/text()"/>
			</xsl:call-template>
		</SELECT>
		<GLUE/>	
	</HBOX>
	<xsl:choose>
		<xsl:when test="@active='browse...'">
			<HBOX>
				<FILE/>	
				<GLUE/>	
			</HBOX>
		</xsl:when>
		<xsl:otherwise>
			<VSPACE height="50"/>
		</xsl:otherwise>
	</xsl:choose>
	<HBOX>
		Search by action name or shortcut:
		<GLUE/>	
	</HBOX>
	<ENTRY/>
	<TITLE width="650" height="200" background="#ffffff" text="General shortcuts preferences">
		<VBOX>
			<HBOX>&lt;HTML&gt;&lt;B&gt;Action name
				<HSPACE width="150"/>
				<GLUE/>
				<xsl:text>&lt;HTML&gt;&lt;B&gt;Shortcut</xsl:text>
			</HBOX>
			<!--SCROLL-->
				<VBOX>
					<xsl:for-each select="action-folder[action[contains(concat(@description,@ctrl),$filter)]]">
						<HBOX>
							<ICON listener="MouseListener">
								<xsl:attribute name="src">
									<xsl:choose>
										<xsl:when test="@state='close'">
											<xsl:text>tree-node-closed.png</xsl:text>
										</xsl:when>
										<xsl:otherwise>
											<xsl:text>tree-node-opened.png</xsl:text>
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
							</ICON>				
							<LABEL text="{@name}"/>
							<GLUE/>	
						</HBOX>
						<xsl:if test="@state='open'">
							<xsl:for-each select="action[contains(concat(@description,@ctrl),$filter)]">
								<HBOX>
									<HSPACE width="30"/>
									<ICON src="tree-item.png"/>
									<LABEL text="{@description}"/>
									<HSPACE width="100"/>							
									<GLUE/>
									<LABEL  text="{concat('CTRL + ', @ctrl)}"/>
								</HBOX>
							</xsl:for-each>
						</xsl:if>
					</xsl:for-each>
				</VBOX>
			<!--/SCROLL-->
		<VGLUE/>
		</VBOX>
	</TITLE>
	<HBOX>
		<BUTTON text="Restore defaults">
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
		</BUTTON>
		<GLUE/>
	</HBOX>	
	<GLUE/>
</xsl:template>

<!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
  ::
  ::     M A I N   P A N E L   :   F O N T S
  ::
  -->

<xsl:template match="desktop-font">
	<TITLE text="Desktop font">
		<VBOX>
			<HBOX>
				<CHECKBOX listener="ActionListener" checked="{@use-system-font}">
					<actionPerformed choose="use-system-font">
						<xsl:call-template name="context"/>
					</actionPerformed>
				</CHECKBOX>
				Use system font
				<GLUE/>
			</HBOX>
			<HBOX>
				Desktop text font:
				<FONT/>
			</HBOX>
			<HBOX>
				Sample
				<GLUE/>
			</HBOX>
			<SAMPLE/>
		</VBOX>
	</TITLE>
</xsl:template>

<xsl:template match="custom-font">
	<xsl:variable name="selected" select="@selected"/>
	<TITLE text="Custom font">
		<HBOX>
			<LIST/>
			<HSPACE width="30"/>
			<xsl:for-each select="font-type[ @name=$selected]">
				<VBOX>
					<HBOX>&lt;HTML&gt;&lt;u&gt;Fonts to use&lt;/u&gt;:
						<GLUE/>	
					</HBOX>
					<HBOX>
						<CHECKBOX listener="ActionListener">
							<xsl:if test="@font-to-use='desktop'">
								<xsl:attribute name="checked">checked</xsl:attribute>
							</xsl:if>
							<actionPerformed set="font-to-use" value="desktop">
								<xsl:call-template name="context"/>
							</actionPerformed>
						</CHECKBOX>
						Desktop text font
						<GLUE/>
					</HBOX>
					<HBOX>
						<CHECKBOX listener="ActionListener">
							<xsl:if test="@font-to-use='custom'">
								<xsl:attribute name="checked">checked</xsl:attribute>
							</xsl:if>
							<actionPerformed set="font-to-use" value="custom">
								<xsl:call-template name="context"/>
							</actionPerformed>
						</CHECKBOX>
						Custom:
						<GLUE/>
					</HBOX>

					<FONT/>
					<HBOX>
						Sample
						<GLUE/>
					</HBOX>
					<SAMPLE/>
				</VBOX>
			</xsl:for-each>
		</HBOX>
	</TITLE>
</xsl:template>

<!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
  ::
  ::     M A I N   P A N E L   :   C O L O R S
  ::
  -->

<xsl:template match="desktop-colors">
	<TITLE text="Desktop color">
		<VBOX>
			<HBOX>
				<CHECKBOX listener="ActionListener" checked="{@use-system-color}">
					<actionPerformed choose="use-system-color">
						<xsl:call-template name="context"/>
					</actionPerformed>
				</CHECKBOX>
				Use system color
				<GLUE/>
			</HBOX>
			<HBOX>
				Text
				<COLOR/>
				<HSPACE width="80"/>
				Background
				<COLOR/>
				<HSPACE width="80"/>
				Cursor
				<COLOR/>
			</HBOX>
		</VBOX>
	</TITLE>
</xsl:template>

<xsl:template match="syntax-highlighting-colors">
	<TITLE text="Syntax highlighting colors">
		<VBOX>
			<HBOX>
				<VBOX background="#ffffff" >
					<HBOX>&lt;HTML&gt;&lt;B&gt;Item
						<HSPACE width="200"/>
						<LABEL text="&lt;HTML&gt;&lt;B&gt;Color"/>
					</HBOX>
					<!-- SCROLL -->
						<VBOX>
							<xsl:for-each select="item">
								<HBOX>
									<LABEL text="{@title}"/>
									<GLUE/>
									<COLOR/>
								</HBOX>
							</xsl:for-each>
						</VBOX>
					<!--/SCROLL -->
				</VBOX>
				<HSPACE width="180"/>
				<TEXT_SAMPLE/>
			</HBOX>

			<HBOX>
				<VBOX>
					<HBOX>
						<CHECKBOX/> 
						Bold
						<GLUE/>
					</HBOX>
					<HBOX>
						<CHECKBOX/> 
						Italic
						<GLUE/>
					</HBOX>
				</VBOX>
				<HSPACE width="25"/>
				<VBOX>
					<HBOX>
						<CHECKBOX/> 
						Underline
						<GLUE/>
					</HBOX>
					<HBOX>
						<CHECKBOX/> 
						Strinke Through
						<GLUE/>
					</HBOX>
				</VBOX>
				<GLUE/>
				<BUTTON text="Restore Default Settings"/>
			</HBOX>
		</VBOX>
	</TITLE>
</xsl:template>

<xsl:template match="display">
	<TITLE text="Display">
		<VBOX>
			<HBOX>
				<CHECKBOX/>
				Wrap Lines
				<GLUE/>
			</HBOX>
			<HBOX>
				Lines to display:
				<SPINNER/>
				<HSPACE width="70"/>
				Columns to display:
				<SPINNER/>
				<GLUE/>
			</HBOX>
			<HBOX>
				Tab size:
				<SPINNER/>
				<GLUE/>
			</HBOX>
			<HBOX>
				Number of lines in console scroll buffer:
				<SPINNER/>
				<GLUE/>
			</HBOX>

		</VBOX>
	</TITLE>
</xsl:template>

<xsl:template match="accessibility">
	<TITLE text="Display">
		<VBOX>
			<HBOX>
				To enable keyboard navigation with arrow keys, 
				assign shotcuts to the Cursor Up and
				<GLUE/>
			</HBOX>
			<HBOX>&lt;HTML&gt;Cursor Down actions in the 
				&lt;U&gt;Shortcuts Panel&lt;/U&gt; (for Console)
				&lt;/HTML&gt;<GLUE/>
			</HBOX>
			<VSPACE height="25"/>
			<HBOX>&lt;HTML&gt;To enable or disable the completion, 
				assign a shortcut action in the &lt;U&gt;Shortcuts 
				panel&lt;/U&gt; (for &lt;/HTML&gt;<GLUE/>
			</HBOX>
			<HBOX>
				Console). Default shortcut for completion is Tab.
				<GLUE/>
			</HBOX>
		</VBOX>
	</TITLE>
</xsl:template>

<!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
  ::
  ::     M A I N   P A N E L :   C O N S O L E
  ::
  -->

<xsl:template match="saving">
	<TITLE text="Saving">
		<VBOX>
			<HBOX>
				<CHECKBOX listener="ActionListener">
					<xsl:if test="@kind='save'">
						<xsl:attribute name="checked">checked</xsl:attribute>
					</xsl:if>
					<actionPerformed set="kind" value="save">
						<xsl:call-template name="context"/>
					</actionPerformed>
				</CHECKBOX>
				Save after
				<SPINNER/>
				command(s)			
				<GLUE/>
			</HBOX>
			<HBOX>
				<CHECKBOX listener="ActionListener">
					<xsl:if test="@kind='quit'">
						<xsl:attribute name="checked">checked</xsl:attribute>
					</xsl:if>
					<actionPerformed set="kind" value="quit">
						<xsl:call-template name="context"/>
					</actionPerformed>
				</CHECKBOX>
				Save history file on quit
				<GLUE/>
			</HBOX>
			<HBOX>
				<CHECKBOX listener="ActionListener">
					<xsl:if test="@kind='disable'">
						<xsl:attribute name="checked">checked</xsl:attribute>
					</xsl:if>
					<actionPerformed set="kind" value="disable">
						<xsl:call-template name="context"/>
					</actionPerformed>
				</CHECKBOX>
				Disable history management
				<GLUE/>
			</HBOX>
			<VSPACE height="20"/>
			<HBOX>
				<CHECKBOX listener="ActionListener" checked="{@consecutive}">
					<actionPerformed choose="consecutive">
						<xsl:call-template name="context"/>
					</actionPerformed>
				</CHECKBOX>
				Save consecutive duplicate commands
				<GLUE/>
			</HBOX>
		</VBOX>
	</TITLE>
</xsl:template>

<xsl:template match="settings">
	<TITLE text="Settings">
		<VBOX>
			<HBOX>
				History file:
				<FILE/>
			</HBOX>
			<HBOX>
				History lines:
				<SPINNER/>
			</HBOX>
		</VBOX>
	</TITLE>
	<VSPACE height="80"/>
</xsl:template>

</xsl:stylesheet>

