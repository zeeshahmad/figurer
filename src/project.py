import rich
import inspect
import importlib.util as iu
import ast

class ProjectFile:
  def __init__(self, path:str) -> None:
    self.path = path
    self.meta = None
    self.figures = None
    self.parse()
  
  def __extract_function_name(self, source_code):
    tree = ast.parse(source_code)
    for node in ast.walk(tree):
        if isinstance(node, ast.FunctionDef):  
            return node.name  

  def parse(self,):
    spec = iu.spec_from_file_location("figures", self.path)
    module = iu.module_from_spec(spec)
    spec.loader.exec_module(module)
    rich.print(module)
    rich.print(module.figures)
    self.meta = module.meta
    self.figures= {}
    for id, func in module.figures.items():
      self.figures[id] = inspect.getsource(func)


  def save(self,):
    locals_to_save = {
      'meta': self.meta,
    }
    rich.print(self.meta)
    temppath = R"C:\Users\zeeshan\figurer\example-project\figures2.py"
    with open(temppath, 'w') as file:
      for key, value in locals_to_save.items():
        if isinstance(value, str):
          value_repr = f"'{value}"
        else:
          value_repr = repr(value)
        file.write(f"{key} = {value_repr}\n\n")
      for key, value in self.figures.items():
        file.write(f"{value}\n")
      file.write("figures = {\n")
      for key, value in self.figures.items():
        func_name = self.__extract_function_name(value)
        file.write(f"  '{key}': {func_name},\n")
      file.write("}\n")

if __name__ == "__main__":
  proj = ProjectFile(R"C:\Users\zeeshan\figurer\example-project\figures.py")
  proj.save()