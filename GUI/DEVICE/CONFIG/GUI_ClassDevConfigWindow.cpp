///
/// \file GUI_ClassDeviceConfigWindow.cpp
/// \brief Classe CLASS_DEVICE_CONFIG_WINDOW
/// \author ANALOG WAY - ADE
/// \date 2018-01-25
///
/// Fonctions de la classe CLASS_DEVICE_CONFIG_WINDOW
///

// C++ Standard
// Librairies Qt
#include <QMessageBox>
// Modules
#include "GUI_ClassDevConfigWindow.h"

/******************************** DEFINITIONS ********************************/

// Nom de la fenêtre
#define  WINDOW_NAME                   QStringLiteral("Configuration Window")

// Taille par défaut de la fenêtre
#define  WINDOW_DEFAULT_WIDTH          500
#define  WINDOW_DEFAULT_HEIGHT         500

// Taille par minimale de la fenêtre
#define  WINDOW_MIN_WIDTH              350
#define  WINDOW_MIN_HEIGHT             350

// Nom des boutons
#define  BUTTON_CHANGE_DEVNAME_NAME    QStringLiteral("Change Name")
#define  BUTTON_CHANGE_COLOR_NAME      QStringLiteral("Change Color")

/*----------------------------- Types publiques -----------------------------*/
/*--------------------------- Constantes publiques --------------------------*/
/*--------------------------- Variables publiques ---------------------------*/
/*------------------------------- Types privés ------------------------------*/
/*----------------------- Prototypes fonctions privées ----------------------*/
/*---------------------------- Constantes privées ---------------------------*/
/*---------------------------- Variables privées ----------------------------*/

/**************************** FIN DES DEFINITIONS ****************************/

/******************************************************************************
 *                             Fonctions privées                              *
 *****************************************************************************/

/******************************************************************************
 *                         Constructeur / Destructeur                         *
 *****************************************************************************/

///
/// \fn CLASS_DEVICE_CONFIG_WINDOW(QObject *parent)
/// \brief Constructeur de CLASS_DEVICE_CONFIG_WINDOW
///
/// \param[in] parent : Objet parent
///
CLASS_DEV_CONFIG_WINDOW::CLASS_DEV_CONFIG_WINDOW(const QColor &windowColor, QWidget *parent):
   CLASS_ABSTRACT_MDI_SUBWINDOW(windowColor, parent),
   f_WindowWidgetLayout(nullptr),
   f_ParametersLayout(nullptr),
   f_DeviceNameEdit(nullptr),
   f_ColorDialog(nullptr),
   f_Line(nullptr)
{
   // On met en place l'interface graphique
   this->SetUpUserInterface();

   // On connecte les bouton à leurs slots respectifs
   QObject::connect(f_ChangeNameButton, &QPushButton::clicked, this, &CLASS_DEV_CONFIG_WINDOW::SLOT_OnChangeNameButtonClicked);
   QObject::connect(f_ChangeColorButton, &QPushButton::clicked, this, &CLASS_DEV_CONFIG_WINDOW::SLOT_OnChangeColorButtonClicked);
}

///
/// \fn ~CLASS_DEVICE_CONFIG_WINDOW()
/// \brief Destructeur de CLASS_DEVICE_CONFIG_WINDOW
///
CLASS_DEV_CONFIG_WINDOW::~CLASS_DEV_CONFIG_WINDOW()
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
/// \fn SLOT_OnChangeNameButtonClicked
/// \brief Slot enclenché lors de l'appui sur le bouton de validation
///
void CLASS_DEV_CONFIG_WINDOW::SLOT_OnChangeNameButtonClicked(void)
{
   // Si le champs n'est pas vide
   if (f_DeviceNameEdit->text().isEmpty() == false)
      // On emet un signal pour modifier le nom de la machine
      emit this->SIGNAL_ChangeDeviceName(f_DeviceNameEdit->text());

   // Le champs est vide, on ne change pas le nom de la machine
   else
      QMessageBox::information(this, WINDOW_NAME, QStringLiteral("You have not enter a name..."));
}

///
/// \fn SLOT_OnChangeColorButtonClicked
/// \brief Slot enclenché lors de l'appui sur le bouton de validation
///
void CLASS_DEV_CONFIG_WINDOW::SLOT_OnChangeColorButtonClicked(void)
{
   // Lors de l'appui sur le bouton on emet un signal pour changer la couleur
   emit this->SIGNAL_ChangeDeviceColor(f_ColorDialog->currentColor());
}

/******************************************************************************
 *                              Méthodes privées                              *
 *****************************************************************************/

///
/// \fn SetUpUserInterface
/// \brief Met en place l'interface graphique de la fenêtre
///
void CLASS_DEV_CONFIG_WINDOW::SetUpUserInterface(void)
{

   // On set le nom de la fenêtre
   this->setWindowTitle(WINDOW_NAME);

   // Définit la taille idéale/minimale pour la fenêtre
   this->SetSizeHint(QSize(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT));
   this->setMinimumSize(QSize(WINDOW_MIN_WIDTH, WINDOW_MIN_HEIGHT));
   // Le fenêtre essaye de se mettre à la taille par défaut
   this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

   // On set le layout principale de la fenêtre
   f_WindowWidgetLayout = new QVBoxLayout();
   f_WindowWidgetLayout->setContentsMargins(0, 0, 0, 0);
   f_WindowWidgetLayout->setSpacing(0);

   // Associe ce layout à la fenêtre MDI via un QWidget intermédiaire
   this->setWidget(new QWidget(this));
   this->widget()->setLayout(f_WindowWidgetLayout);

   // On set le grid layout pour les paramètres
   f_ParametersLayout = new QGridLayout();

   // On set le line edit pour le nom de la machine
   f_DeviceNameEdit = new QLineEdit(this);
   f_ParametersLayout->addWidget(f_DeviceNameEdit, 0, 0, 1, 1);
   // On set le bouton pour changer le nom de la machine
   f_ChangeNameButton = new QPushButton(BUTTON_CHANGE_DEVNAME_NAME, this);
   f_ParametersLayout->addWidget(f_ChangeNameButton, 0, 1, 1, 1);
   f_WindowWidgetLayout->addLayout(f_ParametersLayout);

   // On met en place la ligne qui sépare les paramètres
   f_Line = new QFrame(this);
   f_Line->setFrameShape(QFrame::HLine);
   f_Line->setFrameShadow(QFrame::Sunken);
   f_WindowWidgetLayout->addWidget(f_Line);

   // On set le color dialog pour le choix de la couleur de la fenêtre
   f_ColorDialog = new QColorDialog(this);
   // On paramètre le Color Dialog
   f_ColorDialog->setWindowFlags(Qt::Widget);
   // On n'utilise pas le dialogue natif, et on enlève le "OK" Button
   f_ColorDialog->setOptions(QColorDialog::DontUseNativeDialog|QColorDialog::NoButtons);
   f_WindowWidgetLayout->addWidget(f_ColorDialog);
   // On set le bouton pour changer de couleur
   f_ChangeColorButton = new QPushButton(BUTTON_CHANGE_COLOR_NAME, this);
   f_WindowWidgetLayout->addWidget(f_ChangeColorButton);

   f_WindowWidgetLayout->setStretch(1, 1);
}

/******************************************************************************
 *                             Operateur surcharge                            *
 *****************************************************************************/
