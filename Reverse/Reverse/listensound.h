#ifndef LISTENSOUND_H
#define LISTENSOUND_H

#include <QObject>
#include <QtCore/QIODevice>
#include <QtCore/QPointF>
#include <QtCore/QVector>
#include <QDebug>
#include <QBuffer>
#include <QVector>
#include <QFile>

class ListenSound : public QFile
{
    Q_OBJECT
    QBuffer* buffer;
public:
    explicit ListenSound(QObject *parent = nullptr);

    static const int sampleCount = 6000;
    void setCountTime(uint);
    void bufferClose();

signals:
    void sendBuffer(QBuffer*);

protected:
    qint64 readData(char *data, qint64 maxSize) override;
    qint64 writeData(const char *data, qint64 maxSize) override;
};

#endif // LISTENSOUND_H
