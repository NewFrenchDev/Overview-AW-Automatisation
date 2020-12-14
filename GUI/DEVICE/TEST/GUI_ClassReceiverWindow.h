///
/// \file GUI_ClassReceiverWindow.h
/// \brief
/// \author ANALOG WAY
/// \date 2018-04-05
///

#ifndef GUI_CLASSRECEIVERWINDOW_H
#define GUI_CLASSRECEIVERWINDOW_H

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
#include "COMDRIVER_ClassUniversalPort.h"
#include "global_windows_variables.h"
// Applicatif
#include "GUI_ClassAbstractSubwindow.h"
#include "Class_ThreadDeviceReceiver.h"



///
/// \class CLASS_RECEIVER_WINDOW
/// \brief Classe CLASS_RECEIVER_WINDOW
///
class CLASS_RECEIVER_WINDOW : public CLASS_ABSTRACT_MDI_SUBWINDOW
{
      Q_OBJECT

   public:


      /*--------------------------- Types publiques ---------------------------*/

      /*------------------------- Constantes publiques ------------------------*/

      /*------------------------- Variables publiques -------------------------*/


      /*---------------------- Constructeur / Destructeur ---------------------*/

      ///
      /// \fn CLASS_RECEIVER_WINDOW(QObject *parent)
      /// \brief Constructeur de CLASS_RECEIVER_WINDOW
      ///
      /// \param[in] parent : widget parent
      /// \param[in] windowColor : couleur de la fenêtre
      ///
      explicit CLASS_RECEIVER_WINDOW(const QColor &windowColor, QWidget *parent = nullptr);

      ///
      /// \fn ~CLASS_RECEIVER_WINDOW()
      /// \brief Destructeur de CLASS_RECEIVER_WINDOW
      ///
      ~CLASS_RECEIVER_WINDOW();

      /*------------------------------ Accesseurs -----------------------------*/




      /*------------------------------ Mutateurs ------------------------------*/

      /*-------------------------- Méthodes publiques -------------------------*/

      ///
      /// \fn CreateReceiverLogFile
      /// \brief Créer le fichier de log de la machine réceptrice
      ///
      void CreateReceiverLogFile(void);

      ///
      /// \fn StartReceiver
      /// \brief Lance la vérification des inputs de la machine
      ///
      void StartReceiver(void);

      ///
      /// \fn RebootData
      /// \brief Permet de réinitialiser les données de la fenetre
      ///
      void RebootData(void);

      ///
      /// \fn WriteOutputInfo
      /// \brief Permet dindiquer dans le fichier log du receveur l'output et format correspond à l'appareil source
      ///
      void WriteOutputInfo(void);

      ///
      /// \fn NextInputsOutputPaired
      /// \brief Permet de passer à l'ensemble d'input suivant grace au signal envoyé par la fenetre source
      ///
      void NextInputApplied(void);

      ///
      /// \fn GetSettingSource
      /// \brief Permet de récupérer le fichier de config de l'appareil source choisit par l'utilisateur
      /// \param newSettingSource : nouveau fichier de config pour la machine source
      ///
      void ChangeSettingSource(const QString& newSettingSource);

      /*--------------------------- Signaux publiques -------------------------*/

   signals:

      ///
      /// \fn SIGNAL_SendRawData
      /// \brief Permet d'envoyer des données en brut vers la machine connectée
      /// \param dataToSend : données à envoyer à la machine
      /// \param appendTerm : true pour ajouter automatiquement la terminaison
      ///
      void SIGNAL_SendRawCommand(const QByteArray& dataToSend, Bool appendTerm = false);


      ///
      /// \fn SIGNAL_AccessToConfigReceiver
      /// \brief Permet de transmettre le pointeur de la classe de configReceiver
      /// \param configReceiverToAccess : CLASS_CONFIG_RECEIVER créer dans la fenetre graphique de l'appareil recepteur
      ///
      void SIGNAL_AccessToConfigReceiver(CLASS_CONFIG_RECEIVER* configReceiverToAccess);


      ///
      /// \fn SIGNAL_IdentifiedReceiver
      /// \brief Permet de renvoyer le pointeur de la fenetre receiver à la fenetre device
      /// \param receiverWindow : addresse de la fenetre
      ///
      void SIGNAL_IdentifiedReceiver(CLASS_RECEIVER_WINDOW *receiverWindow);


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


      // Indique la fonction en cours
      typedef enum
      {
         NO_FUNCTION_ASSIGNED,
         ID,
         FIRMWARE,
         INPUT,
         PLUG,
         INFO_SIGNAL,
         CURRENT_SIGNAL,
         CURRENT_RATE,
         CURRENT_FORMAT,
         SIGNAL_WIDHT,
         SIGNAL_HEIGHT,
         FORMAT_WIDHT,
         FORMAT_HEIGHT
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

      /******************************** Partie Récupération info machine **************************************/

      ///
      /// \fn OnGetConfigButtonClicked
      /// \brief Slot enclenché lors de l'appui sur le bouton "Get Config"
      /// \param clicked : état du bouton
      ///
      void OnGetConfigButtonClicked(void);

      ///
      /// \fn AskFirmwareVersion
      /// \brief Récupère l'information sur le firmware, le décode et propose un nom de fichier log
      ///
      void AskFirmwareVersion(void);

      ///
      /// \fn AskInputStatus
      /// \brief Récupère les informations sur les entrées de la machine
      ///
      void CheckInputStatus(void);

      ///
      /// \fn AskPlugStatus
      /// \brief Récupère les informations sur les plugs de la machine
      ///
      void AskPlugStatus(void);

      ///
      /// \fn PrintInputNumber
      /// \brief Affiche le nombre d'input
      ///
      void PrintInputNumber(void);

      /************************************** Fin récupération info machine *********************************/

      ///
      /// \fn AskInfoSignal
      /// \brief Demande à la machine si le signal est valide sur l'input et plug renseignés
      ///
      void AskInfoSignal(void);

      ///
      /// \fn AskCurrentRate
      /// \brief Récupère l'information sur le signal et questionne la machine sur le rate appliqué
      ///
      void AskCurrentRate(void);

      void CalculRate(QString &message);

      ///
      /// \fn AskCurrentFormat
      /// \brief Récupère l'information sur le rate et demande l'information sur le format actuel
      ///
      void AskCurrentFormat(void);

      ///
      /// \fn WriteInfoSignal
      /// \brief Ecrit l'information dans le fichier log
      ///
      void WriteInfoSignal(void);

      ///
      /// \fn AskSignalSize
      /// \brief Demande l'information sur la taille du signal
      ///
      void AskSignalSize(void);


      ///
      /// \fn SendData
      /// \brief Envoie la donnée en raw
      /// \param data : donnée a envoyée à la machine
      ///
      void SendData(QByteArray data);

      ///
      /// \fn AskDeviceID
      /// \brief Demande à l'appareil de s'identifier
      ///
      void AskDeviceID(void);

      ///
      /// \fn ClearMessageList
      /// \brief Remets à zéro f_MessageList
      ///
      void ClearMessageList(void);

      ///
      /// \fn AddNewMessageInfo
      /// \brief Construit la structure info du message
      /// \param type : Type de message
      /// \param data : Données du message
      /// \return  T_MESSAGE_INFO& : Infos du message
      ///
      void AddNewMessageInfo(T_MESSAGE_TYPE type, const QByteArray& data);

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
      QLabel *f_LabelNumberInput;                        ///< Label pour le nombre de sortie

      QLineEdit *f_CommandLineEdit;                      ///< Line edit pour envoyer des commandes
      QTextEdit *f_ReadName;                             ///< Afficheur nom de l'appareil
      QTextEdit *f_ReadNumberInput;                      ///< Afficheur nombre de sortie

      QVector<T_MESSAGE_INFO> f_MessageList;             ///< Liste des données reçues
      SDWord f_TreatedMessage;                           ///< Nombre de messages traités
      Bool f_FirstDataAdded;                             ///< Booléen permettant la configuration de la date courante
      QDateTime f_FirstDataAddedDateTime;                ///< Date à laquelle, on a reçu la première donnée
      QList<Bool> f_ColumnDisabled;                      ///< Statut de désactivation des colonnes

      QTimer *f_RefreshTimer;                             ///< Timer d'affichage

      QMutex f_DisplayMutex;                             ///< Mutex utilisé pour protéger l'accès aux messages et à leurs affichage

      SDWord f_IndexMessageList;                         ///< Indication sur l'index de la liste des données reçues

      ENUM_CURRENT_FUNCTION f_IndicCurrentFonction;           ///< Indicateur sur la fonction en cours

   public:

      CLASS_THREAD_DEVICE_RECEIVER ReceiverThread;       ///< Thread créé pour exécuter des fonctions en arriere plan
      CLASS_CONFIG_RECEIVER* f_ConfigReceiver;           ///< Pointeur vers la classe configReceiver créée par le thread
      CLASS_TEST_RECEIVER* f_TestReceiver;               ///< Pointeur vers la classe testReceiver créée par le thread

};

#endif // GUI_CLASSRECEIVERWINDOW_H
