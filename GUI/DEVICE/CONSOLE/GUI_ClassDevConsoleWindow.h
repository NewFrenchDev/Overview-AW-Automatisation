///
/// \file GUI_ClassDevConsoleWindow.h
/// \brief Classe CLASS_DEV_CONSOLE_WINDOW
/// \author ANALOG WAY - ADE
/// \date 2017-12-28
///

#ifndef CLASS_DEV_CONSOLE_WINDOW_H
#define CLASS_DEV_CONSOLE_WINDOW_H

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
// Modules
#include "COMMON_TypeDefs.h"
#include "VARSMANAGER_ClassVars.h"
#include "VARSMANAGER_ClassVarsTranscoder.h"
// Applicatif
#include "GUI_ClassAbstractSubwindow.h"

///
/// \class CLASS_DEV_CONSOLE_WINDOW
/// \brief Classe CLASS_DEV_CONSOLE_WINDOW
///
class CLASS_DEV_CONSOLE_WINDOW : public CLASS_ABSTRACT_MDI_SUBWINDOW
{
      Q_OBJECT

   public:


      /*--------------------------- Types publiques ---------------------------*/

      /*------------------------- Constantes publiques ------------------------*/

      /*------------------------- Variables publiques -------------------------*/

      /*---------------------- Constructeur / Destructeur ---------------------*/

      ///
      /// \fn CLASS_DEV_CONSOLE_WINDOW(QObject *parent)
      /// \brief Constructeur de CLASS_DEV_CONSOLE_WINDOW
      ///
      /// \param[in] parent : widget parent
      /// \param[in] windowColor : couleur de la fenêtre
      ///
      explicit CLASS_DEV_CONSOLE_WINDOW(const QColor &windowColor, QWidget *parent = nullptr);

      ///
      /// \fn ~CLASS_DEV_CONSOLE_WINDOW()
      /// \brief Destructeur de CLASS_DEV_CONSOLE_WINDOW
      ///
      ~CLASS_DEV_CONSOLE_WINDOW();

      /*------------------------------ Accesseurs -----------------------------*/

      /*------------------------------ Mutateurs ------------------------------*/

      ///
      /// \fn SetToolTipVarsTranscoder
      /// \brief Définit un nouveau transcoder pour le décodage des commandes variables
      /// \param device : Pointe vers le transcoder des commandes vue machine (nullptr si aucun)
      /// \param controller : Pointe vers le transcoder des commandes vue contrôleur (nullptr si aucun)
      ///
      //void SetToolTipVarsTranscoder(CLASS_VARS_TRANSCODER *device, CLASS_VARS_TRANSCODER *controller);

      /*-------------------------- Méthodes publiques -------------------------*/

      /*--------------------------- Signaux publiques -------------------------*/

   signals:

      ///
      /// \fn SIGNAL_SendRawData
      /// \brief Permet d'envoyé des données en brut vers la machine connectée
      /// \param dataToSend : données à envoyer à la machine
      /// \param appendTerm : true pour ajouter automatiquement la terminaison
      ///
      void SIGNAL_SendRawCommand(const QByteArray& dataToSend, Bool appendTerm = false);

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
      ///
      void OnSendButtonClicked(void);

      ///
      /// \fn AddNewMessageInfo
      /// \brief Construit la structure info du message
      /// \param type : Type de message
      /// \param data : Données du message
      /// \return  T_MESSAGE_INFO& : Infos du message
      ///
      const CLASS_DEV_CONSOLE_WINDOW::T_MESSAGE_INFO& AddNewMessageInfo(T_MESSAGE_TYPE type, const QByteArray& data);

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

      ///
      /// \fn DecodeVarsWidgetText
      /// \brief Effectue le décodage d'une commande de variables
      /// \param cellText : Texte de la cellule
      /// \param isRx : true si commande reçue, false si commande transmise
      /// \return QString : Texte à afficher en infobulle
      ///
      //QString DecodeVarsWidgetText(const QString& cellText, Bool isRx);

   private:

       /*------------------------------ Variables ------------------------------*/

   private:

      QVBoxLayout *f_WindowWidgetLayout;                 ///< Layout verticale principale de la fenêtre

      QHBoxLayout *f_TopButtonsLayout;                   ///< Layout acceuillant les boutons en haut de la fenêtre
      QPushButton *f_ExportButton;                       ///< Bouton permettant de lancer un export des données
      QPushButton *f_FiltersButton;                      ///< Bouton permettant la configuration de filtres
      QPushButton *f_ClearButton;                        ///< Bouton qui permet d'effacer la console
      QHBoxLayout *f_SendDataLayout;                     ///< Layout horizontale qui contient les contrôles pour envoyé des commandes

      QLineEdit *f_CommandLineEdit;                      ///< Line edit pour envoyer des commandes
      QPushButton *f_SendButton;                         ///< Bouton pour envouer des commandes
      QTableWidget *f_TabDebugConsole;                   ///< Tableau qui sert de console

      QVector<T_MESSAGE_INFO> f_MessageList;             ///< Liste des données reçues
      SDWord f_TreatedMessage;                           ///< Nombre de messages traités
      Bool f_FirstDataAdded;                             ///< Booléen permettant la configuration de la date courante
      QDateTime f_FirstDataAddedDateTime;                ///< Date à laquelle, on a reçu la première donnée
      QList<Bool> f_ColumnDisabled;                      ///< Statut de désactivation des colonnes
      /*CLASS_VARS_TRANSCODER *f_TipDeviceTranscoder;      ///< Transcoder des commandes du point de vue machine
      CLASS_VARS_TRANSCODER *f_TipControllerTranscoder;  ///< Transcoder des commandes du point de vue contrôleur*/

      QTimer *f_RefreshTimer;                            ///< Timer utilisé pour le rafraîchissement de l'affichage
      QMutex f_DisplayMutex;                             ///< Mutex utilisé pour protéger l'accès aux messages et à leurs affichage
};

#endif // CLASS_DEV_CONSOLE_WINDOW_H
