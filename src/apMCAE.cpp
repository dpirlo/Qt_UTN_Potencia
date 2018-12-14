#include "inc/apMCAE.hpp"

using namespace ap;

/**
 * @brief MCAE::MCAE
 *
 * Constructor de la clase
 *
 * Solo admite como parámetro el tiempo que debe esperar el servicio de comunicación antes de emitir una excepción por _time out_. El tiempo por defecto es de 1 segundo.
 *
 * @note Se documentan las propiedades más importantes.
 *
 * @brief PortBaudRate:
 * Velocidad de funcionamiento de la comunicación con el equipo
 *
 * @brief PSOC_ADC:
 * Tensión por cada unidade de ADC
 * 1 Unidad de ADC = 5.8823 V
 *
 * @param timeout
 */
MCAE::MCAE(size_t timeout)
    :port(serial_port_ptr(new serial_port(io))),
      timeout(timeout),
      read_error(true),
      timer(port->get_io_service()),
      PortBaudRate(921600),
      AnsAP_ON("ON"),
      AnsAP_OFF("OFF"),
      AP_ON(""),
      AP_OFF(""),
      AP_STATUS(""),
      Head_MCAE("#C"),
      Head_MCA("@"),
      End_MCA("\r"),
      End_PSOC("\r\n"),

      /*Funciones trama MCAE*/
      FunCHead("01"),
      FunCSP3("02"),
      FunCPSOC("03"),

      /*Funciones de Tablas*/
      Head_Calib_Coin("&"),
      Energy_Calib_Table("001"),
      X_Calib_Table("002"),
      Y_Calib_Table("003"),
      Window_Limits_Table("004"),
      AnsEnergy_Calib_Table("&101"),
      AnsX_Calib_Table("&102"),
      AnsY_Calib_Table("&103"),
      AnsWindow_Limits_Table("&104"),
      Generic_Received_Size("0001"),
      Generic_Sended_Size("01"),

      /*Funciones trama Coincidencia*/
      Init_Coin("701"),
      Window_Time_Coin("702"),
      Select_Mode_Coin("703"),
      Normal_Coin_Mode("333333333"),
      Auto_Coin_Mode("021120120"),
      Head_Coin("7"),
      Calib_Mode("6411"),

      /*Funciones trama PSOC*/
      PSOC_OFF("$SET,STA,OFF"),
      PSOC_ON("$SET,STA,ON"),
      PSOC_SET("$SET,VCON,"),
      PSOC_STA("$TEMP"),
      PSOC_ANS("$OK"),
      PSOC_ADC(5.8823),
      PSOC_SIZE_SENDED("14"),
      PSOC_SIZE_RECEIVED("0031"),
      PSOC_SIZE_RECEIVED_ALL("0004"),

      /*Funciones trama MCA*/
      AnsMultiInit("@0064310>"),
      AnsHeadInit("@0064020<"),
      BrCst("00"),
      Init_Calib_MCAE("64"),
      Init_MCA("6401"),
      Data_MCA("65"),
      SetHV_MCA("68"),
      SetEscDes("71"),
      SetFir("76"),
      
      SetHVMCA_MCA("67"),
      Temp_MCA("74000"),
      Set_Time_MCA("80"),
      Rate_MCA("0060"),
      Rate_MCA_Demo("0061"),

      /* ENCABEZADO DE ENCENDIDO Y APAGADO DE CABEZALES*/
      Cab_On_Off("#C701090009$CAB")

{
    /* Testing */
}
/**
 * @brief MCAE::~MCAE
 *
 * Destructor de la clase MCAE
 *
 */
MCAE::~MCAE()
{
    portDisconnect();
}
/**
 * @brief MCAE::isPortOpen
 * @return Si está abierto responde _true_
 */
bool MCAE::isPortOpen()
{
    return port->is_open();
}
/**
 * @brief MCAE::portConnect
 *
 * Conexión del objeto puerto serie _port_
 *
 * @param tty_port_name
 * @return Código de error
 */
boost::system::error_code MCAE::portConnect(const char *tty_port_name)
{
    boost::system::error_code ec;
    port->open(tty_port_name);
    if (ec.value()==0)
        port->set_option(serial_port_base::baud_rate(PortBaudRate));

    return ec;
}
/**
 * @brief MCAE::portDisconnect
 *
 * Desconexión del objeto puerto serie _port_
 *
 * @return Código de error
 */
std::error_code MCAE::portDisconnect()
{
    std::error_code ec;
    if (port->is_open())
        port->close();

    return ec;
}
/**
 * @brief MCAE::portWrite
 *
 * Escritura sobre el objeto puerto serie _port_
 *
 * @param msg
 * @param tty_port_name
 * @return Tamaño de la trama en bytes
 */
size_t MCAE::portWrite(string *msg, const char *tty_port_name)
{
    char c_msg[msg->size()+1];
    strcpy(c_msg, msg->c_str());
    size_t bytes_transferred = port->write_some(boost::asio::buffer(c_msg,msg->size()));

    return bytes_transferred;
}
/**
 * @brief MCAE::portRead
 *
 * Lectura sobre el objeto puerto serie _port_ y el resultado se guarda en un puntero a _string_
 *
 * @param msg
 * @param buffer_size
 * @return Tamaño de la trama en bytes
 */
size_t MCAE::portRead(string *msg, int buffer_size)
{
    char c_msg[buffer_size];
    size_t bytes_transferred = port->read_some(boost::asio::buffer(c_msg,buffer_size));
    msg->assign(c_msg);

    return bytes_transferred;
}
/**
 * @brief MCAE::portRead
 * @overload
 *
 * Lectura sobre el objeto puerto serie _port_ y el resultado se guarda en un puntero a _char_
 *
 * @param c_msg
 * @return Tamaño de la trama en bytes
 */
size_t MCAE::portRead(char *c_msg)
{
    int buffer_size=1;
    size_t bytes_transferred = port->read_some(boost::asio::buffer(c_msg,buffer_size));

    return bytes_transferred;
}


/**
 * @brief MCAE::portReadComplete
 *
 * Lectura completa sobre el objeto puerto serie _port_ hasta agotar el temporizador
 *
 * @param error
 * @param bytes_transferred
 */
void MCAE::portReadComplete(const boost::system::error_code& error,
                            size_t bytes_transferred)
{
    read_error = (error || bytes_transferred == 0);
    timer.cancel();
}
/**
 * @brief MCAE::portTimeOut
 *
 * Cuando se llama a este método cancela la actividad sobre el objeto _port_
 *
 * @param error
 */
void MCAE::portTimeOut(const boost::system::error_code& error)
{
    if (error) { return; }
    port->cancel();
}
/**
 * @brief MCAE::portReadOneChar
 *
 * Lectura de a un _char_ sobre el objeto puerto serie _port_ y el resultado se guarda en un puntero a _char_
 *
 * @param val
 * @return El estado de la lectura, _true_ si no hubo inconvenientes.
 */
bool MCAE::portReadOneChar(char& val)
{
    char c;
    val = c = '\0';

    port->get_io_service().reset();
    port->async_read_some(boost::asio::buffer(&c, 1),
                          boost::bind(&MCAE::portReadComplete, this,
                                      boost::asio::placeholders::error,
                                      boost::asio::placeholders::bytes_transferred));
    timer.expires_from_now(boost::posix_time::milliseconds(timeout));
    timer.async_wait(boost::bind(&MCAE::portTimeOut,
                                 this, boost::asio::placeholders::error));

    port->get_io_service().run();

    if (!read_error){
        val = c;
    }

    return !read_error;
}
/**
 * @brief MCAE::portReadString
 *
 * Lectura sobre el objeto puerto serie _port_ y el resultado va a un puntero _string_
 *
 * @param msg
 * @param delimeter
 * @param tty_port_name
 */
void MCAE::portReadString(string *msg, char delimeter, const char *tty_port_name)
{
    char c;
    while (portReadOneChar(c) && c != delimeter)
    {
        msg->push_back(c);

    }

    if (c != delimeter)
    {

        Exceptions exception_timeout("Error de tiempo de lectura. TimeOut!");
        throw exception_timeout;
    }
}
/**
 * @brief MCAE::portReadBufferString
 *
 * Lectura de un determinado _buffer_ sobre el objeto puerto serie _port_ y el resultado va a un puntero _string_
 *
 * @param msg
 * @param buffer_size
 * @param tty_port_name
 */
void MCAE::portReadBufferString(string *msg, int buffer_size, const char *tty_port_name)
{
    char c;
    int buffer=1;
    while (portReadOneChar(c) && buffer <= buffer_size)
    {
        msg->push_back(c);
        buffer++;
    }

    if (buffer <= buffer_size)
    {
        Exceptions exception_timeout("Error de tiempo de lectura. TimeOut!");
        throw exception_timeout;
    }
}

/**
 * @brief MCAE::portFlush
 *
 * Limpieza del _buffer_ de lectura del objeto _port_
 *
 * @return Código de error
 */
std::error_code MCAE::portFlush()
{
    std::error_code ec;

    const bool isFlushed =! ::tcflush(port->native(), TCIOFLUSH);
    if (!isFlushed);
       // ec = std::error_code(errno,error::get_system_category());

    return ec;
}
/**
 * @brief MCAE::sendString
 *
 * Envío de la trama 'msg' al puerto serie
 *
 * @param msg
 * @param end
 * @param port_name
 * @return
 */
size_t MCAE::sendString(string msg, string end, string port_name)
{
    portFlush();
    size_t bytes_transfered = 0;
    //cout<<msg<<endl;
    try{
        string sended= msg + end;
        cout<<msg<<endl;
        bytes_transfered = portWrite(&sended, port_name.c_str());
    }
    catch(boost::system::system_error e){
        Exceptions exception_serial_port((string("No se puede acceder al puerto serie. Error: ")+string(e.what())).c_str());
        throw exception_serial_port;
    }

    return bytes_transfered;
}
/**
 * @brief MCAE::readString
 *
 * Lectura del 'buffer' serie hasta recibir el _delimeter_
 *
 * @param delimeter
 * @param port_name
 * @return
 */
string MCAE::readString(char delimeter, string port_name)
{
    string msg;
    try{
        portReadString(&msg,delimeter, port_name.c_str());
    }
    catch( Exceptions & ex ){
        Exceptions exception_stop(ex.excdesc);
        throw exception_stop;
    }
    return msg;
}
/**
 * @brief MCAE::readBufferString
 *
 * Lectura de un buffer de tamaño 'buffer_size'
 *
 * @param buffer_size
 * @param port_name
 * @return
 */
string MCAE::readBufferString(int buffer_size, string port_name)
{
    string msg;
    try{
        portReadBufferString(&msg,buffer_size, port_name.c_str());
    }
    catch( Exceptions & ex ){
        Exceptions exception_stop(ex.excdesc);
        throw exception_stop;
    }
    return msg;
}
/**
 * @brief MCAE::convertHexToDec
 *
 * Método de conversión de Hexadecimal (_string_) a Decimal (_int_)
 *
 * @param hex_number_s
 * @return dec_number
 */
int MCAE::convertHexToDec(string hex_number_s)
{
    bool ok;
    QString hex_number = QString::fromStdString(hex_number_s);
    int dec_number = hex_number.toInt(&ok,16);

    return dec_number;
}
/**
 * @brief MCAE::convertDecToHex
 *
 * Método de conversión de Decimal (_int_) a Hexadecimal (_string_) en minúsculas (abcdef)
 *
 * @param dec_number
 * @return hex_number
 */
string MCAE::convertDecToHex(int dec_number)
{
    QByteArray hex_number = QByteArray::number(dec_number,16);

    return QString(hex_number).toStdString();
}
/**
 * @brief MCAE::convertDecToHexUpper
 *
 * Método de conversión de Decimal (_int_) a Hexadecimal (_string_) en mayúsculas (ABCDEF)
 *
 * @param dec_number
 * @return hex_number
 */
string MCAE::convertDecToHexUpper(int dec_number)
{
    QByteArray hex_number = QByteArray::number(dec_number,16).toUpper();

    return QString(hex_number).toStdString();
}
/**
 * @brief MCAE::getReverse
 *
 * Método de inversión de _bytes_
 *
 * @param seq
 * @return Secuencia _seq_ en sentido inverso
 */
QByteArray MCAE::getReverse(QByteArray seq)
{
    QByteArray reverse;
    for( QByteArray::const_iterator i = seq.constEnd(); i !=seq.constBegin(); )
    {
        --i;
        reverse += *i;
    }

    return reverse;
}



/**
 * @brief MCAE::convertDoubleToInt
 *
 * Conversión de un valor en _double_ a _int_
 * int=(int)round(double)
 *
 * @param value
 * @return value_int
 */
int MCAE::convertDoubleToInt(double value)
{
    int value_int;
    /** @note: Se elimina esta línea: value=value*1000; Modificado en el firmware de FPGA*/

    return value_int=(int)round(value);
}
/**
 * @brief MCAE::convertToTwoComplement
 *
 * Conversión de un valor _double_ negativo a complemento a 2
 *
 * @param value
 * @param two_complement_bits
 * @return Valor en complemento a 2 en Hexadecimal (upper)
 */
string MCAE::convertToTwoComplement(double value, int two_complement_bits)
{
    int value_int = (1 << two_complement_bits) + convertDoubleToInt(value);

    return convertDecToHexUpper(value_int);
}
/**
 * @brief MCAE::convertToTwoComplement
 * @overload
 *
 * Conversión de un valor _int_ negativo a complemento a 2
 *
 * @param value
 * @param two_complement_bits
 * @return Valor en complemento a 2 en Hexadecimal (lower)
 */
string MCAE::convertToTwoComplement(int value, int two_complement_bits)
{
    int value_int = (1 << two_complement_bits) + value;

    return convertDecToHex(value_int);
}
/**
 * @brief MCAE::formatMCAEStreamSize
 *
 * Formato de la trama para envío
 *
 * @param expected_size
 * @param data_stream
 * @return data_stream en formato de envío
 */
string MCAE::formatMCAEStreamSize(int expected_size, string data_stream)
{
    switch (expected_size) {
    case 2:
        if (data_stream.length()==1) data_stream="0" + data_stream;
        break;
    case 3:
        if (data_stream.length()==1) data_stream="00" + data_stream;
        if (data_stream.length()==2) data_stream="0" + data_stream;
        break;
    case 4:
        if (data_stream.length()==1) data_stream="000" + data_stream;
        if (data_stream.length()==2) data_stream="00" + data_stream;
        if (data_stream.length()==3) data_stream="0" + data_stream;
        break;
    default:
        break;
    }

    return data_stream;
}

