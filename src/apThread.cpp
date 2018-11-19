#include "inc/apThread.hpp"

using namespace ap;

/**
 * @brief Thread::Thread
 *
 * Constructor de la clase
 *
 * Se inicializan todos los _flags_ utilizados durante la ejecución del hilo. Esta clase contiene todos los métodos y propiedades para acceder, administrar,
 * configurar los threads ejecutados en la aplicación *qt-mca*.
 *
 * Recibe como parametro un objeto de la clase MCAE para acceder a todos los dispositivos del tómografo ARPET (fotomultiplicadores, cabezales, placas de
 * alta tensión, etc) y un objeto de la clase QMutex para compartir el recurso del puerto serie.
 *
 * @note Se documentan las propiedades más importantes.
 *
 * @param _arpet
 * @param _mutex
 * @param parent
 */
Thread::Thread(shared_ptr<MCAE> _arpet, QMutex *_mutex, QObject *parent) :
    QObject(parent),
    arpet(_arpet),
    _logging(false),
    _abort(false),
    temp(false),
    rate(false),
    debug(false),
    mutex(_mutex),
    time_sec(1)
{
}
/**
 * @brief Thread::setAbortBool
 *
 * Slot de aborto del proceso
 *
 * @param abort
 */
void Thread::setAbortBool(bool abort)
{
    _abort = abort;
    if (debug) cout<<"Se aborta la operación en el thread: "<<thread()->currentThreadId()<<endl;
}
/**
 * @brief Thread::getLocalDateAndTime
 * @return date en _string_
 */
string Thread::getLocalDateAndTime()
{
    return (QDateTime::currentDateTime().toString().toStdString());
}
/**
 * @brief Thread::abort
 */
void Thread::abort()
{
    mutex->lock();
    if (_logging || _mca)
    {
        _abort = true;
        if (debug) cout<<"Se aborta la operación en el thread: "<<thread()->currentThreadId()<<endl;
    }
    mutex->unlock();
}

void Thread::TimerUpdate(){
    Timer_concluded=true;
}

/**
 * @brief Thread::getElapsedTime
 *
 * Obtiene el tiempo transcurrido de un proceso. Se finaliza con la variable
 * _booleana_ *log_finished*
 *
 */
void Thread::getElapsedTime()
{
    QTime t;
    int secs, mins, hours;
    int elapsed_time=0;
    while (!log_finished)
    {
        t.start();
        QEventLoop loop;
        QTimer::singleShot(1000, &loop, SLOT(quit()));
        loop.exec();
        elapsed_time=elapsed_time + t.elapsed();
        secs = elapsed_time / 1000;
        mins = (secs / 60) % 60;
        hours = (secs / 3600);
        secs = secs % 60;
        etime = QString("%1:%2:%3")
                .arg(hours, 2, 10, QLatin1Char('0'))
                .arg(mins, 2, 10, QLatin1Char('0'))
                .arg(secs, 2, 10, QLatin1Char('0'));
        emit sendElapsedTimeString(etime);
        emit sendFinalElapsedTimeString(etime);
    }
    restoreLoggingVariable();

    emit finished();
}
