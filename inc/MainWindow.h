/**
 * @class MainWindow
 *
 * @brief Clase de entorno gráfico
 *
 * Esta clase corresponde al entorno gráfico de la aplicación *qt-utn*
 *
 *
 * @note Utiliza la clase MCAE para la comunicación con el ComBT.
 *
 * @note Clase heredada de QMainWindow
 *
 * @author Damián Pirlo
 *
 * @version $Version
 *

 *
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QMessageBox"
#include "qcustomplot.h"
#include "SetPreferences.h"
#include "apMCAE.hpp"
#include "apThread.hpp"
#include <QThread>
#include <cstdio>
#include <QString>
#include <QPixmap>
#include <QTextStream>
#include "SetPMTs.h"
#include <inc/QRoundProgressBar.h>

#include "ui_MainWindow.h"


#define TIEMPOS_NULOS_PMTS 0
#define MED_POR_CICLO 320
#define CHAR_LF 0x0A

#define WAIT_MICROSECONDS 1000000
#define DEV_PATH "/dev/"

#define icon_notok      ":/qss_icons/rc/ic_cancel.png"
#define icon_ok         ":/qss_icons/rc/ic_check_circle.png"
#define icon_loading    ":/qss_icons/rc/cargando2.gif"
#define CANTIDAD_ELEMENTOS_PLANAR 97
#define CANTIDAD_ELEMENTOS_COINCIDENCIA 1


using namespace ap;

/**
    El Namespace Ui contiene los métodos y propiedades del entorno gráfico de la aplicación qt-utn.
*/
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    /**
     * @brief The temp_code enum
     *
     * Definición de enum utilizado para las escalas de temperatura.
     *
     */
    enum temp_code {TOO_HOT,HOT,WARM,NORMAL,ERROR,NO_VALUE};

public:
    explicit MainWindow(QWidget *parent = 0);
    void checkCombosStatus();
    ~MainWindow();
    //void resetHitsValues(QString head);
    bool fileExists(QString path);
    SetPMTs *pmt_select;
    /* Area de prueba/testing */

private slots:
    /* Slots de sincronización para QCustomPlot */

    void mousePressGraphsLog();
    void selectionChangedLogGraph();

    /* Threads */
    void CargoTemaOscuro();
    void TimerUpdate();

    /* Slots de sincronización en el entorno gráfico */

    /* Buttons */
    void on_pushButton_send_terminal_clicked();
    void on_pushButton_flush_terminal_clicked();
    void on_pushButton_clear_terminal_clicked();

    /* AutoCalib */


    /* CUIPET */

    /* Buttons de prueba/testing */

    void on_pushButton_p_51_clicked();

    void updateCaption();

    void on_pushButton_send_terminal_2_clicked();

    void on_pushButton_2_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_comboBox_3_currentIndexChanged(int index);


    void on_pb_enviar_conf_clicked();
    
private:
    QString openConfigurationFile();
    QString openLogFile();
    QString openDirectory();
    QString getLogFileName(QString main="");
    bool copyRecursively(const QString &srcFilePath,const QString &tgtFilePath);
    size_t sendString(string msg, string end);
    string readString(char delimeter='\r');
    string getLocalDateAndTime();
    int getPMT(QLineEdit *line_edit);
    int Adq_zone;
    int parseConfigurationFile(bool mode, QString head="");
    int writePreferencesFile(QString pref, QString filename, bool force=false);
    temp_code getTemperatureCode(double temperature);
    QVector<int> getCustomPlotParameters();
    QVector<double> getValuesFromFiles(QString filename, bool hv=false);
    QStringList getLogFromFiles(QString filename, QRegExp rx, string parser);
    QStringList availablePortsName();
    void writeFooterAndHeaderDebug(bool header);
    void writeLogFile(QString log_text, QString main="");
    void writeDebugToStdOutLogFile(QString main="");
    void SetQCustomPlotConfiguration(QCustomPlot *graph, int channels);
    void SetQCustomPlotSlots(string title_pmt_str="", string title_head_str="");
    void getElapsedTime();
    void getHeadStatus(int head_index);
    void setInitialConfigurations();
    void setPreferencesConfiguration();
    void getPreferencesSettingsFile();
    void setPreferencesSettingsFile(QString folder, QString variable, QString value);
    void connectSlots();
    void getPaths();
    void setPMTCustomPlotEnvironment(QList<QString> qlist);
    void setPMTCalibCustomPlotEnvironment(QList<int> qlist);
    void setHeadCustomPlotEnvironment();
signals:


private:
    Ui::MainWindow *ui;
    SetPreferences *pref;
    shared_ptr<MCAE> ComBT;
    QMutex mMutex, bMutex, Mutex_adq,Mutex_copy,Mutex_git;
    QThread *thread;
    Thread *worker;
    QThread *etime_th;
    Thread *etime_wr;
    QThread *mcae_th;
    QString port_name;
    Thread *mcae_wr;
    bool is_abort_mcae, is_abort_log, is_abort_calib, is_moving;
    QString initfile, root_config_path, root_calib_path,root_log_path, preferencesdir, preferencesfile;
    QList<QComboBox*> heads_coin_table;
    QVector<string> logTemp_values,logRate_values;
    bool debug;
    bool init;
    bool log;
    bool stdout_mode;

    QTimer *timer = new QTimer(0);
    int  ogl_flag;
    int offset_MEM  = 0;

    QMovie *movie_cargando = new QMovie(icon_loading);
    bool debug_calib= false;
    QStringList commands_calib;
    QString ruta_log_adquisicion="";
    QString ruta_archivo_adquisicion="";

    QGraphicsScene *scene;
    /* Area de prueba/testing */

public:
    /**
     * @brief getPreferencesDir
     */
    QString getPreferencesDir() const { return preferencesdir; }
    /**
     * @brief setInitFileConfigPath
     * @param file
     */
    void setInitFileConfigPath(QString file) { initfile = file; }
    /**
     * @brief setCalibDirectoryPath
     * @param path
     */
    void setCalibDirectoryPath(QString path) { root_calib_path = path; }
    /**
     * @brief setDebugMode
     *
     * Configura el valor de _debug_ a partir del menú preferencias.
     *
     * @param mode
     */
    void setDebugMode(bool mode) { debug = mode; }
    /**
     * @brief setLogMode
     *
     * Configura el valor de _log_ a partir del menú preferencias.
     *
     * @param mode
     */
    void setLogMode(bool mode) { log = mode; }
    /**
     * @brief setStdOutDebugMode
     *
     * Configura el valor de _stdout\_mode_ a partir del menú preferencias.
     *
     * @param mode
     */
    void setStdOutDebugMode(bool mode) { stdout_mode = mode; }


    void setCommandsAdquire();
    QString get_root_log_path(){return root_log_path;}
};

#endif // MAINWINDOW_H
