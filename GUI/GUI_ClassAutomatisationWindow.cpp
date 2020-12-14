///
/// \file GUI_ClassAUTOMATISATIONWindow.cpp
/// \brief Classe CLASS_AUTOMATISATION_WINDOW
/// \author ANALOG WAY - ADE
/// \date 2017-12-26
///
/// Fonctions de la classe CLASS_AUTOMATISATION_WINDOW
///

// C++ Standard
// Librairies Qt
#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QColor>
#include <QList>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
// Modules
#include "TRACES.h"
// Application
#include "GUI_ClassAutomatisationWindow.h"

/******************************** DEFINITIONS ********************************/

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
/// \fn CLASS_AUTOMATISATION_WINDOW(QObject *parent)
/// \brief Constructeur de CLASS_AUTOMATISATION_WINDOW
///
/// \param[in] parent : Objet parent
///
CLASS_AUTOMATISATION_WINDOW::CLASS_AUTOMATISATION_WINDOW(QWidget *parent) :
   QMainWindow(parent),
   f_FileMenu(nullptr),
   f_LoadMenu(nullptr),
   f_AboutMenu(nullptr),
   f_NewConfigAction(nullptr),
   f_LoadStyleSheetAction(nullptr),
   f_AboutAutomatisationAction(nullptr)
{
   // On met en place l'interface graphique
   this->SetUpUserIntertace();

   // On connecte le signal de création de nouvelle machine
   QObject::connect(f_NewConfigAction, &QAction::triggered, this, &CLASS_AUTOMATISATION_WINDOW::SIGNAL_CreateConfigRequest);

  // On connecte le signal pour le chargement d'une style sheet
   QObject::connect(f_LoadStyleSheetAction, &QAction::triggered, this, &CLASS_AUTOMATISATION_WINDOW::SLOT_LoadStyleSheetActionTriggered);

   QObject::connect(f_AboutAutomatisationAction, &QAction::triggered, this, &CLASS_AUTOMATISATION_WINDOW::SLOT_AboutQtActionTriggered);
}

///
/// \fn ~CLASS_AUTOMATISATION_WINDOW()
/// \brief Destructeur de CLASS_AUTOMATISATION_WINDOW
///
CLASS_AUTOMATISATION_WINDOW::~CLASS_AUTOMATISATION_WINDOW()
{
   delete QThread::currentThread();
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


///
/// \fn CreateNewConfigWindow
/// \brief Instancie une nouvelle fenêtre config
/// \return CLASS_CONFIG_WINDOW * : Objet représentant la fenêtre
///
CLASS_CONFIG_WINDOW * CLASS_AUTOMATISATION_WINDOW::CreateNewConfigWindow(void)
{
   //Création d'une fenêtre config vide
   CLASS_CONFIG_WINDOW *NewConfigWindow = new CLASS_CONFIG_WINDOW();

   // On l'ajoute à la liste
   f_ConfigWindowList.append(NewConfigWindow);

   return NewConfigWindow;
}

///
/// \fn DeleteConfigWindow
/// \brief Instancie une nouvelle fenêtre config
/// \param machineWindowObj : Objet représentant la fenêtre config
///
void CLASS_AUTOMATISATION_WINDOW::DeleteConfigWindow(CLASS_CONFIG_WINDOW *configWindowObj)
{
   // Rien à faire si la fenêtre n'existe pas
   if (f_ConfigWindowList.contains(configWindowObj) == false)
      return;

   // Retire la fenêtre de la liste
   f_ConfigWindowList.removeAll(configWindowObj);
}

/******************************************************************************
 *                               Slots publiques                              *
 *****************************************************************************/

///
/// \fn SLOT_LoadStyleSheetActionTriggered
/// \brief Slot enclenché lors de l'action load styule sheet
///
void CLASS_AUTOMATISATION_WINDOW::SLOT_LoadStyleSheetActionTriggered(void)
{
   QString FileSheetName(QFileDialog::getOpenFileName( this,  QStringLiteral("Load Style Sheet") , QStringLiteral( "/home/" ),  QStringLiteral("CSS (*.css)")));
   emit this->SIGNAL_LoadStyleSheet(FileSheetName);
}


void CLASS_AUTOMATISATION_WINDOW::SLOT_AboutQtActionTriggered(void)
{
   QMessageBox::aboutQt(nullptr, QStringLiteral("Information"));
}

/******************************************************************************
 *                              Méthodes privées                              *
 *****************************************************************************/

///
/// \fn SetUpUserIntertace
/// \brief Met en place l'interface graphique de la fenêtre
///
void CLASS_AUTOMATISATION_WINDOW::SetUpUserIntertace(void)
{
   this->setWindowIcon(QIcon(QStringLiteral(":/PICTURES/myappico.ico")));

   // On change le titre de la fenêtre
   this->setWindowTitle(QStringLiteral("Automatisation"));

   // Définit la taille idéale/minimale pour la fenêtre
   this->setMinimumSize(QSize(800, 950));

   // Ajout d'une image en arrière plan
   QPixmap bkground(QStringLiteral(":/PICTURES/TestBackground.jpg"));
   bkground = bkground.scaled(this->size(), Qt::IgnoreAspectRatio);
   QPalette palette;
   palette.setBrush(QPalette::Background, bkground);
   this->setPalette(palette);

   // On instancie les différents menus
   // "File"
   f_FileMenu = new QMenu(QStringLiteral("New"), this);
   f_NewConfigAction = new QAction(QStringLiteral("Add New Configuration"), this);
   f_FileMenu->addAction(f_NewConfigAction);
   f_NewConfigAction->setIcon(QIcon(":/PICTURES/add.png"));
   // "Load"
   f_LoadMenu = new QMenu(QStringLiteral("Load"), this);
   f_LoadStyleSheetAction = new QAction(QStringLiteral("Load Style Sheet"), this);
   f_LoadMenu->addAction(f_LoadStyleSheetAction);
   f_LoadStyleSheetAction->setIcon(QIcon(":/PICTURES/up_icon.ico"));
   // "About"
   f_AboutMenu = new QMenu(QStringLiteral("About"), this);
   f_AboutAutomatisationAction = new QAction(QStringLiteral("About Qt"), this);
   f_AboutMenu->addAction(f_AboutAutomatisationAction);
   f_AboutAutomatisationAction->setIcon(QIcon(":/PICTURES/qt_icon.png"));

   // Cré la barre de menu
   this->setMenuBar(new QMenuBar(this));
   this->menuBar()->addMenu(f_FileMenu);
   this->menuBar()->addMenu(f_LoadMenu);
   this->menuBar()->addMenu(f_AboutMenu);
}

/******************************************************************************
 *                              Méthodes protégées                            *
 *****************************************************************************/

///
/// \fn closeEvent
/// \brief OverRide du close event de la fenêtre (destruction de tous les éléments correctement)
/// \param closeEvent : évènement de fermeture
///
void CLASS_AUTOMATISATION_WINDOW::closeEvent(QCloseEvent* closeEvent)
{
   // On enregistre les dimensions de la fenêtre
   //CLASS_AUTOMATISATION_CONF::AddKeyInGroup(CONF_WINDOW_GEOMETRY_GROUP, CONF_WINDOW_GEOMETRY, static_cast<QVariant>(this->geometry()));
   QCoreApplication::quit();
   closeEvent->accept();
}

/******************************************************************************
 *                             Operateur surcharge                            *
 *****************************************************************************/
