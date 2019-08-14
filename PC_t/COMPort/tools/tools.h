#ifndef TOOLS_H
#define TOOLS_H

#include <QObject>
#include <QLineEdit>
#include <QDebug>
#include <QTime>
#include <QApplication>
#include <QPlainTextEdit>

class Tools : public QObject
{
    Q_OBJECT
public:
    explicit Tools(QObject *parent = nullptr);

    static void RAWdataOut(QPlainTextEdit *plainText, const QByteArray &data, const int str_length = 50);
    static int lineEditIntValueCheck(QLineEdit &LineEdit, const int min, const int max, QString label = "");
    static double lineEditDoubleValueCheck(QLineEdit &LineEdit, const double min, const double max, QString label = "");
    static quint64 RAWToNumberMSB(const QByteArray &data, quint16 data_pos, quint8 data_length);
    static quint64 RAWToNumberLSB(const QByteArray &data, quint16 data_pos, quint8 data_length);
    static QByteArray NumberToRAWMSB(const quint64 num, const int length);
    static QByteArray NumberToRAWLSB(const quint64 num, const int length);
    static void delay(const int dealay_ms);

signals:

public slots:
};

#endif // TOOLS_H
