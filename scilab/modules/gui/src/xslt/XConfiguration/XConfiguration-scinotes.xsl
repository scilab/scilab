<?xml version='1.0' encoding='iso-8859-1'?>
<xsl:stylesheet version ="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>

	<xsl:template match="scinotes-preferences">
		<Title text="Open files in Scinotes">
			<HBox>
				<xsl:call-template name="BooleanCheckBox">
					<xsl:with-param name="attr-name" select="'restart-reopen'"/>
				</xsl:call-template>
				On restart reopen files from previous scilab session
				<Glue/>
			</HBox>
		</Title>
		<Title text="Automatic File changes">
			<HBox>
				<xsl:call-template name="BooleanCheckBox">
					<xsl:with-param name="attr-name" select="'reload-unedited'"/>
				</xsl:call-template>
				Reload unedited files that have been externally modified
				<Glue/>
			</HBox>
			<HBox>
				<xsl:call-template name="BooleanCheckBox">
					<xsl:with-param name="attr-name" select="'add-line-termination'"/>
				</xsl:call-template>
				Add line termination at end of file.
				<Glue/>
			</HBox>
		</Title>
		<Title text="Recently used file list">
			<VBox>
				<VSpace height="20"/>
				<HBox>
					Number of entries:
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
					<Glue/>
				</HBox>
				<VSpace height="20"/>
			</VBox>
		</Title>
		<Title text="File encoding">
			<VBox>
				<VSpace height="20"/>
				<HBox>
					File encoding:
					<xsl:call-template name="Select">
						<xsl:with-param name="among">
							<option encoding="utf-8"/>
							<option encoding="iso-8859-1"/>
						</xsl:with-param>
					</xsl:call-template>
					<Glue/>
				</HBox>
				<VSpace height="20"/>
			</VBox>
		</Title>
		<Title text="End of file">
			<VBox>
				<VSpace height="20"/>
				<HBox>
					End of line:
					<xsl:call-template name="Select">
						<xsl:with-param name="among">
							<option eol="Unix(LF)"/>
							<option eol="Windows/Dos(CR+LF)"/>
							<option eol="Mac Classic(CR)"/>
						</xsl:with-param>
					</xsl:call-template>
					<Glue/>
				</HBox>
				<VSpace height="20"/>
			</VBox>
		</Title>
	</xsl:template>

	<xsl:template match="scinotes-display">
		<Title text="General display options">
			<VBox>
				<VSpace height="20"/>
				<HBox>
					<xsl:call-template name="BooleanCheckBox">
						<xsl:with-param name="attr-name" select="'highlight-current-line'"/>
					</xsl:call-template>
					Highlight current line 
					<xsl:if test="@highlight-current-line='true'">
						<HSpace width="1"/> with <HSpace width="3"/>
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
					</xsl:call-template>
				 Show line numbers
					<Glue/>
				</HBox>
				<VSpace height="20"/>
				<HBox>
					<xsl:call-template name="BooleanCheckBox">
						<xsl:with-param name="attr-name" select="'wrap-lines'"/>
					</xsl:call-template>
					Wrap line
					<Glue/>
				</HBox>
				<VSpace height="20"/>
				<HBox>
					View
					<HSpace width="3"/> 
					<Label listener="MouseListener" text="custom font" foreground="#0000ff">
						<mouseClicked callback="customScinotesFonts"/>
					</Label>
					<HSpace width="3"/> 
					preferences to change scinotes font.
					<Glue/>
				</HBox>
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
					</xsl:call-template>
					Enable syntax highlighting
					<xsl:if test="@highlight='true'">
						<Glue/>
						<Label listener="MouseListener" text="Set syntax Colors" 
							foreground="#0000ff">
							<mouseClicked callback="customScinotesColors"/>
						</Label>
					</xsl:if>
					<Glue/>
				</HBox>
				<VSpace height="20"/>
				<HBox>
					<xsl:call-template name="BooleanCheckBox">
						<xsl:with-param name="attr-name" select="'hightlight-brackets'"/>
					</xsl:call-template>
				  Highlight corresponding brackets
					<Glue/>
				</HBox>
			</VBox>
		</Title>
		<VSpace height="20"/>
		<Title text="Tabs &amp; Indents">
			<VBox>
				<HBox>
					Tab size:
					<xsl:call-template name="Select">
						<xsl:with-param name="among">
							<option tab-size="2"/>
							<option tab-size="4"/>
							<option tab-size="8"/>
						</xsl:with-param>
					</xsl:call-template>
					<xsl:call-template name="BooleanCheckBox">
						<xsl:with-param name="attr-name" select="'use-spaces'"/>
					</xsl:call-template>Tab key insert spaces
					<Glue/>
				</HBox>
				<VSpace height="20"/>
				<HBox>
						View
						<HSpace width="2"/>
						<Label listener="MouseListener" text="Scinotes tab keys" 
							foreground="#0000ff">
							<mouseClicked callback="customScinotesColors"/>
						</Label>
						<HSpace width="2"/>
						preferences
						<Glue/>
				</HBox>
				<VSpace height="20"/>
				<HBox>
					Indent size:
					<xsl:call-template name="Select">
						<xsl:with-param name="among">
							<option indent-size="2"/>
							<option indent-size="4"/>
							<option indent-size="8"/>
						</xsl:with-param>
					</xsl:call-template>
				</HBox>
				<VSpace height="20"/>
				<HBox>
					<xsl:call-template name="BooleanCheckBox">
						<xsl:with-param name="attr-name" select="'automatic-indent'"/>
					</xsl:call-template>
				  Enable automatic indenting
					<Glue/>
				</HBox>
				<VSpace height="20"/>
			</VBox>
		</Title>
		<VSpace height="40"/>		
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
					<VBox>
						<xsl:if test="@append-flag='false'">
							<VSpace height="15"/>
						</xsl:if>
						<HBox>
							<xsl:call-template name="BooleanCheckBox">
								<xsl:with-param name="attr-name" select="'append-flag'"/>
							</xsl:call-template>
							<Label text="Append filename"/>
							<xsl:if test="@append-flag='true'">
								<Label text=" with "/>
								<Entry text="{@append-with}" listener="ActionListener">
									<actionPerformed choose="append-with">
										<xsl:call-template name="context"/>
									</actionPerformed>							
								</Entry>
							</xsl:if>
							<Glue/>
						</HBox>
						<HBox>
							<xsl:call-template name="InvertedCheckBox">
								<xsl:with-param name="attr-name" select="'append-flag'"/>
							</xsl:call-template>
							<Label text="Replace extension"/>
							<xsl:if test="@append-flag='false'">
								<Label text=" with "/>
								<Entry text="{@replace-with}" listener="ActionListener">
									<actionPerformed choose="replace-with">
										<xsl:call-template name="context"/>
									</actionPerformed>							
								</Entry>
							</xsl:if>
							<Glue/>
						</HBox>
						<xsl:if test="@append-flag='true'">
							<VSpace height="15"/>
						</xsl:if>
					</VBox>
				</Title>
				<Title text="Location">
					<VBox>
						<xsl:if test="@source-flag='false'">
							<VSpace height="10"/>
						</xsl:if>
						<HBox>
							<xsl:call-template name="BooleanCheckBox">
								<xsl:with-param name="attr-name" select="'source-flag'"/>
							</xsl:call-template>
							<Label text="Source file directory"/>
							<Glue/>
						</HBox>
						<HBox>
							<xsl:call-template name="InvertedCheckBox">
								<xsl:with-param name="attr-name" select="'source-flag'"/>
							</xsl:call-template>
							<Label text="Single directory"/>
							<xsl:if test="@source-flag='false'">
								<Label text=": "/>
								<File href="{@single-directory}" mask="*" 
										desc="Choose an autosave directory" 
										listener="ActionListener">
									<actionPerformed choose="single-directory">
										<xsl:call-template name="context"/>
									</actionPerformed>
								</File>
							</xsl:if>
							<Glue/>
						</HBox>
						<xsl:if test="@source-flag='true'">
							<VSpace height="10"/>
						</xsl:if>
					</VBox>
				</Title>
			</xsl:when>
			<xsl:otherwise>
				<VSpace height="350"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
</xsl:stylesheet>

