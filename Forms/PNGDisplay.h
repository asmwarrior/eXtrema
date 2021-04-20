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
#ifndef PNGDisplayH
#define PNGDisplayH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>

#include <cstdio>

class TPNGForm : public TForm
{
__published:	// IDE-managed Components
  TImage *PNGImage;
  TToolBar *PNGToolBar;
  TSpeedButton *DigitizeSpeedButton;
  TStatusBar *PNGStatusBar;
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall FormClose2(TObject *Sender, bool &CanClose);
  void __fastcall DigitizeClick(TObject *Sender);
  void __fastcall ImageMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall ImageMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);

private:	// User declarations
  bool digitizing_;
  bool picking_;

public:		// User declarations
  __fastcall TPNGForm(TComponent* Owner);
  __fastcall ~TPNGForm();
  void __fastcall LoadPNG( FILE * );
  void __fastcall StartDigitizing();
  void __fastcall StopDigitizing();
  void __fastcall StartPicking();
  void __fastcall ClearStatusBar();
};

extern PACKAGE TPNGForm *PNGForm;

#endif
