#include "listensound.h"
#include <QDebug>

ListenSound::ListenSound(QObject *parent) : QFile(parent)
{
    buffer = new QBuffer(this);
    buffer->open(QIODevice::WriteOnly);
}

void ListenSound::bufferClose()
{
//    buffer->close();
     emit sendBuffer(buffer);
}

qint64 ListenSound::readData(char *data, qint64 maxSize)
{
    buffer->write(data,maxSize);
    return QFile::readData(data,maxSize);
}

qint64 ListenSound::writeData(const char *data, qint64 maxSize)
{
    qDebug() << maxSize;
    return QFile::writeData(data,maxSize);
}
