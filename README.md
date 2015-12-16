# tarea3-algoritmos
#### Nicolás Salas V.

Esta es una tarea para el curso CC4102 - Diseño y Análisis de Algoritmos dictado por el profesor Gonzalo Navarro B.
en el Departamento de Ciencias de la Computación de la U. de Chile.

Esta tarea trata sobre comparar 3 estructuras de datos que implementan diccionarios:
- Árbol AVL
- Splay Trees
- Árbol de Búsqueda Binaria corriente

Se proveen todos los sources de los experimentos dentro de la carpeta `src` y un archivo `Makefile` para compilar la tarea.

Para compilar entre al directorio `src` y ejecute el comando

```
$ make
```

**Nota**: Este modo de compilación genera *warnings* por funciones que no tienen uso en `vanemdeboas.c`. Se
pide ignorar dichos warnings, pues en este experimento no se ha incluido los árboles de van emde boas.

Luego simplemente corra la tarea con el comando:

```
$ ./tarea3
```

Esta tarea genera un montón de archivos `.dat`, que mantienen los datos que fueron usados en la elaboración del
informe que se entrega junto con el código. En el entregable están los archivos con los datos usados para
la elaboración de los gráficos en `gnuplot` bajo la carpeta `graficos`, la que incluye, además, un script con
input para gnuplot que permite generar automáticamente las imagenes. **Nota**: Se requiere Gnuplot 5.0.

Aunque escapa al curso, también se proveen tests, que pueden ser usados con `valgrind`, para verificar que las mediciones de ocupación de los espacios de las estructuras son verídicos.

Este README sólo sirve para Linux. En caso de usar cualquier otro sistema operativo se deja al lector la tarea de
investigar cómo compilar el programa.

La tarea original involucraba Árboles de van Emde Boas, los que en esta tarea no se han hecho, ello también
se indica en el informe, con una sección agregada de análisis y opinión sobre esta estructura y su uso real.

### DISCLAIMER
Este repositorio está alojado en *github*, pero de forma **privada**. Se liberará al público en el momento que termine el plazo para entregar la tarea. Si de alguna forma existiese alguien que obtuviese acceso al repositorio, esta **explícitamente** prohibido usar este código para la entrega de la Tarea 3 del Curso Diseño y Análisis de Algoritmos de la Universidad de Chile dictado en el semestre de primavera 2015, por el profesor Gonzalo Navarro. Cualquier otro uso que se le quiera dar a este código está permitido.
