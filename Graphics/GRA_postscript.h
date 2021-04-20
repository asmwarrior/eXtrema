/*
Copyright (C) 2005,...,2009 Joseph L. Chuma

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
#ifndef GRA_POSTSCRIPT
#define GRA_POSTSCRIPT

#include <fstream>

#include "ExString.h"
#include "GRA_outputType.h"

class GRA_color;
class GRA_point;
class GRA_polyline;
class GRA_polygon;
class GRA_multiLineFigure;
class GRA_ellipse;
class GRA_plotSymbol;
class GRA_bitmap;
class GRA_drawableText;

class GRA_postscript : public GRA_outputType
{
public:
  GRA_postscript( char const * );
  ~GRA_postscript();

  GRA_color *GetColor() const;
  void SetColor( GRA_color * );
  void SetColor( int );
  void SetColor( int, int, int );

  void SetLineWidth( int );

  int GetLineWidth() const
  { return lineWidth_; }

  void GetLimits( double &xmin, double &ymin, double &xmax, double &ymax ) const
  {
    xmin = dotsPerInch_*xMin_;
    ymin = dotsPerInch_*yMin_;
    xmax = dotsPerInch_*xMax_;
    ymax = dotsPerInch_*yMax_;
  }

  void Draw();
  void Draw( GRA_point * );
  void Draw( GRA_polyline * );
  void Draw( GRA_polygon * );
  void Draw( GRA_multiLineFigure * );
  void Draw( GRA_ellipse * );
  void Draw( GRA_plotSymbol * );
  void Draw( GRA_bitmap * );
  void Draw( GRA_drawableText * );

  void WorldToOutputType( double, double, double &, double & ) const;

private:
  void Initialize();
  void GenerateOutput( double, double, int );

  std::ofstream outFile_;
  ExString filename_;
  static double dotsPerInch_;
  int r_, g_, b_;
  int lineWidth_;
  int counter_;
};

#endif
