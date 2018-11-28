/**
 * @class ap::Exceptions
 *
 * @brief Clase de manejo de excepciones
 *
 * Esta clase provee métodos y propiedades para el
 * manejo de excepciones.
 *
 * @note Clase heredada de QException
 *
 * @author Damián Pirlo
 *
 * @version $Version
 *
 *
 */
#ifndef APEXCEPTIONS_H
#define APEXCEPTIONS_H

#include <QException>
#include <QDebug>

namespace ap {

  class Exceptions: public QException
  {
  public:
    Exceptions(const char *desc);
    const char *excdesc;
  };
}

#endif // APEXCEPTIONS_H
