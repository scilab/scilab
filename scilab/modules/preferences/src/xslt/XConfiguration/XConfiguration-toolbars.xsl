<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version ="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>

	<xsl:import href="../XCommon-toolbars.xsl"/>

	<xsl:template match="toolbars">
		<xsl:variable name="select" select="@select"/>
		<Title text="Variable size">
			<VBox>
				<HBox>
					<HBox width="50" height="30">Toolbar:
								<Glue/>
  				</HBox>
					<Select listener="ActionListener">
						<actionPerformed choose="select">
							<xsl:call-template name="context"/>
						</actionPerformed>
						<xsl:for-each select="$toolbars/toolbar">
							<option value="{@title}">
								<xsl:if test="@title=$select">
									<xsl:attribute name="selected">selected</xsl:attribute>
								</xsl:if>
							</option>						
						</xsl:for-each>
					</Select>
				</HBox>
				<xsl:for-each select="toolbar[@title=$select]">
					<xsl:variable name="used"      select="item/@title"/>
					<xsl:variable name="reference" select="$toolbars/toolbar[@title=$select]"/>
					<HBox>
						<HBox width="50" height="30">Layout:
							<Glue/>
						</HBox>
						<xsl:for-each select="*">
							<xsl:choose>
								<xsl:when test="position()=../@position">
									<VBox background="#0000ff">
										<xsl:apply-templates select="." mode="button">
											<xsl:with-param name="reference" select="$reference"/>
										</xsl:apply-templates>
									</VBox>
								</xsl:when>
								<xsl:otherwise>
									<xsl:apply-templates select="." mode="button">
										<xsl:with-param name="reference" select="$reference"/>
									</xsl:apply-templates>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:for-each>
						<Label text="[end]"/>
						<Glue/>
					</HBox>
					<HBox>
						<HBox width="50" height="30">Controls:
							<Glue/>
						</HBox>
						<Button text="^" listener="ActionListener">
							<actionPerformed insert="{@position}">
								<xsl:call-template name="context"/>
								<xsl:choose>
									<xsl:when test="@item='0'">
										<separator/>
									</xsl:when>
									<xsl:otherwise>
										<item title="{$reference/item[ position()=@item]/@title}"/>
									</xsl:otherwise>
								</xsl:choose>
							</actionPerformed>
						</Button>
						<Button text="&lt;"/>
						<Button text="&gt;"/>
						<Button text="v" listener="ActionListener">
							<actionPerformed delete="{@position}">
								<xsl:call-template name="context"/>
							</actionPerformed>
						</Button>
						<Glue/>
					</HBox>
					<HBox>
						<VBox width="50" height="250">
							<HBox>
								Actions:
								<Glue/>
							</HBox>
							<VSpace height="200"/>
						</VBox>
						<Scroll>
							<VBox>
								<xsl:variable name="item" select="@item"/>
								<HBox>
									<Label text="|"/>
									<HSpace width="5"/>
									<Label text="Separator">
										<xsl:choose>
											<xsl:when test="$item='0'">
												<xsl:attribute name="background">#0000ff</xsl:attribute>
											</xsl:when>
											<xsl:otherwise>
												<xsl:attribute name="listener">ActionListener</xsl:attribute>
												<actionPerformed set="position" value="0">
													<xsl:call-template name="context"/>
												</actionPerformed> 
											</xsl:otherwise>
										</xsl:choose>
									</Label>
									<Glue/>
								</HBox>
								<xsl:for-each select="$reference">
									<xsl:if test="item[ not(@title = $used)]">
										<HBox>
											<Icon  src= "{$reference/item[@title = current()/@title]/@icon}"/>
											<HSpace width="5"/>
											<Label text="{@title}">
												<xsl:choose>
													<xsl:when test="position()=$item">
														<xsl:attribute name="background">#0000ff</xsl:attribute>
													</xsl:when>
													<xsl:otherwise>
														<xsl:attribute name="listener">ActionListener</xsl:attribute>
														<actionPerformed set="item" value="{position()}">
															<xsl:call-template name="context"/>
														</actionPerformed> 
													</xsl:otherwise>
												</xsl:choose>
											</Label>
											<Glue/>
										</HBox>
									</xsl:if>
								</xsl:for-each>
							</VBox>
						</Scroll>
						<Glue/>
					</HBox>
				</xsl:for-each>					
			</VBox>
		</Title>
	</xsl:template>

	<xsl:template match="item" mode="button">
		<xsl:param name="reference"/>	
		<Icon src="{$reference/item[@title = current()/@title]/@icon}">
			<!-- ... -->
		</Icon>
	</xsl:template>

	<xsl:template match="separator" mode="button">
		<Label text="|">
			<!-- ... -->
		</Label>
	</xsl:template>

</xsl:stylesheet>

