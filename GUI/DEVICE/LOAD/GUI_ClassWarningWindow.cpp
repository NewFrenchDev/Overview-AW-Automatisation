///
/// \file GUI_ClassParsingWarningWindow.cpp
/// \brief Classe CLASS_PARSING_WARNING_WINDOW
/// \author ANALOG WAY - ADE
/// \date 2018-02-09
///
/// Fonctions de la classe CLASS_PARSING_WARNING_WINDOW
///

// C++ Standard
// Librairies Qt
// Modules
#include "GUI_ClassWarningWindow.h"

/******************************** DEFINITIONS ********************************/

// Titre de la fenêtre
#define WINDOW_NAME                QStringLiteral("Parsing Warning")

// Taille par défaut de la fenêtre
#define WINDOW_DEFAULT_WIDTH       900
#define WINDOW_DEFAULT_HEIGHT      350

// Taille minimale de la fenêtre
#define WINDOW_MIN_WIDTH           470
#define WINDOW_MIN_HEIGHT          150

// Nom des boutons
#define BUTTON_OK_NAME             QStringLiteral("OK")

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
/// \fn CLASS_PARSING_WARNING_WINDOW(QObject *parent)
/// \brief Constructeur de CLASS_PARSING_WARNING_WINDOW
///
/// \param[in] parent : Objet parent
///
CLASS_WARNING_WINDOW::CLASS_WARNING_WINDOW(const QString& errorsToDisplay ,QWidget *parent):
   QWidget(parent),
   f_WindowWidgetLayout(nullptr),
   f_ErrorsDisplay(nullptr),
   f_OkButton(nullptr)
{
   // On met en place l'interface graphique
   this->SetUpUserInterface();

   // On connecte le bouton a son slot de fermeture
   QObject::connect(f_OkButton, &QPushButton::clicked, this, &CLASS_WARNING_WINDOW::SLOT_OnOkButtonClicked);

   // On met les erreurs dans la fenêtre
   f_ErrorsDisplay->document()->setPlainText(errorsToDisplay);
}

///
/// \fn ~CLASS_PARSING_WARNING_WINDOW()
/// \brief Destructeur de CLASS_PARSING_WARNING_WINDOW
///
CLASS_WARNING_WINDOW::~CLASS_WARNING_WINDOW()
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
/// \fn SLOT_OnOkButtonClicked
/// \brief Slot enclencher lors de l'appuis sur le bouton Ok
///
void CLASS_WARNING_WINDOW::SLOT_OnOkButtonClicked(void)
{
   // On ferme la fenêtre
   this->close();
}

/******************************************************************************
 *                              Méthodes privées                              *
 *****************************************************************************/

///
/// \fn SetUpUserInterface
/// \brief Met en place l'interface graphique
///
void CLASS_WARNING_WINDOW::SetUpUserInterface(void)
{
   // On change le nom de la fenêtre
   this->setWindowTitle(WINDOW_NAME);

   this->resize(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT);
   // On resize avec les mesures par défaut
   this->setMinimumSize(QSize(WINDOW_MIN_WIDTH, WINDOW_MIN_HEIGHT));
   // Le fenêtre essaye de se mettre à la taille par défaut
   this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

   // On set le layout principal
   f_WindowWidgetLayout = new QVBoxLayout();
   f_WindowWidgetLayout->setContentsMargins(0, 0, 0, 0);
   f_WindowWidgetLayout->setSpacing(0);
   this->setLayout(f_WindowWidgetLayout);

   // On met en place le plain text edit
   f_ErrorsDisplay = new QPlainTextEdit(this);
   f_ErrorsDisplay->setReadOnly(true);
   f_WindowWidgetLayout->addWidget(f_ErrorsDisplay);

   // On met en place le bouton OK
   f_OkButton = new QPushButton(BUTTON_OK_NAME, this);
   f_WindowWidgetLayout->addWidget(f_OkButton);

   // On affiche la fenêtre
   this->show();
}


/******************************************************************************
 *                             Operateur surcharge                            *
 *****************************************************************************/
