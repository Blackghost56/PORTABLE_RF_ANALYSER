#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#include <QObject>


#define USB_0_CMD 			0xAA
#define USB_0_DATA 			0x55
#define USB_0_INFO 			0x95
#define USB_0_ERROR 		0xA5

#define USB_1_CMD_ADC_CONTCONV_START                            0x00
#define USB_1_CMD_ADC_CONTCONV_STOP                             0x01
#define USB_1_CMD_ADC_REQUEST_BUF_SIZE                          0x02
#define USB_1_CMD_ADC_REQUEST_STATE                             0x03

#define USB_1_DATA_ADC_BUF_SIZE 								0x00
#define USB_1_DATA_ADC_CONTCONV 								0x01

#define USB_1_INFO_RX_FIFO_CLEAR								0x00
#define USB_1_INFO_ADC_SET_BUF_SIZE_OK                          0x01
#define USB_1_INFO_ADC_STATE_ON									0x02
#define USB_1_INFO_ADC_STATE_OFF								0x03

#define USB_1_ERROR_ADC_CONTCONV_START                          0x00
#define USB_1_ERROR_ADC_CONTCONV_STOP                           0x01
#define USB_1_ERROR_ADC_SET_BUF_SIZE_ADCRUN                     0x02
#define USB_1_ERROR_ADC_SET_BUF_SIZE_UPPERLIMIT                 0x03
#define USB_1_ERROR_ADC_SET_BUF_SIZE_LOWERLIMIT                 0x04
#define USB_1_ERROR_USB_FIFO_OVERLOAD                           0x05
#define USB_1_ERROR_COMMON							 			0x06

class Descriptor : public QObject
{
    Q_OBJECT
public:
    explicit Descriptor(QObject *parent = nullptr);


    /*enum DATA_0 : quint8 {CMD = 0xAA, DATA = 0x55, INFO = 0x95, ERROR = 0xA5};

    enum CMD : quint8 {ADC_CONTCONV_START = 0x00,
                       ADC_CONTCONV_STOP,
                       ADC_REQUEST_BUF_SIZE};

    enum DATA : quint8 {ADC_BUF_SIZE = 0x00,
                       ADC_CONTCONV};

    enum INFO : quint8 {RX_FIFO_CLEAR = 0x00,
                       ADC_SET_BUF_SIZE_OK};

    enum ERROR : quint8 {ADC_CONTCONV_START = 0x00,
                       ADC_CONTCONV_STOP};*/
signals:

public slots:
};

#endif // DESCRIPTOR_H
