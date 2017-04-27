# Fraktale

Programm, mit dem Fraktale gezeichnet werden können.

Zur Zeit gibt es drei Fraktale zum Anschauen:
- Koch Schneeflocke
- Mandelbrot-Menge
- Julia-Menge

Für weitere Hilfe zur Navigation und Benutzerfläche bitte die im Programm 
angefügte Hilfe einsehen.

### Das Programm erstellen

Das Programm mit folgenden Befehlen erstellt werden:
```
mkdir build
cd build
cmake ../
make
```

Sollte sich die Qt-Bibliothek nicht in einem Standard-Pfad befinden, 
kann mit dem cmake-Parameter  -DCMAKE_PREFIX_PATH="pfad/zu/qt/lib/cmake" 
der Pfad zur Qt-Biblothek angegeben werden.
