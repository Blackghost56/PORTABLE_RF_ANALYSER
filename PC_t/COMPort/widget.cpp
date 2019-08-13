#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->Port_pushButton->setText(portButtonTextOn);

    buildBaudRateMap();
    baudRate = BaudRate_default;

    pCOMCore = new COM_core(this);
    connect(pCOMCore, &COM_core::readyRead, this, &Widget::readyRead);

    pTimerUpdate = new QTimer(this);
    pTimerUpdate->setInterval(UpdateInterval_ms);
    connect(pTimerUpdate, SIGNAL(timeout()), this, SLOT(timeoutTimerHandler()));
    pTimerUpdate->start();

    pPlot = new QCustomPlot;
    QHBoxLayout *pLay = new QHBoxLayout;
    pLay->addWidget(pPlot);
    ui->Plot_widget->setLayout(pLay);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::buildBaudRateMap()
{
    QStringList BaudRatelist;
    BaudRatelist.push_back("1200");
    baudRateMap.insert(BaudRatelist.last(), 1200);
    BaudRatelist.push_back("2400");
    baudRateMap.insert(BaudRatelist.last(), 2400);
    BaudRatelist.push_back("4800");
    baudRateMap.insert(BaudRatelist.last(), 4800);
    BaudRatelist.push_back("9600");
    baudRateMap.insert(BaudRatelist.last(), 9600);
    BaudRatelist.push_back("19200");
    baudRateMap.insert(BaudRatelist.last(), 19200);
    BaudRatelist.push_back("38400");
    baudRateMap.insert(BaudRatelist.last(), 38400);
    BaudRatelist.push_back("57600");
    baudRateMap.insert(BaudRatelist.last(), 57600);
    BaudRatelist.push_back("115200");
    baudRateMap.insert(BaudRatelist.last(), 115200);

    ui->BaudRate_comboBox->addItems(BaudRatelist);
    ui->BaudRate_comboBox->setCurrentIndex(3);
}

void Widget::timeoutTimerHandler()
{
    ui->PortInfo_plainTextEdit->clear();
    QStringList     bufPortList;
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        QString s = QObject::tr("Port: ") + info.portName() + "\n"
                    + QObject::tr("Location: ") + info.systemLocation() + "\n"
                    + QObject::tr("Description: ") + info.description() + "\n"
                    + QObject::tr("Manufacturer: ") + info.manufacturer() + "\n"
                    + QObject::tr("Serial number: ") + info.serialNumber() + "\n"
                    + QObject::tr("Vendor Identifier: ") + (info.hasVendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : QString()) + "\n"
                    + QObject::tr("Product Identifier: ") + (info.hasProductIdentifier() ? QString::number(info.productIdentifier(), 16) : QString()) + "\n"
                    + QObject::tr("Busy: ") + (info.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) + "\n";

        ui->PortInfo_plainTextEdit->appendPlainText(s);
        bufPortList.push_back(info.portName());
    }

    if (portList != bufPortList)
    {
        portList = bufPortList;
        //qDebug() << "port list: " << portList << "\tport: " << port;
        if(!portList.contains(port)) {
            //qDebug() << "disconnect";
            disconnectPort();
        }
        ui->Port_comboBox->clear();
        ui->Port_comboBox->addItems(portList);       
    }
}

void Widget::on_BaudRate_comboBox_currentIndexChanged(const QString &arg1)
{
    if (arg1.isEmpty()){
        //qDebug() << "Incorrect baud rate";
        return;
    }
    baudRate = baudRateMap[arg1];
    ui->Log_plainTextEdit->appendPlainText("Baud rate: " + QString::number(baudRate));
    qDebug() << "Baud rate: " << baudRate;
}

void Widget::on_Port_comboBox_currentIndexChanged(const QString &arg1)
{
    if (arg1.isEmpty()){
        //qDebug() << "Incorrect port";
        return;
    }
    port = arg1;
    ui->Log_plainTextEdit->appendPlainText("Port: " + port);
    qDebug() << "Port: " << port;
}

void Widget::connectPort()
{
    ui->Port_pushButton->setText(portButtonTextOff);
    pCOMCore->initialization(port, baudRate);
    pCOMCore->open(QIODevice::ReadWrite);
}

void Widget::disconnectPort()
{
    ui->Port_pushButton->setText(portButtonTextOn);
    ui->Port_pushButton->setChecked(false);
    pCOMCore->close();
}

void Widget::on_Port_pushButton_clicked()
{
    if (ui->Port_pushButton->isChecked())
    {
        connectPort();
        requestADCBufSize();
        Tools::delay(5);
        requestADCState();
    } else {
        disconnectPort();
    }
}

void Widget::readyRead(QByteArray &data)
{
    //qDebug() << data;
    RAWdataOut(data);
    parser(data);

}

void Widget::RAWdataOut(const QByteArray &data, const int str_length)
{
    QString str;
    for (int i = 0; i < data.length(); i++){
        if ((i % str_length == 0) && (i != 0))
        {
            //qDebug() << str;
            ui->Log_plainTextEdit->appendPlainText(str);
            str.clear();
        }
        str.push_back(QString("%1 ").arg(quint8(data.at(i)), 2, 16, QChar('0')));
    }
    if (!str.isEmpty())
        ui->Log_plainTextEdit->appendPlainText(str);
    //qDebug() << str;
}

void Widget::parser(const QByteArray &data)
{
    if (data.length() >= 1){
        switch (uint8_t(data.at(0))) {
            case USB_0_CMD:
            break;
            case USB_0_DATA:
                parserData(data);
            break;
            case USB_0_INFO:
                parserInfo(data);
            break;
            case USB_0_ERROR:
            break;
        }
    }
}

void Widget::parserData(const QByteArray &data)
{
    if (data.length() >= 2){
        switch (uint8_t(data.at(1))) {
            case USB_1_DATA_ADC_BUF_SIZE:
                parserDATAADCBufSize(data);
            break;
            case USB_1_DATA_ADC_CONTCONV:
                parserDATAADC(data);
            break;
        }
    }
}

void Widget::parserDATAADCBufSize(const QByteArray &data)
{
    if (data.length() < 4){
        return;
    }
    QByteArray buf = data.mid(2, 4);
    ADCBufSize = quint16(Tools::RAWToNumberMSB(buf, 0, 2));
    ui->Time_lineEdit->setText(QString::number(ADCBufSize * ADC_TIME_DISCRETE_NS / 1000.0));
}

void Widget::parserDATAADC(const QByteArray &data)
{
    if (data.length() <= 2){
        return;
    }
    QByteArray buf = data.mid(2, data.length());
    ADCData.clear();
    if (buf.length() % 2 != 0)
        return;
    for (quint16 i = 0; i < buf.length(); i+=2)
        ADCData.push_back(double(quint16(Tools::RAWToNumberLSB(buf, i, 2))) * ADC_SCALE);
    qDebug() << ADCData;
    drawPlot();
}

void Widget::parserInfo(const QByteArray &data)
{
    if (data.length() >= 2){
        switch (uint8_t(data.at(1))) {
        case USB_1_INFO_RX_FIFO_CLEAR:

            break;
        case USB_1_INFO_ADC_SET_BUF_SIZE_OK:

            break;
        case USB_1_INFO_ADC_STATE_ON:
                parserInfoADCState(STATE_ON);
            break;
        case USB_1_INFO_ADC_STATE_OFF:
                parserInfoADCState(STATE_OFF);
            break;
        }
    }
}

void Widget::parserInfoADCState(const bool &state)
{
    if (state == STATE_ON){
        stateDevice = STATE_ON;
        ui->Start_pushButton->setText(stateButtonTextOn);
        ui->Time_lineEdit->setEnabled(false);
    } else {
        stateDevice = STATE_OFF;
        ui->Start_pushButton->setText(stateButtonTextOff);
        ui->Time_lineEdit->setEnabled(true);
    }
}

void Widget::on_Start_pushButton_clicked()
{
    QByteArray buf;
    if (stateDevice == STATE_ON){
        stateDevice = STATE_OFF;
        ui->Start_pushButton->setText(stateButtonTextOff);
        buf.push_back(char(USB_0_CMD));
        buf.push_back(char(USB_1_CMD_ADC_CONTCONV_STOP));
        pCOMCore->write(buf);
        ui->Time_lineEdit->setEnabled(true);
    } else {
        stateDevice = STATE_ON;
        ui->Start_pushButton->setText(stateButtonTextOn);
        buf.push_back(char(USB_0_CMD));
        buf.push_back(char(USB_1_CMD_ADC_CONTCONV_START));
        pCOMCore->write(buf);
        ui->Time_lineEdit->setEnabled(false);
    }
}

void Widget::drawPlot()
{
    //qDebug() << "Draw plot";
    if (pPlot->isHidden())
        return;

    pPlot->clearGraphs();

    // --- Legend ---
    /*pPlot->legend->setVisible(true);
    pPlot->legend->setBrush(QBrush(QColor(200, 200, 200, 127)));
    pPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignVCenter|Qt::AlignRight);*/
    // --------------

    // --- Draw ---
    // Time
    QVector<double> time;
    int timeLength = ADCData.length();
    double ADC_time_us = double(ADC_TIME_DISCRETE_NS) / 1000.0;
    for (auto i = 0; i < timeLength; i++){
        time.push_back(i * ADC_time_us);
    }

    pPlot->addGraph();
    QPen RF_pen;
    RF_pen.setWidth(2);
    RF_pen.setColor(QColor(0, 0, 200));
    pPlot->graph(0)->setPen(RF_pen);
    pPlot->graph(0)->setName(tr("ADC"));
    pPlot->graph(0)->setData(time, ADCData);
    // -----------------

    // --- Set Axis ---
    pPlot->xAxis->setLabel(tr("us"));
    double minX = 0;
    double maxX = time.length() * ADC_time_us;
    pPlot->xAxis->setRange(minX, maxX);
    pPlot->yAxis->setLabel(tr("V"));
    double minY = 0;
    double maxY = ADC_REF_V;
    pPlot->yAxis->setRange(minY, maxY);
    // ----------------

    pPlot->replot();
}

void Widget::requestADCBufSize()
{
    QByteArray buf;
    buf.push_back(char(USB_0_CMD));
    buf.push_back(char(USB_1_CMD_ADC_REQUEST_BUF_SIZE));
    pCOMCore->write(buf);
}

void Widget::setADCBufSize(const int size)
{
    QByteArray buf;
    buf.push_back(char(USB_0_DATA));
    buf.push_back(char(USB_1_DATA_ADC_BUF_SIZE));
    buf.push_back(Tools::NumberToRAWMSB(quint64(size), 2));
    pCOMCore->write(buf);
    Tools::delay(5);
    requestADCBufSize();
}

void Widget::requestADCState()
{
    QByteArray buf;
    buf.push_back(char(USB_0_CMD));
    buf.push_back(char(USB_1_CMD_ADC_REQUEST_STATE));
    pCOMCore->write(buf);
}

void Widget::on_Time_lineEdit_editingFinished()
{
    double buf = Tools::lineEditDoubleValueCheck(*ui->Time_lineEdit, 1.0, 200.0);
    ADCBufSize = quint16(buf * 1000.0 / double(ADC_TIME_DISCRETE_NS));
    setADCBufSize(ADCBufSize);
}

void Widget::on_pushButton_clicked()
{
    ui->Log_plainTextEdit->clear();
}

void Widget::on_pushButton_2_clicked()
{
    QString date = QDateTime::currentDateTimeUtc().toString("dd_MM_yyyy__hh_mm_ss_UTC");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "Log_" + date + ".txt", tr("Text files (*.txt)"));

    if (fileName == "") return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "Error opening file";
        return;
    }

    QTextStream out(&file);

    out << ui->Log_plainTextEdit->toPlainText();

    file.close();
}
