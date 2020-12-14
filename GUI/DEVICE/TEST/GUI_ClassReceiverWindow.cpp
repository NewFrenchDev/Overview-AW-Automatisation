///
/// \file GUI_ClassReceiverWindow.cpp
/// \brief
/// \author ANALOG WAY
/// \date 2018-04-05
///

// C++ Standard
// Librairies Qt
#include <QHeaderView>
#include <QIcon>
#include <QMutexLocker>
#include <QScrollBar>
#include <QSizePolicy>
#include <QTableWidget>
#include <QTimer>
#include <QSettings>
#include <QtMath>
#include <QtDebug>
#include <QMessageBox>
// Modules
#include "TRACES.h"
#include "COMDRIVER_ClassUniversalPort.h"
// Applicatif
#include "GUI_ClassReceiverWindow.h"


/******************************** DEFINITIONS ********************************/

// Nom de la fenêtre
#define WINDOW_NAME                    QStringLiteral("Receiver")

// Taille par défaut de la fenêtre
#define WINDOW_DEFAULT_WIDTH           400
#define WINDOW_DEFAULT_HEIGHT          600
// Taille minimale de la fenêtre
#define WINDOW_MIN_WIDTH               200
#define WINDOW_MIN_HEIGHT              400

// Textes des widgets boutons
#define BUTTON_SEND_TEXT               QStringLiteral("Send")
#define BUTTON_INITIALIZATION          QStringLiteral("Get Config")
#define BUTTON_START                   QStringLiteral("Start")
#define BUTTON_STOP                    QStringLiteral("Stop")
#define BUTTON_CLEAR_TEXT              QStringLiteral("Clear All")

// Tailles de widgets
#define TOP_BUTTON_HEIGHT              30
#define BUTTON_CLEAR_SIZE              QSize(60, TOP_BUTTON_HEIGHT)
#define TEXTEDIT_SEND_DATA_HEIGHT      30
#define BUTTON_SEND_SIZE               QSize(45, TEXTEDIT_SEND_DATA_HEIGHT)
#define BUTTON_COMMAND_SIZE            QSize(200, TEXTEDIT_SEND_DATA_HEIGHT)
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
#define MAX_TREAT_MESSAGES_PER_REFRESH 2000

// Chemin vers l'icône signifiant que la colonne est désactivée
#define DISABLE_ICON_PATH              QStringLiteral(":/PICTURES/led-rouge.png")

// Chemin vers l'icône de l'application
#define ICON_APP                       QStringLiteral(":/PICTURES/myappico.ico")

#define SETTING_VIO                    QStringLiteral("settings/settingVio.ini")
#define SETTING_LIVECORE               QStringLiteral("settings/settingLiveCore.ini")
#define SETTING_NLC                    QStringLiteral("settings/settingNLC.ini")
#define SETTING_MNG                    QStringLiteral("settings/settingMNG.ini")

// Dossier d'écriture des log
#define LOG_REPERTORY                  QStringLiteral("log/")
#define LOG_RECEIVER_DEVICE            QStringLiteral("ReceiverDevice")
#define LOG_CUSTOM_FORMAT              QStringLiteral("CUSTOM")
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
/// \fn CLASS_RECEIVER_WINDOW(QObject *parent)
/// \brief Constructeur de CLASS_RECEIVER_WINDOW
///
/// \param[in] parent : widget parent
/// \param[in] windowColor : couleur de la fenêtre
///
CLASS_RECEIVER_WINDOW::CLASS_RECEIVER_WINDOW(const QColor &windowColor, QWidget *parent) :
   CLASS_ABSTRACT_MDI_SUBWINDOW(windowColor, parent),
   f_WindowWidgetLayout(nullptr),
   f_TopButtonsLayout(nullptr),
   f_SendDataLayout(nullptr),
   f_ConfigLayout(nullptr),
   f_TestLayout(nullptr),
   f_ReadStatusLayout(nullptr),
   f_TabDebugConsole(nullptr),
   f_ClearButton(nullptr),
   f_SendButton(nullptr),
   f_GetConfigButton(nullptr),
   f_LabelName(nullptr),
   f_LabelNumberInput(nullptr),
   f_CommandLineEdit(nullptr),
   f_ReadName(nullptr),
   f_ReadNumberInput(nullptr),
   f_MessageList(),
   f_TreatedMessage(0),
   f_FirstDataAdded(false),
   f_FirstDataAddedDateTime(),
   f_ColumnDisabled(),
   f_DisplayMutex(),
   f_IndexMessageList(0),
   ReceiverThread(),
   f_ConfigReceiver(nullptr),
   f_TestReceiver(nullptr)
{    
   // On met en place l'interface graphique
   this->SetUpUserInterface();

   // Récupération des classe de configuration et de test du thread
   f_ConfigReceiver = ReceiverThread.GetConfigReceiver();
   f_TestReceiver = ReceiverThread.GetTestReceiver();

   //On récupère le temps à l'ouverture de l'interface device
   QString currentTime = QDateTime::currentDateTime().toString(QStringLiteral("dddd dd MMMM yyyy hh:mm:ss.zzz"));
   QString currentTime1 = currentTime.replace(QLatin1String(" "),QLatin1String("_"));
   f_ConfigReceiver->f_CurrentTime = currentTime1.replace(QLatin1String(":"),QLatin1String("."));

   // Timer utilisé pour rafraîchir l'affichage
   f_RefreshTimer = new QTimer(this);
   f_RefreshTimer->setInterval(100);
   f_RefreshTimer->setSingleShot(true);
   QObject::connect(f_RefreshTimer, &QTimer::timeout, this, &CLASS_RECEIVER_WINDOW::DisplayNewMessages);

   // On connecte le signal et le slot pour effacer le tableau
   QObject::connect(f_ClearButton, &QPushButton::clicked, this, &CLASS_RECEIVER_WINDOW::OnClearButtonClicked);

   // On connecte le signal pour envoyé des données en brut
   QObject::connect(f_SendButton, &QPushButton::clicked, this, &CLASS_RECEIVER_WINDOW::OnSendButtonClicked);
   QObject::connect(f_GetConfigButton, &QPushButton::clicked, this, &CLASS_RECEIVER_WINDOW::OnGetConfigButtonClicked);
   QObject::connect(f_CommandLineEdit, &QLineEdit::returnPressed, this, &CLASS_RECEIVER_WINDOW::OnSendButtonClicked);

   // Liaison de la fenetre avec le thread pour la config machine
   QObject::connect(f_ConfigReceiver, &CLASS_CONFIG_RECEIVER::SIGNAL_DeviceNameObtained, this, &CLASS_RECEIVER_WINDOW::AskFirmwareVersion, Qt::QueuedConnection);
   QObject::connect(f_ConfigReceiver, &CLASS_CONFIG_RECEIVER::SIGNAL_InputInformationObtained, this, &CLASS_RECEIVER_WINDOW::CheckInputStatus, Qt::QueuedConnection);
   QObject::connect(f_ConfigReceiver, &CLASS_CONFIG_RECEIVER::SIGNAL_PlugInformationObtained, this, &CLASS_RECEIVER_WINDOW::AskPlugStatus, Qt::QueuedConnection);
   QObject::connect(f_ConfigReceiver, &CLASS_CONFIG_RECEIVER::SIGNAL_InterfaceReceiverObtained, this, &CLASS_RECEIVER_WINDOW::PrintInputNumber, Qt::QueuedConnection);

   // Liaison de la fenetre avec le thread pour le test machine
   QObject::connect(this, &CLASS_RECEIVER_WINDOW::SIGNAL_AccessToConfigReceiver, f_TestReceiver, &CLASS_TEST_RECEIVER::AccessToConfigReceiverData, Qt::QueuedConnection);
   QObject::connect(f_TestReceiver, &CLASS_TEST_RECEIVER::SIGNAL_AskInfoSignal, this, &CLASS_RECEIVER_WINDOW::AskInfoSignal, Qt::QueuedConnection);
   QObject::connect(f_TestReceiver, &CLASS_TEST_RECEIVER::SIGNAL_AskSignalSize, this, &CLASS_RECEIVER_WINDOW::AskSignalSize, Qt::QueuedConnection);

   // On envoie un pointeur sur f_ConfigReceiver créé à la classe de f_TestReceiver pour donner l'accés à ces variables
   emit this->SIGNAL_AccessToConfigReceiver(f_ConfigReceiver);
}

///
/// \fn ~CLASS_RECEIVER_WINDOW()
/// \brief Destructeur de CLASS_RECEIVER_WINDOW
///
CLASS_RECEIVER_WINDOW::~CLASS_RECEIVER_WINDOW()
{
    // On déconnecte le signal et le slot pour effacer le tableau
    QObject::disconnect(f_ClearButton, &QPushButton::clicked, this, &CLASS_RECEIVER_WINDOW::OnClearButtonClicked);
    // On déconnecte le signal pour envoyé des données en brut
    QObject::disconnect(f_SendButton, &QPushButton::clicked, this, &CLASS_RECEIVER_WINDOW::OnSendButtonClicked);
    QObject::disconnect(f_GetConfigButton, &QPushButton::clicked, this, &CLASS_RECEIVER_WINDOW::OnGetConfigButtonClicked);
    QObject::disconnect(f_CommandLineEdit, &QLineEdit::returnPressed, this, &CLASS_RECEIVER_WINDOW::OnSendButtonClicked);

    this->deleteLater();
}

/******************************************************************************
 *                                 Accesseurs                                 *
 *****************************************************************************/

/******************************************************************************
 *                                  Mutateurs                                 *
 *****************************************************************************/

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
void CLASS_RECEIVER_WINDOW::SLOT_NewRxDataAvailable(const QByteArray& data)
{
   if(this->isVisible())
   {
       // Ajout des données dans la fenêtre
       this->AddNewMessageInfo(MESSAGE_RX, data);

       QString message;

       QByteArray::const_iterator itr = data.cbegin();
       QByteArray::const_iterator end = data.cend();

       while(itr != end)
       {
          message.append(*itr);
          ++itr;
       }

       // Lancement de la fonction adéquate du thread
       switch (f_IndicCurrentFonction)
       {
          default:
             break;
          case ID:
             f_IndicCurrentFonction = NO_FUNCTION_ASSIGNED;
             emit this->ReceiverThread.SIGNAL_DeviceName(message);
             break;
          case FIRMWARE :
             f_IndicCurrentFonction = NO_FUNCTION_ASSIGNED;
             emit this->ReceiverThread.SIGNAL_FirmwareVersion(message);
             break;
          case INPUT :
             f_IndicCurrentFonction = NO_FUNCTION_ASSIGNED;
             emit this->ReceiverThread.SIGNAL_NextInputStatus(message);
             break;
          case PLUG:
             f_IndicCurrentFonction = NO_FUNCTION_ASSIGNED;
             emit this->ReceiverThread.SIGNAL_NextPlugStatus(message);
             break;
          case INFO_SIGNAL :
             f_IndicCurrentFonction = NO_FUNCTION_ASSIGNED;
             this->AskCurrentRate();
             break;
          case CURRENT_RATE:
             f_IndicCurrentFonction = NO_FUNCTION_ASSIGNED;
             this->CalculRate(message);
             this->AskCurrentFormat();
             break;
          case CURRENT_FORMAT :
             f_IndicCurrentFonction = NO_FUNCTION_ASSIGNED;
             f_TestReceiver->WriteInfoSignal(message);
             break;
          case SIGNAL_WIDHT :
             f_IndicCurrentFonction = NO_FUNCTION_ASSIGNED;
             f_TestReceiver->WriteSignalSize(message);
             break;
          case SIGNAL_HEIGHT :
             f_IndicCurrentFonction = NO_FUNCTION_ASSIGNED;
             f_TestReceiver->WriteSignalSize(message);
             break;
          case FORMAT_WIDHT :
             f_IndicCurrentFonction = NO_FUNCTION_ASSIGNED;
             f_TestReceiver->WriteSignalSize(message);
             break;
          case FORMAT_HEIGHT :
             f_IndicCurrentFonction = NO_FUNCTION_ASSIGNED;
             f_TestReceiver->WriteSignalSize(message);
             break;
       }

       // Démarre le timer allant déclencher le rafraîchissement si besoin
       if (f_RefreshTimer->isActive() == false)
          f_RefreshTimer->start();
   }
}

///
/// \fn SLOT_NewTxDataVailable
/// \brief Slot enclenché pour ajouter des données passées en TX
/// \param data : données acceptées par la communication à ajouter en TX
///
void CLASS_RECEIVER_WINDOW::SLOT_NewTxDataVailable(const QByteArray& data)
{
   if(this->isVisible())
   {
       this->AddNewMessageInfo(MESSAGE_TX, data);

       // Démarre le timer allant déclencher le rafraîchissement si besoin
       if (f_RefreshTimer->isActive() == false)
          f_RefreshTimer->start();
   }
}

///
/// \fn SLOT_NewTxDataRefused
/// \brief Slot enclenché pour ajouter des données refusées en TX
/// \param data : données refusées par la communication à ajouter en TX
///
void CLASS_RECEIVER_WINDOW::SLOT_NewTxDataRefused(const QByteArray& data)
{
   if(this->isVisible())
   {
       this->AddNewMessageInfo(MESSAGE_TX_REFUSED, data);

       // Démarre le timer allant déclencher le rafraîchissement si besoin
       if (f_RefreshTimer->isActive() == false)
          f_RefreshTimer->start();
   }
}

/******************************************************************************
 *                              Méthodes privées                              *
 *****************************************************************************/

///
/// \fn SetUpUserInterface
/// \brief Met en place l'interface graphique de la fenêtre de debug
///
void CLASS_RECEIVER_WINDOW::SetUpUserInterface(void)
{
   // On change le nom de la fenêtre
   this->setWindowTitle(WINDOW_NAME);

   this->setWindowIcon(QIcon(ICON_APP));

   // Définit la taille idéale/minimale pour la fenêtre
   this->SetSizeHint(QSize(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT));
   this->setMinimumSize(QSize(WINDOW_MIN_WIDTH, WINDOW_MIN_HEIGHT));
   // Le fenêtre essaye de se mettre à la taille par défaut
   this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

   // Crée le layout allant acceuillir les widgets
   f_WindowWidgetLayout = new QVBoxLayout();
   f_WindowWidgetLayout->setContentsMargins(1, 1, 1, 1);
   f_WindowWidgetLayout->setSpacing(1);

   // Crée le layout des boutons en haut de la fenêtre
   f_TopButtonsLayout = new QHBoxLayout();
   f_ClearButton = new QPushButton(BUTTON_CLEAR_TEXT, this);
   f_ClearButton->setObjectName(QStringLiteral("CLASS_RECEIVER_WINDOW__") % QStringLiteral("ClearButton"));
   f_ClearButton->setFixedSize(BUTTON_CLEAR_SIZE);
   f_TopButtonsLayout->addWidget(f_ClearButton);

   // Associe ce layout à la fenêtre MDI via un QWidget intermédiaire
   this->setWidget(new QWidget());
   this->widget()->setLayout(f_WindowWidgetLayout);

   // On créer les widgets pour envoyer des commandes
   f_CommandLineEdit = new QLineEdit(this);
   f_CommandLineEdit->setObjectName(QStringLiteral("CLASS_RECEIVER_WINDOW__") % QStringLiteral("CommandLineEdit"));
   f_CommandLineEdit->setFixedHeight(TEXTEDIT_SEND_DATA_HEIGHT);

   f_SendButton = new QPushButton(BUTTON_SEND_TEXT, this);
   f_SendButton->setObjectName(QStringLiteral("CLASS_RECEIVER_WINDOW__") % QStringLiteral("SendButton"));
   f_SendButton->setFixedSize(BUTTON_SEND_SIZE);

   f_GetConfigButton = new QPushButton(BUTTON_INITIALIZATION, this);
   f_GetConfigButton->setObjectName(QStringLiteral("CLASS_RECEIVER_WINDOW__") % QStringLiteral("GetConfigButton"));
   f_GetConfigButton->setFixedSize(BUTTON_COMMAND_SIZE);

   f_ReadName = new QTextEdit(this);
   f_ReadName->setObjectName(QStringLiteral("CLASS_RECEIVER_WINDOW__") % QStringLiteral("ReadName"));
   f_ReadName->setFixedHeight(TEXTEDIT_SEND_DATA_HEIGHT);
   f_ReadName->setFixedWidth(140);
   f_ReadName->setReadOnly(true);
   f_ReadName->setText("?");

   f_ReadNumberInput = new QTextEdit(this);
   f_ReadNumberInput->setObjectName(QStringLiteral("CLASS_RECEIVER_WINDOW__") % QStringLiteral("NumberInput"));
   f_ReadNumberInput->setFixedHeight(TEXTEDIT_SEND_DATA_HEIGHT);
   f_ReadNumberInput->setFixedWidth(30);
   f_ReadNumberInput->setReadOnly(true);
   f_ReadNumberInput->setText("?");

   // Layout qui contiendra les widget pour l'envois de commandes
   f_SendDataLayout = new QHBoxLayout();
   f_SendDataLayout->addWidget(f_CommandLineEdit);
   f_SendDataLayout->addSpacing(SEND_EDIT_BUTTON_INTERSPACE);
   f_SendDataLayout->addWidget(f_SendButton);

   // Layout qui contient les widgets pour l'envois des formats
   f_ConfigLayout = new QVBoxLayout();
   f_ConfigLayout->setAlignment(Qt::AlignHCenter);
   f_ConfigLayout->addWidget(f_GetConfigButton);
   f_ConfigLayout->addSpacing(SEND_EDIT_BUTTON_INTERSPACE);

   // Layout qui contient les widgets de lecture du STATUS
   f_LabelName = new QLabel(f_ReadName);
   f_LabelName->setText(QStringLiteral(" Device name :"));
   f_LabelName->setFixedWidth(80);
   f_LabelNumberInput = new QLabel(f_ReadNumberInput);
   f_LabelNumberInput->setText(QStringLiteral(" Number Input :"));
   f_LabelNumberInput->setFixedWidth(80);
   f_ReadStatusLayout = new QHBoxLayout();
   f_ReadStatusLayout->addWidget(f_LabelName);
   f_ReadStatusLayout->addWidget(f_ReadName);
   f_ReadStatusLayout->addWidget(f_LabelNumberInput);
   f_ReadStatusLayout->addWidget(f_ReadNumberInput);

   // On paramètre le tableau qui sert de console
   f_TabDebugConsole = new QTableWidget();
   QObject::connect(f_TabDebugConsole->horizontalHeader(), &QHeaderView::sectionClicked, this, &CLASS_RECEIVER_WINDOW::ClickOnHeaderItem);
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
   f_WindowWidgetLayout->addSpacing(20);
   f_WindowWidgetLayout->addLayout(f_ReadStatusLayout);
   f_WindowWidgetLayout->addSpacing(20);
   f_WindowWidgetLayout->addLayout(f_ConfigLayout);
}

///
/// \fn ClickOnHeaderItem
/// \brief Clic sur le header de la table
/// \param columnIndex : Index de la colonne
///
void CLASS_RECEIVER_WINDOW::ClickOnHeaderItem(SDWord columnIndex)
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
   }
}

///
/// \fn OnClearButtonClicked
/// \brief Slot enclenché lors de l'appui sur le bouton "Clear"
/// \param clicked : état du bouton
///
void CLASS_RECEIVER_WINDOW::OnClearButtonClicked(void)
{
   // Protège l'accès à la liste
   QMutexLocker Locker(&f_DisplayMutex);

   // On efface tout le contenu de la table d'affichage et de la liste de commandes
   f_TabDebugConsole->clearContents();
   f_TabDebugConsole->setRowCount(0);
}

///
/// \fn OnSendButtonClicked
/// \brief Slot enclenché lors de l'appui sur le bouton "Send"
///
void CLASS_RECEIVER_WINDOW::OnSendButtonClicked(void)
{
   QString SendString(f_CommandLineEdit->text());

   if (SendString.isEmpty() == true)
      return;

   // On efface la ligne de saisie
   f_CommandLineEdit->clear();

   QByteArray SendData(SendString.toLatin1());

   emit this->SIGNAL_SendRawCommand(SendData, true);
}

/*************************************************************************************************************
 * *************************************   Récupération info machine     *************************************
 * **********************************************************************************************************/

///
/// \fn OnGetConfigButtonClicked
/// \brief Slot enclenché lors de l'appui sur le bouton "Get Config"
/// \param clicked : état du bouton
///
void CLASS_RECEIVER_WINDOW::OnGetConfigButtonClicked(void)
{ 
   f_IndicCurrentFonction = ID;

   // Réitialisation pour les sorties
   f_ConfigReceiver->f_LoopCounter = 0;
   f_ConfigReceiver->f_InputNumber = 0;
   f_ConfigReceiver->f_AvailableInput = 0;
   // Réinitilisation pour les plug
   f_ConfigReceiver->f_LoopPlugCounter = 0;
   f_ConfigReceiver->f_Index = 0;
   // Réinitialisation du tableau 2D
   f_ConfigReceiver->f_InputTab.clear();

   f_ReadNumberInput->setText(QStringLiteral("?"));

   emit this->SIGNAL_IdentifiedReceiver(this);

   // Lancement de l'identification de l'appareil
   this->AskDeviceID();

   return;
}


///
/// \fn AskFirmwareVersion
/// \brief Récupère l'information sur le firmware, le décode et propose un nom de fichier log
///
void CLASS_RECEIVER_WINDOW::AskFirmwareVersion(void)
{ 
   f_IndicCurrentFonction = FIRMWARE;

   // On demande la version du firmware
   if(f_ConfigReceiver->f_ConfigName == QLatin1String("Vio4K") || f_ConfigReceiver->f_ConfigName == QLatin1String("NLC"))
   {
      emit this->SIGNAL_SendRawCommand(QByteArrayLiteral("VEupd"), true);
   }
   else if(f_ConfigReceiver->f_ConfigName == QLatin1String("LiveCore"))
   {
      emit this->SIGNAL_SendRawCommand(QByteArrayLiteral("0,VEupd"), true);
   }

   f_ReadName->setText(f_ConfigReceiver->f_ConfigName);
}


///
/// \fn AskInputStatus
/// \brief Récupère les informations sur les entrées de la machine
///
void CLASS_RECEIVER_WINDOW::CheckInputStatus(void)
{  
   f_IndicCurrentFonction = INPUT;

   if(QVariant(f_ConfigReceiver->f_Input).toInt() < f_ConfigReceiver->f_Settings->value(QStringLiteral("INFO/MAX_INPUT")).toInt())
      emit this->SIGNAL_SendRawCommand(f_ConfigReceiver->f_Input % "," % f_ConfigReceiver->f_Settings->value(QStringLiteral("COMMAND/IN_AVAILABLE")).toByteArray(), true);
}


///
/// \fn AskPlugStatus
/// \brief Récupère les informations sur les plugs de la machine
///
void CLASS_RECEIVER_WINDOW::AskPlugStatus(void)
{ 
   f_IndicCurrentFonction = PLUG;

   if(QVariant(f_ConfigReceiver->f_Plug).toInt() < f_ConfigReceiver->f_Settings->value(QStringLiteral("INFO/MAX_INPUT_PLUG")).toInt())
   {
      if(f_ConfigReceiver->f_ConfigName == QLatin1String("LiveCore"))
         emit this->SIGNAL_SendRawCommand(f_ConfigReceiver->f_Input % "," % f_ConfigReceiver->f_Plug % "," % f_ConfigReceiver->f_Settings->value(QStringLiteral("COMMAND/IN_PLUG_AVAILABLE")).toByteArray(), true);
      else if(f_ConfigReceiver->f_ConfigName == QLatin1String("Vio4K"))
         emit this->SIGNAL_SendRawCommand(f_ConfigReceiver->f_Input % "," % f_ConfigReceiver->f_Plug % "," % f_ConfigReceiver->f_Settings->value(QStringLiteral("COMMAND/IN_PLUG_CONNECTOR_TYPE")).toByteArray(), true);
   }
}

///
/// \fn PrintInputNumber
/// \brief Affiche le nombre d'input
///
void CLASS_RECEIVER_WINDOW::PrintInputNumber(void)
{
   QByteArray InputDetected = QVariant(f_ConfigReceiver->f_AvailableInput).toByteArray();
   f_ReadNumberInput->setText(InputDetected);
}

/***********************************************************************************************************************
 * ****************************************     LAUNCHER        ********************************************************
 * ********************************************************************************************************************/

///
/// \fn CreateReceiverLogFile
/// \brief Créer le fichier de log de la machine réceptrice
///
void CLASS_RECEIVER_WINDOW::CreateReceiverLogFile(void)
{
   // Vérifie si le format custom est sélectionné et change le nom du fichier si c'est le cas
   if(ExtCustomFormatSelected == true)
     f_ConfigReceiver->f_FileName = LOG_REPERTORY % LOG_CUSTOM_FORMAT % "_" % LOG_RECEIVER_DEVICE % "_" % f_ConfigReceiver->f_DeviceName % "_" % f_ConfigReceiver->f_FirmwareVersion % "_" % f_ConfigReceiver->f_CurrentTime;
   else if(ExtCustomFormatSelected == false)
     f_ConfigReceiver->f_FileName = LOG_REPERTORY % LOG_RECEIVER_DEVICE % "_" % f_ConfigReceiver->f_DeviceName % "_" % f_ConfigReceiver->f_FirmwareVersion % "_" % f_ConfigReceiver->f_CurrentTime;

   // Création du fichier de log
   QByteArray Tag = QByteArrayLiteral("\n******************************* Device Receiver : ") % f_ConfigReceiver->f_DeviceName.toLatin1() % QByteArrayLiteral(" connected      Firmware version : ") % f_ConfigReceiver->f_FirmwareVersion.toLatin1() % QByteArrayLiteral(" *************************************\n") ;

   QFile fileValid(f_ConfigReceiver->f_FileName + QStringLiteral("_VALID.txt"));
   if(!fileValid.open(QIODevice::WriteOnly | QIODevice::Append))
      return;
   fileValid.write(Tag);
   fileValid.close();
}


///
/// \fn StartReceiver
/// \brief Lance la vérification des inputs de la machine
///
void CLASS_RECEIVER_WINDOW::StartReceiver(void)
{
   if(f_TestReceiver->f_ListInputArray.isEmpty())
   {
      f_TestReceiver->ListInputFromConfigWindow();

      if(f_TestReceiver->f_ListInputArray.isEmpty()){
         QMessageBox::information(nullptr, QStringLiteral("Configuration manquante"), QStringLiteral("Vous n'avez pas sélectionné les entrées de l'appareil récepteur !"), QMessageBox::Ok);
         return;
      }
   }

   if(f_TestReceiver->f_ListPlugArray.isEmpty())
   {
      f_TestReceiver->ListPlugFromConfigWindow();

      if(f_TestReceiver->f_ListPlugArray.isEmpty())
      {
         QMessageBox::information(nullptr, QStringLiteral("Configuration manquante"), QStringLiteral("Vous n'avez pas sélectionné les plugs d'entrée de l'appareil récepteur !"), QMessageBox::Ok);
         return;
      }
      // On a les meme nombre d'input et de de plug
      f_TestReceiver->f_SizeListInputAndPlugArray = f_TestReceiver->f_ListInputArray.size();
   }

   this->ClearMessageList();
   f_TestReceiver->CheckInfoSignal();
}

///
/// \fn AskInfoSignal
/// \brief Demande à la machine si le signal est valide sur l'input et plug renseignés
///
void CLASS_RECEIVER_WINDOW::AskInfoSignal(void)
{ 
   f_IndicCurrentFonction = INFO_SIGNAL;
   this->ClearMessageList();

   this->SendData(f_TestReceiver->f_InputDecrypted % "," % f_TestReceiver->f_PlugDecrypted % "," % f_ConfigReceiver->f_Settings->value(QStringLiteral("COMMAND/SIG_SCAN_VALID")).toByteArray());
}

///
/// \fn AskCurrentRate
/// \brief Récupère l'information sur le signal et questionne la machine sur le rate appliqué
///
void CLASS_RECEIVER_WINDOW::AskCurrentRate(void)
{
   f_IndicCurrentFonction = CURRENT_RATE;

   f_TestReceiver->f_InfoPlugSignal = f_MessageList.last().Data;
   f_TestReceiver->f_InfoPlugSignal.chop(2);

   this->SendData(f_TestReceiver->f_InputDecrypted % "," % f_TestReceiver->f_PlugDecrypted % "," % f_ConfigReceiver->f_Settings->value(QStringLiteral("COMMAND/SIG_FREQ_FIELD")).toByteArray());
}

///
/// \brief CLASS_RECEIVER_WINDOW::CalculRate
/// \param message
///
void CLASS_RECEIVER_WINDOW::CalculRate(QString &message)
{
   QString CurrentMessageRate;
   QString CurrentRate;

   CurrentMessageRate = message;
   CurrentMessageRate.chop(2);

   CurrentRate = CurrentMessageRate.section(',' , 2);

   int Hz = QVariant(CurrentRate).toInt();

   if(23950 < Hz && Hz < 23985)
      CurrentRate = QLatin1String("23970");
   else if(23985 < Hz && Hz < 24020)
      CurrentRate = QLatin1String("24000");
   else if(24900 < Hz && Hz < 25400)
      CurrentRate = QLatin1String("25000");
   else if(29950 < Hz && Hz < 29985)
      CurrentRate = QLatin1String("29970");
   else if(29985 < Hz && Hz < 30020)
      CurrentRate = QLatin1String("30000");
   else if(47920 < Hz && Hz < 47985)
      CurrentRate = QLatin1String("47950");
   else if(47985 < Hz && Hz < 48050)
      CurrentRate = QLatin1String("48000");
   else if(49800 < Hz && Hz < 50400)
      CurrentRate = QLatin1String("50000");
   else if(59900 < Hz && Hz < 59980)
      CurrentRate = QLatin1String("59940");
   else if(59980 < Hz && Hz < 60400)
      CurrentRate = QLatin1String("60000");
   else if(71950 < Hz && Hz < 72040)
      CurrentRate = QLatin1String("72000");
   else if(74960 < Hz && Hz < 75040)
      CurrentRate = QLatin1String("75000");
   else if(84950 < Hz && Hz < 85050)
      CurrentRate = QLatin1String("85000");
   else if(99800 < Hz && Hz < 100200)
      CurrentRate = QLatin1String("100000");
   else if(119800< Hz && Hz < 119930)
      CurrentRate = QLatin1String("119880");
   else if(119930 < Hz )
      CurrentRate = QLatin1String("120000");


   f_TestReceiver->f_InfoCurrentRate = QString::number(qRound(QVariant(CurrentRate).toDouble()/10));
   if(f_TestReceiver->f_InfoCurrentRate != QLatin1String("0"))
   {
      if(f_TestReceiver->f_InfoCurrentRate.size() == 4)
         f_TestReceiver->f_InfoCurrentRate.insert(2,QLatin1String("."));
      else if(f_TestReceiver->f_InfoCurrentRate.size() == 5)
         f_TestReceiver->f_InfoCurrentRate.insert(3,QLatin1String("."));
   }

   return;
}

///
/// \fn AskCurrentFormat
/// \brief Récupère l'information sur le rate et demande l'information sur le format actuel
///
void CLASS_RECEIVER_WINDOW::AskCurrentFormat(void)
{
   f_IndicCurrentFonction = CURRENT_FORMAT;

   this->SendData(f_TestReceiver->f_InputDecrypted % "," % f_TestReceiver->f_PlugDecrypted % "," % f_ConfigReceiver->f_Settings->value(QStringLiteral("COMMAND/SIG_DETECTED_FORMAT")).toByteArray());
}

///
/// \fn AskSignalSize
/// \brief Demande l'information sur la taille du signal
///
void CLASS_RECEIVER_WINDOW::AskSignalSize(void)
{
   QByteArray Command;

   switch (f_TestReceiver->f_IndicCommand)
   {
      default:
         break;
      case 0:
         f_IndicCurrentFonction = SIGNAL_WIDHT;
         Command = f_ConfigReceiver->f_Settings->value(QStringLiteral("COMMAND/SIG_SIGNAL_WIDHT")).toByteArray();
         break;
      case 1:
         f_IndicCurrentFonction = SIGNAL_HEIGHT;
         Command = f_ConfigReceiver->f_Settings->value(QStringLiteral("COMMAND/SIG_SIGNAL_HEIGHT")).toByteArray();
         break;
      case 2:
         f_IndicCurrentFonction = FORMAT_WIDHT;
         Command = f_ConfigReceiver->f_Settings->value(QStringLiteral("COMMAND/SIG_FORMAT_WIDHT")).toByteArray();
         break;
      case 3:
         f_IndicCurrentFonction = FORMAT_HEIGHT;
         Command = f_ConfigReceiver->f_Settings->value(QStringLiteral("COMMAND/SIG_FORMAT_HEIGHT")).toByteArray();
         break;
   }
   emit this->SendData(f_TestReceiver->f_InputDecrypted % "," % f_TestReceiver->f_PlugDecrypted % "," % Command);
}

///
/// \fn ClearMessageList
/// \brief Remets à zéro f_MessageList
///
void CLASS_RECEIVER_WINDOW::ClearMessageList(void)
{
   QMutableVectorIterator<T_MESSAGE_INFO> itr(f_MessageList);
   while(itr.hasNext())
   {
      itr.next();
      itr.remove();
   }

   f_MessageList.clear();
   f_MessageList.squeeze();
   f_TabDebugConsole->clearContents();
   f_TabDebugConsole->setRowCount(0);
   f_TreatedMessage = 0;
}

///
/// \fn SendData
/// \brief Envoie la donnée en raw
/// \param data : donnée a envoyée à la machine
///
void CLASS_RECEIVER_WINDOW::SendData(QByteArray data)
{
   QByteArray SendData(data);
   emit this->SIGNAL_SendRawCommand(SendData, true);
}

///
/// \fn AskDeviceID
/// \brief Demande à l'appareil de s'identifier
///
void CLASS_RECEIVER_WINDOW::AskDeviceID(void)
{
   QByteArray SendData(QByteArrayLiteral("?"));
   emit this->SIGNAL_SendRawCommand(SendData, true);
}

///
/// \fn RebootData
/// \brief Permet de réinitialiser les données de la fenetre
///
void CLASS_RECEIVER_WINDOW::RebootData(void)
{
   f_TestReceiver->f_ListInputArray.clear();
   f_TestReceiver->f_ListPlugArray.clear();
   f_TestReceiver->f_IndexArrayListInputAndPlug = 0;
   f_TestReceiver->f_IndexListInputAndPlug = 0;
   f_TestReceiver->f_FirstInput = true;
   f_TestReceiver->f_IndicCommand = 0;
   this->ClearMessageList();

   //On récupère le temps à la remise à zéro
   QString currentTime = QDateTime::currentDateTime().toString(QStringLiteral("dddd dd MMMM yyyy hh:mm:ss.zzz"));
   QString currentTime1 = currentTime.replace(QLatin1String(" "),QLatin1String("_"));
   f_ConfigReceiver->f_CurrentTime = currentTime1.replace(QLatin1String(":"),QLatin1String("."));

   //Changement du Nom de fichier pour en créer un nouveau au reboot
   if(ExtCustomFormatSelected == false)
      f_ConfigReceiver->f_FileName = LOG_REPERTORY % LOG_RECEIVER_DEVICE % QStringLiteral("_") % f_ConfigReceiver->f_DeviceName % QStringLiteral("_") % f_ConfigReceiver->f_FirmwareVersion % QStringLiteral("_") % f_ConfigReceiver->f_CurrentTime;
   else if(ExtCustomFormatSelected == true)
      f_ConfigReceiver->f_FileName = LOG_REPERTORY % LOG_CUSTOM_FORMAT % QStringLiteral("_") % LOG_RECEIVER_DEVICE % QStringLiteral("_") % f_ConfigReceiver->f_DeviceName % QStringLiteral("_") % f_ConfigReceiver->f_FirmwareVersion % QStringLiteral("_") % f_ConfigReceiver->f_CurrentTime;
}

///
/// \fn WriteOutputInfo
/// \brief Permet dindiquer dans le fichier log du receveur l'output et format correspond à l'appareil source
///
void CLASS_RECEIVER_WINDOW::WriteOutputInfo(void)
{
   QByteArray Tag = "\n\n\n\n"  % QVariant(QDateTime::currentDateTime().toString("hh:mm:ss.zzz")).toByteArray() %
         " ******************************* Output : "  % ExtOutputName %
         "   Format : " % ExtOutputFormat % " *************************************\n" ;

   QFile fileValid(f_ConfigReceiver->f_FileName + "_VALID.txt");
   if(!fileValid.open(QIODevice::WriteOnly | QIODevice::Append))
      return;
   fileValid.write(Tag);
   fileValid.close();
}

///
/// \fn NextInputsOutputPaired
/// \brief Permet de passer à l'ensemble d'input suivant grace au signal envoyé par la fenetre source
///
void CLASS_RECEIVER_WINDOW::NextInputApplied(void)
{
   f_TestReceiver->f_IndexArrayListInputAndPlug += 1;
}

///
/// \fn GetSettingSource
/// \brief Permet de récupérer le fichier de config de l'appareil source choisit par l'utilisateur
/// \param newSettingSource : nouveau fichier de config pour la machine source
///
void CLASS_RECEIVER_WINDOW::ChangeSettingSource(const QString& newSettingSource)
{
   if(f_ConfigReceiver->f_SettingSource != nullptr)
   {
      delete f_ConfigReceiver->f_SettingSource;
      f_ConfigReceiver->f_SettingSource = nullptr;
   }
   f_ConfigReceiver->f_SettingSource = new QSettings (newSettingSource , QSettings::IniFormat);
}

///
/// \fn AddNewMessageInfo
/// \brief Construit la structure info du message
/// \param type : Type de message
/// \param data : Données du message
/// \return  T_MESSAGE_INFO& : Infos du message
///
void CLASS_RECEIVER_WINDOW::AddNewMessageInfo(T_MESSAGE_TYPE type, const QByteArray& data)
{
   // Protège l'accès à la liste
   QMutexLocker Locker(&f_DisplayMutex);

   T_MESSAGE_INFO NewMessageInfo;

   NewMessageInfo.Type = type;

   QByteArray::const_iterator itr = data.cbegin();
   QByteArray::const_iterator end = data.cend();

   while(itr != end)
   {
      NewMessageInfo.Data.append(*itr);
      ++itr;
   }

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
}

///
/// \fn DisplayNewMessages
/// \brief Affiche les nouveaux messages
///
void CLASS_RECEIVER_WINDOW::DisplayNewMessages(void)
{
   // Protège l'accès à la liste
   QMutexLocker Locker(&f_DisplayMutex);

   SDWord CurrentListSize(f_MessageList.count());
   SDWord CurrentTreatedMessages(0);


   while (f_TreatedMessage < CurrentListSize)
   {
      const T_MESSAGE_INFO& CurrentMessage(f_MessageList.at(f_TreatedMessage++));

      // Pas d'affichage des messages si la colonne correspondante est désactivée
      if ((CurrentMessage.Type == CLASS_RECEIVER_WINDOW::MESSAGE_RX) && (f_ColumnDisabled.at(COLUMN_RX_INDEX) == true))
         continue;
      if (((CurrentMessage.Type == CLASS_RECEIVER_WINDOW::MESSAGE_TX) || ((CurrentMessage.Type == CLASS_RECEIVER_WINDOW::MESSAGE_TX_REFUSED))) && (f_ColumnDisabled.at(COLUMN_TX_INDEX) == true))
         continue;


      // Cré une nouvelle ligne vide
      const SDWord RowIndex(f_TabDebugConsole->rowCount());
      f_TabDebugConsole->insertRow(RowIndex);

      // Définit le temps
      f_TabDebugConsole->setItem(RowIndex, COLUMN_TIME_INDEX, new QTableWidgetItem(GetTimestampString(CurrentMessage.Timestamp)));

      switch (CurrentMessage.Type)
      {
         default :
         case CLASS_RECEIVER_WINDOW::MESSAGE_RX :
         case CLASS_RECEIVER_WINDOW::MESSAGE_RX_DISABLED :
         case CLASS_RECEIVER_WINDOW::MESSAGE_RX_ENABLED :
            f_TabDebugConsole->setItem(RowIndex, COLUMN_RX_INDEX, new QTableWidgetItem(CurrentMessage.Data));
            if (f_ColumnDisabled.at(COLUMN_TX_INDEX) == true)
            {
               f_TabDebugConsole->setItem(RowIndex, COLUMN_TX_INDEX, new QTableWidgetItem());
               f_TabDebugConsole->item(RowIndex, COLUMN_TX_INDEX)->setBackground(Qt::gray);
            }
            break;

         case CLASS_RECEIVER_WINDOW::MESSAGE_TX :
         case CLASS_RECEIVER_WINDOW::MESSAGE_TX_REFUSED :
         case CLASS_RECEIVER_WINDOW::MESSAGE_TX_DISABLED :
         case CLASS_RECEIVER_WINDOW::MESSAGE_TX_ENABLED :
            f_TabDebugConsole->setItem(RowIndex, COLUMN_TX_INDEX, new QTableWidgetItem(CurrentMessage.Data));
            if (f_ColumnDisabled.at(COLUMN_RX_INDEX) == true)
            {
               f_TabDebugConsole->setItem(RowIndex, COLUMN_RX_INDEX, new QTableWidgetItem());
               f_TabDebugConsole->item(RowIndex, COLUMN_RX_INDEX)->setBackground(Qt::gray);
            }
            break;
      }

      // Si on a traité trop de messages, on force une sortie
      if (++CurrentTreatedMessages > MAX_TREAT_MESSAGES_PER_REFRESH)
         break;
   }

   // On redimensionne les lignes par rapport à leur contenu
   f_TabDebugConsole->resizeRowsToContents();

   // On visualise toujours la dernière ligne
   if ((f_TabDebugConsole->verticalScrollBar()->value() >= f_TabDebugConsole->verticalScrollBar()->maximum()) == true)
      f_TabDebugConsole->scrollToBottom();

   if(f_MessageList.count() > 300)
       this->ClearMessageList();
}

///
/// \fn DecodeTimeWidgetText
/// \brief Effectue le décodage d'une cellule "Time"
/// \param cellText : Texte de la cellule
/// \return QString : Texte à afficher en infobulle
///
QString CLASS_RECEIVER_WINDOW::DecodeTimeWidget(SQWord timestamp)
{
   return QString::number(timestamp);
}
