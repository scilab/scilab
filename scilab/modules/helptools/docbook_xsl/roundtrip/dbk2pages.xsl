<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:d="http://docbook.org/ns/docbook"
xmlns:sfa="http://developer.apple.com/namespaces/sfa"
  xmlns:sf="http://developer.apple.com/namespaces/sf"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xmlns:sl="http://developer.apple.com/namespaces/sl"
  xmlns:xi="http://www.w3.org/2001/XInclude"
  xmlns:w='urn:not-yet-implemented'
  xmlns:wx='urn:not-yet-implemented'
  xmlns:aml='urn:not-yet-implemented'
  xmlns:doc='http://docbook.org/ns/docbook'
  exclude-result-prefixes='doc xi w wx aml'>

  <xsl:import href='dbk2wp.xsl'/>

  <xsl:output method="xml" indent='yes' encoding='ascii'/>

  <!-- ********************************************************************
       $Id: dbk2pages.xsl 6910 2007-06-28 23:23:30Z xmldoc $
       ********************************************************************

       This file is part of the XSL DocBook Stylesheet distribution.
       See ../README or http://docbook.sf.net/release/xsl/current/ for
       copyright and other information.

       ******************************************************************** -->

  <xsl:include href='../VERSION'/>
  <xsl:include href='param.xsl'/>

  <xsl:variable name='templatedoc' select='document($pages.template)'/>

  <!-- Lookup style identifiers from their user-visible name -->
  <xsl:variable name='paragraph-styles'
		select='$templatedoc//sf:paragraphstyle'/>
  <xsl:variable name='character-styles'
		select='$templatedoc//sf:characterstyle'/>

  <xsl:template match="/" name='pages.top'>
    <xsl:param name='doc' select='/'/>

    <xsl:if test='not($pages.template)'>
      <xsl:message terminate='yes'>Please specify the template document with the "pages.template" parameter</xsl:message>
    </xsl:if>
    <xsl:if test='not($templatedoc)'>
      <xsl:message terminate='yes'>Unable to open template document "<xsl:value-of select='$pages.template'/>"</xsl:message>
    </xsl:if>

    <sl:document
      sfa:ID="SLPublicationModel-0"
      sl:version="2004093000"
      sl:generator="slingshot"
      sl:app_build_date="Mar  4 2005, 11:22:49">

      <xsl:apply-templates select='$templatedoc/sl:document/*[not(self::sf:text-storage)]'
        mode='doc:copy'/>

      <xsl:apply-templates select='$doc/*'
        mode='doc:toplevel'/>

      <xsl:apply-templates select='$templatedoc/sl:document/sf:text-storage/following-sibling::*'
        mode='doc:copy'/>
    </sl:document>
  </xsl:template>

  <xsl:template name='doc:make-body'>
    <xsl:param name='content'>
      <xsl:apply-templates mode='doc:body'/>
    </xsl:param>

    <sf:text-storage sf:kind='body' sfa:ID='SFWPStorage-7'>
      <sf:stylesheet-ref sfa:IDREF='SFSStylesheet-1'/>
      <sf:text-body>
        <sf:page-start sf:page-index='0'/>
        <sf:container-hint sf:page-index="0" sf:cindex="0" sf:sindex="0" sf:lindex="0" sf:frame-x="56.692913055419922" sf:frame-y="56.692913055419922" sf:frame-w="481.61416625976562" sf:frame-h="714" sf:anchor-loc="0"/>

        <sf:section sf:name="Chapter 1" sf:style="section-style-0">
          <sf:layout sf:style="layout-style-20">
            <xsl:copy-of select='$content'/>
          </sf:layout>
        </sf:section>
      </sf:text-body>
    </sf:text-storage>
  </xsl:template>

  <xsl:template name='doc:make-subsection'>
    <xsl:param name='content'>
      <xsl:apply-templates mode='doc:body'/>
    </xsl:param>

    <xsl:copy-of select='$content'/>
  </xsl:template>

  <xsl:template name='doc:make-paragraph'>
    <xsl:param name='style' select='"d:unknown"'/>
    <xsl:param name='content'>
      <xsl:apply-templates mode='doc:body'/>
    </xsl:param>
    <xsl:param name='outline.level' select='0'/>
    <xsl:param name='attributes.node' select='.'/>

    <sf:p>
      <xsl:if test='$style != ""'>
	<xsl:attribute name='sf:style'>
          <xsl:call-template name='doc:lookup-paragraph-style'>
            <xsl:with-param name='style' select='$style'/>
          </xsl:call-template>
        </xsl:attribute>
      </xsl:if>

      <xsl:call-template name='attributes'>
        <xsl:with-param name='node' select='$attributes.node'/>
      </xsl:call-template>

      <xsl:copy-of select='$content'/>
      <sf:br/>
    </sf:p>
  </xsl:template>

  <xsl:template name='doc:make-phrase'>
    <xsl:param name='style' select='"d:unknown"'/>
    <xsl:param name='content'>
      <xsl:apply-templates mode='doc:body'/>
    </xsl:param>

    <sf:span>
      <xsl:if test='$style != ""'>
	<xsl:attribute name='sf:style'>
	  <xsl:call-template name='doc:lookup-character-style'>
            <xsl:with-param name='style' select='$style'/>
          </xsl:call-template>
        </xsl:attribute>
      </xsl:if>

      <xsl:copy-of select='$content'/>
    </sf:span>
  </xsl:template>

  <xsl:template name='doc:make-hyperlink'>
    <xsl:param name='target'/>
    <xsl:param name='content'>
      <xsl:apply-templates mode='doc:body'/>
    </xsl:param>

    <sf:link href='{$target}'>
      <sf:span>
        <xsl:attribute name='sf:style'>
          <xsl:call-template name='doc:lookup-character-style'>
            <xsl:with-param name='style'>email</xsl:with-param>
          </xsl:call-template>
        </xsl:attribute>
        <xsl:copy-of select='$content'/>
      </sf:span>
    </sf:link>
  </xsl:template>

  <xsl:template name='doc:make-table'/>
  <xsl:template name='doc:make-table-not-yet-implemented'>
    <xsl:param name='columns'/>
    <xsl:param name='content'>
      <xsl:apply-templates mode='doc:body'/>
    </xsl:param>

    <w:tbl>
      <w:tblPr>
        <w:tblW w:w="0" w:type="auto"/>
        <w:tblInd w:w="108" w:type="dxa"/>
        <w:tblLayout w:type="Fixed"/>
      </w:tblPr>
      <w:tblGrid>
        <xsl:copy-of select='$columns'/>
      </w:tblGrid>
      <xsl:copy-of select='$content'/>
    </w:tbl>
  </xsl:template>

  <xsl:template name='doc:make-column'>
    <xsl:param name='width' select='0'/>

    <w:gridcol w:w='{$width}'/>
  </xsl:template>

  <xsl:template name='doc:make-table-row'>
    <xsl:param name='content'>
      <xsl:apply-templates mode='doc:body'/>
    </xsl:param>
    <xsl:param name='is-header' select='false()'/>

    <w:tr>
      <w:trPr>
        <xsl:if test='$is-header'>
          <w:tblHeader/>
        </xsl:if>
      </w:trPr>
      <xsl:copy-of select='$content'/>
    </w:tr>
  </xsl:template>

  <xsl:template name='doc:make-table-cell'>
    <xsl:param name='width' select='0'/>
    <xsl:param name='hidden' select='false()'/>
    <xsl:param name='rowspan' select='1'/>
    <xsl:param name='colspan' select='1'/>
    <xsl:param name='content'>
      <xsl:apply-templates mode='doc:body'/>
    </xsl:param>

    <w:tc>
      <xsl:if test='$colspan != 1 or
                    $width != 0'>
        <w:tcPr>
          <xsl:if test='$colspan != 1 or
                        $width != 0'>
            <w:tcW w:w='{$width}' w:type='dxa'/>
          </xsl:if>
          <xsl:if test='$hidden'>
            <w:vmerge w:val='{$hidden}'/>
          </xsl:if>
          <xsl:if test='$rowspan != 1'>          
            <w:vmerge w:val='restart'/>
          </xsl:if>
          <xsl:if test='$colspan != 1'>
            <w:gridspan w:val='{$colspan}'/>
          </xsl:if>
        </w:tcPr>
      </xsl:if>

      <xsl:copy-of select='$content'/>
    </w:tc>
  </xsl:template>

  <xsl:template name='doc:make-soft-break'>
    <sf:br/>
  </xsl:template>

  <xsl:template name='attributes'>
    <xsl:param name='node' select='.'/>

    <xsl:for-each select='$node/@*'>
      <sf:span>
	<xsl:attribute name='sf:style'>
	  <xsl:call-template name='doc:lookup-character-style'>
	    <xsl:with-param name='style'>attribute-name</xsl:with-param>
	  </xsl:call-template>
	</xsl:attribute>
	<xsl:value-of select='name()'/>
      </sf:span>
      <sf:span>
	<xsl:attribute name='sf:style'>
	  <xsl:call-template name='doc:lookup-character-style'>
	    <xsl:with-param name='style'>attribute-value</xsl:with-param>
	  </xsl:call-template>
	</xsl:attribute>
	<xsl:value-of select='.'/>
      </sf:span>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name='doc:lookup-paragraph-style'>
    <xsl:param name='style'/>

    <xsl:if test='not($paragraph-styles[@sf:name = $style])'>
      <xsl:message>unable to find paragraph style "<xsl:value-of select='$style'/>"</xsl:message>
    </xsl:if>

    <xsl:value-of select='$paragraph-styles[@sf:name = $style]/@sf:ident'/>
  </xsl:template>
  <xsl:template name='doc:lookup-character-style'>
    <xsl:param name='style'/>

    <xsl:if test='not($character-styles[@sf:name = $style])'>
      <xsl:message>unable to find character style "<xsl:value-of select='$style'/>"</xsl:message>
    </xsl:if>

    <xsl:value-of select='$character-styles[@sf:name = $style]/@sf:ident'/>
  </xsl:template>
</xsl:stylesheet>
