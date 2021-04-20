/*
Copyright (C) 2007 Joseph L. Chuma, TRIUMF

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
#ifndef FCN_CANOPEN
#define FCN_CANOPEN

#include "ArrayFunction.h"

class FCN_canopen : public ArrayFunction
{
public:
  static FCN_canopen *Definition()
  { return &fcn_canopen_; }
  
  void ProcessArgument( bool, int, int & );
  void ArrayEval( Workspace * );
  
private:
  FCN_canopen() : ArrayFunction( "CANOPEN", 1, 1, MIXED2NUM, CHARACTER )
  {}
  
  static FCN_canopen fcn_canopen_;
};

#endif
 
