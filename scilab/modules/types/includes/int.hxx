/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __INT_HH__
#define __INT_HH__

#include "types.hxx"

namespace types
{
	class EXTERN_TYPES Int : public GenericType
	{
  public :
    enum IntType {
      Type8 = 1,
      TypeUnsigned8 = 11,
      Type16 = 2,
      TypeUnsigned16 = 12,
      Type32 = 4,
      TypeUnsigned32 = 14,
      Type64 = 8,
      TypeUnsigned64 = 18
		};

    Int(char _cData);
    Int(unsigned char _ucData);
    Int(short _sData);
    Int(unsigned short _usData);
    Int(int _iData);
    Int(unsigned int _uiData);
    Int(long long _llData);
    Int(unsigned long long _ullData);

		Int(int _iRows, int _iCols, IntType _itType);

		Int(int _iRows, int _iCols, char* _pcData);
		Int(int _iRows, int _iCols, unsigned char* _pucData);
		Int(int _iRows, int _iCols, short* _psData);
		Int(int _iRows, int _iCols, unsigned short* _pusData);
		Int(int _iRows, int _iCols, int* _piData);
		Int(int _iRows, int _iCols, unsigned int* _puiData);
		Int(int _iRows, int _iCols, long long* _pllData);
		Int(int _iRows, int _iCols, unsigned long long* _pullData);

		virtual ~Int();

		/*data management*/
		void*									data_get(void) const;

		inline bool						data_set(char *_pcData);
		inline bool						data_set(unsigned char *_pucData);
		inline bool						data_set(short *_psData);
		inline bool						data_set(unsigned short *_pusData);
		inline bool						data_set(int *_piData);
		inline bool						data_set(unsigned int *_puiData);
		inline bool						data_set(long long *_pllData);
		inline bool						data_set(unsigned long long *_pullData);

		bool									data_set(int _iRows, int _iCols, char _cData);
		bool									data_set(int _iRows, int _iCols, unsigned char _ucData);
		bool									data_set(int _iRows, int _iCols, short _sData);
		bool									data_set(int _iRows, int _iCols, unsigned short _usData);
		bool									data_set(int _iRows, int _iCols, int _iData);
		bool									data_set(int _iRows, int _iCols, unsigned int _uiData);
		bool									data_set(int _iRows, int _iCols, long long _llData);
		bool									data_set(int _iRows, int _iCols, unsigned long long _ullData);

		char									data8_get(int _iRows, int _iCols);
		unsigned char					dataUnsigned8_get(int _iRows, int _iCols);
		short									data16_get(int _iRows, int _iCols);
		unsigned short				dataUnsigned16_get(int _iRows, int _iCols);
		int										data32_get(int _iRows, int _iCols);
		unsigned int					dataUnsigned32_get(int _iRows, int _iCols);
		long long							data64_get(int _iRows, int _iCols);
		unsigned long long		dataUnsigned64_get(int _iRows, int _iCols);

		/*zero or one set filler*/
		bool									zero_set(void);
		bool									one_set(void);

		/*Config management*/
    void									whoAmI(void);

    Int*									getAsInt(void);
		IntType								getIntType(void);

		string								toString(int _iPrecision, int _iLineLen);

  protected :
		RealType							getType(void);

  private :
		IntType								m_itType;
		//8 bits
    char*									m_pcData;
    unsigned char*				m_pucData;
		//16 bits
    short*								m_psData;
    unsigned short*				m_pusData;
		//32 bits
    int*									m_piData;
    unsigned int*					m_puiData;
		//64 bits
    long long*						m_pllData;
    unsigned long long*		m_pullData;

		void									CreateInt(int _iRows, int _iCols, IntType _itType, void* _pvData);
		bool									data_set(IntType _itType, void *_pvData);

	};
}
#endif /* !__INT_HH__ */
