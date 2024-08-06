from cx_Freeze import setup, Executable

build_options = {'packages': [], 'excludes': []}
version = "0.1"


msi_data = {
}

bdist_msi_options = {
  'data': msi_data,
  'install_icon': 'icon/icon.ico',
  'upgrade_code': "{51d33540-f9a0-41e2-8c92-e0f30ce2d6ea}"
}

base = 'gui'

executables = [
    Executable('figurer.py', base=base, target_name = 'Figurer', icon=R"icon/icon.ico")
]

setup(name='Figurer',
      version = version,
      description = 'Live figure editing',
      options = {'build_exe': build_options, 'bdist_msi': bdist_msi_options},
      executables = executables,
      )