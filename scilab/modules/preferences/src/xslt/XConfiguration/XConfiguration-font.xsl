<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version ="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                >
  <!-- = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
       ::
       ::     M A I N   P A N E L   :   F O N T S
       ::
  -->

  <xsl:variable name="font-family">
    <!-- TODO transmit font family list to XSL -->
    <option font-family='Bitstream Charter'/>
    <option font-family='Century Schoolbook L'/>
    <option font-family='Courier 10 Pitch'/>
    <option font-family='DejaVu Sans'/>
    <option font-family='DejaVu Sans Condensed'/>
    <option font-family='DejaVu Sans Light'/>
    <option font-family='DejaVu Sans Mono'/>
    <option font-family='DejaVu Serif'/>
    <option font-family='DejaVu Serif Condensed'/>
    <option font-family='Dialog'/>
    <option font-family='DialogInput'/>
    <option font-family='Dingbats'/>
    <option font-family='FreeMono'/>
    <option font-family='FreeSans'/>
    <option font-family='FreeSerif'/>
    <option font-family='gargi'/>
    <option font-family='Garuda'/>
    <option font-family='KacstOne'/>
    <option font-family='Kedage'/>
    <option font-family='Khmer OS'/>
    <option font-family='Khmer OS System'/>
    <option font-family='Kinnari'/>
    <option font-family='Liberation Mono'/>
    <option font-family='Liberation Sans'/>
    <option font-family='Liberation Serif'/>
    <option font-family='Lohit Bengali'/>
    <option font-family='Lohit Gujarati'/>
    <option font-family='Lohit Hindi'/>
    <option font-family='Lohit Punjabi'/>
    <option font-family='Lohit Tamil'/>
    <option font-family='Loma'/>
    <option font-family='Mallige'/>
    <option font-family='Meera'/>
    <option font-family='Monospaced'/>
    <option font-family='Mukti Narrow'/>
    <option font-family='Nimbus Mono L'/>
    <option font-family='Nimbus Roman No9 L'/>
    <option font-family='Nimbus Sans L'/>
    <option font-family='Norasi'/>
    <option font-family='OpenSymbol'/>
    <option font-family='Phetsarath OT'/>
    <option font-family='Pothana2000'/>
    <option font-family='Purisa'/>
    <option font-family='Rachana'/>
    <option font-family='Rekha'/>
    <option font-family='Saab'/>
    <option font-family='SansSerif'/>
    <option font-family='Sawasdee'/>
    <option font-family='Serif'/>
    <option font-family='Standard Symbols L'/>
    <option font-family='Symbol'/>
    <option font-family='TakaoPGothic'/>
    <option font-family='Tlwg Typist'/>
    <option font-family='Tlwg Typo'/>
    <option font-family='TlwgMono'/>
    <option font-family='TlwgTypewriter'/>
    <option font-family='Ubuntu'/>
    <option font-family='Ubuntu Light'/>
    <option font-family='Umpush'/>
    <option font-family='UnBatang'/>
    <option font-family='UnDotum'/>
    <option font-family='URW Bookman L'/>
    <option font-family='URW Chancery L'/>
    <option font-family='URW Gothic L'/>
    <option font-family='URW Palladio L'/>
    <option font-family='utkal'/>
    <option font-family='Vemana2000'/>
    <option font-family='Waree'/>
    <option font-family='WenQuanYi Micro Hei'/>
    <option font-family='WenQuanYi Micro Hei Mono'/>
  </xsl:variable>

  <xsl:variable name="font-face">
    <option font-face="plain"/>
    <option font-face="bold"/>
    <option font-face="italic"/>
    <option font-face="bold italic"/>
  </xsl:variable>

  <xsl:variable name="font-size">
    <option font-size="6"/>
    <option font-size="7"/>
    <option font-size="8"/>
    <option font-size="9"/>
    <option font-size="10"/>
    <option font-size="11"/>
    <option font-size="12"/>
    <option font-size="14"/>
    <option font-size="16"/>
    <option font-size="18"/>
    <option font-size="20"/>
    <option font-size="24"/>
    <option font-size="28"/>
    <option font-size="32"/>
  </xsl:variable>

  <xsl:template match="fonts">
    <xsl:variable name="core-desktop-font">
      <xsl:choose>
        <xsl:when test="@system='true'">
          <font font-name="Monospaced"    font-face="plain"   font-size="11"/>
        </xsl:when>
        <xsl:otherwise>
          <font font-name="{@font-name}" font-face="{@font-face}" font-size="{@font-size}"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="desktop-font" select="nodeset($core-desktop-font)/font"/>

    <Title text="Desktop font">
      <VBox>
        <HBox>
          <xsl:call-template name="BooleanCheckBox">
            <xsl:with-param name="text" select="'Use system font'"/>
            <xsl:with-param name="attr-name" select="'system'"/>
          </xsl:call-template>
          <Glue/>
        </HBox>
        <HBox>
          Desktop text font:
          <FontSelector font-name="{@font-name}"
			font-face="{@font-face}"
			font-size="{@font-size}"
			listener = "ActionListener" >
            <actionPerformed choose="font-name, font-face, font-size">
              <xsl:call-template name="context"/>
            </actionPerformed>
          </FontSelector>
	  <Panel/>
        </HBox>
        <HBox>
          Sample:
          <Glue/>
        </HBox>
        <Panel>
          <Label
              text="Puffins are cool!"
              font-family="{$desktop-font/@font-family}"
              font-face  ="{$desktop-font/@font-face}"
              font-size  ="{$desktop-font/@font-size}"
              />
        </Panel>
      </VBox>
    </Title>
    <Title text="Custom font">
      <HBox>
        <Table item="{@item}" mode="select" listener="TableListener">
          <tableSelect choose="item">
            <xsl:call-template name="context"/>
          </tableSelect>
          <tableCol title="Name"        attr="name"/>
          <xsl:for-each select="item">
            <tableRow name="{@name}"/>
          </xsl:for-each>
        </Table>
        <HSpace width="30"/>
        <xsl:for-each select="item[ position()=current()/@item]">
          <VBox>
            <HBox>&lt;HTML&gt;&lt;u&gt;Fonts to use&lt;/u&gt;:
            <Glue/>
            </HBox>
            <HBox>
              <xsl:call-template name="BooleanRadio">
                <xsl:with-param name="text" select="'Desktop text font'"/>
                <xsl:with-param name="attr-name" select="'desktop'"/>
              </xsl:call-template>
              <Glue/>
            </HBox>
            <HBox>
              <xsl:call-template name="InvertedRadio">
                <xsl:with-param name="text" select="'Custom:'"/>
                <xsl:with-param name="attr-name" select="'desktop'"/>
              </xsl:call-template>
              <Glue/>
            </HBox>
            <xsl:choose>
              <xsl:when test="@desktop='true'">
                <xsl:for-each select="$desktop-font">
                  <xsl:call-template name="Select">
                    <xsl:with-param name="enable" select="'false'"/>
                    <xsl:with-param name="among" select="$font-family"/>
                  </xsl:call-template>
                  <xsl:call-template name="Select">
                    <xsl:with-param name="enable" select="'false'"/>
                    <xsl:with-param name="among" select="$font-size"/>
                  </xsl:call-template>
                  <xsl:call-template name="Select">
                    <xsl:with-param name="enable" select="'false'"/>
                    <xsl:with-param name="among" select="$font-face"/>
                  </xsl:call-template>
                </xsl:for-each>
              </xsl:when>
              <xsl:otherwise>
                <xsl:call-template name="Select">
                  <xsl:with-param name="among" select="$font-family"/>
                </xsl:call-template>
                <xsl:call-template name="Select">
                  <xsl:with-param name="among" select="$font-size"/>
                </xsl:call-template>
                <xsl:call-template name="Select">
                  <xsl:with-param name="among" select="$font-face"/>
                </xsl:call-template>
              </xsl:otherwise>
            </xsl:choose>
            <HBox>
              Sample
              <Glue/>
            </HBox>
            <Panel>
              <xsl:choose>
                <xsl:when test="@desktop='true'">
                  <Label
                      text="Puffins are cool!"
                      font-family="{$desktop-font/@font-family}"
                      font-face  ="{$desktop-font/@font-face}"
                      font-size  ="{$desktop-font/@font-size}"
                      />
                </xsl:when>
                <xsl:otherwise>
                  <Label
                      text="Puffins are so cool!"
                      font-family="{@font-family}"
                      font-face  ="{@font-face}"
                      font-size  ="{@font-size}"
                      />
                </xsl:otherwise>
              </xsl:choose>
            </Panel>
          </VBox>
        </xsl:for-each>
      </HBox>
    </Title>
  </xsl:template>
</xsl:stylesheet>

