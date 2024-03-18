# Biblioteca de Sincronización
La Biblioteca de Sincronización proporciona una implementación de semáforos, monitores y barreras para la programación concurrente y paralela en C, permitiendo evitar condiciones de carrera al trabajar sobre regiones críticas. Se utiliza el **mutex** y **cond** de la biblioteca *pthread* para manejar los bloqueos y desbloqueos de los mutex y las esperas y notificaciones de los hilos.

## Tabla de Contenidos
- [Prerrequisitos](#prerrequisitos)
- [Instalación](#instalación)
- [Uso](#uso)
- 
## Prerrequisitos
Probado con:
-Fedora Workstation 39
-gcc (GCC) 13.2.1 20231205 (Red Hat 13.2.1-6)

## Instalación
1. Clone el repositorio.
2. Realice un include de **sync.h** respetando la ruta en la que se encuentra el repositorio. O copie los archivos **sync.c** y **sync.h** a su proyecto y realice #include "sync.h".

## Uso
Posterior al include de **sync.h** en su archivo fuente, inicialice las estructuras de sincronización
### Semáforo
Haga la declaración de una variable de tipo *semaphore*. Ej:  Semaphore sem;
Invoque el método *init_semaphore*, este recibe como parámetros una referencia a la variable del semáforo y un número entero que corresponde al contador del semáforo. Dentro de este se inicializa el mutex, cond y contador del semáforo con los métodos respectivos de pthread.
Utilice el método *wait_semaphore* cuando vaya a empezar una región crítica
