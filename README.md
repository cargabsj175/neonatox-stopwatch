# Cronómetro NeonatoX ⏱️

Un **cronómetro simple y elegante** desarrollado en **C** usando **GTK 4**, diseñado para ser rápido, minimalista y fácil de usar.  
Incluye funciones de inicio, pausa, reinicio y **registro de marcas (laps)**, además de un pequeño menú con opción de "Acerca de".

---

## 🚀 Características

- Interfaz gráfica moderna usando **GTK 4.0+**
- Precisión en milisegundos (`mm:ss.mmm`)
- Botones de:
  - **Iniciar** (Start)
  - **Detener** (Stop)
  - **Marca** (Lap) – hasta 100 marcas
  - **Reiniciar** (Reset)
- Lista de marcas desplazable (scrollable)
- Menú superior con:
  - Información "Acerca de"
  - Salir de la aplicación
- Código limpio y modular en C

---

## 📷 Captura de Pantalla

*(Agrega aquí una imagen de la aplicación cuando tengas una)*

![Captura del Cronómetro](screenshot.png)

---

## 🛠️ Dependencias

Asegúrate de tener **GTK 4** instalado en tu sistema.  
En distribuciones basadas en Debian/Ubuntu:

```bash
sudo apt install libgtk-4-dev build-essential
```

En Arch Linux / Manjaro:

```bash
sudo pacman -S gtk4 base-devel
```

---

## 📦 Compilación

### 🔧 Usando Meson (Recomendado)

```bash
meson setup builddir
meson compile -C builddir
./builddir/neonatox-stopwatch
```

### 🖥️ Compilación Manual (opcional)

```bash
gcc src/main.c -o neonatox-stopwatch `pkg-config --cflags --libs gtk4`
```

Esto generará el ejecutable `neonatox-stopwatch`.

---

## ▶️ Uso

Ejecuta en terminal:

```bash
./neonatox-stopwatch
```

Se abrirá la ventana principal del cronómetro.

---

## 🗂️ Estructura del Proyecto

```
├── src/
│   └── main.c       # Código principal del cronómetro
├── meson.build      # Script de configuración de Meson
├── README.md        # Este archivo
└── data/
    ├── neonatox-stopwatch.svg     # Icono
    └── neonatox-stopwatch.desktop # Lanzador
```

---

## 📄 Licencia

Este proyecto está licenciado bajo la **GNU General Public License v3.0** – puedes usarlo, modificarlo y distribuirlo libremente.  

---

## 👨‍💻 Autor

Desarrollado por **Equipo NeonatoX**  
🌐 [https://neonatox.github.io](https://neonatox.github.io)
