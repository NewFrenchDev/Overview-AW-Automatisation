///
/// \file GUI_ClassTestWindow.h
/// \brief Classe CLASS_TEST_WINDOW
/// \author ANALOG WAY - GLE
/// \date 2018-04-02
///

#ifndef CLASS_SOURCE_WINDOW_H
#define CLASS_SOURCE_WINDOW_H

// C++ Standard
// Librairies Qt
#include <QDateTime>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMdiSubWindow>
#include <QMutexLocker>
#include <QPushButton>
#include <QString>
#include <QTableWidget>
#include <QTimer>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QSettings>
// Modules
#include "COMMON_TypeDefs.h"
#include "VARSMANAGER_ClassVars.h"
#include "VARSMANAGER_ClassVarsTranscoder.h"
#include "COMM_ClassCommManager.h"
#include "global_windows_variables.h"
#include "Class_ConfigSource.h"
#include "Class_ThreadDeviceSource.h"
// Applicatif
#include "GUI_ClassAbstractSubwindow.h"

///
/// \class CLASS_TEST_WINDOW
/// \brief Classe CLASS_TEST_WINDOW
///
class CLASS_SOURCE_WINDOW : public CLASS_ABSTRACT_MDI_SUBWINDOW
{
      Q_OBJECT

   public:


      /*--------------------------- Types publiques ---------------------------*/

      /*------------------------- Constantes publiques ------------------------*/

      /*------------------------- Variables publiques -------------------------*/

      /*---------------------- Constructeur / Destructeur ---------------------*/

      ///
      /// \fn CLASS_TEST_WINDOW(QObject *parent)
      /// \brief Constructeur de CLASS_TEST_WINDOW
      ///
      /// \param[in] parent : widget parent
      /// \param[in] windowColor : couleur de la fenêtre
      ///
      explicit CLASS_SOURCE_WINDOW(const QColor &windowColor, QWidget *parent = nullptr);

      ///
      /// \fn ~CLASS_TEST_WINDOW()
      /// \brief Destructeur de CLASS_TEST_WINDOW
      ///
      ~CLASS_SOURCE_WINDOW();

      /*------------------------------ Accesseurs -----------------------------*/




      /*------------------------------ Mutateurs ------------------------------*/

      /*-------------------------- Méthodes publiques -------------------------*/

      /******************************** Partie Récupération info machine **************************************/

      ///
      /// \fn OnGetConfignButtonClicked
      /// \brief Slot enclenché lors de l'appui sur le bouton "Get Config"
      /// \param clicked : état du bouton
      ///
      void OnGetConfigButtonClicked(void);

      ///
      /// \fn AskFirmwareVersion
      /// \brief Questionne la machine sur la version de son firmware
      ///
      void AskFirmwareVersion(void);

      ///
      /// \fn AskOutputStatus
      /// \brief Questionne la machine sur le status de ses outputs
      ///
      void AskOutputStatus(void);

      ///
      /// \fn AskPlugStatus
      /// \brief Question la machine sur l'état des ses plugs
      ///
      void AskPlugStatus(void);

      ///
      /// \fn PrintOutputNumber
      /// \brief Affiche le nombre d'output sur la fenetre
      ///
      void PrintOutputNumber(void);

      /************************************** Fin Récupération info machine *********************************/

      ///
      /// \fn StartSource
      /// \brief Lance le test de la machine source
      ///
      void StartSource(void);

      ///
      /// \fn StopAll
      /// \brief Arrete les timers et réinitialise la variable de FirstLaunch de la class testSource
      ///
      void StopAll(void);

      ///
      /// \fn Stop
      /// \brief Arrete tout à la fin du test
      ///
      void Stop(void);

      ///
      /// \fn SendCommandFormat
      /// \brief Envoie la commande pour l'application du format sur la machine
      ///
      void SendCommandFormat(void);

      ///
      /// \fn SendCommandRate
      /// \brief Envoie la commande pour l'application du rate sur la machine
      ///
      void SendCommandRate(void);

      ///
      /// \fn AskStatusPlug
      /// \brief Questionne la machine sur le status des plugs
      ///
      void AskStatusPlug(void);

      ///
      /// \fn VerifyCompatibility
      /// \brief Vérifie la compatibilité du format et rate
      ///
      void VerifyCompatibility(void);

      ///
      /// \fn RebootData
      /// \brief Remet à zéro les données de la fenetre
      ///
      void RebootData(void);

      ///
      /// \fn ChangeSourceSetting
      /// \brief Permet de changer le fichier setting pris en compte par le programme lors du chargement d'un nouveau fichier de config pour l'appareil source
      /// \param newSetting : Nom du nouveau fichier de config de la machine source
      ///
      void ChangeSourceSetting(const QString &newSetting);

      ///
      /// \fn ClearMessageList
      /// \brief Remets à zéro f_MessageList
      ///
      void ClearMessageList(void);

      /*--------------------------- Signaux publiques -------------------------*/

   signals:

      ///
      /// \fn SIGNAL_SendRawData
      /// \brief Permet d'envoyé des données en brut vers la machine connectée
      /// \param dataToSend : données à envoyer à la machine
      /// \param appendTerm : true pour ajouter automatiquement la terminaison
      ///
      void SIGNAL_SendRawCommand(const QByteArray& dataToSend, Bool appendTerm = false);

      ///
      /// \brief SIGNAL_CreateReceiverFileLog
      ///
      void SIGNAL_CreateReceiverFileLog(void);

      ///
      /// \fn SIGNAL_ConfigSourceAccess
      /// \brief Permet de transmettre le pointeur sur la classe configSource créer par le thread assigné à la machine source
      /// \param configSource
      ///
      void SIGNAL_ConfigSourceAccess(CLASS_CONFIG_SOURCE* configSource);

      ///
      /// \fn SIGNAL_IdentifiedSource
      /// \brief Permet de récupérer le pointeur de la fenetre source et de l'envoyer à la fenêtre device
      /// \param sourceWindow
      ///
      void SIGNAL_IdentifiedSource(CLASS_SOURCE_WINDOW *sourceWindow);

      ///
      /// \fn SIGNAL_Start
      /// \brief Permet d'indiquer au thread en background de commencer.
      ///
      void SIGNAL_Start(void);

      ///
      /// \fn SIGNAL_Final
      /// \brief Permet d'indiquer la fin du test après avoir passer en revu tous les formats de toute les sorties à tous les rates
      ///
      void SIGNAL_Final(void);

      /*---------------------------- Slots publiques --------------------------*/

   public slots:

      ///
      /// \fn SLOT_NewDataAvailable
      /// \brief Slot qui affiche les données recues par l'universal port
      /// \param data : données diponible qui seront affichées
      /// \param portInfo : infomration sur le port de commication
      ///
      void SLOT_NewRxDataAvailable(const QByteArray& data);

      ///
      /// \fn SLOT_NewTxDataVailable
      /// \brief Slot enclenché pour ajouter des données passées en TX
      /// \param data : données acceptées par la communication à ajouter en TX
      ///
      void SLOT_NewTxDataVailable(const QByteArray& data);

      ///
      /// \fn SLOT_NewTxDataRefused
      /// \brief Slot enclenché pour ajouter des données refusées en TX
      /// \param data : données refusées par la communication à ajouter en TX
      ///
      void SLOT_NewTxDataRefused(const QByteArray& data);     

      /*----------------------------- Types privés ----------------------------*/

   private :

      // Type de message
      typedef enum _MESSAGE_TYPE
      {
         MESSAGE_RX,
         MESSAGE_TX,
         MESSAGE_TX_REFUSED,
         MESSAGE_RX_DISABLED,
         MESSAGE_RX_ENABLED,
         MESSAGE_TX_DISABLED,
         MESSAGE_TX_ENABLED,
      } T_MESSAGE_TYPE;

      // Structure regroupant le temps et le message
      typedef struct _MESSAGE_INFO
      {
         T_MESSAGE_TYPE Type;
         SQWord Timestamp;
         QString Data;
      } T_MESSAGE_INFO;


      // Type de fonction en cours d'utilisation
      typedef enum
      {
         NO_FUNCTION_ASSIGNED,
         ID,
         FIRMWARE,
         OUTPUT,
         PLUG,
         STATUS_PLUG
      } ENUM_CURRENT_FUNCTION;

      /*--------------------------- Méthodes privées --------------------------*/

      ///
      /// \fn SetUpUserInterface
      /// \brief Met en place l'interface graphique de la fenêtre de debug
      ///
      void SetUpUserInterface(void);

      ///
      /// \fn ClickOnHeaderItem
      /// \brief Clic sur le header de la table
      /// \param columnIndex : Index de la colonne
      ///
      void ClickOnHeaderItem(SDWord columnIndex);

      ///
      /// \fn OnClearButtonClicked
      /// \brief Slot enclenché lors de l'appui sur le bouton "Clear"
      /// \param clicked : état du bouton
      ///
      void OnClearButtonClicked(void);

      ///
      /// \fn OnSendButtonClicked
      /// \brief Slot enclenché lors de l'appui sur le bouton "Send"
      /// \param clicked : état du bouton
      ///
      void OnSendButtonClicked(void);

      ///
      /// \fn NextPLugToTestOrNextRate
      /// \brief Test le plug suivant sinon passe à la fonction suivante
      ///
      void NextPLugToTestOrNextRate(void);

      ///
      /// \fn SelectCustomSlot
      /// \brief Envoie la commande pour sélectionner le custom slot
      /// \param command : commande de selection de slot
      ///
      void SelectCustomSlot(const QByteArray &command);


      void ChangeToRateCustom(void);

      ///
      /// \fn RetardLauncherStatusPlug
      /// \brief Permet de retarder le lancement de la fonction en passant par un timer
      ///
      void RetardLauncherStatusPlugForCustom(void);

      ///
      /// \fn SendData
      /// \brief Envoie la donnée en raw
      /// \param data : donnée à envoyer à la machine
      ///
      void SendData(QByteArray data);

      ///
      /// \fn AskDeviceID
      /// \brief Demande à l'appareil de s'identifier
      ///
      void AskDeviceID(void);

      ///
      /// \fn AddNewMessageInfo
      /// \brief Construit la structure info du message
      /// \param type : Type de message
      /// \param data : Données du message
      /// \return  T_MESSAGE_INFO& : Infos du message
      ///
      void AddNewMessageInfo(T_MESSAGE_TYPE type, const QByteArray &data);

      ///
      /// \fn DisplayNewMessages
      /// \brief Affiche les nouveaux messages
      ///
      void DisplayNewMessages(void);

      ///
      /// \fn DecodeTimeWidget
      /// \brief Effectue le décodage d'une cellule "Time"
      /// \param timestamp : Timestamp contenu dans la cellule
      /// \return QString : Texte à afficher en infobulle
      ///
      QString DecodeTimeWidget(SQWord timestamp);


   private:

       /*------------------------------ Variables ------------------------------*/

   private:

      QVBoxLayout *f_WindowWidgetLayout;                 ///< Layout verticale principale de la fenêtre
      QHBoxLayout *f_TopButtonsLayout;                   ///< Layout acceuillant les boutons en haut de la fenêtre
      QHBoxLayout *f_SendDataLayout;                     ///< Layout horizontale qui contient les contrôles pour envoyé des commandes
      QVBoxLayout *f_ConfigLayout;                       ///< Layout verticale qui contient le bouton d'initialisation
      QHBoxLayout *f_TestLayout;                         ///< Layout horizontale qui contient les bouton start et stop
      QHBoxLayout *f_ReadStatusLayout;                   ///< Layout horizontale qui contient les afficheurs

      QTableWidget *f_TabDebugConsole;                   ///< Tableau qui sert de console

      QPushButton *f_ClearButton;                        ///< Bouton de remise à zéro
      QPushButton *f_SendButton;                         ///< Bouton pour envouer des commandes
      QPushButton *f_GetConfigButton;                    ///< Bouton pour récupérer les informations sur la machine

      QLabel *f_LabelName;                               ///< Label pour le nom de la machine
      QLabel *f_LabelNumberOutput;                       ///< Label pour le nombre de sortie

      QLineEdit *f_CommandLineEdit;                      ///< Line edit pour envoyer des commandes
      QTextEdit *f_ReadName;                             ///< Afficheur nom de l'appareil
      QTextEdit *f_ReadNumberOutput;                     ///< Afficheur nombre de sortie

      QVector<T_MESSAGE_INFO> f_MessageList;             ///< Liste des données reçues
      SDWord f_TreatedMessage;                           ///< Nombre de messages traités
      Bool f_FirstDataAdded;                             ///< Booléen permettant la configuration de la date courante
      QDateTime f_FirstDataAddedDateTime;                ///< Date à laquelle, on a reçu la première donnée
      QList<Bool> f_ColumnDisabled;                      ///< Statut de désactivation des colonnes

      QMutex f_DisplayMutex;                             ///< Mutex utilisé pour protéger l'accès aux messages et à leurs affichage

      SDWord f_IndexMessageList;                         ///< Indication sur l'index de la liste des données reçues

      QTimer *f_RefreshTimer;
      QTimer *f_RandomCustomFormatTimer;                 ///< Timer utilisé pour lancer la génération d'une liste de format custom aléatoire
      QTimer *f_RateTimer;                               ///< Timer utilisé pour laisser un délai avant l'envoi de la commande de rate
      QTimer *f_PlugTimer;                               ///< Timer utilisé pour laisser un délai avant l'envoi de la commande pour les plugs

      ENUM_CURRENT_FUNCTION f_IndicCurrentFonction;           ///< Indicateur sur la fonction en cours

      QString f_UntraitedMessage;

   public:

      CLASS_THREAD_DEVICE_SOURCE SourceThread;           ///< Thread créé pour exécuter des fonctions en arriere plan
      CLASS_CONFIG_SOURCE *f_ConfigSource;               ///< Pointeur vers la classe testSource créée par le thread
      CLASS_TEST_SOURCE *f_TestSource;                   ///< Pointeur vers la classe configSource créée par le thread

};

#endif // CLASS_SOURCE_WINDOW_H
