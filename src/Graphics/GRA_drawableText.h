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
#ifndef GRA_DRAWABLETEXT
#define GRA_DRAWABLETEXT

#include <vector>
#include <ostream>

#include "wx/wx.h"

#include "GRA_drawableObject.h"

class GRA_color;
class GRA_font;
class GRA_simpleText;
class GRA_window;
class GRA_wxWidgets;

class GRA_drawableText : public GRA_drawableObject
{
private:
  typedef std::vector<GRA_simpleText*> textVec;
  typedef textVec::const_iterator textVecIter;

  // this class has a vector of pointers to GRA_simpleText objects (simple strings)
public:
  GRA_drawableText( wxString const &, bool =false );
  GRA_drawableText( wxString const &, double, double, double, double, int, GRA_font *, GRA_color * );

  ~GRA_drawableText()
  { DeleteStuff(); }
  
  GRA_drawableText( GRA_drawableText const &rhs ) : GRA_drawableObject(rhs)
  { CopyStuff(rhs); }
  
  GRA_drawableText &operator=( GRA_drawableText const &rhs )
  {
    if( this != &rhs )
    {
      DeleteStuff();
      type_ = rhs.type_;
      CopyStuff( rhs );
    }
    return *this;
  }
  
  bool operator==( GRA_drawableText const & ) const;

  wxString GetString() const
  { return inputString_; }
        
  textVec const &GetStrings() const
  { return strings_; }

  bool GetGraphUnits() const
  { return graphUnits_; }

  void SetX( double x )
  { x_ = x; }

  double GetX() const
  { return x_; }

  void SetY( double y )
  { y_ = y; }

  double GetY() const
  { return y_; }

  double GetHeight() const
  { return height_; }

  double GetAngle() const
  {
    double result = angle_ - static_cast<int>(angle_/360.)*360.;
    if( result < 0.0 )result += 360.;
    return result;
  }

  int GetAlignment() const
  { return alignment_; }

  GRA_font *GetFont() const
  { return font_; }

  GRA_color *GetColor() const
  { return color_; }
  
  void Parse();
  void Draw( GRA_wxWidgets *, wxDC & );
  void Erase( GRA_wxWidgets *, wxDC & );

  friend std::ostream &operator<<( std::ostream &, GRA_drawableText const & );
  
private:
  wxString inputString_;
  textVec strings_;
  bool graphUnits_;
  double x_, y_, height_, angle_;
  int alignment_;
  GRA_font *font_;
  GRA_color *color_;
  //
  double subsupFactor_;
  //
  void CopyStuff( GRA_drawableText const & );
  void DeleteStuff();
  //
  void GetStringStuff( std::size_t &, double &, double &, double &, double &, double &,
                       std::vector<int> &, double, double, GRA_wxWidgets *, wxDC & );
  void AlignXY( double, double, double &, double &, double &, double &, double & );

  void DetermineCommand( wxString &, double &, double &, double &,
                         bool &, bool &, int &, int & );
  wxChar Special( wxString const &, wxString & );


};

#endif
