#include "inc/SetPreferences.h"
#include "ui_SetPreferences.h"
#include "qprocess.h"

/**
 * @brief SetPreferences::SetPreferences
 *
 * Constructor de la clase.
 * @param parent
 */
SetPreferences::SetPreferences(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetPreferences)
{
    ui->setupUi(this);
}
/**
 * @brief SetPreferences::accept
 * @overload
 */
void SetPreferences::accept()
{
    QString NombreBT=ui->comboBox_ID->currentText();
    int i=ui->comboBox_ID->currentIndex();

    if(!NombreBT.contains("HC")){
        QMessageBox::critical(this,tr("Atención: tiene que setear un HC-05 ó un HC-06"),tr("vuelva a elegir un dispositivo valido"));

        return;
    }
    QList<QString>bluetooth=NombreBT.split(" ");

    QString BT=bluetooth.at(1);
    BT.replace("(","");
    BT.replace(")","");
    QProcess killall;
    killall.waitForStarted();
    killall.start("sudo rfcomm connect 0 "+BT +" 1");
    QString output;
    while(killall.waitForReadyRead(100000)) {
        output+=killall.readAll();
    }

    QDialog::accept();
}
/**
 * @brief SetPreferences::reject
 * @overload
 */
void SetPreferences::reject()
{
//    ui->checkBox_Debug->setChecked(debconsole);
//    ui->checkBox_Log->setChecked(logfile);
//    ui->checkBox_StdOut->setChecked(stdoutmode);
    QDialog::reject();
}
/**
 * @brief SetPreferences::exec
 * @overload
 */
int SetPreferences::exec()
{
//    ui->checkBox_Debug->setChecked(debconsole);
//    ui->checkBox_Log->setChecked(logfile);
//    ui->checkBox_StdOut->setChecked(stdoutmode);
//    ui->lineEdit_config_file->setText(initfile);
//    ui->lineEdit_config_calib->setText(root_calib_path);
    QDialog::exec();
}

/**
 * @brief SetPreferences::~SetPreferences
 *
 * Destructor de la clase.
 */
SetPreferences::~SetPreferences()
{
    delete ui;
}

/**
 * @brief SetPreferences::openConfigurationFile
 * @return
 */
QString SetPreferences::openConfigurationFile(bool dir)
{
    QString filename;

    if(!dir)
    {
        filename = QFileDialog::getOpenFileName(this, tr("Abrir archivo de configuración"),
                                                QDir::homePath(),
                                                tr("Configuración (*.ini);;Texto (*.txt);;Todos (*.*)"));
    }
    else
    {
        filename = QFileDialog::getExistingDirectory(this, tr("Seleccionar el directorio de configuración"),
                                                     QDir::homePath());
    }

    return filename;
}
/**
 * @brief SetPreferences::on_pushButton_open_config_file_clicked
 */
//void SetPreferences::on_pushButton_open_config_file_clicked()
//{
//    initfile = openConfigurationFile();
//    //ui->lineEdit_config_file->setText(initfile);
//}
/**
 * @brief SetPreferences::on_pushButton_open_config_calib_clicked
 */
//void SetPreferences::on_pushButton_open_config_calib_clicked()
//{
//    root_calib_path = openConfigurationFile(true);
//    //ui->lineEdit_config_calib->setText(root_calib_path);
//}

void SetPreferences::on_pushButton_clicked()
{

    ui->comboBox_ID->clear();
    QProcess killall;
    killall.waitForStarted();
    killall.start("bt-device -l");
    QString output;
    while(killall.waitForReadyRead(1000)) {
        output+=killall.readAll();
    }
    ListaBT.clear();
    ListaBT.append(output.split('\n'));

    ui->comboBox_ID->addItems(ListaBT);
//    if(output.contains("20:16:02:30:99:61"))
//    {
//        killall.execute("sudo rfcomm connect 0 20:16:02:30:99:61 1");
//        killall.waitForFinished(1000);
//        //cout<<killall.readAll().toStdString()<<endl;
//    }
}
