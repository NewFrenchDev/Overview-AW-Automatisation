///
/// \file GUI_ClassDevConsoleWindow.cpp
/// \brief Classe CLASS_DEV_CONSOLE_WINDOW
/// \author ANALOG WAY - ADE
/// \date 2017-12-28
///
/// Fonctions de la classe CLASS_DEV_CONSOLE_WINDOW
///

// C++ Standard
// Librairies Qt
#include <QHeaderView>
#include <QIcon>
#include <QMutexLocker>
#include <QScrollBar>
#include <QSizePolicy>
#include <QTableWidget>
// Modules
#include "TRACES.h"
// Applicatif
#include "GUI_ClassDevConsoleWindow.h"

/******************************** DEFINITIONS ********************************/

// Nom de la fenêtre
#define WINDOW_NAME                    QStringLiteral("Console")

// Taille par défaut de la fenêtre
#define WINDOW_DEFAULT_WIDTH           400
#define WINDOW_DEFAULT_HEIGHT          600
// Taille minimale de la fenêtre
#define WINDOW_MIN_WIDTH               200
#define WINDOW_MIN_HEIGHT              400

// Textes des widgets boutons
#define BUTTON_EXPORT_TEXT             QStringLiteral("Export")
#define BUTTON_FILTERS_TEXT            QStringLiteral("Filters")
#define BUTTON_SEND_TEXT               QStringLiteral("Send")
#define BUTTON_CLEAR_TEXT              QStringLiteral("Clear All")

// Tailles de widgets
#define TOP_BUTTON_HEIGHT              30
#define BUTTON_EXPORT_SIZE             QSize(60, TOP_BUTTON_HEIGHT)
#define BUTTON_FILTERS_SIZE            QSize(60, TOP_BUTTON_HEIGHT)
#define BUTTON_CLEAR_SIZE              QSize(60, TOP_BUTTON_HEIGHT)
#define TEXTEDIT_SEND_DATA_HEIGHT      30
#define BUTTON_SEND_SIZE               QSize(45, TEXTEDIT_SEND_DATA_HEIGHT)
#define SEND_EDIT_BUTTON_INTERSPACE    10

// Colonnes de la table
#define COLUMN_COUNT                   3
#define COLUMN_TIME_TEXT               QStringLiteral("Time (ms)")
#define COLUMN_TIME_INDEX              0
#define COLUMN_TX_TEXT                 QStringLiteral("  TX  ")
#define COLUMN_TX_INDEX                1
#define COLUMN_RX_TEXT                 QStringLiteral("  RX  ")
#define COLUMN_RX_INDEX                2

// Nombre maximum de messages pouvant être traités pas boucle
#define MAX_TREAT_MESSAGES_PER_REFRESH 1000

// Chemin vers l'icône signifiant que la colonne est désactivée
#define DISABLE_ICON_PATH              QStringLiteral(":/PICTURES/led-rouge.png")

/*----------------------------- Types publiques -----------------------------*/
/*--------------------------- Constantes publiques --------------------------*/
/*--------------------------- Variables publiques ---------------------------*/
/*------------------------------- Types privés ------------------------------*/
/*----------------------- Prototypes fonctions privées ----------------------*/
static QString GetTimestampString(SQWord timestamp);

/*---------------------------- Constantes privées ---------------------------*/
/*---------------------------- Variables privées ----------------------------*/

/**************************** FIN DES DEFINITIONS ****************************/

/******************************************************************************
 *                             Fonctions privées                              *
 *****************************************************************************/

///
/// \fn GetTimestampString
/// \brief Conversion d'un timestamp en millisecondes vers la chaîne à afficher
/// \param timestamp : Timestamp en ms
/// \return QString : Chaîne à afficher
///
static QString GetTimestampString(SQWord timestamp)
{
   // Millisecondes
   QString TimeString(QString::number(timestamp % 1000).rightJustified(3, QChar('0')));
   timestamp /= 1000;
   // Secondes
   TimeString.prepend(QString::number(timestamp % 60).rightJustified(2, QChar('0')) % QStringLiteral("."));
   timestamp /= 60;
   if (timestamp <= 0) return TimeString;
   // Minutes
   TimeString.prepend(QString::number(timestamp % 60).rightJustified(2, QChar('0')) % QStringLiteral(":"));
   timestamp /= 60;
   if (timestamp <= 0) return TimeString;
   // Heures
   TimeString.prepend(QString::number(timestamp % 24).rightJustified(2, QChar('0')) % QStringLiteral(":"));
   timestamp /= 24;
   if (timestamp <= 0) return TimeString;
   // Jours
   TimeString.prepend(QString::number(timestamp) % QStringLiteral("j "));

   return TimeString;
}

/******************************************************************************
 *                         Constructeur / Destructeur                         *
 *****************************************************************************/

///
/// \fn CLASS_DEV_CONSOLE_WINDOW(QObject *parent)
/// \brief Constructeur de CLASS_DEV_CONSOLE_WINDOW
///
/// \param[in] parent : widget parent
/// \param[in] windowColor : couleur de la fenêtre
///
CLASS_DEV_CONSOLE_WINDOW::CLASS_DEV_CONSOLE_WINDOW(const QColor &windowColor, QWidget *parent) :
   CLASS_ABSTRACT_MDI_SUBWINDOW(windowColor, parent),
   f_WindowWidgetLayout(nullptr),
   f_TopButtonsLayout(nullptr),
   f_ExportButton(nullptr),
   f_FiltersButton(nullptr),
   f_ClearButton(nullptr),
   f_SendDataLayout(nullptr),
   f_CommandLineEdit(nullptr),
   f_SendButton(nullptr),
   f_TabDebugConsole(nullptr),
   f_MessageList(),
   f_TreatedMessage(0),
   f_FirstDataAdded(false),
   f_FirstDataAddedDateTime(),
   f_ColumnDisabled(),
   //f_TipDeviceTranscoder(nullptr),
   //f_TipControllerTranscoder(nullptr),
   f_RefreshTimer(nullptr),
   f_DisplayMutex()
{
   // On met en place l'interface graphique
   this->SetUpUserInterface();

   // On connecte le signal et le slot pour effacer le tableau
   QObject::connect(f_ClearButton, &QPushButton::clicked, this, &CLASS_DEV_CONSOLE_WINDOW::OnClearButtonClicked);

   // On connecte le signal pour envoyé des données en brut
   QObject::connect(f_SendButton, &QPushButton::clicked, this, &CLASS_DEV_CONSOLE_WINDOW::OnSendButtonClicked);
   QObject::connect(f_CommandLineEdit, &QLineEdit::returnPressed, this, &CLASS_DEV_CONSOLE_WINDOW::OnSendButtonClicked);
}

///
/// \fn ~CLASS_DEV_CONSOLE_WINDOW()
/// \brief Destructeur de CLASS_DEV_CONSOLE_WINDOW
///
CLASS_DEV_CONSOLE_WINDOW::~CLASS_DEV_CONSOLE_WINDOW()
{
}

/******************************************************************************
 *                                 Accesseurs                                 *
 *****************************************************************************/

/******************************************************************************
 *                                  Mutateurs                                 *
 *****************************************************************************/

///
/// \fn SetToolTipVarsTranscoder
/// \brief Définit un nouveau transcoder pour le décodage des commandes variables
/// \param device : Pointe vers le transcoder des commandes vue machine (nullptr si aucun)
/// \param controller : Pointe vers le transcoder des commandes vue contrôleur (nullptr si aucun)
///
/*void CLASS_DEV_CONSOLE_WINDOW::SetToolTipVarsTranscoder(CLASS_VARS_TRANSCODER *device, CLASS_VARS_TRANSCODER *controller)
{
   f_TipDeviceTranscoder = device;
   f_TipControllerTranscoder = controller;
}*/

/******************************************************************************
 *                             Méthodes publiques                             *
 *****************************************************************************/

/******************************************************************************
 *                               Slots publiques                              *
 *****************************************************************************/

///
/// \fn SLOT_NewDataAvailable
/// \brief Slot qui affiche les données recues par l'universal port
/// \param data : données diponible qui seront affichées
/// \param portInfo : infomration sur le port de commication
///
void CLASS_DEV_CONSOLE_WINDOW::SLOT_NewRxDataAvailable(const QByteArray& data)
{
   AddNewMessageInfo(MESSAGE_RX, data);

   //  Afficher les données
   this->DisplayNewMessages();
}

///
/// \fn SLOT_NewTxDataVailable
/// \brief Slot enclenché pour ajouter des données passées en TX
/// \param data : données acceptées par la communication à ajouter en TX
///
void CLASS_DEV_CONSOLE_WINDOW::SLOT_NewTxDataVailable(const QByteArray& data)
{
   AddNewMessageInfo(MESSAGE_TX, data);

   //  Afficher les données
   this->DisplayNewMessages();
}

///
/// \fn SLOT_NewTxDataRefused
/// \brief Slot enclenché pour ajouter des données refusées en TX
/// \param data : données refusées par la communication à ajouter en TX
///
void CLASS_DEV_CONSOLE_WINDOW::SLOT_NewTxDataRefused(const QByteArray& data)
{
   AddNewMessageInfo(MESSAGE_TX_REFUSED, data);

   //  Afficher les données
   this->DisplayNewMessages();
}

/******************************************************************************
 *                              Méthodes privées                              *
 *****************************************************************************/

///
/// \fn SetUpUserInterface
/// \brief Met en place l'interface graphique de la fenêtre de debug
///
void CLASS_DEV_CONSOLE_WINDOW::SetUpUserInterface(void)
{
   // On change le nom de la fenêtre
   this->setWindowTitle(WINDOW_NAME);

   // Définit la taille idéale/minimale pour la fenêtre
   this->SetSizeHint(QSize(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT));
   this->setMinimumSize(QSize(WINDOW_MIN_WIDTH, WINDOW_MIN_HEIGHT));
   // Le fenêtre essaye de se mettre à la taille par défaut
   this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

   // Cré le layout allant acceuillir les widgets
   f_WindowWidgetLayout = new QVBoxLayout();
   f_WindowWidgetLayout->setContentsMargins(1, 1, 1, 1);
   f_WindowWidgetLayout->setSpacing(1);

   // Cré le layout des boutons en haut de la fenêtre
   f_TopButtonsLayout = new QHBoxLayout();
   f_ExportButton = new QPushButton(BUTTON_EXPORT_TEXT, this);
   f_ExportButton->setObjectName(QStringLiteral("CLASS_DEV_CONSOLE_WINDOW__") % QStringLiteral("ExportButton"));
   f_ExportButton->setFixedSize(BUTTON_EXPORT_SIZE);
   f_FiltersButton = new QPushButton(BUTTON_FILTERS_TEXT, this);
   f_FiltersButton->setObjectName(QStringLiteral("CLASS_DEV_CONSOLE_WINDOW__") % QStringLiteral("FiltersButton"));
   f_FiltersButton->setFixedSize(BUTTON_FILTERS_SIZE);
   f_ClearButton = new QPushButton(BUTTON_CLEAR_TEXT, this);
   f_ClearButton->setObjectName(QStringLiteral("CLASS_DEV_CONSOLE_WINDOW__") % QStringLiteral("ClearButton"));
   f_ClearButton->setFixedSize(BUTTON_CLEAR_SIZE);
   f_TopButtonsLayout->addWidget(f_ExportButton);
   f_TopButtonsLayout->addSpacerItem(new QSpacerItem(0, TOP_BUTTON_HEIGHT, QSizePolicy::Expanding, QSizePolicy::Minimum));
   f_TopButtonsLayout->addWidget(f_FiltersButton);
   f_TopButtonsLayout->addSpacerItem(new QSpacerItem(0, TOP_BUTTON_HEIGHT, QSizePolicy::Expanding, QSizePolicy::Minimum));
   f_TopButtonsLayout->addWidget(f_ClearButton);

   // Associe ce layout à la fenêtre MDI via un QWidget intermédiaire
   this->setWidget(new QWidget(this));
   this->widget()->setLayout(f_WindowWidgetLayout);

   // On créer les widgets pour envoyer des commandes
   f_CommandLineEdit = new QLineEdit(this);
   f_CommandLineEdit->setObjectName(QStringLiteral("CLASS_DEV_CONSOLE_WINDOW__") % QStringLiteral("CommandLineEdit"));
   f_CommandLineEdit->setFixedHeight(TEXTEDIT_SEND_DATA_HEIGHT);
   f_SendButton = new QPushButton(BUTTON_SEND_TEXT, this);
   f_SendButton->setObjectName(QStringLiteral("CLASS_DEV_CONSOLE_WINDOW__") % QStringLiteral("SendButton"));
   f_SendButton->setFixedSize(BUTTON_SEND_SIZE);
   // Layout qui contiendra les widget pour l'envois de commandes
   f_SendDataLayout = new QHBoxLayout();
   f_SendDataLayout->addWidget(f_CommandLineEdit);
   f_SendDataLayout->addSpacing(SEND_EDIT_BUTTON_INTERSPACE);
   f_SendDataLayout->addWidget(f_SendButton);
   // On paramètre le tableau qui sert de console
   f_TabDebugConsole = new QTableWidget(this);
   QObject::connect(f_TabDebugConsole->horizontalHeader(), &QHeaderView::sectionClicked, this, &CLASS_DEV_CONSOLE_WINDOW::ClickOnHeaderItem);
   f_TabDebugConsole->setMouseTracking(true);
   f_TabDebugConsole->setColumnCount(COLUMN_COUNT);
   f_TabDebugConsole->setSortingEnabled(false);
   f_TabDebugConsole->setEditTriggers(QAbstractItemView::NoEditTriggers);
   f_TabDebugConsole->verticalHeader()->hide();
   f_TabDebugConsole->horizontalHeader()->setSelectionMode(QAbstractItemView::NoSelection);
   f_TabDebugConsole->setSelectionMode(QAbstractItemView::SingleSelection);
   // Colonne TIME
   f_TabDebugConsole->setHorizontalHeaderItem(COLUMN_TIME_INDEX, new QTableWidgetItem(COLUMN_TIME_TEXT));
   f_TabDebugConsole->horizontalHeader()->setSectionResizeMode(COLUMN_TIME_INDEX, QHeaderView::ResizeToContents);
   f_ColumnDisabled.append(false);
   // Colonne TX
   f_TabDebugConsole->setHorizontalHeaderItem(COLUMN_TX_INDEX, new QTableWidgetItem(COLUMN_TX_TEXT));
   f_TabDebugConsole->horizontalHeader()->setSectionResizeMode(COLUMN_TX_INDEX, QHeaderView::Stretch);
   f_ColumnDisabled.append(false);
   // Colonne RX
   f_TabDebugConsole->setHorizontalHeaderItem(COLUMN_RX_INDEX, new QTableWidgetItem(COLUMN_RX_TEXT));
   f_TabDebugConsole->horizontalHeader()->setSectionResizeMode(COLUMN_RX_INDEX, QHeaderView::Stretch);
   f_ColumnDisabled.append(false);

   // Layout de la fenêtre
   f_WindowWidgetLayout->addLayout(f_TopButtonsLayout);
   f_WindowWidgetLayout->addWidget(f_TabDebugConsole);
   f_WindowWidgetLayout->addLayout(f_SendDataLayout);
}

///
/// \fn ClickOnHeaderItem
/// \brief Clic sur le header de la table
/// \param columnIndex : Index de la colonne
///
void CLASS_DEV_CONSOLE_WINDOW::ClickOnHeaderItem(SDWord columnIndex)
{
   // Si c'est la colonne RX ou TX
   if ((columnIndex == COLUMN_RX_INDEX) || (columnIndex == COLUMN_TX_INDEX))
   {
      QTableWidgetItem *WidgetItem(f_TabDebugConsole->horizontalHeaderItem(columnIndex));

      // Si la colonne est active, on la désactive :
      // - Affiche une LED rouge
      // - Change la couleur de fond
      // - Rétrécie la colonne
      if (f_ColumnDisabled.at(columnIndex) == false)
      {
         f_ColumnDisabled[columnIndex] = true;

         // Signale à l'affichage la désactivation
         AddNewMessageInfo((columnIndex == COLUMN_RX_INDEX) ? MESSAGE_RX_DISABLED : MESSAGE_TX_DISABLED, QString(QStringLiteral("DISABLED")).toUtf8());

         // Met à jour le header
         WidgetItem->setIcon(QIcon(QPixmap(DISABLE_ICON_PATH)));
         WidgetItem->setBackground(Qt::gray);
         f_TabDebugConsole->horizontalHeader()->setSectionResizeMode(columnIndex, QHeaderView::ResizeToContents);
      }
      // Sinon on recré le header avec les paramètres par défaut
      else
      {
         f_ColumnDisabled[columnIndex] = false;

         // Signale à l'affichage la réactivation
         AddNewMessageInfo((columnIndex == COLUMN_RX_INDEX) ? MESSAGE_RX_ENABLED : MESSAGE_TX_ENABLED, QString(QStringLiteral("ENABLED")).toUtf8());

         // Recré un header
         f_TabDebugConsole->setHorizontalHeaderItem(columnIndex, new QTableWidgetItem(f_TabDebugConsole->horizontalHeaderItem(columnIndex)->text()));
         f_TabDebugConsole->horizontalHeader()->setSectionResizeMode(columnIndex, QHeaderView::Stretch);
      }

      // Démarre le timer allant déclencher le rafraîchissement si besoin
      if (f_RefreshTimer->isActive() == false)
         f_RefreshTimer->start();
   }
}

///
/// \fn OnClearButtonClicked
/// \brief Slot enclenché lors de l'appui sur le bouton "Clear"
/// \param clicked : état du bouton
///
void CLASS_DEV_CONSOLE_WINDOW::OnClearButtonClicked(void)
{
   // Protège l'accès à la liste
   QMutexLocker Locker(&f_DisplayMutex);

   // On efface tout le contenu de la table d'affichage et de la liste de commandes
   f_TabDebugConsole->clearContents();
   f_TabDebugConsole->setRowCount(0);
   f_MessageList.clear();
   f_TreatedMessage = 0;
}

///
/// \fn OnSendButtonClicked
/// \brief Slot enclenché lors de l'appui sur le bouton "Send"
///
void CLASS_DEV_CONSOLE_WINDOW::OnSendButtonClicked(void)
{
   QString SendString(f_CommandLineEdit->text());

   if (SendString.isEmpty() == true)
      return;

   // On efface la ligne de saisie
   f_CommandLineEdit->clear();

   QByteArray SendData(SendString.toLatin1());

   emit this->SIGNAL_SendRawCommand(SendData, true);
}

///
/// \fn AddNewMessageInfo
/// \brief Construit la structure info du message
/// \param type : Type de message
/// \param data : Données du message
/// \return  T_MESSAGE_INFO& : Infos du message
///
const CLASS_DEV_CONSOLE_WINDOW::T_MESSAGE_INFO& CLASS_DEV_CONSOLE_WINDOW::AddNewMessageInfo(T_MESSAGE_TYPE type, const QByteArray& data)
{
   // Protège l'accès à la liste
   QMutexLocker Locker(&f_DisplayMutex);

   T_MESSAGE_INFO NewMessageInfo;

   NewMessageInfo.Type = type;
   NewMessageInfo.Data = QString(data);

   // Selon si c'est le premier message reçu ou non
   if (f_FirstDataAdded == true)
   {
      NewMessageInfo.Timestamp = f_FirstDataAddedDateTime.msecsTo(QDateTime::currentDateTime());
   }
   else
   {
      f_FirstDataAddedDateTime = QDateTime::currentDateTime();
      f_FirstDataAdded = true;
      NewMessageInfo.Timestamp = 0;
   }

   f_MessageList.append(NewMessageInfo);
   return f_MessageList.last();
}

///
/// \fn DisplayNewMessages
/// \brief Affiche les nouveaux messages
///
void CLASS_DEV_CONSOLE_WINDOW::DisplayNewMessages(void)
{
   // Protège l'accès à la liste
   QMutexLocker Locker(&f_DisplayMutex);

   SDWord CurrentListSize(f_MessageList.count());
   SDWord CurrentTreatedMessages(0);

   // On repositionnera la scroll bar sur le dernier élément si c'était déjà le cas
   Bool ScrollBarAtBottom(f_TabDebugConsole->verticalScrollBar()->value() >= f_TabDebugConsole->verticalScrollBar()->maximum());

   while (f_TreatedMessage < CurrentListSize)
   {
      const T_MESSAGE_INFO& CurrentMessage(f_MessageList.at(f_TreatedMessage++));

      // Pas d'affichage des messages si la colonne correspondante est désactivée
      if ((CurrentMessage.Type == CLASS_DEV_CONSOLE_WINDOW::MESSAGE_RX) && (f_ColumnDisabled.at(COLUMN_RX_INDEX) == true))
         continue;
      if (((CurrentMessage.Type == CLASS_DEV_CONSOLE_WINDOW::MESSAGE_TX) || ((CurrentMessage.Type == CLASS_DEV_CONSOLE_WINDOW::MESSAGE_TX_REFUSED))) && (f_ColumnDisabled.at(COLUMN_TX_INDEX) == true))
         continue;

      // Cré une nouvelle ligne vide
      SDWord RowIndex(f_TabDebugConsole->rowCount());
      f_TabDebugConsole->insertRow(RowIndex);
      // Définit le temps
      QTableWidgetItem *TimeStampItem(new QTableWidgetItem(GetTimestampString(CurrentMessage.Timestamp)));
      TimeStampItem->setToolTip(DecodeTimeWidget(CurrentMessage.Timestamp));
      f_TabDebugConsole->setItem(RowIndex, COLUMN_TIME_INDEX, TimeStampItem);
      // Cré l'élément pour la donnée
      QTableWidgetItem *DataWidget(new QTableWidgetItem(CurrentMessage.Data));

      switch (CurrentMessage.Type)
      {
         default :
            break;

         case CLASS_DEV_CONSOLE_WINDOW::MESSAGE_TX_REFUSED :
         case CLASS_DEV_CONSOLE_WINDOW::MESSAGE_RX_DISABLED :
         case CLASS_DEV_CONSOLE_WINDOW::MESSAGE_TX_DISABLED :
            DataWidget->setForeground(Qt::red);
            break;

         case CLASS_DEV_CONSOLE_WINDOW::MESSAGE_RX_ENABLED :
         case CLASS_DEV_CONSOLE_WINDOW::MESSAGE_TX_ENABLED :
            DataWidget->setForeground(Qt::green);
            break;
      }

      switch (CurrentMessage.Type)
      {
         default :
            break;

         case CLASS_DEV_CONSOLE_WINDOW::MESSAGE_RX_DISABLED :
         case CLASS_DEV_CONSOLE_WINDOW::MESSAGE_RX_ENABLED :
         case CLASS_DEV_CONSOLE_WINDOW::MESSAGE_TX_DISABLED :
         case CLASS_DEV_CONSOLE_WINDOW::MESSAGE_TX_ENABLED :
            DataWidget->setBackground(Qt::gray);
            break;
      }

      switch (CurrentMessage.Type)
      {
         default :
         case CLASS_DEV_CONSOLE_WINDOW::MESSAGE_RX :
         case CLASS_DEV_CONSOLE_WINDOW::MESSAGE_RX_DISABLED :
         case CLASS_DEV_CONSOLE_WINDOW::MESSAGE_RX_ENABLED :
            //DataWidget->setToolTip(DecodeVarsWidgetText(CurrentMessage.Data, true));
            f_TabDebugConsole->setItem(RowIndex, COLUMN_RX_INDEX, DataWidget);
            if (f_ColumnDisabled.at(COLUMN_TX_INDEX) == true)
            {
               QTableWidgetItem *TxWidget(new QTableWidgetItem());
               TxWidget->setBackground(Qt::gray);
               f_TabDebugConsole->setItem(RowIndex, COLUMN_TX_INDEX, TxWidget);
            }
            break;

         case CLASS_DEV_CONSOLE_WINDOW::MESSAGE_TX :
         case CLASS_DEV_CONSOLE_WINDOW::MESSAGE_TX_REFUSED :
         case CLASS_DEV_CONSOLE_WINDOW::MESSAGE_TX_DISABLED :
         case CLASS_DEV_CONSOLE_WINDOW::MESSAGE_TX_ENABLED :
            //DataWidget->setToolTip(DecodeVarsWidgetText(CurrentMessage.Data, false));
            f_TabDebugConsole->setItem(RowIndex, COLUMN_TX_INDEX, DataWidget);
            if (f_ColumnDisabled.at(COLUMN_RX_INDEX) == true)
            {
               QTableWidgetItem *RxWidget(new QTableWidgetItem());
               RxWidget->setBackground(Qt::gray);
               f_TabDebugConsole->setItem(RowIndex, COLUMN_RX_INDEX, RxWidget);
            }
            break;
      }

      // Si on a traité trop de messages, on force une sortie
      if (++CurrentTreatedMessages > MAX_TREAT_MESSAGES_PER_REFRESH)
         break;
   }

   // On visualise toujours la dernière ligne
   if (ScrollBarAtBottom == true)
      f_TabDebugConsole->scrollToBottom();

   // Relance la fonction s'il reste des messages à traiter
   if (f_TreatedMessage < f_MessageList.count())
      this->DisplayNewMessages();
}

///
/// \fn DecodeTimeWidgetText
/// \brief Effectue le décodage d'une cellule "Time"
/// \param cellText : Texte de la cellule
/// \return QString : Texte à afficher en infobulle
///
QString CLASS_DEV_CONSOLE_WINDOW::DecodeTimeWidget(SQWord timestamp)
{
   return QString::number(timestamp);
}

///
/// \fn DecodeVarsWidgetText
/// \brief Effectue le décodage d'une commande de variables
/// \param cellText : Texte de la cellule
/// \param isRx : true si commande reçue, false si commande transmise
/// \return QString : Texte à afficher en infobulle
///
/*QString CLASS_DEV_CONSOLE_WINDOW::DecodeVarsWidgetText(const QString& cellText, Bool isRx)
{
   if (cellText.startsWith(QStringLiteral("E10")) == true) return QStringLiteral("Command Error");
   else if (cellText.startsWith(QStringLiteral("E11")) == true) return QStringLiteral("Dimension Value Error");
   else if (cellText.startsWith(QStringLiteral("E12")) == true) return QStringLiteral("Index Error");
   else if (cellText.startsWith(QStringLiteral("E13")) == true) return QStringLiteral("Command Value Error");

   CLASS_VARS *CommandVarDesc;
   QVariant CommandValue;
   SDWord LinearIndex;
   CLASS_VARS_TRANSCODER::T_EVT_TYPE CommandEvtType;

   // Une commande a destination de la machine
   if (isRx == false)
   {
      // On ne peut rien faire sans transcodeur vue machine
      if (f_TipDeviceTranscoder == nullptr)
         return QString();

      QString ErrorString;

      // Lance un décodage
      if (f_TipDeviceTranscoder->DecodeFromString(cellText, &CommandVarDesc, &CommandValue, &LinearIndex, ErrorString, &CommandEvtType) == false)
         return ErrorString.isEmpty() == false ? ErrorString : QStringLiteral("Failed to decode");
   }
   // Une commande en provenance de la machine
   else
   {
      // On ne peut rien faire sans transcodeur vue contrôleur
      if (f_TipControllerTranscoder == nullptr)
         return QString();

      QString ErrorString;

      // Lance un décodage
      if (f_TipControllerTranscoder->DecodeFromString(cellText, &CommandVarDesc, &CommandValue, &LinearIndex, ErrorString, &CommandEvtType) == false)
         return ErrorString.isEmpty() == false ? ErrorString : QStringLiteral("Failed to decode");
   }

   QString ToolTip;

   // on construit le texte illustrant la commande décodée
   switch (CommandEvtType)
   {
      default :
      case CLASS_VARS_TRANSCODER::T_EVT_TYPE::READ_ALL :
      case CLASS_VARS_TRANSCODER::T_EVT_TYPE::READ_ALL_IGNORE_DEFAULT :
         return (QStringLiteral("READ ALL: ") % CommandVarDesc->GetVarName());

      case CLASS_VARS_TRANSCODER::T_EVT_TYPE::READ_IDX :
         if (CommandVarDesc->GetIndex() != nullptr)ToolTip = QStringLiteral("READ INDEX: ") % CommandVarDesc->GetVarName() % QStringLiteral("\r\nINDEX: {");
         else return (QStringLiteral("READ: ") % CommandVarDesc->GetVarName());
         break;

      case CLASS_VARS_TRANSCODER::T_EVT_TYPE::WRITE_IDX :
         if (CommandVarDesc->GetIndex() != nullptr) ToolTip = QStringLiteral("WRITE INDEX: ") % CommandVarDesc->GetVarName() % QStringLiteral("\r\nINDEX: {");
         else ToolTip = QStringLiteral("WRITE: ") % CommandVarDesc->GetVarName() % QStringLiteral("\r\nVALUE: ");
         break;
   }

   // Si présence d'un index, on le transforme en string
   const CLASS_VARS_INDEX *CommandIndex(CommandVarDesc->GetIndex());
   if (CommandIndex != nullptr)
   {
      QList<SDWord> IndexValues;
      CommandIndex->GetIndexInList(LinearIndex, IndexValues);

      for (SDWord DimIndex = 0; DimIndex < IndexValues.count(); DimIndex++)
      {
         if (DimIndex > 0) ToolTip.append(QStringLiteral(", "));

         const CLASS_VARS_ENUM *CurrentDimEnum(CommandIndex->GetDim(DimIndex)->GetEnum());
         ToolTip.append((CurrentDimEnum == nullptr) ? QString::number(IndexValues.at(DimIndex)) : CurrentDimEnum->GetLiteralValue(IndexValues.at(DimIndex)));
      }

      ToolTip.append(QStringLiteral("}\r\nVALUE: "));
   }

   // Pour la valeur, cela dépend du type de la variable
   switch (CommandVarDesc->GetItemType())
   {
      default :
      case CLASS_VARS::T_TYPE::INVALID :
         return QStringLiteral("Failed to decode");

      case CLASS_VARS::T_TYPE::BOOL :
         ToolTip.append((CommandValue.toBool() == false) ? QStringLiteral("FALSE") : QStringLiteral("TRUE"));
         break;

      case CLASS_VARS::T_TYPE::UNSIGNED :
      case CLASS_VARS::T_TYPE::SIGNED :
         if (CommandVarDesc->GetEnum() == nullptr) ToolTip.append(QString::number(CommandValue.toLongLong()));
         else ToolTip.append(CommandVarDesc->GetEnum()->GetLiteralValue(CommandValue.toInt()));
         break;

      case CLASS_VARS::T_TYPE::FLOAT :
         ToolTip.append(QString::number(CommandValue.toDouble()));
         break;

      case CLASS_VARS::T_TYPE::STRING :
         ToolTip.append(CommandValue.toString());
         break;

      case CLASS_VARS::T_TYPE::BOOL_ARRAY :
         ToolTip.append(QStringLiteral("{"));

         // Si c'est un tableau de booléen classique
         if (CommandVarDesc->GetEnum() == nullptr)
         {
            const QBitArray ValueArray(CommandValue.toBitArray());

            for (SDWord BitIndex = 0; BitIndex < ValueArray.count(); BitIndex++)
            {
               ToolTip.append((ValueArray.at(BitIndex) == false) ? QStringLiteral("FALSE, ") : QStringLiteral("TRUE, "));

               if ((BitIndex != 0) && ((BitIndex + 1) < ValueArray.count()) && ((BitIndex % 10) == 0))
                  ToolTip.append(QStringLiteral("\r\n"));
            }

            ToolTip.chop(2);
         }
         else
         {
            const CLASS_VARS_ENUM *ValueEnum(CommandVarDesc->GetEnum());

            const QBitArray ValueArray(CommandValue.toBitArray());
            SDWord RaiseBitsCount(0);

            for (SDWord BitIndex = 0; BitIndex < ValueArray.count(); BitIndex++)
            {
               // On ne fait apparaître que les bits à true
               if (ValueArray.at(BitIndex) == true)
                  continue;

               ToolTip.append(ValueEnum->GetLiteralValue(BitIndex) % QStringLiteral(", "));

               if ((RaiseBitsCount != 0) && ((BitIndex + 1) < ValueArray.count()) && ((RaiseBitsCount % 4) == 0))
                  ToolTip.append(QStringLiteral("\r\n"));

               RaiseBitsCount++;
            }

            ToolTip.chop(2);
            ToolTip.append(QStringLiteral("}"));
         }
         ToolTip.append(QStringLiteral("}"));
         break;

      case CLASS_VARS::T_TYPE::BYTE_ARRAY :
         ToolTip.append(QStringLiteral("{"));

         {
            const QByteArray ValueArray(CommandValue.toByteArray());

            for (SDWord ByteIndex = 0; ByteIndex < ValueArray.count(); ByteIndex++)
            {
               ToolTip.append(QString::number(ValueArray.at(ByteIndex)) % QStringLiteral(", "));

               if ((ByteIndex != 0) && ((ByteIndex + 1) < ValueArray.count()) && ((ByteIndex % 10) == 0))
                  ToolTip.append(QStringLiteral("\r\n"));
            }
         }

         ToolTip.chop(2);
         ToolTip.append(QStringLiteral("}"));
         break;

      case CLASS_VARS::T_TYPE::VARIANT_ARRAY :
         ToolTip = QStringLiteral("Not supported");
         break;
   }

   return ToolTip;
}*/

/******************************************************************************
 *                             Operateur surcharge                            *
 *****************************************************************************/
