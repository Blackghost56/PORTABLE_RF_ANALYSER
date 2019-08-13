#include "com_core.h"

COM_core::COM_core(QObject *parent) : QObject(parent)
{
    connect(&serialPort, &QSerialPort::readyRead, this, &COM_core::handleReadyRead);
    connect(&serialPort, &QSerialPort::bytesWritten, this, &COM_core::handleBytesWritten);
    connect(&serialPort, &QSerialPort::errorOccurred, this, &COM_core::handleError);
}

void COM_core::initialization(const QString portName, const qint32 baudRate, const QSerialPort::DataBits dataBits, const QSerialPort::StopBits stopBits, const QSerialPort::Parity parity)
{
    if (!serialPort.isOpen())
    {
       serialPort.setPortName(portName);
       serialPort.setBaudRate(baudRate);
       serialPort.setDataBits(dataBits);
       serialPort.setStopBits(stopBits);
       serialPort.setParity(parity);
    }
}

bool COM_core::open(QIODevice::OpenMode mode)
{
    bool state = serialPort.open(mode);
    if (!state)
        qDebug() << "Failed to open port";

    return state;
}

void COM_core::close()
{
    serialPort.close();
}

void COM_core::write(const QByteArray &data)
{
    const qint64 bytesWritten = serialPort.write(data);
    g_bytesWritten = data.length();

    if (bytesWritten == -1) {
        qDebug() << tr("Failed to write the data to port %1, error: %2").arg(serialPort.portName()).arg(serialPort.errorString());
    } else if (bytesWritten != data.size()) {
        qDebug() << tr("Failed to write all the data to port %1, error: %2").arg(serialPort.portName()).arg(serialPort.errorString());
    }
}

void COM_core::handleReadyRead()
{
    QByteArray data = serialPort.readAll();

    emit readyRead(data);
}

void COM_core::handleBytesWritten(qint64 bytes)
{
    if (bytes == g_bytesWritten) {
        g_bytesWritten = -1;
        qDebug() << tr("Data successfully sent to port %1").arg(serialPort.portName());
    }
}

void COM_core::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        qDebug() << tr("An I/O error occurred while reading the data from port %1, error: %2").arg(serialPort.portName()).arg(serialPort.errorString());
    }
}
