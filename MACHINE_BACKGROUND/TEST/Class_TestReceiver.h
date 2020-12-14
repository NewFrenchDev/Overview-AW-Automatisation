///
/// \file Class_TestReceiver.h
/// \brief Classe CLASS_TEST_RECEIVER
/// \author ANALOG WAY - GLE
/// \date 2018-07-10
///

#ifndef CLASS_TESTRECEIVER_H
#define CLASS_TESTRECEIVER_H

// C++ Standard
// Librairies Qt
#include <QObject>
#include <QSettings>
#include <QFile>
// Modules
#include "COMMON_TypeDefs.h"
#include "global_windows_variables.h"
#include "Class_ConfigReceiver.h"

///
/// \class CLASS_TEST_RECEIVER
/// \brief Classe CLASS_TEST_RECEIVER
///
class CLASS_TEST_RECEIVER : public QObject
{
      Q_OBJECT

   public:

      /*--------------------------- Types publiques ---------------------------*/

      /*------------------------- Constantes publiques ------------------------*/

      /*------------------------- Variables publiques -------------------------*/

      /*---------------------- Constructeur / Destructeur ---------------------*/

      ///
      /// \fn CLASS_TEST_RECEIVER()
      /// \brief Constructeur de CLASS_TEST_RECEIVER
      ///
      CLASS_TEST_RECEIVER();

      ///
      /// \fn ~CLASS_TEST_RECEIVER()
      /// \brief Destructeur de CLASS_TEST_RECEIVER
      ///
      ~CLASS_TEST_RECEIVER();

      /*------------------------------ Accesseurs -----------------------------*/

      /*------------------------------ Mutateurs ------------------------------*/

      /*-------------------------- MÃ©thodes publiques -------------------------*/

      ///
      /// \fn AccessToConfigReceiverData
      /// \brief Récupère le pointeur vers le configReceiver crée
      /// \param configReceiverToAccess : configReceiver à accéder pour les informations
      ///
      void AccessToConfigReceiverData(CLASS_CONFIG_RECEIVER *configReceiverToAccess);

      ///
      /// \fn CheckInfoSignal
      /// \brief Demande à vérifier l'information sur le signal
      ///
      void CheckInfoSignal(void);

      ///
      /// \fn WriteInfoSignal
      /// \brief Ecrit l'information du signal dans le fichier log
      /// \param message : information récupérée à partie de la fenetre du receiver
      ///
      void WriteInfoSignal(const QString &message);

      ///
      /// \fn WriteSignalSize
      /// \brief Ecrit l'information sur la taille du signal dans le fichier log
      /// \param message : information récupérée à partie de la fenetre du receiver
      ///
      void WriteSignalSize(const QString &message);

      ///
      /// \fn ListInputFromConfigWindow
      /// \brief Permet de créer notre liste d'input à partir des données stockées dans la fenêtre de configuration
      ///
      void ListInputFromConfigWindow(void);

      ///
      /// \fn ListPlugFromConfigWindow
      /// \brief Permet de créer la liste de plug des inputs à partir des données stockées dans la fenêtre de configuration
      ///
      void ListPlugFromConfigWindow(void);

      /*--------------------------- Signaux publiques -------------------------*/

   signals:

      ///
      /// \fn SIGNAL_CurrentInput
      /// \brief Permet de renvoyer l'input actuellement utilisé à la fenetre de configuration
      /// \param text : Input actuellement en cours de test
      ///
      void SIGNAL_CurrentInput(const QString &text);

      ///
      /// \fn SIGNAL_NextRateApplied
      /// \brief Indique à la fenetre source de passer au rate suivant
      ///
      void SIGNAL_NextRateApplied(void);

      ///
      /// \fn SIGNAL_NextFormatApplied
      /// \brief Indique à la fenetre source de passer au format suivant
      ///
      void SIGNAL_NextFormatApplied(void);

      ///
      /// \fn SIGNAL_NextSlotsCustom
      /// \brief Indique à la fenetre source de passer au slot suivant
      ///
      void SIGNAL_NextSlotCustom(void);

      ///
      /// \fn SIGNAL_AskInfoSignal
      /// \brief Demande l'envoie de la commande pour questionner la machine sur le signal
      ///
      void SIGNAL_AskInfoSignal(void);

      ///
      /// \fn SIGNAL_AskSignalSize
      /// \brief Demande l'envoie de la commande pour questionner la machine sur la taille du signal
      ///
      void SIGNAL_AskSignalSize(void);


      /*---------------------------- Slots publiques --------------------------*/

   public slots:

      /*----------------------------- Types privÃ©s ----------------------------*/

      /*--------------------------- MÃ©thodes privÃ©es --------------------------*/

   protected:

   private:

      /*------------------------------ Variables ------------------------------*/

   protected:

   public:

      QByteArray f_InputDecrypted;                        ///< Stocke la variable décrypter du l'input grace au fichier de config
      QByteArray f_PlugDecrypted;                         ///< Stocke la variable décrypter du plug grace au fichier de config
      QByteArray f_RealPlug;                              ///< Pour le cas du Vio4K qui pour lui la plupart des plug en entrée est gardé en mémoire selon leur position et non par leur identifiant de config
      QString f_InfoPlugSignal;                           ///< Récupère la chaine de caractère correspondant à l'information sur le signal
      QString f_InfoCurrentRate;                          ///< Récupère la chaine de caractère correspondant à l'information sur le rate actuel
      QString f_InfoCurrentFormat;                        ///< Récupère la chaine de caractère correspondant à l'information sur le format actuel

      QVector<QStringList> f_ListInputArray;              ///< Tableau contenant les listes d'input
      QVector<QStringList> f_ListPlugArray;               ///< Tableau contenant les listes de Plug
      SDWord f_SizeListInputAndPlugArray;                 ///< Pour récupérer la tailles des tableaux
      SDWord f_IndexArrayListInputAndPlug;                ///< Pour se déplacer dans les tableaux contenant les liste
      SDWord f_IndexListInputAndPlug;                     ///< Pour se déplacer dans les listes Input et Plug

      Bool f_FirstInput;                                  ///< Indique si c'est le premier input

      Byte f_IndicCommand;                                ///< Pour identifier quelle commande envoyer dans le GetSignalSize

      CLASS_CONFIG_RECEIVER* f_ConfigReceiverAccessed;    ///< Récupère le pointeur vers le configReceiver généré dans la fenêtre graphique du receiver
};

#endif // CLASS_TESTRECEIVER_H
