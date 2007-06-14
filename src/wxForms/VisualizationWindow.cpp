/*
Copyright (C) 2005,...,2007 Joseph L. Chuma, TRIUMF

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
#include <iostream>
#include <fstream>

#include "wx/config.h"
#include "wx/dcbuffer.h"
#include "wx/notebook.h"

#include "VisualizationWindow.h"
#include "VisualizationSpeedButtonPanel.h"
#include "GRA_window.h"
#include "GRA_setOfCharacteristics.h"
#include "GRA_distanceCharacteristic.h"
#include "GRA_wxWidgets.h"
#include "ExGlobals.h"
#include "EGraphicsError.h"
#include "GraphForm.h"
#include "TextForm.h"
#include "PeakFindForm.h"
#include "GRA_drawableText.h"
#include "GraphicsPage.h"
#include "SetAspectRatioForm.h"

// the event tables connect the wxWidgets events with the
// event handler functions which process them
//
BEGIN_EVENT_TABLE( VisualizationWindow, wxFrame )
  EVT_MENU( ID_importPNG, VisualizationWindow::OnImportPNG )
  EVT_MENU( ID_importJPEG, VisualizationWindow::OnImportJPEG )
  EVT_MENU( ID_savePS, VisualizationWindow::OnSavePS )
  EVT_MENU( ID_savePNG, VisualizationWindow::OnSavePNG )
  EVT_MENU( ID_saveJPEG, VisualizationWindow::OnSaveJPEG )
  EVT_MENU( ID_print, VisualizationWindow::OnPrint )
  EVT_MENU( ID_clearGraphicsPage, VisualizationWindow::OnClearGraphicsPage )
  EVT_MENU( ID_clearWindow, VisualizationWindow::OnClearWindow )
  EVT_MENU( ID_drawGraph, VisualizationWindow::OnDrawGraph )
  EVT_MENU( ID_drawText, VisualizationWindow::OnDrawText )
  EVT_MENU( ID_replotAll, VisualizationWindow::OnReplotAll )
  EVT_MENU( ID_replotCurrent, VisualizationWindow::OnReplotCurrent )
  EVT_MENU( ID_zerolinesH, VisualizationWindow::OnZerolinesH )
  EVT_MENU( ID_zerolinesV, VisualizationWindow::OnZerolinesV )
  EVT_MENU( ID_zerolinesB, VisualizationWindow::OnZerolinesB )
  EVT_MENU( ID_resetDefaults, VisualizationWindow::OnResetDefaults )
  EVT_MENU( ID_configureWindow, VisualizationWindow::OnConfigureWindow )
  EVT_MENU( ID_newPage, VisualizationWindow::OnNewPage )
  EVT_MENU( ID_removePage, VisualizationWindow::OnRemovePage )
  EVT_MENU( ID_aspectRatio, VisualizationWindow::OnSetAspectRatio )
  EVT_CLOSE( VisualizationWindow::CloseEventHandler )
  EVT_SIZE( VisualizationWindow::OnSize )
END_EVENT_TABLE()
  //EVT_SET_FOCUS( VisualizationWindow::OnSetFocus )
  //EVT_KILL_FOCUS( VisualizationWindow::OnKillFocus )

VisualizationWindow::VisualizationWindow( wxWindow *parent )
    : wxFrame( parent,wxID_ANY,wxT("Visualization Window"),
               wxDefaultPosition,wxDefaultSize,
               wxDEFAULT_FRAME_STYLE&~wxCLOSE_BOX )
{
  graphForm_ = 0;
  textForm_ = 0;
  peakFindForm_ = 0;
  textToPlace_ = 0;
  setAspectRatioForm_ = 0;
  
  ExGlobals::SetVisualizationWindow( this );

  wxConfigBase *config = wxConfigBase::Get();
  int ulx = config->Read( wxT("/VisualizationWindow/UPPERLEFTX"), 10l );
  int uly = config->Read( wxT("/VisualizationWindow/UPPERLEFTY"), 10l );
  int width = config->Read( wxT("/VisualizationWindow/WIDTH"), 700l );
  int height = config->Read( wxT("/VisualizationWindow/HEIGHT"), 500l );
  double aspectRatio;
  config->Read( wxT("/VisualizationWindow/ASPECTRATIO"), &aspectRatio,
                ExGlobals::GetAspectRatio() );
  //SetSize( ulx, uly, std::max(400,width), std::max(250,height) );
  SetSize( ulx, uly, width, height );

  wxMenu *fileMenu = new wxMenu();

  wxMenu *importMenu = new wxMenu();
  importMenu->Append( ID_importPNG, wxT("PNG"), wxT("import a PNG file") );
  importMenu->Enable( ID_importPNG, false );
  importMenu->Append( ID_importJPEG, wxT("JPEG"), wxT("import a JPEG file") );
  importMenu->Enable( ID_importJPEG, false );
  fileMenu->Append( wxID_ANY, wxT("Import"), importMenu, wxT("import a graphics file") );

  wxMenu *saveMenu = new wxMenu();
  saveMenu->Append( ID_savePS, wxT("PostScript"),
                    wxT("save the graphics to a PostScript file") );
  saveMenu->Append( ID_savePNG, wxT("PNG"),
                    wxT("save the graphics to a PNG file") );
  saveMenu->Append( ID_saveJPEG, wxT("JPEG"),
                    wxT("save the graphics to a JPEG file") );
  fileMenu->Append( wxID_ANY, wxT("Save drawing"), saveMenu,
                    wxT("save the graphics to a file") );
  
  fileMenu->Append( ID_print, wxT("Print"), wxT("send graphics to a printer") );
  
  wxMenu *drawingMenu = new wxMenu();
  
  wxMenu *clearMenu = new wxMenu();
  clearMenu->Append( ID_clearGraphicsPage, wxT("Clear graphics page"),
                     wxT("clear the graphics page") );
  clearMenu->Append( ID_clearWindow, wxT("Clear current window"),
                     wxT("clear the current window") );
  drawingMenu->Append( wxID_ANY, wxT("Clear"), clearMenu, wxT("clear the graphics") );
  
  drawingMenu->Append( ID_drawGraph, wxT("Draw a graph"), wxT("draw a graph") );
  drawingMenu->Append( ID_drawText, wxT("Draw text"), wxT("draw text") );

  wxMenu *replotMenu = new wxMenu();
  replotMenu->Append( ID_replotAll, wxT("Replot all windows"),
                      wxT("replot all windows") );
  replotMenu->Append( ID_replotCurrent, wxT("Replot current window"),
                      wxT("replot just the current window") );
  drawingMenu->Append( wxID_ANY, wxT("Replot"), replotMenu, wxT("replot the graphics") );

  wxMenu *zerolinesMenu = new wxMenu();
  zerolinesMenu->Append( ID_zerolinesH, wxT("Horizontal"),
                         wxT("draw horizontal line through (0,0)") );
  zerolinesMenu->Append( ID_zerolinesV, wxT("Vertical"),
                         wxT("draw vertical line through (0,0)") );
  zerolinesMenu->Append( ID_zerolinesB, wxT("Both"),
                         wxT("draw horizontal and vertical lines through (0,0)") );
  drawingMenu->Append( wxID_ANY, wxT("Draw zerolines"), zerolinesMenu,
                       wxT("draw lines through (0,0)") );
  
  wxMenu *setupMenu = new wxMenu();
  
  setupMenu->Append( ID_resetDefaults, wxT("Reset extrema defaults"), wxT("reset program defaults") );
  setupMenu->Append( ID_configureWindow, wxT("Configure subwindow"),
                     wxT("configure graphics subwindow") );
  setupMenu->Enable( ID_configureWindow, false );
  setupMenu->Append( ID_aspectRatio, wxT("Set the graphics page aspect ratio"),
                     wxT("set graphics page aspect ratio") );
  setupMenu->Enable( ID_aspectRatio, false );

  wxMenu *pagesMenu = new wxMenu();
  pagesMenu->Append( ID_newPage, wxT("New page"), wxT("make new graphics page") );
  pagesMenu->Append( ID_removePage, wxT("Remove last page"), wxT("remove the last graphcis page") );
  
  wxMenuBar *menuBar = new wxMenuBar();
  menuBar->Append( fileMenu, wxT("&File") );
  menuBar->Append( drawingMenu, wxT("&Drawing") );
  menuBar->Append( setupMenu, wxT("&Setup") );
  menuBar->Append( pagesMenu, wxT("&Pages") );

  SetMenuBar( menuBar );
  
  // for a vertical sizer:
  // proportion = 0    means no vertical expansion
  // proportion > 0    allows for vertical expansion
  // Expand()          allows for horizontal expansion
  
  wxBoxSizer *sizer = new wxBoxSizer( wxVERTICAL );

  visualizationSpeedButtonPanel_ = new VisualizationSpeedButtonPanel(this);
  sizer->Add( visualizationSpeedButtonPanel_, wxSizerFlags(0).Expand().Border(wxALL,1) );

  double ratio = static_cast<double>(height)/static_cast<double>(width);
  ratio > aspectRatio ? height = static_cast<int>(aspectRatio*width+0.5) :
                        width = static_cast<int>(height/aspectRatio+0.5);

  notebook_ = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_RIGHT );

  sizer->Add( notebook_, wxSizerFlags(1).Expand().Center().Border(wxALL,5) );
  SetSizer( sizer );

  GraphicsPage *page = new GraphicsPage(notebook_);

  statusBar_ = new MyStatusBar( this );
  SetStatusBar( statusBar_ );

  ExGlobals::SetMonitorLimits( 0, 0, width, height );
  ExGlobals::SetAspectRatio( aspectRatio );
  page->ResetWindows();
  wxClientDC dc( page );
  dc.SetBackground( wxBrush(wxT("WHITE"),wxSOLID) );
  dc.Clear();

  // Show the window.
  // Frames, unlike simple controls, are not shown when created initially.
  Show( true );
}

void VisualizationWindow::CloseEventHandler( wxCloseEvent &WXUNUSED(event) )
{
  wxConfigBase *config = wxConfigBase::Get();
  if( config )
  {
    int ulx, uly;
    GetPosition( &ulx, &uly );
    config->Write( wxT("/VisualizationWindow/UPPERLEFTX"), static_cast<long>(ulx) );
    config->Write( wxT("/VisualizationWindow/UPPERLEFTY"), static_cast<long>(uly) );
    int width, height;
    GetSize( &width, &height );
    config->Write( wxT("/VisualizationWindow/WIDTH"), static_cast<long>(width) );
    config->Write( wxT("/VisualizationWindow/HEIGHT"), static_cast<long>(height) );
    config->Write( wxT("/VisualizationWindow/ASPECTRATIO"), ExGlobals::GetAspectRatio() );
  }
  Destroy();
}

void VisualizationWindow::RefreshGraphics()
{ static_cast<GraphicsPage*>(notebook_->GetCurrentPage())->RefreshGraphics(); }

void VisualizationWindow::ClearWindows()
{
  // destroy all drawableObjects in all graph windows on the current page
  //
  static_cast<GraphicsPage*>(notebook_->GetCurrentPage())->ClearWindows();
}

void VisualizationWindow::ReplotAllWindows()
{ static_cast<GraphicsPage*>(notebook_->GetCurrentPage())->ReplotAllWindows(); }

void VisualizationWindow::ReplotCurrentWindow( bool repaint )
{ static_cast<GraphicsPage*>(notebook_->GetCurrentPage())->ReplotCurrentWindow(repaint); }

void VisualizationWindow::SetInteractiveText( GRA_drawableText *dt )
{
  static_cast<GraphicsPage*>(notebook_->GetCurrentPage())->SetTextPlacementMode( dt );
  SetFocus();
  Raise();
  //
  std::vector<wxString> lines;
  lines.push_back( wxString(wxT("text: "))+dt->GetString() );
  lines.push_back( wxT(" ") );
  lines.push_back( wxT("click in the visualization window to place the string") );
  ExGlobals::ShowHint( lines );
}

void VisualizationWindow::SaveBitmap( int xmin, int ymin, int xmax, int ymax,
                                      wxString const &filename, int type )
{
  GRA_wxWidgets ps( xmin, ymin, xmax, ymax );
  wxBitmap tempBM( xmax-xmin, ymax-ymin );
  wxMemoryDC dc;
  dc.SelectObject( tempBM );
  dc.SetBackground( wxBrush(wxT("WHITE"),wxSOLID) );
  dc.Clear();
  dc.StartDoc( wxT("produced by EXTREMA") );
  try
  {
    static_cast<GraphicsPage*>(notebook_->GetCurrentPage())->DrawGraphWindows(&ps,dc);
  }
  catch ( EGraphicsError const &e )
  {
    throw;
  }
  dc.EndDoc();
  wxImage image( tempBM.ConvertToImage() );
  image.SaveFile( filename, type );
}

void VisualizationWindow::SavePS( wxString const &filename )
{
  try
  {
    static_cast<GraphicsPage*>(notebook_->GetCurrentPage())->SavePS( filename );
  }
  catch ( EGraphicsError const &e )
  {
    throw;
  }
}

void VisualizationWindow::ResetPages()
{
  Layout();
  
  wxSize size( notebook_->GetClientSize() );
  //
  // the 50 below takes care of the width of the tab on the notebook
  int width = size.GetWidth() - 50;
  int height = size.GetHeight();

  double ratio = static_cast<double>(height)/static_cast<double>(width);
  double aspectRatio = ExGlobals::GetAspectRatio();
  ratio > aspectRatio ? height = static_cast<int>(aspectRatio*width+0.5) :
                        width = static_cast<int>(height/aspectRatio+0.5);
  
  ExGlobals::SetMonitorLimits( 0, 0, width, height );

  notebook_->SetClientSize( width+50, height );

  int nPages = notebook_->GetPageCount();
  for( int i=0; i<nPages; ++i )
  {
    notebook_->GetPage(i)->SetClientSize( width, height );
    static_cast<GraphicsPage*>(notebook_->GetPage(i))->Paint();
  }
}

void VisualizationWindow::ClearAllPages()
{
  for( int i=0; i<notebook_->GetPageCount(); ++i )
  {
    static_cast<GraphicsPage*>(notebook_->GetPage(i))->ClearWindows();
    wxClientDC dc( notebook_->GetPage(i) );
    dc.SetBackground( wxBrush(wxT("WHITE"),wxSOLID) );
    dc.Clear();
  }
}

void VisualizationWindow::ResetWindows()
{
  for( int i=0; i<notebook_->GetPageCount(); ++i )
    static_cast<GraphicsPage*>(notebook_->GetPage(i))->ResetWindows();
}

int VisualizationWindow::GetNumberOfPages()
{ return notebook_->GetPageCount(); }

wxWindow *VisualizationWindow::GetPage()
{ return notebook_->GetCurrentPage(); }

GraphicsPage *VisualizationWindow::GetPage( int n )
{ return static_cast<GraphicsPage*>(notebook_->GetPage(n-1)); }

void VisualizationWindow::MakeFirstPage()
{ new GraphicsPage(notebook_); }
  
void VisualizationWindow::DeleteAllPages()
{ notebook_->DeleteAllPages(); }

void VisualizationWindow::SetWindowNumber( int n )
{ static_cast<GraphicsPage*>(notebook_->GetCurrentPage())->SetWindowNumber(n); }

int VisualizationWindow::GetWindowNumber()
{ return static_cast<GraphicsPage*>(notebook_->GetCurrentPage())->GetWindowNumber(); }

int VisualizationWindow::GetNumberOfWindows()
{ return static_cast<GraphicsPage*>(notebook_->GetCurrentPage())->GetNumberOfWindows(); }

std::vector<GRA_window*> &VisualizationWindow::GetGraphWindows()
{ return static_cast<GraphicsPage*>(notebook_->GetCurrentPage())->GetGraphWindows(); }

GRA_window *VisualizationWindow::GetGraphWindow()
{ return static_cast<GraphicsPage*>(notebook_->GetCurrentPage())->GetGraphWindow(); }

GRA_window *VisualizationWindow::GetGraphWindow( int n )
{ return static_cast<GraphicsPage*>(notebook_->GetCurrentPage())->GetGraphWindow(n); }

GRA_window *VisualizationWindow::GetGraphWindow( double x, double y )
{ return static_cast<GraphicsPage*>(notebook_->GetCurrentPage())->GetGraphWindow(x,y); }

void VisualizationWindow::AddGraphWindow( GRA_window *gw )
{ static_cast<GraphicsPage*>(notebook_->GetCurrentPage())->AddGraphWindow(gw); }

void VisualizationWindow::DrawGraphWindows( GRA_wxWidgets *output, wxDC &dc )
{ static_cast<GraphicsPage*>(notebook_->GetCurrentPage())->DrawGraphWindows(output,dc); }

void VisualizationWindow::OnClearGraphicsPage( wxCommandEvent &WXUNUSED(event) )
{
  wxWindow *window = notebook_->GetCurrentPage();
  wxClientDC dc( window );
  dc.Clear();
  static_cast<GraphicsPage*>(window)->ClearWindows();
  if( ExGlobals::StackIsOn() )ExGlobals::WriteStack( wxT("CLEAR") );  
}

void VisualizationWindow::OnClearWindow( wxCommandEvent &WXUNUSED(event) )
{
  GRA_window *gw = static_cast<GraphicsPage*>(notebook_->GetCurrentPage())->GetGraphWindow();
  gw->Erase();
  gw->Clear();
  static_cast<GraphicsPage*>(notebook_->GetCurrentPage())->Paint();
}

void VisualizationWindow::OnReplotAll( wxCommandEvent &WXUNUSED(event) )
{
  GraphicsPage *graphicsPage = static_cast<GraphicsPage*>(notebook_->GetCurrentPage());
  bool nothingToReplot = true;
  std::size_t numWindows = graphicsPage->GetNumberOfWindows();
  for( std::size_t i=0; i<numWindows; ++i )
  {
    if( !graphicsPage->GetGraphWindow(i)->GetDrawableObjects().empty() )
    {
      nothingToReplot = false;
      break;
    }
  }
  if( nothingToReplot )
  {
    wxMessageBox( wxT("there is nothing to replot in any window"), wxT("warning"),
                  wxOK|wxICON_ERROR );
    return;
  }
  try
  {
    graphicsPage->ReplotAllWindows();
  }
  catch ( EGraphicsError const &e )
  {
    wxMessageBox( wxString(e.what(),wxConvUTF8), wxT("error"), wxOK|wxICON_ERROR );
    return;
  }
}

void VisualizationWindow::OnReplotCurrent( wxCommandEvent &WXUNUSED(event) )
{
  GraphicsPage *graphicsPage = static_cast<GraphicsPage*>(notebook_->GetCurrentPage());
  if( graphicsPage->GetGraphWindow()->GetDrawableObjects().empty() )
  {
    wxMessageBox( wxT("there is nothing to replot in the current window"), wxT("error"),
                  wxOK|wxICON_ERROR );
    return;
  }
  try
  {
    graphicsPage->ReplotCurrentWindow( true );
  }
  catch ( EGraphicsError const &e )
  {
    wxMessageBox( wxString(e.what(),wxConvUTF8), wxT("error"), wxOK|wxICON_ERROR );
    return;
  }
}

void VisualizationWindow::OnSize( wxSizeEvent &event )
{
  SetSize( event.GetSize() );
  ResetPages();
}

void VisualizationWindow::OnSavePS( wxCommandEvent &WXUNUSED(event) )
{
  wxString wildcard( wxT("PostScript (*.eps)|*.eps") );
  wxFileDialog *fd = new wxFileDialog( this, wxT("Choose an output file"), wxT(""), wxT(""),
                                       wildcard, wxSAVE|wxOVERWRITE_PROMPT|wxCHANGE_DIR );
  if( fd->ShowModal() != wxID_OK )return;
  wxString filename( fd->GetFilename() );
  if( filename.empty() )return;
  if( filename.find_last_of(wxT('.')) == filename.npos )filename += wxT(".eps");
  std::ofstream outFile;
  outFile.open( filename.mb_str(wxConvUTF8), std::ios_base::out );
  if( !outFile.is_open() )
  {
    wxMessageBox( wxString(wxT("unable to open "))+filename, wxT("error"),
                  wxOK|wxICON_ERROR );
    return;
  }
  outFile.close();
  try
  {
    static_cast<GraphicsPage*>(notebook_->GetCurrentPage())->SavePS( filename );
  }
  catch ( EGraphicsError const &e )
  {
    wxMessageBox( wxString(e.what(),wxConvUTF8), wxT("error"), wxOK|wxICON_ERROR );
    return;
  }
}

void VisualizationWindow::OnSavePNG( wxCommandEvent &WXUNUSED(event) )
{
  wxString wildcard( wxT("PNG (*.png)|*.png") );
  wxFileDialog *fd = new wxFileDialog( this, wxT("Choose an output file"), wxT(""), wxT(""),
                                       wildcard, wxSAVE|wxOVERWRITE_PROMPT|wxCHANGE_DIR );
  if( fd->ShowModal() != wxID_OK )return;
  wxString filename( fd->GetFilename() );
  if( filename.empty() )return;
  if( filename.find_last_of(wxT('.')) == filename.npos )filename += wxT(".png");
  std::ofstream outFile;
  outFile.open( filename.mb_str(wxConvUTF8), std::ios_base::out );
  if( !outFile.is_open() )
  {
    wxMessageBox( wxString(wxT("unable to open "))+filename, wxT("error"),
                  wxOK|wxICON_ERROR );
    return;
  }
  outFile.close();
  try
  {
    int xminM, yminM, xmaxM, ymaxM;
    ExGlobals::GetMonitorLimits( xminM, yminM, xmaxM, ymaxM );
    SaveBitmap( xminM, yminM, xmaxM, ymaxM, filename, wxBITMAP_TYPE_PNG );
  }
  catch ( EGraphicsError const &e )
  {
    wxMessageBox( wxString(e.what(),wxConvUTF8), wxT("error"), wxOK|wxICON_ERROR );
    return;
  }
}

void VisualizationWindow::OnSaveJPEG( wxCommandEvent &WXUNUSED(event) )
{
  wxString wildcard( wxT("JPEG (*.jpeg)|*.jpeg") );
  wxFileDialog *fd = new wxFileDialog( this, wxT("Choose an output file"), wxT(""), wxT(""),
                                       wildcard, wxSAVE|wxOVERWRITE_PROMPT|wxCHANGE_DIR );
  if( fd->ShowModal() != wxID_OK )return;
  wxString filename( fd->GetFilename() );
  if( filename.empty() )return;
  if( filename.find_last_of(wxT('.')) == filename.npos )filename += wxT(".jpeg");
  std::ofstream outFile;
  outFile.open( filename.mb_str(wxConvUTF8), std::ios_base::out );
  if( !outFile.is_open() )
  {
    wxMessageBox( wxString(wxT("unable to open "))+filename, wxT("error"),
                  wxOK|wxICON_ERROR );
    return;
  }
  outFile.close();
  try
  {
    int xminM, yminM, xmaxM, ymaxM;
    ExGlobals::GetMonitorLimits( xminM, yminM, xmaxM, ymaxM );
    SaveBitmap( xminM, yminM, xmaxM, ymaxM, filename, wxBITMAP_TYPE_JPEG );
  }
  catch ( EGraphicsError const &e )
  {
    wxMessageBox( wxString(e.what(),wxConvUTF8), wxT("error"), wxOK|wxICON_ERROR );
    return;
  }
}

void VisualizationWindow::OnPrint( wxCommandEvent &event )
{ visualizationSpeedButtonPanel_->OnPrintDrawing( event ); }

void VisualizationWindow::OnNewPage( wxCommandEvent &WXUNUSED(event) )
{ new GraphicsPage(notebook_); }

void VisualizationWindow::OnRemovePage( wxCommandEvent &WXUNUSED(event) )
{
  int nPages = notebook_->GetPageCount();
  if( nPages > 1 )notebook_->RemovePage( nPages-1 );
}

void VisualizationWindow::NewPage( int n )
{
  // this function is only called for n > maxPage
  //
  int maxPage = notebook_->GetPageCount();
  int i = maxPage-1;
  while( ++i < n )
  {
    new GraphicsPage(notebook_);
  }
}

void VisualizationWindow::SetPage( int n )
{
#if wxMINOR_VERSION < 8
  notebook_->SetSelection( n-1 );
#else
  notebook_->ChangeSelection( n-1 );
#endif
}

void VisualizationWindow::InheritPage( int m )
{
  static_cast<GraphicsPage*>(notebook_->GetCurrentPage())->
      InheritFrom( static_cast<GraphicsPage*>(notebook_->GetPage(m-1)) );
}

void VisualizationWindow::ZeroGraphForm()
{ graphForm_ = 0; }

GraphForm *VisualizationWindow::GetGraphForm()
{ return graphForm_; }

void VisualizationWindow::SetGraphForm( GraphForm *graphForm )
{ graphForm_ = graphForm; }

void VisualizationWindow::ZeroTextForm()
{ textForm_ = 0; }

TextForm *VisualizationWindow::GetTextForm()
{ return textForm_; }

void VisualizationWindow::SetTextForm( TextForm *textForm )
{ textForm_ = textForm; }

MyStatusBar *VisualizationWindow::GetStatusBar()
{ return statusBar_; }

void VisualizationWindow::ZeroPeakFindForm()
{ peakFindForm_ = 0; }

void VisualizationWindow::ZeroSetAspectRatioForm()
{ setAspectRatioForm_ = 0; }

void VisualizationWindow::OnPeakFind( wxCommandEvent &WXUNUSED(event) )
{
  if( peakFindForm_ )peakFindForm_->Raise();
  else
  {
    peakFindForm_ = new PeakFindForm( this );
    peakFindForm_->Show();
  }
}

void VisualizationWindow::OnDrawGraph( wxCommandEvent &WXUNUSED(event) )
{
  if( graphForm_ )graphForm_->Raise();
  else
  {
    graphForm_ = new GraphForm( this );
    graphForm_->Show();
  }
}

void VisualizationWindow::OnDrawText( wxCommandEvent &WXUNUSED(event) )
{
  if( textForm_ )textForm_->Raise();
  else
  {
    textForm_ = new TextForm( this );
    textForm_->Show();
  }
}

void VisualizationWindow::OnZerolinesH( wxCommandEvent &WXUNUSED(event) )
{
  try
  {
    ExGlobals::GetGraphWindow()->DrawZerolines( true, false );
  }
  catch ( EGraphicsError &e )
  {
    wxMessageBox( wxString(e.what(),wxConvUTF8), wxT("error"), wxOK|wxICON_ERROR );
    return;
  }
}

void VisualizationWindow::OnZerolinesV( wxCommandEvent &WXUNUSED(event) )
{
  try
  {
    ExGlobals::GetGraphWindow()->DrawZerolines( false, true );
  }
  catch ( EGraphicsError &e )
  {
    wxMessageBox( wxString(e.what(),wxConvUTF8), wxT("error"), wxOK|wxICON_ERROR );
    return;
  }
}

void VisualizationWindow::OnZerolinesB( wxCommandEvent &WXUNUSED(event) )
{
  try
  {
    ExGlobals::GetGraphWindow()->DrawZerolines( true, true );
  }
  catch ( EGraphicsError &e )
  {
    wxMessageBox( wxString(e.what(),wxConvUTF8), wxT("error"), wxOK|wxICON_ERROR );
    return;
  }
}

void VisualizationWindow::OnResetDefaults( wxCommandEvent &WXUNUSED(event) )
{ ExGlobals::GetGraphWindow()->SetDefaults(); }

void VisualizationWindow::OnConfigureWindow( wxCommandEvent &WXUNUSED(event) )
{}

void VisualizationWindow::OnSetAspectRatio( wxCommandEvent &WXUNUSED(event) )
{
  if( setAspectRatioForm_ )setAspectRatioForm_->Raise();
  else
  {
    setAspectRatioForm_ = new SetAspectRatioForm( this );
    setAspectRatioForm_->Show();
  }
}

std::ostream &operator<<( std::ostream &out, VisualizationWindow const *vw )
{
  int ulx, uly;
  vw->GetPosition( &ulx, &uly );
  int width, height;
  vw->GetSize( &width, &height );
  out << "<visualizationwindow top=\"" << uly << "\" left=\"" << ulx
      << "\" height=\"" << height << "\" width=\"" << width
      << "\" aspectratio=\"" << ExGlobals::GetAspectRatio()
      << "\" currentpage=\"" << vw->notebook_->GetSelection() << "\"/>\n";
  return out;
}

BEGIN_EVENT_TABLE( MyStatusBar, wxStatusBar )
  EVT_BUTTON( ID_changeUnits, MyStatusBar::OnChangeUnits )
  EVT_SIZE( MyStatusBar::OnSize )
END_EVENT_TABLE()

MyStatusBar::MyStatusBar( VisualizationWindow *parent )
    : wxStatusBar( (wxWindow*)parent, wxID_ANY )
{
  unitsType_ = 0;
  units_[0] = wxT("graph units");
  units_[1] = wxT("world units");
  units_[2] = wxT("percentages");
  units_[3] = wxT("pixels");

  int widths[] = { 150, -1, -1 };
  SetFieldsCount( 3, widths );
  int sbStyles[3];
  sbStyles[0] = wxSB_FLAT;
  sbStyles[1] = wxSB_NORMAL;
  sbStyles[2] = wxSB_NORMAL;
  SetStatusStyles( 3, sbStyles );

  unitsButton_ = new wxButton( this, ID_changeUnits, units_[0] );
  wxFont f( unitsButton_->GetFont() );
  f.SetPointSize( 7 );
  unitsButton_->SetFont( f );

  SetButtonSize();
}

int MyStatusBar::GetUnitsType()
{ return unitsType_; }

void MyStatusBar::OnChangeUnits( wxCommandEvent &WXUNUSED(event) )
{
  if( ++unitsType_ == 4 )unitsType_ = 0;
  unitsButton_->SetLabel( units_[unitsType_] );
}

void MyStatusBar::OnSize( wxSizeEvent &WXUNUSED(event) )
{ SetButtonSize(); }

void MyStatusBar::SetButtonSize()
{
  wxRect rect;
  GetFieldRect( 0, rect );
  unitsButton_->SetSize( rect.x, rect.y-1, rect.width, rect.height+2 );
}

// end of file
