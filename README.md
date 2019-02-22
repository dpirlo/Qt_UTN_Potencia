# QT UTN Potencia

[![N|Solid](https://www.frba.utn.edu.ar/wp-content/uploads/2017/10/Logos-UTN.BA-cs6-transparente.png)](https://www.frba.utn.edu.ar/)

[![Build Status](https://travis-ci.org/joemccann/dillinger.svg?branch=master)](https://github.com/dpirlo/Qt_UTN_Potencia/tree/master)

Programa de manejo y monitoreo de variables de tensión y corriente para el uso académico de la cátedra de Electrónica de Potencia UTN-FRBA.

  - Controla el valor de disparo de los tiristores
  - Monitorea tensión y corriente bajo demanda
  - Guarda imagenes de graficos en computadora
  - Se conecta mediante bluetooth con el uC utilizado

# New Features!

  - Guardado de gráficos, calculo de potencias y tensiones eficaces 
  - Utilización de la herramienta [Snapcraft] para el modo usuario final (sin necesidad de instalar [QtCreator]) que garantiza la compilación independientemente del Linux utilizado 

>Dicho proyecto representa un proyecto anual 
>en conjunto con todo el grupo de la cátedra de 
>Miquel del año 2018. El software en cuestión 
>fue realizado por [Damián Pirlo] desarrollado 
> en [QtCreator]



### Installation

Este proyecto requiere conección a internet para que [Snapcraft] descargue las librerías necesarias, en su defecto se requerirá tener instalado el [QtCreator].

Instalación del ejecutable sin entorno de desarrollo.

```sh
$ cd Qt_UTN_Potencia
$ snapcraft
```

Limpieza del los archivos de instalación.

```sh
$ snapcraft clean
```

### Desarrollo

Querés seguir con el proyecto? 

Crea un fork desde el link a continuación [Repositorio-QT-UTN-Potencia]

#### Entorno de desarrollo
Para continuar con el desarrollo:
```sh
$ cd Qt_UTN_Potencia 
$ qtcreator qt_mca.pro
```
Compilado del programa sin abrir el QtCreator:
```sh
$ cd Qt_UTN_Potencia
$ qmake
$ make
```



### Todos

 - Faltan modos que quedaron en construcción para las generaciones venideras

License
----

UTN-FRBA


**Free Software, Hell Yeah!**

[//]: # (These are reference links used in the body of this note and get stripped out when the markdown processor does its job. There is no need to format nicely because it shouldn't be seen. Thanks SO - http://stackoverflow.com/questions/4823468/store-comments-in-markdown-syntax)

   [Damián Pirlo]: <https://www.facebook.com/dpirlo>
   [git-repo-url]: <https://github.com/dpirlo/Qt_UTN_Potencia.git>
   [Snapcraft]: <https://snapcraft.io/>
   [QtCreator]: <https://www.qt.io/download>
   [Repositorio-QT-UTN-Potencia]: <https://github.com/dpirlo/Qt_UTN_Potencia/tree/master>


