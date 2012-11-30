�x����y�3U[9������>h�����=~7��:�t�������������0�w`��]�%��v}�>E��Oӧ����>^����g��N�Z�=����f�f�f�f�������������No4ÌF��h6Z�V��n�0F����c�1�oL0&��vc�1ŘjL3�3���,�Øm�1����c���Xl,1�ˌ��
c����4V]�c���Xol0��c��kl26[�>��0��hl3���c����m�1������q�x���ϲöӶ׶���MS��h2��&��j���M�HS�)�c�5ř�M	�DS��nJ6��RMi�tS�)Ӕer��M9�\S�)�T`*4��M%�RS���Ta�4U���j��Tc�5ՙ�M&��cj4yMM�fS��g��V�`Mm�����a�4u��M=�^S���4`4�M�M#��?�v�.���IX�E�bk���Zf-�VX+�UV�����Xk�u�zk��m�X�^k����b�Y�ր��*XEk�5hm�vX;�G�����vC����e�6�z}��ݿ����O�?����=�}�uۖؖ>>��C��W�p�\e��}+m�/W����;|��t��>��� f�������⪒�*�;=��f6at�K�CX\Ewv�\b��a\��61��(���!�nn_Hd�TU/"L%Q���HJVu��_B,%�ˈ���'g��^ItT��+G�W#ٞ��jb��(�_G�e�'�܉ym�Dt}c���~�9�>{#�Z�*(*�DԦn&�\�-�V��.�*�w�V�WmC��b'�Y���Md��{���){��\W�A�����G����;N�@�)"��4q�8K�|���D���7�����KDn�]�n�u\&
kM�9���,9}V��(��9�I(�Lw�g�"͡$�3Mi~�x�?͞�"z�lYƊ���	�h(��İGMp������N��!�������w���=�YQ8�p��vfvf^#j�k�o�ܷ��xgCY�ò�:{JItand and accept it fully.                                        */
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
