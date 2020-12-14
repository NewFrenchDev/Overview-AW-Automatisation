///
/// \file Class_CustomFormat.cpp
/// \brief Classe CLASS_CUSTOM_FORMAT
/// \author ANALOG WAY - GLE
/// \date 2018-06-08
///
/// Fonctions de la classe CLASS_CUSTOM_FORMAT
///

// C++ Standard
// Librairies Qt
// Modules
#include "Class_CustomFormat.h"

/******************************** DEFINITIONS ********************************/

//// Indication la fonction rand prend la valeur max en exclue et min inclue

//Pour les petits formats
#define SMALL_MAX_ACTIVE_H       1500
#define SMALL_MIN_ACTIVE_H       600
#define SMALL_MAX_FRONTPORCH_H   100
#define SMALL_MIN_FRONTPORCH_H   1
#define SMALL_MAX_BACKPORCH_H    100
#define SMALL_MIN_BACKPORCH_H    1
#define SMALL_MAX_SYNCHRO_H      150
#define SMALL_MIN_SYNCHRO_H      1
#define SMALL_MAX_ACTIVE_V       900
#define SMALL_MIN_ACTIVE_V       500
#define SMALL_MAX_FRONTPORCH_V   100
#define SMALL_MIN_FRONTPORCH_V   1
#define SMALL_MAX_BACKPORCH_V    100
#define SMALL_MIN_BACKPORCH_V    1
#define SMALL_MAX_SYNCHRO_V      10
#define SMALL_MIN_SYNCHRO_V      1

//Pour les grands formats
#define BIG_MAX_ACTIVE_H         4096
#define BIG_MIN_ACTIVE_H         1500
#define BIG_MAX_FRONTPORCH_H     250
#define BIG_MIN_FRONTPORCH_H     100
#define BIG_MAX_BACKPORCH_H      250
#define BIG_MIN_BACKPORCH_H      100
#define BIG_MAX_SYNCHRO_H        300
#define BIG_MIN_SYNCHRO_H        150
#define BIG_MAX_ACTIVE_V         2160
#define BIG_MIN_ACTIVE_V         900
#define BIG_MAX_FRONTPORCH_V     200
#define BIG_MIN_FRONTPORCH_V     100
#define BIG_MAX_BACKPORCH_V      200
#define BIG_MIN_BACKPORCH_V      100
#define BIG_MAX_SYNCHRO_V        20
#define BIG_MIN_SYNCHRO_V        10

#define POLARITY                 2        //  2 exclu



//// Nom des fichiers à lire
// Le fichier de config du Vio
#define SETTING_VIO                    QStringLiteral("settings/settingVio.ini")
// Le fichier de config du LiveCore
#define SETTING_LIVECORE               QStringLiteral("settings/settingLiveCore.ini")
// Le fichier de config du NLC
#define SETTING_NLC                    QStringLiteral("settings/settingNLC.ini")
// Le fichier de config du NMG
#define SETTING_MNG                    QStringLiteral("settings/settingMNG.ini")

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
/// \fn CLASS_CUSTOM_FORMAT(QObject *parent)
/// \brief Constructeur de CLASS_CUSTOM_FORMAT
///
/// \param[in] parent : Objet parent
///
CLASS_CUSTOM_FORMAT::CLASS_CUSTOM_FORMAT(QObject *parent):
   QObject(parent),
   f_settingSource(nullptr),
   f_slot1Selected(false),
   f_indexListFormatGenerated(0),
   f_indexListInfo(0)
{
   this->GenerateRandomFormat();
}

///
/// \fn ~CLASS_CUSTOM_FORMAT()
/// \brief Destructeur de CLASS_CUSTOM_FORMAT
///
CLASS_CUSTOM_FORMAT::~CLASS_CUSTOM_FORMAT()
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

/******************************************************************************
 *                               Slots publiques                              *
 *****************************************************************************/

/******************************************************************************
 *                              MÃ©thodes privÃ©es                              *
 *****************************************************************************/
///
/// \fn LoadConfigFileForCustomFormat
/// \brief Charge le fichier config adéquate pour l'éxécution des autres fonctions
///
void CLASS_CUSTOM_FORMAT::LoadConfigFileForCustomFormat(void)
{
   if(f_settingSource != nullptr)
   {
      delete f_settingSource;
      f_settingSource = nullptr;
   }

   if(ExtSourceName == QLatin1String("LiveCore"))
      f_settingSource = new QSettings (SETTING_LIVECORE , QSettings::IniFormat);
   else if(ExtSourceName == QLatin1String("Vio4K"))
      f_settingSource = new QSettings (SETTING_VIO , QSettings::IniFormat);
   else if(ExtSourceName == QLatin1String("NLC"))
      f_settingSource = new QSettings (SETTING_NLC , QSettings::IniFormat);
   else if(ExtSourceName == QLatin1String("MNG"))
      f_settingSource = new QSettings (SETTING_MNG , QSettings::IniFormat);
}

///
/// \brief CLASS_CUSTOM_FORMAT::GetCustomFormatSetting
/// \return
///
QSettings* CLASS_CUSTOM_FORMAT::GetCustomFormatSetting(void)
{
   return f_settingSource;
}


///
/// \fn SelectSlot0
/// \brief Pour dire à la machine de sélection le SLOT 0 pour pouvoir stocker notre format custom
/// \return la commande en QByteArray
///
QByteArray CLASS_CUSTOM_FORMAT::SelectSlot0(void)
{
   f_slot1Selected = true;

   QByteArray Command;
   if(ExtSourceName == QLatin1String("Vio4K"))
      Command = QByteArrayLiteral("0,1") % f_settingSource->value(QStringLiteral("COMMAND/CUSTOM_COMP_LOAD_FROM_CUSTOM")).toByteArray();
   else if(ExtSourceName == QLatin1String("LiveCore"))
      Command = QByteArrayLiteral("0,1") % f_settingSource->value(QStringLiteral("COMMAND/OCCOMPUTE_LOAD")).toByteArray();
   return Command;
}

///
/// \fn SelectSlot1
/// \brief Pour dire à la machine de sélection le SLOT 1 pour pouvoir stocker notre format custom
/// \return la commande en QByteArray
///
QByteArray CLASS_CUSTOM_FORMAT::SelectSlot1(void)
{
   f_slot1Selected = false;

   QByteArray Command;
   if(ExtSourceName == QLatin1String("Vio4K"))
      Command = "1,1" % f_settingSource->value(QStringLiteral("COMMAND/CUSTOM_COMP_LOAD_FROM_CUSTOM")).toByteArray();
   else if(ExtSourceName == QLatin1String("LiveCore"))
      Command = "1,1" % f_settingSource->value(QStringLiteral("COMMAND/OCCOMPUTE_LOAD")).toByteArray();
   return Command;
}


///
/// \fn CheckStatus
/// \brief Permet de vérifier si le format créer est conforme
///        Etape obligatoire avant de sauvegarder le format
/// \return la commande en QByteArray
///
QByteArray CLASS_CUSTOM_FORMAT::CheckStatus(void)
{
   QByteArray Command;

   if(ExtSourceName == QLatin1String("Vio4K"))
      Command = "1" % f_settingSource->value(QStringLiteral("COMMAND/CUSTOM_COMP_CHECK_REQUEST")).toByteArray();
   else if(ExtSourceName == QLatin1String("LiveCore"))
      Command = "1" % f_settingSource->value(QStringLiteral("COMMAND/OCCOMPUTE_CHECK_REQUEST")).toByteArray();

   return Command;
}

///
/// \fn SaveCustomFormat
/// \brief Sauvegarde le format dans le slot précédemment sélectionner
/// \return la commande en QByteArray
///
QByteArray CLASS_CUSTOM_FORMAT::SaveCustomFormat(void)
{
   QByteArray Command;

   if(f_slot1Selected == false)
   {
      if(ExtSourceName == QLatin1String("Vio4K"))
         Command = "1,1" % f_settingSource->value(QStringLiteral("COMMAND/CUSTOM_COMP_SAVE_FORMAT")).toByteArray();
      else if(ExtSourceName == QLatin1String("LiveCore"))
         Command = "1,1" % f_settingSource->value(QStringLiteral("COMMAND/OCCOMPUTE_SAVE")).toByteArray();
   }
   else
   {
      if(ExtSourceName == QLatin1String("Vio4K"))
         Command = "0,1" % f_settingSource->value(QStringLiteral("COMMAND/CUSTOM_COMP_SAVE_FORMAT")).toByteArray();
      else if(ExtSourceName == QLatin1String("LiveCore"))
         Command = "0,1" % f_settingSource->value(QStringLiteral("COMMAND/OCCOMPUTE_SAVE")).toByteArray();
   }
   return Command;
}

///
/// \fn LoadActiveH
/// \brief Creer la commande pour charger un nombre pour l'ActiveH
/// \return la commande en QByteArray
///
QByteArray CLASS_CUSTOM_FORMAT::LoadActiveH(void)
{
   CUSTOM_FORMAT Format;
   QByteArray info;
   Format = f_ListCustomFormatGenerated.at(f_indexListFormatGenerated);
   if(ExtSourceName == QLatin1String("Vio4K"))
      info = QVariant(Format.ActiveH).toByteArray() % f_settingSource->value(QStringLiteral("COMMAND/CUSTOM_COMP_FULL_CVT_HUTIL")).toByteArray();
   else if(ExtSourceName == QLatin1String("LiveCore"))
      info = QVariant(Format.ActiveH).toByteArray() % f_settingSource->value(QStringLiteral("COMMAND/OCCOMPUTE_FULL_HUTIL")).toByteArray();
   return info;
}

///
/// \fn LoadFrontPorchH
/// \brief Creer la commande pour charger un nombre pour le FrontPorchH
/// \return la commande en QByteArray
///
QByteArray CLASS_CUSTOM_FORMAT::LoadFrontPorchH(void)
{
   CUSTOM_FORMAT Format;
   QByteArray info;
   Format = f_ListCustomFormatGenerated.at(f_indexListFormatGenerated);
   if(ExtSourceName == QLatin1String("Vio4K"))
      info = QVariant(Format.FrontPorchH).toByteArray() % f_settingSource->value(QStringLiteral("COMMAND/CUSTOM_COMP_FULL_HFRONTPORCH")).toByteArray();
   else if(ExtSourceName == QLatin1String("LiveCore"))
      info = QVariant(Format.FrontPorchH).toByteArray() % f_settingSource->value(QStringLiteral("COMMAND/OCCOMPUTE_FULL_HFRONTPORCH")).toByteArray();
   return info;
}

///
/// \fn LoadBackPorchH
/// \brief Creer la commande pour charger un nombre pour le BackPorchH
/// \return la commande en QByteArray
///
QByteArray CLASS_CUSTOM_FORMAT::LoadBackPorchH(void)
{
   CUSTOM_FORMAT Format;
   QByteArray info;
   Format = f_ListCustomFormatGenerated.at(f_indexListFormatGenerated);
   if(ExtSourceName == QLatin1String("Vio4K"))
      info = QVariant(Format.BackPorchH).toByteArray()  % f_settingSource->value(QStringLiteral("COMMAND/CUSTOM_COMP_FULL_HBACKPORCH")).toByteArray();
   else if(ExtSourceName == QLatin1String("LiveCore"))
      info = QVariant(Format.BackPorchH).toByteArray()  % f_settingSource->value(QStringLiteral("COMMAND/OCCOMPUTE_FULL_HBACKPORCH")).toByteArray();
   return info;
}

///
/// \fn LoadSynchroH
/// \brief Creer la commande pour charger un nombre pour le BackPorchH
/// \return la commande en QByteArray
///
QByteArray CLASS_CUSTOM_FORMAT::LoadSynchroH(void)
{
   CUSTOM_FORMAT Format;
   QByteArray info;
   Format = f_ListCustomFormatGenerated.at(f_indexListFormatGenerated);
   if(ExtSourceName == QLatin1String("Vio4K"))
      info = QVariant(Format.SynchroH).toByteArray() % f_settingSource->value(QStringLiteral("COMMAND/CUSTOM_COMP_FULL_HSYNC")).toByteArray();
   else if(ExtSourceName == QLatin1String("LiveCore"))
      info = QVariant(Format.SynchroH).toByteArray() % f_settingSource->value(QStringLiteral("COMMAND/OCCOMPUTE_FULL_HSYNC")).toByteArray();
   return info;
}

///
/// \fn LoadPolarityH
/// \brief Creer la commande pour charger un nombre pour le SynchroH
/// \return la commande en QByteArray
///
QByteArray CLASS_CUSTOM_FORMAT::LoadPolarityH(void)
{
   CUSTOM_FORMAT Format;
   QByteArray info;
   Format = f_ListCustomFormatGenerated.at(f_indexListFormatGenerated);
   if(ExtSourceName == QLatin1String("Vio4K"))
      info = QVariant(Format.PolarityH).toByteArray() % f_settingSource->value(QStringLiteral("COMMAND/CUSTOM_COMP_FULL_HSYNC_POL")).toByteArray();
   else if(ExtSourceName == QLatin1String("LiveCore"))
      info = QVariant(Format.PolarityH).toByteArray() % f_settingSource->value(QStringLiteral("COMMAND/OCCOMPUTE_FULL_HSYNC_POL")).toByteArray();
   return info;
}

///
/// \fn LoadActiveV
/// \brief Creer la commande pour charger un nombre pour le ActiveV
/// \return la commande en QByteArray
///
QByteArray CLASS_CUSTOM_FORMAT::LoadActiveV(void)
{
   CUSTOM_FORMAT Format;
   QByteArray info;
   Format = f_ListCustomFormatGenerated.at(f_indexListFormatGenerated);
   if(ExtSourceName == QLatin1String("Vio4K"))
      info = QVariant(Format.ActiveV).toByteArray()  % f_settingSource->value(QStringLiteral("COMMAND/CUSTOM_COMP_FULL_CVT_VUTIL")).toByteArray();
   else if(ExtSourceName == QLatin1String("LiveCore"))
      info = QVariant(Format.ActiveV).toByteArray()  % f_settingSource->value(QStringLiteral("COMMAND/OCCOMPUTE_FULL_VUTIL")).toByteArray();
   return info;
}

///
/// \fn LoadFrontPorchV
/// \brief Creer la commande pour charger un nombre pour le FrontPorchV
/// \return la commande en QByteArray
///
QByteArray CLASS_CUSTOM_FORMAT::LoadFrontPorchV(void)
{
   CUSTOM_FORMAT Format;
   QByteArray info;
   Format = f_ListCustomFormatGenerated.at(f_indexListFormatGenerated);
   if(ExtSourceName == QLatin1String("Vio4K"))
      info = QVariant(Format.FrontPorchV).toByteArray() % f_settingSource->value(QStringLiteral("COMMAND/CUSTOM_COMP_FULL_VFRONTPORCH")).toByteArray();
   else if(ExtSourceName == QLatin1String("LiveCore"))
      info = QVariant(Format.FrontPorchV).toByteArray() % f_settingSource->value(QStringLiteral("COMMAND/OCCOMPUTE_FULL_VFRONTPORCH")).toByteArray();
   return info;
}

///
/// \fn LoadBackPorchV
/// \brief Creer la commande pour charger un nombre pour le BackPorchV
/// \return la commande en QByteArray
///
QByteArray CLASS_CUSTOM_FORMAT::LoadBackPorchV(void)
{
   CUSTOM_FORMAT Format;
   QByteArray info;
   Format = f_ListCustomFormatGenerated.at(f_indexListFormatGenerated);
   if(ExtSourceName == QLatin1String("Vio4K"))
      info = QVariant(Format.BackPorchV).toByteArray() % f_settingSource->value(QStringLiteral("COMMAND/CUSTOM_COMP_FULL_VBACKPORCH")).toByteArray();
   else if(ExtSourceName == QLatin1String("LiveCore"))
      info = QVariant(Format.BackPorchV).toByteArray() % f_settingSource->value(QStringLiteral("COMMAND/OCCOMPUTE_FULL_VBACKPORCH")).toByteArray();
   return info;
}

///
/// \fn LoadSynchroV
/// \brief Creer la commande pour charger un nombre pour le SynchroV
/// \return la commande en QByteArray
///
QByteArray CLASS_CUSTOM_FORMAT::LoadSynchroV(void)
{
   CUSTOM_FORMAT Format;
   QByteArray info;
   Format = f_ListCustomFormatGenerated.at(f_indexListFormatGenerated);
   if(ExtSourceName == QLatin1String("Vio4K"))
      info = QVariant(Format.SynchroV).toByteArray() % f_settingSource->value(QStringLiteral("COMMAND/CUSTOM_COMP_FULL_VSYNC")).toByteArray();
   else if(ExtSourceName == QLatin1String("LiveCore"))
      info = QVariant(Format.SynchroV).toByteArray() % f_settingSource->value(QStringLiteral("COMMAND/OCCOMPUTE_FULL_VSYNC")).toByteArray();
   return info;
}

///
/// \fn LoadPolarityV
/// \brief Creer la commande pour charger un nombre pour le PolarityV
/// \return la commande en QByteArray
///
QByteArray CLASS_CUSTOM_FORMAT::LoadPolarityV(void)
{
   CUSTOM_FORMAT Format;
   QByteArray info;
   Format = f_ListCustomFormatGenerated.at(f_indexListFormatGenerated);
   if(ExtSourceName == QLatin1String("Vio4K"))
      info = QVariant(Format.PolarityV).toByteArray() % f_settingSource->value(QStringLiteral("COMMAND/CUSTOM_COMP_FULL_VSYNC_POL")).toByteArray();
   else if(ExtSourceName == QLatin1String("LiveCore"))
      info = QVariant(Format.PolarityV).toByteArray() % f_settingSource->value(QStringLiteral("COMMAND/OCCOMPUTE_FULL_VSYNC_POL")).toByteArray();
   return info;
}

///
/// \fn LoadRandomRate
/// \brief Creer la commande pour charger un nombre pour le Rate
/// \return la commande en QByteArray
///
QByteArray CLASS_CUSTOM_FORMAT::LoadRandomRate(void)
{
   CUSTOM_FORMAT Format;
   QByteArray info;
   Format = f_ListCustomFormatGenerated.at(f_indexListFormatGenerated);
   if(ExtSourceName == QLatin1String("Vio4K"))
      info = QVariant(Format.Rate).toByteArray() % f_settingSource->value(QStringLiteral("COMMAND/CUSTOM_COMP_FULL_CVT_RATE")).toByteArray();
   else if(ExtSourceName == QLatin1String("LiveCore"))
      info = QVariant(Format.Rate).toByteArray() % f_settingSource->value(QStringLiteral("COMMAND/OCCOMPUTE_FULL_RATE")).toByteArray();
   return info;
}

///
/// \brief CLASS_CUSTOM_FORMAT::GetPixels
/// \return
///
QByteArray CLASS_CUSTOM_FORMAT::GetPixels(void)
{
   CUSTOM_FORMAT Format;
   QByteArray info;

   Format = f_ListCustomFormatGenerated.at(f_indexListInfo);
   info = QVariant(Format.ActiveH).toByteArray();

   return info;
}

///
/// \brief CLASS_CUSTOM_FORMAT::GetLines
/// \return
///
QByteArray CLASS_CUSTOM_FORMAT::GetLines(void)
{
   CUSTOM_FORMAT Format;
   QByteArray info;

   Format = f_ListCustomFormatGenerated.at(f_indexListInfo);
   info = QVariant(Format.ActiveV).toByteArray();

   return info;
}



///
/// \fn NextCustomFormat
/// \brief Permet de passer au format suivant stocker dans la liste f_ListCustomFormatGenerated
///
void CLASS_CUSTOM_FORMAT::NextCustomFormat(void)
{
   if(f_indexListFormatGenerated < f_ListCustomFormatGenerated.size() - 1)
      f_indexListFormatGenerated += 1;
   else
   {
      f_indexListFormatGenerated = 0;
   }
}

///
/// \brief CLASS_CUSTOM_FORMAT::NextFormatInfo
///
void CLASS_CUSTOM_FORMAT::NextFormatInfo(void)
{
   f_indexListInfo += 1;
}

///
/// \brief CLASS_CUSTOM_FORMAT::ReinitializeIndexCustomList
///
void CLASS_CUSTOM_FORMAT::ReinitializeIndexCustomList(void)
{
   f_indexListFormatGenerated = 0;
   f_indexListInfo = 0;
}

///
/// \fn GenerateRandomFormat
/// \brief Génèrer 20 formats custom aléatoires
///
void CLASS_CUSTOM_FORMAT::GenerateRandomFormat(void)
{
   for(int numberformat = 0; numberformat < 10; ++numberformat)
   {
      CUSTOM_FORMAT Format;
      int randomPixels;

      randomPixels = rand() % (SMALL_MAX_ACTIVE_H - SMALL_MIN_ACTIVE_H) + SMALL_MIN_ACTIVE_H;

      for(int i = 0; i < 8 ; ++i)
      {
         if(randomPixels % 4 == 0)
         {
            Format.ActiveH = randomPixels;
         }
         else
         {
            randomPixels += 1;
         }
      }

      Format.FrontPorchH = rand() % (SMALL_MAX_FRONTPORCH_H - SMALL_MIN_FRONTPORCH_H) + SMALL_MIN_FRONTPORCH_H;
      Format.BackPorchH = rand() % (SMALL_MAX_BACKPORCH_H - SMALL_MIN_BACKPORCH_H) + SMALL_MIN_BACKPORCH_H;
      Format.SynchroH = rand() % (SMALL_MAX_SYNCHRO_H - SMALL_MIN_SYNCHRO_H) + SMALL_MIN_SYNCHRO_H;
      Format.PolarityH = rand() % POLARITY;
      Format.ActiveV = rand() % (SMALL_MAX_ACTIVE_V - SMALL_MIN_ACTIVE_V) + SMALL_MIN_ACTIVE_V;
      Format.FrontPorchV = rand() % (SMALL_MAX_FRONTPORCH_V - SMALL_MIN_FRONTPORCH_V) + SMALL_MIN_FRONTPORCH_V;
      Format.BackPorchV = rand() % (SMALL_MAX_BACKPORCH_V - SMALL_MIN_BACKPORCH_V) + SMALL_MIN_BACKPORCH_V;
      Format.SynchroV = rand() % (SMALL_MAX_SYNCHRO_V - SMALL_MIN_SYNCHRO_V) + SMALL_MIN_SYNCHRO_V;
      Format.PolarityV = rand() % POLARITY;
      Format.Rate = (rand() % (81 - 40) + 40) * 1000;

      f_ListCustomFormatGenerated.append(Format);
   }

   for(int numberformat = 10; numberformat < 20; ++numberformat)
   {
      CUSTOM_FORMAT Format;
      int randomPixels;

      randomPixels = rand() % (BIG_MAX_ACTIVE_H - BIG_MIN_ACTIVE_H) + BIG_MIN_ACTIVE_H;

      for(int i = 0; i < 8 ; ++i)
      {
         if(randomPixels % 4 == 0)
         {
            Format.ActiveH = randomPixels;
         }
         else
         {
            randomPixels += 1;
         }
      }

      Format.FrontPorchH = rand() % (BIG_MAX_FRONTPORCH_H - BIG_MIN_FRONTPORCH_H) + BIG_MIN_FRONTPORCH_H;
      Format.BackPorchH = rand() % (BIG_MAX_BACKPORCH_H - BIG_MIN_BACKPORCH_H) + BIG_MIN_BACKPORCH_H;
      Format.SynchroH = rand() % (BIG_MAX_SYNCHRO_H - BIG_MIN_SYNCHRO_H) + BIG_MIN_SYNCHRO_H;
      Format.PolarityH = rand() % POLARITY;
      Format.ActiveV = rand() % (BIG_MAX_ACTIVE_V - BIG_MIN_ACTIVE_V) + BIG_MIN_ACTIVE_V;
      Format.FrontPorchV = rand() % (BIG_MAX_FRONTPORCH_V - BIG_MIN_FRONTPORCH_V) + BIG_MIN_FRONTPORCH_V;
      Format.BackPorchV = rand() % (BIG_MAX_BACKPORCH_V - BIG_MIN_BACKPORCH_V) + BIG_MIN_BACKPORCH_V;
      Format.SynchroV = rand() % (BIG_MAX_SYNCHRO_V - BIG_MIN_SYNCHRO_V) + BIG_MIN_SYNCHRO_V;
      Format.PolarityV = rand() % POLARITY;
      Format.Rate = (rand() % (81 - 40) + 40) * 1000;

      f_ListCustomFormatGenerated.append(Format);
   }
}

/******************************************************************************
 *                             Operateur surcharge                            *
 *****************************************************************************/
