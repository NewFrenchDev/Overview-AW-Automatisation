///
/// \file Class_ThreadDeviceReceiver.cpp
/// \brief Classe CLASS_THREAD_DEVICE_RECEIVER
/// \author ANALOG WAY - GLE
/// \date 2018-07-04
///
/// Fonctions de la classe CLASS_THREAD_DEVICE_RECEIVER
///

// C++ Standard
// Librairies Qt
#include <QtCore>
#include <QString>
#include <QtDebug>
#include <QMessageBox>
// Modules
#include "Class_ThreadDeviceReceiver.h"
#include "TRACES.h"

/******************************** DEFINITIONS ********************************/

/*----------------------------- Types publiques -----------------------------*/
/*--------------------------- Constantes publiques --------------------------*/
/*--------------------------- Variables publiques ---------------------------*/
/*------------------------------- Types privÃ©s ------------------------------*/
/*----------------------- Prototypes fonctions privÃ©es ----------------------*/
/*---------------------------- Constantes privÃ©es ---------------------------*/
/*---------------------------- Variables privÃ©es ----------------------------*/

/**************************** FIN DES DEFINITIONS ****************************/

/******************************************************************************
 *                             Fonctions privÃ©es                              *
 *****************************************************************************/

/******************************************************************************
 *                         Constructeur / Destructeur                         *
 *****************************************************************************/

///
/// \fn CLASS_THREAD_DEVICE_RECEIVER(QObject *parent)
/// \brief Constructeur de CLASS_THREAD_DEVICE_RECEIVER
///
/// \param[in] parent : Objet parent
///
CLASS_THREAD_DEVICE_RECEIVER::CLASS_THREAD_DEVICE_RECEIVER():
   f_ReceiverThread(nullptr),
   f_ConfigReceiver(nullptr),
   f_TestReceiver(nullptr)
{
   f_ReceiverThread = new QThread;
   f_ReceiverThread->setObjectName(QStringLiteral("RECEIVER THREAD"));

   // Partie configuration
   f_ConfigReceiver = new CLASS_CONFIG_RECEIVER;  
   f_ConfigReceiver->moveToThread(f_ReceiverThread);
   QObject::connect(f_ReceiverThread, &QThread::finished, f_ConfigReceiver, &QObject::deleteLater);

   QObject::connect(this, &CLASS_THREAD_DEVICE_RECEIVER::SIGNAL_DeviceName, f_ConfigReceiver, &CLASS_CONFIG_RECEIVER::ChargeConfigDevice, Qt::QueuedConnection);
   QObject::connect(this, &CLASS_THREAD_DEVICE_RECEIVER::SIGNAL_FirmwareVersion, f_ConfigReceiver, &CLASS_CONFIG_RECEIVER::DecodeFirmwareVersion, Qt::QueuedConnection);
   QObject::connect(this, &CLASS_THREAD_DEVICE_RECEIVER::SIGNAL_NextInputStatus, f_ConfigReceiver, &CLASS_CONFIG_RECEIVER::CheckInputStatus, Qt::QueuedConnection);
   QObject::connect(this, &CLASS_THREAD_DEVICE_RECEIVER::SIGNAL_NextPlugStatus, f_ConfigReceiver, &CLASS_CONFIG_RECEIVER::CheckPlugStatus, Qt::QueuedConnection);

   // Partie test
   f_TestReceiver = new CLASS_TEST_RECEIVER;
   f_TestReceiver->moveToThread(f_ReceiverThread);
   QObject::connect(f_ReceiverThread, &QThread::finished, f_TestReceiver, &QObject::deleteLater);

   QObject::connect(qApp, &QCoreApplication::aboutToQuit, f_ReceiverThread, &QThread::deleteLater, Qt::QueuedConnection);

   f_ReceiverThread->start();
}

///
/// \fn ~CLASS_THREAD_DEVICE_RECEIVER()
/// \brief Destructeur de CLASS_THREAD_DEVICE_RECEIVER
///
CLASS_THREAD_DEVICE_RECEIVER::~CLASS_THREAD_DEVICE_RECEIVER()
{
   f_ReceiverThread->exit();
}


/******************************************************************************
 *                                 Accesseurs                                 *
 *****************************************************************************/

///
/// \brief CLASS_CONFIG_RECEIVER::GetConfigReceiver
/// \return pointeur sur la CLASS_CONFIG_RECEIVER créé
///
CLASS_CONFIG_RECEIVER* CLASS_THREAD_DEVICE_RECEIVER::GetConfigReceiver(void)
{
   return f_ConfigReceiver;
}

///
/// \brief CLASS_THREAD_DEVICE_RECEIVER::GetTestReceiver
/// \return  pointeur sur la CLASS_TEST_RECEIVER créé
///
CLASS_TEST_RECEIVER* CLASS_THREAD_DEVICE_RECEIVER::GetTestReceiver(void)
{
   return f_TestReceiver;
}

/******************************************************************************
 *                                  Mutateurs                                 *
 *****************************************************************************/

/******************************************************************************
 *                             MÃ©thodes publiques                             *
 *****************************************************************************/

/******************************************************************************
 *                               Slots publiques                              *
 *****************************************************************************/

/******************************************************************************
 *                              MÃ©thodes privÃ©es                              *
 *****************************************************************************/

/******************************************************************************
 *                             Operateur surcharge                            *
 *****************************************************************************/
