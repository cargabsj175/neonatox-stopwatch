project('neonatox-stopwatch', 'c',
  version: '0.1.0',
  default_options: ['warning_level=3', 'c_std=c11'])

# Dependencias
gtk_dep = dependency('gtk4', version: '>=4.0')

# Ejecutable
app_executable = executable('neonatox-stopwatch',
  'src/main.c',
  dependencies: gtk_dep,
  install: true
)

# Instalar .desktop file
install_data('data/neonatox-stopwatch.desktop',
  install_dir: '/usr/share/applications',
  rename: 'neonatox-stopwatch.desktop'
)

# Instalar ícono (64x64)
install_data('data/neonatox-stopwatch.svg',
  install_dir: '/usr/share/icons/hicolor/64x64/apps',
  rename: 'neonatox-stopwatch.svg'
)
