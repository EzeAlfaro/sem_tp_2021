
## Guia de armado de mini tp semáforos

Se necesita armar un programa en C, con semáforos e hilos, que simula una competencia entre 4 equipos, el ganador es quien haga el primer sanguche de milanesa completo;



---



## Nuestras precondiciones 



*   4 equipos de cocineros (hilos)
*   gana el primero que entrega un sanguche de mila
*   solo existe una sartén con capacidad de una sola milanesa
*   solo existe un horno que cocina dos panes a la vez
*   solo existe un salero
*   leer receta desde archivo externo
*   generar log 



---



## Pasos para armar un sanguche


## Para la mila



1. _Cortar (2 dientes de ajo y perejil)_
2. _mezclar 1 huevo_
3. _Salar * usar salero_
4. _agregar carne_
5. _rebozar mila_
6. _fritar  * usar sartén (cocinar 5 minutos)_


## Para el pan



7. _hornear Pan * usar horno (10 min)_


## para el acompañamiento



8. _preparar lechuga, tomate y pepino_


##  para el armado



9. _armar sanguche_

**_*  tienen que usar elementos compartidos_**


## 

---



## Pseudocódigo

Teniendo en cuenta de que en él enunciado nos avisa qué algunas tareas se pueden paralelizar y la necesidad de usar hilos y semáforos vamos a organizar nuestro pseudocódigo de la siguiente manera:

Sabemos que existen elementos compartidos en la competencia, estos serán en nuestro código, **semáforos globales**

_sem_salero()_

_sem_sarten()_

_sem_horno(2) _(Está iniciado en dos para permitir el doble uso del horno)

Además de estos, existirá un grupo de semáforos exclusivos de cada grupo, el siguiente orden está organizado para aprovechar al máximo la paralelización de tareas:



1. **_Tarea1- milanesa_**
*   _cortar() (2 dientes de ajo y perejil)_
*   _mezclar()_
*   _salar() * usar salero_
*   _agregarCarne()_
*   _rebozarMila()_
*   _fritar () * usar sartén (cocinar 5 minutos)_
*   _habilitar sem_milanesaLista()_
2. **_Tarea2 - pan_**
*   _hornearPan() * usar horno (10 min)_
*   _habilitar sem_panListo()_
3. **_Tarea3 - extras_**
*   _cortarExtras()_
*   _habilitar extrasListo()_

Realizadas estas 3 tareas se procede a armar el sandwich siempre utilizando **semaforos**.


## Evolución del tp y problemas encontrados

Una vez organizado el pseudocódigo, se comenzó a modificar el código base, fácilmente se crearon los 4 hilos y su ejecución fue un éxito, al adaptarse al código de ejemplo fueron surgiendo problemas mínimos que fueron resueltos rápidamente, como por ejemplo la falta de creación de un hilo, entre otros errores minimos.

Para una implementación más rápida, se eligió proceder con la implementación del código sin la interacción de los semáforos globales, ni las interacciones con archivos externos, una vez que se verificó el correcto funcionamiento, se procedió a generar la intervención por medio de los semáforos globales, su implementación fue sencilla y muy similar a los anteriores.


## Bibliografía

[https://parzibyte.me/blog/2019/10/16/escribir-archivo-con-c-usando-fprintf/](https://parzibyte.me/blog/2019/10/16/escribir-archivo-con-c-usando-fprintf/)
