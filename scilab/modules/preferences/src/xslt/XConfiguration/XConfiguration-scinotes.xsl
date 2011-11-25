<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version ="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>

	<xsl:template match="scinotes-preferences">
		<Title text="Open files in Scinotes">
			<xsl:call-template name="BooleanCheckBox">
				<xsl:with-param name="attr-name" select="'restart-reopen'"/>
				<xsl:with-param name="text" select="'On restart reopen files from previous scilab session'"/>
			</xsl:call-template>
		</Title>
		<Title text="Automatic File changes">
			<xsl:call-template name="BooleanCheckBox">
				<xsl:with-param name="attr-name" select="'reload-unedited'"/>
				<xsl:with-param name="text" select="'Reload unedited files that have been externally modified'"/>
			</xsl:call-template>
		
			<xsl:call-template name="BooleanCheckBox">
				<xsl:with-param name="attr-name" select="'add-line-termination'"/>
				<xsl:with-param name="text" select="'Add line termination at end of file.'"/>
			</xsl:call-template>
		</Title>
		<Title text="Recently used file list">
			<Grid>
				<Label text="Number of entries:"/>
				<Panel gridx="2">
					<xsl:call-template name="Select">
						<xsl:with-param name="among">
							<option number-of-entries="1"/>
							<option number-of-entries="2"/>
							<option number-of-entries="3"/>
							<option number-of-entries="4"/>
							<option number-of-entries="5"/>
							<option number-of-entries="6"/>
							<option number-of-entries="7"/>
							<option number-of-entries="8"/>
							<option number-of-entries="9"/>
							<option number-of-entries="10"/>
							<option number-of-entries="20"/>
							<option number-of-entries="40"/>
							<option number-of-entries="80"/>
						</xsl:with-param>
					</xsl:call-template>
				</Panel>
			</Grid>
		</Title>
		<Title text="File encoding">
			<Grid>
				<Label text="File encoding:"/>
				<Panel gridx="2">
					<xsl:call-template name="Select">
						<xsl:with-param name="among">
							<option encoding="utf-8"/>
							<option encoding="iso-8859-1"/>
						</xsl:with-param>
					</xsl:call-template>
				</Panel>
			</Grid>
		</Title>
		<Title text="End of file">
			<Grid>
				<Label text="End of line:"/>
				<Panel gridx="2">
					<xsl:call-template name="Select">
						<xsl:with-param name="among">
							<option eol="Unix(LF)"/>
							<option eol="Windows/Dos(CR+LF)"/>
							<option eol="Mac Classic(CR)"/>
						</xsl:with-param>
					</xsl:call-template>
				</Panel>
			</Grid>
		</Title>
	</xsl:template>

	<xsl:template match="scinotes-display">
		<Title text="General display options">
			<VBox>
				<VSpace height="20"/>
				<HBox>
					<xsl:call-template name="BooleanCheckBox">
						<xsl:with-param name="attr-name" select="'highlight-current-line'"/>
						<xsl:with-param name="text">
							<xsl:choose>
								<xsl:when test="@highlight-current-line='true'">Highlight current line with</xsl:when>
								<xsl:otherwise>Highlight current line</xsl:otherwise>
							</xsl:choose>
						</xsl:with-param>
					</xsl:call-template>
					<xsl:if test="@highlight-current-line='true'">
						<Color color="{@current-line-color}" listener="ActionListener">
							<actionPerformed choose="current-line-color">
								<xsl:call-template name="context"/>
							</actionPerformed>
						</Color>
					</xsl:if>
					<Glue/>
				</HBox>
				<VSpace height="20"/>
				<HBox>
					<xsl:call-template name="BooleanCheckBox">
						<xsl:with-param name="attr-name" select="'show-line-numbers'"/>
						<xsl:with-param name="text" select="'Show line numbers'"/>
					</xsl:call-template>
					<Glue/>
				</HBox>
				<VSpace height="20"/>
				<HBox>
					<xsl:call-template name="BooleanCheckBox">
						<xsl:with-param name="attr-name" select="'wrap-lines'"/>
						<xsl:with-param name="text" select="'Wrap line'"/>
					</xsl:call-template>
					<Glue/>
				</HBox>
				<VSpace height="20"/>
				<Grid insets="large" anchor="west">
					<Label text="View"/>
					<Link gridx="2" listener="MouseListener" text="custom font">
                        <mouseClicked set="path" value="2/" context="/"/>
					</Link>
					<Label gridx="3" text="preferences to change Scinotes fonts."/>
				</Grid>
					
			</VBox>
		</Title>
		<VSpace height="200"/>
	</xsl:template>

	<xsl:template match="scinotes-syntax">
		<Title text="Syntax">
			<VBox>
				<VSpace height="20"/>
				<HBox>
					<xsl:call-template name="BooleanCheckBox">
						<xsl:with-param name="attr-name" select="'highlight'"/>
						<xsl:with-param name="text" select="'Enable syntax highlighting'"/>
					</xsl:call-template>
					<xsl:if test="@highlight='true'">
						<HSpace width="30"/>
						<Link listener="MouseListener" text="Set syntax Colors">
                            <mouseClicked set="path" value="3/" context="/"/>
						</Link>
					</xsl:if>
					<Glue/>
				</HBox>
				<VSpace height="20"/>
				<HBox>
					<xsl:call-template name="BooleanCheckBox">
						<xsl:with-param name="attr-name" select="'hightlight-brackets'"/>
						<xsl:with-param name="text" select="'Highlight corresponding brackets'"/>
					</xsl:call-template>
					<Glue/>
				</HBox>
			</VBox>
		</Title>
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
	</xsl:template>

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
						Automatically delete autosave files
						<Glue/>
					</HBox>
				</Title>
				<Title text="File name">
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
						<File gridx="2" gridy="2" enable="{@source-flag='false'}" href="{@single-directory}" mask="*" 
								desc="Choose an autosave directory" 
								listener="ActionListener">
							<actionPerformed choose="single-directory">
								<xsl:call-template name="context"/>
							</actionPerformed>
						</File>
					</Grid>
				</Title>
			</xsl:when>
			<xsl:otherwise>
				<VSpace height="350"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
</xsl:stylesheet>

