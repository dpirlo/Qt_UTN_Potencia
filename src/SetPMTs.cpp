#include "inc/SetPMTs.h"
#include "ui_SetPMTs.h"

/**
 * @brief SetPMTs::SetPMTs
 *
 * Constructor de la clase.
 * @param parent
 */
SetPMTs::SetPMTs(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SetPMTs)
{
  ui->setupUi(this);
  SetPMTPushButtonList();
}

/**
 * @brief SetPMTs::~SetPMTs
 *
 * Destructor de la clase.
 */
SetPMTs::~SetPMTs()
{
  delete ui;
}

/**
 * @brief SetPMTs::SetPMTPushButtonList
 *
 * Método privado que llena una lista con los QPushButton's correspondientes a los fotomultiplicadores (48 PMTs).
 */
void SetPMTs::SetPMTPushButtonList()
{

}

/**
 * @brief SetPMTs::ConfigurePMTList
 *
 * Método público que configura el estado actual de la lista de fotomultiplicadores seleccionados en función del botón presionado.
 *
 * @see pmt_selected_list
 */
void SetPMTs::ConfigureImage(QString foto)
{

//    QString filename = ":/qss_icons/rc/1.png";
    QPixmap image;
    image.load(":/qss_icons/rc/"+foto+".png");
//    //pmt_select->ConfigureImage(filename);

    ui->foto->setPixmap((image));
    ui->foto->setScaledContents(true);
    ui->foto->show();

}
/**
 * @brief SetPMTs::accept
 * @overload
 */
void SetPMTs::accept()
{   
  //ConfigurePMTList();

  QDialog::accept();
}
/**
 * @brief SetPMTs::reject
 * @overload
 */
void SetPMTs::reject()
{
  QDialog::reject();
}
/**
 * @brief SetPMTs::ClearPMTBoard
 *
 * Método privado que elimina la selección actual de PMTs.
 *
 */
void SetPMTs::ClearPMTBoard()
{

}
/**
 * @brief SetPMTs::SetAllPMTBoard
 *
 * Método privado que selecciona la totalidad de PMTs.
 *
 */
void SetPMTs::SetAllPMTBoard()
{

}
/**
 * @brief SetPMTs::on_pushButton_select_all_clicked
 */
void SetPMTs::on_pushButton_select_all_clicked()
{
  SetAllPMTBoard();
}
/**
 * @brief SetPMTs::on_pushButton_select_none_clicked
 */
void SetPMTs::on_pushButton_select_none_clicked()
{
  ClearPMTBoard();
}
