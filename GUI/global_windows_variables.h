#ifndef GLOBAL_WINDOWS_VARIABLES_H
#define GLOBAL_WINDOWS_VARIABLES_H

#include <QtCore>

//// Variable globale

extern QString ExtSourceName;
extern QString ExtReceiverName;

extern QVector<QString> *ExtTableau;
extern QVector<QVector<QString>*> ExtTableau2D;

extern QString ExtRateValue;
extern QByteArray ExtOutputName;
extern QByteArray ExtOutputFormat;
extern QByteArray ExtCustomRate;

extern bool ExtModeScrutationSelected;
extern bool ExtCustomFormatSelected;

extern bool ExtFirstSlot;

#endif // GLOBAL_WINDOWS_VARIABLES_H
