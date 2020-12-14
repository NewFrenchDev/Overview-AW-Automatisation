///
/// \file Class_TestReceiver.cpp
/// \brief Classe CLASS_TEST_RECEIVER
/// \author ANALOG WAY - GLE
/// \date 2018-07-10
///
/// Fonctions de la classe CLASS_TEST_RECEIVER
///

// C++ Standard
// Librairies Qt
// Modules
#include "Class_TestReceiver.h"

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
/// \fn CLASS_TEST_RECEIVER(QObject *parent)
/// \brief Constructeur de CLASS_TEST_RECEIVER
///
/// \param[in] parent : Objet parent
///
CLASS_TEST_RECEIVER::CLASS_TEST_RECEIVER():
   f_InputDecrypted(""),
   f_PlugDecrypted(""),
   f_RealPlug(""),
   f_InfoPlugSignal(""),
   f_InfoCurrentRate(""),
   f_InfoCurrentFormat(""),
   f_ListInputArray(),
   f_ListPlugArray(),
   f_SizeListInputAndPlugArray(0),
   f_IndexArrayListInputAndPlug(0),
   f_IndexListInputAndPlug(0),
   f_FirstInput(true),
   f_IndicCommand(0),
   f_ConfigReceiverAccessed(nullptr)
{
}

///
/// \fn ~CLASS_TEST_RECEIVER()
/// \brief Destructeur de CLASS_TEST_RECEIVER
///
CLASS_TEST_RECEIVER::~CLASS_TEST_RECEIVER()
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
/// \fn AccessToConfigReceiverData
/// \brief Récupère le pointeur vers le configReceiver crée
/// \param configReceiverToAccess : configReceiver à accéder pour les informations
///
void CLASS_TEST_RECEIVER::AccessToConfigReceiverData(CLASS_CONFIG_RECEIVER* configReceiverToAccess)
{
   if(f_ConfigReceiverAccessed != nullptr)
   {
      delete f_ConfigReceiverAccessed;
      f_ConfigReceiverAccessed = nullptr;
   }
   f_ConfigReceiverAccessed = configReceiverToAccess;
}

///
/// \fn CheckInfoSignal
/// \brief Demande à vérifier l'information sur le signal
///
void CLASS_TEST_RECEIVER::CheckInfoSignal(void)
{
   if(f_IndexListInputAndPlug == 0)
     f_FirstInput = true;


   if(f_IndexArrayListInputAndPlug < f_SizeListInputAndPlugArray)
   {
      if(f_IndexListInputAndPlug < f_ListInputArray.at(f_IndexArrayListInputAndPlug).size())
      {
         f_InputDecrypted = f_ConfigReceiverAccessed->f_SettingsRx->value(QStringLiteral("INPUT_IDENTIFIED/") % f_ListInputArray.at(f_IndexArrayListInputAndPlug).at(f_IndexListInputAndPlug)).toByteArray();

         /// ############################################## Spécification machines ##############################################
         if(f_ConfigReceiverAccessed->f_ConfigName == QLatin1String("LiveCore"))
         {
            f_PlugDecrypted = f_ConfigReceiverAccessed->f_Settings->value(QStringLiteral("INPUT_PLUG/") % f_ListPlugArray.at(f_IndexArrayListInputAndPlug).at(f_IndexListInputAndPlug)).toByteArray();
         }
         else if (f_ConfigReceiverAccessed->f_ConfigName == QLatin1String("Vio4K"))
         {
            if(QVariant(f_InputDecrypted).toInt() < 7 )
               f_PlugDecrypted = "0";
            else if(QVariant(f_InputDecrypted).toInt() >= 7)
               f_PlugDecrypted = f_ConfigReceiverAccessed->f_Settings->value(QStringLiteral("INPUT_PLUG/") % f_ListPlugArray.at(f_IndexArrayListInputAndPlug).at(f_IndexListInputAndPlug)).toByteArray();
         }
         /// ####################################################################################################################

         f_RealPlug = f_ConfigReceiverAccessed->f_Settings->value(QStringLiteral("INPUT_PLUG/") % f_ListPlugArray.at(f_IndexArrayListInputAndPlug).at(f_IndexListInputAndPlug)).toByteArray();

         emit this->SIGNAL_CurrentInput(f_ListInputArray.at(f_IndexArrayListInputAndPlug).at(f_IndexListInputAndPlug));

         emit this->SIGNAL_AskInfoSignal();

         // On passe au plug suivant
         f_IndexListInputAndPlug += 1 ;
      }
      else
      {
         if(ExtCustomFormatSelected == false)
         {
            // On indique à l'appareil source qu'il faut passer au rate suivant
            emit this->SIGNAL_NextRateApplied();
         }
         else if(ExtCustomFormatSelected == true)
         {
            if(ExtFirstSlot == false)
            {
               // On indique à l'appareil source qu'il faut passer au format suivant
               emit this->SIGNAL_NextFormatApplied();
            }
            else if(ExtFirstSlot == true)
            {
               ExtFirstSlot = false;
               // On indique à l'appareil source qu'il faut recharger les slots
               emit this->SIGNAL_NextSlotCustom();
            }
         }
         f_IndexListInputAndPlug = 0;
      }
   }
}


///
/// \fn WriteInfoSignal
/// \brief Ecrit l'information du signal dans le fichier log
/// \param message : information récupérée à partie de la fenetre du receiver
///
void CLASS_TEST_RECEIVER::WriteInfoSignal(const QString &message)
{
   QString FormatValue, FormatDetected;
   QByteArray Tag, TagFormat;
   QByteArray TagRate ;

   // Ecriture du rate appliqué
   if(ExtCustomFormatSelected == false)
      TagRate = "\n " % QVariant(QDateTime::currentDateTime().toString(QStringLiteral("hh:mm:ss.zzz"))).toByteArray() %  " **************************************************** Rate Applied : " % f_ConfigReceiverAccessed->f_SettingSource->value(QStringLiteral("RATE_VALUE/") % ExtRateValue).toByteArray() %  " *************************************************************";
   else
      TagRate = "\n " % QVariant(QDateTime::currentDateTime().toString(QStringLiteral("hh:mm:ss.zzz"))).toByteArray() %  " **************************************************** Rate Applied : " % ExtCustomRate %  " Hz *************************************************************";

   // On récupère l'information sur le format
   f_InfoCurrentFormat = message;
   f_InfoCurrentFormat.chop(2);
   FormatValue = f_InfoCurrentFormat.section(',' , 2);

   // Vérifie l'existence du format dans le fichier de config
   FormatDetected = f_ConfigReceiverAccessed->f_Settings->value("INPUT_FORMAT_VALUE/" % FormatValue).toByteArray() ;
   if(FormatDetected != "")
   {
      TagFormat = " Format détecté : " % f_ConfigReceiverAccessed->f_Settings->value("INPUT_FORMAT_VALUE/" % FormatValue).toByteArray();
   }
   else
   {
      TagFormat = " Format détecté : UNNAMED";
   }

   QFile fileValid(f_ConfigReceiverAccessed->f_FileName % QStringLiteral("_VALID.txt"));
   if(!fileValid.open(QIODevice::WriteOnly | QIODevice::Append))
      return;

   // Ecriture dans le fichier log
   if(f_InfoPlugSignal == f_ConfigReceiverAccessed->f_Settings->value(QStringLiteral("COMMAND/SIG_SCAN_VALID")).toString() % f_InputDecrypted % QStringLiteral(",") % f_PlugDecrypted % QStringLiteral(",1"))
   {
      Tag = "\n" % f_ConfigReceiverAccessed->f_Settings->value(QStringLiteral("INPUT_VALUE/") % QVariant(f_InputDecrypted).toString()).toByteArray() % " Signal valide sur le plug " % f_ConfigReceiverAccessed->f_Settings->value(QStringLiteral("INPUT_PLUG_VALUE/") % QVariant(f_RealPlug).toString()).toByteArray() % "\t\t" % TagFormat % "\t\t Rate detecté : " % QVariant(f_InfoCurrentRate).toByteArray() % " Hz" ;
      if(f_FirstInput == true)
      {
         f_FirstInput = false;
         fileValid.write(TagRate);
      }
      fileValid.write(Tag);
   }
   else
   {
      Tag = "\n" % f_ConfigReceiverAccessed->f_Settings->value(QStringLiteral("INPUT_VALUE/") % QVariant(f_InputDecrypted).toString()).toByteArray() % " Signal invalide sur le plug " % f_ConfigReceiverAccessed->f_Settings->value("INPUT_PLUG_VALUE/" % QVariant(f_RealPlug).toString()).toByteArray() % "\t\t" % TagFormat % "\t\t Rate detecté : " % QVariant(f_InfoCurrentRate).toByteArray() % " Hz";
      if(f_FirstInput == true)
      {
         f_FirstInput = false;
         fileValid.write(TagRate);
      }
      fileValid.write(Tag);
   }
   fileValid.close();

   emit this->SIGNAL_AskSignalSize();
}



///
/// \fn WriteSignalSize
/// \brief Ecrit l'information sur la taille du signal dans le fichier log
/// \param message : information récupérée à partie de la fenetre du receiver
///
void CLASS_TEST_RECEIVER::WriteSignalSize(const QString &message)
{
   QString InfoSize;
   int lenght = 0;

   InfoSize = message;
   InfoSize.chop(2);
   int size = InfoSize.size();

   for(int i = 0; i < size ; ++i)
   {
      if(InfoSize.at(lenght) == QLatin1String(","))
      {
         InfoSize.remove(0 , lenght);
         lenght = 0;
      }
      lenght += 1;
   }

   // Si une virgule a été omise l'enlever
   if(InfoSize.at(0) == QLatin1String(","))
      InfoSize.remove(0 , 1);

   QFile fileValid(f_ConfigReceiverAccessed->f_FileName + QStringLiteral("_VALID.txt"));
   if(!fileValid.open(QIODevice::WriteOnly | QIODevice::Append))
      return;

   switch (f_IndicCommand)
   {
      default:
         break;
      case 0:
         fileValid.write("\n Largeur du signal : " % QVariant(InfoSize).toByteArray() % " pixels");
         f_IndicCommand += 1;
         emit this->SIGNAL_AskSignalSize();
         break;
      case 1:
         fileValid.write("\n Hauteur du signal : " % QVariant(InfoSize).toByteArray() % " lines");
         f_IndicCommand += 1;
         emit this->SIGNAL_AskSignalSize();
         break;
      case 2:
         fileValid.write("\n Largeur du format : " % QVariant(InfoSize).toByteArray() % " pixels");
         f_IndicCommand += 1;
         emit this->SIGNAL_AskSignalSize();
         break;
      case 3:
         fileValid.write("\n Hauteur du format : " % QVariant(InfoSize).toByteArray() % " lines");
         f_IndicCommand = 0;
         this->CheckInfoSignal();
         break;
   }
   fileValid.close();
}


///
/// \fn ListInputFromConfigWindow
/// \brief Permet de créer notre liste d'input à partir des données stockées dans la fenêtre de configuration
/// \return Liste de QString
///
void CLASS_TEST_RECEIVER::ListInputFromConfigWindow(void)
{
   for(int ligne = 0; ligne < 24; ++ligne)
   {
      if(ExtTableau2D.at(ligne)->at(0) != QLatin1String(""))
      {
         QStringList ListInput;
         for(int colonne = 2; colonne < 18; colonne += 2)
         {
            if(ExtTableau2D.at(ligne)->at(colonne) != QLatin1String(""))
            {
               ListInput.append(ExtTableau2D.at(ligne)->at(colonne));
            }
         }
         f_ListInputArray.append(ListInput);
      }
   }
}

///
/// \fn ListPlugFromConfigWindow
/// \brief Permet de créer la liste de plug des inputs à partir des données stockées dans la fenêtre de configuration
///
void CLASS_TEST_RECEIVER::ListPlugFromConfigWindow(void)
{
   for(int ligne = 0; ligne < 24; ++ligne)
   {
      if(ExtTableau2D.at(ligne)->at(0) != QLatin1String(""))
      {
         QStringList ListPlug;
         for(int colonne = 3; colonne < 18; colonne += 2)
         {
            if(ExtTableau2D.at(ligne)->at(colonne) != QLatin1String(""))
            {
               ListPlug.append(ExtTableau2D.at(ligne)->at(colonne));
            }
         }
         f_ListPlugArray.append(ListPlug);
      }
   }
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
