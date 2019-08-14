#include "tools.h"

Tools::Tools(QObject *parent) : QObject(parent)
{

}

void Tools::RAWdataOut(QPlainTextEdit *plainText, const QByteArray &data, const int str_length)
{
    QString str;
    for (int i = 0; i < data.length(); i++){
        if ((i % str_length == 0) && (i != 0))
        {
            plainText->appendPlainText(str);
            str.clear();
        }
        str.push_back(QString("%1 ").arg(quint8(data.at(i)), 2, 16, QChar('0')));
    }
    if (!str.isEmpty())
        plainText->appendPlainText(str);
}

int Tools::lineEditIntValueCheck(QLineEdit &LineEdit, const int min, const int max, QString label)
{
    int buf = 0;
    bool flag = false;

    buf = LineEdit.text().toInt(&flag);

    if (!flag)
    {
        qDebug() << "Input value '" << label << "' incorrected";
        buf = min;
    }
    if (buf < min)
    {
        qDebug() << "Input value '" << label << "' less than the lower limit";
        buf = min;
        LineEdit.setText(QString::number(min));
    }
    if (buf > max)
    {
        qDebug() << "Input value '" << label << "' is greater than the upper limit";
        buf = max;
        LineEdit.setText(QString::number(max));
    }
    return buf;
}

double Tools::lineEditDoubleValueCheck(QLineEdit &LineEdit, const double min, const double max, QString label)
{
    double buf = 0;
    bool flag = false;

    buf = LineEdit.text().toDouble(&flag);

    if (!flag)
    {
        qDebug() << "Input value '" << label << "' incorrected";
        buf = min;
    }
    if (buf < min)
    {
        qDebug() << "Input value '" << label << "' less than the lower limit";
        buf = min;
        LineEdit.setText(QString::number(min, 'f', 2));
    }
    if (buf > max)
    {
        qDebug() << "Input value '" << label << "' is greater than the upper limit";
        buf = max;
        LineEdit.setText(QString::number(max, 'f', 2));
    }
    return buf;
}

quint64 Tools::RAWToNumberMSB(const QByteArray &data, quint16 data_pos, quint8 data_length)
{
    if (data_length > 7) {return 0;}

    quint16 msb_addr = data_pos;
    quint64 buf = quint8(data.at(msb_addr));

    for (int i = 1; i < data_length; i++)
    {
        buf = (buf << 8) | quint8(data.at(msb_addr + i));
    }

    return buf;
}

quint64 Tools::RAWToNumberLSB(const QByteArray &data, quint16 data_pos, quint8 data_length)
{
    if (data_length > 7) {return 0;}

    quint16 msb_addr = data_pos + data_length - 1;
    quint64 buf = quint8(data.at(msb_addr));

    for (int i = 1; i < data_length; i++)
    {
        buf = (buf << 8) | quint8(data.at(msb_addr - i));
    }

    return buf;
}

QByteArray Tools::NumberToRAWLSB(const quint64 num, const int length)
{
    QByteArray array;
    if (length > 7) {return array;}

    char buf = 0;

    for (int i = 0; i < length; i++){
        buf = char((num >> (8 * i) ) & 0xFF);
        array.push_back(buf);
    }

    return array;
}

void Tools::delay(const int dealay_ms)
{
    QTime time;
    time.start();
    for(;time.elapsed() < dealay_ms;)
    {
        qApp->processEvents();
    }
}

QByteArray Tools::NumberToRAWMSB(const quint64 num, const int length)
{
    QByteArray array;
    if (length > 7) {return array;}

    char buf = 0;

    for (int i = (length - 1); i >= 0; i--){
        buf = char((num >> (8 * i) ) & 0xFF);
        array.push_back(buf);
    }

    return array;
}
