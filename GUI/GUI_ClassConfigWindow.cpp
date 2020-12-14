///
/// \file GUI_ClassConfigWindow.cpp
/// \brief Classe CLASS_CONFIG_WINDOW
/// \author ANALOG WAY - GLE
/// \date 2018-05-02
///
/// Fonctions de la classe CLASS_CONFIG_WINDOW
///

// C++ Standard
// Librairies Qt
#include <QApplication>
#include <QCloseEvent>
#include <QColor>
#include <QList>
#include <QStringList>
#include <QLabel>
#include <QAccessible>
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollArea>
#include <QDir>
// Modules
#include "GUI_ClassConfigWindow.h"

/******************************** DEFINITIONS ********************************/

// Chemin vers l'icône de l'application
#define ICON_APP                       QStringLiteral(":/PICTURES/myappico.ico")

// Nom des fichiers de config des machines
#define SETTING_VIO                    QStringLiteral("settings/settingVio.ini")
#define SETTING_LIVECORE               QStringLiteral("settings/settingLiveCore.ini")
#define SETTING_NLC                    QStringLiteral("settings/settingNLC.ini")
#define SETTING_MNG                    QStringLiteral("settings/settingMNG.ini")

/*----------------------------- Types publiques -----------------------------*/
/*--------------------------- Constantes publiques --------------------------*/
/*--------------------------- Variables publiques ---------------------------*/
QString ExtSourceName = "";
QString ExtReceiverName = "";
QVector<QString> *ExtTableau = nullptr;
QVector<QVector<QString>*> ExtTableau2D;
bool ExtModeScrutationSelected = false;
bool ExtCustomFormatSelected = false;
bool ExtFirstSlot = false;
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
/// \fn CLASS_CONFIGWINDOWS(QObject *parent)
/// \brief Constructeur de CLASS_CONFIGWINDOWS
///
/// \param[in] parent : Objet parent
///
CLASS_CONFIG_WINDOW::CLASS_CONFIG_WINDOW(QWidget *parent):
   QMainWindow(parent),
   f_NewMachineMenu(nullptr),
   f_NewMachineAction(nullptr),
   f_PrincipalLayout(nullptr),
   f_GetConfigLayout(nullptr),
   f_LabelLayout(nullptr),
   f_CentralLayout(nullptr),
   f_ConfigLayout(nullptr),
   f_TxLayout(nullptr),
   f_RxLayout(nullptr),
   f_HorizontalRxLayout(nullptr),
   f_HorizontalTxLayout(nullptr),
   f_ApplicationLayout(nullptr),
   f_InfoLabelLayout(nullptr),
   f_InfoLayout(nullptr),
   f_SaveLoadLayout(nullptr),
   f_ProgressBarLayout(nullptr),
   mesLayoutRx(),
   mesLayoutTx(),
   mesComboBoxInput(),
   mesComboBoxOutput(),
   mesGroupBox(),
   mesVerticalLayout(),
   f_Line(nullptr),
   f_GetConfigButton(nullptr),
   f_LoadConfigFileButton(nullptr),
   f_ApplyButton(nullptr),
   f_StartButton(nullptr),
   f_StopButton(nullptr),
   f_SaveButton(nullptr),
   f_LoadButton(nullptr),
   f_RebootButton(nullptr),
   f_LabelOutputIndic(nullptr),
   f_LabelFormat(nullptr),
   f_LabelRate(nullptr),
   f_LabelInputIndic(nullptr),
   f_OutputIndic(nullptr),
   f_FormatIndic(nullptr),
   f_RateIndic(nullptr),
   f_InputIndic(nullptr),
   f_ProgressBar(nullptr),
   f_NbreTotalOutput(0),
   f_TotalTime(0),
   f_ComboBoxNumberOutput(nullptr),
   f_NumberOutputList(),
   f_ComboBoxModeSelect(nullptr),
   f_ComboboxInputPlug(nullptr),
   f_ComboboxOutputPlug(nullptr),
   f_ComboBoxInputAdded(nullptr),
   f_ComboBoxOutputAdded(nullptr),
   f_SelectNumberInput(nullptr),
   f_GlobalScroll(nullptr),
   f_RxContainer(nullptr),
   f_TxContainer(nullptr),
   f_InfoContainer(nullptr),
   f_SourceNameLabel(nullptr),
   f_ReceiverNameLabel(nullptr),
   f_SettingRx(nullptr),
   f_SettingTx(nullptr),
   f_SettingRxTx(nullptr),
   f_SettingSource(nullptr),
   f_IndexComboBoxRx(0),
   f_IndexComboBoxTx(0),
   f_NumberOutput(1),
   f_NumberGroupInput(1),
   f_StockNumOutputAdded(0),
   f_Tableau2D(),
   f_Tableau2DCopy(),
   f_Connection(false),
   f_TimeValue(0),
   f_SourceWindow(nullptr),
   f_ReceiverWindow(nullptr),
   f_TestSource(nullptr),
   f_TestReceiver(nullptr)
{
   // On met en place l'interface graphique
   this->SetUpUserIntertace();

   // On remplit tous les tableaux pour stocker les widgets et information utiles
   this->CreateAllArray();  

   // On connecte le signal de création de nouvelle machine
   QObject::connect(f_NewMachineAction, &QAction::triggered, this, &CLASS_CONFIG_WINDOW::SIGNAL_CreateMachineRequest);

   // On connecte le signal de récupération des configuration
   QObject::connect(f_GetConfigButton, &QPushButton::clicked, this, &CLASS_CONFIG_WINDOW::OnGetConfigButtonClicked);
   QObject::connect(f_LoadConfigFileButton, &QPushButton::clicked, this, &CLASS_CONFIG_WINDOW::OnLoadConfigFileButtonClicked);
   QObject::connect(f_ComboBoxNumberOutput, &QComboBox::currentTextChanged, this, &CLASS_CONFIG_WINDOW::OnComboxNumOutputCreateOutputInterface);
   QObject::connect(f_ApplyButton, &QPushButton::clicked, this, &CLASS_CONFIG_WINDOW::OnApplyButtonClicked);
   QObject::connect(f_StartButton, &QPushButton::clicked, this, &CLASS_CONFIG_WINDOW::OnStartButtonClicked);
   QObject::connect(f_StopButton, &QPushButton::clicked, this, &CLASS_CONFIG_WINDOW::OnStopButtonClicked);
   QObject::connect(f_SaveButton, &QPushButton::clicked, this, &CLASS_CONFIG_WINDOW::OnSaveButtonClicked);
   QObject::connect(f_LoadButton, &QPushButton::clicked, this, &CLASS_CONFIG_WINDOW::OnLoadButtonClicked);
   QObject::connect(f_RebootButton, &QPushButton::clicked, this, &CLASS_CONFIG_WINDOW::OnRebootButtonClicked);
   QObject::connect(f_EraseInterfaceButton, &QPushButton::clicked, this, &CLASS_CONFIG_WINDOW::OnEraseInterfaceButtonClicked);
   QObject::connect(f_ComboBoxModeSelect, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CLASS_CONFIG_WINDOW::OnComboBoxModeSelectionned);
}

///
/// \fn ~CLASS_CONFIGWINDOWS()
/// \brief Destructeur de CLASS_CONFIGWINDOWS
///
CLASS_CONFIG_WINDOW::~CLASS_CONFIG_WINDOW()
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
/// \fn CreateNewDeviceWindow
/// \brief Instancie une nouvelle fenêtre machine
/// \return CLASS_DEVICE_WINDOW * : Objet représentant la fenêtre
///
CLASS_DEVICE_WINDOW * CLASS_CONFIG_WINDOW::CreateNewDeviceWindow(void)
{
   // Création d'une fenêtre machine vide
   CLASS_DEVICE_WINDOW *NewMachineWindow = new CLASS_DEVICE_WINDOW();

   QObject::connect(NewMachineWindow, &CLASS_DEVICE_WINDOW::SIGNAL_MySourceWindow, this, &CLASS_CONFIG_WINDOW::SetSourceWindow);
   QObject::connect(NewMachineWindow, &CLASS_DEVICE_WINDOW::SIGNAL_MyReceiverWindow, this, &CLASS_CONFIG_WINDOW::SetReceiverWindow);

   // On l'ajoute à la liste
   f_MachineWindowList.append(NewMachineWindow);

   return NewMachineWindow;
}

///
/// \fn DeleteDeviceWindow
/// \brief Instancie une nouvelle fenêtre machine
/// \param machineWindowObj : Objet représentant la fenêtre machine
///
void CLASS_CONFIG_WINDOW::DeleteDeviceWindow(CLASS_DEVICE_WINDOW *machineWindowObj)
{
   // Rien à faire si la fenêtre n'existe pas
   if (f_MachineWindowList.contains(machineWindowObj) == false)
      return;

   // Retire la fenêtre de la liste
   f_MachineWindowList.removeAll(machineWindowObj);

   return;
}

/******************************************************************************
 *                               Slots publiques                              *
 *****************************************************************************/

/******************************************************************************
 *                              MÃ©thodes privÃ©es                              *
 *****************************************************************************/

///
/// \fn SetUpUserIntertace
/// \brief Met en place l'interface graphique de la fenêtre
///
void CLASS_CONFIG_WINDOW::SetUpUserIntertace(void)
{
   // Icone de la fenetre
   this->setWindowIcon(QIcon(ICON_APP));

   // On change le titre de la fenêtre
   this->setWindowTitle(QStringLiteral("Panneau de configuration des machines"));

   // Définit la taille idéale/minimale pour la fenêtre
   this->setMinimumSize(QSize(800, 950));

   // On instancie les différents menus
   // "New"
   f_NewMachineMenu = new QMenu(QStringLiteral("New"), this);
   f_NewMachineAction = new QAction(QStringLiteral("Add Device"), this);
   f_NewMachineMenu->addAction(f_NewMachineAction);
   f_NewMachineAction->setIcon(QIcon(QStringLiteral(":/PICTURES/add.png")));

   // Création de la barre de menu
   this->setMenuBar(new QMenuBar(this));
   this->menuBar()->addMenu(f_NewMachineMenu);

   // Création du widget
   f_CentralWindow = new QWidget(this);
   setCentralWidget(f_CentralWindow);

   // Création des buttons
   f_GetConfigButton = new QPushButton(QStringLiteral("Get Config"), this);
   f_GetConfigButton->setFixedSize(150,30);
   f_LoadConfigFileButton = new QPushButton(QStringLiteral("Load config file"), this);
   f_LoadConfigFileButton->setFixedSize(QSize(150, 30));
   f_LoadConfigFileButton->setContentsMargins(0,0,0,0);
   f_LoadConfigFileButton->setIcon(QIcon(QStringLiteral(":/PICTURES/up_icon.ico")));
   f_LoadConfigFileButton->setDisabled(true);
   f_ApplyButton = new QPushButton(QStringLiteral("Apply"), this);
   f_ApplyButton->setFixedSize(QSize(150, 30));
   f_ApplyButton->setContentsMargins(0,0,0,0);
   f_ApplyButton->setIcon(QIcon(QStringLiteral(":/PICTURES/apply.png")));
   f_ApplyButton->setDisabled(true);
   f_StartButton = new QPushButton( QStringLiteral("Start"), this);
   f_StartButton->setContentsMargins(0,0,0,0);
   f_StartButton->setFixedSize(QSize(150,30));
   f_StartButton->setIcon(QIcon(QStringLiteral(":/PICTURES/play_icon.png")));
   f_StartButton->setDisabled(true);
   f_StopButton = new QPushButton( QStringLiteral("Pause"), this);
   f_StopButton->setContentsMargins(0,0,0,0);
   f_StopButton->setFixedSize(QSize(150,30));
   f_StopButton->setIcon(QIcon(QStringLiteral(":/PICTURES/pause_icon.png")));
   f_StopButton->setDisabled(true);
   f_SaveButton = new QPushButton( QStringLiteral("Save configuration"), this);
   f_SaveButton->setContentsMargins(0,0,0,0);
   f_SaveButton->setFixedSize(QSize(150,30));
   f_SaveButton->setIcon(QIcon(QStringLiteral(":/PICTURES/down_icon.png")));
   f_LoadButton = new QPushButton( QStringLiteral("Load configuration"), this);
   f_LoadButton->setContentsMargins(0,0,0,0);
   f_LoadButton->setFixedSize(QSize(150,30));
   f_LoadButton->setIcon(QIcon(QStringLiteral(":/PICTURES/up_icon.ico")));
   f_RebootButton = new QPushButton( QStringLiteral("Remise à zéro") , this);
   f_RebootButton->setContentsMargins(0,0,0,0);
   f_RebootButton->setFixedSize(QSize(150,30));
   f_RebootButton->setIcon(QIcon(QStringLiteral(":/PICTURES/reboot.png")));
   f_RebootButton->setIconSize(QSize(40,40));
   f_EraseInterfaceButton = new QPushButton(QStringLiteral("Effacer l'interface"), this);
   f_EraseInterfaceButton->setContentsMargins(0,0,0,0);
   f_EraseInterfaceButton->setFixedSize(QSize(150,30));

   f_LabelOutputIndic = new QLabel(this);
   f_LabelOutputIndic->setText(QStringLiteral("Sortie en cours de test"));
   f_LabelFormat = new QLabel(this);
   f_LabelFormat->setText(QStringLiteral("Format appliqué"));
   f_LabelRate = new QLabel(this);
   f_LabelRate->setText(QStringLiteral("Rate appliqué"));
   f_LabelInputIndic = new QLabel(this);
   f_LabelInputIndic->setText(QStringLiteral("Entrée en cours de test"));
   f_OutputIndic = new QLineEdit(f_CentralWindow);
   f_OutputIndic->setReadOnly(true);
   f_FormatIndic = new QLineEdit(f_CentralWindow);
   f_FormatIndic->setReadOnly(true);
   f_RateIndic = new QLineEdit(f_CentralWindow);
   f_RateIndic->setReadOnly(true);
   f_InputIndic = new QLineEdit(f_CentralWindow);
   f_InputIndic->setReadOnly(true);

   // Création d'autres widget
   f_ComboBoxNumberOutput = new QComboBox(f_CentralWindow);
   for(int i = 1; i < 25; ++i)
   {
      QString caracNumb;
      caracNumb.setNum(i);
      f_NumberOutputList.insert(i, caracNumb);
   }
   f_NumberOutputList.prepend(QLatin1String("Add output"));
   f_ComboBoxNumberOutput->insertItems(0, f_NumberOutputList);
   f_ComboBoxNumberOutput->setFixedSize(150,30);
   f_ComboBoxNumberOutput->setContentsMargins(50,0,0,0);

   // Ajout d'un RadioButton pour sélection les formats custom
   f_ComboBoxModeSelect = new QComboBox(f_CentralWindow);
   QStringList ListMode;
   ListMode.append((QStringLiteral("Mode par défaut")));
   ListMode.append((QStringLiteral("Mode format custom")));
   ListMode.append((QStringLiteral("Mode scrutation")));
   f_ComboBoxModeSelect->insertItems(0, ListMode);
   f_ComboBoxModeSelect->setFixedSize(150,30);
   f_ComboBoxModeSelect->setContentsMargins(50,0,0,0);

   f_Line = new QFrame();
   f_Line->setFrameShape(QFrame::VLine);
   f_Line->setFrameShadow(QFrame::Raised);

   f_SourceNameLabel = new QLabel(this);
   f_SourceNameLabel->setAlignment(Qt::AlignHCenter);
   f_SourceNameLabel->setText(QStringLiteral("Source"));
   f_ReceiverNameLabel = new QLabel(this);
   f_ReceiverNameLabel->setAlignment(Qt::AlignHCenter);
   f_ReceiverNameLabel->setText(QStringLiteral("Receiver"));

   f_Container = new QWidget();
   f_RxContainer = new QWidget();
   f_TxContainer = new QWidget();
   f_InfoContainer = new QWidget();
   f_GlobalScroll = new QScrollArea();
   f_GlobalScroll->setWidgetResizable(true);

   f_ProgressBar = new QProgressBar(f_CentralWindow);
   f_ProgressBar->setFixedWidth(this->width()/2);
   f_ProgressBar->setMinimum(0);
   f_ProgressBar->setValue(f_TimeValue);
   f_ProgressBar->hide();

   // Création de layout
   f_PrincipalLayout = new QVBoxLayout();
   f_PrincipalLayout->setContentsMargins(0,0,0,0); 
   f_GetConfigLayout = new QHBoxLayout();
   f_GetConfigLayout->setContentsMargins(0,0,0,0);
   f_LabelLayout = new QHBoxLayout();
   f_LabelLayout->setContentsMargins(0,0,0,0);
   f_CentralLayout = new QVBoxLayout();
   f_CentralLayout->setContentsMargins(0,0,0,0);
   f_ConfigLayout = new QHBoxLayout();
   f_ConfigLayout->setContentsMargins(0,0,0,0);
   f_RxLayout = new QVBoxLayout();
   f_RxLayout->setContentsMargins(0,0,0,0);
   f_TxLayout = new QVBoxLayout();
   f_TxLayout->setContentsMargins(0,0,0,0);
   f_ApplicationLayout = new QHBoxLayout();
   f_ApplicationLayout->setContentsMargins(0,0,0,0);
   f_InfoLabelLayout = new QHBoxLayout();
   f_InfoLabelLayout->setContentsMargins(0,0,0,0);
   f_InfoLayout = new QHBoxLayout();
   f_InfoLayout->setContentsMargins(0,0,0,0);
   f_SaveLoadLayout = new QHBoxLayout();
   f_SaveLoadLayout->setContentsMargins(0,0,0,0);
   f_ProgressBarLayout = new QHBoxLayout();
   f_ProgressBarLayout->setContentsMargins(0,0,0,0);

   // Mise en place des layouts et des widgets
   f_RxContainer->setLayout(f_RxLayout);
   f_TxContainer->setLayout(f_TxLayout);
   f_Container->setLayout(f_ConfigLayout);
   f_GlobalScroll->setWidget(f_Container);
   f_GlobalScroll->show();

   f_GetConfigLayout->addWidget(f_GetConfigButton, 0,  Qt::AlignHCenter);
   f_GetConfigLayout->addWidget(f_ComboBoxNumberOutput, 0,  Qt::AlignHCenter);
   f_GetConfigLayout->addWidget(f_LoadConfigFileButton, 0,  Qt::AlignHCenter);
   f_GetConfigLayout->addWidget(f_ComboBoxModeSelect, 0, Qt::AlignHCenter);
   f_LabelLayout->addWidget(f_SourceNameLabel);
   f_LabelLayout->addWidget(f_ReceiverNameLabel);
   f_ConfigLayout->addWidget(f_TxContainer);
   f_ConfigLayout->addWidget(f_Line);
   f_ConfigLayout->addWidget(f_RxContainer);
   f_CentralLayout->addWidget(f_GlobalScroll);
   f_ApplicationLayout->addWidget(f_RebootButton, 0,  Qt::AlignHCenter);
   f_ApplicationLayout->addWidget(f_EraseInterfaceButton, 0, Qt::AlignHCenter);
   f_ApplicationLayout->addWidget(f_ApplyButton, 0, Qt::AlignHCenter);
   f_SaveLoadLayout->addWidget(f_StartButton, 0, Qt::AlignHCenter);
   f_SaveLoadLayout->addWidget(f_StopButton, 0, Qt::AlignHCenter);
   f_SaveLoadLayout->addWidget(f_SaveButton, 0, Qt::AlignHCenter);
   f_SaveLoadLayout->addWidget(f_LoadButton, 0, Qt::AlignHCenter);
   f_InfoLabelLayout->addWidget(f_LabelOutputIndic, 0, Qt::AlignHCenter);
   f_InfoLabelLayout->addWidget(f_LabelFormat, 0, Qt::AlignHCenter);
   f_InfoLabelLayout->addWidget(f_LabelRate, 0, Qt::AlignHCenter);
   f_InfoLabelLayout->addWidget(f_LabelInputIndic, 0, Qt::AlignHCenter);
   f_InfoLayout->addWidget(f_OutputIndic, 0,  Qt::AlignHCenter);
   f_InfoLayout->addWidget(f_FormatIndic, 0, Qt::AlignHCenter);
   f_InfoLayout->addWidget(f_RateIndic, 0, Qt::AlignHCenter);
   f_InfoLayout->addWidget(f_InputIndic, 0,  Qt::AlignHCenter);
   f_ProgressBarLayout->addWidget(f_ProgressBar, 0,  Qt::AlignHCenter);

   f_PrincipalLayout->addSpacing(10);
   f_PrincipalLayout->addLayout(f_GetConfigLayout);
   f_PrincipalLayout->addLayout(f_LabelLayout);
   f_PrincipalLayout->addLayout(f_CentralLayout);
   f_PrincipalLayout->addLayout(f_ApplicationLayout);
   f_PrincipalLayout->addLayout(f_SaveLoadLayout);
   f_PrincipalLayout->addSpacing(10);
   f_PrincipalLayout->addLayout(f_InfoLabelLayout);
   f_PrincipalLayout->addLayout(f_InfoLayout);
   f_PrincipalLayout->addSpacing(5);
   f_PrincipalLayout->addLayout(f_ProgressBarLayout);
   f_PrincipalLayout->addSpacing(10);

   f_CentralWindow->setLayout(f_PrincipalLayout);
}

///
/// \fn CreateAllArray
/// \brief Creer tous les tableau nécessaire à la fenêtre
///
void CLASS_CONFIG_WINDOW::CreateAllArray(void)
{
   // Création du tableau de layout
   for(int i = 0; i < 500; ++i)
   {
      f_HorizontalRxLayout = new QHBoxLayout();
      f_HorizontalTxLayout = new QHBoxLayout();
      mesLayoutRx.append(f_HorizontalRxLayout);
      mesLayoutTx.append(f_HorizontalTxLayout);
   }

   // Création du tableau de stockage de vertical layout
   for(int i = 0; i < 100; ++i)
   {
      QVBoxLayout* VerticalLayout = new QVBoxLayout();
      mesVerticalLayout.append(VerticalLayout);
   }

   // Création du tableau de Combobox
   for(int i= 0; i < 300 ; ++i)
   {
      QString name;
      name.setNum(i);
      f_ComboboxInputPlug = new QComboBox();
      f_ComboboxInputPlug ->installEventFilter(this);
      f_ComboboxOutputPlug  = new QComboBox();
      f_ComboboxOutputPlug ->installEventFilter(this);
      f_ComboboxInputPlug ->setObjectName(name);
      f_ComboboxOutputPlug ->setObjectName(name);
      mesComboBoxInput.append(f_ComboboxInputPlug );
      mesComboBoxOutput.append(f_ComboboxOutputPlug );
   }
   // Création du tableau de GroupBox
   for(int i = 0; i < 24; ++i)
   {
      QGroupBox* GroupBox = new QGroupBox();
      GroupBox->setFixedHeight(400);
      GroupBox->setObjectName(QVariant(i).toString());
      GroupBox->setTitle(QStringLiteral("Group") % QString::number(i + 1));
      mesGroupBox.append(GroupBox);
   }

   // Création Tableau2D pour stocker les informations renseignées par les Combobox
   for(int ligne = 0; ligne < 24; ++ligne)
   {
      ExtTableau = new QVector<QString>[18];
      QVector<QString> *Tableau = new QVector<QString>[18];
      QVector<QString> *TableauCopy = new QVector<QString>[18];

      for(int colonne = 0 ; colonne < 18 ; ++colonne)
      {
          Tableau->append(QLatin1String(""));
          TableauCopy->append(QLatin1String(""));
          ExtTableau->append(QLatin1String(""));
      }
      f_Tableau2D.insert(ligne, Tableau);
      f_Tableau2DCopy.insert(ligne, TableauCopy);
      ExtTableau2D.insert(ligne, ExtTableau);
   }
}


///
/// \fn OnGetConfigButtonClicked
/// \brief Slot enclenché lors de l'appui sur le bouton "Get Config Rx and Tx "
///
void CLASS_CONFIG_WINDOW::OnGetConfigButtonClicked(void)
{
   // Si nous avons bien détecté les fenetres source et receiver procéder aux connexions
   if(f_SourceWindow == nullptr || f_ReceiverWindow == nullptr)
      return;

   QObject::connect(this, &CLASS_CONFIG_WINDOW::SIGNAL_NewSettingSource, f_SourceWindow, &CLASS_SOURCE_WINDOW::ChangeSourceSetting);
   QObject::connect(this, &CLASS_CONFIG_WINDOW::SIGNAL_NewSettingSource, f_ReceiverWindow, &CLASS_RECEIVER_WINDOW::ChangeSettingSource);

   // Vérifie si le fichier existe pour eviter tout conflit supprimer le fichier déjà existant
   QFile file(QStringLiteral("settings/settingRxTx.ini"));
   if(file.exists())
      file.remove();

   // Indication des noms des appareil sur l'interface
   f_ReceiverNameLabel->setText(f_ReceiverWindow->f_ConfigReceiver->f_ConfigName);
   f_SourceNameLabel->setText(f_SourceWindow->f_ConfigSource->f_ConfigName);

   /// ############################################## Spécification machines ##############################################
   // On charge les fichier de config correspondant
   if(f_SettingRx != nullptr)
   {
      delete f_SettingRx;
      f_SettingRx = nullptr;
   }

   // Machine réceptrice
   switch (f_ReceiverWindow->f_ConfigReceiver->f_MyReceiverDevice)
   {
      default:
         break;
      case CLASS_CONFIG_RECEIVER::LIVECORE:
         f_SettingRx = new QSettings (QStringLiteral("settings/settingRx_LiveCore.ini") , QSettings::IniFormat);
         break;
      case CLASS_CONFIG_RECEIVER::VIO4K:
         f_SettingRx = new QSettings (QStringLiteral("settings/settingRx_Vio4K.ini"), QSettings::IniFormat);
         break;
      case CLASS_CONFIG_RECEIVER::NLC:
         f_SettingRx = new QSettings (QStringLiteral("settings/settingRx_NLC.ini") , QSettings::IniFormat);
         break;
      case CLASS_CONFIG_RECEIVER::MNG:
         f_SettingRx = new QSettings (QStringLiteral("settings/settingRx_MNG.ini") , QSettings::IniFormat);
         break;
   }

   // Machine source
   if(f_SettingTx != nullptr)
   {
      delete f_SettingTx;
      f_SettingTx = nullptr;
   }

   if(f_SettingSource != nullptr)
   {
      delete f_SettingSource;
      f_SettingSource = nullptr;
   }

   switch (f_SourceWindow->f_ConfigSource->f_MySourceDevice)
   {
      default:
         break;
      case CLASS_CONFIG_SOURCE::LIVECORE:
         f_SettingTx = new QSettings (QStringLiteral("settings/settingTx_LiveCore.ini") , QSettings::IniFormat);
         f_SettingSource = new QSettings (SETTING_LIVECORE , QSettings::IniFormat);
         emit this->SIGNAL_NewSettingSource(SETTING_LIVECORE);
         break;
      case CLASS_CONFIG_SOURCE::VIO4K:
         f_SettingTx = new QSettings (QStringLiteral("settings/settingTx_Vio4K.ini") , QSettings::IniFormat);
         f_SettingSource = new QSettings (SETTING_VIO , QSettings::IniFormat);
         emit this->SIGNAL_NewSettingSource(SETTING_VIO);
         break;
      case CLASS_CONFIG_SOURCE::NLC:
         f_SettingTx = new QSettings (QStringLiteral("settings/settingTx_NLC.ini") , QSettings::IniFormat);
         f_SettingSource = new QSettings (SETTING_NLC , QSettings::IniFormat);
         emit this->SIGNAL_NewSettingSource(SETTING_NLC);
         break;
      case CLASS_CONFIG_SOURCE::MNG:
         f_SettingTx = new QSettings (QStringLiteral("settings/settingTx_MNG.ini") , QSettings::IniFormat);
         f_SettingSource = new QSettings (SETTING_MNG , QSettings::IniFormat);
         emit this->SIGNAL_NewSettingSource(SETTING_MNG);
         break;
   }

   /// #####################################################################################################################

   // On initialise le nouveau fichier de config regroupant les informations nécessaires
   if(f_SettingRxTx != nullptr)
   {
      delete f_SettingRxTx;
      f_SettingRxTx = nullptr;
   }

   f_SettingRxTx = new QSettings (QStringLiteral("settings/settingRxTx.ini") , QSettings::IniFormat);

   /******************************************************* Partie INPUT *************************************************************/

   this->RewriteInfo(f_SettingRx, f_SettingRxTx, QStringLiteral("INPUT_IDENTIFIED"), QStringLiteral("INPUT"));

   this->RewriteInfo(f_SettingRx, f_SettingRxTx, QStringLiteral("PLUG_IDENTIFIED"), QStringLiteral("INPUT_PLUG"));

   /******************************************************* Partie OUTPUT ************************************************************/

   this->RewriteInfo(f_SettingTx, f_SettingRxTx, QStringLiteral("OUTPUT_IDENTIFIED"), QStringLiteral("OUTPUT"));

   this->RewriteInfo(f_SettingTx, f_SettingRxTx, QStringLiteral("PLUG_IDENTIFIED"), QStringLiteral("OUTPUT_PLUG"));

   // On active le bouton Apply et LoadConfigFile
   f_ApplyButton->setEnabled(true);
   f_LoadConfigFileButton->setEnabled(true);
}

///
/// \fn CreateOutputInterface
/// \brief Creer le nombre d'output selon la demande de l'utilisateur
/// \param currentText
///
void CLASS_CONFIG_WINDOW::OnComboxNumOutputCreateOutputInterface(QString currentText)
{
   QString fileName = QStringLiteral("settings/settingRxTx.ini");
   QFile file(fileName);

   // Tant que le bouton de GetConfig n'est pas appuyé on n'exécute pas la fonction
   if(f_SettingRxTx == nullptr || !file.exists())
   {
      QMessageBox::information(nullptr, QStringLiteral("Hmm..."), QStringLiteral("Clic on Get Config first"), QMessageBox::Ok);
      f_ComboBoxNumberOutput->disconnect();
      f_ComboBoxNumberOutput->setCurrentIndex(0);
      QObject::connect(f_ComboBoxNumberOutput, &QComboBox::currentTextChanged, this, &CLASS_CONFIG_WINDOW::OnComboxNumOutputCreateOutputInterface);
      return;
   }

   if(file.isOpen() == true)
      file.close();

   // Information visuelle pour indiquer de ne pas faire plus de 24 sorties
   QMessageBox *info = new QMessageBox(QMessageBox::Warning, QStringLiteral("Information output"), QStringLiteral("24 outputs authorized"), QMessageBox::Ok, nullptr );
   int number;
   number = QVariant(currentText).toInt();

   for(int i = 0; i < number; ++i)
   {
      if(f_NumberOutput < 25)
      {
         this->AddNewComboBox();
         if(f_StockNumOutputAdded == 0)
         {
            f_RxLayout->addWidget(mesGroupBox.at(i));
         }
         else
         {
            f_RxLayout->addWidget(mesGroupBox.at(f_StockNumOutputAdded + i));
         }
      }
      else
      {
         info->show();
      }
   }
   f_StockNumOutputAdded += number;
   // Remet l'index sur l'indicateur
   f_ComboBoxNumberOutput->setCurrentIndex(0);
}

///
/// \fn AddNewComboBox
/// \brief Ajoute un nouveau comboBox dans le layout désigné
/// \param OutputOrInputGroupName
/// \param LayoutApplied
///
void CLASS_CONFIG_WINDOW::AddNewComboBox(void)
{
   QStringList List;
   QStringList ListNumberofInput;
   QString number;
   QWidget *widget = new QWidget(f_CentralWindow);
   widget->setObjectName(QStringLiteral("Erasable"));
   QLabel *label = new QLabel(QStringLiteral("Output") % QString::number(f_NumberOutput), widget);
   label->setContentsMargins(0,0,0,0);

   // Création de la liste du nombre d'input voulu
   for(int i = 1; i < 9 ; ++i)
   {
      QString number;
      number.setNum(i);
      ListNumberofInput.append(number);
   }

   f_SettingRxTx->beginGroup(QStringLiteral("OutputOrInputGroupName"));
   List = f_SettingRxTx->allKeys();
   List = ReorderList(List);
   f_SettingRxTx->endGroup();

   // Rajout d'un indicateur en début des listes
   List.prepend(QLatin1String("Select output"));
   ListNumberofInput.prepend(QLatin1String("Select number input"));
   // Récupération de l'index pour un association des combobox entre eux
   number.setNum(f_IndexComboBoxTx);

   QVBoxLayout *VerticalLayout = new QVBoxLayout();

   // Création du Combobox de la sortie
   f_ComboBoxOutputAdded = new QComboBox();
   f_ComboBoxOutputAdded->installEventFilter(this);
   f_ComboBoxOutputAdded->setObjectName(number);
   f_ComboBoxOutputAdded->insertItems(0, List);

   // Création du Combobox de sélection du nombre d'entrée
   f_SelectNumberInput = new QComboBox();
   f_SelectNumberInput->installEventFilter(this);
   f_SelectNumberInput->setObjectName(number);
   f_SelectNumberInput->insertItems(0, ListNumberofInput);

   // Ajout des wigdet dans le vertical layout
   mesLayoutTx.at(f_IndexComboBoxTx)->addWidget(f_ComboBoxOutputAdded);
   mesLayoutTx.at(f_IndexComboBoxTx)->addWidget(mesComboBoxOutput.at(f_IndexComboBoxTx));
   VerticalLayout->addLayout(mesLayoutTx.at(f_IndexComboBoxTx));
   VerticalLayout->addWidget(f_SelectNumberInput);

   // Connexion des Combobox aux fonctions
   mesLayoutTx.at(f_IndexComboBoxTx)->connect(f_ComboBoxOutputAdded, &QComboBox::currentTextChanged, this, &CLASS_CONFIG_WINDOW::ComboBoxOutputListChanged);
   mesLayoutTx.at(f_IndexComboBoxTx)->connect(f_ComboBoxOutputAdded, &QComboBox::currentTextChanged, this, &CLASS_CONFIG_WINDOW::Fill2dArray);
   mesLayoutTx.at(f_IndexComboBoxTx)->connect(mesComboBoxOutput.at(f_IndexComboBoxTx), &QComboBox::currentTextChanged, this, &CLASS_CONFIG_WINDOW::Fill2dArray);
   mesLayoutTx.at(f_IndexComboBoxTx)->connect(f_SelectNumberInput, &QComboBox::currentTextChanged, this, &CLASS_CONFIG_WINDOW::CreateGroupBoxInput);

   // Insertion du layout dans un widget
   widget->setLayout(VerticalLayout);
   widget->setFixedHeight(400);

   // Insertion widget dans layout
   f_TxLayout->addWidget(widget);

   // Garder en mémoire l'index du tableau des layout et le nombre de sortie ajouter
   f_IndexComboBoxTx += 1;
   f_NumberOutput += 1;
}

///
/// \fn ComboBoxInputListChanged
/// \brief Adapte la liste du combobox de plug d'entrée
/// \param currentText
///
void CLASS_CONFIG_WINDOW::ComboBoxInputListChanged(QString currentText)
{
   QObject *pointeur = sender();
   QString valeurName;
   int valeur;
   valeurName = pointeur->objectName();
   valeur = valeurName.toInt();

   this->CreateComboBoxList(QStringLiteral("INPUT"), QStringLiteral("INPUT_PLUG"), currentText, mesComboBoxInput.at(valeur));
}


///
/// \fn ComboBoxOutputListChanged
/// \brief  Adapte la liste du combobox de plug de sortie
/// \param currentText
///
void CLASS_CONFIG_WINDOW::ComboBoxOutputListChanged(QString currentText)
{
   QObject *pointeur = sender();
   QString valeurName;
   int valeur;
   valeurName = pointeur->objectName();
   valeur = valeurName.toInt();

   this->CreateComboBoxList(QStringLiteral("OUTPUT"), QStringLiteral("OUTPUT_PLUG"), currentText, mesComboBoxOutput.at(valeur));
}


///
/// \fn Fill2dArray
/// \brief Remplit le tableau 2D stockant les inputs outputs et plugs
///
void CLASS_CONFIG_WINDOW::Fill2dArray(void)
{
   QObject *pointeur = sender();
   QString valeurName;
   QString type;
   QObject *grandparent;

   valeurName = pointeur->objectName();
   grandparent = pointeur->parent()->parent();
   QString nameGrandParent = grandparent->objectName();

   type = grandparent->metaObject()->className();
   if(type == QLatin1String("QGroupBox"))
   {
      QList<QComboBox *> InputWidgets = grandparent->findChildren<QComboBox *>();
      for(int i = 0; i < InputWidgets.size(); ++i)
      {
         QString Data;
         Data = InputWidgets.at(i)->currentText();
         if(Data != QLatin1String("Select input"))
         {
            f_Tableau2D.at(nameGrandParent.toInt())->replace(i+2, Data);
         }
      }
   }
   else
   {
      QList<QComboBox *> OutputWidgets = pointeur->parent()->findChildren<QComboBox *>();
      for(int i = 0; i < OutputWidgets.size(); i+=3)
      {
         QString Data;
         Data = OutputWidgets.at(i)->currentText();
         if(Data != QLatin1String("Select output"))
         f_Tableau2D.at(valeurName.toInt())->replace(0, Data);
      }
      for(int i = 1; i < OutputWidgets.size(); i+=3)
      {
         QString Data;
         Data = OutputWidgets.at(i)->currentText();
         f_Tableau2D.at(valeurName.toInt())->replace(1, Data);
      }
   }
}


///
/// \fn CreateComboBoxList
/// \brief Création de la liste du comboBox
/// \param IndicOutputOrInput
/// \param IndicPlug
/// \param ComboBox
///
void CLASS_CONFIG_WINDOW::CreateComboBoxList(QString IndicOutputOrInput, QString IndicPlug, QString currentText, QComboBox* ComboBox)
{
   f_SettingRxTx->beginGroup(IndicOutputOrInput);
   QStringList OutputOrInput = f_SettingRxTx->allKeys();
   f_SettingRxTx->endGroup();

   int count = 0;

   ComboBox->clear();

   for(int i = 0; i < OutputOrInput.size(); ++i)
   {
      if(currentText == OutputOrInput.at(i))
      {
         f_SettingRxTx->beginGroup(IndicPlug);
         QStringList Plug = f_SettingRxTx->allKeys();
         f_SettingRxTx->endGroup();

         QStringList result;
         result = Plug.filter(currentText);

         for(int j = 0; j < result.size() ; ++j)
         {
            QByteArray tab;
            tab = QVariant(result.at(j)).toByteArray();
            for(int k = 0 ; k < tab.size(); ++k)
            {
               count += 1;
               if(tab.at(k) == '/')
               {
                  if(count != currentText.size()+1)
                  {
                     result.removeOne(tab);
                     j -= 1;
                  }
                  else
                  {
                     tab.remove(0, count);
                     result.replace(j, tab);
                  }
               }
            }
            count = 0;
         }
         ComboBox->insertItems(0, result);
      }
   }
}


///
/// \fn CreateGroupBoxInput
/// \brief Creer les GroupeBox regroupant les input pour un output
/// \param currentText
///
void CLASS_CONFIG_WINDOW::CreateGroupBoxInput(QString currentText)
{
   QStringList List;
   f_SettingRxTx->beginGroup(QStringLiteral("INPUT"));
   List = f_SettingRxTx->allKeys();
   List = ReorderList(List);
   f_SettingRxTx->endGroup();
   List.prepend(QStringLiteral("Select input"));

   QObject *pointeur = sender();
   QString valeurName;
   int valeur;
   valeurName = pointeur->objectName();
   valeur = valeurName.toInt();

   int number;
   number = currentText.toInt();

   // Effacer contenu du groupbox s'il existe déja
   QLayoutItem *item;
   while((item = mesVerticalLayout.at(valeur)->itemAt(0)) != nullptr)
   {
      mesVerticalLayout.at(valeur)->removeItem(item);
      delete item->widget();
      delete item;
   }

   // On mets a jour f_Tableau2D
   for(int colonne = 2 ; colonne < 18 ; ++colonne)
   {
      f_Tableau2D.at(valeur)->replace(colonne , QLatin1String(""));
   }

   // On crée les comboxbox et ont les ajoute sur l'interface
   for(int i = 0; i < number; ++i)
   {
      // Création liste déroulante des inputs
      f_ComboBoxInputAdded = new QComboBox();
      f_ComboBoxInputAdded->installEventFilter(this);
      f_ComboBoxInputAdded->setObjectName(QString::number(f_IndexComboBoxRx));
      f_ComboBoxInputAdded->insertItems(0, List);
      // Ajout de la liste déroulante dans un layout
      mesLayoutRx.at(f_IndexComboBoxRx)->addWidget(f_ComboBoxInputAdded);
      mesLayoutRx.at(f_IndexComboBoxRx)->addWidget(mesComboBoxInput.at(f_IndexComboBoxRx));
      // Connexion pour le lancement des fonctions adéquates
      mesLayoutRx.at(f_IndexComboBoxRx)->connect(f_ComboBoxInputAdded, &QComboBox::currentTextChanged, this, &CLASS_CONFIG_WINDOW::ComboBoxInputListChanged);
      mesLayoutRx.at(f_IndexComboBoxRx)->connect(f_ComboBoxInputAdded, &QComboBox::currentTextChanged, this, &CLASS_CONFIG_WINDOW::Fill2dArray);
      mesLayoutRx.at(f_IndexComboBoxRx)->connect(mesComboBoxInput.at(f_IndexComboBoxRx), &QComboBox::currentTextChanged, this, &CLASS_CONFIG_WINDOW::Fill2dArray);

      QWidget *widget = new QWidget(f_CentralWindow);
      widget->setObjectName(QStringLiteral("Erasable"));
      widget->setLayout(mesLayoutRx.at(f_IndexComboBoxRx));
      mesVerticalLayout.at(valeur)->addWidget(widget);
      f_IndexComboBoxRx += 1;
   }
   mesGroupBox.at(valeur)->setLayout(mesVerticalLayout.at(valeur));
   f_NumberGroupInput += 1;
}

///
/// \fn ReorderList
/// \brief Réorganise la liste en remettant les éléments dans l'ordre
/// \param List
/// \return QStringList : la liste réordonnée
///
QStringList CLASS_CONFIG_WINDOW::ReorderList(QStringList List)
{
   QByteArray tab1, tab2;
   QByteArray number1, number2;
   int valeur1, valeur2;

   for(int i = 0; i < List.size(); ++i)
   {
      tab1 = QVariant(List.at(i)).toByteArray();

      for(int j = 0; j < tab1.size(); ++j)
      {
         QChar carac = tab1.at(j);
         if(carac.isDigit() == true)
            number1.append(carac);
      }

      valeur1 = number1.toInt();

      for(int c = i + 1; c < List.size() ; ++c)
      {
         tab2 = QVariant(List.at(c)).toByteArray();
         for(int k = 0; k < tab2.size(); ++k)
         {
            QChar carac = tab2.at(k);
            if(carac.isDigit() == true)
               number2.append(carac);
         }

         valeur2 = number2.toInt();

         if(valeur2 < valeur1)
         {
            valeur1 = valeur2;
            List.swap(i,c);
         }

         number2.clear();
      }
      number1.clear();
   }
    return List;
}


///
/// \fn RewriteInfo
/// \brief Créer le le fichier de configuration RxTx
/// \param settingsToRead
/// \param settingsToWrite
/// \param groupToRead
/// \param groupToCreate
///
void CLASS_CONFIG_WINDOW::RewriteInfo(QSettings* settingsToRead, QSettings* settingsToWrite, QString groupToRead, QString groupToCreate)
{
   QStringList ListKey;

   settingsToRead->beginGroup(groupToRead);
   ListKey = settingsToRead->allKeys();
   settingsToRead->endGroup();

   settingsToWrite->beginGroup(groupToCreate);
   for(int i = 0; i < ListKey.size() ; ++i )
   {
      settingsToWrite->setValue(ListKey.at(i), settingsToRead->value(groupToRead % QStringLiteral("/") % ListKey.at(i)));
   }
   settingsToWrite->endGroup();

   return;
}


///
/// \fn SaveToFile
/// \brief Sauvegarde une interface de configuration dans un fichier Configuration.txt
///
void CLASS_CONFIG_WINDOW::OnSaveButtonClicked(void)
{
   QString fileName = QFileDialog::getSaveFileName(this, QStringLiteral("Sauvegarde configuration"), QStringLiteral("") , QStringLiteral("Configuration (*.txt) ;; All Files(*)"));

   // Si aucun nom entré ne rien faire
   if(fileName.isEmpty())
      return;

   else
   {
      QFile file(fileName);
      if(!file.open(QIODevice::WriteOnly))
      {
         QMessageBox::information(this, QStringLiteral("Impossible d'ouvrir le fichier"), file.errorString());
         return;
      }

      QTextStream out(&file);

      for(int i = 0; i < 24; ++i)
      {
         for(int j = 0; j < 18 ; ++j)
         {
            QByteArray data;
            data = QVariant(f_Tableau2D.at(i)->at(j)).toByteArray();
            out << data;
            out << QStringLiteral("\r\n");
         }
      }
      file.close();
   }
   return;
}

///
/// \fn LoadFromFile
/// \brief Charge une interface de configuration et stocke dans le Tableau2DCopy via un fichier Configuration.txt
///
void CLASS_CONFIG_WINDOW::OnLoadButtonClicked(void)
{
   if(f_SettingRxTx == nullptr)
   {
      QMessageBox::information(this, QStringLiteral("Fichier de config manquant"), QStringLiteral("Récupérer les fichiers de config des appareils avant de lancer le chargement d'une ancienne config"), QMessageBox::Ok);
      return;
   }

   QString fileName = QFileDialog::getOpenFileName(this, QStringLiteral("Ouverture configuration"), "", QStringLiteral("Configuration (*.txt) ;; All Files(*)"));

   if(fileName.isEmpty())
      return;

   else{
      QFile file(fileName);

      if(!file.open(QIODevice::ReadOnly))
      {
         QMessageBox::information(this, QStringLiteral("Impossible d'ouvrir le fichier"), file.errorString());
         return;
      }

      QTextStream in(&file);

      while(!in.atEnd())
      {
         for(int i = 0; i < 24; ++i)
         {
            for(int j = 0; j < 18; ++j)
            {
                QString text = in.readLine();

                if(text == QLatin1String("\r\n")){
                   text = QStringLiteral("");
                }
                f_Tableau2DCopy.at(i)->replace(j , text);
            }
         }
      }
      file.close();

      if (f_Tableau2DCopy.isEmpty())
      {
          QMessageBox::information(this, QStringLiteral("Aucune données dans le fichier"), QStringLiteral("Le fichier que vous comptez ouvrir ne contient aucune donnée."));
      }

   }
   this->UpdateInterface();

   return;
}

///
/// \fn UpdateInterface
/// \brief Charge une interface selon les données du tableau Tableau2DCopy
///
void CLASS_CONFIG_WINDOW::UpdateInterface(void)
{
   for(int i = 0; i < 24; ++i)
   {
      int nombreInput = 0;
      int StartIndexInput = 2;

      if(f_Tableau2DCopy.at(i)->at(0) != QLatin1String(""))
      {
         f_ComboBoxNumberOutput->setCurrentText(QStringLiteral("1"));
         f_ComboBoxOutputAdded->setCurrentText(f_Tableau2DCopy.at(i)->at(0));
         mesComboBoxOutput.at(i)->setCurrentText(f_Tableau2DCopy.at(i)->at(1));
      }

      // Mise en place des paire de menu déroulante dans chaque GroupBox
      for(int j = 2; j < 18; ++j)
      {
         if((j%2 == 0) && (f_Tableau2DCopy.at(i)->at(j) != QLatin1String("")))
         {
            nombreInput += 1;
            f_SelectNumberInput->setCurrentText(QString::number(nombreInput));
         }
      }

      // On récupère les combobox du GroupBox dans une liste
      QList<QComboBox *> comboboxInput = mesGroupBox.at(i)->findChildren<QComboBox *>();

      for(int k = 0; k < comboboxInput.size(); k += 2 )
      {
         comboboxInput.at(k)->setCurrentText(f_Tableau2DCopy.at(i)->at(StartIndexInput));
         comboboxInput.at(k + 1)->setCurrentText(f_Tableau2DCopy.at(i)->at(StartIndexInput + 1));
         StartIndexInput += 2;
      }
   }

   return;
}

///
/// \fn OnLoadConfigFileButtonClicked
/// \brief Slot enclenché lors de l'appui sur le bouton "Load config file"
///
void CLASS_CONFIG_WINDOW::OnLoadConfigFileButtonClicked(void)
{
   if(f_SourceWindow != nullptr && f_ReceiverWindow != nullptr)
   {
      QObject::connect(this, &CLASS_CONFIG_WINDOW::SIGNAL_NewSettingSource, f_SourceWindow, &CLASS_SOURCE_WINDOW::ChangeSourceSetting);
      QObject::connect(this, &CLASS_CONFIG_WINDOW::SIGNAL_NewSettingSource, f_ReceiverWindow, &CLASS_RECEIVER_WINDOW::ChangeSettingSource);

      QString fileName = QFileDialog::getOpenFileName(this, QStringLiteral("Ouverture configuration"), QStringLiteral(""), QStringLiteral("ConfigFiles (*.ini) ;; All Files(*)"));

      if(fileName.isEmpty())
         return;
      else
      {
         QFile file(fileName);

         if(!file.open(QIODevice::ReadOnly))
         {
            QMessageBox::information(this, QStringLiteral("Impossible d'ouvrir le fichier"), file.errorString());
            return;
         }
         emit this->SIGNAL_NewSettingSource(fileName);

         if(f_SettingSource != nullptr)
            delete f_SettingSource;

         f_SettingSource = new QSettings(fileName , QSettings::IniFormat);
         file.close();
      }
      statusBar()->showMessage(QStringLiteral("Fichier chargé !"));
   }
   else
   {
      QMessageBox::information(nullptr, QStringLiteral("Hmm... not the right moment"), QStringLiteral("Configurate your devices first"), QMessageBox::Ok);
   }
   return;
}

///
/// \fn OnApplyButtonClicked
/// \brief Slot enclenché lors de l'appui sur le bouton "Apply"
/// \param clicked : état du bouton
///
void CLASS_CONFIG_WINDOW::OnApplyButtonClicked(void)
{
   if(!ExtTableau2D.isEmpty())
      ExtTableau2D.clear();

   //On donne les du tableau au fenetre des appareils source et récepteur via une variable externe
   ExtTableau2D = f_Tableau2D;

   // On connecte les fenetres si on a  bien récupérer leurs adresses
   if((f_SourceWindow != nullptr) && (f_ReceiverWindow != nullptr) && f_Connection == false)
   {
      if(f_TestSource != nullptr)
      {
         delete f_TestSource;
         f_TestSource = nullptr;
      }
      f_TestSource = f_SourceWindow->SourceThread.GetTestSource();

      if(f_TestReceiver != nullptr)
      {
         delete f_TestReceiver;
         f_TestReceiver = nullptr;
      }
      f_TestReceiver = f_ReceiverWindow->ReceiverThread.GetTestReceiver();

      QObject::connect(f_TestSource, &CLASS_TEST_SOURCE::SIGNAL_Start, f_ReceiverWindow, &CLASS_RECEIVER_WINDOW::StartReceiver);
      QObject::connect(f_TestSource, &CLASS_TEST_SOURCE::SIGNAL_GetOutputInfo, f_ReceiverWindow, &CLASS_RECEIVER_WINDOW::WriteOutputInfo);
      QObject::connect(f_TestSource, &CLASS_TEST_SOURCE::SIGNAL_NextOutputApplied, f_ReceiverWindow, &CLASS_RECEIVER_WINDOW::NextInputApplied);
      QObject::connect(f_TestSource, &CLASS_TEST_SOURCE::SIGNAL_CurrentOutput, this, &CLASS_CONFIG_WINDOW::WriteOutputIndic);
      QObject::connect(f_TestSource, &CLASS_TEST_SOURCE::SIGNAL_CurrentFormat, this, &CLASS_CONFIG_WINDOW::WriteFormatIndic);
      QObject::connect(f_TestSource, &CLASS_TEST_SOURCE::SIGNAL_CurrentRate, this, &CLASS_CONFIG_WINDOW::WriteRateIndic);
      QObject::connect(f_SourceWindow, &CLASS_SOURCE_WINDOW::SIGNAL_Final, this, &CLASS_CONFIG_WINDOW::CompleteProgressBar);

      QObject::connect(f_TestReceiver, &CLASS_TEST_RECEIVER::SIGNAL_NextRateApplied, f_TestSource, &CLASS_TEST_SOURCE::SendRate);
      QObject::connect(f_TestReceiver, &CLASS_TEST_RECEIVER::SIGNAL_CurrentInput, this, &CLASS_CONFIG_WINDOW::WriteInputIndic);
      QObject::connect(f_TestReceiver, &CLASS_TEST_RECEIVER::SIGNAL_NextFormatApplied, f_TestSource, &CLASS_TEST_SOURCE::SendFormat);
      QObject::connect(f_TestReceiver, &CLASS_TEST_RECEIVER::SIGNAL_NextSlotCustom, f_TestSource, &CLASS_TEST_SOURCE::SelectCustomSlot);

      QObject::connect(this, &CLASS_CONFIG_WINDOW::SIGNAL_RebootSource, f_SourceWindow, &CLASS_SOURCE_WINDOW::RebootData);
      QObject::connect(this, &CLASS_CONFIG_WINDOW::SIGNAL_RebootReceiver, f_ReceiverWindow, &CLASS_RECEIVER_WINDOW::RebootData);

      // Connexion des fenetre source et receiver pour la création de fichier
      QObject::connect(f_SourceWindow, &CLASS_SOURCE_WINDOW::SIGNAL_CreateReceiverFileLog, f_ReceiverWindow, &CLASS_RECEIVER_WINDOW::CreateReceiverLogFile);

      QObject::connect(f_TestSource, &CLASS_TEST_SOURCE::SIGNAL_AddTimeValue, this, &CLASS_CONFIG_WINDOW::SetProgressBarTimeValue);

      f_Connection = true;
   }
   // Il faut vérifier que le tableau ne soit pas vide avant de lancer le test
   if(ExtTableau2D.at(0)->at(0) != QLatin1String("") )
   {
      Bool PlugNotAdaptedOnCustomMode = false;

      // Vérification si tout les plug sont sélectionnés
      QList<QComboBox *> AllComboBox = this->findChildren<QComboBox *>();
      foreach (QComboBox *combobox, AllComboBox)
      {
         if(combobox->currentText() == QLatin1String("Select output") || combobox->currentText() == QLatin1String("Select input"))
         {
            QMessageBox::information(this, QStringLiteral("Un oubli de votre part..."), QStringLiteral("Output/Input non sélectionné !"), QMessageBox::Ok);
            return;
         }

         // On indique à l'utilisateur que le SDI et OPTICAL SFP ne sont pas pris en compte lors de l'utilisation de format custom
         if(ExtCustomFormatSelected == true && (combobox->currentText() == QLatin1String("SDI") || combobox->currentText() == QLatin1String("OPTICAL SFP") ))
            PlugNotAdaptedOnCustomMode = true;
      }
      // Message à l'utilisateur de plug non adapté pour le mode custom
      if(PlugNotAdaptedOnCustomMode == true)
         QMessageBox::information(this, QStringLiteral("Spécification à propos du format custom"), QStringLiteral("Le test des formats custom ne s'effectue pas sur les plugs SDI et OPTICAL SFP"), QMessageBox::Ok);

      f_StartButton->setEnabled(true);
      f_StopButton->setEnabled(true);
   }

   //On récupère le nombre de format choisi par l'utilisateur
   f_SettingSource->beginGroup(QStringLiteral("OUTPUT_FORMAT_VALUE"));
   QStringList listFormat = f_SettingSource->allKeys();
   int nombreFormat = listFormat.size();
   f_SettingSource->endGroup();

   //On récupère le nombre de rate choisi par l'utilisateur
   f_SettingSource->beginGroup(QStringLiteral("RATE_VALUE"));
   QStringList listRate = f_SettingSource->allKeys();
   int nombreRate = listRate.size();
   f_SettingSource->endGroup();

   //Programmation de la progress bar en fonction du nombre de sortie et du mode sélectionné
   if(ExtCustomFormatSelected == false)
   {
      QString DeviceSourceName = f_SourceWindow->SourceThread.GetConfigSource()->f_ConfigName;
      if(DeviceSourceName == QLatin1String("Vio4K") || DeviceSourceName == QLatin1String("NLC"))
         f_TotalTime = f_StockNumOutputAdded * (18000 * nombreRate + 28000) * nombreFormat;
      else if(DeviceSourceName == QLatin1String("LiveCore"))
         f_TotalTime = f_StockNumOutputAdded * (5000 * nombreRate + 10000) * nombreFormat;
   }
   else
   {
      f_TotalTime = f_StockNumOutputAdded * 45000 * 20;
   }
   f_ProgressBar->setMaximum(f_TotalTime);

   //On indique que le test peut etre lancé
   statusBar()->showMessage(QStringLiteral("Ready"));

   return;
}

///
/// \brief CLASS_CONFIG_WINDOW::OnRebootButtonClicked
///
void CLASS_CONFIG_WINDOW::OnRebootButtonClicked(void)
{
   emit this->SIGNAL_RebootSource();
   emit this->SIGNAL_RebootReceiver();

   f_OutputIndic->clear();
   f_FormatIndic->clear();
   f_RateIndic->clear();
   f_InputIndic->clear();

   f_TimeValue = 0;

   f_ProgressBar->setValue(0);
   f_ProgressBar->update();
   statusBar()->showMessage(QStringLiteral("Données réinitialisées"));

   return;
}

///
/// \fn GetSourceWindow
/// \brief Permet de récupérer le pointeur de la fenetre de l'appareil désignée comme source via un signal émit par la dite fenêtre
/// \param sourceWindow
///
void CLASS_CONFIG_WINDOW::SetSourceWindow(CLASS_SOURCE_WINDOW *sourceWindow)
{
   f_SourceWindow = sourceWindow;
}

///
/// \fn GetReceiverWindow
/// \brief Permet de récupérer le pointeur de la fenetre de l'appareil désignée comme réceptrice via un signal émit par la dite fenêtre
/// \param receiverWindow
///
void CLASS_CONFIG_WINDOW::SetReceiverWindow(CLASS_RECEIVER_WINDOW *receiverWindow)
{
   f_ReceiverWindow = receiverWindow;
}

///
/// \fn OnStartButtonClicked
/// \brief Slot enclenché lors de l'appui sur le bouton "Start"
/// \param clicked : état du bouton
///
void CLASS_CONFIG_WINDOW::OnStartButtonClicked(void)
{
   // On réactive les signaux si ils sont bloqués
   f_SourceWindow->f_TestSource->blockSignals(false);
   f_ReceiverWindow->f_TestReceiver->blockSignals(false);

   // Lancement du test
   f_SourceWindow->StartSource();

   //f_ProgressBar->show();
   statusBar()->showMessage(QStringLiteral("Test en cours ..."));

   return;
}

///
/// \fn OnStopButtonClicked
/// \brief Slot enclenché lors de l'appui sur le bouton "Stop"
/// \param clicked : état du bouton
///
void CLASS_CONFIG_WINDOW::OnStopButtonClicked(void)
{
   f_SourceWindow->StopAll();

   // On bloque les signaux des threads pour empecher toute autre fonction de poursuivre
   f_SourceWindow->f_TestSource->blockSignals(true);
   f_ReceiverWindow->f_TestReceiver->blockSignals(true);

   statusBar()->showMessage(QStringLiteral("Test à l'arrêt"));

   return;
}

///
/// \fn WriteOutputIndic
/// \brief Permet d'afficher l'output testé
/// \param text
///
void CLASS_CONFIG_WINDOW::WriteOutputIndic(const QString &text)
{
   f_OutputIndic->setText(text);
   f_FormatIndic->clear();
   f_RateIndic->clear();
   f_InputIndic->clear();

   return;
}

///
/// \fn WriteFormatIndic
/// \brief Permet d'afficher le format appliqué sur l'output
/// \param text
///
void CLASS_CONFIG_WINDOW::WriteFormatIndic(const QString &text)
{
   f_FormatIndic->setText(text);
   f_RateIndic->clear();
   f_InputIndic->clear();

   return;
}

///
/// \fn WriteRateIndic
/// \brief Permet d'afficher le rate appliqué sur l'output et l'input
/// \param text
///
void CLASS_CONFIG_WINDOW::WriteRateIndic(const QString &text)
{
   f_RateIndic->setText(text);
   f_InputIndic->clear();

   return;
}

///
/// \fn WriteInputIndic
/// \brief Permet d'afficher l'input en cours de test
/// \param text
///
void CLASS_CONFIG_WINDOW::WriteInputIndic(const QString &text)
{
   f_InputIndic->setText(text);

   return;
}


///
/// \fn CompleteProgressBar
/// \brief Mets la progress Bar à 100% si ce n'est pas le cas à la fin du test
///
void CLASS_CONFIG_WINDOW::CompleteProgressBar(void)
{
   f_TimeValue = f_TotalTime;
   //f_ProgressBar->setValue(f_TimeValue);
   statusBar()->showMessage(QStringLiteral("Test terminé !"));

   // On bloque les signaux des threads pour empecher toute autre fonction de poursuivre
   f_SourceWindow->f_TestSource->blockSignals(true);
   f_ReceiverWindow->f_TestReceiver->blockSignals(true);
}

///
/// \fn CustomFormatSelectionned
/// \brief Indique si les format custom ont été sélectionnés
///
void CLASS_CONFIG_WINDOW::OnComboBoxModeSelectionned(int currentIndex)
{
   switch (currentIndex)
   {
      default:
         ExtCustomFormatSelected = false;
         ExtModeScrutationSelected = false;
         break;
      case 1:
         ExtCustomFormatSelected = true;
         ExtModeScrutationSelected = false;
         break;
      case 2:
         ExtCustomFormatSelected = false;
         ExtModeScrutationSelected = true;
         break;
   }

   return;
}

///
/// \brief CLASS_CONFIG_WINDOW::SetTimeValue
/// \param value
///
void CLASS_CONFIG_WINDOW::SetProgressBarTimeValue(int value)
{
   f_TimeValue += value;
   //f_ProgressBar->setValue(f_TimeValue);

   return;
}


void CLASS_CONFIG_WINDOW::OnEraseInterfaceButtonClicked(void)
{
    QList<QWidget *> AllComboBox = this->findChildren<QWidget *>(QStringLiteral("Erasable"));

    foreach (QWidget *combobox, AllComboBox)
    {
       delete combobox;
    }

    mesComboBoxInput.clear();
    mesComboBoxInput.squeeze();
    mesComboBoxOutput.clear();
    mesComboBoxOutput.squeeze();
    mesVerticalLayout.clear();
    mesVerticalLayout.squeeze();
    mesLayoutRx.clear();
    mesLayoutTx.clear();

    // Création du tableau de layout
    for(int i = 0; i < 500; ++i)
    {
       f_HorizontalRxLayout = new QHBoxLayout();
       f_HorizontalTxLayout = new QHBoxLayout();
       mesLayoutRx.append(f_HorizontalRxLayout);
       mesLayoutTx.append(f_HorizontalTxLayout);
    }

    // Création du tableau de stockage de vertical layout
    for(int i = 0; i < 100; ++i)
    {
       QVBoxLayout* VerticalLayout = new QVBoxLayout();
       mesVerticalLayout.append(VerticalLayout);
    }

    // Création du tableau de Combobox
    for(int i= 0; i < 300 ; ++i)
    {
       QString name;
       name.setNum(i);
       f_ComboboxInputPlug = new QComboBox();
       f_ComboboxInputPlug ->installEventFilter(this);
       f_ComboboxOutputPlug  = new QComboBox();
       f_ComboboxOutputPlug ->installEventFilter(this);
       f_ComboboxInputPlug ->setObjectName(name);
       f_ComboboxOutputPlug ->setObjectName(name);
       mesComboBoxInput.append(f_ComboboxInputPlug );
       mesComboBoxOutput.append(f_ComboboxOutputPlug );
    }


    QList<QGroupBox *> AllGroupBox = this->findChildren<QGroupBox *>();

    foreach(QGroupBox *groupBox, AllGroupBox)
    {
        mesGroupBox.removeOne(groupBox);
        delete groupBox;
    }

    mesGroupBox.clear();
    mesGroupBox.squeeze();

    // Création du tableau de GroupBox
    for(int i = 0; i < 25; ++i)
    {
       QGroupBox* GroupBox = new QGroupBox();
       GroupBox->setFixedHeight(400);
       GroupBox->setObjectName(QVariant(i).toString());
       GroupBox->setTitle(QStringLiteral("Group") % QString::number(i + 1));
       mesGroupBox.append(GroupBox);
    }


    // Réinitialisation des variables
    f_NumberOutput = 1;
    f_NumberGroupInput = 0;
    f_StockNumOutputAdded = 0;
    f_IndexComboBoxRx = 0;
    f_IndexComboBoxTx = 0;


}

/******************************************************************************
 *                              Méthodes protégées                            *
 *****************************************************************************/
///
/// \fn closeEvent
/// \brief OverRide du close event de la fenêtre (destruction de tous les éléments correctement)
/// \param closeEvent : évènement de fermeture
///
void CLASS_CONFIG_WINDOW::closeEvent(QCloseEvent* closeEvent)
{
   // Réponse de la message box
   QMessageBox::StandardButton Reply;

   // On initialise  la réponse de la message box
   Reply = QMessageBox::question(this, QStringLiteral("Configuration"), QStringLiteral("Are you sure ?"), QMessageBox::Yes|QMessageBox::No);

   // On agit en fonction de la réponse
   if (Reply == QMessageBox::Yes)
   {
      closeEvent->accept();
      QFile file(QStringLiteral("settingRxTx.ini"));
      file.remove();
      delete this;
   }
   else
      closeEvent->ignore();

   return;
}

///
/// \fn eventFilter
/// \brief Override du eventfilter pour les QObject
/// \param obj : QObject avec lequel interagit l'utilisateur
/// \param event : évènement dans la fenetre
///
bool CLASS_CONFIG_WINDOW::eventFilter(QObject *obj, QEvent *event)
{
   QString type;
   type = obj->metaObject()->className();
   // Si c'est une Combobox on ignore le signal de la mollette
   if(type == QLatin1String("QComboBox"))
   {
      if(event->type() == QEvent::Wheel)
      {
         event->ignore();
         return true;
      }
      else
         return false;
   }
   else
      return false;
}
/******************************************************************************
 *                             Operateur surcharge                            *
 *****************************************************************************/
