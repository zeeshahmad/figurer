# figurer
Editor for quick formatting of figures made using `matplotlib`.

Note: prototype in yet progress, it's not built yet. Written in C++ using Qt6 (with QT Creator for the layout of gui elements). Contributions would be welcome!

This is a small editor that scans a given latex document (e.g. academic paper) for `\includegraphics` commands (e.g. imported `matplotlib` figures) using [TexSoup](https://github.com/alvinwan/texsoup), then generates an associated json file that organises python code for corresponding figures. The main feature is automatic running of python code (using [pybind11](https://github.com/pybind/pybind11)) to update figure live as the python code is typed/modified. 

## Possible features:
- Separate loading of plot data, and plotting. The python code that loads data could run every time the data file is updated. The python code that plots could run separately many times after that. This would focus on figure editing.
- Possibly implement scanning of word docx documents too

I'm new to QT, so any comments on the design pattern of the code are welcome :)
