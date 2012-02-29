<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version ="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>	<!--
		||
		||   L E F T   P A N E L   M A N A G E M E N T
		||
		-->

	<xsl:template match="*" mode="left-panel">
		<xsl:apply-templates mode="inner-left-panel"/>
		<XCosPreview height="200" width="200">
           <tableBlock>
             <root>
               <xsl:apply-templates select="." mode="preview"/>
             </root>
           </tableBlock>
        </XCosPreview>
		<!-- TODO -->
	</xsl:template>

	<xsl:template match="*" mode="inner-left-panel">
		<HBox>
			<Icon src="media-playback-start.png"/>
			<xsl:choose>
				<xsl:when test="local-name()=../@step">
					<Label text="{local-name()}" background="#0000ff" foreground="#ffffff"/>
				</xsl:when>
				<xsl:otherwise>
					<Label text="{local-name()}" listener="MouseListener">
						<mouseClicked set="step" value="{local-name()}" context="/"/>
					</Label>
				</xsl:otherwise>
			</xsl:choose>
			<Glue/>
			<HSpace width="25"/>
		</HBox>
	</xsl:template>

	<!--
		||
		||   C O M M O N   M A I N   P A N E L S
		||
		-->

	<xsl:template match="parameters" mode ="main-panel">
		<xsl:if test="ancestor::*/@step='parameters'">
			<xsl:for-each select="item[position()=current()/@item]">
				<Title text="{concat('User parameter [', @name, ']:')}">
					<VBox>
						<HBox>
							Variable name:
							<Entry text="{@name}" listener="ActionListener">
								<actionPerformed choose="name">
									<xsl:call-template name="context"/>
								</actionPerformed>
							</Entry>
							Data-type:
							<xsl:call-template name="Select">
								<xsl:with-param name="among">
									<option type="double"/>
									<option type="integer"/>
								</xsl:with-param>
							</xsl:call-template>
						</HBox>
						<HBox>
							Minimum:
							<Entry text="{@min}" listener="ActionListener">
								<actionPerformed choose="min">
									<xsl:call-template name="context"/>
								</actionPerformed>
							</Entry>
							Test:
							<Entry text="{@test}" listener="ActionListener">
								<actionPerformed choose="test">
									<xsl:call-template name="context"/>
								</actionPerformed>
							</Entry>
							Maximum:
							<Entry text="{@max}" listener="ActionListener">
								<actionPerformed choose="max">
									<xsl:call-template name="context"/>
								</actionPerformed>
							</Entry>
						</HBox>
						<HBox>
							Description:
							<Entry text="{@description}" listener="ActionListener">
								<actionPerformed choose="description">
									<xsl:call-template name="context"/>
								</actionPerformed>
							</Entry>
						</HBox>
						<HBox>
							Tooltip:
							<Entry text="{@tooltip}" listener="ActionListener">
								<actionPerformed choose="tooltip">
									<xsl:call-template name="context"/>
								</actionPerformed>
							</Entry>
						</HBox>
					</VBox>
				</Title>
			</xsl:for-each>
			<Title text="Select parameter setup">
				<Table mode="select" item="{@item}" size="dynamic" listener="TableListener">
					<tableSelect choose="item">
						<xsl:call-template name="context"/>
					</tableSelect>
					<tableAdd>
						<xsl:call-template name="context"/>
						<item name="parameter"    type="double"  min="0.0" max="999.9"
          		test="0.0" tooltip="tooltip" description="A parameter"/>
					</tableAdd>
					<tableCol title="Name"        attr="name"/>
					<tableCol title="Type"        attr="type"/>
					<tableCol title="Minimum"     attr="min"/>
					<tableCol title="Test value"  attr="test"/>
					<tableCol title="Maximum"     attr="max"/>
					<xsl:for-each select="item">
						<tableRow name="{@name}" type="{@type}"  min="{@min}" max="{@max}" test="{@test}"/>
					</xsl:for-each>
				</Table>
			</Title>

		</xsl:if>
	</xsl:template>

	<xsl:template match="interface" mode ="main-panel">
		<xsl:if test="ancestor::*/@step='interface'">
			<Title text="Inputs/Outputs">
				<VBox>
					<HBox>&lt;HTML&gt;&lt;B&gt;Regular Inputs<HSpace width="250"/>&lt;HTML&gt;&lt;B&gt;Activation Inputs
					</HBox>
					<HBox>
						<xsl:for-each select="regular-inputs">
							<Table size="dynamic" mode="cell" listener="TableListener">
								<tableChanged>
									<xsl:call-template name="context"/>
								</tableChanged>
								<tableAdd>
									<xsl:call-template name="context"/>
									<item  name="parameter" type="double" rows="" columns=""/>
								</tableAdd>
                                <tableDel remove="selection">
									<xsl:call-template name="context"/>
								</tableDel>
								<tableCol title="Name"      attr="name"    editable="true"/>
								<tableCol title="Data type" attr="type"    editable="true"/>
								<tableCol title="Rows"      attr="rows"    editable="true"/>
								<tableCol title="Columns"   attr="columns" editable="true"/>
								<xsl:for-each select="item">
									<tableRow name="{@name}" type="{@type}"  rows="{@rows}"  columns="{@columns}"/>
								</xsl:for-each>
							</Table>
						</xsl:for-each>
						<HSpace width="30"/>
						<xsl:for-each select="activation-inputs">
							<Table size="dynamic" mode="cell" listener="TableListener">
								<tableChanged>
									<xsl:call-template name="context"/>
								</tableChanged>
								<tableAdd>
									<xsl:call-template name="context"/>
									<item  name="input"/>
								</tableAdd>
								<tableCol title="Name"      attr="name"   editable="true"/>
								<xsl:for-each select="item">
									<tableRow name="{@name}"/>
								</xsl:for-each>
							</Table>
						</xsl:for-each>
					</HBox>
					<HBox>&lt;HTML&gt;&lt;B&gt;Regular Outputs<HSpace width="250"/>&lt;HTML&gt;&lt;B&gt;Activation Outputs<HSpace width="1"/>
					</HBox>
					<HBox>
						<xsl:for-each select="regular-outputs">
							<Table size="dynamic" mode="cell" listener="TableListener">
								<tableChanged>
									<xsl:call-template name="context"/>
								</tableChanged>
								<tableAdd>
									<xsl:call-template name="context"/>
									<item  name="parameter" type="double" rows="" columns=""/>
								</tableAdd>
								<tableCol title="Name"      attr="name"     editable="true"/>
								<tableCol title="Data type" attr="type"     editable="true"/>
								<tableCol title="Rows"      attr="rows"     editable="true"/>
								<tableCol title="Columns"   attr="columns"  editable="true"/>
								<xsl:for-each select="item">
									<tableRow name="{@name}" type="{@type}"  rows="{@rows}"  columns="{@columns}"/>
								</xsl:for-each>
							</Table>
						</xsl:for-each>
						<HSpace width="30"/>
						<xsl:for-each select="activation-outputs">
							<Table size="dynamic" mode="cell" listener="TableListener">
								<tableChanged>
									<xsl:call-template name="context"/>
								</tableChanged>
								<tableAdd>
									<xsl:call-template name="context"/>
									<item name="String"/>
								</tableAdd>
								<tableCol title="Name"      attr="name"   editable="true"/>
								<xsl:for-each select="item">
									<tableRow name="{@name}"/>
								</xsl:for-each>
							</Table>
						</xsl:for-each>
					</HBox>
				</VBox>
			</Title>
		</xsl:if>
	</xsl:template>

	<xsl:template match="graphics" mode ="main-panel">
		<xsl:if test="ancestor::*/@step='graphics'">
			<xsl:for-each select="item[position()=current()/@item]">
				<Title text="{concat('Current setup: ', @title)}">
					<VBox>
						<HBox>
							Color:
							<Glue/>
							<Color color="{@color}" listener="ActionListener">
								<actionPerformed choose="color">
									<xsl:call-template name="context"/>
								</actionPerformed>
							</Color>
								<Glue/>
						</HBox>
						<xsl:if test="@family">
							<HBox>
								Font family:
								<Glue/>
								<xsl:call-template name="Select">
									<xsl:with-param name="among">
										<option family="Arial"/>
										<option family="Courier New"/>
										<option family="System"/>
										<option family="Times New Roman"/>
									</xsl:with-param>
								</xsl:call-template>
								<Glue/>
							</HBox>
						</xsl:if>
						<xsl:if test="@size">
							<HBox>
								Font size:
								<HSpace width="15"/>
								<Glue/>
								<xsl:call-template name="Select">
									<xsl:with-param name="among">
										<option size="6"/>
										<option size="8"/>
										<option size="9"/>
										<option size="10"/>
										<option size="11"/>
										<option size="12"/>
										<option size="14"/>
										<option size="16"/>
									</xsl:with-param>
								</xsl:call-template>
								<Glue/>
							</HBox>
						</xsl:if>
						<xsl:if test="@face">
							<HBox>
								Font face:
								<HSpace width="10"/>
								<Glue/>
								<xsl:call-template name="Select">
									<xsl:with-param name="among">
										<option face="plain"/>
										<option face="bold"/>
										<option face="italic"/>
										<option face="bold italic"/>
									</xsl:with-param>
								</xsl:call-template>
								<Glue/>
							</HBox>
						</xsl:if>
						<xsl:if test="not(@family)">
							<VSpace height="85"/>
						</xsl:if>
					</VBox>
				</Title>
			</xsl:for-each>
			<VSpace height="50"/>
			<Title text="Select graphic setup">
				<Table mode="select" item="{@item}" listener="TableListener">
					<tableSelect choose="item">
						<xsl:call-template name="context"/>
					</tableSelect>
					<tableCol title="Kind"        attr="title"/>
					<tableCol title="Foreground"  attr="color"/>
					<tableCol title="Font family" attr="family"/>
					<tableCol title="Font size"   attr="size"/>
					<tableCol title="Font face"   attr="face"/>
					<xsl:for-each select="item">
						<tableRow title="{@title}" family="{@family}"  size="{@size}"  face="{@face}"  color="{@color}"/>
					</xsl:for-each>
				</Table>
			</Title>
			<VSpace height="20"/>
		</xsl:if>
	</xsl:template>

	<xsl:template match="documentation" mode ="main-panel">
		<xsl:if test="ancestor::*/@step='documentation'">
			<Title text="Documentation">
				<VBox>
					<xsl:for-each select="short-description">
						<HBox>&lt;HTML&gt;&lt;B&gt;Title
							<Glue/>
						</HBox>
						<HBox>
							<Entry text="{@text}" listener="ActionListener">
								<actionPerformed choose="text">
									<xsl:call-template name="context"/>
								</actionPerformed>
							</Entry>
						</HBox>
					</xsl:for-each>
					<xsl:for-each select="long-description">
						<HBox>&lt;HTML&gt;&lt;B&gt;Description
							<Glue/>
						</HBox>
						<Textarea listener="DocumentListener" width="350" height="300">
							<!--documentUpdate> TODO
								<xsl:call-template name="context"/>
							</documentUpdate>
							<xsl:copy-of select="node()"/ -->
						</Textarea >
					</xsl:for-each>
				</VBox>
			</Title>
		</xsl:if>
	</xsl:template>

</xsl:stylesheet>

