/**
 * @class ap::Thread
 *
 * @brief Clase Thread para la aplicación *qt-mca*
 *
 * Esta clase provee métodos y propiedades para el manejo de hilos de procesamiento en
 * segundo plano durante la ejecución de la aplicación *qt-mca*. Principalmente se utiliza
 * para el _logging_ de tasa y temperatura de cada cámara gamma.
 *
 * @note Clase heredada de QObject
 *
 * @author Ariel Hernández
 *
 * @version $Version
 *
 * Contacto: ahernandez@cae.cnea.gov.ar
 *           ariel.h.estevenz@ieee.org
 *
 */
#ifndef APTHREAD_H
#define APTHREAD_H

#include "apMCAE.hpp"
#include <QTimer>
#include <QMutex>
#include <QEventLoop>
#include <QThread>
#include <QDebug>
#include <QObject>
#include "qdatetime.h"

#define     UPDATED     0
#define     UPDATEABLE  1
#define     OFFLINE     2
#define     ERROR_SERVER 3
#define ADQ_POR_TAMANO 0
#define ADQ_POR_TIEMPO 1
#define ADQ_TIEMPO_INTERMEDIO 2


namespace ap {

  class Thread : public QObject
  {
      Q_OBJECT

  public:
      explicit Thread(shared_ptr<MCAE> _arpet, QMutex* _mutex ,QObject *parent = 0);
      void requestLog();
      void requestMCA();
      int GitUpdater();
      void abort();
      string getLocalDateAndTime();



  private:
      shared_ptr<MCAE> arpet;
      QList<int> checkedHeads;
      QList<QString> pmt_selected_list;
      bool _abort;
      bool _logging;
      bool _mode;
      bool _mca;
      bool _centroid;
      bool _CabCalib;
      QMutex* mutex;
      QString port_name;
      bool temp;
      bool rate;
      bool debug;
      int time_sec;
      bool log_finished;
      bool Timer_concluded=true;
      QString etime;
      QStringList commands;
      int cantidad_archivos;
      QString time;
      QString date;

      bool running;

  private:    //funciones privadas
      bool Grabar_FPGA();
      bool Adquirir();
      bool MoveToServer();
      int GitFetchChecker();


  signals:

      /**
       * @brief logRequested
       */
      void logRequested();

      /**
       * @brief finished
       */
      void finished();
      /**
       * @brief finishedElapsedTime
       * @param var
       */
      void finishedElapsedTime(bool var);
      /**
       * @brief sendLogErrorCommand
       */
      void sendLogErrorCommand();
      /**
       * @brief sendMCAErrorCommand
       */
      void sendMCAErrorCommand();
      /**
       * @brief startElapsedTime
       */
      void startElapsedTime();
      /**
       * @brief sendElapsedTimeString
       * @param eatime_string
       */
      void sendElapsedTimeString(QString eatime_string);
      /**
       * @brief sendFinalElapsedTimeString
       * @param eatime_string
       */
      void sendFinalElapsedTimeString(QString eatime_string);


  public slots:
      void setAbortBool(bool abort);
      void setCentroidMode(bool mode) { _centroid = mode; }
      void cancelLogging(bool var) { log_finished = var; }
      void receivedFinalElapsedTimeString(QString eatime_string) { etime = eatime_string; }
      void setCommands(QStringList Command){commands=(Command);}
      void setCantArchivos(int cant){cantidad_archivos=cant;}
      void getElapsedTime();
      void TimerUpdate();

  public:
      /**
       * @brief restoreLoggingVariable
       */
      void restoreLoggingVariable() { log_finished = false; }

      /**
       * @brief setPortName
       * @param port
       */
      void setPortName(QString port) {port_name = port;}

      /**
       * @brief setDebugMode
       * @param _debug
       */
      void setDebugMode(bool _debug) {debug = _debug;}
      /**
       * @brief setTimeBetweenLogs
       * @param _sec
       */
      void setTimeBetweenLogs(int _sec) {time_sec = _sec;}

      bool getrunning(){return running;}

  };

}

#endif // APTHREAD_H
