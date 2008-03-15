<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:d="http://docbook.org/ns/docbook"
xmlns:w='http://schemas.microsoft.com/office/word/2003/wordml'
  xmlns:v='urn:schemas-microsoft-com:vml'
  xmlns:w10="urn:schemas-microsoft-com:office:word"
  xmlns:aml="http://schemas.microsoft.com/aml/2001/core"
  xmlns:wx='http://schemas.microsoft.com/office/word/2003/auxHint'
  xmlns:o="urn:schemas-microsoft-com:office:office"
  xmlns:dt="uuid:C2F41010-65B3-11d1-A29F-00AA00C14882"
  xmlns:sl='http://schemas.microsoft.com/schemaLibrary/2003/core'
  xmlns:doc='http://www.oasis-open.org/docbook/xml/4.0'
  exclude-result-prefixes='doc'>

  <xsl:output method="xml" indent='yes' standalone='yes' encoding='UTF-8'/>

  <!-- ********************************************************************
       $Id: docbook.xsl 6910 2007-06-28 23:23:30Z xmldoc $
       ********************************************************************

       This file is part of the XSL DocBook Stylesheet distribution.
       See ../README or http://docbook.sf.net/release/xsl/current/ for
       copyright and other information.

       ******************************************************************** -->

  <xsl:include href='../VERSION'/>
  <xsl:include href='param.xsl'/>

  <xsl:variable name='templatedoc' select='document($wordml.template)'/>

  <xsl:template match="/" name='wordml.top'>
    <xsl:param name='doc' select='/'/>

    <xsl:if test='not($wordml.template)'>
      <xsl:message terminate='yes'>Please specify the template document with the "wordml.template" parameter</xsl:message>
    </xsl:if>
    <xsl:if test='not($templatedoc)'>
      <xsl:message terminate='yes'>Unable to open template document "<xsl:value-of select='$wordml.template'/>"</xsl:message>
    </xsl:if>

    <xsl:processing-instruction name='mso-application'>
      <xsl:text>progid="Word.Document"</xsl:text>
    </xsl:processing-instruction>
    <xsl:text>
</xsl:text>

    <xsl:variable name='info'
      select='$doc/d:book/d:bookinfo|$doc/d:article/d:articleinfo'/>
    <xsl:variable name='authors' select='$info/d:author|$info/d:authorinitials|$info/d:authorgroup/d:author|$info/d:authorgroup/d:editor'/>

    <w:wordDocument
      w:macrosPresent="no" w:embeddedObjPresent="no" w:ocxPresent="no">
      <xsl:attribute name='xml:space'>preserve</xsl:attribute>

      <o:DocumentProperties>
        <o:Author>
          <xsl:choose>
            <xsl:when test='$authors'>
              <xsl:apply-templates select='$authors[1]' mode='docprop.author'/>
            </xsl:when>
            <xsl:otherwise>Unknown</xsl:otherwise>
          </xsl:choose>
        </o:Author>
        <o:LastAuthor>
          <xsl:choose>
            <xsl:when test='$info/d:revhistory/d:revision[1]/*[self::d:author|self::d:authorinitials]'>
              <xsl:apply-templates select='$info/d:revhistory/d:revision[1]/*[self::d:author|self::d:authorinitials]' mode='docprop.author'/>
            </xsl:when>
            <xsl:when test='$authors'>
              <xsl:apply-templates select='$authors[1]' mode='docprop.author'/>
            </xsl:when>
            <xsl:otherwise>Unknown</xsl:otherwise>
          </xsl:choose>
        </o:LastAuthor>
        <o:Revision>1</o:Revision>
        <o:TotalTime></o:TotalTime>

        <!-- dummy values -->
        <o:Created>2004-01-01T07:07:00Z</o:Created>
        <o:LastSaved>2004-01-01T08:08:00Z</o:LastSaved>

        <o:Pages>1</o:Pages>
        <o:Words>1</o:Words>
        <o:Characters>1</o:Characters>

        <!-- could derive this from author -->
        <o:Company>DocBook</o:Company>

        <o:Lines>1</o:Lines>
        <o:Paragraphs>1</o:Paragraphs>
        <o:CharactersWithSpaces>1</o:CharactersWithSpaces>
        <o:Version>11.6113</o:Version>
      </o:DocumentProperties>

      <xsl:apply-templates select='$templatedoc/w:wordDocument/o:CustomDocumentProperties|$templatedoc/w:wordDocument/w:fonts|$templatedoc/w:wordDocument/w:lists|$templatedoc/w:wordDocument/w:styles' mode='copy'/>

      <w:docPr>
        <w:view w:val="print"/>
        <w:zoom w:percent="100"/>
        <w:doNotEmbedSystemFonts/>
        <w:attachedTemplate w:val=""/>
        <w:defaultTabStop w:val="720"/>
        <w:autoHyphenation/>
        <w:hyphenationZone w:val="357"/>
        <w:doNotHyphenateCaps/>
        <w:evenAndOddHeaders/>
        <w:characterSpacingControl w:val="DontCompress"/>
        <w:optimizeForBrowser/>
        <w:validateAgainstSchema/>
        <w:saveInvalidXML w:val="off"/>
        <w:ignoreMixedContent w:val="off"/>
        <w:alwaysShowPlaceholderText w:val="off"/>
        <w:footnotePr>
          <w:footnote w:type="separator">
            <w:p>
              <w:r>
                <w:separator/>
              </w:r>
            </w:p>
          </w:footnote>
          <w:footnote w:type="continuation-separator">
            <w:p>
              <w:r>
                <w:continuationSeparator/>
              </w:r>
            </w:p>
          </w:footnote>
        </w:footnotePr>
        <w:endnotePr>
          <w:endnote w:type="separator">
            <w:p>
              <w:r>
                <w:separator/>
              </w:r>
            </w:p>
          </w:endnote>
          <w:endnote w:type="continuation-separator">
            <w:p>
              <w:r>
                <w:continuationSeparator/>
              </w:r>
            </w:p>
          </w:endnote>
        </w:endnotePr>
        <w:compat>
          <w:breakWrappedTables/>
          <w:snapToGridInCell/>
          <w:wrapTextWithPunct/>
          <w:useAsianBreakRules/>
          <w:useWord2002TableStyleRules/>
        </w:compat>
        <w:docVars>
        </w:docVars>
      </w:docPr>

      <xsl:apply-templates select='$doc/*' mode='toplevel'/>

    </w:wordDocument>
  </xsl:template>

  <xsl:template match='d:author|d:editor' mode='docprop.author'>
    <xsl:apply-templates select='d:firstname|d:personname/d:firstname' mode='docprop.author'/>
    <xsl:text> </xsl:text>
    <xsl:apply-templates select='d:surname|d:personname/d:surname' mode='docprop.author'/>
  </xsl:template>
  <xsl:template match='d:authorinitials' mode='docprop.author'>
    <xsl:value-of select='.'/>
  </xsl:template>

  <xsl:template match='d:book|d:article' mode='toplevel'>
    <w:body>
      <wx:sect>
        <wx:sub-section>
          <xsl:apply-templates select='*'/>
        </wx:sub-section>
      </wx:sect>
    </w:body>
  </xsl:template>
  <xsl:template match='*' mode='toplevel'>
    <w:body>
      <wx:sect>
        <wx:sub-section>
          <xsl:apply-templates select='*'/>
        </wx:sub-section>
      </wx:sect>
    </w:body>
  </xsl:template>

  <xsl:template match='d:book|d:article|d:chapter|d:section|d:sect1|d:sect2|d:sect3|d:sect4|d:sect5|d:simplesect'>
    <wx:sub-section>
      <xsl:apply-templates select='*'/>
    </wx:sub-section>
  </xsl:template>

  <xsl:template match='d:articleinfo |
		       d:chapterinfo |
		       d:bookinfo'>
    <xsl:apply-templates select='d:title|d:subtitle|d:titleabbrev'/>
    <xsl:apply-templates select='d:author|d:releaseinfo'/>
    <!-- current implementation ignores all other metadata -->
    <xsl:for-each select='*[not(self::d:title|self::d:subtitle|self::d:titleabbrev|self::d:author|self::d:releaseinfo)]'>
      <xsl:call-template name='nomatch'/>
    </xsl:for-each>
  </xsl:template>

  <xsl:template match='d:title|d:subtitle|d:titleabbrev'>
    <w:p>
      <w:pPr>
        <w:pStyle>
          <xsl:attribute name='w:val'>
            <xsl:choose>
              <xsl:when test='(parent::d:section or
                              parent::d:sectioninfo/parent::d:section) and
                              count(ancestor::d:section) > 5'>
                <xsl:message>section nested deeper than 5 levels</xsl:message>
                <xsl:text>sect5-</xsl:text>
                <xsl:value-of select='name()'/>
              </xsl:when>
              <xsl:when test='parent::d:section or
                              parent::d:sectioninfo/parent::d:section'>
                <xsl:text>sect</xsl:text>
                <xsl:value-of select='count(ancestor::d:section)'/>
                <xsl:text>-</xsl:text>
                <xsl:value-of select='name()'/>
              </xsl:when>
              <xsl:when test='contains(name(..), "d:info")'>
                <xsl:value-of select='name(../..)'/>
                <xsl:text>-</xsl:text>
                <xsl:value-of select='name()'/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select='name(..)'/>
                <xsl:text>-</xsl:text>
                <xsl:value-of select='name()'/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:attribute>
        </w:pStyle>
        <w:outlineLvl w:val='{count(ancestor::*) - count(parent::*[contains(name(), "info")]) - 1}'/>
      </w:pPr>

      <xsl:choose>
        <xsl:when test='contains(name(..), "d:info")'>
          <xsl:call-template name='attributes'>
            <xsl:with-param name='node' select='../..'/>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:call-template name='attributes'>
            <xsl:with-param name='node' select='..'/>
          </xsl:call-template>
        </xsl:otherwise>
      </xsl:choose>

      <xsl:apply-templates/>
    </w:p>
  </xsl:template>

  <doc:template name='metadata' xmlns=''>
    <title>Metadata</title>

    <para>TODO: Handle all metadata elements, apart from titles.</para>
  </doc:template>
  <xsl:template match='*[contains(name(), "d:info")]/*[not(self::d:title|self::d:subtitle|self::d:titleabbrev)]' priority='0'/>

  <xsl:template match='d:author|d:editor|d:othercredit'>
    <w:p>
      <w:pPr>
        <w:pStyle w:val='{name()}'/>
      </w:pPr>

      <xsl:call-template name='attributes'/>

      <xsl:apply-templates select='d:personname|d:surname|d:firstname|d:honorific|d:lineage|d:othername|d:contrib'/>
    </w:p>
    <xsl:apply-templates select='d:affiliation|d:address'/>
    <xsl:apply-templates select='d:authorblurb|d:personblurb'/>
  </xsl:template>
  <xsl:template match='d:affiliation'>
    <w:p>
      <w:pPr>
        <w:pStyle w:val='affiliation'/>
      </w:pPr>

      <xsl:call-template name='attributes'/>

      <xsl:apply-templates/>
    </w:p>
  </xsl:template>
  <xsl:template match='d:address[parent::d:author|parent::d:editor|parent::d:othercredit]'>
    <w:p>
      <w:pPr>
        <w:pStyle w:val='para-continue'/>
      </w:pPr>

      <xsl:call-template name='attributes'/>

      <xsl:apply-templates/>
    </w:p>
  </xsl:template>
  <!-- do not attempt to handle recursive structures -->
  <xsl:template match='d:address[not(parent::d:author|parent::d:editor|parent::d:othercredit)]'>
    <xsl:apply-templates select='node()[not(self::d:affiliation|self::d:authorblurb)]'/>
  </xsl:template>
  <!-- TODO -->
  <xsl:template match='d:authorblurb|d:personblurb'/>

  <!-- TODO: handle inline markup (eg. emphasis) -->
  <xsl:template match='d:surname|d:firstname|d:honorific|d:lineage|d:othername|d:contrib|d:email|d:shortaffil|d:jobtitle|d:orgname|d:orgdiv|d:street|d:pob|d:postcode|d:city|d:state|d:country|d:phone|d:fax|d:citetitle'>
    <xsl:if test='preceding-sibling::*'>
      <w:r>
        <w:t>
          <xsl:text> </xsl:text>
        </w:t>
      </w:r>
    </xsl:if>
    <xsl:call-template name='handle-linebreaks'>
      <xsl:with-param name='style' select='name()'/>
    </xsl:call-template>
  </xsl:template>
  <xsl:template match='d:email'>
    <xsl:variable name='address'>
      <xsl:choose>
        <xsl:when test='starts-with(., "mailto:")'>
          <xsl:value-of select='.'/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>mailto:</xsl:text>
          <xsl:value-of select='.'/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <w:hlink w:dest='{$address}'>
      <xsl:call-template name='handle-linebreaks'>
	<xsl:with-param name='style'>Hyperlink</xsl:with-param>
      </xsl:call-template>
    </w:hlink>
  </xsl:template>
  <!-- otheraddr often contains ulink -->
  <xsl:template match='d:otheraddr'>
    <xsl:choose>
      <xsl:when test='d:ulink'>
        <xsl:for-each select='d:ulink'>
          <xsl:variable name='prev' select='preceding-sibling::d:ulink[1]'/>
          <xsl:choose>
            <xsl:when test='$prev'>
              <xsl:for-each
                select='preceding-sibling::node()[generate-id(following-sibling::d:ulink[1]) = generate-id(current())]'>
		<xsl:call-template name='handle-linebreaks'>
		  <xsl:with-param name='style'>otheraddr</xsl:with-param>
		</xsl:call-template>
              </xsl:for-each>
            </xsl:when>
            <xsl:when test='preceding-sibling::node()'>
	      <xsl:call-template name='handle-linebreaks'>
		<xsl:with-param name='style'>otheraddr</xsl:with-param>
	      </xsl:call-template>
            </xsl:when>
          </xsl:choose>
          <xsl:apply-templates select='.'/>
        </xsl:for-each>
        <xsl:if test='d:ulink[last()]/following-sibling::node()'>
	  <xsl:call-template name='handle-linebreaks'>
	    <xsl:with-param name='text'
	      select='d:ulink[last()]/following-sibling::node()'/>
	    <xsl:with-param name='style'>otheraddr</xsl:with-param>
	  </xsl:call-template>
        </xsl:if>
      </xsl:when>
      <xsl:otherwise>
	<xsl:call-template name='handle-linebreaks'>
	  <xsl:with-param name='style'>otheraddr</xsl:with-param>
	</xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  <xsl:template match='d:ulink'>
    <w:hlink w:dest='{@url}'>
      <xsl:call-template name='handle-linebreaks'>
	<xsl:with-param name='style'>Hyperlink</xsl:with-param>
      </xsl:call-template>
    </w:hlink>
  </xsl:template>

  <!-- Cannot round-trip this element -->
  <xsl:template match='d:personname'>
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match='d:releaseinfo'>
    <w:p>
      <w:pPr>
        <w:pStyle w:val='releaseinfo'/>
      </w:pPr>

      <xsl:call-template name='attributes'/>

      <xsl:apply-templates/>
    </w:p>
  </xsl:template>

  <xsl:template match='d:para'>
    <xsl:param name='class'/>

    <xsl:variable name='block' select='d:blockquote|d:calloutlist|d:classsynopsis|d:funcsynopsis|d:figure|d:glosslist|d:graphic|d:informalfigure|d:informaltable|d:itemizedlist|d:literallayout|d:mediaobject|d:mediaobjectco|d:note|d:caution|d:warning|d:important|d:tip|d:orderedlist|d:programlisting|d:revhistory|d:segmentedlist|d:simplelist|d:table|d:variablelist'/>

    <xsl:choose>
      <xsl:when test='$block'>
        <w:p>
          <w:pPr>
            <w:pStyle>
              <xsl:attribute name='w:val'>
                <xsl:choose>
                  <xsl:when test='$class != ""'>
                    <xsl:value-of select='$class'/>
                  </xsl:when>
                  <xsl:otherwise>Normal</xsl:otherwise>
                </xsl:choose>
              </xsl:attribute>
            </w:pStyle>
          </w:pPr>

          <xsl:call-template name='attributes'/>

          <xsl:apply-templates select='$block[1]/preceding-sibling::node()'/>
        </w:p>
        <xsl:for-each select='$block'>
          <xsl:apply-templates select='.'/>
          <w:p>
            <w:pPr>
              <w:pStyle>
                <xsl:attribute name='w:val'>
                  <xsl:choose>
                    <xsl:when test='$class != ""'>
                      <xsl:value-of select='$class'/>
                    </xsl:when>
                    <xsl:otherwise>Normal</xsl:otherwise>
                  </xsl:choose>
                </xsl:attribute>
              </w:pStyle>
            </w:pPr>
            <xsl:apply-templates select='following-sibling::node()[generate-id(preceding-sibling::*[self::d:blockquote|self::d:calloutlist|self::d:figure|self::d:glosslist|self::d:graphic|self::d:informalfigure|self::d:informaltable|self::d:itemizedlist|self::d:literallayout|self::d:mediaobject|self::d:mediaobjectco|self::d:note|self::d:caution|self::d:warning|self::d:important|self::d:tip|self::d:orderedlist|self::d:programlisting|self::d:revhistory|self::d:segmentedlist|self::d:simplelist|self::d:table|self::d:variablelist][1]) = generate-id(current())]'/>
          </w:p>
        </xsl:for-each>
      </xsl:when>
      <xsl:otherwise>
        <w:p>
          <w:pPr>
            <w:pStyle>
              <xsl:attribute name='w:val'>
                <xsl:choose>
                  <xsl:when test='$class != ""'>
                    <xsl:value-of select='$class'/>
                  </xsl:when>
                  <xsl:otherwise>Normal</xsl:otherwise>
                </xsl:choose>
              </xsl:attribute>
            </w:pStyle>
          </w:pPr>

          <xsl:call-template name='attributes'/>

          <xsl:apply-templates/>
        </w:p>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  <xsl:template match='d:simpara'>
    <xsl:param name='class'/>

    <w:p>
      <w:pPr>
        <w:pStyle>
          <xsl:attribute name='w:val'>
            <xsl:choose>
              <xsl:when test='$class != ""'>
                <xsl:value-of select='concat("sim-", $class)'/>
              </xsl:when>
              <xsl:otherwise>simpara</xsl:otherwise>
            </xsl:choose>
          </xsl:attribute>
        </w:pStyle>
      </w:pPr>

      <xsl:call-template name='attributes'/>

      <xsl:apply-templates/>
    </w:p>
  </xsl:template>

  <xsl:template match='d:emphasis'>
    <w:r>
      <w:rPr>
        <xsl:if test='@role = "bold" or @role = "d:strong"'>
          <w:b/>
        </xsl:if>
        <xsl:if test='not(@role)'>
          <w:i/>
        </xsl:if>
      </w:rPr>
      <w:t>
	<!-- TODO: use handle-linebreaks -->
        <xsl:value-of select='.'/>
      </w:t>
    </w:r>
  </xsl:template>

  <xsl:template match='d:informalfigure'>
    <xsl:if test='d:mediaobject/d:imageobject/d:imagedata'>
      <w:p>
	<w:pPr>
	  <w:pStyle w:val='informalfigure-imagedata'/>
	</w:pPr>
	<xsl:call-template name='attributes'/>

	<w:r>
	  <w:t>
	    <xsl:apply-templates select='d:mediaobject/d:imageobject/d:imagedata/@fileref'
				 mode='textonly'/>
	  </w:t>
	</w:r>
      </w:p>
    </xsl:if>
    <xsl:for-each select='*[not(self::d:mediaobject)]'>
      <xsl:call-template name='nomatch'/>
    </xsl:for-each>
  </xsl:template>

  <xsl:template match='d:mediaobject|d:mediaobjectco'>
    <xsl:apply-templates select='d:objectinfo/d:title'/>
    <xsl:apply-templates select='d:objectinfo/d:subtitle'/>
    <!-- TODO: indicate error for other children of objectinfo -->

    <xsl:apply-templates select='*[not(self::d:objectinfo)]'/>
  </xsl:template>
  <xsl:template match='d:imageobject|d:imageobjectco|d:audioobject|d:videoobject'>
    <xsl:apply-templates select='d:objectinfo/d:title'/>
    <xsl:apply-templates select='d:objectinfo/d:subtitle'/>
    <!-- TODO: indicate error for other children of objectinfo -->

    <xsl:apply-templates select='d:areaspec'/>

    <xsl:choose>
      <xsl:when test='d:imagedata|d:audiodata|d:videodata'>
	<w:p>
	  <w:pPr>
	    <w:pStyle w:val='{name()}-{name(imagedata|audiodata|videodata)}'/>
	  </w:pPr>
	  <xsl:call-template name='attributes'/>

	  <w:r>
	    <w:t>
	      <xsl:apply-templates select='*/@fileref'
				   mode='textonly'/>
	    </w:t>
	  </w:r>
	</w:p>
      </xsl:when>
      <xsl:when test='self::d:imageobjectco/d:imageobject/d:imagedata'>
	<w:p>
	  <w:pPr>
	    <w:pStyle w:val='{name()}-imagedata'/>
	  </w:pPr>
	  <xsl:call-template name='attributes'/>

	  <w:r>
	    <w:t>
	      <xsl:apply-templates select='*/@fileref'
				   mode='textonly'/>
	    </w:t>
	  </w:r>
	</w:p>
      </xsl:when>
    </xsl:choose>
    <xsl:apply-templates select='d:calloutlist'/>

    <xsl:for-each select='*[not(self::d:imageobject |
			        self::d:imagedata |
			        self::d:audiodata |
				self::d:videodata |
				self::d:areaspec  |
				self::d:calloutlist)]'>
      <xsl:call-template name='nomatch'/>
    </xsl:for-each>
  </xsl:template>
  <xsl:template match='d:textobject'>
    <xsl:choose>
      <xsl:when test='d:objectinfo/d:title|d:objectinfo|d:subtitle'>
	<xsl:apply-templates select='d:objectinfo/d:title'/>
	<xsl:apply-templates select='d:objectinfo/d:subtitle'/>
	<!-- TODO: indicate error for other children of objectinfo -->
      </xsl:when>
      <xsl:otherwise>
	<!-- synthesize a title so that the parent textobject
	     can be recreated.
	  -->
	<w:p>
	  <w:pPr>
	    <w:pStyle w:val='textobject-title'/>
	  </w:pPr>

	  <w:r>
	    <w:t>
	      <xsl:text>Text Object </xsl:text>
	      <xsl:number level='any'/>
	    </w:t>
	  </w:r>
	</w:p>
      </xsl:otherwise>
    </xsl:choose>

    <xsl:apply-templates select='*[not(self::d:objectinfo)]'/>
  </xsl:template>

  <xsl:template match='d:caption'>
    <w:p>
      <w:pPr>
	<w:pStyle w:val='caption'/>
      </w:pPr>

      <xsl:choose>
	<xsl:when test='not(*)'>
	  <xsl:apply-templates/>
	</xsl:when>
	<xsl:otherwise>
	  <xsl:apply-templates select='d:para[1]/node()'/>
	  <xsl:for-each select='text()|*[not(self::d:para)]|d:para[position() != 1]'>
	    <xsl:call-template name='nomatch'/>
	  </xsl:for-each>
	</xsl:otherwise>
      </xsl:choose>
    </w:p>
  </xsl:template>

  <xsl:template match='d:areaspec'>
    <w:p>
      <w:pPr>
	<w:pStyle w:val='{name()}'/>
      </w:pPr>
      <xsl:call-template name='attributes'/>

      <w:r>
	<w:t></w:t>
      </w:r>
    </w:p>
  </xsl:template>
  <xsl:template match='d:area'>
    <w:p>
      <w:pPr>
	<w:pStyle w:val='{name()}'/>
      </w:pPr>
      <xsl:call-template name='attributes'/>

      <w:r>
	<w:t></w:t>
      </w:r>
    </w:p>
  </xsl:template>

  <xsl:template match='d:calloutlist'>
    <xsl:apply-templates select='d:callout'/>
  </xsl:template>

  <xsl:template match='d:callout'>
    <w:p>
      <w:pPr>
	<w:pStyle w:val='callout'/>
      </w:pPr>
      <xsl:call-template name='attributes'/>

      <!-- Normally a para would be the first child of a callout -->
      <xsl:apply-templates select='*[1][self::d:para]/node()' mode='list'/>
    </w:p>
    <!-- This is to catch the case where a listitem's first child is not a paragraph.
       - We may not be able to represent this properly.
      -->
    <xsl:apply-templates select='*[1][not(self::d:para)]' mode='list'/>

    <xsl:apply-templates select='*[position() != 1]' mode='list'/>
  </xsl:template>

  <xsl:template match='d:table|d:informaltable'>

    <w:tbl>
      <w:tblPr>
        <w:tblW w:w="0" w:type="auto"/>
        <w:tblInd w:w="108" w:type="dxa"/>
        <w:tblLayout w:type="Fixed"/>
      </w:tblPr>
      <w:tblGrid>
        <xsl:apply-templates select='d:tgroup/d:colspec' mode='column'/>
      </w:tblGrid>
      <xsl:apply-templates/>
    </w:tbl>
  </xsl:template>

  <xsl:template match='d:colspec' mode='column'>
    <w:gridcol w:w='{@colwidth}'/>
  </xsl:template>

  <xsl:template match='d:colspec'/>

  <xsl:template name='repeat'>
    <xsl:param name='repeats' select='0'/>
    <xsl:param name='content'/>

    <xsl:if test='$repeats > 0'>
      <xsl:copy-of select='$content'/>
      <xsl:call-template name='repeat'>
        <xsl:with-param name='repeats' select='$repeats - 1'/>
        <xsl:with-param name='content' select='$content'/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>
  <xsl:template match='d:tgroup|d:tbody|d:thead'>
    <xsl:apply-templates/>
  </xsl:template>
  <xsl:template match='d:row'>
    <w:tr>
      <w:trPr>
        <xsl:if test='parent::d:thead'>
          <w:tblHeader/>
        </xsl:if>
      </w:trPr>
      <xsl:apply-templates/>
    </w:tr>
  </xsl:template>

  <xsl:template match='d:entry'>

    <!-- 
         Position = Sum(i,preceding-sibling[@colspan = ""]) + entry[i].@colspan)
      -->

    <xsl:variable name='position'>
      <xsl:call-template name='sumSibling'>
        <xsl:with-param name='sum' select='"1"'/>
        <xsl:with-param name='node' select='.'/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name='limit' select='$position + @colspan'/>
    <w:tc>
      <w:tcPr>
        <xsl:choose>
          <xsl:when test='@colspan != ""'>

            <!-- Select all the colspec nodes which correspond to the
                 column. That is all the nodes between the current 
                 column number and the column number plus the span.
              -->

            <xsl:variable name='combinedWidth'>
              <xsl:call-template name='sum'>
                <xsl:with-param name='nodes' select='ancestor::*[self::d:table|self::d:informaltable][1]/d:tgroup/d:colspec[not(position() &lt; $position) and position() &lt; $limit]'/>
                <xsl:with-param name='sum' select='"0"'/>
              </xsl:call-template>
            </xsl:variable>
            <w:tcW w:w='{$combinedWidth}' w:type='dxa'/>
          </xsl:when>
          <xsl:otherwise>
            <w:tcW w:w='{ancestor::*[self::table|self::informaltable][1]/tgroup/colspec[position() = $position]/@colwidth}' w:type='dxa'/>
          </xsl:otherwise>
        </xsl:choose>

      </w:tcPr>
      <xsl:if test='@hidden != ""'>
          <w:vmerge w:val=''/>
      </xsl:if>
      <xsl:if test='@rowspan != ""'>          
        <w:vmerge w:val='restart'/>
      </xsl:if>        
      <xsl:if test='@colspan != ""'>
        <w:gridspan w:val='{@colspan}'/>
      </xsl:if>
      <xsl:choose>
        <xsl:when test='not(d:para)'>
          <!-- TODO: check for any block elements -->
          <w:p>
            <xsl:apply-templates/>
          </w:p>
        </xsl:when>
        <xsl:otherwise>
          <xsl:apply-templates/>
        </xsl:otherwise>
      </xsl:choose>
    </w:tc>
  </xsl:template>

  <!-- Calculates the position by adding the 
       count of the preceding siblings where they aren't colspans
       and adding the colspans of those entries which do.
    -->

  <xsl:template name='sumSibling'>    
    <xsl:param name='sum'/>
    <xsl:param name='node'/>

    <xsl:variable name='add'>
      <xsl:choose>
        <xsl:when test='$node/preceding-sibling::d:entry/@colspan != ""'>
          <xsl:value-of select='$node/preceding-sibling::d:entry/@colspan'/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select='"1"'/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test='count($node/preceding-sibling::d:entry) &gt; 0'>
        <xsl:call-template name='sumSibling'>
          <xsl:with-param name='sum' select='$sum + $add'/>
          <xsl:with-param name='node' select='$node/preceding-sibling::d:entry[1]'/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select='$sum'/>
      </xsl:otherwise>
    </xsl:choose>
    
  </xsl:template>

  <xsl:template name='sum'>
    <xsl:param name='sum' select='"0"'/>
    <xsl:param name='nodes'/>

    <xsl:variable name='tmpSum' select='$sum + $nodes[1]/@colwidth'/>

    <xsl:choose>
      <xsl:when test='count($nodes) &gt; 1'>
        <xsl:call-template name='sum'>
          <xsl:with-param name='nodes' select='$nodes[position() != 1]'/>
          <xsl:with-param name='sum' select='$tmpSum'/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select='$tmpSum'/>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template match='*[self::d:para|self::d:simpara]/text()[string-length(normalize-space(.)) != 0]'>
    <xsl:call-template name='handle-linebreaks'/>
  </xsl:template>

  <xsl:template match='text()[not(parent::d:para|parent::d:simpara|parent::d:literallayout|parent::d:programlisting)][string-length(normalize-space(.)) != 0]'>
    <xsl:call-template name='handle-linebreaks'/>
  </xsl:template>
  <xsl:template match='text()[string-length(normalize-space(.)) = 0]'/>
  <xsl:template match='d:literallayout/text()|d:programlisting/text()'>
    <xsl:call-template name='handle-linebreaks'/>
  </xsl:template>
  <xsl:template name='handle-linebreaks'>
    <xsl:param name='text' select='.'/>
    <xsl:param name='style'/>

    <xsl:choose>
      <xsl:when test='not($text)'/>
      <xsl:when test='contains($text, "&#d:xa;")'>
        <w:r>
	  <xsl:if test='$style != ""'>
	    <w:rPr>
	      <w:rStyle w:val='{$style}'/>
	    </w:rPr>
	  </xsl:if>
          <w:t>
            <xsl:value-of select='substring-before($text, "&#d:xa;")'/>
          </w:t>
        </w:r>
        <xsl:call-template name='handle-linebreaks-aux'>
          <xsl:with-param name='text'
            select='substring-after($text, "&#d:xa;")'/>
	  <xsl:with-param name='style' select='$style'/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <w:r>
	  <xsl:if test='$style != ""'>
	    <w:rPr>
	      <w:rStyle w:val='{$style}'/>
	    </w:rPr>
	  </xsl:if>
          <w:t>
            <xsl:value-of select='$text'/>
          </w:t>
        </w:r>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- pre-condition: leading linefeed has been stripped -->
  <xsl:template name='handle-linebreaks-aux'>
    <xsl:param name='text'/>
    <xsl:param name='style'/>

    <xsl:choose>
      <xsl:when test='contains($text, "&#d:xa;")'>
        <w:r>
	  <xsl:if test='$style != ""'>
	    <w:rPr>
	      <w:rStyle w:val='{$style}'/>
	    </w:rPr>
	  </xsl:if>
          <w:br/>
          <w:t>
            <xsl:value-of select='substring-before($text, "&#d:xa;")'/>
          </w:t>
        </w:r>
        <xsl:call-template name='handle-linebreaks-aux'>
          <xsl:with-param name='text' select='substring-after($text, "&#d:xa;")'/>
	  <xsl:with-param name='style' select='$style'/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <w:r>
	  <xsl:if test='$style != ""'>
	    <w:rPr>
	      <w:rStyle w:val='{$style}'/>
	    </w:rPr>
	  </xsl:if>
          <w:br/>
          <w:t>
            <xsl:value-of select='$text'/>
          </w:t>
        </w:r>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match='d:authorblurb|d:formalpara|d:legalnotice|d:note|d:caution|d:warning|d:important|d:tip'>
    <xsl:apply-templates select='*'>
      <xsl:with-param name='class'>
        <xsl:value-of select='name()'/>
      </xsl:with-param>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template match='d:blockquote'>
    <xsl:apply-templates select='d:blockinfo|d:title'>
      <xsl:with-param name='class'>
        <xsl:value-of select='name()'/>
      </xsl:with-param>
    </xsl:apply-templates>
    <xsl:apply-templates select='*[not(self::d:blockinfo|self::d:title|self::d:attribution)]'>
      <xsl:with-param name='class' select='"d:blockquote"'/>
    </xsl:apply-templates>
    <xsl:if test='d:attribution'>
      <w:p>
        <w:pPr>
          <w:pStyle w:val='blockquote-attribution'/>
        </w:pPr>

        <xsl:call-template name='attributes'/>

        <xsl:apply-templates select='d:attribution/node()'/>
      </w:p>
    </xsl:if>
  </xsl:template>

  <xsl:template match='d:literallayout|d:programlisting'>
    <xsl:param name='class'/>

    <w:p>
      <w:pPr>
        <w:pStyle w:val='{name()}'/>
      </w:pPr>

      <xsl:call-template name='attributes'/>

      <xsl:apply-templates/>
    </w:p>
  </xsl:template>

  <xsl:template match='d:bridgehead'>
    <w:p>
      <w:pPr>
        <w:pStyle w:val='bridgehead'/>
      </w:pPr>

      <xsl:call-template name='attributes'/>

      <xsl:apply-templates/>
    </w:p>
  </xsl:template>

  <xsl:template match='d:itemizedlist|d:orderedlist'>
    <xsl:apply-templates select='d:listitem'/>
  </xsl:template>

  <xsl:template match='d:listitem'>
    <w:p>
      <w:pPr>
        <!-- variablelist listitems are not handled here -->
        <w:pStyle w:val='{name(..)}{count(ancestor::itemizedlist|ancestor::orderedlist)}'/>
        <w:listPr>
          <wx:t wx:val='&#xB7;'/>
          <wx:font wx:val='Symbol'/>
        </w:listPr>
      </w:pPr>
      <!-- Normally a para would be the first child of a listitem -->
      <xsl:apply-templates select='*[1][self::d:para]/node()' mode='list'/>
    </w:p>
    <!-- This is to catch the case where a listitem's first child is not a paragraph.
       - We may not be able to represent this properly.
      -->
    <xsl:apply-templates select='*[1][not(self::d:para)]' mode='list'/>

    <xsl:apply-templates select='*[position() != 1]' mode='list'/>
  </xsl:template>  

  <xsl:template match='*' mode='list'>
    <xsl:apply-templates select='.'>
      <xsl:with-param name='class' select='"para-continue"'/>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template match='d:variablelist'>
    <xsl:apply-templates select='*[not(self::d:varlistentry)]'/>

    <w:tbl>
      <w:tblPr>
        <w:tblW w:w='0' w:type='auto'/>
        <w:tblInd w:w='108' w:type='dxa'/>
        <w:tblLayout w:type='Fixed'/>
      </w:tblPr>
      <w:tblGrid>
        <w:gridcol w:w='2160'/>
        <w:gridcol w:w='6480'/>
      </w:tblGrid>
      <xsl:apply-templates select='d:varlistentry'/>
    </w:tbl>
  </xsl:template>
  <xsl:template match='d:varlistentry'>
    <w:tr>
      <w:trPr>
      </w:trPr>

      <w:tc>
        <w:tcPr>
          <w:tcW w:w='2160' w:type='dxa'/>
        </w:tcPr>
        <w:p>
          <w:pPr>
            <w:pStyle w:val='variablelist-term'/>
          </w:pPr>
          <xsl:apply-templates select='d:term[1]/node()'/>
          <xsl:for-each select='d:term[position() != 1]'>
            <w:r>
              <w:br/>
            </w:r>
            <xsl:apply-templates/>
          </xsl:for-each>
        </w:p>
      </w:tc>
      <w:tc>
        <w:tcPr>
          <w:tcW w:w='6480' w:type='dxa'/>
        </w:tcPr>
        <xsl:apply-templates select='d:listitem/node()'/>
      </w:tc>
    </w:tr>
  </xsl:template>

  <!-- These elements are not displayed.
     - However, they may need to be added (perhaps as hidden text)
     - for round-tripping.
    -->
  <xsl:template match='d:anchor|d:areaset|d:audiodata|d:audioobject|
                       d:beginpage|
                       d:constraint|
                       d:indexterm|d:itermset|
                       d:keywordset|
                       d:msg'/>

  <xsl:template match='*' name='nomatch'>
    <xsl:message>
      <xsl:value-of select='name()'/>
      <xsl:text> encountered</xsl:text>
      <xsl:if test='parent::*'>
        <xsl:text> in </xsl:text>
        <xsl:value-of select='name(parent::*)'/>
      </xsl:if>
      <xsl:text>, but no template matches.</xsl:text>
    </xsl:message>

    <xsl:choose>
      <xsl:when test='self::d:abstract |
                      self::d:ackno |
                      self::d:address |
                      self::d:answer |
                      self::d:appendix |
                      self::d:artheader |
                      self::d:authorgroup |
                      self::d:bibliodiv |
                      self::d:biblioentry |
                      self::d:bibliography |
                      self::d:bibliomixed |
                      self::d:bibliomset |
                      self::d:biblioset |
                      self::d:bridgehead |
                      self::d:calloutlist |
                      self::d:caption |
                      self::d:classsynopsis |
                      self::d:colophon |
                      self::d:constraintdef |
                      self::d:copyright |
                      self::d:dedication |
                      self::d:epigraph |
                      self::d:equation |
                      self::d:example |
                      self::d:figure |
                      self::d:funcsynopsis |
                      self::d:glossary |
                      self::d:glossdef |
                      self::d:glossdiv |
                      self::d:glossentry |
                      self::d:glosslist |
                      self::d:graphic |
                      self::d:highlights |
                      self::d:imageobject |
                      self::d:imageobjectco |
                      self::d:index |
                      self::d:indexdiv |
                      self::d:indexentry |
                      self::d:informalequation |
                      self::d:informalexample |
                      self::d:informalfigure |
                      self::d:lot |
                      self::d:lotentry |
                      self::d:mediaobject |
                      self::d:mediaobjectco |
                      self::d:member |
                      self::d:msgentry |
                      self::d:msgset |
                      self::d:part |
                      self::d:partintro |
                      self::d:personblurb |
                      self::d:preface |
                      self::d:printhistory |
                      self::d:procedure |
                      self::d:programlisting |
                      self::d:programlistingco |
                      self::d:publisher |
                      self::d:qandadiv |
                      self::d:qandaentry |
                      self::d:qandaset |
                      self::d:question |
                      self::d:refdescriptor |
                      self::d:refentry |
                      self::d:refentrytitle |
                      self::d:reference |
                      self::d:refmeta |
                      self::d:refname |
                      self::d:refnamediv |
                      self::d:refpurpose |
                      self::d:refsect1 |
                      self::d:refsect2 |
                      self::d:refsect3 |
                      self::d:refsection |
                      self::d:refsynopsisdiv |
                      self::d:screen |
                      self::d:screenco |
                      self::d:screenshot |
                      self::d:seg |
                      self::d:seglistitem |
                      self::d:segmentedlist |
                      self::d:segtitle |
                      self::d:set |
                      self::d:setindex |
                      self::d:sidebar |
                      self::d:simplelist |
                      self::d:simplemsgentry |
                      self::d:step |
                      self::d:stepalternatives |
                      self::d:subjectset |
                      self::d:substeps |
                      self::d:task |
                      self::d:textobject |
                      self::d:toc |
                      self::d:videodata |
                      self::d:videoobject |
                      self::*[not(starts-with(name(), "d:informal")) and contains(name(), "d:info")]'>
        <w:p>
          <w:pPr>
            <w:pStyle w:val='blockerror'/>
          </w:pPr>
          <w:r>
            <w:t>
              <xsl:value-of select='name()'/>
              <xsl:text> encountered</xsl:text>
              <xsl:if test='parent::*'>
                <xsl:text> in </xsl:text>
                <xsl:value-of select='name(parent::*)'/>
              </xsl:if>
              <xsl:text>, but no template matches.</xsl:text>
            </w:t>
          </w:r>
        </w:p>
      </xsl:when>
      <!-- Some elements are sometimes blocks, sometimes inline
      <xsl:when test='self::d:affiliation |
                      self::d:alt |
                      self::d:attribution |
                      self::d:collab |
                      self::d:collabname |
                      self::d:confdates |
                      self::d:confgroup |
                      self::d:confnum |
                      self::d:confsponsor |
                      self::d:conftitle |
                      self::d:contractnum |
                      self::d:contractsponsor |
                      self::d:contrib |
                      self::d:corpauthor |
                      self::d:corpcredit |
                      self::d:corpname |
                      self::d:edition |
                      self::d:editor |
                      self::d:jobtitle |
                      self::d:personname |
                      self::d:publishername |
                      self::d:remark'>

      </xsl:when>
      -->
      <xsl:otherwise>
        <w:r>
          <w:rPr>
            <w:rStyle w:val='inlineerror'/>
          </w:rPr>
          <w:t>
            <xsl:value-of select='name()'/>
            <xsl:text> encountered</xsl:text>
            <xsl:if test='parent::*'>
              <xsl:text> in </xsl:text>
              <xsl:value-of select='name(parent::*)'/>
            </xsl:if>
            <xsl:text>, but no template matches.</xsl:text>
          </w:t>
        </w:r>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name='attributes'>
    <xsl:param name='node' select='.'/>

    <xsl:if test='$node/@*'>
      <aml:annotation aml:id='{count(preceding::*) + 1}' w:type='Word.Comment.Start'/>
      <w:r>
        <w:rPr>
          <w:rStyle w:val='attributes'/>
        </w:rPr>
        <w:t>
          <xsl:text> </xsl:text>
        </w:t>
      </w:r>
      <aml:annotation aml:id='{count(preceding::*) + 1}' w:type='Word.Comment.End'/>
      <w:r>
        <w:rPr>
          <w:rStyle w:val='CommentReference'/>
        </w:rPr>
        <aml:annotation aml:id='{count(preceding::*) + 1}' aml:author="DocBook" aml:createdate='2004-12-23T00:01:00' w:type='Word.Comment' w:initials='DBK'>
          <aml:content>
            <w:p>
              <w:pPr>
                <w:pStyle w:val='CommentText'/>
              </w:pPr>
              <w:r>
                <w:rPr>
                  <w:rStyle w:val='CommentReference'/>
                </w:rPr>
                <w:annotationRef/>
              </w:r>
              <xsl:for-each select='$node/@*'>
                <w:r>
                  <w:rPr>
                    <w:rStyle w:val='attribute-name'/>
                  </w:rPr>
                  <w:t>
                    <xsl:value-of select='name()'/>
                  </w:t>
                </w:r>
                <w:r>
                  <w:t>=</w:t>
                </w:r>
                <w:r>
                  <w:rPr>
                    <w:rStyle w:val='attribute-value'/>
                  </w:rPr>
                  <w:t>
                    <xsl:value-of select='.'/>
                  </w:t>
                </w:r>
              </xsl:for-each>
            </w:p>
          </aml:content>
        </aml:annotation>
      </w:r>
    </xsl:if>
  </xsl:template>

  <xsl:template match='*' mode='copy'>
    <xsl:copy>
      <xsl:for-each select='@*'>
        <xsl:copy/>
      </xsl:for-each>
      <xsl:apply-templates mode='copy'/>
    </xsl:copy>
  </xsl:template>

</xsl:stylesheet>
