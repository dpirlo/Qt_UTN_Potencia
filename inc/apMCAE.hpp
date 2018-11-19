/**
 * @class ap::MCAE
 *
 * @brief Clase MCA Extendido
 *
 * Esta clase provee métodos y propiedades para el manejo del protocolo de comunicación del
 * tomográfo por emisión de positrones AR-PET. A partir del uso de esta clase se puede realizar
 * el mantenimiento, configuración y monitoreo del equipo.
 * La aplicación *qt-mca* utiliza esta clase en conjunto con clases de entorno gráfico de Qt.
 *
 * @author Ariel Hernández
 *
 * @version $Version
 *
 * Contacto: ahernandez@cae.cnea.gov.ar
 *           ariel.h.estevenz@ieee.org
 *
 */
#ifndef APMCAE_H
#define APMCAE_H

#include <boost/asio/serial_port.hpp>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/spirit/include/qi_eol.hpp>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "apExceptions.hpp"

using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace boost::system;

#define DS1820_FACTOR 0.0625
#define CHANNELS 256
#define CHANNELS_PMT 256
#define SERIAL_PORT_READ_BUF_SIZE 1
#define PMTs 48
#define MAX_HV_VALUE 4095
#define MAX_HIGH_HV_VOLTAGE 1390
#define MIN_HIGH_HV_VOLTAGE 700
#define HV_BUFFER_SIZE 3
#define PMT_BUFFER_SIZE 2
#define TIME_BUFFER_SIZE 2
#define RECEIVED_BUFFER_SIZE 4
#define RECEIVED_RATE_BUFFER_SIZE 20
#define SENDED_BUFFER_SIZE 2
#define CS_BUFFER_SIZE 2
#define CS_CALIB_BUFFER_SIZE 3
#define CRLF_SIZE 2
#define COIN_BUFFER_SIZE 2
#define COIN_BYTES_ADV 9
#define MIN_TEMPERATURE 20
#define MAX_POS_X 2048
#define MAX_VEL_X 100
#define MAX_POS_Y 1024
#define MAX_VEL_Y 100
#define MIN_POS_X 0
#define MIN_VEL_X 50
#define MIN_POS_Y 0
#define MIN_VEL_Y 50
#define MAX_ARCHIVOS 99
#define MIN_ARCHIVOS 1
#define MAX_ARCHIVOS_SIZE 1024
#define Canal_Saturados 255
#define MCA_Head_Crudo  "0"
#define MCA_Head_Calib  "50"
#define MCA_PMTs_Calib  50
#define Size_ADQ 0
#define Date_ADQ 4
#define ALMOHADON "49"
#define Time_ADQ 5
#define No_Timeout -1
#define Name_ADQ 2
#define Timeout_10sec 10000
#define Path_ADQ 1
#define Log_ADQ 3
#define Title_ADQ 7
/**
 * @brief serial_port_ptr
 *
 * Definición del tipo serial_port_ptr como un _shared pointer_ de la clase *serial_port*
 */
typedef shared_ptr<serial_port> serial_port_ptr;

/**
 * El Namespace ap contiene los métodos y propiedades relacionados con la comunicación del equipo ARPET.
 */
namespace ap {

  class MCAE
  {
  private:
    /**
         * @brief The string_code enum
         *
         * Definición de enum utilizado para el código de envío de datos serie.
         *
         */
    enum string_code {a,b,c,d,e,f,no_value};

  public:
    /** @note: Se encuentra configurado con un _timeout_ de 50ms por defecto*/
    MCAE(size_t timeout=50);
    void portReadString(string *msg, char delimeter, const char *tty_port_name);
    void portReadBufferString(string *msg, int buffer_size, const char *tty_port_name);
    size_t portWrite(string *msg, const char *tty_port_name);
    size_t sendString(string msg, string end, string port_name);
    string readString(char delimeter, string port_name);
    string readBufferString(int buffer_size, string port_name);
    error_code portFlush();
    error_code portConnect(const char *tty_port_name);
    error_code portDisconnect();
//    void getMCASplitData(string msg_data, int channels);
//    void setMCAEStream(string pmt_dec, int size_stream, string function, string channel_dec="");
//    void setMCAEStream(string pmt_dec, string function, double time);
//    void setMCAEStream(string function, QVector<double> table);
//    void setMCAEStream(string function, string data_one, string data_two="", bool time=false);
//    void setPSOCEStream(string function, string size_received, string psoc_value_dec="");
//    bool verifyMCAEStream(string data_received, string data_to_compare);
//    string getMCA(string pmt, string function, string head, int channels, string port_name);
//    string setHV(string head, string channel_dec, string port_name);
//    string setHV(string head, string pmt, string channel_dec, string port_name);
//    string InitSP3(string head, string port_name);
//    string setFIR(string head,string on_off, string port_name);
//    string setCalibTable(string head, string calib_function, QVector<double> table, string port_name);
//    string setTime(string head, double time_value, string pmt, string port_name);
//    string getTemp(string head, string pmt, string port_name);
//    string getAlmohada(string head, string port_name);
//    vector<int> getRate(string head, string port_name);
//    vector<int> getRateCoin(string head, string port_name);
//    vector<int> getRateCoinDemo(string head, string port_name);
//    string sendCamilla(string command, string port_name);
//    QVector<QString> parserPSOCStream(string stream);
//    double getPMTTemperature(string temp_stream);
    bool isPortOpen();
    ~MCAE();
    string setEscDes(string head,string on_off, string port_name);

    /* Area de métodos en testing */


  private:
    size_t portRead(string *msg, int buffer_size);
    size_t portRead(char *c_msg);
//    string portReadMCAELine();
//    string portReadPSOCLine();
    void portReadComplete(const boost::system::error_code& error, size_t bytes_transferred);
    void portTimeOut(const boost::system::error_code& error);
    bool portReadOneChar(char& val);
//    void setMCAStream(string pmt, string function, string channel="");
//    void setMCAStream(string pmt, string function, double time);
//    void setMCAStream(string function);
//    void setEscDesEStream(string function, string data_one, string data_two);
//    void setFIR_EStream(string function, string data_one, string data_two);
//    void setCalibStream(string function, QVector<double> table);
//    void setCoinStream(string function, string data_one, string data_two="", bool time=false);
//    void setPSOCStream(string function, string psoc_value="");
//    int getMCACheckSum(string data);
//    string getMCAFormatStream(string data);
//    string convertToMCAFormatStream(string data_with_cs);
//    string convertFromMCAFormatStream(string data_with_cs);
//    MCAE::string_code getMCAStringValues(string const& in_string);
//    MCAE::string_code setMCAStringValues(string const& in_string);
//    void getMCAHitsData(QByteArray data_mca);
//    string getHVValueCode(int channel_dec);
//    string getPMTCode(int pmt_dec);
//    /** @todo: Verificar el funcionamiento de este método. */
//    bool portReadCharArray(int nbytes);
    bool verifyStream(string data_received, string data_to_compare);
    string formatMCAEStreamSize(int expected_size, string data_stream);
    bool verifyCheckSum(string data_mca);
    int convertHexToDec(string hex_number);
    string convertDecToHex(int dec_number);
    string convertDecToHexUpper(int dec_number);
//    string getCalibTableFormat(string function, QVector<double> table);
    int convertDoubleToInt(double value);
//    vector<int> parserRateStream(string stream);
//    vector<int> parserRateStreamCoin(string stream);
    string convertToTwoComplement(double value, int two_complement_bits=10);
    string convertToTwoComplement(int value, int two_complement_bits=8);
    QByteArray getReverse(QByteArray seq);

    /* Area de métodos en testing */

  protected:
    /**
         * @brief io
         *
         * Servicio de Input/Output utilizado para la adquisición de datos serie
         */
    io_service io;
    /**
         * @brief port
         *
         * Objeto del tipo _serial_port_ptr_
         *
         * @see serial_port_ptr
         */
    serial_port_ptr port;

  public:
    /**
     * @brief OK
     */
    static const int OK=0000;
    /**
         * @brief FAILED
         */
    static const int FAILED=0001;
    /**
         * @brief FILE_NOT_FOUND
         */
    static const int FILE_NOT_FOUND=0002;

  private:
    string FunCHead, FunCSP3, FunCPSOC, BrCst;
    string Init_Calib_MCAE, Init_MCA, Data_MCA, SetHV_MCA, Temp_MCA, Set_Time_MCA, Rate_MCA, SetHVMCA_MCA,Rate_MCA_Demo,SetFir,SetEscDes;
    string Head_Calib_Coin, Head_MCAE, End_MCA, End_PSOC;
    string Header_MCAE, Trama_MCAE, Trama_MCA, Trama_PSOC, Trama_Calib, Trama_Coin;
    string PSOC_OFF, PSOC_ON, PSOC_SET, PSOC_STA, PSOC_ANS, PSOC_SIZE_SENDED, PSOC_SIZE_RECEIVED, PSOC_SIZE_RECEIVED_ALL;
    string Energy_Calib_Table, X_Calib_Table, Y_Calib_Table, Window_Limits_Table;
    string Init_Coin, Window_Time_Coin, Select_Mode_Coin, Head_Coin, Calib_Mode;
    string Auto_Coin_Mode, Normal_Coin_Mode;
    string Generic_Received_Size, Generic_Sended_Size;
    double PSOC_ADC;
    string init_MCA, MCA, HV;
    string AnsMultiInit, AnsHeadInit;
    string AnsEnergy_Calib_Table, AnsX_Calib_Table, AnsY_Calib_Table, AnsWindow_Limits_Table;
    string AP_ON, AP_OFF, AP_STATUS;
    string AnsAP_ON, AnsAP_OFF,Cab_On_Off;
    size_t timeout;
    /** @todo: Verificar esta propiedad si no es _deprecated_*/
    char * data;
    bool read_error;
    deadline_timer timer;
    int PortBaudRate;
    string Head_MCA;
    long long time_mca;
    int frame, HV_pmt, offset, var, temp;
    QVector<double> channels_id;
    QVector<double> hits_mca;

    /* Area de propiedades en testing */

  public:

    /**
         * @brief getPort
         * @return port
         */
    serial_port_ptr getPort() const { return port; }

  };

}

#endif // APMCAE_H
