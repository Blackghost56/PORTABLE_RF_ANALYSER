#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QMap>
#include <QDebug>
#include <QHBoxLayout>
#include "com_core.h"
#include "descriptor.h"
#include "plot/qcustomplot.h"
#include "tools/tools.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
private:
    const quint8            prefix = 0x40;
    const int               sizeFloat = sizeof(float);
    const qint32            BaudRate_default = 9600;
    const int               UpdateInterval_ms = 1000;
    const QString           portButtonTextOn = tr("Connected");
    const QString           portButtonTextOff = tr("Disconnect");
    enum STATE_DEVICE : bool {STATE_ON = true, STATE_OFF = false};
    const QString           stateButtonTextOn = tr("Stop");
    const QString           stateButtonTextOff = tr("Start");
    static constexpr const double            ADC_REF_V = 3.3;       // V
    static constexpr const double            ADC_BIT = 4096;
    static constexpr const double            ADC_SCALE = ADC_REF_V/ADC_BIT;
    static constexpr const int               ADC_TIME_DISCRETE_NS = 140;  //ns

    QTimer                  *pTimerUpdate;
    QStringList             portList;
    QMap<QString, qint32>   baudRateMap;
    qint32                  baudRate;
    QString                 port;
    COM_core                *pCOMCore;
    bool                    stateDevice = STATE_OFF;
    QCustomPlot             *pPlot;
    QVector<double>         ADCData;
    quint16                 ADCBufSize;         // 2 byte 1 point (real size in byte x2)



    void buildBaudRateMap();
    void connectPort();
    void disconnectPort();
    void RAWdataOut(const QByteArray &data, const int str_length = 50);
    void parser(const QByteArray &data);

    void parserData(const QByteArray &data);
    void parserDATAADCBufSize(const QByteArray &data);
    void parserDATAADC(const QByteArray &data);

    void parserInfo(const QByteArray &data);
    void parserInfoADCState(const bool &state);

    void drawPlot();
    void requestADCBufSize();
    void setADCBufSize(const int size);
    void requestADCState();


    union
    {
        float _float;
        char _char[sizeof(float)];
    } converter;

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
private slots:
    void timeoutTimerHandler();
    void on_BaudRate_comboBox_currentIndexChanged(const QString &arg1);
    void on_Port_comboBox_currentIndexChanged(const QString &arg1);
    void on_Port_pushButton_clicked();
    void readyRead(QByteArray &data);
    void on_Start_pushButton_clicked();
    void on_Time_lineEdit_editingFinished();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // WIDGET_H
