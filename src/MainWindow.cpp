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
    initfile(""),
    root_config_path(""),
    root_log_path(QDir::homePath() +""),
    preferencesdir(QDir::homePath() + ""),
    preferencesfile(""),
    ui(new Ui::MainWindow)
{
    ComBT =  (new MCAE());
    thread = new QThread();
    worker = new Thread(ComBT, &mMutex);

    ui->setupUi(this);
    /** @todo: Ver la posibilidad de maximizar la ventana */
    this->setFixedSize(this->maximumSize());
    setPreferencesConfiguration();
    QTimer *timerw = new QTimer(this);
    connect(timerw, SIGNAL(timeout()), this, SLOT(updateCaption()));
    CargoTemaOscuro();
    timerw->start(1000);
    
    pmt_select = new SetPMTs(this);
    //GitCheckerStarter();
    connectSlots();


}
/**
 * @brief MainWindow::~MainWindow
 *
 * Destructor de la clase
 *
 */
MainWindow::~MainWindow()
{
    ComBT->portDisconnect();
    worker->abort();
    thread->wait();
    DisconnectBluetooth();
    delete thread;
    delete worker;
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
    pref = new SetPreferences(this);

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

//    QProcess killall;
//    killall.waitForStarted();
//    killall.start("rfcomm -a");
//    QString output;
//    while(killall.waitForReadyRead(1000)) {
//        output+=killall.readAll();
//    }

//    if(output.contains("20:16:02:30:99:61"))
//    {
//        killall.execute("sudo rfcomm connect 0 20:16:02:30:99:61 1");
//        killall.waitForFinished(1000);
//        //cout<<killall.readAll().toStdString()<<endl;
//    }
}

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
        msg = ComBT->readString(delimeter, port_name.toStdString());
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

    ComBT->portFlush();
    size_t bytes_transfered = 0;

    try{
        bytes_transfered = ComBT->sendString(msg, end, port_name.toStdString());
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
//void MainWindow::on_pushButton_send_terminal_clicked()
//{
//    QString sended ="";// ui->lineEdit_terminal->text();
//    size_t bytes=0;
//    string msg;
//   // string end_stream=ComBT->getEnd_MCA();
//    QString Cabezal="";//ui->comboBox_head_select_terminal->currentText();
//    ComBT->portDisconnect();

//    try
//    {

//        port_name=ui->EditText_Puerto->text();
//        ComBT->portConnect(port_name.toStdString().c_str());

//        //if(ui->checkBox_end_terminal->isChecked()) end_stream=ComBT->getEnd_PSOC();

//        bytes = sendString(sended.toStdString(),"");
//        msg = readString();

//        QString q_msg=QString::fromStdString(msg);
//        QString q_bytes=QString::number(bytes);

//        //ui->label_size_terminal->setText(q_bytes);
//        //ui->label_received_terminal->setText(q_msg);
//        ComBT->portDisconnect();
//    }
//    catch(Exceptions & ex)
//    {
//        QMessageBox::critical(this,tr("Atención"),tr(ex.excdesc));
//        ComBT->portDisconnect();
//    }
//}
/**
 * @brief MainWindow::on_pushButton_flush_terminal_clicked
 */
//void MainWindow::on_pushButton_flush_terminal_clicked()
//{
//    ComBT->portFlush();
//}



/**
 * @brief MainWindow::readBufferString
 *
 * Método general de lectura de _buffer_ de datos por protocolo serie
 *
 * @param buffer_size
 * @return Mensaje recibido en _string_
 */
string MainWindow::readBufferString(int buffer_size)
{
    mMutex.lock();

    string msg;
    try{
        msg = ComBT->readBufferString(buffer_size,port_name.toStdString());
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
 * @brief MainWindow::on_pushButton_clear_terminal_clicked
 */
//void MainWindow::on_pushButton_clear_terminal_clicked()
//{
//    //ui->lineEdit_terminal->clear();
//}

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

/**
 * @brief MainWindow::selectionChangedLogGraph
 */

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

//void MainWindow::on_pushButton_p_51_clicked()
//{
//}



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

    QString sended ="";// ui->lineEdit_terminal->text();
    size_t bytes=0;
    string msg;
   // string end_stream=ComBT->getEnd_MCA();
    QString port=ui->EditText_Puerto->text();
    //ComBT->portDisconnect();

    try
    {

        port_name=ui->EditText_Puerto->text();
        ComBT->portConnect(port_name.toStdString().c_str());

        sended = ui->lineEdit_terminal_2->text();

        bytes = sendString(sended.toStdString(),"");
        msg = readString();

        ui->label_size_terminal_2->setText(QString::fromStdString(msg));

        QString q_msg=QString::fromStdString(msg);
        QString q_bytes=QString::number(bytes);

        //ui->label_size_terminal->setText(q_bytes);
        //ui->label_received_terminal->setText(q_msg);
        ComBT->portDisconnect();
    }
    catch(Exceptions & ex)
    {
        QMessageBox::critical(this,tr("Atención"),tr(ex.excdesc));
        ComBT->portDisconnect();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QVector<double> x(MED_POR_CICLO), CorT(MED_POR_CICLO), CorS(MED_POR_CICLO),CorR(MED_POR_CICLO),
            TenT(MED_POR_CICLO), TenS(MED_POR_CICLO),TenR(MED_POR_CICLO),
            PotS(MED_POR_CICLO), PotR(MED_POR_CICLO),PotT(MED_POR_CICLO),
            PotEfaux(MED_POR_CICLO), tenEfaux(MED_POR_CICLO),CorEfaux(MED_POR_CICLO),
            PotEf(MED_POR_CICLO),TenEf(MED_POR_CICLO),CorEf(MED_POR_CICLO);

    double VmedCor=0,VmedTen=0,VefCor,VefTen,VefPot,VmedPot=0;
    int b;
    int bytes;
    QString men;
    QByteArray mensaje;
    port_name="/dev/rfcomm0";
    QString sended ="";// ui->lineEdit_terminal->text();
    //size_t bytes=0;
    string msg;
    int getModo=ui->comboBox_3->currentIndex();
    QString Modo;
    switch(getModo){
    case 0:{
        Modo="S";
        break;
    }
    case 1:{
        Modo="T";
        break;
    }
    case 2:{
        Modo="D";
        break;
    }
    default:
        Modo="";
    }
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

    try{
        ComBT->portConnect(port_name.toStdString().c_str());

        switch(ui->comboBox_3->currentIndex()){
        case 0:{
            switch(ui->cb_Fase->currentIndex()){
            case 0:{

                ////////////////// USADO PARA DEBUGUEAR ///////////////
                sended ="MV;";

                bytes = sendString(sended.toStdString(),"");

                msg=readString();//readBufferString(bytes+2);
                bytes=(unsigned int )(0x00ff&msg[1]);

                men=QString::fromStdString(msg);
                mensaje.clear();
                mensaje.append(men.toUtf8());
                TenR.clear();



                for (int i=3;i<(bytes);i++){

                    b=(((0x00ff&msg[i+3])-127));
                    TenR.append((b));
                    VmedTen+=TenR[i-3];
                    VefTen+=(TenR[i-3]*TenR[i-3]);
                    x[i-3] = i-3;
                }

                VmedTen=VmedTen*5000/((bytes-3)*0.185*255);

                ui->specPMTs_3->graph(0)->setData(x, TenR);
                break;
            }
            case 1:{


                ////////////////// USADO PARA DEBUGUEAR ///////////////
                sended ="MI;";

                bytes = sendString(sended.toStdString(),"");

                msg=readString();//readBufferString(bytes+2);
                int bytes=(unsigned int )(0x00ff&msg[1]);

                men=QString::fromStdString(msg);
                mensaje.clear();
                mensaje.append(men.toUtf8());
                CorR.clear();
                for (int i=3;i<(bytes);i++){

                    b=(((0x00ff&msg[i+3])-127));
                    CorR.append((b));
                    VmedCor+=CorR[i-3];
                    VefCor+=(CorR[i-3]*CorR[i-3]);
                    x[i-3] = i-3;
                }

                VmedCor=VmedCor*5000/((bytes-3)*0.185*255);

                ui->specPMTs_3->graph(0)->setData(x, CorR);


                break;

            }
            case 2:{
                sended ="MV;";

                bytes = sendString(sended.toStdString(),"");

                msg=readString();//readBufferString(bytes+2);
                bytes=(unsigned int )(0x00ff&msg[1]);

                men=QString::fromStdString(msg);
                mensaje.clear();
                mensaje.append(men.toUtf8());
                TenR.clear();



                for (int i=3;i<(bytes);i++){

                    b=(((0x00ff&msg[i+3])-127));
                    TenR.append((b));
                    VmedTen+=TenR[i-3];
                    VefTen+=(TenR[i-3]*TenR[i-3]);
                    x[i-3] = i-3;
                }

                VmedTen=VmedTen*5000/((bytes-3)*0.185*255);

                sended ="MI;";

                bytes = sendString(sended.toStdString(),"");

                msg=readString();//readBufferString(bytes+2);
                int bytes=(unsigned int )(0x00ff&msg[1]);

                men=QString::fromStdString(msg);
                mensaje.clear();
                mensaje.append(men.toUtf8());
                CorR.clear();
                for (int i=3;i<(bytes);i++){

                    b=(((0x00ff&msg[i+3])-127));
                    CorR.append((b));
                    VmedCor+=CorR[i-3];
                    VefCor+=(CorR[i-3]*CorR[i-3]);
                    x[i-3] = i-3;
                }

                VmedCor=VmedCor*5000/((bytes-3)*0.185*255);



                for (int i=0;i<MED_POR_CICLO;i++){
                    PotR[i]=TenR[i]*CorR[i];
                }
                ui->specPMTs_3->graph(0)->setData(x, TenR);
                ui->specPMTs_3->graph(1)->setData(x, CorR);
                ui->specPMTs_3->graph(2)->setData(x, PotR);
                break;
            }
            }
            break;

        }
        case 1:{
            switch(ui->cb_Fase->currentIndex()){
            case 0:{

                ////////////////// USADO PARA DEBUGUEAR ///////////////
                sended ="MV;";

                bytes = sendString(sended.toStdString(),"");

                msg=readString();//readBufferString(bytes+2);
                bytes=(unsigned int )(0x00ff&msg[1]);

                men=QString::fromStdString(msg);
                mensaje.clear();
                mensaje.append(men.toUtf8());
                TenR.clear();



                for (int i=3;i<(bytes);i++){

                    b=(((0x00ff&msg[i+3])-127));
                    TenR.append((b));
                    VmedTen+=TenR[i-3];
                    VefTen+=(TenR[i-3]*TenR[i-3]);
                    x[i-3] = i-3;
                }

                VmedTen=VmedTen*5000/((bytes-3)*0.185*255);

                ui->specPMTs_3->graph(0)->setData(x, TenR);
                break;
            }
            case 1:{


                ////////////////// USADO PARA DEBUGUEAR ///////////////
                sended ="MI;";

                bytes = sendString(sended.toStdString(),"");

                msg=readString();//readBufferString(bytes+2);
                int bytes=(unsigned int )(0x00ff&msg[1]);

                men=QString::fromStdString(msg);
                mensaje.clear();
                mensaje.append(men.toUtf8());
                CorR.clear();
                for (int i=3;i<(bytes);i++){

                    b=(((0x00ff&msg[i+3])-127));
                    CorR.append((b));
                    VmedCor+=CorR[i-3];
                    VefCor+=(CorR[i-3]*CorR[i-3]);
                    x[i-3] = i-3;
                }

                VmedCor=VmedCor*5000/((bytes-3)*0.185*255);

                ui->specPMTs_3->graph(0)->setData(x, CorR);


                break;

            }
            case 2:{
                sended ="MV;";

                bytes = sendString(sended.toStdString(),"");

                msg=readString();//readBufferString(bytes+2);
                bytes=(unsigned int )(0x00ff&msg[1]);

                men=QString::fromStdString(msg);
                mensaje.clear();
                mensaje.append(men.toUtf8());
                TenR.clear();



                for (int i=3;i<(bytes);i++){

                    b=(((0x00ff&msg[i+3])-127));
                    TenR.append((b));
                    VmedTen+=TenR[i-3];
                    VefTen+=(TenR[i-3]*TenR[i-3]);
                    x[i-3] = i-3;
                }

                VmedTen=VmedTen*5000/((bytes-3)*0.185*255);

                sended ="MI;";

                bytes = sendString(sended.toStdString(),"");

                msg=readString();//readBufferString(bytes+2);
                int bytes=(unsigned int )(0x00ff&msg[1]);

                men=QString::fromStdString(msg);
                mensaje.clear();
                mensaje.append(men.toUtf8());
                CorR.clear();
                for (int i=3;i<(bytes);i++){

                    b=(((0x00ff&msg[i+3])-127));
                    CorR.append((b));
                    VmedCor+=CorR[i-3];
                    VefCor+=(CorR[i-3]*CorR[i-3]);
                    x[i-3] = i-3;
                }

                VmedCor=VmedCor*5000/((bytes-3)*0.185*255);



                for (int i=0;i<MED_POR_CICLO;i++){
                    PotR[i]=TenR[i]*CorR[i];
                }
                ui->specPMTs_3->graph(0)->setData(x, TenR);
                ui->specPMTs_3->graph(1)->setData(x, CorR);
                ui->specPMTs_3->graph(2)->setData(x, PotR);
                break;
            }
            }
            break;

        }
        case 3:{
        switch(ui->cb_Fase->currentIndex()){
        case 0:{
            for (int i=0; i<MED_POR_CICLO; ++i)
            {
              x[i] = i;
              TenR[i]=qCos(i/10.0);
              CorR[i]=qSin(i/10.0);
              PotR[i]=TenR[i]*CorR[i];
              VmedTen+=TenR[i];
              VmedCor+=CorR[i];
              VmedPot+=abs(PotR[i]);
              VefTen+=(TenR[i]*TenR[i]);
              VefCor+=(CorR[i]*CorR[i]);
              VefPot+=(PotR[i]*PotR[i]);
            }


            /*

            PotEfaux(MED_POR_CICLO), tenEfaux(MED_POR_CICLO),CorEfaux(MED_POR_CICLO),
            PotEf(MED_POR_CICLO),TenEf(MED_POR_CICLO),CorEf(MED_POR_CICLO);

            int VmedCor=0,VmedTen=0,VefCor,VefTen,VmedPot=0;

*/

            ui->specPMTs_3->graph(0)->setData(x, TenR);
            ui->specPMTs_3->graph(1)->setData(x, CorR);
            ui->specPMTs_3->graph(2)->setData(x, PotR);
            break;
        }
        case 1:{
            for (int i=0; i<MED_POR_CICLO; ++i)
            {
              x[i] = i;
              TenS[i]=qSin(i/10.0);
              CorS[i]=qSin(i/10.0)*0.8;
              PotS[i]=TenS[i]*CorS[i];
              VmedTen+=TenS[i];
              VmedCor+=CorS[i];
              VmedPot+=abs(PotS[i]);
              VefTen+=(TenS[i]*TenS[i]);
              VefCor+=(CorS[i]*CorS[i]);
              VefPot+=(PotS[i]*PotS[i]);
            }

            ui->specPMTs_3->graph(0)->setData(x, TenS);
            ui->specPMTs_3->graph(1)->setData(x, CorS);
            ui->specPMTs_3->graph(2)->setData(x, PotS);

            break;
        }
        case 2:{
            for (int i=0; i<MED_POR_CICLO; ++i)
            {
              x[i] = i;
              TenT[i]=qSin(i/10.0);
              CorT[i]=qCos(i/10.0)+0.5;
              PotT[i]=TenT[i]*CorT[i];
              VmedTen+=TenT[i];
              VmedCor+=CorT[i];
              VmedPot+=abs(PotT[i]);
              VefTen+=(TenT[i]*TenT[i]);
              VefCor+=(CorT[i]*CorT[i]);
              VefPot+=(PotT[i]*PotT[i]);
            }

            ui->specPMTs_3->graph(0)->setData(x, TenT);
            ui->specPMTs_3->graph(1)->setData(x, CorT);
            ui->specPMTs_3->graph(2)->setData(x, PotT);

            break;
        }
        }
        break;

    }
    }

    ui->label_tenEficaz->setText("Ten. Eficaz: "+QString::number(sqrt(VefTen)/bytes));
    ui->label_corEficaz->setText("Cor. Eficaz: "+QString::number(sqrt(VefCor)/bytes));
    ui->label_potEficaz->setText("Pot. Eficaz: "+QString::number(sqrt(VefPot)/bytes));
    ui->label_tenMedia->setText("Ten. Media: "+QString::number((VmedTen)));
    ui->label_corMedia->setText("Cor. Media: "+QString::number((VmedCor)));
    ui->label_potMedia->setText("Pot. Media: "+QString::number((VmedPot)));

    ComBT->portDisconnect();
    }

    catch(Exceptions & ex)
    {
        QMessageBox::critical(this,tr("Atención"),tr(ex.excdesc));
        ComBT->portDisconnect();
    }

    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    ui->specPMTs_3->graph(0)->rescaleAxes();
    // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
    ui->specPMTs_3->graph(1)->rescaleAxes(true);
    ui->specPMTs_3->graph(2)->rescaleAxes(true);
    // Note: we could have also just called customPlot->rescaleAxes(); instead
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    ui->specPMTs_3->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->specPMTs_3->rescaleAxes();
    ui->specPMTs_3->replot();
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

//void MainWindow::on_pushButton_4_clicked()
//{

//}

void MainWindow::on_pushButton_3_clicked()
{

    int Grafico=ui->comboBox_3->currentIndex();
    QString filename = QString::number(Grafico+1);
   // QImage image(filename);
    pmt_select->ConfigureImage(filename);
    //pmt_select->foto->setPixmap(QPixmap::fromImage(image));
    pmt_select->show();
}

void MainWindow::on_comboBox_3_currentIndexChanged(int index)
{
    if (index>1){
        ui->cb_alpha_pw->hide();
        ui->le_conf->hide();
        ui->pb_enviar_conf->hide();
    }else{
        ui->cb_alpha_pw->show();
        ui->le_conf->show();
        ui->pb_enviar_conf->show();
    }
}

void MainWindow::on_pb_enviar_conf_clicked()
{
    QString sended ="";// ui->lineEdit_terminal->text();
    size_t bytes=0;
    string msg;
   // string end_stream=ComBT->getEnd_MCA();
    QString port=ui->EditText_Puerto->text();
    //ComBT->portDisconnect();
    int var =ui->le_conf->text().toInt();
//    int SetConfig=ui->cb_alpha_pw->currentIndex();
    int getModo=ui->comboBox_3->currentIndex();

    QString Config;
    QString Modo;
    switch(getModo){
    case 0:{
        Modo="S";
        break;
    }
    case 1:{
        Modo="T";
        break;
    }
    default:
        return;
    }


//    switch(SetConfig){
//    case 0:{
//        Config="A";
//        break;
//    }
//    case 1:{
//        Config="D";
//        break;
//    }
//    default:
//        return;
//    }
    try
    {

        port_name="/dev/rfcomm0";
        ComBT->portConnect(port_name.toStdString().c_str());
        if (var<2)var=2;
        sended =Modo+(char)((((unsigned int )var*8/9)))+';';
        QByteArray send=sended.toLatin1();

        string algo=""+(char)send.at(0)+(char)send.at(1)+(char)send.at(2)+'\0';
        bytes = sendString(send.data(),"");

        msg = readString();
        cout << msg<<endl;
        ui->label_size_terminal_2->setText(QString::fromStdString(msg));

        QString q_msg=QString::fromStdString(msg);
        QString q_bytes=QString::number(bytes);

        //ui->label_size_terminal->setText(q_bytes);
        //ui->label_received_terminal->setText(q_msg);
        ComBT->portDisconnect();
    }
    catch(Exceptions & ex)
    {
        QMessageBox::critical(this,tr("Atención"),tr(ex.excdesc));
        ComBT->portDisconnect();
    }
}
/**
 * @brief MainWindow::DisconnectBluetooth
 */
void MainWindow::DisconnectBluetooth(){
    QProcess killall;
    killall.waitForStarted();
    killall.start("sudo pkill rfcomm");
    QString output;
    while(killall.waitForReadyRead(10000)) {
        output+=killall.readAll();
    }
    cout<<output.toStdString()<<endl;
}


void MainWindow::on_actionPreferencias_triggered()
{
    getPreferencesSettingsFile();
    pref->setCalibDir(root_calib_path);
    pref->setConfFile(initfile);
    pref->setDegugConsoleValue(debug);
    pref->setLogFileValue(log);
    pref->setDebugStdOutValue(stdout_mode);

    int ret = pref->exec();
    bool debug_console = pref->getDegugConsoleValue();
    bool log_file = pref->getLogFileValue();
    bool stdout_pref = pref->getDebugStdOutValue();
    QString file = pref->getInitFileConfigPath();
    QString calib_path = pref->getCalibDirectoryPath();

    if(ret == QDialog::Accepted)
    {
        setDebugMode(debug_console);
        setLogMode(log_file);
        setStdOutDebugMode(stdout_pref);
        QString boolDebugText = debug_console ? "true" : "false";
        QString boolLogText = log_file ? "true" : "false";
        QString boolStdOutText = stdout_pref ? "true" : "false";
        setPreferencesSettingsFile("Modo", "debug", boolDebugText );
        setPreferencesSettingsFile("Modo", "log", boolLogText );
        setPreferencesSettingsFile("Modo", "stdout", boolStdOutText );
        setPreferencesSettingsFile("Paths", "conf_set_file", file);
        setPreferencesSettingsFile("Paths", "calib_set_file", calib_path);
        getPreferencesSettingsFile();
        writeDebugToStdOutLogFile();
    }
}
