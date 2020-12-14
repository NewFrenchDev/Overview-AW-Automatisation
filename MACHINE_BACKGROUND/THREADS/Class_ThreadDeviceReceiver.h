///
/// \file Class_ThreadDeviceReceiver.h
/// \brief Classe CLASS_THREAD_DEVICE_RECEIVER
/// \author ANALOG WAY - GLE
/// \date 2018-07-04
///

#ifndef CLASS_THREADDEVICERECEIVER_H
#define CLASS_THREADDEVICERECEIVER_H

// C++ Standard
// Librairies Qt
#include <QObject>
#include <QSettings>
#include <QFile>
#include <QThread>
// Modules
#include "COMMON_TypeDefs.h"
#include "Class_ConfigReceiver.h"
#include "Class_TestReceiver.h"

///
/// \class CLASS_THREAD_DEVICE_RECEIVER
/// \brief Classe CLASS_THREAD_DEVICE_RECEIVER
///
class CLASS_THREAD_DEVICE_RECEIVER : public QObject
{
      Q_OBJECT

   public:

      /*--------------------------- Types publiques ---------------------------*/

      /*------------------------- Constantes publiques ------------------------*/

      /*------------------------- Variables publiques -------------------------*/

      /*---------------------- Constructeur / Destructeur ---------------------*/

      ///
      /// \fn CLASS_THREAD_DEVICE_RECEIVER(QObject *parent)
      /// \brief Constructeur de CLASS_THREAD_DEVICE_RECEIVER
      ///
      ///
      CLASS_THREAD_DEVICE_RECEIVER();

      ///
      /// \fn ~CLASS_THREAD_DEVICE_RECEIVER()
      /// \brief Destructeur de CLASS_THREAD_DEVICE_RECEIVER
      ///
      ~CLASS_THREAD_DEVICE_RECEIVER();


      /*------------------------------ Accesseurs -----------------------------*/

      ///
      /// \fn GetConfigReceiver
      /// \brief Récupère le ConfigReceiver généré dans le thread
      /// \return CLASS_CONFIG_RECEIVER
      ///
      CLASS_CONFIG_RECEIVER* GetConfigReceiver(void);

      ///
      /// \fn GetTestReceiver
      /// \brief Récupère le TestReceiver généré dans le thread
      /// \return CLASS_TEST_RECEIVER
      ///
      CLASS_TEST_RECEIVER* GetTestReceiver(void);


      /*------------------------------ Mutateurs ------------------------------*/

      /*-------------------------- MÃ©thodes publiques -------------------------*/

      /*--------------------------- Signaux publiques -------------------------*/

   signals:

      ///
      /// \fn SIGNAL_DeviceName
      /// \param message : info a décodée
      ///
      void SIGNAL_DeviceName(const QString& message);

      ///
      /// \fn SIGNAL_FirmwareVersion
      /// \param message : info a décodée
      ///
      void SIGNAL_FirmwareVersion(const QString& message);

      ///
      /// \fn SIGNAL_NextInputStatus
      /// \param message : info a décodée
      ///
      void SIGNAL_NextInputStatus(const QString& message);

      ///
      /// \fn SIGNAL_NextPlugStatus
      /// \param message : info a décodée
      ///
      void SIGNAL_NextPlugStatus(const QString& message);

      /*---------------------------- Slots publiques --------------------------*/

   public slots:

      /*----------------------------- Types privÃ©s ----------------------------*/

      /*--------------------------- MÃ©thodes privÃ©es --------------------------*/

   protected:

   private:

      /*------------------------------ Variables ------------------------------*/

   protected:

   private:

   public:

      QThread *f_ReceiverThread;                                         ///< Thread pour la manipulation des class config et test en parallèle au MAIN_THREAD
      CLASS_CONFIG_RECEIVER* f_ConfigReceiver;                          ///< ConfigReceiver générée pour récupérer la configuration physique de la machine en background
      CLASS_TEST_RECEIVER* f_TestReceiver;                              ///< TestReceiver généré pour lancer les test de la machine réceptrice en background
};

#endif // CLASS_THREADDEVICERECEIVER_H
