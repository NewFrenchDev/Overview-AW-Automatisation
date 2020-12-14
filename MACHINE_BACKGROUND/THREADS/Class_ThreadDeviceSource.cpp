///
/// \file Class_ThreadTestSource.cpp
/// \brief Classe CLASS_THREAD_TEST_SOURCE
/// \author ANALOG WAY  - GLE
/// \date 2018-07-04
///
/// Fonctions de la classe CLASS_THREAD_TEST_SOURCE
///

// C++ Standard
// Librairies Qt
// Modules
#include "Class_ThreadDeviceSource.h"

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
/// \fn CLASS_THREAD_TEST_SOURCE(QObject *parent)
/// \brief Constructeur de CLASS_THREAD_TEST_SOURCE
///
/// \param[in] parent : Objet parent
///
CLASS_THREAD_DEVICE_SOURCE::CLASS_THREAD_DEVICE_SOURCE():
   f_SourceThread(nullptr),
   f_ConfigSource(nullptr),
   f_TestSource(nullptr)
{
   f_SourceThread = new QThread;
   f_SourceThread->setObjectName(QStringLiteral("SOURCE THREAD"));

   // Partie configuration
   f_ConfigSource = new CLASS_CONFIG_SOURCE;
   f_ConfigSource->moveToThread(f_SourceThread);
   QObject::connect(f_SourceThread, &QThread::finished, f_ConfigSource, &QObject::deleteLater);


   QObject::connect(this, &CLASS_THREAD_DEVICE_SOURCE::SIGNAL_DeviceName, f_ConfigSource, &CLASS_CONFIG_SOURCE::ChargeConfigDevice, Qt::QueuedConnection);
   QObject::connect(this, &CLASS_THREAD_DEVICE_SOURCE::SIGNAL_FirmwareVersion, f_ConfigSource, &CLASS_CONFIG_SOURCE::DecodeFirmwareVersion, Qt::QueuedConnection);
   QObject::connect(this, &CLASS_THREAD_DEVICE_SOURCE::SIGNAL_NextOutputStatus, f_ConfigSource, &CLASS_CONFIG_SOURCE::CheckOutputStatus, Qt::QueuedConnection);
   QObject::connect(this, &CLASS_THREAD_DEVICE_SOURCE::SIGNAL_NextPlugStatus, f_ConfigSource, &CLASS_CONFIG_SOURCE::CheckPlugStatus, Qt::QueuedConnection);

   // Partie test
   f_TestSource = new CLASS_TEST_SOURCE;   
   f_TestSource->moveToThread(f_SourceThread);
   QObject::connect(f_SourceThread, &QThread::finished, f_TestSource, &QObject::deleteLater);

   QObject::connect(qApp, &QCoreApplication::aboutToQuit, f_SourceThread, &QThread::deleteLater, Qt::QueuedConnection);

   f_SourceThread->start();
}

///
/// \fn ~CLASS_THREAD_TEST_SOURCE()
/// \brief Destructeur de CLASS_THREAD_TEST_SOURCE
///
CLASS_THREAD_DEVICE_SOURCE::~CLASS_THREAD_DEVICE_SOURCE()
{
   f_SourceThread->exit();
}

/******************************************************************************
 *                                 Accesseurs                                 *
 *****************************************************************************/
///
/// \brief CLASS_THREAD_DEVICE_SOURCE::GetTestSource
/// \return
///
CLASS_TEST_SOURCE* CLASS_THREAD_DEVICE_SOURCE::GetTestSource(void)
{
   return f_TestSource;
}

///
/// \brief CLASS_THREAD_DEVICE_SOURCE::GetConfigSource
/// \return
///
CLASS_CONFIG_SOURCE* CLASS_THREAD_DEVICE_SOURCE::GetConfigSource(void)
{
   return f_ConfigSource;
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
