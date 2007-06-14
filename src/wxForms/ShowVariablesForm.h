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
#ifndef EXTREMA_SHOWVARIABLESFORM
#define EXTREMA_SHOWVARIABLESFORM

#include "wx/wx.h"
#include "wx/grid.h"

class AnalysisWindow;
class VarInfoForm;

class ShowVariablesForm : public wxFrame
{
public:
  ShowVariablesForm( AnalysisWindow * );
  void ZeroVarInfoForm();
  
  // event handlers
  void OnRefresh( wxCommandEvent & );
  void OnClose( wxCommandEvent & );
  void CloseEventHandler( wxCloseEvent & );
  void OnSelectCell( wxGridEvent & );

private:
  AnalysisWindow *analysisWindow_;
  wxGrid *varGrid_;
  VarInfoForm *varInfoForm_;

  void FillGrid();

  // any class wishing to process wxWidgets events must use this macro
  DECLARE_EVENT_TABLE()
};

#endif
