///
/// \file Class_ConfigReceiver.h
/// \brief Classe CLASS_CONFIG_RECEIVER
/// \author ANALOG WAY - GLE
/// \date 2018-07-04
///

#ifndef CLASS_CONFIGRECEIVER_H
#define CLASS_CONFIGRECEIVER_H

// C++ Standard
// Librairies Qt
#include <QObject>
#include <QSettings>
#include <QVector>
#include <QMutex>
// Modules
#include "COMMON_TypeDefs.h"

///
/// \class CLASS_CONFIG_RECEIVER
/// \brief Classe CLASS_CONFIG_RECEIVER
///
class CLASS_CONFIG_RECEIVER : public QObject
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
      /// \fn CLASS_CONFIG_RECEIVER(QObject *parent)
      /// \brief Constructeur de CLASS_CONFIG_RECEIVER
      ///
      ///
      CLASS_CONFIG_RECEIVER();

      ///
      /// \fn ~CLASS_CONFIG_RECEIVER()
      /// \brief Destructeur de CLASS_CONFIG_RECEIVER
      ///
      ~CLASS_CONFIG_RECEIVER();


      /*------------------------------ Accesseurs -----------------------------*/

      /*------------------------------ Mutateurs ------------------------------*/

      /*-------------------------- MÃ©thodes publiques -------------------------*/

      ///
      /// \fn ChargeConfigDevice
      /// \brief Récupère l'information de la fenetre à propos du nom de l'appareil et charge le fichier de config adéquate
      ///
      void ChargeConfigDevice(const QString &message);

      ///
      /// \fn DecodeFirmwareVersion
      /// \brief Récupère l'information brute sur la version du firmware et le décode
      ///
      void DecodeFirmwareVersion(const QString &message);

      ///
      /// \fn GetInputData
      /// \brief Récupère l'information sur l'input
      ///
      void CheckInputStatus(const QString &message);

      ///
      /// \fn GetPlugData
      /// \brief Récupère l'information sur le plug
      ///
      void CheckPlugStatus(const QString &message);

      ///
      /// \fn CreateRxFileConfig
      /// \brief Créer le fichier de config de la machine réceptrice
      ///
      void CreateRxFileConfig(void);

      ///
      /// \fn SetReceiverNameFromConfigName
      /// \brief Permet d'indiquer à la fenetre de configuration le nom de la machine réceptrice
      ///
      void SetReceiverNameFromConfigName(void);

      /*--------------------------- Signaux publiques -------------------------*/

   signals:

      ///
      /// \fn SIGNAL_DeviceNameObtained
      /// \brief
      ///
      void SIGNAL_DeviceNameObtained(void);

      ///
      /// \brief SIGNAL_FirmwareObtained
      ///
      void SIGNAL_FirmwareObtained(void);

      ///
      /// \brief SIGNAL_InputInformation
      ///
      void SIGNAL_InputInformationObtained(void);

      ///
      /// \brief SIGNAL_PlugInformation
      ///
      void SIGNAL_PlugInformationObtained(void);

      ///
      /// \brief SIGNAL_InterfaceReceiver
      ///
      void SIGNAL_InterfaceReceiverObtained(void);

      /*---------------------------- Slots publiques --------------------------*/

   public slots:

      /*----------------------------- Types privÃ©s ----------------------------*/
   private:



      /*--------------------------- MÃ©thodes privÃ©es --------------------------*/

   protected:

   private:

      /*------------------------------ Variables ------------------------------*/

   protected:

   public:

      QSettings *f_Settings;                             ///< Pour la sélection du fichier config
      QSettings *f_SettingsRx;                           ///< Pour créer le fichier de config de l'appareil receiver
      QSettings *f_SettingSource;                        ///< Pour récupérer le fichier config sélectionner par l'utilisateur dans la fenetre de configuration

      QString f_DeviceName;                              ///< Nom de l'appareil connecté pour le fichier de log
      QString f_ConfigName;                              ///< Nom pour le fichier de config créé
      ENUM_NAME_DEVICE f_MyReceiverDevice;
      QString f_FileName;                                ///< Nom du fichier log
      QString f_FirmwareVersion;                         ///< Version du firmware après décodage
      QString f_CurrentTime;                             ///< Temps au moment de l'ouverture de la fenetre de test

      SDWord f_InputNumber;                              ///< Indicateur sur le nombre de sorties
      SDWord f_LoopCounter;                              ///< Compteur pour agir comme une boucle for
      QVector<int> f_Tab;                                ///< Tableau stockant les plugs d'une sortie
      SDWord f_AvailableInput;                             ///< Indication sur le nombre de sortie valide
      QByteArray f_Input;                                ///< Indicateur de la sortie dans l'initialisation
      SDWord f_CurrentInput;                             ///< Indicateur sur la sortie dans la partie lancement

      SDWord f_LoopPlugCounter;                          ///< Compteur pour agir comme une boucle for
      SDWord f_Index;                                    ///< Pour se déplacer dans le tableau
      QVector<QVector<int>> f_InputTab;                  ///< Tableau 2D ligne : output / colonne : plug
      QByteArray f_Plug;                                 ///< Indicateur sur le plug

      SDWord f_CurrentFormat;                            ///< Stocke valeur du format actuel
      SDWord f_CurrentRate;                              ///< Stocke valeur du rate actuel
      SDWord f_CurrentInputPlugValue;                    ///< Stocke valeur du plug de sortie actuel
};

#endif // CLASS_CONFIGRECEIVER_H
