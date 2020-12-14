///
/// \file GUI_ClassTestWindow.cpp
/// \brief Classe CLASS_TEST_WINDOW
/// \author ANALOG WAY - GLE
/// \date 2018-04-02
///
/// Fonctions de la classe CLASS_TEST_WINDOW
///
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
#include <QFile>
#include <QtDebug>
#include <QMessageBox>
// Modules
#include "TRACES.h"
// Applicatif
#include "GUI_ClassSourceWindow.h"


/******************************** DEFINITIONS ********************************/

// Nom de la fenêtre
#define WINDOW_NAME                    QStringLiteral("Source")

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
#define MAX_TREAT_MESSAGES_PER_REFRESH 1000

// Chemin vers l'icône signifiant que la colonne est désactivée
#define DISABLE_ICON_PATH              QStringLiteral(":/PICTURES/led-rouge.png")

// Chemin vers l'icône de l'application
#define ICON_APP                       QStringLiteral(":/PICTURES/myappico.ico")

// Le fichier de config du Vio
#define SETTING_VIO                    QStringLiteral("settings/settingVio.ini")
#define SETTING_LIVECORE               QStringLiteral("settings/settingLiveCore.ini")
#define SETTING_NLC                    QStringLiteral("settings/settingNLC.ini")
#define SETTING_MNG                    QStringLiteral("settings/settingMNG.ini")

// Dossier d'écriture des log
#define LOG_REPERTORY                  QStringLiteral("log/")
#define LOG_SOURCE_DEVICE              QStringLiteral("SourceDevice")
#define LOG_CUSTOM_FORMAT              QStringLiteral("CUSTOM")

// Compatibilité format et rate
#define NOT_INFORMED                   0
#define INCOMPATIBLE                   1
#define COMPATIBLE                     2


/*----------------------------- Types publiques -----------------------------*/
/*--------------------------- Constantes publiques --------------------------*/
/*--------------------------- Variables publiques ---------------------------*/
QString ExtRateValue = QLatin1String("");
QByteArray ExtOutputFormat = "";
QByteArray ExtOutputName = "";
QByteArray ExtCustomRate = "";
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
/// \fn CLASS_TEST_WINDOW(QObject *parent)
/// \brief Constructeur de CLASS_TEST_WINDOW
///
/// \param[in] parent : widget parent
/// \param[in] windowColor : couleur de la fenêtre
///
CLASS_SOURCE_WINDOW::CLASS_SOURCE_WINDOW(const QColor &windowColor, QWidget *parent) :
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
   f_LabelNumberOutput(nullptr),
   f_CommandLineEdit(nullptr),
   f_ReadName(nullptr),
   f_ReadNumberOutput(nullptr),
   f_MessageList(),
   f_TreatedMessage(0),
   f_FirstDataAdded(false),
   f_FirstDataAddedDateTime(),
   f_ColumnDisabled(),
   f_DisplayMutex(),
   f_IndexMessageList(0),
   f_RandomCustomFormatTimer(nullptr),
   f_RateTimer(nullptr),
   f_PlugTimer(nullptr),
   f_IndicCurrentFonction(NO_FUNCTION_ASSIGNED),
   SourceThread(),
   f_ConfigSource(nullptr),
   f_TestSource(nullptr)
{
   // On met en place l'interface graphique
   this->SetUpUserInterface();

   // Recupération de la class de configuration et de test
   f_TestSource = SourceThread.GetTestSource();
   f_ConfigSource = SourceThread.GetConfigSource();

   f_MessageList.reserve(10000);

   //On récupère le temps à l'ouverture de l'interface device
   QString currentTime = QDateTime::currentDateTime().toString(QStringLiteral("dddd dd MMMM yyyy hh:mm:ss.zzz")) ;
   QString currentTime1 = currentTime.replace(QLatin1String(" "),QLatin1String("_"));
   f_ConfigSource->f_CurrentTime = currentTime1.replace(QLatin1String(":") , QLatin1String("."));

   // On connecte le timer avec la création d'un liste de custom format renouvelé toute les 10ms
   f_RandomCustomFormatTimer = new QTimer(this);
   f_RandomCustomFormatTimer->setInterval(30);
   f_RandomCustomFormatTimer->start();
   QObject::connect(f_RandomCustomFormatTimer, &QTimer::timeout, f_TestSource, &CLASS_TEST_SOURCE::GenerateCustomFormatList);

   // Timer utilisé pour rafraîchir l'affichage
   f_RefreshTimer = new QTimer(this);
   f_RefreshTimer->setInterval(100);
   f_RefreshTimer->setSingleShot(true);
   QObject::connect(f_RefreshTimer, &QTimer::timeout, this, &CLASS_SOURCE_WINDOW::DisplayNewMessages);

   // Timer pour les rate
   f_RateTimer = new QTimer(this);
   f_RateTimer->setSingleShot(true);
   QObject::connect(f_RateTimer, &QTimer::timeout, f_TestSource, &CLASS_TEST_SOURCE::SendRate);

   // Timer pour les rate
   f_PlugTimer = new QTimer(this);
   f_PlugTimer->setSingleShot(true);
   QObject::connect(f_PlugTimer, &QTimer::timeout, f_TestSource, &CLASS_TEST_SOURCE::CheckOutputPlugStatus);

   // On connecte le signal et le slot pour effacer le tableau
   QObject::connect(f_ClearButton, &QPushButton::clicked, this, &CLASS_SOURCE_WINDOW::OnClearButtonClicked);

   // On connecte le signal pour envoyé des données en brut
   QObject::connect(f_SendButton, &QPushButton::clicked, this, &CLASS_SOURCE_WINDOW::OnSendButtonClicked);
   QObject::connect(f_GetConfigButton, &QPushButton::clicked, this, &CLASS_SOURCE_WINDOW::OnGetConfigButtonClicked);
   QObject::connect(f_CommandLineEdit, &QLineEdit::returnPressed, this, &CLASS_SOURCE_WINDOW::OnSendButtonClicked);

   // Liaison de la fenetre avec le thread pour la config machine
   QObject::connect(f_ConfigSource, &CLASS_CONFIG_SOURCE::SIGNAL_DeviceNameObtained, this, &CLASS_SOURCE_WINDOW::AskFirmwareVersion, Qt::QueuedConnection);
   QObject::connect(f_ConfigSource, &CLASS_CONFIG_SOURCE::SIGNAL_OutputInformationObtained, this, &CLASS_SOURCE_WINDOW::AskOutputStatus, Qt::QueuedConnection);
   QObject::connect(f_ConfigSource, &CLASS_CONFIG_SOURCE::SIGNAL_PlugInformationObtained, this, &CLASS_SOURCE_WINDOW::AskPlugStatus, Qt::QueuedConnection);
   QObject::connect(f_ConfigSource, &CLASS_CONFIG_SOURCE::SIGNAL_InterfaceSourceObtained, this, &CLASS_SOURCE_WINDOW::PrintOutputNumber, Qt::QueuedConnection);

   // Liaison de la fenetre avec le thread pour le test machine
   QObject::connect(this, &CLASS_SOURCE_WINDOW::SIGNAL_ConfigSourceAccess, f_TestSource, &CLASS_TEST_SOURCE::AccessToConfigSourceData, Qt::QueuedConnection);
   QObject::connect(this, &CLASS_SOURCE_WINDOW::SIGNAL_Start, f_TestSource, &CLASS_TEST_SOURCE::SendFormat, Qt::QueuedConnection);
   QObject::connect(f_TestSource, &CLASS_TEST_SOURCE::SIGNAL_SendFormat, this, &CLASS_SOURCE_WINDOW::SendCommandFormat, Qt::QueuedConnection);
   QObject::connect(f_TestSource, &CLASS_TEST_SOURCE::SIGNAL_SendRate, this, &CLASS_SOURCE_WINDOW::SendCommandRate, Qt::QueuedConnection);
   QObject::connect(f_TestSource, &CLASS_TEST_SOURCE::SIGNAL_StatusPlug, this, &CLASS_SOURCE_WINDOW::AskStatusPlug, Qt::QueuedConnection);
   QObject::connect(f_TestSource, &CLASS_TEST_SOURCE::SIGNAL_NextPlugToTest, this, &CLASS_SOURCE_WINDOW::NextPLugToTestOrNextRate, Qt::QueuedConnection);

   // Liason de la fenetre avec le thread pour le test des custom
   QObject::connect(f_TestSource, &CLASS_TEST_SOURCE::SIGNAL_CustomSlotSelectionned, this, &CLASS_SOURCE_WINDOW::SelectCustomSlot, Qt::QueuedConnection);
   QObject::connect(f_TestSource, &CLASS_TEST_SOURCE::SIGNAL_ChangeToRateCustom, this, &CLASS_SOURCE_WINDOW::ChangeToRateCustom, Qt::QueuedConnection);
   QObject::connect(f_TestSource, &CLASS_TEST_SOURCE::SIGNAL_RetardLaunchStatusPlug, this, &CLASS_SOURCE_WINDOW::RetardLauncherStatusPlugForCustom, Qt::QueuedConnection);
   QObject::connect(f_TestSource, &CLASS_TEST_SOURCE::SIGNAL_Stop, this, &CLASS_SOURCE_WINDOW::Stop, Qt::QueuedConnection);

   // On envoie un pointeur sur f_ConfigSource créé à la classe de f_TestSource pour donner l'accés à ces variables
   emit this->SIGNAL_ConfigSourceAccess(f_ConfigSource);
}

///
/// \fn ~CLASS_TEST_WINDOW()
/// \brief Destructeur de CLASS_TEST_WINDOW
///
CLASS_SOURCE_WINDOW::~CLASS_SOURCE_WINDOW()
{
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
void CLASS_SOURCE_WINDOW::SLOT_NewRxDataAvailable(const QByteArray& data )
{ 
    if(this->isVisible())
    {
        // Ajout des données dans la fenêtre
        this->AddNewMessageInfo(T_MESSAGE_TYPE::MESSAGE_RX, data);

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
              emit this->SourceThread.SIGNAL_DeviceName(message);
              break;
           case FIRMWARE:
              f_IndicCurrentFonction = NO_FUNCTION_ASSIGNED;
              emit this->SourceThread.SIGNAL_FirmwareVersion(message);
              break;
           case OUTPUT:
              f_IndicCurrentFonction = NO_FUNCTION_ASSIGNED;
              emit this->SourceThread.SIGNAL_NextOutputStatus(message);
              break;
           case PLUG:
              f_IndicCurrentFonction = NO_FUNCTION_ASSIGNED;
              emit this->SourceThread.SIGNAL_NextPlugStatus(message);
              break;
           case STATUS_PLUG:
              f_IndicCurrentFonction = NO_FUNCTION_ASSIGNED;
              this->VerifyCompatibility();
              break;
        }

        //Cas pour les customs
        switch (f_TestSource->f_StepCustom)
        {
           default:
              break;
           case CLASS_TEST_SOURCE::RANDOM_RATE :
              emit this->SIGNAL_SendRawCommand(f_TestSource->SetRandomRate(), true);
              break;
           case CLASS_TEST_SOURCE::ACTIVE_H :
              emit this->SIGNAL_SendRawCommand(f_TestSource->SetActiveH(), true);
              break;
           case CLASS_TEST_SOURCE::FRONT_PORCH_H :
              emit this->SIGNAL_SendRawCommand(f_TestSource->SetFrontPorchH(), true);
              break;
           case CLASS_TEST_SOURCE::BACK_PORCH_H :
              emit this->SIGNAL_SendRawCommand(f_TestSource->SetBackPorchH(), true);
              break;
           case CLASS_TEST_SOURCE::SYNCHRO_H :
              emit this->SIGNAL_SendRawCommand(f_TestSource->SetSynchroH(), true);
              break;
           case CLASS_TEST_SOURCE::POLARITY_H :
              emit this->SIGNAL_SendRawCommand(f_TestSource->SetPolarityH(), true);
              break;
           case CLASS_TEST_SOURCE::ACTIVE_V :
              emit this->SIGNAL_SendRawCommand(f_TestSource->SetActiveV(), true);
              break;
           case CLASS_TEST_SOURCE::FRONT_PORCH_V :
              emit this->SIGNAL_SendRawCommand(f_TestSource->SetFrontPorchV(), true);
              break;
           case CLASS_TEST_SOURCE::BACK_PORCH_V :
              emit this->SIGNAL_SendRawCommand(f_TestSource->SetBackPorchV(), true);
              break;
           case CLASS_TEST_SOURCE::SYNCHRO_V :
              emit this->SIGNAL_SendRawCommand(f_TestSource->SetSynchroV(), true);
              break;
           case CLASS_TEST_SOURCE::POLARITY_V :
              emit this->SIGNAL_SendRawCommand(f_TestSource->SetPolarityV(), true);
              break;
           case CLASS_TEST_SOURCE::CHECK_CUSTOM :
              emit this->SIGNAL_SendRawCommand(f_TestSource->CheckCustomStatus(), true);
              break;
           case CLASS_TEST_SOURCE::SAVE_CUSTOM :
              emit this->SIGNAL_SendRawCommand(f_TestSource->SaveCustomFormat(), true);
              break;
           case CLASS_TEST_SOURCE::NEXT_CUSTOM :
              f_TestSource->NextCustom();
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
void CLASS_SOURCE_WINDOW::SLOT_NewTxDataVailable(const QByteArray& data)
{
    if(this->isVisible())
    {
        this->AddNewMessageInfo(T_MESSAGE_TYPE::MESSAGE_TX, data);

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
void CLASS_SOURCE_WINDOW::SLOT_NewTxDataRefused(const QByteArray& data)
{
    if(this->isVisible())
    {
        this->AddNewMessageInfo(T_MESSAGE_TYPE::MESSAGE_TX_REFUSED, data);

        // Démarre le timer allant déclencher le rafraîchissement si besoin
        if (f_RefreshTimer->isActive() == false && f_MessageList.count() > 10)
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
void CLASS_SOURCE_WINDOW::SetUpUserInterface(void)
{
   // On change le nom de la fenêtre
   this->setWindowTitle(WINDOW_NAME);

   // Icone de la fenetre
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
   f_ClearButton->setObjectName(QStringLiteral("CLASS_TEST_WINDOW__") % QStringLiteral("ClearButton"));
   f_ClearButton->setFixedSize(BUTTON_CLEAR_SIZE);
   f_TopButtonsLayout->addWidget(f_ClearButton);

   // Associe ce layout à la fenêtre MDI via un QWidget intermédiaire
   this->setWidget(new QWidget());
   this->widget()->setLayout(f_WindowWidgetLayout);

   // On créer les widgets pour envoyer des commandes
   f_CommandLineEdit = new QLineEdit(this);
   f_CommandLineEdit->setObjectName(QStringLiteral("CLASS_TEST_WINDOW__") % QStringLiteral("CommandLineEdit"));
   f_CommandLineEdit->setFixedHeight(TEXTEDIT_SEND_DATA_HEIGHT);

   f_SendButton = new QPushButton(BUTTON_SEND_TEXT, this);
   f_SendButton->setObjectName(QStringLiteral("CLASS_TEST_WINDOW__") % QStringLiteral("SendButton"));
   f_SendButton->setFixedSize(BUTTON_SEND_SIZE);

   f_GetConfigButton = new QPushButton(BUTTON_INITIALIZATION, this);
   f_GetConfigButton->setObjectName(QStringLiteral("CLASS_TEST_WINDOW__") % QStringLiteral("GetConfigButton"));
   f_GetConfigButton->setFixedSize(BUTTON_COMMAND_SIZE);

   f_ReadName = new QTextEdit(this);
   f_ReadName->setObjectName(QStringLiteral("CLASS_TEST_WINDOW__") % QStringLiteral("ReadName"));
   f_ReadName->setFixedHeight(TEXTEDIT_SEND_DATA_HEIGHT);
   f_ReadName->setFixedWidth(140);
   f_ReadName->setReadOnly(true);
   f_ReadName->setText("?");

   f_ReadNumberOutput = new QTextEdit(this);
   f_ReadNumberOutput->setObjectName(QStringLiteral("CLASS_TEST_WINDOW__") % QStringLiteral("NumberOutput"));
   f_ReadNumberOutput->setFixedHeight(TEXTEDIT_SEND_DATA_HEIGHT);
   f_ReadNumberOutput->setFixedWidth(30);
   f_ReadNumberOutput->setReadOnly(true);
   f_ReadNumberOutput->setText("?");

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
   f_LabelNumberOutput = new QLabel(f_ReadNumberOutput);
   f_LabelNumberOutput->setText(QStringLiteral(" Number output :"));
   f_LabelNumberOutput->setFixedWidth(80);
   f_ReadStatusLayout = new QHBoxLayout();
   f_ReadStatusLayout->addWidget(f_LabelName);
   f_ReadStatusLayout->addWidget(f_ReadName);
   f_ReadStatusLayout->addWidget(f_LabelNumberOutput);
   f_ReadStatusLayout->addWidget(f_ReadNumberOutput);

   // On paramètre le tableau qui sert de console
   f_TabDebugConsole = new QTableWidget();
   QObject::connect(f_TabDebugConsole->horizontalHeader(), &QHeaderView::sectionClicked, this, &CLASS_SOURCE_WINDOW::ClickOnHeaderItem);
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
void CLASS_SOURCE_WINDOW::ClickOnHeaderItem(SDWord columnIndex)
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
         AddNewMessageInfo((columnIndex == COLUMN_RX_INDEX) ? MESSAGE_RX_DISABLED : MESSAGE_TX_DISABLED, {QString(QStringLiteral("DISABLED")).toUtf8()});

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
         AddNewMessageInfo((columnIndex == COLUMN_RX_INDEX) ? MESSAGE_RX_ENABLED : MESSAGE_TX_ENABLED, {QString(QStringLiteral("ENABLED")).toUtf8()});

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
void CLASS_SOURCE_WINDOW::OnClearButtonClicked(void)
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
void CLASS_SOURCE_WINDOW::OnSendButtonClicked(void)
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
 * *************************************   INITIALIZATION      ***********************************************
 * **********************************************************************************************************/
///
/// \fn OnGetConfigButtonClicked
/// \brief Slot enclenché lors de l'appui sur le bouton "Get Config"
/// \param clicked : état du bouton
///
void CLASS_SOURCE_WINDOW::OnGetConfigButtonClicked(void)
{
   f_IndicCurrentFonction = ID;

   // Réitialisation pour les sorties
   f_ConfigSource->f_LoopCounter = 0;
   f_ConfigSource->f_OutputNumber = 0;
   f_ConfigSource->f_AvailableOutput = 0;
   // Réinitilisation pour les plug
   f_ConfigSource->f_LoopPlugCounter = 0;
   f_ConfigSource->f_Index = 0;
   // Réinitialisation du tableau 2D
   f_ConfigSource->f_OutputTab.clear();

   f_ReadNumberOutput->setText(QStringLiteral("?"));

   // Permet d'indiquer que cette fenetre est celle de l'appareil source aux fenetre parents
   emit this->SIGNAL_IdentifiedSource(this);

   // On arrete la création de nouvelle liste de format custom
   if(f_RandomCustomFormatTimer->isActive() == true)
      f_RandomCustomFormatTimer->stop();

   // Lancement de l'identification de l'appareil
   this->AskDeviceID();
}

///
/// \fn AskFirmwareVersion
/// \brief Questionne la machine sur la version de son firmware
///
void CLASS_SOURCE_WINDOW::AskFirmwareVersion(void)
{
   f_IndicCurrentFonction = FIRMWARE;

   // On demande la version du firmware
   if(f_ConfigSource->f_ConfigName == QLatin1String("Vio4K") || f_ConfigSource->f_ConfigName == QLatin1String("NLC"))
   {
      emit this->SIGNAL_SendRawCommand("VEupd", true);
   }
   else if(f_ConfigSource->f_ConfigName == QLatin1String("LiveCore"))
   {
      emit this->SIGNAL_SendRawCommand("0,VEupd", true);
   }
   f_ReadName->setText(f_ConfigSource->f_ConfigName);
}


///
/// \fn AskOutputStatus
/// \brief Questionne la machine sur le status de ses outputs
///
void CLASS_SOURCE_WINDOW::AskOutputStatus(void)
{
   f_IndicCurrentFonction = OUTPUT;
   //AwTracesLevelError(AW_TRACE_SRC_INFO % "GetOutputData - " % "Output: " % Output % "Plug: " % Plug);

   if(QVariant(f_ConfigSource->f_Output).toInt() < f_ConfigSource->f_Settings->value(QStringLiteral("INFO/MAX_OUTPUT")).toInt() && QVariant(f_ConfigSource->f_Plug).toInt() < f_ConfigSource->f_Settings->value(QStringLiteral("INFO/MAX_OUTPUT_PLUG")).toInt())
      emit this->SIGNAL_SendRawCommand(f_ConfigSource->f_Output % "," % f_ConfigSource->f_Settings->value(QStringLiteral("COMMAND/OUT_AVAILABLE")).toByteArray() , true);
}

///
/// \fn AskPlugStatus
/// \brief Question la machine sur l'état des ses plugs
///
void CLASS_SOURCE_WINDOW::AskPlugStatus(void)
{
   f_IndicCurrentFonction = PLUG;

   if(QVariant(f_ConfigSource->f_Output).toInt() < f_ConfigSource->f_Settings->value(QStringLiteral("INFO/MAX_OUTPUT")).toInt() && QVariant(f_ConfigSource->f_Plug).toInt() < f_ConfigSource->f_Settings->value(QStringLiteral("INFO/MAX_OUTPUT_PLUG")).toInt())
      emit this->SIGNAL_SendRawCommand(f_ConfigSource->f_Output % "," % f_ConfigSource->f_Plug % "," % f_ConfigSource->f_Settings->value(QStringLiteral("COMMAND/OUT_PLUG_STATUS")).toByteArray(), true);
}

///
/// \fn PrintOutputNumber
/// \brief Affiche le nombre d'output sur la fenetre
///
void CLASS_SOURCE_WINDOW::PrintOutputNumber(void)
{
   QByteArray OutputDetected = QVariant(f_ConfigSource->f_AvailableOutput).toByteArray();
   f_ReadNumberOutput->setText(OutputDetected);
}


/***********************************************************************************************************************
 * ****************************************     LAUNCHER        ********************************************************
 * ********************************************************************************************************************/

///
/// \fn StopAll
/// \brief Arrete les timers et réinitialise la variable de FirstLaunch de la class testSource
///
void CLASS_SOURCE_WINDOW::StopAll(void)
{
   f_TestSource->f_FirstLaunch = true;
   // On arrete les timers
   f_RateTimer->stop();
   f_PlugTimer->stop();
}

///
/// \fn StartSource
/// \brief Lance le test de la machine source
///
void CLASS_SOURCE_WINDOW::StartSource(void)
{
   if(ExtCustomFormatSelected == true)
      f_ConfigSource->f_FileName = LOG_REPERTORY % LOG_CUSTOM_FORMAT % "_" %  LOG_SOURCE_DEVICE % "_" % f_ConfigSource->f_DeviceName % "_" % f_ConfigSource->f_FirmwareVersion % "_" % f_ConfigSource->f_CurrentTime;
   else if(ExtCustomFormatSelected == false)
      f_ConfigSource->f_FileName = LOG_REPERTORY % LOG_SOURCE_DEVICE % "_" % f_ConfigSource->f_DeviceName % "_" % f_ConfigSource->f_FirmwareVersion % "_" % f_ConfigSource->f_CurrentTime;

  // Indication dans le log
   QByteArray Tag;
   Tag = "\n******************************* Device Source : " % f_ConfigSource->f_DeviceName.toLatin1() % " connected      Firmware version : " % f_ConfigSource->f_FirmwareVersion.toLatin1() % " ************************************* \n\n" ;

   // Ecriture du tag nom de l'appareil et la version du firmware
   QFile fileNotValid(f_ConfigSource->f_FileName + QStringLiteral("_NOT_VALID.txt"));
   if(!fileNotValid.open(QIODevice::WriteOnly | QIODevice::Append))
      return;
   fileNotValid.write(Tag);
   fileNotValid.close();


   QFile fileValid(f_ConfigSource->f_FileName + QStringLiteral("_VALID.txt"));
   if(!fileValid.open(QIODevice::WriteOnly | QIODevice::Append))
      return;
   fileValid.write(Tag);
   fileValid.close();

   emit this->SIGNAL_CreateReceiverFileLog();

   // Récupération Liste Output
   f_TestSource->f_ListOutput = f_TestSource->ListOutputFromConfigWindow();
   //Récupération Liste Plug
   f_TestSource->f_ListPlug = f_TestSource->ListPlugFromConfigWindow();

   // Etape de vérification
   if(f_TestSource->f_ListOutput.isEmpty()){
      QMessageBox::information(nullptr, QStringLiteral("Configuration manquante"), QStringLiteral("Vous n'avez pas sélectionné les sorties de l'appareil source !"), QMessageBox::Ok);
      return;
   }

   // Ouverture du fichier de config adéquate
   if(f_ConfigSource->f_Settings != nullptr)
   {
      delete f_ConfigSource->f_SettingsTx;
      f_ConfigSource->f_SettingsTx = nullptr;
   }

   f_ConfigSource->f_SettingsTx = new QSettings(QStringLiteral("settings/settingTx_") % f_ConfigSource->f_ConfigName % QStringLiteral(".ini"), QSettings::IniFormat);

   f_TestSource->f_CurrentOutput = f_ConfigSource->f_SettingsTx->value(QStringLiteral("OUTPUT_IDENTIFIED/") % f_TestSource->f_ListOutput.at(f_TestSource->f_IndexListOutput)).toInt();
   f_TestSource->f_CurrentPlug = f_TestSource->f_ListPlug.at(f_TestSource->f_IndexListPlug);

   // On vérifie le choix de format sélectionné et on démarre le test adéquate
   if(ExtCustomFormatSelected == false)
   {
      /// ############################################## Spécification machines ##############################################
      // Pour le livecore nous ne testons pas les 4 premier format et n'utilisons pas le rate de custom
      if(f_ConfigSource->f_ConfigName == QLatin1String("LiveCore"))
      {
         f_TestSource->f_CurrentRate = 1;
         f_TestSource->f_CurrentFormat = 4;
         f_TestSource->f_IndexListFormat += 4;
      }
      /// ####################################################################################################################

      // Création de la liste de format adaptée
      if((f_TestSource->f_CurrentPlug == QLatin1String("SDI")) || (f_TestSource->f_CurrentPlug == QLatin1String("OPTICAL SFP")))
      {
         f_TestSource->CreateListFormat(f_TestSource->f_CurrentOutput , f_ConfigSource->f_Settings->value(QStringLiteral("INFO/MAX_OUTPUT_FORMAT_SDI_OPT")).toInt());
      }
      else
      {
         f_TestSource->CreateListFormat(f_TestSource->f_CurrentOutput , f_ConfigSource->f_Settings->value(QStringLiteral("INFO/MAX_OUTPUT_FORMAT")).toInt());
      }
      // Création de la liste de rate adaptée
      f_TestSource->CreateListRate(f_TestSource->f_CurrentOutput , f_ConfigSource->f_Settings->value(QStringLiteral("INFO/MAX_RATE")).toInt());

      f_TestSource->SendFormat();
   }
   else if(ExtCustomFormatSelected == true)
   {
      /// ############################################## Spécification machines ##############################################
      // On démarre au premier format custom de chaque appareil
      if(f_ConfigSource->f_ConfigName == QLatin1String("Vio4K") || f_ConfigSource->f_ConfigName == QLatin1String("NLC"))
         f_TestSource->f_CurrentFormat = 58;
      else if(f_ConfigSource->f_ConfigName == QLatin1String("LiveCore"))
         f_TestSource->f_CurrentFormat = 38;
      /// ####################################################################################################################

      f_TestSource->CreateCommandListCustomFormat(f_TestSource->f_CurrentOutput);
      f_TestSource->f_ListCustomFormat->LoadConfigFileForCustomFormat();
      f_ConfigSource->f_Settings = f_TestSource->f_ListCustomFormat->GetCustomFormatSetting();
      f_TestSource->SelectCustomSlot();
   }
}

///
/// \fn Stop
/// \brief Arrete tout à la fin du test
///
void CLASS_SOURCE_WINDOW::Stop(void)
{
   this->StopAll();
   emit this->SIGNAL_Final();
}

///
/// \fn SendCommandFormat
/// \brief Envoie la commande pour l'application du format sur la machine
///
void CLASS_SOURCE_WINDOW::SendCommandFormat(void)
{
   this->SendData(f_TestSource->f_FormatList.at(f_TestSource->f_IndexListFormat));

   if(f_RateTimer->isActive() == false)
   {
      if(f_ConfigSource->f_ConfigName == QLatin1String("Vio4K"))
      {
         f_TestSource->f_Time = 12000;
         f_RateTimer->start(12000);
      }
      else if(f_ConfigSource->f_ConfigName == QLatin1String("LiveCore"))
      {
         f_TestSource->f_Time = 6000;
         f_RateTimer->start(6000);
      }
   }
}

///
/// \fn SendCommandRate
/// \brief Envoie la commande pour l'application du rate sur la machine
///
void CLASS_SOURCE_WINDOW::SendCommandRate(void)
{
   // Réinitilisation des variables
   this->ClearMessageList();
   // On envoie la commande à l'élément de la liste
   this->SendData(f_TestSource->f_RateList.at(f_TestSource->f_IndexListRate));

   if(f_PlugTimer->isActive() == false)
   {
      if(f_ConfigSource->f_ConfigName == QLatin1String("Vio4K"))
      {
         f_TestSource->f_Time = 30000;
         f_PlugTimer->start(30000);
      }
      else if(f_ConfigSource->f_ConfigName == QLatin1String("LiveCore"))
      {
         f_TestSource->f_Time = 10000;
         f_PlugTimer->start(10000);
      }
   }
}

///
/// \fn RetardLauncherStatusPlug
/// \brief Permet de retarder le lancement de la fonction en passant par un timer
///
void CLASS_SOURCE_WINDOW::RetardLauncherStatusPlugForCustom(void)
{
   if(f_PlugTimer->isActive() == false)
   {
      f_TestSource->f_Time = 30000;
      f_PlugTimer->start(30000);
   }
}


///
/// \fn AskStatusPlug
/// \brief Questionne la machine sur le status des plugs
///
void CLASS_SOURCE_WINDOW::AskStatusPlug(void)
{
   f_IndicCurrentFonction = STATUS_PLUG;

   if(f_ConfigSource->f_ConfigName == QLatin1String("Vio4K") || f_ConfigSource->f_ConfigName == QLatin1String("NLC"))
      this->SIGNAL_SendRawCommand(f_TestSource->f_OutputValue % "," % f_TestSource->f_OutputPlugValue % "," % f_ConfigSource->f_Settings->value(QStringLiteral("COMMAND/OUT_PLUG_STATUS")).toByteArray() , true);
   else if(f_ConfigSource->f_ConfigName == QLatin1String("LiveCore"))
      this->SIGNAL_SendRawCommand(f_TestSource->f_OutputValue % "," % f_ConfigSource->f_Settings->value(QStringLiteral("COMMAND/OUT_FORMAT_RATE_VALIDITY")).toByteArray() , true);
}

///
/// \fn VerifyCompatibility
/// \brief Vérifie la compatibilité du format et rate
///
void CLASS_SOURCE_WINDOW::VerifyCompatibility(void)
{
   // Indicateurs
   f_TestSource->f_InfoCompatibility = NOT_INFORMED;

   /// ############################################## Spécification machines ##############################################
   // On vérifie si le format est supporté
   if(f_ConfigSource->f_ConfigName == QLatin1String("Vio4K"))
   {
      for(int i = 0 ; i < f_MessageList.size() ; ++i)
      {
         if(f_MessageList.at(i).Data.contains(QLatin1String("FORMAT NOT SUPPORTED")))
            f_TestSource->f_InfoCompatibility = INCOMPATIBLE;

         else if(f_MessageList.at(i).Data.contains(QLatin1String("FORMAT SUPPORTED")))
            f_TestSource->f_InfoCompatibility = COMPATIBLE;
      }
      f_TestSource->f_PlugStatus = f_MessageList.last().Data;
      f_TestSource->f_PlugStatus.chop(2);
   }
   else if(f_ConfigSource->f_ConfigName == QLatin1String("LiveCore"))
   {
      // Création d'une liste ne conservant que les Data
      QStringList DataList;
      for(int i = 0; i < f_MessageList.size() ; ++i)
      {
         if(f_MessageList[i].Type == MESSAGE_RX )
         {
            if(f_MessageList.at(i).Data.endsWith(QLatin1String("\r\n")))
            {
               QString DataTraited = f_MessageList.at(i).Data;
               DataTraited.chop(2);
               DataList.append(DataTraited);
            }
         }
      }
      // On indique si il y compatibilité à la class TestSource
      if(DataList.at(0).startsWith(QVariant(f_ConfigSource->f_Settings->value(QStringLiteral("COMMAND/OUT_RATE")).toByteArray() % f_TestSource->f_OutputValue % "," % QVariant(f_TestSource->f_CurrentRate).toByteArray()).toString()))
      {      
         if(DataList.endsWith(QVariant(f_ConfigSource->f_Settings->value(QStringLiteral("COMMAND/OUT_FORMAT_RATE_VALIDITY")).toByteArray() % f_TestSource->f_OutputValue % QStringLiteral(",1")).toString()))
         { 
            f_TestSource->f_InfoCompatibility = COMPATIBLE;
            f_TestSource->f_PlugStatus = QVariant(f_ConfigSource->f_Settings->value(QStringLiteral("COMMAND/OUT_PLUG_STATUS")).toByteArray() % f_TestSource->f_OutputValue % QStringLiteral(",1")).toString();
         }
         else
         {
            f_TestSource->f_InfoCompatibility = INCOMPATIBLE;
            f_TestSource->f_PlugStatus = QVariant(f_ConfigSource->f_Settings->value(QStringLiteral("COMMAND/OUT_PLUG_STATUS")).toByteArray() % f_TestSource->f_OutputValue % QStringLiteral(",") % f_TestSource->f_OutputPlugValue % QStringLiteral(",0")).toString();
         }
      }
      else
      {
         f_TestSource->f_InfoCompatibility = INCOMPATIBLE;
         f_TestSource->f_PlugStatus = QVariant(f_ConfigSource->f_Settings->value(QStringLiteral("COMMAND/OUT_PLUG_STATUS")).toByteArray() % f_TestSource->f_OutputValue % QStringLiteral(",") % f_TestSource->f_OutputPlugValue % QStringLiteral(",0")).toString();
      }
   }
   /// ######################################################################################################################
   ///
   // On lance l'écriture
   f_TestSource->WriteFileLog();
}

///
/// \fn NextPLugToTestOrNextRate
/// \brief Test le plug suivant sinon passe à la fonction suivante
///
void CLASS_SOURCE_WINDOW::NextPLugToTestOrNextRate(void)
{
   // Plug suivant si compatible
   if( f_TestSource->f_InfoCompatibility == COMPATIBLE )
   {
      f_TestSource->f_CurrentOutputPlugValue += 1;
      f_TestSource->CheckOutputPlugStatus();
   }
   else
   {
      // On vérifie si on est en format standard
      if(ExtCustomFormatSelected == false)
      {
         // Réinitialisation
         f_TestSource->f_CurrentOutputPlugValue = 0;
         // Mise a jour numéro du rate
         f_TestSource->f_CurrentRate += 1;
         // On passe au rate suivant
         f_TestSource->f_IndexListRate += 1;
         this->ClearMessageList();
         f_TestSource->SendRate();
      }
      else
      {
         f_TestSource->f_CurrentOutputPlugValue += 1;
         f_TestSource->CheckOutputPlugStatus();
      }
   }
}

///
/// \fn SelectCustomSlot
/// \brief Envoie la commande pour sélectionner le custom slot
/// \param command : commande de selection de slot
///
void CLASS_SOURCE_WINDOW::SelectCustomSlot(const QByteArray &command)
{
   emit this->SIGNAL_SendRawCommand(command, true);  
   return;
}


///
/// \brief CLASS_SOURCE_WINDOW::ChangeToRateCustom
///
void CLASS_SOURCE_WINDOW::ChangeToRateCustom(void)
{
   this->SendData(QVariant(f_TestSource->f_CurrentOutput).toByteArray() % ",0OUrat");
   return;
}

///
/// \fn SendData
/// \brief Envoie la donnée en raw
/// \param data : donnée à envoyer à la machine
///
void CLASS_SOURCE_WINDOW::SendData(QByteArray data)
{
   QByteArray SendData(data);
   emit this->SIGNAL_SendRawCommand(SendData, true);

   /// ############################################## Spécification machines ##############################################
   if(f_ConfigSource->f_ConfigName == QLatin1String("Vio4K") || f_ConfigSource->f_ConfigName == QLatin1String("NLC"))
   {
      QByteArray currentOutput;
      currentOutput.setNum(f_TestSource->f_CurrentOutput);
      QByteArray SendData2(currentOutput % "," % f_ConfigSource->f_Settings->value(QStringLiteral("INFO/OUTPUT_ACTIVE")).toByteArray() % f_ConfigSource->f_Settings->value(QStringLiteral("COMMAND/OUT_FORMAT_UPDATE")).toByteArray());
      emit this->SIGNAL_SendRawCommand(SendData2, true);
   }
   /// ####################################################################################################################

   return;
}

///
/// \fn AskDeviceID
/// \brief Demande à l'appareil de s'identifier
///
void CLASS_SOURCE_WINDOW::AskDeviceID()
{
   QByteArray SendData(QByteArrayLiteral("?"));
   emit this->SIGNAL_SendRawCommand(SendData, true);

   return;
}


///
/// \fn RebootData
/// \brief Permet de remettre à zéro les données de cette fenetre
///
void CLASS_SOURCE_WINDOW::RebootData(void)
{
   this->StopAll();

   f_TestSource->f_IndexListFormat = 0;
   f_TestSource->f_IndexListRate = 0;
   f_TestSource->f_IndexListOutput = 0;
   f_TestSource->f_IndexListPlug = 0;
   f_TestSource->f_CurrentOutput = 0;
   f_TestSource->f_CurrentFormat = 0;
   f_TestSource->f_CurrentOutputPlugValue = 0;
   f_TestSource->f_CurrentRate = 0;
   f_TestSource->f_Time = 0;
   f_TestSource->f_ListOutput.clear();
   f_TestSource->f_ListPlug.clear();
   f_TestSource->f_FormatList.clear();
   f_TestSource->f_RateList.clear();
   this->ClearMessageList();

   f_TestSource->f_FirstLaunch = true;
   f_TestSource->f_FirstCustomSlot = true;
   f_TestSource->f_TwoSlotSaved = false;

   f_TestSource->f_StepCustom = CLASS_TEST_SOURCE::NONE;

   if(f_TestSource->f_ListCustomFormat != nullptr)
      f_TestSource->f_ListCustomFormat->ReinitializeIndexCustomList();

   // On récupère le temps à la remise à zéro
   QString currentTime = QDateTime::currentDateTime().toString(QStringLiteral("dddd dd MMMM yyyy hh:mm:ss.zzz"));
   QString currentTime1 = currentTime.replace(QLatin1String(" "),QLatin1String("_"));
   f_ConfigSource->f_CurrentTime = currentTime1.replace(QLatin1String(":"),QLatin1String("."));

   // Changement du Nom de fichier pour en créer un nouveau au reboot
   if(ExtCustomFormatSelected == false)
      f_ConfigSource->f_FileName = LOG_REPERTORY % LOG_SOURCE_DEVICE % QStringLiteral("_") % f_ConfigSource->f_DeviceName % QStringLiteral("_") % f_ConfigSource->f_FirmwareVersion % QStringLiteral("_") % f_ConfigSource->f_CurrentTime;
   else if (ExtCustomFormatSelected == true)
      f_ConfigSource->f_FileName = LOG_REPERTORY % LOG_CUSTOM_FORMAT % QStringLiteral("_") % LOG_SOURCE_DEVICE % QStringLiteral("_") % f_ConfigSource->f_DeviceName % QStringLiteral("_") % f_ConfigSource->f_FirmwareVersion % QStringLiteral("_") % f_ConfigSource->f_CurrentTime;

   return;
}

///
/// \fn ChangeSourceSetting
/// \brief Permet de changer le fichier setting pris en compte par le programme lors du chargement d'un nouveau fichier de config pour l'appareil source
/// \param newSetting : Nom du nouveau fichier de config de la machine source
///
void CLASS_SOURCE_WINDOW::ChangeSourceSetting(const QString& newSetting)
{
   if(f_ConfigSource->f_Settings != nullptr)
   {
      delete f_ConfigSource->f_Settings;
      f_ConfigSource->f_Settings = nullptr;
   }
   f_ConfigSource->f_Settings = new QSettings(newSetting , QSettings::IniFormat);
}

///
/// \brief CLASS_SOURCE_WINDOW::ClearMessageList
///
void CLASS_SOURCE_WINDOW::ClearMessageList(void)
{
   // Parcourt de la liste
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
   f_IndexMessageList = 0;
   f_TreatedMessage = 0;
}

/***************************************************************************************************************************************************
 * *************************************************************************************************************************************************
 * ************************************************************************************************************************************************/


///
/// \fn AddNewMessageInfo
/// \brief Construit la structure info du message
/// \param type : Type de message
/// \param data : Données du message
/// \return  T_MESSAGE_INFO& : Infos du message
///
void CLASS_SOURCE_WINDOW::AddNewMessageInfo(T_MESSAGE_TYPE type, const QByteArray &data)
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
   f_IndexMessageList = f_IndexMessageList + 1;
}

///
/// \fn DisplayNewMessages
/// \brief Affiche les nouveaux messages
///
void CLASS_SOURCE_WINDOW::DisplayNewMessages(void)
{
   // Protège l'accès à la liste
   QMutexLocker Locker(&f_DisplayMutex);

   // On limite le nombre de message traité par bouclage
   SDWord CurrentListSize(f_MessageList.count());
   SDWord CurrentTreatedMessages(0);

   // Parcourt de la liste
   while (f_TreatedMessage < CurrentListSize)
   {
      const T_MESSAGE_INFO& CurrentMessage(f_MessageList.at(f_TreatedMessage++));

      // Pas d'affichage des messages si la colonne correspondante est désactivée
      if ((CurrentMessage.Type == CLASS_SOURCE_WINDOW::MESSAGE_RX) && (f_ColumnDisabled.at(COLUMN_RX_INDEX) == true))
         continue;
      if (((CurrentMessage.Type == CLASS_SOURCE_WINDOW::MESSAGE_TX) || ((CurrentMessage.Type == CLASS_SOURCE_WINDOW::MESSAGE_TX_REFUSED))) && (f_ColumnDisabled.at(COLUMN_TX_INDEX) == true))
         continue;


      // Cré une nouvelle ligne vide
      const SDWord RowIndex(f_TabDebugConsole->rowCount());
      f_TabDebugConsole->insertRow(RowIndex);

      // Définit le temps
      f_TabDebugConsole->setItem(RowIndex, COLUMN_TIME_INDEX, new QTableWidgetItem(GetTimestampString(CurrentMessage.Timestamp)));

      switch (CurrentMessage.Type)
      {
         default :
         case CLASS_SOURCE_WINDOW::MESSAGE_RX :
         case CLASS_SOURCE_WINDOW::MESSAGE_RX_DISABLED :
         case CLASS_SOURCE_WINDOW::MESSAGE_RX_ENABLED :
            f_TabDebugConsole->setItem(RowIndex, COLUMN_RX_INDEX, new QTableWidgetItem(CurrentMessage.Data));
            if (f_ColumnDisabled.at(COLUMN_TX_INDEX) == true)
            {
               f_TabDebugConsole->setItem(RowIndex, COLUMN_TX_INDEX, new QTableWidgetItem());
               f_TabDebugConsole->item(RowIndex, COLUMN_TX_INDEX)->setBackground(Qt::gray);
            }
            break;

         case CLASS_SOURCE_WINDOW::MESSAGE_TX :
         case CLASS_SOURCE_WINDOW::MESSAGE_TX_REFUSED :
         case CLASS_SOURCE_WINDOW::MESSAGE_TX_DISABLED :
         case CLASS_SOURCE_WINDOW::MESSAGE_TX_ENABLED :
            f_TabDebugConsole->setItem(RowIndex, COLUMN_TX_INDEX, new QTableWidgetItem(CurrentMessage.Data));
            if (f_ColumnDisabled.at(COLUMN_RX_INDEX) == true)
            {
               f_TabDebugConsole->setItem(RowIndex, COLUMN_RX_INDEX, new QTableWidgetItem());
               f_TabDebugConsole->item(RowIndex, COLUMN_RX_INDEX)->setBackground(Qt::gray);
            }
            break;
      }

      ++CurrentTreatedMessages;

      // Si on a traité trop de messages, on force une sortie
      if (++CurrentTreatedMessages > MAX_TREAT_MESSAGES_PER_REFRESH)
         break;
   }

   // On redimensionne les lignes par rapport à leur contenu
   f_TabDebugConsole->resizeRowsToContents();

   // On visualise toujours la dernière ligne
   if ((f_TabDebugConsole->verticalScrollBar()->value() >= f_TabDebugConsole->verticalScrollBar()->maximum()) == true)
      f_TabDebugConsole->scrollToBottom();
}

///
/// \fn DecodeTimeWidgetText
/// \brief Effectue le décodage d'une cellule "Time"
/// \param cellText : Texte de la cellule
/// \return QString : Texte à afficher en infobulle
///
QString CLASS_SOURCE_WINDOW::DecodeTimeWidget(SQWord timestamp)
{
   return QString::number(timestamp);
}

/******************************************************************************
 *                             Operateur surcharge                            *
 *****************************************************************************/
