# Fraktale

Programm, mit dem Fraktale gezeichnet werden können.

### Build

Zum Erstellen des Programms muss in der Datei 'CMakeLists.txt'
die Zeile `set (CMAKE_PREFIX_PATH "../Qt/5.8/gcc_64/lib/cmake")`
angepasst werden, so dass der Pfad zur Qt-Bibliothek angegeben wird.

Danach kann das Programm mit folgenden Befehlen erstellt werden.
```
mkdir build
cd build
cmake ../
make
```