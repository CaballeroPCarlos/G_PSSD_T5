# Proyecto: Elaborar un algoritmo distribuido en C

El objetivo del proyecto es desarrollar programas en C que permitan simular un **consenso distribuido** sobre cualquier tema aplicable.  

Uno de los programas actuará como **servidor (coordinador)**, mientras que el otro será el **cliente (participante)**. Cada participante podrá votar **COMMIT** o **ABORT**, y el consenso dependerá de que **todos los participantes voten COMMIT** para que la transacción se confirme.

---

## 🧰 Requisitos

### 1. Descargar herramientas necesarias

- **Editor de código:** [Visual Studio Code](https://code.visualstudio.com/)  
- **Compilador de C:** [MinGW (TDM-GCC)](https://jmeubank.github.io/tdm-gcc)

> 💡 Este trabajo se realizó en **Windows**.

**Fuente de referencia:**  
- [Instalar y configurar Visual Studio Code para programar en C/C++ en Windows](https://youtu.be/qQT-6WufAEE?si=226KOMnVIcc3iIuL)  
- Canal: [Sevivon Studio](https://www.youtube.com/@SevivonStudio)

---

### 2. Verificar la instalación del compilador

Después de instalar **MinGW**, abre **PowerShell**  
(puedes hacerlo con **Shift + clic derecho** en cualquier carpeta → “Abrir PowerShell aquí”) y ejecuta:

```bash
gcc --version
```

Si aparece la versión de GCC (por ejemplo, `gcc (tdm64-1) 10.3.0`), la instalación fue exitosa.

---

### 3. Extensiones útiles en Visual Studio Code

Dentro de VS Code, instala las siguientes extensiones desde el panel lateral (ícono de bloques):

* **C/C++ (de Microsoft):** autocompletado, resaltado de sintaxis y depuración.
* **Code Runner:** permite ejecutar programas fácilmente.
* **C/C++ Compile Run:** facilita la compilación y ejecución rápida de archivos C/C++.

---

## ▶️ Ejecución de los proyectos `.c`

### Archivos principales

* `coordinator.c`: actuará como **servidor**, detectando el ingreso de participantes y recibiendo sus votos.
* `participant.c`: se usarán **tres instancias** de este programa para simular tres participantes.

> Cada programa se ejecutará en su propia terminal, por lo que necesitarás **cuatro terminales**: uno para el coordinador y tres para los participantes.

---

### Compilación

Dentro de la carpeta donde se encuentra el proyecto, abre la terminal o PowerShell (**Shift + clic derecho**) o utiliza la terminal integrada de **VS Code** (**Ctrl + Ñ**).

Compila los programas con los siguientes comandos:

```bash
gcc -o coordinator coordinator.c
gcc -o participant participant.c
```

Explicación:

* `gcc`: invoca el compilador de C.
* `-o coordinator`: indica que el programa compilado se llamará **coordinator.exe**.
* `coordinator.c`: es el archivo fuente que estás compilando.

---

### Ejecución

1. Primero, ejecuta el coordinador:

```bash
./coordinator
```

2. Luego, ejecuta **tres instancias del participante**, cada una en su propia terminal:

```bash
./participant
```

> Cada participante votará **1 (COMMIT)** o **0 (ABORT)** de forma manual. La decisión final se mostrará en todas las terminales según el consenso global.
