<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"

  xmlns:d="http://docbook.org/ns/docbook"
xmlns:sfa="http://developer.apple.com/namespaces/sfa"
  xmlns:sf="http://developer.apple.com/namespaces/sf"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xmlns:appsl="http://developer.apple.com/namespaces/sl"

  xmlns:dbk='http://docbook.org/ns/docbook'
  xmlns:rnd='http://docbook.org/ns/docbook/roundtrip'
  exclude-result-prefixes='sfa sf xsi appsl'>

  <xsl:output method="xml" indent='yes'/>

  <!-- ********************************************************************
       $Id: pages2normalise.xsl 7266 2007-08-22 11:58:42Z xmldoc $
       ********************************************************************

       This file is part of the XSL DocBook Stylesheet distribution.
       See ../README or http://nwalsh.com/docbook/xsl/ for copyright
       and other information.

       ******************************************************************** -->

  <xsl:strip-space elements='*'/>
  <xsl:preserve-space elements='sf:span'/>

  <xsl:key name='styles'
	   match='sf:paragraphstyle[not(ancestor::appsl:section-prototypes)] |
		  sf:characterstyle[not(ancestor::appsl:section-prototypes)] |
                  sf:table-style'
	   use='@sf:ident|@sfa:ID'/>

  <xsl:key name='ids'
    match='*'
    use='@sfa:ID'/>

  <xsl:template match='appsl:document'>
    <dbk:article>
      <!-- TODO: headers and footers -->
      <xsl:apply-templates select='sf:text-storage'/>
    </dbk:article>
  </xsl:template>

  <xsl:template match='sf:p'>
    <xsl:choose>
      <xsl:when test='sf:attachment-ref and
                      count(*) = count(sf:attachment-ref|sf:br)'>
        <xsl:apply-templates/>
      </xsl:when>
      <xsl:otherwise>
        <dbk:para>
          <xsl:variable name='style-name'>
            <xsl:call-template name='rnd:find-style'/>
          </xsl:variable>
          <xsl:if test='$style-name != "" and
                        $style-name != "d:para"'>
            <xsl:attribute name='rnd:style'>
              <xsl:value-of select='$style-name'/>
            </xsl:attribute>
          </xsl:if>

          <xsl:apply-templates/>
        </dbk:para>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match='sf:span'>
    <xsl:variable name='style-name'
		  select='key("d:styles", @sf:style)/self::sf:characterstyle/@sf:name'/>

    <xsl:variable name='char-style'>
      <xsl:call-template name='rnd:find-style'>
        <xsl:with-param name='char-style-name' select='$style-name'/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test='$style-name = "attribute-name"'>
	<xsl:if test='not(preceding-sibling::node()[not(self::text()) or (self::text() and normalize-space() != "")])'>
          <xsl:attribute name='{.}'>
            <xsl:apply-templates select='following-sibling::*[1][self::sf:span]'
              mode='attribute'/>
          </xsl:attribute>
	</xsl:if>
      </xsl:when>
      <xsl:when test='$style-name = "attribute-value"'/>
      <xsl:when test='$style-name = ""'>
        <xsl:apply-templates/>
      </xsl:when>
      <xsl:when test='$char-style = "d:superscript" or
                      $char-style = "d:subscript"'>
        <xsl:element name='{$char-style}'
          namespace='http://docbook.org/ns/docbook'>
          <xsl:apply-templates/>
        </xsl:element>
      </xsl:when>
      <xsl:otherwise>
	<dbk:emphasis>
          <xsl:choose>
            <xsl:when test='$char-style = "emphasis-bold" or
                            $char-style = "emphasis-strong"'>
              <xsl:attribute name='role'>bold</xsl:attribute>
            </xsl:when>
            <xsl:when test='$char-style != "" and
                            $char-style != "d:emphasis"'>
              <xsl:attribute name='rnd:style'>
                <xsl:value-of select='$char-style'/>
              </xsl:attribute>
            </xsl:when>
          </xsl:choose>

	  <xsl:apply-templates/>
        </dbk:emphasis>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match='*' mode='attribute'>
    <xsl:variable name='style-name'
		  select='key("d:styles", @sf:style)/self::sf:characterstyle/@sf:name'/>

    <xsl:if test='$style-name = "attribute-value"'>
      <xsl:apply-templates/>
    </xsl:if>
  </xsl:template>

  <xsl:template match='sf:br'/>
  <xsl:template match='sf:lnbr|sf:crbr'>
    <xsl:text>&#xa;</xsl:text>
  </xsl:template>
  <xsl:template match='sf:tab'>
    <xsl:text>        </xsl:text>
  </xsl:template>
  <xsl:template match='sf:link'>
    <dbk:ulink url='{@href}'>
      <xsl:apply-templates/>
    </dbk:ulink>
  </xsl:template>

  <xsl:template match='sf:attachment-ref'>
    <xsl:if test='@sf:kind = "table-attachment"'>
      <xsl:apply-templates select='key("d:ids", @sfa:IDREF)'/>
    </xsl:if>
  </xsl:template>

  <xsl:template match='sf:attachment[@sf:kind = "table-attachment"]'>
    <!-- Assumes: (1) table has a header row, (2) cell style is not redefined, (3) no column spans -->
    <xsl:variable name='hdr-cells'
      select='.//sf:table-cell[key("d:ids", sf:tableCellStyle-ref/@sfa:IDREF)/@sf:parent-ident = "table-header-row-cell-style-default"]'/>

    <xsl:variable name='num-cols' select='count($hdr-cells)'/>

    <!-- This is unreliable... must account for row/column spans -->
    <xsl:variable name='num-rows'
      select='floor(count(sf:tableAttachmentTable/sf:tableModelCells/sf:tableCellArrayCellsByRow/sf:table-cell) div $num-cols)'/>

    <xsl:choose>
      <xsl:when test='not($num-rows) or $num-rows = ""'>
        <xsl:message> cannot determine num-rows </xsl:message>
        <xsl:comment> cannot determine num-rows </xsl:comment>
      </xsl:when>

      <xsl:otherwise>
        <!-- TODO: borders, column widths -->
        <dbk:informaltable>
          <dbk:tgroup cols='{$num-cols}'>
            <xsl:call-template name='rnd:make-colspecs'>
              <xsl:with-param name='num-cols' select='$num-cols'/>
            </xsl:call-template>
            <dbk:tbody>
              <xsl:apply-templates/>
            </dbk:tbody>
          </dbk:tgroup>
        </dbk:informaltable>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  <xsl:template name='rnd:make-colspecs'>
    <xsl:param name='num-cols' select='0'/>

    <xsl:choose>
      <xsl:when test='$num-cols &lt;= 0 or
                      $num-cols = ""'/>
      <xsl:otherwise>
        <dbk:colspec/>
        <xsl:call-template name='rnd:make-colspecs'>
          <xsl:with-param name='num-cols' select='$num-cols - 1'/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  <xsl:template match='sf:tableCellArrayCellsByRow'>
    <xsl:variable name='hdr-cells'
      select='sf:table-cell[key("d:ids", sf:tableCellStyle-ref/@sfa:IDREF)/@sf:parent-ident = "table-header-row-cell-style-default"]'/>

    <xsl:variable name='num-cols' select='count($hdr-cells)'/>

    <xsl:variable name='num-rows'
      select='floor(count(sf:table-cell) div $num-cols)'/>

    <xsl:call-template name='rnd:table-rows'>
      <xsl:with-param name='num-cols' select='$num-cols'/>
      <xsl:with-param name='cells' select='sf:table-cell'/>
    </xsl:call-template>
  </xsl:template>
  <xsl:template name='rnd:table-rows'>
    <xsl:param name='num-cols' select='0'/>
    <xsl:param name='cells' select='/..'/>

    <xsl:choose>
      <xsl:when test='count($cells) = 0'/>
      <xsl:otherwise>
        <dbk:row>
          <xsl:apply-templates select='$cells[position() &lt;= $num-cols]'/>
        </dbk:row>
        <xsl:call-template name='rnd:table-rows'>
          <xsl:with-param name='num-cols'
            select='$num-cols'/>
          <xsl:with-param name='cells'
            select='$cells[position() > $num-cols]'/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  <xsl:template match='sf:table-cell'>
    <dbk:entry>
      <xsl:apply-templates select='sf:tableCellContent/sf:text-storage/sf:text-body/sf:layout'/>
    </dbk:entry>
  </xsl:template>
  <xsl:template match='sf:tableAttachmentTable |
                       sf:tableModelCells'>
    <xsl:apply-templates/>
  </xsl:template>
  <xsl:template match='sf:tableModelPartitionSource |
                       sf:tableModelStyle-ref |
                       sf:tableModelVectors |
                       sf:tableCellArrayCellsByColumn |
                       sf:tableModelTableID'/>

  <xsl:template match='sf:text-storage |
                       sf:text-body |
                       sf:section |
                       sf:layout'>
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match='sf:stylesheet|sf:stylesheet-ref |
		       sf:container-hint |
		       sf:page-start|sf:br |
		       sf:selection-start|sf:selection-end |
		       sf:insertion-point |
		       sf:ghost-text |
                       sf:attachments'/>

  <xsl:template match='*'>
    <xsl:message>element "<xsl:value-of select='name()'/>" not handled</xsl:message>
  </xsl:template>

  <xsl:template name='rnd:find-style'>
    <xsl:param name='ident' select='@sf:style'/>
    <xsl:param name='para-style-name'
	       select='key("d:styles", $ident)/self::sf:paragraphstyle/@sf:name'/>
    <xsl:param name='char-style-name'
	       select='key("d:styles", $ident)/self::sf:characterstyle/@sf:name'/>

    <xsl:choose>
      <xsl:when test='$ident = "paragraph-style-default"'/>
      <xsl:when test='$para-style-name != ""'>
	<xsl:value-of select='$para-style-name'/>
      </xsl:when>
      <xsl:when test='key("d:styles", $ident)/self::sf:characterstyle/sf:property-map/sf:superscript/sf:number/@sfa:number = "1"'>superscript</xsl:when>
      <xsl:when test='key("d:styles", $ident)/self::sf:characterstyle/sf:property-map/sf:subscript/sf:number/@sfa:number = "1"'>subscript</xsl:when>
      <xsl:when test='$char-style-name != "" or
		      key("d:styles", $ident)/self::sf:characterstyle/sf:property-map/*'>
	<xsl:value-of select='$char-style-name'/>
      </xsl:when>
    </xsl:choose>
  </xsl:template>
</xsl:stylesheet>
