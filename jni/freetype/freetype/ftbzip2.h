éxº“˛”yÙ3U[9´∆ ˇ”ı>hÏáÔÎ◊˜=~7ÌÛ:˛t§Ô¯èûÚÉŒ˛ÔÔ”Î˙0Ωw`”Ô±]∑%ÍìÙv}≤>Eü™O”ßÎ≥Ù˝Îî>^ü†œ–gæÁNØZØ=˛§¶€fÿf⁄fŸf€ÊÿÊ⁄ÊŸÊø˚›˝œÓNo4√åF£…h6ZåV£Õnå0F£å—∆c¨1ŒoL0&ìåvc≤1≈òjL3¶3åô∆,£√òmÃ1ÊÛå˘∆c°±»Xl,1ñÀåÂ∆
c•± Ë4V]∆c≠±ŒXol0∫çc£—kl26[å>£ﬂ0∂£hl3çÌ∆cß±ÀÿmÏ1ˆ˚å˝∆„†q»xœ¯èœ≤√∂”∂◊∂ÔÒìÍMSò…h2ôÃ&ã…j≤ô¬M¶HSî)⁄cä5≈ô‚M	¶DSí…nJ6•òRMi¶tSÜ)”îerò≤M9¶\Sû)ﬂT`*4ôäM%¶RSô©‹Ta™4Uôú¶jìÀTc™5’ôÍM&∑…cj4yMM¶fSã…gÚõ¶Vì`Mm¶†©›‘aÍ4uô∫M=¶^Sü©ﬂ4`4ôÓôÜM˜M#¶ø?≈v€.€Ó¯IX≠E÷bkâµ‘Zf-∑VX+≠UVßµ⁄Í≤÷Xk≠u÷zkÉ’mıX≠^kìµŸ⁄bıY˝÷Äµ’*XEkõ5hm∑vX;ˇGˇ˛¥ÇÜvCá°”–eË6Ùz}Ô˚›øÒû‚ÔO¯?π∂‡Â=⁄}Ëu€ñÿñ>>∫ÏC«¸Wûp˘\e≈„}+m´/W¿àˇ ;|ÿtóﬁ>ùŒ f≥à°äí∫…‚™í‹*É;=æªf6atﬂKòCX\EwvÈ\b†Ëa\í´61¬Ó(≥πÁ!Ÿnn_HdÊTU/"L%QÓ≈ƒHJVuì‡¨_B,%…ÀàËƒÂÑ'gë·^ItTì¬+GÎW#Ÿû¨˜jb±ñ(À_G‰e≠'‚‹âymÆDt}c••Ó~ö9π>{#ëZË*(*⁄D‘¶n& \Æ-ƒV¬·.»*åw≈VµWmC¢ﬁb'—Y∫ãÿMdª˜{ë¨∑){à°\WÓA¢≥¯“ˆGà£»ﬂ;Nú@ﬂ)"≤Ë4qÜ8K†|ú∏ÄDæ≤ú7àéö±∫KDnÈî]Ôn¨u\&
kMÈ9Ó∂ Ë,9}V∑å(™∂9ÂI(àLwøg§"Õ°$“3Mi~◊xº?ÕûØ"z´lY∆äúä†	Üh(Ù∞ƒ∞GMpÑÜ‡â≤‚N∑ñ!ÆçÆ¯““‚wóª–=íYQ8êpïËvfvf^#jãkØoÓ‹∑â˙xgCYÈ√≤Ú:{JItand and accept it fully.                                        */
/*                                                                         */
/***************************************************************************/


#ifndef __FTBZIP2_H__
#define __FTBZIP2_H__

#include <ft2build.h>
#include FT_FREETYPE_H

#ifdef FREETYPE_H
#error "freetype.h of FreeType 1 has been loaded!"
#error "Please fix the directory search order for header files"
#error "so that freetype.h of FreeType 2 is found first."
#endif


FT_BEGIN_HEADER

  /*************************************************************************/
  /*                                                                       */
  /* <Section>                                                             */
  /*    bzip2                                                              */
  /*                                                                       */
  /* <Title>                                                               */
  /*    BZIP2 Streams                                                      */
  /*                                                                       */
  /* <Abstract>                                                            */
  /*    Using bzip2-compressed font files.                                 */
  /*                                                                       */
  /* <Description>                                                         */
  /*    This section contains the declaration of Bzip2-specific functions. */
  /*                                                                       */
  /*************************************************************************/


 /************************************************************************
  *
  * @function:
  *   FT_Stream_OpenBzip2
  *
  * @description:
  *   Open a new stream to parse bzip2-compressed font files.  This is
  *   mainly used to support the compressed `*.pcf.bz2' fonts that come
  *   with XFree86.
  *
  * @input:
  *   stream ::
  *     The target embedding stream.
  *
  *   source ::
  *     The source stream.
  *
  * @return:
  *   FreeType error code.  0~means success.
  *
  * @note:
  *   The source stream must be opened _before_ calling this function.
  *
  *   Calling the internal function `FT_Stream_Close' on the new stream will
  *   *not* call `FT_Stream_Close' on the source stream.  None of the stream
  *   objects will be released to the heap.
  *
  *   The stream implementation is very basic and resets the decompression
  *   process each time seeking backwards is needed within the stream.
  *
  *   In certain builds of the library, bzip2 compression recognition is
  *   automatically handled when calling @FT_New_Face or @FT_Open_Face.
  *   This means that if no font driver is capable of handling the raw
  *   compressed file, the library will try to open a bzip2 compressed stream
  *   from it and re-open the face with it.
  *
  *   This function may return `FT_Err_Unimplemented_Feature' if your build
  *   of FreeType was not compiled with bzip2 support.
  */
  FT_EXPORT( FT_Error )
  FT_Stream_OpenBzip2( FT_Stream  stream,
                       FT_Stream  source );

 /* */


FT_END_HEADER

#endif /* __FTBZIP2_H__ */


/* END */
