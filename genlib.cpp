#include "genlib.h"

GenLib::GenLib(QObject *parent) :
    QObject(parent)
{
}

GenLib::~GenLib()
{
}

QString GenLib::getIncludeString(QString snippetName)
{
    QString result = QString("\n#include %1").arg(snippetName);

    return result.trimmed();
}



