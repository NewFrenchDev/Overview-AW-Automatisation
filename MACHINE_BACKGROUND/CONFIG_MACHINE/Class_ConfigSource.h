///
/// \file Threads.h
/// \brief
/// \author ANALOG WAY - GLE
/// \date 2018-07-03
///

#ifndef CLASS_CONFIG_SOURCE_H
#define CLASS_CONFIG_SOURCE_H

// C++ Standard
// Librairies Qt
#include <QObject>
#include <QSettings>
#include <QFile>
#include <QThread>
// Modules
#include "COMMON_TypeDefs.h"
#include "global_windows_variables.h"



class CLASS_CONFIG_SOURCE : public QObject
{
      Q_OBJECT

   public:


      /*--------------------------- Types publiques ---------------------------*/

      typedef enum
      {
         UNKNOW,
         LIVECORE,
         VIO4K,
         NLC,
         MNG
      }ENUM_NAME_DEVICE;

      /*------------------------- Constantes publiques ------------------------*/

      /*------------------------- Variables publiques -------------------------*/

      /*---------------------- Constructeur / Destructeur ---------------------*/
      ///
      /// \brief CLASS_CONFIG_SOURCE
      ///
      CLASS_CONFIG_SOURCE();

      ///

      ~CLASS_CONFIG_SOURCE();

   public:

      ///
      /// \fn ChargeConfigDevice
      /// \brief Récupère l'information de la fenetre à propos du nom de l'appareil et charge le fichier de config adéquate
      /// \param message
      ///
      void ChargeConfigDevice(const QString &message);

      ///
      /// \fn DecodeFirmwareVersion
      /// \brief Récupère l'information brute sur la version du firmware et le décode
      /// \param message
      ///
      void DecodeFirmwareVersion(const QString &message);

      ///
      /// \fn CheckOutputData
      /// \brief Récupère l'information sur l'output
      /// \param message
      ///
      void CheckOutputStatus(const QString &message);

      ///
      /// \fn CheckPlugData
      /// \brief Récupère l'information sur le plug
      /// \param message
      ///
      void CheckPlugStatus(const QString &message);

      ///
      /// \fn CreateTxFileConfig
      /// \brief Créer le fichier de config de la machine source comportant les informations les outputs et les plug
      ///
      void CreateTxFileConfig(void);

      ///
      /// \fn SetSourceNameFromConfigName
      /// \brief Indique à la fenetre de configuration qui est la machine source
      ///
      void SetSourceNameFromConfigName(void);


     /*--------------------------- Signaux publiques -------------------------*/

   signals:

      ///
      /// \brief SIGNAL_DeviceNameObtained
      /// \param configName
      /// \param deviceName
      ///
      void SIGNAL_DeviceNameObtained(void);

      ///
      /// \brief SIGNAL_OutputInformationObtained
      /// \param output
      /// \param plug
      /// \param settings
      ///
      void SIGNAL_OutputInformationObtained(void);

      ///
      /// \brief SIGNAL_PlugInformationObtained
      /// \param plug
      ///
      void SIGNAL_PlugInformationObtained(void);

      ///
      /// \brief SIGNAL_InterfaceSourceObtained
      /// \param nbreOutput
      ///
      void SIGNAL_InterfaceSourceObtained(void);

      /*---------------------------- Slots publiques --------------------------*/

   public slots:

      /*----------------------------- Types privÃ©s ----------------------------*/
   private:



      /*--------------------------- MÃ©thodes privÃ©es --------------------------*/

   protected:

   private:

      /*------------------------------ Variables ------------------------------*/


   public:

      QSettings *f_Settings;                             ///< Pour la sélection du fichier config
      QSettings *f_SettingsTx;                           ///< Pour créer le fichier de config de l'appareil source

      QString f_DeviceName;                              ///< Nom de l'appareil connecté pour le fichier de log
      QString f_ConfigName;                              ///< Nom pour le fichier de config créé
      ENUM_NAME_DEVICE f_MySourceDevice;
      QString f_FileName;                                ///< Nom du fichier log
      QString f_FirmwareVersion;                         ///< Nom de lu firmware
      QString f_CurrentTime;                             ///< Temps au moment de l'ouverture de la fenetre de test

      SDWord f_OutputNumber;                                ///< Indicateur sur le nombre de sorties
      SDWord f_LoopCounter;                                 ///< Compteur pour agir comme une boucle for
      QVector<int> f_Tab;                                ///< Tableau stockant les plugs d'une sortie
      SDWord f_AvailableOutput;                             ///< Indication sur le nombre de sortie valide
      QByteArray f_Output;                               ///< Indicateur de la sortie dans l'initialisation
      SDWord f_CurrentOutput;                               ///< Indicateur sur la sortie dans la partie lancement

      SDWord f_LoopPlugCounter;                             ///< Compteur pour agir comme une boucle for
      SDWord f_Index;                                       ///< Pour identifier l'index du tableau
      QVector<QVector<int>> f_OutputTab;                 ///< Tableau 2D ligne : output colonne : plug
      QByteArray f_Plug;                                 ///< Indicateur sur le plug

      SDWord f_CurrentFormat;                               ///< Stocke valeur du format actuel
      SDWord f_CurrentRate;                                 ///< Stocke valeur du rate actuel
      SDWord f_CurrentOutputPlugValue;                      ///< Stocke valeur du plug de sortie actuel
};

#endif // CLASS_CONFIG_SOURCE_H
