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
Haga la declaración de una variable de tipo *Semaphore*. Ej:  Semaphore sem;

Invoque el método *init_semaphore*, este recibe como parámetros una referencia a la variable del semáforo y un número entero que corresponde al contador del semáforo.
Dentro de este se inicializa el mutex, cond y contador del semáforo con los métodos respectivos de pthread.

Utilice el método *wait_semaphore* cuando vaya a empezar una región crítica que necesite ser protegida. El método recibe una referencia del semáforo. Bloquea el mutex, verifica que aún tenga recursos a su disposición y en caso de tenerlos le resta 1 al contador y desbloquea el mutex. En caso de no tener recursos(contador en 0), genera una estructura WaitingThread, la agrega a la lista, le asigna la condición de espera y desbloquea el mutex.

Utilice el método *post_semaphore* cuando la región crítica haya finalizado. Este método recibe una referencia del semáforo, bloquea el mutex, decrementa el contador y si hay algun hilo en la lista de espera, lo libera. Después desbloquea el mutex.

Utilice el método *destroy_semaphore* para eliminar las estructuras inicializadas.

## Monitor
Haga la declaración de una variable tipo *Monitor*. Ej Monitor mon;

Invoque el método *monitor_init*, este recibe como parámetros una referencia a la variable del monitor.
Dentro de este se inicializa el mutex y cond del monitor con los métodos respectivos de pthread y una variable entera que representa si el monitor está ocupado.

Utilice el método *monitor_enter* cuando vaya a empezar una región crítica que necesite ser protegida. El método recibe una referencia del montor. Bloquea el mutex, verifica que el mutex no esté ocupado y en caso de estarlo manda un wait al hilo. En caso de no estar ocupado, cambia la variable a ocupado y desbloquea el mutex.

Utilice el método *monitor_exit* cuando la región crítica haya finalizado o se necesita forzar al hilo a salir. Este método recibe una referencia del monitor, bloquea el mutex, cambia la variable de ocupado, notifica al hilo esperando que ya puede utilizar el monitor y desbloquea el mutex.

### Barrera
Haga la declaración de una variable tipo *Barrier*. Ej Barrier bar;

Invoque el método *barrier_init*, este recibe como parámetros una referencia a la variable de la barrera.
Dentro de este se inicializa el mutex y cond de la barrera con los métodos respectivos de pthread y dos variables enteras, una que representa la cantidad de hilos que han llegado y otra que representa el número máximo de hilos que la barrera puede retener.

Utilice el método *barrier_sync* cuando vaya a empezar una región crítica que necesite ser protegida. El método recibe una referencia de la barrera. Bloquea el mutex, incrementa la variable de hilos que han llegado, verifica si ya la cantidad de hilos llegados es la misma que la cantidad de hilos máximos y en caso de serlo, restablece la variable de hilos llegados y les notifica que ya pueden continuar y desbloquea el mutex. En caso de que no se haya cumplido la cantidad máxima de hilos, se deja este en espera y se desbloquea el mutex.
