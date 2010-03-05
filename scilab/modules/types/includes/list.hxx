/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

#include <list>
#include "container.hxx"

namespace types
{
  class List : public Container
  {
  public :
																	 List();
																	~List(); 

  private :
																	List(List *_oListCopyMe);
    std::vector<InternalType *>*	getData();

  public :
    int														size_get(); 

    void													whoAmI(void) { std::cout << "types::List"; };

    RealType											getType(void) { return RealList; } 

  /**
   ** append(InternalType *_typedValue)
   ** Append the given value to the end of the List
   */
    void													append(InternalType *_typedValue);

  /**
   ** Clone
   ** Create a new List and Copy all values.
   */
    List*													clone();

    std::string										toString(int _iPrecision, int _iLineLen);

    List*													getAsList(void) { return this; }

		bool													insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, vector<types::InternalType*>* _poSource, bool _bAsVector);
		std::vector<InternalType*>		extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector);

  private :
    std::vector<InternalType *>*	m_plData;
  };
}
