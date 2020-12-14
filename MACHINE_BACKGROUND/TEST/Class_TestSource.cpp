///
/// \file Class_TestSource.cpp
/// \brief Classe CLASS_TEST_SOURCE
/// \author ANALOG WAY - GLE
/// \date 2018-07-04
///
/// Fonctions de la classe CLASS_TEST_SOURCE
///

// C++ Standard
// Librairies Qt
// Modules
#include "Class_TestSource.h"


/******************************** DEFINITIONS ********************************/
// Compatibilité format et rate
#define NOT_INFORMED                   0
#define INCOMPATIBLE                   1
#define COMPATIBLE                     2

// Output/Plug absent
#define NO_PLUG                        -1
#define NO_OUTPUT                      -1
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
/// \fn CLASS_TEST_SOURCE()
/// \brief Constructeur de CLASS_TEST_SOURCE
///
///
CLASS_TEST_SOURCE::CLASS_TEST_SOURCE():
  f_IndexListFormat(0),
  f_IndexListRate(0),
  f_FormatList(),
  f_RateList(),
  f_CurrentFormat(0),
  f_CurrentRate(0),
  f_CurrentOutputPlugValue(0),
  f_CurrentOutput(0),
  f_ListOutput(),
  f_IndexListOutput(0),
  f_CurrentPlug(""),
  f_ListPlug(),
  f_IndexListPlug(0), 
  f_TwoSlotSaved(false),
  f_FirstCustomSlot(true),
  f_FirstLaunch(true),
  f_ListCustomFormat(nullptr),
  f_InfoCompatibility(0),
  f_PlugStatus(""),
  f_StepCustom(NONE),
  f_Time(0),
  f_ConfigSourceAccessed(nullptr)
{
}

///
/// \fn ~CLASS_TEST_SOURCE()
/// \brief Destructeur de CLASS_TEST_SOURCE
///
CLASS_TEST_SOURCE::~CLASS_TEST_SOURCE()
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
/// \fn AccessToConfigSourceData
/// \brief Récupère le pointeur sur le configSource créé
/// \param configSourceToAccess : le configSource à accéder pour les informations
///
void CLASS_TEST_SOURCE::AccessToConfigSourceData(CLASS_CONFIG_SOURCE* configSourceToAccess)
{
   if(f_ConfigSourceAccessed != nullptr)
   {
      delete f_ConfigSourceAccessed;
      f_ConfigSourceAccessed = nullptr;
   }

   f_ConfigSourceAccessed = configSourceToAccess;
}


///
/// \fn SendFormat
/// \brief Envoie le format
///
void CLASS_TEST_SOURCE::SendFormat(void)
{
   QString testFormatConfigFile;
   testFormatConfigFile = f_ConfigSourceAccessed->f_Settings->value(QStringLiteral("OUTPUT_FORMAT_VALUE/") % QVariant(f_CurrentFormat).toString()).toString();

   // On vérifie que le format est écrit dans le fichier de config si non on passe au suivant
   if( testFormatConfigFile == QLatin1String("") && (f_IndexListFormat < f_FormatList.size()) && ExtCustomFormatSelected == false)
   {
      f_CurrentFormat += 1;
      f_IndexListFormat += 1;
      // On relance la fonction
      this->SendFormat();
   }
   else if(ExtCustomFormatSelected == false)
   {
      if(f_IndexListFormat < f_FormatList.size())
      {
         /// ############################################## Spécification machines ##############################################
         // Format à ignorer s'ils sont présents dans le fichier de config
         if(f_ConfigSourceAccessed->f_ConfigName == QLatin1String("LiveCore"))
         {
            if(f_CurrentFormat == 38)
            {
               for(int i = 38; i < 48; ++i)
               {
                  f_CurrentFormat += 1;
                  f_IndexListFormat += 1;
               }
            }
         }
         /// ###################################################################################################################

         this->AddOutputAndFormatTagInLogFile();

         // Envoi le signal pour récupérer les information dans la fenetre de l'appareil receiver après avoir renseigné le nom et le format en global
         emit this->SIGNAL_GetOutputInfo();

         // On dit au GUI d'envoyer le format
         emit this->SIGNAL_SendFormat();
      }
      else
      {
         // Si on a testé toutes les sorties on arrête
         if(f_IndexListOutput == (f_ListOutput.size() - 1))
            emit this->SIGNAL_Stop();      
         else
         {
            // On passe à la sortie suivante
            f_CurrentOutput = f_ConfigSourceAccessed->f_SettingsTx->value(QStringLiteral("OUTPUT_IDENTIFIED/") % f_ListOutput.at(f_IndexListOutput += 1)).toInt();
            f_CurrentPlug = f_ListPlug.at(f_IndexListPlug += 1);

            // Création de la liste de format adaptée
            if((f_CurrentPlug == QLatin1String("SDI")) || (f_CurrentPlug == QLatin1String("OPTICAL SFP")))
            {
               this->CreateListFormat(f_CurrentOutput , f_ConfigSourceAccessed->f_Settings->value(QStringLiteral("INFO/MAX_OUTPUT_FORMAT_SDI_OPT")).toInt());
            }
            else
            {
               this->CreateListFormat(f_CurrentOutput , f_ConfigSourceAccessed->f_Settings->value(QStringLiteral("INFO/MAX_OUTPUT_FORMAT")).toInt());
            }

            // Création de la liste de rate adaptée
            this->CreateListRate(f_CurrentOutput , f_ConfigSourceAccessed->f_Settings->value(QStringLiteral("INFO/MAX_RATE")).toInt());

            // On recommence depuis le premier format
            f_IndexListFormat = 0;
            f_CurrentFormat = 0;
            emit this->SIGNAL_NextOutputApplied();

            this->SendFormat();
         }
      }
   }
   else if(ExtCustomFormatSelected == true)
   {
      // On ignore s'il s'agit d'un SDI ou d'un OPTICAL_SFP
      if(f_CurrentPlug == QLatin1String("SDI") || f_CurrentPlug == QLatin1String("OPTICAL SFP"))
      {
         f_CurrentOutput = f_ConfigSourceAccessed->f_SettingsTx->value(QStringLiteral("OUTPUT_IDENTIFIED/") % f_ListOutput.at(f_IndexListOutput += 1)).toInt();
         f_CurrentPlug = f_ListPlug.at(f_IndexListPlug += 1);
         // On signale à l'appareil récepteur qu'on passe au output suivant
         emit this->SIGNAL_NextOutputApplied();
         this->CreateCommandListCustomFormat(f_CurrentOutput);

         this->SendFormat();
      }
      else if(f_IndexListFormat < f_FormatList.size())
      {
         /// ############################################## Spécification machines ##############################################
         if(f_CurrentFormat == 60 || f_CurrentFormat == 40)
         {
            if(f_ConfigSourceAccessed->f_ConfigName == QLatin1String("Vio4K"))
               f_CurrentFormat = 58;
            else if(f_ConfigSourceAccessed->f_ConfigName == QLatin1String("LiveCore"))
               f_CurrentFormat = 38;
         }
         /// ####################################################################################################################

         this->AddOutputAndFormatTagInLogFile();

         // Envoi le signal pour récupérer les information dans la fenetre de l'appareil receiver après avoir renseigné le nom et le format en global
         emit this->SIGNAL_GetOutputInfo();

         // On envoie la commande correspondant à l'élément de la liste
         emit this->SIGNAL_SendFormat();
      }
      else
      {
         // Si on a testé toutes les sorties on arrête
         if(f_IndexListOutput == (f_ListOutput.size() - 1))
         {
            emit this->SIGNAL_Stop();
         }
         else
         {
            // On passe à la sortie suivante
            f_CurrentOutput = f_ConfigSourceAccessed->f_SettingsTx->value("OUTPUT_IDENTIFIED/" % f_ListOutput.at(f_IndexListOutput += 1)).toInt();
            f_CurrentPlug = f_ListPlug.at(f_IndexListPlug += 1);
            // On signale à l'appareil récepteur qu'on passe au output suivant
            emit this->SIGNAL_NextOutputApplied();

            if( ((f_CurrentPlug == QLatin1String("SDI") || f_CurrentPlug == QLatin1String("OPTICAL SFP")) && f_IndexListOutput == (f_ListOutput.size() - 1)) || f_IndexListOutput >= f_ListOutput.size())
            {
               emit this->SIGNAL_Stop();
               return;
            }

            // On ignore la sortie si c'est un SDI ou OPTICAL_SFP et on passe à la suivante
            else if(((f_CurrentPlug == QLatin1String("SDI") || f_CurrentPlug == QLatin1String("OPTICAL SFP")) && f_IndexListOutput < (f_ListOutput.size() - 1)))
            {
               f_CurrentOutput = f_ConfigSourceAccessed->f_SettingsTx->value("OUTPUT_IDENTIFIED/" % f_ListOutput.at(f_IndexListOutput += 1)).toInt();
               f_CurrentPlug = f_ListPlug.at(f_IndexListPlug += 1);
               // On signale à l'appareil récepteur qu'on passe au output suivant
               emit this->SIGNAL_NextOutputApplied();
            }

            // On recré les commandes qui correspondent à la sortie
            this->CreateCommandListCustomFormat(f_CurrentOutput);

            // On recommence depuis le premier format
            f_IndexListFormat = 0;

            /// ############################################## Spécification machines ##############################################
            if(f_ConfigSourceAccessed->f_ConfigName == "Vio4K")
               f_CurrentFormat = 58;
            else if(f_ConfigSourceAccessed->f_ConfigName == "LiveCore")
               f_CurrentFormat = 38;
            /// ####################################################################################################################

            // On réinitialise la variable de premier lancement des custom format
            f_FirstLaunch = true;

            // On réinitialise l'index de la liste de custom format
            this->f_ListCustomFormat->ReinitializeIndexCustomList();

            // On relance la fonction comme si c'était la première fois
            this->SelectCustomSlot();
         }
      }
   }
}


///
/// \fn SendRate
/// \brief Indique à la fenetre d'envoyer le rate
///
void CLASS_TEST_SOURCE::SendRate(void)
{
   // Ajout du temps pour la progress bar de la fenetre de configuration
   emit this->SIGNAL_AddTimeValue(f_Time);
   // Réinitialisation du temps
   f_Time = 0;

   if(ExtCustomFormatSelected == false)
   {
      QString testRateConfigFile;
      testRateConfigFile = f_ConfigSourceAccessed->f_Settings->value("RATE_VALUE/" % QVariant(f_CurrentRate).toString()).toString();

      // On vérifie que le format est écrit dans le fichier de config si non on passe au suivant
      if(testRateConfigFile == "" && (f_IndexListRate < f_RateList.size()))
      {
         f_CurrentRate += 1;
         f_IndexListRate += 1;
         // On relance la fonction
         this->SendRate();
      }
      else
      {
         // Si format standard sélectionné
         if(f_IndexListRate < f_RateList.size())
         {
            // Indication dans le log
            QString rateName;
            rateName.setNum(f_CurrentRate);
            ExtRateValue = rateName;

            emit this->SIGNAL_CurrentRate(f_ConfigSourceAccessed->f_Settings->value("RATE_VALUE/" % rateName).toString());

            emit this->SIGNAL_SendRate();
         }
         else
         {
            // Retour au premier rate de la liste
            f_IndexListRate = 0;

            /// ############################################## Spécification machines ##############################################
            if(f_ConfigSourceAccessed->f_ConfigName == "Vio4K" || f_ConfigSourceAccessed->f_ConfigName == "NLC")
               f_CurrentRate = 0;
            else if(f_ConfigSourceAccessed->f_ConfigName == "LiveCore")
               f_CurrentRate = 1;
            /// ####################################################################################################################

            // On passe au format suivant
            f_CurrentFormat += 1;
            // Mise à jour de l'index pour passer au format suivant
            f_IndexListFormat += 1;
            this->SendFormat();
         }
      }
   }
   else if(ExtCustomFormatSelected == true)
   {
      QByteArray rateName;
      int rateNum;
      rateNum = (f_ListCustomFormat->f_ListCustomFormatGenerated.at(f_IndexListFormat).Rate) / 1000;
      rateName.setNum(rateNum);
      ExtCustomRate = rateName;
      emit this->SIGNAL_CurrentRate(QVariant(rateName).toString() % " Hz");

      if(f_ConfigSourceAccessed->f_ConfigName == "LiveCore")
         emit this->SIGNAL_ChangeToRateCustom();

      // On change de slot custom
      if(f_FirstCustomSlot == true)
         f_FirstCustomSlot = false;
      else
         f_FirstCustomSlot = true;

      emit this->SIGNAL_RetardLaunchStatusPlug();
   }
}

///
/// \fn CheckOutputPlugStatus
/// \brief Indique à la fenetre de vérifier le status du plug
///
void CLASS_TEST_SOURCE::CheckOutputPlugStatus(void)
{
   // On envoie le temps écoulé à la fenetre de configuration
   emit this->SIGNAL_AddTimeValue(f_Time);
   // Réinitialisation du temps
   f_Time = 0;

   if(f_CurrentOutput < f_ConfigSourceAccessed->f_OutputTab.size())
   {
      f_OutputValue.setNum(f_CurrentOutput);
      if(f_CurrentOutputPlugValue < f_ConfigSourceAccessed->f_OutputTab[f_CurrentOutput].size())
      {
        if(f_ConfigSourceAccessed->f_OutputTab[f_CurrentOutput].at(f_CurrentOutputPlugValue) != NO_PLUG )
         {
            f_OutputPlugValue.setNum(f_ConfigSourceAccessed->f_OutputTab[f_CurrentOutput].at(f_CurrentOutputPlugValue));
            emit this->SIGNAL_StatusPlug();
         }
         else
            f_CurrentOutputPlugValue += 1;
      }
      else
      {
         // Réinitialisation
         f_CurrentOutputPlugValue = 0;

         if(ExtCustomFormatSelected == false)
         {
            // Mise a jour numéro du rate
            f_CurrentRate += 1;
            // On passe au rate suivant
            f_IndexListRate += 1;
         }
         else if(ExtCustomFormatSelected == true)
         {
            // On passe au format suivant
            f_IndexListFormat += 1;
            f_CurrentFormat += 1;

            if(f_IndexListFormat % 2 == 0 && f_IndexListFormat != 0)
            {
               f_FirstLaunch = true;
               // Permet d'indiquer à l'appareil récepteur que nous revenons au slot 1
               ExtFirstSlot = true;
            }
         }
         emit this->SIGNAL_Start();
      }
   }
}


///
/// \fn WriteFileLog
/// \brief Ecrit dans le log de la machine source
///
void CLASS_TEST_SOURCE::WriteFileLog(void)
{
   // Variables locales
   QString MessagePlugStatus;
   QString outputValue;
   QString PlugStatusValidity;
   QString outputPlugValue;
   QByteArray Tag;
   // Mise a jour des variables locales
   outputValue.setNum(f_CurrentOutput);
   outputPlugValue.setNum(f_ConfigSourceAccessed->f_OutputTab[f_CurrentOutput].at(f_CurrentOutputPlugValue));


   // Création fichier
   QFile fileNotValid(f_ConfigSourceAccessed->f_FileName + "_NOT_VALID.txt");
   QFile fileValid(f_ConfigSourceAccessed->f_FileName + "_VALID.txt");
   QTextStream outNotValid(&fileNotValid);
   QTextStream outValid(&fileValid);

   // On vérifie que les fichiers sont bien ouverts
   if(!fileNotValid.open(QIODevice::WriteOnly | QIODevice::Append))
      return;
   if(!fileValid.open(QIODevice::WriteOnly | QIODevice::Append))
      return;

   // Ecriture uniquement si c'est le premier plug
   if(f_CurrentOutputPlugValue == 0)
   {
      // Ecriture du rate appliqué
      if(ExtCustomFormatSelected == false)
         Tag = "\n " % QVariant(QDateTime::currentDateTime().toString("hh:mm:ss.zzz")).toByteArray() %  " ******************************* Rate : " % f_ConfigSourceAccessed->f_Settings->value("RATE_VALUE/" % ExtRateValue).toByteArray() % " *************************************" ;
      else
         Tag = "\n " % QVariant(QDateTime::currentDateTime().toString("hh:mm:ss.zzz")).toByteArray() %  " ******************************* Rate : " % ExtCustomRate % " Hz *************************************" ;

      /// ############################################## Spécification machines ##############################################
      if(f_InfoCompatibility == INCOMPATIBLE)
      {
         fileNotValid.write(Tag);
         if(f_ConfigSourceAccessed->f_ConfigName == "Vio4K")
            fileNotValid.write("\n RATE NOT SUPPORTED ");
         else if(f_ConfigSourceAccessed->f_ConfigName == "LiveCore")
            fileNotValid.write("\n FORMAT AND RATE NOT COMPATIBLE");

         if(ExtCustomFormatSelected == true)
         {
            fileNotValid.write("\n " % f_ListCustomFormat->GetPixels() % " pixels");
            fileNotValid.write("\n " % f_ListCustomFormat->GetLines() % " lines" );
            f_ListCustomFormat->NextFormatInfo();
         }
      }
      else if(f_InfoCompatibility == COMPATIBLE)
      {
         fileValid.write(Tag);
         if(f_ConfigSourceAccessed->f_ConfigName == "Vio4K")
            fileValid.write("\n RATE SUPPORTED ");
         else if(f_ConfigSourceAccessed->f_ConfigName == "LiveCore")
            fileValid.write("\n FORMAT AND RATE COMPATIBLE");

         if(ExtCustomFormatSelected == true)
         {
            fileValid.write("\n " % f_ListCustomFormat->GetPixels() % " pixels");
            fileValid.write("\n " % f_ListCustomFormat->GetLines() % " lines" );           
            f_ListCustomFormat->NextFormatInfo();
         }
      }
      else if(f_InfoCompatibility == NOT_INFORMED)
      {
         fileNotValid.write(Tag);
         fileNotValid.write("\n NOT INFORMED ");

         if(ExtCustomFormatSelected == true)
         {
            fileNotValid.open(QIODevice::WriteOnly | QIODevice::Append);
            fileNotValid.write("\n " % f_ListCustomFormat->GetPixels() % " pixels");
            fileNotValid.write("\n " % f_ListCustomFormat->GetLines() % " lines" );        
            f_ListCustomFormat->NextFormatInfo();
         }
      }
   }

   /// ############################################## Spécification machines ##############################################
   // Distinction des machines pour évaluer le plugStatus
   if(f_ConfigSourceAccessed->f_ConfigName == "Vio4K")
      PlugStatusValidity = f_ConfigSourceAccessed->f_Settings->value("COMMAND/OUT_PLUG_STATUS").toString() % outputValue % "," % outputPlugValue % "," % f_ConfigSourceAccessed->f_Settings->value("OUTPUT_PLUG_STATUS/ACTIVE").toString();
   else if(f_ConfigSourceAccessed->f_ConfigName == "LiveCore")
      PlugStatusValidity = f_ConfigSourceAccessed->f_Settings->value("COMMAND/OUT_PLUG_STATUS").toString() % outputValue %  ",1";
   /// ####################################################################################################################

   // Ecriture si le plug est actif
   if(f_PlugStatus == PlugStatusValidity)
   {
      MessagePlugStatus = "Plug " % f_ConfigSourceAccessed->f_Settings->value("OUTPUT_PLUG_VALUE/" % outputPlugValue).toString() % " is active";
      if(f_InfoCompatibility == INCOMPATIBLE || f_InfoCompatibility == NOT_INFORMED)
      {
         outNotValid << "\n  " % MessagePlugStatus ;
      }
      else if(f_InfoCompatibility == COMPATIBLE)
      {
         outValid << "\n " % MessagePlugStatus;
      }
   }

   // Fermeture des fichiers
   fileNotValid.close();
   fileValid.close();

   emit this->SIGNAL_NextPlugToTest();
}

///
/// \fn AddOutputAndFormatTagInLogFile
/// \brief Ecrit le tag de l'Output et du format dans le fichier log
///
void CLASS_TEST_SOURCE::AddOutputAndFormatTagInLogFile(void)
{
   // Indication dans le log
   QString formatName;
   QString outputName;

   formatName.setNum(f_CurrentFormat);
   outputName.setNum(f_CurrentOutput);
   ExtOutputFormat = f_ConfigSourceAccessed->f_Settings->value("OUTPUT_FORMAT_VALUE/" % formatName).toByteArray();
   ExtOutputName = f_ConfigSourceAccessed->f_Settings->value("OUTPUT_VALUE/" % outputName).toByteArray();

   emit this->SIGNAL_CurrentOutput(QVariant(ExtOutputName).toString());
   emit this->SIGNAL_CurrentFormat(QVariant(ExtOutputFormat).toString());

   QByteArray Tag = "\n\n\n" % QVariant(QDateTime::currentDateTime().toString("hh:mm:ss.zzz")).toByteArray() %  " ******************************* Output : "  % f_ConfigSourceAccessed->f_Settings->value("OUTPUT_VALUE/" % outputName).toByteArray() % "   Format : " % f_ConfigSourceAccessed->f_Settings->value("OUTPUT_FORMAT_VALUE/" % formatName).toByteArray() % " *************************************\n" ;
   // Ecriture du tag nom de l'appareil et la version du firmware
   QFile fileNotValid(f_ConfigSourceAccessed->f_FileName + "_NOT_VALID.txt");
   if(!fileNotValid.open(QIODevice::WriteOnly | QIODevice::Append))
      return;
   fileNotValid.write(Tag);
   fileNotValid.close();

   // On vérifie que le fichier est ouvert
   QFile fileValid(f_ConfigSourceAccessed->f_FileName + "_VALID.txt");
   if(!fileValid.open(QIODevice::WriteOnly | QIODevice::Append))
      return;
   fileValid.write(Tag);
   fileValid.close();
}



///
/// \fn CreateListFormat
/// \brief Crée la liste de format
/// \param outputNumber : nombre d'output
/// \param numberFormat : nombre de format
///
void CLASS_TEST_SOURCE::CreateListFormat(int outputNumber , int formatNumber)
{
   // Création liste de format
   QByteArray IndexFormat;
   QByteArray output;

   // Si une liste existe déjà on la supprime
   f_FormatList.clear();

   // Création de la liste en fonction de l'output actuel
   output.setNum(outputNumber);
   if(f_ConfigSourceAccessed->f_OutputTab[outputNumber].contains(NO_OUTPUT))
      qDebug() << " Sortie " % output % " indisponible";
   else
   {
      if(ExtCustomFormatSelected == false)
      {
         for(int i=0 ; i < formatNumber ; ++i )
         {
            IndexFormat.setNum(i);
            output.setNum(outputNumber);
            f_FormatList.append( output % "," % IndexFormat % f_ConfigSourceAccessed->f_Settings->value("COMMAND/OUT_FORMAT").toByteArray());
         }
      }
   }
}

///
/// \fn CreateListRate
/// \brief Crée la liste de rate
/// \param outputNumber : nombre d'output
/// \param rateNumber : nombre de rate
///
void CLASS_TEST_SOURCE::CreateListRate(int outputNumber , int rateNumber)
{
   // Création liste de rate
   QByteArray IndexRate;
   QByteArray output;

   // Si une liste existe déjà on la supprime
   f_RateList.clear();

   output.setNum(outputNumber);
   if(f_ConfigSourceAccessed->f_OutputTab[outputNumber].contains(NO_OUTPUT))
      qDebug() << " Sortie " % output % " indisponible";
   else
   {
      /// ############################################## Spécification machines ##############################################
      if(f_ConfigSourceAccessed->f_ConfigName == "Vio4K")
      {
         for(int i = 0 ; i < rateNumber ; ++i )
         {
            IndexRate.setNum(i);
            output.setNum(outputNumber);
            f_RateList.append( output % "," % IndexRate % f_ConfigSourceAccessed->f_Settings->value("COMMAND/OUT_RATE").toByteArray());
         }
      }
      else if(f_ConfigSourceAccessed->f_ConfigName == "LiveCore")
      {
         for(int i = 1 ; i < rateNumber ; ++i )
         {
            IndexRate.setNum(i);
            output.setNum(outputNumber);
            f_RateList.append( output % "," % IndexRate % f_ConfigSourceAccessed->f_Settings->value("COMMAND/OUT_RATE").toByteArray());
         }
      }
      /// ####################################################################################################################
   }
}

///
/// \fn ListOutputFromConfigWindow
/// \brief Permet de créer notre liste d'output à partir des données stockées dans la fenêtre de configuration
/// \return Liste de QString
///
QStringList CLASS_TEST_SOURCE::ListOutputFromConfigWindow(void)
{
   QStringList ListOutput;

   for(int ligne = 0; ligne < 24; ++ligne)
   {
      if(ExtTableau2D.at(ligne)->at(0) != "")
         ListOutput.append(ExtTableau2D.at(ligne)->at(0));
   }
   return ListOutput;
}

///
/// \fn ListPlugFromConfigWindow
/// \brief Permet de créer notre liste d'output à partir des données stockées dans la fenêtre de configuration
/// \return Liste de QString
///
QStringList CLASS_TEST_SOURCE::ListPlugFromConfigWindow(void)
{
   QStringList ListPlug;

   for(int ligne = 0; ligne < 24; ++ligne)
   {
      if(ExtTableau2D.at(ligne)->at(1) != "")
         ListPlug.append(ExtTableau2D.at(ligne)->at(1));
   }
   return ListPlug;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///
/// \fn  CreateCommandListCustomFormat
/// \brief Créer une liste de commande pour l'application des formats custom
/// \param outputNumber : nombre d'output
///
void CLASS_TEST_SOURCE::CreateCommandListCustomFormat(int outputNumber)
{
   // Création liste de format
   QByteArray IndexFormat;
   QByteArray output;

   output.setNum(outputNumber);

   // Si une liste existe déjà on la supprime
   f_FormatList.clear();

   /// ############################################## Spécification machines ##############################################
   if(ExtSourceName == "Vio4K")
   {
      for( int i = 0 ; i < 10 ; ++i)
      {
         for( int firstCustomFormat = 58 ; firstCustomFormat < 60; ++firstCustomFormat )
         {
            IndexFormat.setNum(firstCustomFormat);
            f_FormatList.append( output % "," % IndexFormat % f_ConfigSourceAccessed->f_Settings->value("COMMAND/OUT_FORMAT").toByteArray());
         }
      }
   }
   else if(ExtSourceName == "LiveCore")
   {
      for( int i = 0; i < 10; ++ i)
      {
         for( int firstCustomFormat = 38 ; firstCustomFormat < 40; ++firstCustomFormat )
         {
            IndexFormat.setNum(firstCustomFormat);
            f_FormatList.append( output % "," % IndexFormat % f_ConfigSourceAccessed->f_Settings->value("COMMAND/OUT_FORMAT").toByteArray());
         }
      }
   }
   /// ######################################################################################################################
}

///
/// \fn SelectCustomSlot
/// \brief Selectionne le slot à utiliser
///
void CLASS_TEST_SOURCE::SelectCustomSlot(void)
{
   QByteArray Command;

   if(f_ListCustomFormat->f_slot1Selected == false)
      Command = f_ListCustomFormat->SelectSlot0();
   else
      Command = f_ListCustomFormat->SelectSlot1();

   f_StepCustom = RANDOM_RATE;
   emit this->SIGNAL_CustomSlotSelectionned(Command);
}

///
/// \fn SetRandomRate
/// \brief Applique un rate aléatoire
///
QByteArray CLASS_TEST_SOURCE::SetRandomRate(void)
{
   QByteArray Command;
   Command = f_ListCustomFormat->LoadRandomRate();

   f_StepCustom = ACTIVE_H;
   return Command;
}

///
/// \fn SetActiveH
/// \brief Applique le nouveau ActiveH
///
QByteArray CLASS_TEST_SOURCE::SetActiveH(void)
{
   QByteArray Command;
   Command = f_ListCustomFormat->LoadActiveH();

   f_StepCustom = FRONT_PORCH_H;
   return Command;
}

///
/// \fn SetFrontPorchH
/// \brief Applique le nouveau FrontPorchH
///
QByteArray CLASS_TEST_SOURCE::SetFrontPorchH(void)
{
   QByteArray Command;
   Command = f_ListCustomFormat->LoadFrontPorchH();

   f_StepCustom = BACK_PORCH_H;
   return Command;
}

///
/// \fn SetBackPorchH
/// \brief Applique le nouveau BackPorchH
///
QByteArray CLASS_TEST_SOURCE::SetBackPorchH(void)
{
   QByteArray Command;
   Command = f_ListCustomFormat->LoadBackPorchH();

   f_StepCustom = SYNCHRO_H;
   return Command;
}

///
/// \fn SetSynchroH
/// \brief Applique la nouvelle SynchroH
///
QByteArray CLASS_TEST_SOURCE::SetSynchroH(void)
{
   QByteArray Command;
   Command = f_ListCustomFormat->LoadSynchroH();

   f_StepCustom = POLARITY_H;
   return Command;
}

///
/// \fn SetPolarityH
/// \brief Applique la PolarityH
///
QByteArray CLASS_TEST_SOURCE::SetPolarityH(void)
{
   QByteArray Command;
   Command = f_ListCustomFormat->LoadPolarityH();

   f_StepCustom = ACTIVE_V;
   return Command;
}

///
/// \fn SetActiveV
/// \brief Applique le nouveau ActiveV
///
QByteArray CLASS_TEST_SOURCE::SetActiveV(void)
{
   QByteArray Command;
   Command = f_ListCustomFormat->LoadActiveV();

   f_StepCustom = FRONT_PORCH_V;
   return Command;
}

///
/// \fn FrontPorchV
/// \brief Applique le nouveau FrontPorchV
///
QByteArray CLASS_TEST_SOURCE::SetFrontPorchV(void)
{
   QByteArray Command;
   Command = f_ListCustomFormat->LoadFrontPorchV();

   f_StepCustom = BACK_PORCH_V;
   return Command;
}

///
/// \fn SetBackPorchV
/// \brief Applique le nouveau BackPorchV
///
QByteArray CLASS_TEST_SOURCE::SetBackPorchV(void)
{
   QByteArray Command;
   Command = f_ListCustomFormat->LoadBackPorchV();

   f_StepCustom = SYNCHRO_V;
   return Command;
}

///
/// \fn SetSynchroV
/// \brief Applique la nouvelle SynchroV
///
QByteArray CLASS_TEST_SOURCE::SetSynchroV(void)
{
   QByteArray Command;
   Command = f_ListCustomFormat->LoadSynchroV();

   f_StepCustom = POLARITY_V;
   return Command;
}

///
/// \fn SetPolarityV
/// \brief Applique la PolarityV
///
QByteArray CLASS_TEST_SOURCE::SetPolarityV(void)
{
   QByteArray Command;
   Command = f_ListCustomFormat->LoadPolarityV();

   f_StepCustom = CHECK_CUSTOM;
   return Command;
}

///
/// \fn CheckCustomStatus
/// \brief Applique la vérification du format obligatoire avant la sauvegarde
///
QByteArray CLASS_TEST_SOURCE::CheckCustomStatus(void)
{
   QByteArray Command;
   Command = f_ListCustomFormat->CheckStatus();

   f_StepCustom = SAVE_CUSTOM;
   return Command;
}

///
/// \fn SaveCustomFormat
/// \brief Sauvegarde le format dans le slot sélectionné
///
QByteArray CLASS_TEST_SOURCE::SaveCustomFormat(void)
{
   QByteArray Command;
   Command = f_ListCustomFormat->SaveCustomFormat();

   f_StepCustom = NEXT_CUSTOM;
   return Command;
}

///
/// \fn NextCustom
/// \brief Passe au custom suivant suivi de son envoi à la machine précédé du chargement du second slot si besoin
///
void CLASS_TEST_SOURCE::NextCustom(void)
{
   f_StepCustom = NONE;

   f_ListCustomFormat->NextCustomFormat();

   if(f_TwoSlotSaved == true)
   {
      f_TwoSlotSaved = false;
      if(f_FirstLaunch == true)
      {
         f_FirstLaunch = false;
         this->SendFormat();
      }
   }
   else
   {
      f_TwoSlotSaved = true;

      this->SelectCustomSlot();
   }
}


///
/// \fn GenerateCustomFormatList
/// \brief Génère une liste de format custom
///
void CLASS_TEST_SOURCE::GenerateCustomFormatList(void)
{
   if(f_ListCustomFormat != nullptr)
   {
      delete f_ListCustomFormat;
      f_ListCustomFormat = nullptr;
      f_ListCustomFormat = new CLASS_CUSTOM_FORMAT;
   }
   else
       f_ListCustomFormat = new CLASS_CUSTOM_FORMAT;
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
