///
/// \file Class_ThreadTestSource.h
/// \brief Classe CLASS_THREAD_TEST_SOURCE
/// \author ANALOG WAY  - GLE
/// \date 2018-07-04
///

#ifndef CLASS_THREADTESTSOURCE_H
#define CLASS_THREADTESTSOURCE_H

// C++ Standard
// Librairies Qt
#include <QObject>
#include <QThread>
// Modules
#include "COMMON_TypeDefs.h"
#include "Class_TestSource.h"
#include "Class_ConfigSource.h"

///
/// \class CLASS_THREAD_TEST_SOURCE
/// \brief Classe CLASS_THREAD_TEST_SOURCE
///
class CLASS_THREAD_DEVICE_SOURCE : public QObject
{
      Q_OBJECT


   public:

      /*--------------------------- Types publiques ---------------------------*/

      /*------------------------- Constantes publiques ------------------------*/

      /*------------------------- Variables publiques -------------------------*/

      /*---------------------- Constructeur / Destructeur ---------------------*/

      ///
      /// \fn CLASS_THREAD_TEST_SOURCE(QObject *parent)
      /// \brief Constructeur de CLASS_THREAD_TEST_SOURCE
      ///
      ///
      CLASS_THREAD_DEVICE_SOURCE();

      ///
      /// \fn ~CLASS_THREAD_TEST_SOURCE()
      /// \brief Destructeur de CLASS_THREAD_TEST_SOURCE
      ///
      ~CLASS_THREAD_DEVICE_SOURCE();

      /*------------------------------ Accesseurs -----------------------------*/

      ///
      /// \fn GetTestSource
      /// \brief Récupère le TestSource généré dans le thread
      /// \return pointeur vers CLASS_TEST_SOURCE
      ///
      CLASS_TEST_SOURCE* GetTestSource(void);

      ///
      /// \fn GetConfigSource
      /// \brief Récupère le ConfigSource généré dans le thread
      /// \return pointeur vers CLASS_CONFIG_SOURCE
      ///
      CLASS_CONFIG_SOURCE* GetConfigSource(void);

      /*------------------------------ Mutateurs ------------------------------*/

      /*-------------------------- MÃ©thodes publiques -------------------------*/

      /*--------------------------- Signaux publiques -------------------------*/

   signals:

      ///
      /// \fn SIGNAL_DeviceName
      /// \param message : information à décoder
      ///
      void SIGNAL_DeviceName(const QString &message);

      ///
      /// \fn SIGNAL_FirmwareVersion
      /// \param message : information à décoder
      ///
      void SIGNAL_FirmwareVersion(const QString &message);

      ///
      /// \fn SIGNAL_NextOutputStatus
      /// \param message : information à décoder
      ///
      void SIGNAL_NextOutputStatus(const QString &message);

      ///
      /// \fn SIGNAL_NextPlugStatus
      /// \param message : information à décoder
      ///
      void SIGNAL_NextPlugStatus(const QString &message);

      /*---------------------------- Slots publiques --------------------------*/

   public slots:

      /*----------------------------- Types privÃ©s ----------------------------*/

      /*--------------------------- MÃ©thodes privÃ©es --------------------------*/

   protected:

   private:

      /*------------------------------ Variables ------------------------------*/     
   protected:

   public:

      QThread *f_SourceThread;                                           ///< Thread pour la manipulation des class config et test en parallèle au MAIN_THREAD
      CLASS_CONFIG_SOURCE *f_ConfigSource;                              ///< ConfigSource générée pour récupérer la configuration physique de la machine en background
      CLASS_TEST_SOURCE *f_TestSource;                                  ///< TestReceiver généré pour lancer les test de la machine réceptrice en background

};

#endif // CLASS_THREADTESTSOURCE_H
