from gui.forms import MainFrame as MainFrame_
import wx
from src.project import ProjectFile


class MainFrame(MainFrame_):
  def __init__(self, parent):
    super().__init__(parent)
    self.__project:ProjectFile = None

    self.icon = wx.Icon()
    self.icon.CopyFromBitmap(wx.Bitmap("icon/icon.ico", wx.BITMAP_TYPE_ANY))
    self.SetIcon(self.icon)

    self.on_change_project_open()

    self.open_close_button.Bind(wx.EVT_BUTTON, self.on_open_close_button)
    self.tab_panels = []
  
  @property
  def project(self, ):
    return self.__project
  
  @project.setter
  def project(self, value):
    self.__project = value
    self.on_change_project_open()
  
  @property
  def projectIsOpen(self,):
    return (self.project != None)

  def on_open_close_button(self, e):
    match self.open_close_button.Label:
      case "Close":
        self.project = None
      case "Open":
        with wx.FileDialog(self, "Open Figurer project file", wildcard="Figurer project (*.py)|*.py",
                          style=wx.FD_OPEN | wx.FD_FILE_MUST_EXIST) as fileDialog:

            if fileDialog.ShowModal() == wx.ID_CANCEL:
                return     # the user changed their mind
            pathname = fileDialog.GetPath()
            try:
              self.project = ProjectFile(pathname)
            except Exception as e:
              wx.MessageBox(f"Error: {e}", wx.OK | wx.ICON_ERROR)

  def on_change_project_open(self,):
    projectIsOpen = self.projectIsOpen
    self.new_button.Enable(not projectIsOpen)
    self.save_button.Enable(projectIsOpen)
    self.rescan_button.Enable(projectIsOpen)

    if projectIsOpen:
      self.open_close_button.Label = "Close"
    else:
      self.open_close_button.Label = "Open"

    self.tabs.Enable(projectIsOpen)
    self.repopulate_tabs()
  
  def repopulate_tabs(self, ):
    #while self.tabs.GetPageCount() >0:
      #self.tabs.DeletePage(0)
    if self.projectIsOpen:
      for id, code in self.project.figures.items():
        self.tab_panels.append(wx.Panel(self.tabs))