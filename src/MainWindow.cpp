#include "inc/MainWindow.h"
#include "ui_MainWindow.h"
#include "ui_SetPreferences.h"
#include "ui_SetPMTs.h"

#include "ui/validate_cal.hpp"

/**
 * @brief MainWindow::MainWindow
 *
 * Constructor de la clase
 *
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    is_abort_mcae(true),
    is_abort_calib(true),
    is_abort_log(true),
    debug(false),
    init(false),
    log(true),
    stdout_mode(false),
    initfile("/media/arpet/pet/calibraciones/03-info/cabezales/ConfigINI/config_cabs_linux.ini"),
    root_config_path("/media/arpet/pet/calibraciones/03-info/cabezales"),
    root_log_path(QDir::homePath() +"/.qt-mca/logs"),
    preferencesdir(QDir::homePath() + "/.qt-mca"),
    preferencesfile("qt-mca.conf"),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /** @todo: Ver la posibilidad de maximizar la ventana */
    this->setFixedSize(this->maximumSize());
    setPreferencesConfiguration();
    QTimer *timerw = new QTimer(this);
    connect(timerw, SIGNAL(timeout()), this, SLOT(updateCaption()));
    CargoTemaOscuro();
    timerw->start(1000);
    
    //GitCheckerStarter();
    

}
/**
 * @brief MainWindow::~MainWindow
 *
 * Destructor de la clase
 *
 */
MainWindow::~MainWindow()
{
    arpet->portDisconnect();
    worker->abort();
    thread->wait();
    delete thread;
    delete worker;

    etime_wr->abort();
    etime_th->wait();
    delete etime_th;
    delete etime_wr;

    mcae_wr->abort();
    mcae_th->wait();
    delete mcae_th;
    delete mcae_wr;

    delete pref;
    delete ui;
}
/**
 * @brief MainWindow::setPreferencesConfiguration
 */
void MainWindow::setPreferencesConfiguration()
{
    /*Configuración inicial de preferencias*/
    QString default_pref_file = "[Modo]\ndebug=false\nlog=true\nstdout=false\n[Paths]\nconf_set_file=" + initfile + "\ncalib_set_file=" + root_calib_path + "\n";
    writePreferencesFile(default_pref_file, preferencesfile);
    getPreferencesSettingsFile();
    writeDebugToStdOutLogFile();
}
/**
 * @brief MainWindow::SetQCustomPlotConfiguration
 * @param graph
 * @param channels
 */
void MainWindow::SetQCustomPlotConfiguration(QCustomPlot *graph, int channels)
{
    graph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                           QCP::iSelectLegend | QCP::iSelectPlottables);
    graph->axisRect()->setupFullAxesBox();
    graph->xAxis->setRange(0, channels);
    graph->yAxis->setRange(-5, 5);
    graph->xAxis->setLabel("Canales");
    graph->yAxis->setLabel("Cuentas");
    graph->legend->setVisible(true);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    graph->legend->setFont(legendFont);
    graph->legend->setSelectedFont(legendFont);
    graph->setNoAntialiasingOnDrag(true);
    graph->legend->setSelectableParts(QCPLegend::spItems);
    graph->plotLayout()->insertRow(0);
}
/**
 * @brief MainWindow::SetQCustomPlotSlots
 * @param title_pmt_str
 * @param title_head_str
 */
void MainWindow::SetQCustomPlotSlots(string title_pmt_str, string title_head_str)
{
    /* Slots para PMTs */

//    QCPTextElement *title_pmt = new QCPTextElement(ui->specPMTs, title_pmt_str.c_str(), QFont("sans", 16, QFont::Bold));
//    connect(ui->specPMTs, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChangedPMT()));
//    connect(ui->specPMTs, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePressPMT()));
//    connect(ui->specPMTs, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheelPMT()));
//    connect(ui->specPMTs->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->specPMTs->xAxis2, SLOT(setRange(QCPRange)));
//    connect(ui->specPMTs->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->specPMTs->yAxis2, SLOT(setRange(QCPRange)));
//    connect(ui->specPMTs, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClickPMT(QCPAxis*,QCPAxis::SelectablePart)));
//    connect(ui->specPMTs, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClickPMT(QCPLegend*,QCPAbstractLegendItem*)));
//    connect(title_pmt, SIGNAL(doubleClicked(QMouseEvent*)), this, SLOT(titleDoubleClickPMT(QMouseEvent*)));
//    ui->specPMTs->setContextMenuPolicy(Qt::CustomContextMenu);
//    connect(ui->specPMTs, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequestPMT(QPoint)));
//    ui->specPMTs->plotLayout()->addElement(0, 0, title_pmt);
    ///// Comportamientos posibles de la ventana de graficos Ver mas adelante

    connect(ui->specPMTs_3, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePressGraphsLog()));
    connect(ui->specPMTs_3, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChangedLogGraph()));

}

/**
 * @brief MainWindow::connectSlots
 */
void MainWindow::connectSlots()
{
}









/**
 * @brief MainWindow::checkStatusFPGA
 * @param status
 */


/**
 * @brief MainWindow::getLocalDateAndTime
 * @return
 */
string MainWindow::getLocalDateAndTime()
{
    return (QDateTime::currentDateTime().toString().toStdString());
}
/**
 * @brief MainWindow::getLogFileName
 * @param main
 * @return filename
 */
QString MainWindow::getLogFileName(QString main)
{
    QString suffix = QDateTime::currentDateTime().toString("yyyyMMddhh");
    QString prefix = "LOG";
    QString extension = ".log";

    return prefix + main + suffix + extension;
}
/**
 * @brief MainWindow::writeDebugToStdOutLogFile
 * @param main
 */
void MainWindow::writeDebugToStdOutLogFile(QString main)
{
    if (stdout_mode)
    {
        QString logFile = getPreferencesDir() + "/debug/Debug_" + getLogFileName(main);
        freopen(logFile.toLocal8Bit().data(), "a+", stdout);
    }
}
/**
 * @brief MainWindow::writeLogFile
 */
void MainWindow::writeLogFile(QString log_text, QString main)
{
    if (log)
    {
        QString logFile = getPreferencesDir() + "/logs/" + getLogFileName(main);
        QFile logger( logFile );
        logger.open(QIODevice::WriteOnly | QIODevice::Append);
        //QTextStream out(&logger);
        //out << log_text;
        //for (int j; j<log_text.length();j++){
            logger.write( log_text.toUtf8());

        //  }
        logger.close();
    }
}
/**
 * @brief MainWindow::copyRecursively
 * @param srcFilePath
 * @param tgtFilePath
 * @return
 */
bool MainWindow::copyRecursively(const QString &srcFilePath, const QString &tgtFilePath)
{
    QFileInfo srcFileInfo(srcFilePath);
    if (srcFileInfo.isDir()) {
        QDir targetDir(tgtFilePath);
        targetDir.cdUp();
        if (!targetDir.mkdir(QFileInfo(tgtFilePath).fileName()))
              return false;
        QDir sourceDir(srcFilePath);
        QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
        foreach (const QString &fileName, fileNames) {
            const QString newSrcFilePath
                    = srcFilePath + QLatin1Char('/') + fileName;
            const QString newTgtFilePath
                    = tgtFilePath + QLatin1Char('/') + fileName;
            if (!copyRecursively(newSrcFilePath, newTgtFilePath))
                  return false;
        }
    } else {
        if (!QFile::copy(srcFilePath, tgtFilePath))
              return false;
    }
    return true;
}
/**
 * @brief MainWindow::writeFooterAndHeaderDebug
 * @param header
 */
void MainWindow::writeFooterAndHeaderDebug(bool header)
{
    if (header)
    {
        if(debug) cout<<"[LOG-DBG] "<<getLocalDateAndTime()<<" ================================="<<endl;
    }
    else
    {
        if(debug) cout<<"[END-LOG-DBG] ======================================================"<<endl;
    }
}

/* Menu: Preferencias */

/**
 * @brief MainWindow::writePreferencesFile
 * @param pref
 * @param filename
 * @param force
 * @return MCAE::FILE_NOT_FOUND o MCAE::OK
 */
int MainWindow::writePreferencesFile(QString pref, QString filename, bool force)
{
    QString path = getPreferencesDir();
    QString logs = getPreferencesDir() + "/logs";
    QString debugs = getPreferencesDir() + "/debug";
    QDir dir(path);
    QDir log(logs);
    QDir deb(debugs);
    if (!dir.exists())
    {
        dir.mkdir(path);
    }
    if (!log.exists())
    {
        log.mkdir(logs);
    }
    if (!deb.exists())
    {
        deb.mkdir(debugs);
    }

    QFile file( path + "/" + filename );
    bool exist = file.exists();

    if (!force)
    {
        if (!exist)
        {
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            if(debug) cout<<"No se encuentra el archivo de preferencias. Se crea el archivo por defecto."<<endl;
            QTextStream out(&file);
            out << pref;
            file.close();
            return MCAE::FILE_NOT_FOUND;
        }
    }
    else
    {
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << pref;
        file.close();
    }

    return MCAE::OK;
}
/**
 * @brief MainWindow::getPreferencesSettingsFile
 */
void MainWindow::getPreferencesSettingsFile()
{
    QString qtmca_conf = getPreferencesDir() +"/"+ preferencesfile;
    QSettings qtmcasettins(qtmca_conf, QSettings::IniFormat);

    //qtmcasettins.status();
    debug = qtmcasettins.value("Modo/debug", "US").toBool();
    log = qtmcasettins.value("Modo/log", "US").toBool();
    stdout_mode = qtmcasettins.value("Modo/stdout", "US").toBool();
    initfile = qtmcasettins.value("Paths/conf_set_file", "US").toString();
    root_calib_path = qtmcasettins.value("Paths/calib_set_file", "US").toString();

}
/**
 * @brief MainWindow::setPreferencesSettingsFile
 * @param folder
 * @param variable
 * @param value
 */
void MainWindow::setPreferencesSettingsFile(QString folder, QString variable, QString value)
{
    QString qtmca_conf = getPreferencesDir() +"/"+ preferencesfile;
    QSettings qtmcasettins(qtmca_conf, QSettings::IniFormat);

    qtmcasettins.setValue(folder+"/"+variable, value);
}



/**
 * @brief MainWindow::getValuesFromFiles
 *
 * Analizador de archivo de texto
 *
 * @param filename
 * @param hv
 * @return Vector con los valores obtenidos del archivo
 */
QVector<double> MainWindow::getValuesFromFiles(QString filename, bool hv)
{
    QVector<double> values;
    QRegExp rx("(\\t)");
    QFile inputFile(filename);

    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        if(hv)
        {
            while (!in.atEnd())
            for(int i=0;i<PMTs;i++)
            {
                QString line = in.readLine();
                QStringList fields = line.split(rx);
                QString q_hv=fields.at(1);
                values.push_back(q_hv.toDouble());
            }
        }
        else
        {
            while (!in.atEnd())
            {
                QString line = in.readLine();
                values.push_back(line.toDouble());

            }
        }
        inputFile.close();
    }

    return values;
}


/**
 * @brief MainWindow::parseConfigurationFile
 *
 * Analizador del archivo de configuración
 *
 * @param filename
 * @return
 */

/**
 * @brief MainWindow::openConfigurationFile
 * @return
 */
QString MainWindow::openConfigurationFile()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Abrir archivo de configuración"),
                                                    root_config_path,
                                                    tr("Configuración (*.ini);;Texto (*.txt)"));
    initfile = filename;
    return filename;
}
/**
 * @brief MainWindow::openLogFile
 * @return
 */
QString MainWindow::openLogFile()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Abrir archivo de log"),
                                                    root_log_path,
                                                    tr("Log (*.log)"));
    initfile = filename;
    return filename;
}
/**
 * @brief MainWindow::openDirectory
 * @return
 */
QString MainWindow::openDirectory()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Seleccionar el directorio"),
                                                         QDir::homePath());
    return directory;
}




/**
 * @brief MainWindow::readString
 *
 * Método general de lectura de datos por protocolo serie
 *
 * @param delimeter
 * @return Mensaje recibido en _string_
 */
string MainWindow::readString(char delimeter)
{
    mMutex.lock();

    string msg;
    try{
        msg = arpet->readString(delimeter, port_name.toStdString());
    }
    catch( Exceptions & ex ){
        Exceptions exception_stop(ex.excdesc);
        mMutex.unlock();
        throw exception_stop;
    }

    mMutex.unlock();

    return msg;
}

/**
 * @brief MainWindow::sendString
 *
 * Método general de escritura de datos por protocolo serie
 *
 * @param msg
 * @param end
 * @return Mensaje recibido en _string_
 */
size_t MainWindow::sendString(string msg, string end)
{
    mMutex.lock();

    arpet->portFlush();
    size_t bytes_transfered = 0;

    try{
        bytes_transfered = arpet->sendString(msg, end, port_name.toStdString());
    }
    catch(boost::system::system_error e){
        Exceptions exception_serial_port((string("No se puede acceder al puerto serie. Error: ")+string(e.what())).c_str());
        mMutex.unlock();
        throw exception_serial_port;
    }

    mMutex.unlock();

    return bytes_transfered;
}



/* Terminal */
/**
 * @brief MainWindow::on_pushButton_send_terminal_clicked
 */
void MainWindow::on_pushButton_send_terminal_clicked()
{
    QString sended ="";// ui->lineEdit_terminal->text();
    size_t bytes=0;
    string msg;
   // string end_stream=arpet->getEnd_MCA();
    QString Cabezal="";//ui->comboBox_head_select_terminal->currentText();
    arpet->portDisconnect();

    try
    {

        port_name=ui->EditText_Puerto->text();
        arpet->portConnect(port_name.toStdString().c_str());

        //if(ui->checkBox_end_terminal->isChecked()) end_stream=arpet->getEnd_PSOC();

        bytes = sendString(sended.toStdString(),"");
        msg = readString();

        QString q_msg=QString::fromStdString(msg);
        QString q_bytes=QString::number(bytes);

        //ui->label_size_terminal->setText(q_bytes);
        //ui->label_received_terminal->setText(q_msg);
        arpet->portDisconnect();
    }
    catch(Exceptions & ex)
    {
        QMessageBox::critical(this,tr("Atención"),tr(ex.excdesc));
        arpet->portDisconnect();
    }
}
/**
 * @brief MainWindow::on_pushButton_flush_terminal_clicked
 */
void MainWindow::on_pushButton_flush_terminal_clicked()
{
    arpet->portFlush();
}
/**
 * @brief MainWindow::on_pushButton_clear_terminal_clicked
 */
void MainWindow::on_pushButton_clear_terminal_clicked()
{
    //ui->lineEdit_terminal->clear();
}

QVector<int> MainWindow::getCustomPlotParameters()
{
    QVector<int> param(6);
    param[0]=rand()%245+10;//R
    param[1]=rand()%245+10;//G
    param[2]=rand()%245+10;//B
    param[3]=rand()%5+1; //LineStyle
    param[4]=rand()%14+1;//ScatterShape
    param[5]=rand()/(double)RAND_MAX*2+1;//setWidthF

    return param;
}







/**
 * @brief MainWindow::mousePressGraphsLog
 */
void MainWindow::mousePressGraphsLog()
{
    if (ui->specPMTs_3->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
        ui->specPMTs_3->axisRect()->setRangeDrag(ui->specPMTs_3->xAxis->orientation());
    else if (ui->specPMTs_3->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
        ui->specPMTs_3->axisRect()->setRangeDrag(ui->specPMTs_3->yAxis->orientation());
    else
        ui->specPMTs_3->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::selectionChangedLogGraph(){
    if (ui->specPMTs_3->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->specPMTs_3->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
            ui->specPMTs_3->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->specPMTs_3->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
    {
        ui->specPMTs_3->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        ui->specPMTs_3->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    }

    if (ui->specPMTs_3->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->specPMTs_3->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
            ui->specPMTs_3->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->specPMTs_3->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
    {
        ui->specPMTs_3->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        ui->specPMTs_3->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    }
    for (int i=0; i<ui->specPMTs_3->graphCount(); ++i)
    {
        QCPGraph *graph = ui->specPMTs_3->graph(i);
        QCPPlottableLegendItem *item = ui->specPMTs_3->legend->itemWithPlottable(graph);
        if (item->selected() || graph->selected())
        {
            item->setSelected(true);
            graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
        }
    }
}



/**
 * @brief MainWindow::getLogFromFiles
 *
 * Analizador de archivo de texto
 *
 * @param filename
 * @param hv
 * @return Vector con los valores obtenidos del archivo
 */
QStringList MainWindow::getLogFromFiles(QString filename,QRegExp rx, string parser)
{
    QString values;
    QFile inputFile(filename);

    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);

            while (!in.atEnd())
            {
                QString line = in.readLine();
                while ((-1)==line.toStdString().find(parser)&&!in.atEnd()){
                   line = in.readLine();
                }
                if (in.atEnd()) break;
                values+=line+",";
            }

        inputFile.close();
    }

    return values.split(rx, QString::SkipEmptyParts);
}

void MainWindow::on_pushButton_p_51_clicked()
{
}



bool MainWindow::fileExists(QString path) {
    QFileInfo check_file(path);
    // check if file exists and if yes: Is it really a file and no directory?
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
          return false;
    }
}



/**
 * @brief MainWindow::updateCaption Este es un timer que actualiza la lista de cabezales disponible y habilita o deshabilita los checkbox de cabezales
 *
 */
void MainWindow::updateCaption(){

}


void MainWindow::CargoTemaOscuro(){

    QFile f(":qdarkstyle/style.qss");
    if (!f.exists())
    {
        printf("Unable to set stylesheet, file not found\n");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }
}


void MainWindow::TimerUpdate(){
    worker->TimerUpdate();
}



void MainWindow::on_pushButton_send_terminal_2_clicked()
{

}

void MainWindow::on_pushButton_2_clicked()
{
    ui->specPMTs_3->clearGraphs();
    ui->specPMTs_3->addGraph();
    ui->specPMTs_3->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    //ui->specPMTs_3->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
    ui->specPMTs_3->addGraph();
    ui->specPMTs_3->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph
    ui->specPMTs_3->addGraph();
    ui->specPMTs_3->graph(2)->setPen(QPen(Qt::yellow)); // line color red for second graph
    ui->specPMTs_3->graph(2)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
    // generate some points of data (y0 for first, y1 for second graph):
    QVector<double> x(251), CorT(251), CorS(251),CorR(251),TenT(251), TenS(251),TenR(251),PotS(251), PotR(251),PotT(251);
    int VmedCor=0,VmedTen=0,VefCor,VefTen,VmedPot=0;
    // configure right and top axis to show ticks but no labels:
    // (see QCPAxisRect::setupFullAxesBox for a quicker method to do this)
    ui->specPMTs_3->xAxis2->setVisible(true);
    ui->specPMTs_3->xAxis2->setTickLabels(false);
    ui->specPMTs_3->yAxis2->setVisible(true);
    ui->specPMTs_3->yAxis2->setTickLabels(false);
    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(ui->specPMTs_3->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->specPMTs_3->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->specPMTs_3->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->specPMTs_3->yAxis2, SLOT(setRange(QCPRange)));
    // pass data points to graphs:


    switch(ui->cb_Fase->currentIndex()){
        case 0:{
        for (int i=0; i<251; ++i)
        {
          x[i] = i;
          TenR[i]=qCos(i/10.0);
          CorR[i]=qSin(i/10.0);
          PotR[i]=TenR[i]*CorR[i];
          VmedTen+=TenR[i];
          VmedCor+=CorR[i];
          VmedPot+=abs(PotR[i]);
        }
        ui->specPMTs_3->graph(0)->setData(x, TenR);
        ui->specPMTs_3->graph(1)->setData(x, CorR);
        ui->specPMTs_3->graph(2)->setData(x, PotR);
        ui->label_info->setText("VMed de Corriente: "+QString::number(VmedCor)+"   VMed de Tension: "+QString::number(VmedTen)+"    Vmed de Potencia: "+QString::number(VmedPot));
        break;
    }
    case 1:{
        for (int i=0; i<251; ++i)
        {
          x[i] = i;
          TenS[i]=qSin(i/10.0);
          CorS[i]=qSin(i/10.0)*0.8;
          PotS[i]=TenS[i]*CorS[i];
          VmedTen+=TenS[i];
          VmedCor+=CorS[i];
          VmedPot+=abs(PotS[i]);
        }
        ui->specPMTs_3->graph(0)->setData(x, TenS);
        ui->specPMTs_3->graph(1)->setData(x, CorS);
        ui->specPMTs_3->graph(2)->setData(x, PotS);
        ui->label_info->setText("VMed de Corriente: "+QString::number(VmedCor)+"   VMed de Tension: "+QString::number(VmedTen)+"    Vmed de Potencia: "+QString::number(VmedPot));

        break;
    }
    case 2:{
        for (int i=0; i<251; ++i)
        {
          x[i] = i;
          TenT[i]=qSin(i/10.0);
          CorT[i]=qCos(i/10.0);
          PotT[i]=TenT[i]*CorT[i];
          VmedTen+=TenT[i];
          VmedCor+=CorT[i];
          VmedPot+=abs(PotT[i]);
        }
        ui->specPMTs_3->graph(0)->setData(x, TenT);
        ui->specPMTs_3->graph(1)->setData(x, CorT);
        ui->specPMTs_3->graph(2)->setData(x, PotT);
        ui->label_info->setText("VMed de Corriente: "+QString::number(VmedCor)+"   VMed de Tension: "+QString::number(VmedTen)+"    Vmed de Potencia: "+QString::number(VmedPot));

        break;
    }


    }



    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    ui->specPMTs_3->graph(0)->rescaleAxes();
    // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
    ui->specPMTs_3->graph(1)->rescaleAxes(true);
    ui->specPMTs_3->graph(2)->rescaleAxes(true);
    // Note: we could have also just called customPlot->rescaleAxes(); instead
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    ui->specPMTs_3->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
   switch(index){
    case 0:{
       ui->specPMTs_3->clearGraphs();
       ui->specPMTs_3->addGraph();
       ui->specPMTs_3->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
       ui->specPMTs_3->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
       ui->specPMTs_3->addGraph();
       ui->specPMTs_3->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph
       // generate some points of data (y0 for first, y1 for second graph):
       QVector<double> x(251), y0(251), y1(251);
       for (int i=0; i<251; ++i)
       {
         x[i] = i;
         y0[i] = qExp(-i/150.0)*qCos(i/10.0); // exponentially decaying cosine
         y1[i] = qExp(-i/150.0);              // exponential envelope
       }
       // configure right and top axis to show ticks but no labels:
       // (see QCPAxisRect::setupFullAxesBox for a quicker method to do this)
       ui->specPMTs_3->xAxis2->setVisible(true);
       ui->specPMTs_3->xAxis2->setTickLabels(false);
       ui->specPMTs_3->yAxis2->setVisible(true);
       ui->specPMTs_3->yAxis2->setTickLabels(false);
       // make left and bottom axes always transfer their ranges to right and top axes:
       connect(ui->specPMTs_3->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->specPMTs_3->xAxis2, SLOT(setRange(QCPRange)));
       connect(ui->specPMTs_3->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->specPMTs_3->yAxis2, SLOT(setRange(QCPRange)));
       // pass data points to graphs:
       ui->specPMTs_3->graph(0)->setData(x, y0);
       ui->specPMTs_3->graph(1)->setData(x, y1);
       // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
       ui->specPMTs_3->graph(0)->rescaleAxes();
       // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
       ui->specPMTs_3->graph(1)->rescaleAxes(true);
       // Note: we could have also just called customPlot->rescaleAxes(); instead
       // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
       ui->specPMTs_3->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
       break;
   }
   case 1:{
       ui->specPMTs_3->clearGraphs();
       ui->specPMTs_3->addGraph();
       ui->specPMTs_3->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
       ui->specPMTs_3->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
       ui->specPMTs_3->addGraph();
       ui->specPMTs_3->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph
       // generate some points of data (y0 for first, y1 for second graph):
       QVector<double> x(251), y0(251), y1(251);
       for (int i=0; i<251; ++i)
       {
         x[i] = i;
         y0[i] = qExp(-i/150.0)*qCos(i/10.0); // exponentially decaying cosine
         y1[i] = qExp(-i/150.0);              // exponential envelope
       }
       // configure right and top axis to show ticks but no labels:
       // (see QCPAxisRect::setupFullAxesBox for a quicker method to do this)
       ui->specPMTs_3->xAxis2->setVisible(true);
       ui->specPMTs_3->xAxis2->setTickLabels(false);
       ui->specPMTs_3->yAxis2->setVisible(true);
       ui->specPMTs_3->yAxis2->setTickLabels(false);
       // make left and bottom axes always transfer their ranges to right and top axes:
       connect(ui->specPMTs_3->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->specPMTs_3->xAxis2, SLOT(setRange(QCPRange)));
       connect(ui->specPMTs_3->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->specPMTs_3->yAxis2, SLOT(setRange(QCPRange)));
       // pass data points to graphs:
       QCPBars *myBars = new QCPBars(ui->specPMTs_3->xAxis, ui->specPMTs_3->yAxis);
       // now we can modify properties of myBars:
       myBars->setName("Bars Series 1");
       QVector<double> keyData;
       QVector<double> valueData;
       keyData << 1 << 2 << 3;
       valueData << 2 << 4 << 8;
       myBars->setData(keyData, valueData);
       ui->specPMTs_3->rescaleAxes();
       ui->specPMTs_3->replot();
       // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
       ui->specPMTs_3->graph(0)->rescaleAxes();
       // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
       ui->specPMTs_3->graph(1)->rescaleAxes(true);
       // Note: we could have also just called customPlot->rescaleAxes(); instead
       // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
       ui->specPMTs_3->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
       break;
   }
   case 2:{
       ui->specPMTs_3->clearGraphs();
        break;
   }


   }
}

void MainWindow::on_pushButton_4_clicked()
{

}

void MainWindow::on_pushButton_3_clicked()
{
    SetPMTs.show();
}