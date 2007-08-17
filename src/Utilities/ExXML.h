/*
Copyright (C) 2006 Joseph L. Chuma, TRIUMF

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
#ifndef EXTREMA_XML_H
#define EXTREMA_XML_H

#include <fstream>
#include <string>
#include <vector>

#include <libxml/xmlreader.h>
#include <libxml/xpath.h>

#include "wx/wx.h"

template <typename T> class my_cast
{
private:
  void const *value;
public:
  my_cast( void const *input ) : value(input) { }
  operator T () const { return static_cast<T>(value); }
};

class ExXML
{
public:
  ExXML() : reader_(0), version_(wxT("1.0")), encoding_(wxT("ISO-8859-1"))
  {}

  virtual ~ExXML()
  { if( reader_ )xmlFreeTextReader(reader_); }

  std::ofstream &GetStream()
  { return outputFile_; }

  void OpenFileForRead( wxString );
  void OpenFileForWrite( wxString );
  void CloseFile();

  int NextElementNode();
  wxString GetName();
  wxString GetTextValue();

  int GetType()
  {
    // for the enum definitions of the node types see xmlreader.h
    //
    // node types:
    // 1   start of element           8   comment                  15  end element
    // 2   attribute                  9   document node            16  end entity
    // 3   text node                  10  DTD/Doctype node         17  xml declaration
    // 4   CDATA section              11  document fragment
    // 5   entity reference           12  notation node
    // 6   entity declaration         13  whitespace
    // 7   processing instruction     14  significant whitespace
    //
    return xmlTextReaderNodeType(reader_);
  }

  wxString GetAttributeValue( wxString const & );

protected:
  xmlTextReaderPtr reader_;
  bool elementIsOpen_;
  std::vector<wxString> names_;
  std::ofstream outputFile_;
  wxString encoding_, version_;

  wxString Encode( wxString const & );
};

#endif