///
/// \file Class_ConfigReceiver.cpp
/// \brief Classe CLASS_CONFIG_RECEIVER
/// \author ANALOG WAY - GLE
/// \date 2018-07-04
///
/// Fonctions de la classe CLASS_CONFIG_RECEIVER
///

// C++ Standard
// Librairies Qt
#include <QFile>
#include <QString>
// Modules
#include "Class_ConfigReceiver.h"
#include "global_windows_variables.h"

/******************************** DEFINITIONS ********************************/

/*----------------------------- Types publiques -----------------------------*/
/*--------------------------- Constantes publiques --------------------------*/
// Nom des fichiers de config des machines
#define SETTING_VIO                    QStringLiteral("settings/settingVio.ini")
#define SETTING_LIVECORE               QStringLiteral("settings/settingLiveCore.ini")
#define SETTING_NLC                    QStringLiteral("settings/settingNLC.ini")
#define SETTING_MNG                    QStringLiteral("settings/settingMNG.ini")

// Dossier d'écriture des log
#define LOG_REPERTORY                  QStringLiteral("log/")
#define LOG_RECEIVER_DEVICE            QStringLiteral("ReceiverDevice")
#define LOG_CUSTOM_FORMAT              QStringLiteral("CUSTOM")

// Input/plug absent
#define NO_INPUT                       -1
#define NO_PLUG                        -1
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
/// \fn CLASS_CONFIG_RECEIVER(QObject *parent)
/// \brief Constructeur de CLASS_CONFIG_RECEIVER
///
///
CLASS_CONFIG_RECEIVER::CLASS_CONFIG_RECEIVER():
   f_Settings(nullptr),
   f_SettingsRx(nullptr),
   f_SettingSource(nullptr),
   f_InputNumber(0),
   f_LoopCounter(0),   
   f_AvailableInput(0),
   f_CurrentInput(0),
   f_LoopPlugCounter(0),
   f_Index(0),
   f_CurrentFormat(0),
   f_CurrentRate(0),
   f_CurrentInputPlugValue(0)
{
}

///
/// \fn ~CLASS_CONFIG_RECEIVER()
/// \brief Destructeur de CLASS_CONFIG_RECEIVER
///
CLASS_CONFIG_RECEIVER::~CLASS_CONFIG_RECEIVER()
{
}

/******************************************************************************
 *                                 Accesseurs                                 *
 *****************************************************************************/

/******************************************************************************
 *                                  Mutateurs                                 *
 *****************************************************************************/

/******************************************************************************
 *                             MÃ©thodes publiques                             *
 *****************************************************************************/

///
/// \fn Initialize
/// \brief Récupère le nom de l'appareil et charge le fichier config adéquate
/// \param timeout
///
void CLASS_CONFIG_RECEIVER::ChargeConfigDevice(const QString& message)
{
   // Réitialisation pour les sorties
   f_LoopCounter = 0;
   f_InputNumber = 0;
   f_AvailableInput = 0;
   // Réinitilisation pour les plug
   f_LoopPlugCounter = 0;
   f_Index = 0;
   // Réinitialisation du tableau 2D
   f_InputTab.clear();

   /// ############################################## Spécification machines ##############################################
   // Lecture des fichiers de config
   QSettings *settingsVio = new QSettings(SETTING_VIO, QSettings::IniFormat);
   QSettings *settingsLive = new QSettings(SETTING_LIVECORE, QSettings::IniFormat);
   QSettings *settingsNlc = new QSettings(SETTING_NLC, QSettings::IniFormat);
   QSettings *settingsMng = new QSettings(SETTING_MNG, QSettings::IniFormat);

   settingsVio->beginGroup(QStringLiteral("DEVICE"));
   QStringList keysVio = settingsVio->allKeys();
   settingsLive->beginGroup(QStringLiteral("DEVICE"));
   QStringList keysLive = settingsLive->allKeys();
   settingsNlc->beginGroup(QStringLiteral("DEVICE"));
   QStringList keysNlc = settingsNlc->allKeys();
   settingsMng->beginGroup(QStringLiteral("DEVICE"));
   QStringList keysMng = settingsMng->allKeys();
   /// ####################################################################################################################

   // On stocke le nom de l'appareil avec lequel on communique
   f_DeviceName = message;
   f_DeviceName.chop(2);

   /// ############################################## Spécification machines ##############################################

   // On s'assure qu'on a pas déja alloué une adresse pour le settings
   if(f_Settings != nullptr)
   {
      delete f_Settings;
      f_Settings = nullptr;
   }

   // On récupère les info sur l'identité de l'appareil
   for(int i = 0 ; i < keysVio.size() ; ++i)
   {
      if(f_DeviceName == keysVio.at(i) )
      {
         f_DeviceName = settingsVio->value(keysVio.at(i)).toString();
         f_ConfigName = QLatin1String("Vio4K");
         f_MyReceiverDevice = VIO4K;
         // Chargement du fichier de config correspondant pour le programme
         f_Settings = new QSettings(SETTING_VIO , QSettings::IniFormat);
      }
   }

   for(int j = 0 ; j < keysLive.size() ; ++j)
   {
      if(f_DeviceName == keysLive.at(j))
      {
         f_DeviceName = settingsLive->value(keysLive.at(j)).toString();
         f_ConfigName = QLatin1String("LiveCore");
         f_MyReceiverDevice = LIVECORE;
         //Chargement du fichier de config correspondant pour le programme
         f_Settings = new QSettings(SETTING_LIVECORE, QSettings::IniFormat);
      }
   }

   for(int i = 0 ; i < keysNlc.size() ; ++i)
   {
      if(f_DeviceName == keysNlc.at(i) )
      {
         f_DeviceName = settingsNlc->value(keysNlc.at(i)).toString();
         f_ConfigName = QLatin1String("NLC");
         f_MyReceiverDevice = NLC;
         // Chargement du fichier de config correspondant pour le programme
         f_Settings = new QSettings(SETTING_NLC , QSettings::IniFormat);
      }
   }

   for(int j = 0 ; j < keysMng.size() ; ++j)
   {
      if(f_DeviceName == keysMng.at(j))
      {
         f_DeviceName = settingsMng->value(keysMng.at(j)).toString();
         f_ConfigName = QLatin1String("MNG");
         f_MyReceiverDevice = MNG;
         // Chargement du fichier de config correspondant pour le programme
         f_Settings = new QSettings(SETTING_MNG, QSettings::IniFormat);
      }
   }
   /// #####################################################################################################################

   // Si le nom n'est pas renseigné s'arrêté
   if(f_ConfigName == QLatin1String(""))
      return;

   this->SetReceiverNameFromConfigName();

   emit this->SIGNAL_DeviceNameObtained();
}

///
/// \fn GetFirmwareVersion
/// \brief Récupère l'information sur le firmware, le décode et propose un nom de fichier log
/// \param timeout
///
void CLASS_CONFIG_RECEIVER::DecodeFirmwareVersion(const QString& message)
{
   // Variable de stockage
   QString version;
   QString majorVersion;
   QString minorVersion;
   QString buildVersion;

   // Variable taille des groupes de bits
   int endMajor = 8;
   int endMinor = 16;

   // Variables de stockage des nombres
   int numberMajor = 0;
   int numberMinor = 0;
   int numberBuild = 0;


   // Stock l'info sur le firmware dans une variable et on récupère uniquement les 8 chiffres
   QString InfoVersion = message;
   InfoVersion.chop(2);
   QString InfoNumber;
   if(f_ConfigName == QLatin1String("Vio4K"))
      InfoNumber = InfoVersion.remove(0 , 5);
   else if(f_ConfigName == QLatin1String("LiveCore"))
      InfoNumber = InfoVersion.remove(0 , 7);

   // Conversion QString en int pour le calcul
   uint InfoVersionDec = InfoNumber.toUInt();

   // Conversion DEC -> BIN
   QString InfoVersionBin = QString::number(InfoVersionDec , 2) ;

   // Mettre la longueur de InfoVersionBin à 32
   if(InfoVersionBin.size() < 32)
   {
      for(int i = InfoVersionBin.size() ; i < 32 ; ++i )
         InfoVersionBin.prepend(QLatin1String("0"));
   }

   // Traduction de la version
   if(InfoVersionBin[0] == QLatin1String("0"))
      version = QLatin1String("Release");
   else if(InfoVersionBin[0] == QLatin1String("1"))
      version = QLatin1String("BETA");

   // Traduction Major Version
   for(int i = 1 ; i < endMajor ; ++i)
   {
      int digit = 0;
      QChar carac = InfoVersionBin.at(i);
      digit = carac.digitValue() ;
      int number = static_cast<int> (digit * qPow(2, endMajor - i - 1 ));
      numberMajor += number;
   }

   // Traduction Minor Version
   for(int i = 8 ; i < endMinor ; ++i )
   {
      int digit = 0;
      QChar carac = InfoVersionBin.at(i);
      digit = carac.digitValue() ;
      int number = static_cast<int> (digit * qPow(2, endMinor - i - 1));
      numberMinor += number;
   }

   // Traduction Minor Version
   for(int i = 16 ; i < InfoVersionBin.size() ; ++i )
   {
      int digit = 0;
      QChar carac = InfoVersionBin.at(i);
      digit = carac.digitValue() ;
      int number = static_cast<int> (digit * qPow(2, InfoVersionBin.size() - i - 1));
      numberBuild += number;
   }

   // Mise en forme de la version en QString pour le tag dans le fichier Log
   majorVersion = QString::number(numberMajor);
   minorVersion = QString::number(numberMinor);
   buildVersion = QString::number(numberBuild);
   f_FirmwareVersion = majorVersion % QStringLiteral(".") % minorVersion % QStringLiteral(".") % buildVersion % QStringLiteral(" ") % version;

   // On initialise le nom du fichier log grâce aux infos récupérées
   if(ExtCustomFormatSelected == false)
      f_FileName = LOG_REPERTORY % LOG_RECEIVER_DEVICE % QStringLiteral("_") % f_DeviceName % QStringLiteral("_") % f_FirmwareVersion % QStringLiteral("_") % f_CurrentTime ;
   else
      f_FileName = LOG_REPERTORY % LOG_CUSTOM_FORMAT % QStringLiteral("_") % LOG_RECEIVER_DEVICE % QStringLiteral("_") % f_DeviceName % QStringLiteral("_") % f_FirmwareVersion % QStringLiteral("_") % f_CurrentTime;

   // On initialise le compteur à 0 pour la fonction GetDataInput
   f_Input.setNum(f_InputNumber);

   // Lancement de la demande d'information sur la sortie
   if(f_DeviceName == QLatin1String("") || (f_DeviceName == QLatin1String("?")))
      return;

   emit this->SIGNAL_InputInformationObtained();
}


///
/// \fn GetInputData
/// \brief Récupère les informations sur les entrées de la machine
/// \param timeout
///
void CLASS_CONFIG_RECEIVER::CheckInputStatus(const QString& message)
{
   QString InfoInput = message;
   InfoInput.chop(2);
   if(InfoInput == f_Settings->value(QStringLiteral("COMMAND/IN_AVAILABLE")).toString() % f_Input % QStringLiteral(",") % f_Settings->value(QStringLiteral("INFO/INPUT_ACTIVE")).toString())
   {
      f_InputNumber += 1;
      f_Plug.setNum(f_LoopPlugCounter);

      emit this->SIGNAL_PlugInformationObtained();

      f_AvailableInput = f_InputNumber;
   }

   else if(InfoInput == f_Settings->value(QStringLiteral("COMMAND/IN_AVAILABLE")).toString() % f_Input % QStringLiteral(",") % f_Settings->value(QStringLiteral("INFO/INPUT_NOT_AVAILABLE")).toString())
   {
      // On met une valeur dans le tableau on utilisera ceci pour indiquer qu'une sortie est indisponible
      f_Tab.insert(0, NO_INPUT);
      // Le plug est réinitialisé à 0
      f_Plug.setNum(0);
      // On se base sur le nombre de Input disponible pour remplir le tableau pour éviter un saut d'indice
      f_InputTab.insert(f_LoopCounter, f_Tab);

      // On passe à la boucle suivante
      f_LoopCounter += 1 ;
      f_Tab.clear();

      // On demande l'information sur la sortie
      f_Input.setNum(f_LoopCounter);

      if(f_LoopCounter < f_Settings->value(QStringLiteral("INFO/MAX_INPUT")).toInt())
         emit this->SIGNAL_InputInformationObtained();
      else
      {
         emit this->SIGNAL_InterfaceReceiverObtained();
         this->CreateRxFileConfig();
      }
   }  
}


///
/// \fn GetPlugData
/// \brief Récupère les informations sur les plugs de la machine
/// \param timeout
///
void CLASS_CONFIG_RECEIVER::CheckPlugStatus(const QString &message)
{
   // On récupère les plugs disponibles sur la sortie dans un tableau
   QString InfoPlug = message;
   InfoPlug.chop(2);

   if(f_ConfigName == QLatin1String("LiveCore"))
   {
      if((InfoPlug != f_Settings->value(QStringLiteral("COMMAND/IN_PLUG_AVAILABLE")).toString() % f_Input % QStringLiteral(",") % f_Plug % QStringLiteral(",") % f_Settings->value(QStringLiteral("INFO/INPUT_NOT_AVAILABLE")).toString()) && (InfoPlug != QLatin1String("E11") ))
      {
         f_Tab.insert(f_Index, f_LoopPlugCounter);
         f_Index += 1;
      }

      f_LoopPlugCounter += 1;

      f_Input.setNum(f_LoopCounter);
      f_Plug.setNum(f_LoopPlugCounter);

      if(f_LoopPlugCounter < f_Settings->value(QStringLiteral("INFO/MAX_INPUT_PLUG")).toInt())
         emit this->SIGNAL_PlugInformationObtained();

      else
      {
         // On met à jour le tableau 2D de sortie et de plug
         f_InputTab.insert(f_LoopCounter, f_Tab);

         // On réinitialise avant de changer de sortie
         f_LoopPlugCounter = 0;
         f_Index = 0;
         f_Tab.clear();

         // On passe à la sortie suivante
         f_LoopCounter += 1;
         f_Input.setNum(f_LoopCounter);

         if(f_InputNumber < f_Settings->value(QStringLiteral("INFO/MAX_INPUT")).toInt())
            emit this->SIGNAL_InputInformationObtained();
      }

   }
   else if (f_ConfigName == QLatin1String("Vio4K"))
   {
      QString plug;
      plug = InfoPlug.section(QLatin1String(",") , 2 , 3);

      if(f_LoopCounter < 7)
      {
         f_Tab.insert(f_Index, plug.toInt());

         // On met à jour le tableau 2D de sortie et de plug
         f_InputTab.insert(f_LoopCounter, f_Tab);

         // On réinitialise avant de changer de sortie
         f_LoopPlugCounter = 0;
         f_Index = 0;
         f_Tab.clear();

         // On passe à la sortie suivante
         f_LoopCounter += 1;
         f_Input.setNum(f_LoopCounter);

         emit this->SIGNAL_InputInformationObtained();
      }
      else if(f_LoopCounter == 7 || f_LoopCounter == 8 )
      {
         f_Tab.insert(f_Index, plug.toInt());
         f_Index += 1;
         if(f_LoopPlugCounter < f_Settings->value(QStringLiteral("INFO/MAX_INPUT_PLUG")).toInt() - 1)
         {
            f_LoopPlugCounter += 1;
            f_Plug.setNum(f_LoopPlugCounter);
            emit this->SIGNAL_PlugInformationObtained();
         }
         else
         {
            f_InputTab.insert(f_LoopCounter, f_Tab);

            // Réinitialisation tableau
            f_Index = 0;
            f_Tab.clear();
            f_LoopPlugCounter = 0;

            // On passe à l'input suivant
            f_LoopCounter += 1;
            f_Input.setNum(f_LoopCounter);

            if(f_LoopCounter == f_Settings->value(QStringLiteral("INFO/MAX_INPUT")).toInt())
            {
               emit this->SIGNAL_InterfaceReceiverObtained();
               this->CreateRxFileConfig();
            }
            else
            {
               emit this->SIGNAL_InputInformationObtained();
            }
         }
      }
   }
}

///
/// \fn CreateRxFileConfig
/// \brief Créer le fichier de configuration de l'appareil récepteur
///
void CLASS_CONFIG_RECEIVER::CreateRxFileConfig(void)
{
   /// ############################################## Spécification machines ##############################################
   // Vérifie si le fichier existe
   QFile fileRxLivecore(QStringLiteral("settings/settingRx_LiveCore.ini"));
   QFile fileRxVio(QStringLiteral("settings/settingRx_Vio4K.ini"));
   QFile fileRxNlc(QStringLiteral("settings/settingRx_NLC.ini"));
   QFile fileRxMng(QStringLiteral("settings/settingRx_MNG.ini"));

   if(fileRxLivecore.exists())
      fileRxLivecore.remove();

   if(fileRxVio.exists())
      fileRxVio.remove();

   if(fileRxNlc.exists())
      fileRxNlc.remove();

   if(fileRxMng.exists())
      fileRxMng.remove();

   /// ####################################################################################################################

   // Création du fichier de config de la source
   if(f_SettingsRx != nullptr)
   {
      delete f_SettingsRx;
      f_SettingsRx = nullptr;
   }

   f_SettingsRx = new QSettings(QStringLiteral("settings/settingRx_") % f_ConfigName % QStringLiteral(".ini"), QSettings::IniFormat);

   f_SettingsRx->beginGroup(QStringLiteral("INFORMATION"));
   f_SettingsRx->setValue(QStringLiteral("MAX_INPUT_DETECTED") , f_AvailableInput);
   f_SettingsRx->endGroup();

   f_SettingsRx->beginGroup(QStringLiteral("INPUT_IDENTIFIED"));
   for(int i = 0 ; i < f_InputTab.size() ; ++i )
   {
      if(f_InputTab.at(i).at(0) != NO_INPUT)
      {
         QString iStr;
         iStr.setNum(i);
         f_SettingsRx->setValue(f_Settings->value(QStringLiteral("INPUT_VALUE/") % iStr).toString() , i);
      }
   }
   f_SettingsRx->endGroup();

   // Création d'un récapitulatif des plug détecté
   f_SettingsRx->beginGroup(QStringLiteral("PLUG_IDENTIFIED"));
   for(int i = 0 ; i < f_InputTab.size() ; ++i )
   {
      QString iStr;
      iStr.setNum(i);
      f_SettingsRx->beginGroup(f_Settings->value(QStringLiteral("INPUT_VALUE/") % iStr).toString());
      if(f_InputTab.at(i).at(0) != NO_PLUG)
      {
         for(int j = 0 ; j < f_InputTab.at(i).size() ; ++j)
         {
            if(f_InputTab.at(i).at(j) != NO_PLUG)
            {
               QString jStr;
               jStr.setNum(f_InputTab.at(i).at(j));
               f_SettingsRx->setValue(f_Settings->value(QStringLiteral("INPUT_PLUG_VALUE/") % jStr).toString() , f_InputTab.at(i).at(j));
            }
         }
      }
      f_SettingsRx->endGroup();
   }
   f_SettingsRx->endGroup();

   // Création des groupe d'Input avec leur clé correspondant au plug
   for(int i = 0 ; i < f_InputTab.size() ; ++i)
   {
      QString iStr;
      iStr.setNum(i);
      if(f_InputTab.at(i).at(0) != NO_INPUT)
      {
         f_SettingsRx->beginGroup(f_Settings->value(QStringLiteral("INPUT_VALUE/") % iStr).toString());
         for(int j = 0 ; j < f_InputTab.at(i).size() ; ++j)
         {
            if(f_InputTab.at(i).at(j) != NO_PLUG)
            {
               QString jStr;
               jStr.setNum(f_InputTab.at(i).at(j));
               f_SettingsRx->setValue(f_Settings->value(QStringLiteral("INPUT_PLUG_VALUE/") % jStr).toString() , f_InputTab.at(i).at(j));
            }
         }
         f_SettingsRx->endGroup();
      }
   }
}


///
/// \brief CLASS_CONFIG_RECEIVER::SetReceiverNameFromConfigName
///
void CLASS_CONFIG_RECEIVER::SetReceiverNameFromConfigName()
{
   ExtReceiverName = f_ConfigName;
}


/******************************************************************************
 *                               Slots publiques                              *
 *****************************************************************************/

/******************************************************************************
 *                              MÃ©thodes privÃ©es                              *
 *****************************************************************************/

/******************************************************************************
 *                             Operateur surcharge                            *
 *****************************************************************************/
