///
/// \file Threads.cpp
/// \brief
/// \author ANALOG WAY - GLE
/// \date 2018-07-03
///

// C++ Standard
// Librairies Qt
#include <QtMath>
#include <QString>
#include <QtDebug>
#include <QMessageBox>
// Modules
#include "Class_ConfigSource.h"
#include "TRACES.h"

/******************************** DEFINITIONS ********************************/

/*----------------------------- Types publiques -----------------------------*/

/*--------------------------- Constantes publiques --------------------------*/
// Les fichiers de config
#define SETTING_VIO                    QStringLiteral("settings/settingVio.ini")
#define SETTING_LIVECORE               QStringLiteral("settings/settingLiveCore.ini")
#define SETTING_NLC                    QStringLiteral("settings/settingNLC.ini")
#define SETTING_MNG                    QStringLiteral("settings/settingMNG.ini")

// Dossier d'écriture des log
#define LOG_REPERTORY                  QStringLiteral("log/")
#define LOG_SOURCE_DEVICE              QStringLiteral("SourceDevice")
#define LOG_CUSTOM_FORMAT              QStringLiteral("CUSTOM")

// Output/Plug absent
#define NO_PLUG                        -1
#define NO_OUTPUT                      -1
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
 *                            Fonctions publiques                             *
 *****************************************************************************/
///
/// \brief CLASS_CONFIG_SOURCE::CLASS_CONFIG_SOURCE
///
CLASS_CONFIG_SOURCE::CLASS_CONFIG_SOURCE():
   f_Settings(nullptr),
   f_SettingsTx(nullptr),
   f_OutputNumber(0),
   f_LoopCounter(0),
   f_AvailableOutput(0),
   f_Output("0"),
   f_CurrentOutput(0),
   f_LoopPlugCounter(0),
   f_Index(0),
   f_Plug("0"),
   f_CurrentFormat(0),
   f_CurrentRate(0),
   f_CurrentOutputPlugValue(0)
{

}

///
/// \brief CLASS_CONFIG_SOURCE::~CLASS_CONFIG_SOURCE
///
CLASS_CONFIG_SOURCE::~CLASS_CONFIG_SOURCE()
{

}


///
/// \fn GetConfig
/// \brief Récupère le nom de l'appareil et charge le fichier config adéquate
/// \param timeout
///
void CLASS_CONFIG_SOURCE::ChargeConfigDevice(const QString &message)
{
   // Réitialisation pour les sorties
   f_LoopCounter = 0;
   f_OutputNumber = 0;
   f_AvailableOutput = 0;
   // Réinitilisation pour les plug
   f_LoopPlugCounter = 0;
   f_Index = 0;
   // Réinitialisation du tableau 2D
   f_OutputTab.clear();


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

   /// #####################################################################################################################


   // On stocke le nom de l'appareil avec lequel on communique
   f_DeviceName = message;
   f_DeviceName.chop(2);


   /// ############################################## Spécification machines ##############################################

   //
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
         f_MySourceDevice = VIO4K;
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
         f_MySourceDevice = LIVECORE;
         // Chargement du fichier de config correspondant pour le programme
         f_Settings = new QSettings(SETTING_LIVECORE, QSettings::IniFormat);
      }
   }

   for(int i = 0 ; i < keysNlc.size() ; ++i)
   {
      if(f_DeviceName == keysNlc.at(i) )
      {
         f_DeviceName = settingsNlc->value(keysNlc.at(i)).toString();
         f_ConfigName = QLatin1String("NLC");
         f_MySourceDevice = NLC;
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
         f_MySourceDevice = MNG;
         // Chargement du fichier de config correspondant pour le programme
         f_Settings = new QSettings(SETTING_MNG, QSettings::IniFormat);
      }
   }
   /// ####################################################################################################################

   // Si le nom n'est pas renseigné s'arrêté
   if(f_ConfigName == QLatin1String(""))
      return;

   this->SetSourceNameFromConfigName();

   emit this->SIGNAL_DeviceNameObtained();
}



///
/// \fn GetFirmwareVersion
/// \brief Récupère l'information sur le firmware, le décode et propose un nom de fichier log
/// \param timeout
///
void CLASS_CONFIG_SOURCE::DecodeFirmwareVersion(const QString &message)
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
      f_FileName = LOG_REPERTORY %  LOG_SOURCE_DEVICE % QStringLiteral("_") % f_DeviceName % QStringLiteral("_") % f_FirmwareVersion % QStringLiteral("_") %f_CurrentTime;
   else
      f_FileName = LOG_REPERTORY %  LOG_CUSTOM_FORMAT % QStringLiteral("_") % LOG_SOURCE_DEVICE % QStringLiteral("_") % f_DeviceName % QStringLiteral("_") % f_FirmwareVersion % QStringLiteral("_") %f_CurrentTime;

   // On initialise le compteur à 0 pour la fonction GetDataOutput
   f_Output.setNum(f_OutputNumber);

   // Lancement de la demande d'information sur la sortie
   if(f_DeviceName == QLatin1String("") || f_DeviceName == QLatin1String("?"))
      return;

   emit this->SIGNAL_OutputInformationObtained();
}



///
/// \fn GetDataOutput
/// \brief Récupère les informations sur les sorties de la machine
/// \param timeout
///
void CLASS_CONFIG_SOURCE::CheckOutputStatus(const QString &message)
{
   QString InfoOutput = message;
   InfoOutput.chop(2);
   if(InfoOutput == f_Settings->value(QStringLiteral("COMMAND/OUT_AVAILABLE")).toString() % f_Output % QStringLiteral(",") % f_Settings->value(QStringLiteral("INFO/OUTPUT_ACTIVE")).toString())
   {
      f_OutputNumber += 1;
      f_Plug.setNum(f_LoopPlugCounter);

      emit this->SIGNAL_PlugInformationObtained();

      f_AvailableOutput = f_OutputNumber;
   }

   else if(InfoOutput == f_Settings->value(QStringLiteral("COMMAND/OUT_AVAILABLE")).toString() % f_Output % QStringLiteral(",") % f_Settings->value(QStringLiteral("INFO/OUTPUT_NOT_AVAILABLE")).toString())
   {
      // On met une valeur dans le tableau on utilisera ceci pour indiquer qu'une sortie est indisponible
      f_Tab.insert(0, NO_OUTPUT);
      // Le plug est réinitialisé à 0
      f_Plug.setNum(0);
      // On se base sur le nombre de output disponible pour remplir le tableau pour éviter un saut d'indice
      f_OutputTab.insert(f_LoopCounter, f_Tab);

      // On passe à la boucle suivante
      f_LoopCounter += 1 ;
      f_Tab.clear();

      // On demande l'information sur la sortie
      f_Output.setNum(f_LoopCounter);

      if(f_LoopCounter < f_Settings->value(QStringLiteral("INFO/MAX_OUTPUT")).toInt())
         emit this->SIGNAL_OutputInformationObtained();
      else
      {
         emit this->SIGNAL_InterfaceSourceObtained();
         this->CreateTxFileConfig();
      }
   }
}


///
/// \fn GetDataPlug
/// \brief Récupère les informations sur les plugs de la machine
/// \param timeout
///
void CLASS_CONFIG_SOURCE::CheckPlugStatus(const QString &message)
{
   // On récupère les plugs disponibles sur la sortie dans un tableau  
   QString InfoPlug = message;
   InfoPlug.chop(2);
   if(InfoPlug != f_Settings->value(QStringLiteral("COMMAND/OUT_PLUG_STATUS")).toString() % f_Output % QStringLiteral(",") % f_Plug % QStringLiteral(",") % f_Settings->value(QStringLiteral("INFO/OUTPUT_NOT_AVAILABLE")).toString())
   {
      f_Tab.insert(f_Index, f_LoopPlugCounter);
      f_Index += 1;
   }

   f_LoopPlugCounter += 1;

   f_Output.setNum(f_LoopCounter);
   f_Plug.setNum(f_LoopPlugCounter);

   if( f_LoopPlugCounter < f_Settings->value(QStringLiteral("INFO/MAX_OUTPUT_PLUG")).toInt())
      emit this->SIGNAL_PlugInformationObtained();

   else
   {
      // On met à jour le tableau 2D de sortie et de plug
      f_OutputTab.insert(f_LoopCounter, f_Tab);

      // On réinitialise avant de changer de sortie
      f_LoopPlugCounter = 0;
      f_Plug.setNum(f_LoopPlugCounter);
      f_Index = 0;
      f_Tab.clear();

      // On passe à la sortie suivante
      f_LoopCounter += 1;
      f_Output.setNum(f_LoopCounter);

      if(f_LoopCounter < f_Settings->value(QStringLiteral("INFO/MAX_OUTPUT")).toInt())
         emit this->SIGNAL_OutputInformationObtained();
      else
      {
         emit this->SIGNAL_InterfaceSourceObtained();
         this->CreateTxFileConfig();
      }
   }
}

///
/// \fn CreateTxFileConfig
/// \brief Créer le fichier de configuration de la machine source
///
void CLASS_CONFIG_SOURCE::CreateTxFileConfig(void)
{
   /// ############################################## Spécification machines ##############################################
   QFile fileTxLivecore(QStringLiteral("settings/settingTx_LiveCore.ini"));
   QFile fileTxVio(QStringLiteral("settings/settingTx_Vio4K.ini"));
   QFile fileTxNlc(QStringLiteral("settings/settingTx_NLC.ini"));
   QFile fileTxMng(QStringLiteral("settings/settingTx_MNG.ini"));

   //Suppression des fichiers s'ils existent
   if(fileTxLivecore.exists())
      fileTxLivecore.remove();

   if(fileTxVio.exists())
      fileTxVio.remove();

   if(fileTxNlc.exists())
      fileTxNlc.remove();

   if(fileTxMng.exists())
      fileTxMng.remove();

   /// #####################################################################################################################

   // Création du fichier de config de la source
   if(f_SettingsTx != nullptr)
   {
      delete f_SettingsTx;
      f_SettingsTx = nullptr;
   }

   f_SettingsTx = new QSettings(QStringLiteral("settings/settingTx_") % f_ConfigName % QStringLiteral(".ini"), QSettings::IniFormat);

   f_SettingsTx->beginGroup(QStringLiteral("OUTPUT_IDENTIFIED"));
   for(int i = 0 ; i < f_OutputTab.size() ; ++i )
   {
      if(f_OutputTab.at(i).at(0) != NO_OUTPUT)
      {
         QString iStr;
         iStr.setNum(i);
         f_SettingsTx->setValue(f_Settings->value(QStringLiteral("OUTPUT_VALUE/") % iStr).toString() , i);
      }
   }
   f_SettingsTx->endGroup();

   f_SettingsTx->beginGroup(QStringLiteral("PLUG_IDENTIFIED"));
   for(int i = 0 ; i < f_OutputTab.size() ; ++i )
   {
      if(f_OutputTab.at(i).at(0) != NO_OUTPUT)
      {
         QString iStr;
         iStr.setNum(i);
         for(int j = 0 ; j < f_OutputTab.at(i).size() ; ++j)
         {
            if(f_OutputTab.at(i).at(j) != NO_PLUG)
            {
               QString jStr;
               jStr.setNum(f_OutputTab.at(i).at(j));
               f_SettingsTx->setValue(f_Settings->value(QStringLiteral("OUTPUT_VALUE/") % iStr).toString() % QStringLiteral("/") % f_Settings->value(QStringLiteral("OUTPUT_PLUG_VALUE/") % jStr).toString() , f_OutputTab.at(i).at(j));
            }
         }
      }
   }
   f_SettingsTx->endGroup();
}

///
/// \brief CLASS_CONFIG_SOURCE::SetSourceNameFromConfigName
///
void CLASS_CONFIG_SOURCE::SetSourceNameFromConfigName()
{
   ExtSourceName = f_ConfigName;
}
