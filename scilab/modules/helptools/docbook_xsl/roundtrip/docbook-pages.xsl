<?xml version="1.0" encoding="utf-8"?>
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
  xmlns:doc='http://www.oasis-open.org/docbook/xml/4.0'
  exclude-result-prefixes='doc xi w wx aml'>

  <xsl:output method="xml" indent='yes' encoding='ascii'/>

  <!-- ********************************************************************
       $Id: docbook-pages.xsl 6910 2007-06-28 23:23:30Z xmldoc $
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

      <xsl:apply-templates select='$templatedoc/sl:document/*[not(self::sf:text-storage)]' mode='copy'/>

      <xsl:apply-templates select='$doc/*' mode='toplevel'/>

      <xsl:apply-templates select='$templatedoc/sl:document/sf:text-storage/following-sibling::*' mode='copy'/>
    </sl:document>
  </xsl:template>

  <xsl:template match='d:book|d:article|d:chapter|d:preface|d:appendix' mode='toplevel'>
    <sf:text-storage sf:kind='body' sfa:ID='SFWPStorage-7'>
      <sf:stylesheet-ref sfa:IDREF='SFSStylesheet-1'/>
      <sf:text-body>
        <sf:page-start sf:page-index='0'/>
        <sf:container-hint sf:page-index="0" sf:cindex="0" sf:sindex="0" sf:lindex="0" sf:frame-x="56.692913055419922" sf:frame-y="56.692913055419922" sf:frame-w="481.61416625976562" sf:frame-h="714" sf:anchor-loc="0"/>

        <sf:section sf:name="Chapter 1" sf:style="section-style-0">
          <sf:layout sf:style="layout-style-20">
            <xsl:apply-templates select='*'/>
          </sf:layout>
        </sf:section>
      </sf:text-body>
    </sf:text-storage>
  </xsl:template>

  <xsl:template match='d:book|d:article|d:part|d:section|d:sect1|d:sect2|d:sect3|d:sect4|d:sect5|d:simplesect|d:bibliodiv'>
    <xsl:apply-templates select='*'/>
  </xsl:template>

  <xsl:template match='d:articleinfo|d:bookinfo|d:chapterinfo|d:prefaceinfo|d:appendixinfo'>
    <xsl:apply-templates select='d:title|d:subtitle|d:titleabbrev'/>
    <xsl:apply-templates select='d:author|d:releaseinfo|d:revhistory|d:abstract'/>
    <!-- current implementation ignores all other metadata -->
    <xsl:for-each select='*[not(self::d:title|self::d:subtitle|self::d:titleabbrev|self::d:author|self::d:releaseinfo|self::d:revhistory|self::d:abstract)]'>
      <xsl:call-template name='nomatch'/>
    </xsl:for-each>
  </xsl:template>

  <!-- It is easier for authors to have metadata for a component 
       appearing after the corresponding title, rather than before it.
       The reverse transformation will put things back the right way.
    -->

  <xsl:template match='d:sectioninfo|d:sect1info|d:sect2info|d:sect3info|d:sect4info|d:sect5info |
		       d:appendix|d:bibliography|d:chapter'>
    <xsl:apply-templates select='d:title|d:subtitle|d:titleabbrev'/>
    <xsl:apply-templates select='*[local-name() = concat(local-name(current()), "d:info")]'/>
    <xsl:apply-templates select='*[not(self::d:title|self::d:subtitle|self::d:titleabbrev) and
				 local-name() != concat(local-name(current()), "d:info")]'/>
  </xsl:template>

  <xsl:template match='d:title|d:subtitle|d:titleabbrev'>
    <sf:p>
      <xsl:attribute name='sf:style'>
        <xsl:choose>
          <xsl:when test='(parent::d:section or
                          parent::d:sectioninfo/parent::d:section) and
                          count(ancestor::d:section) > 5'>
            <xsl:message>section nested deeper than 5 levels</xsl:message>
	    <xsl:call-template name='lookup-paragraph-style'>
	      <xsl:with-param name='name'>
		<xsl:text>sect5-</xsl:text>
		<xsl:value-of select='name()'/>
	      </xsl:with-param>
	    </xsl:call-template>
          </xsl:when>
	  <xsl:when test='parent::d:sectioninfo'>
	    <xsl:call-template name='lookup-paragraph-style'>
	      <xsl:with-param name='name'>
		<xsl:text>sect</xsl:text>
		<xsl:value-of select='count(ancestor::d:section)'/>
		<xsl:text>-</xsl:text>
		<xsl:value-of select='name()'/>
	      </xsl:with-param>
	    </xsl:call-template>
	  </xsl:when>
          <xsl:when test='parent::d:sect1info |
			  parent::d:sect2info |
			  parent::d:sect3info |
			  parent::d:sect4info |
			  parent::d:sect5info |
			  parent::d:appendixinfo |
			  parent::d:bibliographyinfo |
			  parent::d:chapterinfo'>
	    <xsl:call-template name='lookup-paragraph-style'>
	      <xsl:with-param name='name'>
		<xsl:value-of select='substring-before(name(..), "d:info")'/>
		<xsl:text>-</xsl:text>
		<xsl:value-of select='name()'/>
	      </xsl:with-param>
	    </xsl:call-template>
	  </xsl:when>
	  <xsl:when test='parent::d:section'>
	    <xsl:call-template name='lookup-paragraph-style'>
	      <xsl:with-param name='name'>
		<xsl:text>sect</xsl:text>
		<xsl:value-of select='count(ancestor::d:section)'/>
		<xsl:text>-</xsl:text>
		<xsl:value-of select='name()'/>
	      </xsl:with-param>
	    </xsl:call-template>
	  </xsl:when>
          <xsl:when test='parent::d:sect1 |
			  parent::d:sect2 |
			  parent::d:sect3 |
			  parent::d:sect4 |
			  parent::d:sect5 |
			  parent::d:appendix |
			  parent::d:bibliography |
			  parent::d:bibliodiv |
			  parent::d:biblioentry |
			  parent::d:chapter |
			  parent::d:qandaset |
			  parent::d:qandadiv'>
	    <xsl:call-template name='lookup-paragraph-style'>
	      <xsl:with-param name='name'>
		<xsl:value-of select='name(..)'/>
		<xsl:text>-</xsl:text>
		<xsl:value-of select='name()'/>
	      </xsl:with-param>
	    </xsl:call-template>
          </xsl:when>
          <xsl:when test='parent::d:book|../parent::d:book |
			  parent::d:article|../parent::d:article |
			  parent::d:part|../parent::d:part|
			  parent::d:formalpara'>
	    <xsl:call-template name='lookup-paragraph-style'>
	      <xsl:with-param name='name'>
		<xsl:value-of select='name(ancestor::*[self::d:book|self::d:article|self::d:part|self::d:formalpara][1])'/>
		<xsl:text>-</xsl:text>
		<xsl:value-of select='name()'/>
	      </xsl:with-param>
	    </xsl:call-template>
          </xsl:when>
	  <xsl:when test='parent::d:objectinfo|parent::d:blockinfo'>
	    <xsl:call-template name='lookup-paragraph-style'>
	      <xsl:with-param name='name'>
		<xsl:value-of select='name(../..)'/>
		<xsl:text>-</xsl:text>
		<xsl:value-of select='name()'/>
	      </xsl:with-param>
	    </xsl:call-template>
	  </xsl:when>
          <xsl:otherwise>
	    <xsl:call-template name='lookup-paragraph-style'>
	      <xsl:with-param name='name'>Title</xsl:with-param>
	    </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
       </xsl:attribute>

       <xsl:apply-templates/>
       <sf:br/>
    </sf:p>
  </xsl:template>

  <doc:template name='metadata' xmlns=''>
    <title>Metadata</title>

    <para>TODO: Handle all metadata elements, apart from titles.</para>
  </doc:template>
  <xsl:template match='*[contains(name(), "d:info")]/*[not(self::d:title|self::d:subtitle|self::d:titleabbrev)]' priority='0'/>

  <xsl:template match='d:author|d:editor|d:othercredit'>
    <sf:p>
      <xsl:attribute name='sf:style'>
	<xsl:call-template name='lookup-paragraph-style'>
	  <xsl:with-param name='name'>
	    <xsl:value-of select='name()'/>
	  </xsl:with-param>
	</xsl:call-template>
      </xsl:attribute>

      <xsl:call-template name='attributes'/>

      <xsl:apply-templates select='d:personname|d:surname|d:firstname|d:honorific|d:lineage|d:othername|d:contrib'/>
      <sf:br/>
    </sf:p>
    <xsl:apply-templates select='d:affiliation|d:address'/>
    <xsl:apply-templates select='d:authorblurb|d:personblurb'/>
  </xsl:template>
  <xsl:template match='d:affiliation'>
    <sf:p>
      <xsl:attribute name='sf:style'>
	<xsl:call-template name='lookup-paragraph-style'>
	  <xsl:with-param name='name'>affiliation</xsl:with-param>
	</xsl:call-template>
      </xsl:attribute>

      <xsl:call-template name='attributes'/>

      <xsl:apply-templates/>
      <sf:br/>
    </sf:p>
  </xsl:template>
  <xsl:template match='d:address[parent::d:author|parent::d:editor|parent::d:othercredit|parent::d:publisher]'>
    <sf:p>
      <xsl:attribute name='sf:style'>
	<xsl:call-template name='lookup-paragraph-style'>
	  <xsl:with-param name='name'>para-continue</xsl:with-param>
	</xsl:call-template>
      </xsl:attribute>

      <xsl:call-template name='attributes'/>

      <xsl:apply-templates/>
      <sf:br/>
    </sf:p>
  </xsl:template>
  <!-- do not attempt to handle recursive structures -->
  <xsl:template match='d:address[not(parent::d:author|parent::d:editor|parent::d:othercredit|parent::d:publisher)]'>
    <sf:p>
      <xsl:attribute name='sf:style'>
	<xsl:call-template name='lookup-paragraph-style'>
	  <xsl:with-param name='name'>address</xsl:with-param>
	</xsl:call-template>
      </xsl:attribute>
      <xsl:apply-templates select='node()[not(self::d:affiliation|self::d:authorblurb)]'/>
      <sf:br/>
    </sf:p>
  </xsl:template>
  <!-- TODO -->
  <xsl:template match='d:authorblurb|d:personblurb'/>

  <xsl:template match='d:surname|d:firstname|d:honorific|d:lineage|d:othername|d:contrib|d:email|d:shortaffil|d:jobtitle|d:orgname|d:orgdiv|d:street|d:pob|d:postcode|d:city|d:state|d:country|d:phone|d:fax|d:citetitle'>
    <xsl:if test='preceding-sibling::*'>
      <xsl:text> </xsl:text>
    </xsl:if>
    <sf:span>
      <xsl:attribute name='sf:style'>
	<xsl:call-template name='lookup-character-style'>
	  <xsl:with-param name='name'>
	    <xsl:value-of select='name()'/>
	  </xsl:with-param>
	</xsl:call-template>
      </xsl:attribute>

      <xsl:apply-templates mode='text-run'/>
    </sf:span>
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
    <sf:link href='{$address}'>
      <sf:span>
	<xsl:attribute name='sf:style'>
	  <xsl:call-template name='lookup-character-style'>
	    <xsl:with-param name='name'>email</xsl:with-param>
	  </xsl:call-template>
	</xsl:attribute>
        <xsl:apply-templates mode='text-run'/>
      </sf:span>
    </sf:link>
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
                <sf:span>
		  <xsl:attribute name='sf:style'>
		    <xsl:call-template name='lookup-character-style'>
		      <xsl:with-param name='name'>otheraddr</xsl:with-param>
		    </xsl:call-template>
		  </xsl:attribute>
                  <xsl:apply-templates select='.' mode='text-run'/>
                </sf:span>
              </xsl:for-each>
            </xsl:when>
            <xsl:when test='preceding-sibling::node()'>
              <sf:span>
		<xsl:attribute name='sf:style'>
		  <xsl:call-template name='lookup-character-style'>
		    <xsl:with-param name='name'>otheraddr</xsl:with-param>
		  </xsl:call-template>
		</xsl:attribute>
                <xsl:apply-templates mode='text-run'/>
              </sf:span>
            </xsl:when>
          </xsl:choose>
          <xsl:apply-templates select='.'/>
        </xsl:for-each>
        <xsl:if test='d:ulink[last()]/following-sibling::node()'>
          <sf:span>
	    <xsl:attribute name='sf:style'>
	      <xsl:call-template name='lookup-character-style'>
		<xsl:with-param name='name'>otheraddr</xsl:with-param>
	      </xsl:call-template>
	    </xsl:attribute>
            <xsl:apply-templates select='d:ulink[last()]/following-sibling::node()' mode='text-run'/>
          </sf:span>
        </xsl:if>
      </xsl:when>
      <xsl:otherwise>
        <sf:span>
	  <xsl:attribute name='sf:style'>
	    <xsl:call-template name='lookup-character-style'>
	      <xsl:with-param name='name'>otheraddr</xsl:with-param>
	    </xsl:call-template>
	  </xsl:attribute>
          <xsl:apply-templates mode='text-run'/>
        </sf:span>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  <xsl:template match='d:ulink'>
    <sf:link href='{@url}'>
      <sf:span>
	<xsl:attribute name='sf:style'>
	  <xsl:call-template name='lookup-character-style'>
	    <xsl:with-param name='name'>ulink</xsl:with-param>
	  </xsl:call-template>
	</xsl:attribute>
      </sf:span>
      <xsl:apply-templates mode='text-run'/>
    </sf:link>
  </xsl:template>

  <xsl:template match='d:inlinegraphic'>
    <sf:span>
      <xsl:attribute name='sf:style'>
	<xsl:call-template name='lookup-character-style'>
	  <xsl:with-param name='name' select='name()'/>
	</xsl:call-template>
      </xsl:attribute>
      <xsl:value-of select='@fileref'/>
    </sf:span>
  </xsl:template>

  <!-- Cannot round-trip this element -->
  <xsl:template match='d:personname'>
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match='d:releaseinfo|d:bibliomisc|d:bibliorelation|d:publishername|d:isbn'>
    <sf:p>
      <xsl:attribute name='sf:style'>
	<xsl:call-template name='lookup-paragraph-style'>
	  <xsl:with-param name='name' select='name()'/>
	</xsl:call-template>
      </xsl:attribute>

      <xsl:call-template name='attributes'/>

      <xsl:apply-templates/>
      <sf:br/>
    </sf:p>
  </xsl:template>

  <xsl:template match='d:revhistory|d:biblioentry'>
    <xsl:apply-templates/>
  </xsl:template>
  <xsl:template match='d:revision'>
    <sf:p>
      <xsl:attribute name='sf:style'>
	<xsl:call-template name='lookup-paragraph-style'>
	  <xsl:with-param name='name'>revision</xsl:with-param>
	</xsl:call-template>
      </xsl:attribute>

      <xsl:call-template name='attributes'/>

      <!-- not currently supporting author -->
      <xsl:apply-templates select='d:revnumber|d:date|d:authorinitials'/>
      <sf:br/>
    </sf:p>
    <!-- not currently supporting revdescription -->
    <xsl:apply-templates select='d:revremark'/>
  </xsl:template>
  <xsl:template match='d:revnumber|d:date|d:authorinitials'>
    <xsl:if test='preceding-sibling::*'>
      <xsl:text> </xsl:text>
    </xsl:if>
    <sf:span>
      <xsl:attribute name='sf:style'>
	<xsl:call-template name='lookup-character-style'>
	  <xsl:with-param name='name'>
	    <xsl:value-of select='name()'/>
	  </xsl:with-param>
	</xsl:call-template>
      </xsl:attribute>

      <xsl:apply-templates/>
    </sf:span>
  </xsl:template>
  <xsl:template match='d:revremark'>
    <sf:p>
      <xsl:attribute name='sf:style'>
	<xsl:call-template name='lookup-paragraph-style'>
	  <xsl:with-param name='name'>revremark</xsl:with-param>
	</xsl:call-template>
      </xsl:attribute>

      <xsl:call-template name='attributes'/>

      <xsl:apply-templates/>
      <sf:br/>
    </sf:p>
  </xsl:template>

  <xsl:template match='d:abstract'>
    <xsl:apply-templates>
      <xsl:with-param name='class'>abstract</xsl:with-param>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template match='d:para'>
    <xsl:param name='class'/>

    <xsl:variable name='block' select='d:blockquote|d:calloutlist|d:classsynopsis|d:funcsynopsis|d:figure|d:glosslist|d:graphic|d:informalfigure|d:informaltable|d:itemizedlist|d:literallayout|d:mediaobject|d:mediaobjectco|d:note|d:caution|d:warning|d:important|d:tip|d:orderedlist|d:programlisting|d:programlistingco|d:revhistory|d:segmentedlist|d:simplelist|d:table|d:variablelist'/>

    <xsl:choose>
      <xsl:when test='$block'>
        <sf:p>
	  <xsl:call-template name='para-style'>
	    <xsl:with-param name='class' select='$class'/>
	  </xsl:call-template>

          <xsl:call-template name='attributes'/>

          <xsl:apply-templates select='$block[1]/preceding-sibling::node()'/>
          <sf:br/>
        </sf:p>
        <xsl:for-each select='$block'>
          <xsl:apply-templates select='.'/>
          <sf:p>
	    <xsl:call-template name='para-style'>
	      <xsl:with-param name='class' select='$class'/>
	    </xsl:call-template>
            <xsl:apply-templates select='following-sibling::node()[generate-id(preceding-sibling::*[self::d:blockquote|self::d:calloutlist|self::d:figure|self::d:glosslist|self::d:graphic|self::d:informalfigure|self::d:informaltable|self::d:itemizedlist|self::d:literallayout|self::d:mediaobject|self::d:mediaobjectco|self::d:note|self::d:caution|self::d:warning|self::d:important|self::d:tip|self::d:orderedlist|self::d:programlisting|self::d:programlistingco|self::d:revhistory|self::d:segmentedlist|self::d:simplelist|self::d:table|self::d:variablelist][1]) = generate-id(current())]'/>
            <sf:br/>
          </sf:p>
        </xsl:for-each>
      </xsl:when>
      <xsl:otherwise>
        <sf:p>
	  <xsl:call-template name='para-style'>
	    <xsl:with-param name='class' select='$class'/>
	  </xsl:call-template>

          <xsl:call-template name='attributes'/>

          <xsl:apply-templates/>
          <sf:br/>
        </sf:p>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  <xsl:template match='d:simpara'>
    <xsl:param name='class'/>

    <sf:p>
      <xsl:attribute name='sf:style'>
	<xsl:call-template name='lookup-paragraph-style'>
	  <xsl:with-param name='name'>simpara</xsl:with-param>
	</xsl:call-template>
      </xsl:attribute>

      <xsl:call-template name='attributes'/>

      <xsl:apply-templates/>
      <sf:br/>
    </sf:p>
  </xsl:template>
  <xsl:template name='para-style'>
    <xsl:param name='class'/>

    <xsl:variable name='style' select='$paragraph-styles[@sf:name = $class]'/>

    <xsl:attribute name='sf:style'>
      <xsl:choose>
	<xsl:when test='$style'>
	  <xsl:value-of select='$style/@sf:ident'/>
	</xsl:when>
	<xsl:otherwise>
	  <xsl:call-template name='lookup-paragraph-style'>
	    <xsl:with-param name='name'>para</xsl:with-param>
	  </xsl:call-template>
	</xsl:otherwise>
      </xsl:choose>
    </xsl:attribute>
  </xsl:template>

  <xsl:template match='d:emphasis'>
    <sf:span>
      <xsl:attribute name='sf:style'>
	<xsl:choose>
          <xsl:when test='@role = "d:underline"'>
            <xsl:call-template name='lookup-character-style'>
	      <xsl:with-param name='name'>emphasis-underline</xsl:with-param>
	    </xsl:call-template>
          </xsl:when>
          <xsl:when test='@role = "bold" or @role = "d:strong"'>
            <xsl:call-template name='lookup-character-style'>
	      <xsl:with-param name='name'>emphasis-bold</xsl:with-param>
	    </xsl:call-template>
          </xsl:when>
          <xsl:when test='not(@role) or @role="italic"'>
            <xsl:call-template name='lookup-character-style'>
	      <xsl:with-param name='name'>emphasis</xsl:with-param>
	    </xsl:call-template>
          </xsl:when>
	</xsl:choose>
      </xsl:attribute>

      <xsl:apply-templates/>
    </sf:span>
  </xsl:template>

  <xsl:template match='d:filename|d:sgmltag|d:application|d:literal'>
    <sf:span>
      <xsl:attribute name='sf:style'>
        <xsl:call-template name='lookup-character-style'>
	  <xsl:with-param name='name'>
	    <xsl:value-of select='name()'/>
	  </xsl:with-param>
	</xsl:call-template>
      </xsl:attribute>

      <xsl:apply-templates/>
    </sf:span>
  </xsl:template>

  <xsl:template match='d:example|d:figure'>
    <sf:p>
      <xsl:attribute name='sf:style'>
	<xsl:call-template name='lookup-paragraph-style'>
	  <xsl:with-param name='name'>
	    <xsl:value-of select='name()'/>
	    <xsl:text>-title</xsl:text>
	  </xsl:with-param>
	</xsl:call-template>
      </xsl:attribute>

      <xsl:apply-templates select='d:title' mode='textonly'/>
      <sf:br/>
    </sf:p>
    <xsl:apply-templates select='*[not(self::d:title)][1]'/>
    <xsl:apply-templates select='*[not(self::d:title)][position() != 1]'
			 mode='error'/>
  </xsl:template>

  <xsl:template match='d:informalfigure'>
    <xsl:if test='d:mediaobject/d:imageobject/d:imagedata'>
      <sf:p>
	<xsl:attribute name='sf:style'>
	  <xsl:call-template name='lookup-paragraph-style'>
	    <xsl:with-param name='name'>informalfigure-imagedata</xsl:with-param>
	  </xsl:call-template>
	</xsl:attribute>

	<xsl:apply-templates select='d:mediaobject/d:imageobject/d:imagedata/@fileref'
			     mode='textonly'/>
	<sf:br/>
      </sf:p>
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
	<sf:p>
	  <xsl:attribute name='sf:style'>
	    <xsl:call-template name='lookup-paragraph-style'>
	      <xsl:with-param name='name'>
		<xsl:value-of select='name()'/>
		<xsl:text>-</xsl:text>
		<xsl:value-of select='name(d:imagedata|d:audiodata|d:videodata)'/>
	      </xsl:with-param>
	    </xsl:call-template>
	  </xsl:attribute>

	  <xsl:apply-templates select='*/@fileref'
			       mode='textonly'/>
	  <sf:br/>
	</sf:p>
      </xsl:when>
      <xsl:when test='self::d:imageobjectco/d:imageobject/d:imagedata'>
	<sf:p>
	  <xsl:attribute name='sf:style'>
	    <xsl:call-template name='lookup-paragraph-style'>
	      <xsl:with-param name='name'>
		<xsl:value-of select='name()'/>
		<xsl:text>-imagedata</xsl:text>
	      </xsl:with-param>
	    </xsl:call-template>
	  </xsl:attribute>

	  <xsl:apply-templates select='d:imageobject/d:imagedata/@fileref'
			       mode='textonly'/>
	  <sf:br/>
	</sf:p>
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
	<sf:p>
	  <xsl:attribute name='sf:style'>
	    <xsl:call-template name='lookup-paragraph-style'>
	      <xsl:with-param name='name'>textobject-title</xsl:with-param>
	    </xsl:call-template>
	  </xsl:attribute>

	  <xsl:text>Text Object </xsl:text>
	  <xsl:number level='any'/>
	  <sf:br/>
	</sf:p>
      </xsl:otherwise>
    </xsl:choose>

    <xsl:apply-templates select='*[not(self::d:objectinfo)]'/>
  </xsl:template>

  <xsl:template match='d:caption'>
    <sf:p>
      <xsl:attribute name='sf:style'>
	<xsl:call-template name='lookup-paragraph-style'>
	  <xsl:with-param name='name'>caption</xsl:with-param>
	</xsl:call-template>
      </xsl:attribute>

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
      <sf:br/>
    </sf:p>
  </xsl:template>

  <xsl:template match='d:qandaset|d:qandadiv'>
    <xsl:apply-templates/>
  </xsl:template>
  <xsl:template match='d:qandaentry'>
    <xsl:for-each select='d:revhistory'>
      <xsl:call-template name='nomatch'/>
    </xsl:for-each>
    <xsl:apply-templates select='*[not(self::d:revhistory)]'/>
  </xsl:template>
  <xsl:template match='d:question|d:answer'>
    <xsl:choose>
      <xsl:when test='*[1][self::d:para]'>
	<sf:p>
	  <xsl:attribute name='sf:style'>
	    <xsl:call-template name='lookup-paragraph-style'>
	      <xsl:with-param name='name'>
		<xsl:value-of select='name()'/>
	      </xsl:with-param>
	    </xsl:call-template>
	  </xsl:attribute>

	  <xsl:apply-templates select='*[1]/node()'/>
	  <sf:br/>
	</sf:p>

	<xsl:apply-templates select='*[position() != 1]' mode='question'/>
      </xsl:when>
      <xsl:otherwise>
	<xsl:message>first element in a question must be a paragraph</xsl:message>
	<xsl:apply-templates mode='error'/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  <xsl:template match='d:para' mode='question'>
    <xsl:apply-templates select='.'>
      <xsl:with-param name='class' select='"para-continue"'/>
    </xsl:apply-templates>
  </xsl:template>
  <xsl:template match='*' mode='question'>
    <xsl:apply-templates select='.'/>
  </xsl:template>

  <xsl:template match='d:table|d:informaltable' mode='not-yet-implemented'>

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

  <xsl:template match='text()[string-length(normalize-space(.)) = 0]'/>
  <xsl:template match='text()' mode='text-run'>
    <xsl:value-of select='.'/>
  </xsl:template>
  <xsl:template match='d:literallayout/text()|d:programlisting/text()'>
    <xsl:call-template name='handle-linebreaks'/>
  </xsl:template>
  <xsl:template name='handle-linebreaks'>
    <xsl:param name='text' select='.'/>

    <xsl:choose>
      <xsl:when test='not($text)'/>
      <xsl:when test='contains($text, "&#d:xa;")'>
        <xsl:value-of select='substring-before($text, "&#d:xa;")'/>
        <xsl:call-template name='handle-linebreaks-aux'>
          <xsl:with-param name='text'
            select='substring-after($text, "&#d:xa;")'/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select='$text'/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- pre-condition: leading linefeed has been stripped -->
  <xsl:template name='handle-linebreaks-aux'>
    <xsl:param name='text'/>

    <xsl:choose>
      <xsl:when test='contains($text, "&#d:xa;")'>
        <sf:lnbr/>
        <xsl:value-of select='substring-before($text, "&#d:xa;")'/>
        <xsl:call-template name='handle-linebreaks-aux'>
          <xsl:with-param name='text' select='substring-after($text, "&#d:xa;")'/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <sf:lnbr/>
        <xsl:value-of select='$text'/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match='d:authorblurb|d:formalpara|d:highlights|d:legalnotice|d:note|d:caution|d:warning|d:important|d:tip'>
    <xsl:apply-templates select='*'>
      <xsl:with-param name='class'>
        <xsl:if test='parent::d:highlights'>
	  <xsl:value-of select='name(..)'/>
	  <xsl:text>-</xsl:text>
	</xsl:if>
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

  <xsl:template match='d:programlistingco'>
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match='d:literallayout|d:programlisting'>
    <xsl:param name='class'/>

    <sf:p>
      <xsl:attribute name='sf:style'>
	<xsl:call-template name='lookup-paragraph-style'>
	  <xsl:with-param name='name'>
	    <xsl:if test='$class != ""'>
	      <xsl:value-of select='$class'/>
	      <xsl:text>-</xsl:text>
	    </xsl:if>
	    <xsl:choose>
	      <xsl:when test='self::d:programlisting/parent::d:programlistingco'>
		<xsl:value-of select='name(..)'/>
	      </xsl:when>
	      <xsl:otherwise>
		<xsl:value-of select='name()'/>
	      </xsl:otherwise>
	    </xsl:choose>
	  </xsl:with-param>
	</xsl:call-template>
      </xsl:attribute>

      <xsl:apply-templates/>
      <sf:br/>
    </sf:p>
  </xsl:template>

  <xsl:template match='d:areaspec'>
    <sf:p>
      <xsl:attribute name='sf:style'>
	<xsl:call-template name='lookup-paragraph-style'>
	  <xsl:with-param name='name' select='name()'/>
	</xsl:call-template>
      </xsl:attribute>

      <xsl:call-template name='attributes'/>

      <sf:br/>
    </sf:p>
    <xsl:apply-templates/>
  </xsl:template>
  <xsl:template match='d:area'>
    <sf:p>
      <xsl:attribute name='sf:style'>
	<xsl:call-template name='lookup-paragraph-style'>
	  <xsl:with-param name='name' select='name()'/>
	</xsl:call-template>
      </xsl:attribute>

      <xsl:call-template name='attributes'/>

      <xsl:apply-templates/>
      <sf:br/>
    </sf:p>
  </xsl:template>

  <xsl:template match='d:calloutlist'>
    <xsl:apply-templates select='d:callout'/>
  </xsl:template>

  <xsl:template match='d:callout'>
    <sf:p>
      <xsl:attribute name='sf:style'>
	<xsl:call-template name='lookup-paragraph-style'>
	  <xsl:with-param name='name'>
	    <xsl:value-of select='name()'/>
	  </xsl:with-param>
	</xsl:call-template>
      </xsl:attribute>

      <xsl:call-template name='attributes'/>

      <!-- Normally a para would be the first child of a callout -->
      <xsl:apply-templates select='*[1][self::d:para]/node()' mode='list'/>
      <sf:br/>
    </sf:p>
    <!-- This is to catch the case where a listitem's first child is not a paragraph.
       - We may not be able to represent this properly.
      -->
    <xsl:apply-templates select='*[1][not(self::d:para)]' mode='list'/>

    <xsl:apply-templates select='*[position() != 1]' mode='list'/>
  </xsl:template>

  <xsl:template match='d:itemizedlist|d:orderedlist'>
    <xsl:apply-templates select='d:listitem'/>
  </xsl:template>

  <xsl:template match='d:listitem'>
    <sf:p>
      <xsl:attribute name='sf:style'>
	<xsl:choose>
	  <xsl:when test='../parent::d:highlights'>
	    <xsl:call-template name='lookup-paragraph-style'>
	      <xsl:with-param name='name'>
		<xsl:text>highlights-</xsl:text>
		<xsl:value-of select='name(..)'/>
	      </xsl:with-param>
	    </xsl:call-template>
	  </xsl:when>
	  <xsl:otherwise>
	    <xsl:call-template name='lookup-paragraph-style'>
	      <xsl:with-param name='name'>
		<xsl:value-of select='name(..)'/>
		<xsl:value-of select='count(ancestor::d:itemizedlist|ancestor::d:orderedlist|ancestor::d:calloutlist)'/>
	      </xsl:with-param>
	    </xsl:call-template>
	  </xsl:otherwise>
	</xsl:choose>
      </xsl:attribute>

      <!-- Normally a para would be the first child of a listitem -->
      <xsl:apply-templates select='*[1][self::d:para]/node()' mode='list'/>
      <sf:br/>
    </sf:p>
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

  <xsl:template match='d:bridgehead'>
    <sf:p>
      <xsl:attribute name='sf:style'>
	<xsl:call-template name='lookup-paragraph-style'>
	  <xsl:with-param name='name' select='name()'/>
	</xsl:call-template>
      </xsl:attribute>

      <xsl:call-template name='attributes'/>

      <xsl:apply-templates/>
      <sf:br/>
    </sf:p>
  </xsl:template>

  <xsl:template match='xi:include'>
    <sf:p>
      <xsl:attribute name='sf:style'>
	<xsl:call-template name='lookup-paragraph-style'>
	  <xsl:with-param name='name'>xinclude</xsl:with-param>
	</xsl:call-template>
      </xsl:attribute>

      <xsl:value-of select='@href'/>
      <sf:br/>
    </sf:p>
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

  <xsl:template match='*' mode='error'>
    <sf:p>
      <xsl:attribute name='sf:style'>
	<xsl:call-template name='lookup-paragraph-style'>
	  <xsl:with-param name='name'>blockerror</xsl:with-param>
	</xsl:call-template>
      </xsl:attribute>

      <xsl:value-of select='name()'/>
      <xsl:text> encountered</xsl:text>
      <xsl:if test='parent::*'>
        <xsl:text> in </xsl:text>
        <xsl:value-of select='name(parent::*)'/>
      </xsl:if>
      <xsl:text> cannot be supported.</xsl:text>
      <sf:br/>
    </sf:p>
  </xsl:template>
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
                      self::d:chapter |
                      self::d:classsynopsis |
                      self::d:colophon |
                      self::d:constraintdef |
                      self::d:copyright |
                      self::d:dedication |
                      self::d:epigraph |
                      self::d:equation |
                      self::d:funcsynopsis |
                      self::d:glossary |
                      self::d:glossdef |
                      self::d:glossdiv |
                      self::d:glossentry |
                      self::d:glosslist |
                      self::d:graphic |
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
        <sf:p>
	  <xsl:attribute name='sf:style'>
	    <xsl:call-template name='lookup-paragraph-style'>
	      <xsl:with-param name='name'>blockerror</xsl:with-param>
	    </xsl:call-template>
	  </xsl:attribute>

          <xsl:value-of select='name()'/>
          <xsl:text> encountered</xsl:text>
          <xsl:if test='parent::*'>
            <xsl:text> in </xsl:text>
            <xsl:value-of select='name(parent::*)'/>
          </xsl:if>
          <xsl:text>, but no template matches.</xsl:text>
	  <sf:br/>
        </sf:p>
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
        <sf:span>
	  <xsl:attribute name='sf:style'>
	    <xsl:call-template name='lookup-character-style'>
	      <xsl:with-param name='name'>inlineerror</xsl:with-param>
	    </xsl:call-template>
	  </xsl:attribute>

          <xsl:value-of select='name()'/>
          <xsl:text> encountered</xsl:text>
          <xsl:if test='parent::*'>
            <xsl:text> in </xsl:text>
            <xsl:value-of select='name(parent::*)'/>
          </xsl:if>
          <xsl:text>, but no template matches.</xsl:text>
        </sf:span>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name='attributes'>
    <xsl:param name='node' select='.'/>

    <xsl:for-each select='$node/@*'>
      <sf:span>
	<xsl:attribute name='sf:style'>
	  <xsl:call-template name='lookup-character-style'>
	    <xsl:with-param name='name'>attribute-name</xsl:with-param>
	  </xsl:call-template>
	</xsl:attribute>
	<xsl:value-of select='name()'/>
      </sf:span>
      <sf:span>
	<xsl:attribute name='sf:style'>
	  <xsl:call-template name='lookup-character-style'>
	    <xsl:with-param name='name'>attribute-value</xsl:with-param>
	  </xsl:call-template>
	</xsl:attribute>
	<xsl:value-of select='.'/>
      </sf:span>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name='lookup-paragraph-style'>
    <xsl:param name='name'/>

    <xsl:if test='not($paragraph-styles[@sf:name = $name])'>
      <xsl:message>unable to find character style "<xsl:value-of select='$name'/>"</xsl:message>
    </xsl:if>

    <xsl:value-of select='$paragraph-styles[@sf:name = $name]/@sf:ident'/>
  </xsl:template>
  <xsl:template name='lookup-character-style'>
    <xsl:param name='name'/>

    <xsl:if test='not($character-styles[@sf:name = $name])'>
      <xsl:message>unable to find character style "<xsl:value-of select='$name'/>"</xsl:message>
    </xsl:if>

    <xsl:value-of select='$character-styles[@sf:name = $name]/@sf:ident'/>
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
