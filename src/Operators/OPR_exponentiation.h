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
#ifndef OPR_EXPONENTIATION
#define OPR_EXPONENTIATION

#include "ScalarOperator.h"

class OPR_exponentiation : public ScalarOperator
{
public:
  static OPR_exponentiation *Instance()
  {
    if( !opr_exponentiation_ )opr_exponentiation_ = new OPR_exponentiation();
    return opr_exponentiation_;
  }
  
  double ScalarEval( double a ) const
  { return 0.0; }
  
  double ScalarEval( double, double ) const;

private:
  OPR_exponentiation() : ScalarOperator( wxT("^"), 0, -7 )
  {}
  
  static OPR_exponentiation *opr_exponentiation_;
};

#endif
