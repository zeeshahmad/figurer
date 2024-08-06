# -*- coding: utf-8 -*-

###########################################################################
## Python code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
## http://www.wxformbuilder.org/
##
## PLEASE DO *NOT* EDIT THIS FILE!
###########################################################################

import wx
import wx.xrc
import wx.stc

import gettext
_ = gettext.gettext

###########################################################################
## Class MainFrame
###########################################################################

class MainFrame ( wx.Frame ):

    def __init__( self, parent ):
        wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = _(u"Figurer"), pos = wx.DefaultPosition, size = wx.Size( 1141,693 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )

        self.SetSizeHints( wx.DefaultSize, wx.DefaultSize )
        self.SetBackgroundColour( wx.Colour( 255, 255, 255 ) )

        bSizer1 = wx.BoxSizer( wx.VERTICAL )

        bSizer2 = wx.BoxSizer( wx.HORIZONTAL )

        self.open_close_button = wx.Button( self, wx.ID_ANY, _(u"Open"), wx.DefaultPosition, wx.DefaultSize, 0 )
        bSizer2.Add( self.open_close_button, 0, wx.ALL, 5 )

        self.m_button2 = wx.Button( self, wx.ID_ANY, _(u"Save"), wx.DefaultPosition, wx.DefaultSize, 0 )
        bSizer2.Add( self.m_button2, 0, wx.ALL, 5 )

        self.m_button3 = wx.Button( self, wx.ID_ANY, _(u"Rescan TeX"), wx.DefaultPosition, wx.DefaultSize, 0 )
        bSizer2.Add( self.m_button3, 0, wx.ALL, 5 )


        bSizer1.Add( bSizer2, 0, wx.EXPAND, 5 )

        bSizer5 = wx.BoxSizer( wx.HORIZONTAL )

        self.m_splitter1 = wx.SplitterWindow( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.SP_3D )
        self.m_splitter1.Bind( wx.EVT_IDLE, self.m_splitter1OnIdle )

        self.m_panel1 = wx.Panel( self.m_splitter1, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
        bSizer6 = wx.BoxSizer( wx.VERTICAL )

        self.m_notebook3 = wx.Notebook( self.m_panel1, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, 0 )
        self.m_panel4 = wx.Panel( self.m_notebook3, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
        bSizer8 = wx.BoxSizer( wx.VERTICAL )

        self.m_scintilla1 = wx.stc.StyledTextCtrl( self.m_panel4, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, 0)
        self.m_scintilla1.SetUseTabs ( False )
        self.m_scintilla1.SetTabWidth ( 2 )
        self.m_scintilla1.SetIndent ( 2 )
        self.m_scintilla1.SetTabIndents( True )
        self.m_scintilla1.SetBackSpaceUnIndents( True )
        self.m_scintilla1.SetViewEOL( False )
        self.m_scintilla1.SetViewWhiteSpace( False )
        self.m_scintilla1.SetMarginWidth( 2, 0 )
        self.m_scintilla1.SetIndentationGuides( True )
        self.m_scintilla1.SetReadOnly( False )
        self.m_scintilla1.SetMarginType ( 1, wx.stc.STC_MARGIN_SYMBOL )
        self.m_scintilla1.SetMarginMask ( 1, wx.stc.STC_MASK_FOLDERS )
        self.m_scintilla1.SetMarginWidth ( 1, 16)
        self.m_scintilla1.SetMarginSensitive( 1, True )
        self.m_scintilla1.SetProperty ( "fold", "1" )
        self.m_scintilla1.SetFoldFlags ( wx.stc.STC_FOLDFLAG_LINEBEFORE_CONTRACTED | wx.stc.STC_FOLDFLAG_LINEAFTER_CONTRACTED )
        self.m_scintilla1.SetMarginType( 0, wx.stc.STC_MARGIN_NUMBER )
        self.m_scintilla1.SetMarginWidth( 0, self.m_scintilla1.TextWidth( wx.stc.STC_STYLE_LINENUMBER, "_99999" ) )
        self.m_scintilla1.MarkerDefine( wx.stc.STC_MARKNUM_FOLDER, wx.stc.STC_MARK_BOXPLUS )
        self.m_scintilla1.MarkerSetBackground( wx.stc.STC_MARKNUM_FOLDER, wx.BLACK)
        self.m_scintilla1.MarkerSetForeground( wx.stc.STC_MARKNUM_FOLDER, wx.WHITE)
        self.m_scintilla1.MarkerDefine( wx.stc.STC_MARKNUM_FOLDEROPEN, wx.stc.STC_MARK_BOXMINUS )
        self.m_scintilla1.MarkerSetBackground( wx.stc.STC_MARKNUM_FOLDEROPEN, wx.BLACK )
        self.m_scintilla1.MarkerSetForeground( wx.stc.STC_MARKNUM_FOLDEROPEN, wx.WHITE )
        self.m_scintilla1.MarkerDefine( wx.stc.STC_MARKNUM_FOLDERSUB, wx.stc.STC_MARK_EMPTY )
        self.m_scintilla1.MarkerDefine( wx.stc.STC_MARKNUM_FOLDEREND, wx.stc.STC_MARK_BOXPLUS )
        self.m_scintilla1.MarkerSetBackground( wx.stc.STC_MARKNUM_FOLDEREND, wx.BLACK )
        self.m_scintilla1.MarkerSetForeground( wx.stc.STC_MARKNUM_FOLDEREND, wx.WHITE )
        self.m_scintilla1.MarkerDefine( wx.stc.STC_MARKNUM_FOLDEROPENMID, wx.stc.STC_MARK_BOXMINUS )
        self.m_scintilla1.MarkerSetBackground( wx.stc.STC_MARKNUM_FOLDEROPENMID, wx.BLACK)
        self.m_scintilla1.MarkerSetForeground( wx.stc.STC_MARKNUM_FOLDEROPENMID, wx.WHITE)
        self.m_scintilla1.MarkerDefine( wx.stc.STC_MARKNUM_FOLDERMIDTAIL, wx.stc.STC_MARK_EMPTY )
        self.m_scintilla1.MarkerDefine( wx.stc.STC_MARKNUM_FOLDERTAIL, wx.stc.STC_MARK_EMPTY )
        self.m_scintilla1.SetSelBackground( True, wx.SystemSettings.GetColour(wx.SYS_COLOUR_HIGHLIGHT ) )
        self.m_scintilla1.SetSelForeground( True, wx.SystemSettings.GetColour(wx.SYS_COLOUR_HIGHLIGHTTEXT ) )
        bSizer8.Add( self.m_scintilla1, 1, wx.EXPAND |wx.ALL, 5 )


        self.m_panel4.SetSizer( bSizer8 )
        self.m_panel4.Layout()
        bSizer8.Fit( self.m_panel4 )
        self.m_notebook3.AddPage( self.m_panel4, _(u"Figure 1"), False )

        bSizer6.Add( self.m_notebook3, 1, wx.EXPAND |wx.ALL, 5 )


        self.m_panel1.SetSizer( bSizer6 )
        self.m_panel1.Layout()
        bSizer6.Fit( self.m_panel1 )
        self.m_panel2 = wx.Panel( self.m_splitter1, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
        bSizer7 = wx.BoxSizer( wx.VERTICAL )

        self.m_panel3 = wx.Panel( self.m_panel2, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.TAB_TRAVERSAL )
        bSizer9 = wx.BoxSizer( wx.VERTICAL )

        self.m_bitmap1 = wx.StaticBitmap( self.m_panel3, wx.ID_ANY, wx.NullBitmap, wx.DefaultPosition, wx.DefaultSize, 0 )
        bSizer9.Add( self.m_bitmap1, 1, wx.ALL|wx.EXPAND, 5 )


        self.m_panel3.SetSizer( bSizer9 )
        self.m_panel3.Layout()
        bSizer9.Fit( self.m_panel3 )
        bSizer7.Add( self.m_panel3, 1, wx.EXPAND |wx.ALL, 5 )


        self.m_panel2.SetSizer( bSizer7 )
        self.m_panel2.Layout()
        bSizer7.Fit( self.m_panel2 )
        self.m_splitter1.SplitVertically( self.m_panel1, self.m_panel2, 543 )
        bSizer5.Add( self.m_splitter1, 1, wx.EXPAND, 5 )


        bSizer1.Add( bSizer5, 1, wx.EXPAND, 5 )


        self.SetSizer( bSizer1 )
        self.Layout()
        self.m_statusBar1 = self.CreateStatusBar( 1, wx.STB_SIZEGRIP, wx.ID_ANY )

        self.Centre( wx.BOTH )

    def __del__( self ):
        pass

    def m_splitter1OnIdle( self, event ):
        self.m_splitter1.SetSashPosition( 543 )
        self.m_splitter1.Unbind( wx.EVT_IDLE )


