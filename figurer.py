import wx
import gui.forms

if __name__ == "__main__":
  app = wx.App()
  frame = gui.forms.MainFrame(None)
  icon = wx.EmptyIcon()
  icon.CopyFromBitmap(wx.Bitmap("icon/icon.ico", wx.BITMAP_TYPE_ANY))
  frame.SetIcon(icon)
  frame.Show()
  app.MainLoop()