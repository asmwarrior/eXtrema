/*
Copyright (C) 2005 Joseph L. Chuma, TRIUMF

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#ifndef OPR_TRANSPOSE
#define OPR_TRANSPOSE

#define OPERATORS_LINKAGE_
/*
#if defined BUILDING_OPERATORS_DLL_
#define OPERATORS_LINKAGE_ __declspec(dllexport)
#else
#define OPERATORS_LINKAGE_ __declspec(dllimport)
#endif
*/

#include "ArrayOperator.h"
 
class OPERATORS_LINKAGE_ OPR_transpose : public ArrayOperator
{
public:
  static OPR_transpose *Definition()
  { return &opr_transpose_; }
  
  void ProcessOperand( bool, int, ExprCodes *,
                       std::deque<ExprCodes*> &, int &, int & );
  void CalcOperand( bool, int, int *, Workspace * );
  void ArrayEval( Workspace * );
  
private:
  OPR_transpose() : ArrayOperator( "<-", 9, 0 )
  {}
  
  static OPR_transpose opr_transpose_;
};

#endif
