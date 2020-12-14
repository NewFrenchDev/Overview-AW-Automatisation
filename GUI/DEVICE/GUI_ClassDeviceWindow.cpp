///
/// \file GUI_ClassDeviceWindow.cpp
/// \brief Classe CLASS_DEVICE_WINDOW
/// \author ANALOG WAY - ADE
/// \date 2017-12-26
///
/// Fonctions de la classe CLASS_DEVICE_WINDOW
///

// C++ Standard
// Librairies Qt
#include <QCloseEvent>
#include <QMessageBox>
#include <QToolBar>
#include <QToolButton>
// Modules
#include "TRACES.h"
// Applicatif
#include "GUI_ClassDeviceWindow.h"

/******************************** DEFINITIONS ********************************/

/*----------------------------- Types publiques -----------------------------*/
/*--------------------------- Constantes publiques --------------------------*/
/*--------------------------- Variables publiques ---------------------------*/
/*------------------------------- Types privés ------------------------------*/
/*----------------------- Prototypes fonctions privées ----------------------*/
/*---------------------------- Constantes privées ---------------------------*/
// Textes affichés dans le menu
#define ACTION_MENU_CONNECTION         QStringLiteral("Connection")
#define ACTION_MENU_CONSOLE            QStringLiteral("Console")
#define ACTION_MENU_LOAD               QStringLiteral("Load")
#define ACTION_MENU_CONFIG             QStringLiteral("Configuration")
#define ACTION_MENU_VARIABLES          QStringLiteral("Variables")
#define ACTION_MENU_TEST               QStringLiteral("Source")
#define ACTION_MENU_RECEIVER           QStringLiteral("Receiver")

// Style à appliquer aux boutons ToolBar lorque la fenêtre associée est active
#define BUTTON_ACTION_ACTIVE_STYLE     QStringLiteral("{background-color:#FF4500}")

/*---------------------------- Variables privées ----------------------------*/

/**************************** FIN DES DEFINITIONS ****************************/

/******************************************************************************
 *                             Fonctions privées                              *
 *****************************************************************************/

/******************************************************************************
 *                         Constructeur / Destructeur                         *
 *****************************************************************************/

///
/// \fn CLASS_DEVICE_WINDOW(QObject *parent)
/// \brief Constructeur de CLASS_DEVICE_WINDOW
///
/// \param[in] parent : Objet parent
///
CLASS_DEVICE_WINDOW::CLASS_DEVICE_WINDOW(QWidget *parent) :
   QMainWindow(parent),
   f_MdiArea(nullptr),
   f_WindowBackGroundColor(Qt::blue),
   f_WindowToolBar(nullptr),
   f_SubWindowActionTable(),
   f_CommWindow(nullptr),
   f_ConfigDeviceWindow(nullptr),
   f_SourceWindow(nullptr),
   f_ReceiverWindow(nullptr)
{
   this->setWindowIcon(QIcon(QStringLiteral(":/PICTURES/myappico.ico")));

   // Modifie la couleur de fond
   this->ModifyBackgroundColor(f_WindowBackGroundColor);
   // Active la couleur de fond
   this->setAutoFillBackground(true);

   // Cré une barre d'outils
   f_WindowToolBar = new QToolBar(this);
   f_WindowToolBar->setFloatable(false);
   f_WindowToolBar->setMovable(false);
   this->addToolBar(f_WindowToolBar);

   // On crée les fenêtres
   f_CommWindow = new CLASS_COMMUNICATION_WINDOW(f_WindowBackGroundColor, this);
   f_ConfigDeviceWindow = new CLASS_DEV_CONFIG_WINDOW(f_WindowBackGroundColor, this);
   f_SourceWindow = new CLASS_SOURCE_WINDOW(f_WindowBackGroundColor, this);
   f_ReceiverWindow = new CLASS_RECEIVER_WINDOW(f_WindowBackGroundColor, this);

   // On cré une action par fenêtre
   f_SubWindowActionTable.insert(f_WindowToolBar->addAction(ACTION_MENU_CONNECTION), f_CommWindow);
   f_SubWindowActionTable.insert(f_WindowToolBar->addAction(ACTION_MENU_CONFIG), f_ConfigDeviceWindow);
   f_SubWindowActionTable.insert(f_WindowToolBar->addAction(ACTION_MENU_TEST), f_SourceWindow);
   f_SubWindowActionTable.insert(f_WindowToolBar->addAction(ACTION_MENU_RECEIVER), f_ReceiverWindow);

   // On nomme le widget associé à l'action pour pour pouvoir lui appliquer un style
   for (QAction *WindowAction : f_SubWindowActionTable.keys())
   {
      QWidget *WidgetAction(f_WindowToolBar->widgetForAction(WindowAction));
      if (WidgetAction == nullptr) continue;
      WidgetAction->setObjectName(WindowAction->text().simplified().remove(QStringLiteral(" ")));
   }

   // Cré la zone MDI et la place en tant que central widget
   f_MdiArea = new QMdiArea(this);
   this->setCentralWidget(f_MdiArea);

   // On les ajoute toute dans la MDI mais cachée
   for (QMdiSubWindow *SubWindow : f_SubWindowActionTable.values())
      f_MdiArea->addSubWindow(SubWindow)->hide();

   // On lie les actions de la toolbar à notre slot
   QObject::connect(f_WindowToolBar, &QToolBar::actionTriggered, this, &CLASS_DEVICE_WINDOW::ActionMenuTriggered);
   // On lie la demande de fermeture des fenêtres à notre slot
   QObject::connect(f_CommWindow, &CLASS_COMMUNICATION_WINDOW::SIGNAL_CloseRequested, this, &CLASS_DEVICE_WINDOW::SubWindowCloseRequest);
   QObject::connect(f_ConfigDeviceWindow, &CLASS_DEV_CONFIG_WINDOW::SIGNAL_CloseRequested, this, &CLASS_DEVICE_WINDOW::SubWindowCloseRequest);
   QObject::connect(f_SourceWindow, &CLASS_SOURCE_WINDOW::SIGNAL_CloseRequested, this, &CLASS_DEVICE_WINDOW::SubWindowCloseRequest);
   QObject::connect(f_SourceWindow, &CLASS_SOURCE_WINDOW::SIGNAL_IdentifiedSource, this, &CLASS_DEVICE_WINDOW::SIGNAL_MySourceWindow);
   QObject::connect(f_ReceiverWindow, &CLASS_RECEIVER_WINDOW::SIGNAL_CloseRequested, this, &CLASS_DEVICE_WINDOW::SubWindowCloseRequest);
   QObject::connect(f_ReceiverWindow, &CLASS_RECEIVER_WINDOW::SIGNAL_IdentifiedReceiver, this, &CLASS_DEVICE_WINDOW::SIGNAL_MyReceiverWindow);

   // On connecte les signaux de la fenêtre de configuration de la machine
   QObject::connect(f_ConfigDeviceWindow, &CLASS_DEV_CONFIG_WINDOW::SIGNAL_ChangeDeviceName, this, &CLASS_DEVICE_WINDOW::SetDisplayedWindowName);
   QObject::connect(f_ConfigDeviceWindow, &CLASS_DEV_CONFIG_WINDOW::SIGNAL_ChangeDeviceColor, this, &CLASS_DEVICE_WINDOW::SetWindowColor);
}

///
/// \fn ~CLASS_DEVICE_WINDOW()
/// \brief Destructeur de CLASS_DEVICE_WINDOW
///
CLASS_DEVICE_WINDOW::~CLASS_DEVICE_WINDOW()
{
   // On lie les actions de la toolbar à notre slot
   QObject::disconnect(f_WindowToolBar, &QToolBar::actionTriggered, this, &CLASS_DEVICE_WINDOW::ActionMenuTriggered);
   // On lie la demande de fermeture des fenêtres à notre slot
   QObject::disconnect(f_CommWindow, &CLASS_COMMUNICATION_WINDOW::SIGNAL_CloseRequested, this, &CLASS_DEVICE_WINDOW::SubWindowCloseRequest);
   QObject::disconnect(f_ConfigDeviceWindow, &CLASS_DEV_CONFIG_WINDOW::SIGNAL_CloseRequested, this, &CLASS_DEVICE_WINDOW::SubWindowCloseRequest);
   QObject::disconnect(f_SourceWindow, &CLASS_SOURCE_WINDOW::SIGNAL_CloseRequested, this, &CLASS_DEVICE_WINDOW::SubWindowCloseRequest);
   QObject::disconnect(f_SourceWindow, &CLASS_SOURCE_WINDOW::SIGNAL_IdentifiedSource, this, &CLASS_DEVICE_WINDOW::SIGNAL_MySourceWindow);
   QObject::disconnect(f_ReceiverWindow, &CLASS_RECEIVER_WINDOW::SIGNAL_CloseRequested, this, &CLASS_DEVICE_WINDOW::SubWindowCloseRequest);
   QObject::disconnect(f_ReceiverWindow, &CLASS_RECEIVER_WINDOW::SIGNAL_IdentifiedReceiver, this, &CLASS_DEVICE_WINDOW::SIGNAL_MyReceiverWindow);

   // On connecte les signaux de la fenêtre de configuration de la machine
   QObject::disconnect(f_ConfigDeviceWindow, &CLASS_DEV_CONFIG_WINDOW::SIGNAL_ChangeDeviceName, this, &CLASS_DEVICE_WINDOW::SetDisplayedWindowName);
   QObject::disconnect(f_ConfigDeviceWindow, &CLASS_DEV_CONFIG_WINDOW::SIGNAL_ChangeDeviceColor, this, &CLASS_DEVICE_WINDOW::SetWindowColor);

   f_CommWindow->deleteLater();
   f_ConfigDeviceWindow->deleteLater();
   f_SourceWindow->deleteLater();
   f_ReceiverWindow->deleteLater();
}

/******************************************************************************
 *                                 Accesseurs                                 *
 *****************************************************************************/
///
/// \fn GetCommWindowObj
/// \brief Renvoie l'objet représentant la fennêtre de communication
/// \return CLASS_COMMUNICATION_WINDOW * : Pointeur vers la fenêtre
///
CLASS_COMMUNICATION_WINDOW * CLASS_DEVICE_WINDOW::GetCommWindowObj(void) const
{
   return f_CommWindow;
}


///
/// \fn GetConfigWindowObj
/// \brief Renvoie l'objet représenant la fenêtre "configuration"
/// \return CLASS_DEV_CONFIG_WINDOW * : Pointeur vers la fenêtre
///
CLASS_DEV_CONFIG_WINDOW * CLASS_DEVICE_WINDOW::GetConfigWindowObj(void) const
{
   return f_ConfigDeviceWindow;
}



///
/// \fn GetTestWindowObj
/// \brief Renvoie l'objet représenant la fenêtre "test"
/// \return CLASS_TEST_WINDOW * : Pointeur vers la fenêtre
///
CLASS_SOURCE_WINDOW * CLASS_DEVICE_WINDOW::GetSourceWindowObj(void) const
{
   return f_SourceWindow;
}


///
/// \fn GetReceiverWindowObj
/// \brief Renvoie l'objet représenant la fenêtre "receiver"
/// \return CLASS_RECEIVER_WINDOW * : Pointeur vers la fenêtre
///
CLASS_RECEIVER_WINDOW * CLASS_DEVICE_WINDOW::GetReceiverWindowObj(void) const
{
   return f_ReceiverWindow;
}

/******************************************************************************
 *                                  Mutateurs                                 *
 *****************************************************************************/

///
/// \fn SetDisplayedWindowName
/// \brief Modifie le nom affiché dans la fenêtre
/// \param name : Nom à afficher
///
void CLASS_DEVICE_WINDOW::SetDisplayedWindowName(const QString& name)
{
   this->setWindowTitle(name);
}

///
/// \fn SetWindowColor
/// \brief Définit la couleur de la fenêtre
/// \param windowColor : Couleur de la fenêtre
///
void CLASS_DEVICE_WINDOW::SetWindowColor(const QColor& windowColor)
{
   f_WindowBackGroundColor = windowColor;

   // Application pour cet élément
   this->ModifyBackgroundColor(windowColor);

   // On change ensuite les couleurs des sous fenêtres de la machines
   f_CommWindow->ModifyBackGroundColor(windowColor);
   f_ConfigDeviceWindow->ModifyBackGroundColor(windowColor);
   f_SourceWindow->ModifyBackGroundColor(windowColor);
   f_ReceiverWindow->ModifyBackGroundColor(windowColor);
}

///
/// \fn SetNewStyleSheet
/// \brief Met en place une nouvelle feuille de style
/// \param StyleSheetContent : contenu de la feuille de style
///
void CLASS_DEVICE_WINDOW::SetNewStyleSheet(const QString &StyleSheetContent)
{
   // On met ne place la nouvelle style sheet sur la machine window
   this->setStyleSheet(StyleSheetContent);

   // On change ensutie les style sheet des sous fenêtres de la machines
   f_CommWindow->setStyleSheet(StyleSheetContent);
   f_ConfigDeviceWindow->setStyleSheet(StyleSheetContent);
   f_SourceWindow->setStyleSheet(StyleSheetContent);
   f_ReceiverWindow->setStyleSheet(StyleSheetContent);
}

/******************************************************************************
 *                             Méthodes publiques                             *
 *****************************************************************************/

/******************************************************************************
 *                               Slots publiques                              *
 *****************************************************************************/

/******************************************************************************
 *                              Méthodes privées                              *
 *****************************************************************************/

///
/// \fn ActionMenuTriggered
/// \brief Evènement sur le champ du menu
/// \param triggerAction : Action déclenchée
///
void CLASS_DEVICE_WINDOW::ActionMenuTriggered(QAction *triggerAction)
{
    QMdiSubWindow *SubWindow(f_SubWindowActionTable.value(triggerAction));

    // On ne fait rien si aucune fenêtre liée à l'action
    if (SubWindow == nullptr)
        return;

    // On inverse la visibilité de la fenêtre
    Bool WindowHasToBeVisible(!SubWindow->isVisible());

    if(SubWindow == f_ReceiverWindow && f_SourceWindow->isVisible())
    {
        SubWindow->setVisible(WindowHasToBeVisible);
        f_SourceWindow->setVisible(!WindowHasToBeVisible);
        QWidget *WidgetAction(f_WindowToolBar->widgetForAction(f_SubWindowActionTable.key(f_SourceWindow)));
        // Supprime le style actif du bouton
        QString WidgetStyleSheet(WidgetAction->styleSheet());
        QString ActiveWidgetStyle(QStringLiteral("QWidget#") % WidgetAction->objectName() % BUTTON_ACTION_ACTIVE_STYLE);
        WidgetStyleSheet.remove(ActiveWidgetStyle);
        WidgetAction->setStyleSheet(WidgetStyleSheet);
    }
    else if(SubWindow == f_SourceWindow && f_ReceiverWindow->isVisible())
    {
        SubWindow->setVisible(WindowHasToBeVisible);
        f_ReceiverWindow->setVisible(!WindowHasToBeVisible);
        QWidget *WidgetAction(f_WindowToolBar->widgetForAction(f_SubWindowActionTable.key(f_ReceiverWindow)));
        // Supprime le style actif du bouton
        QString WidgetStyleSheet(WidgetAction->styleSheet());
        QString ActiveWidgetStyle(QStringLiteral("QWidget#") % WidgetAction->objectName() % BUTTON_ACTION_ACTIVE_STYLE);
        WidgetStyleSheet.remove(ActiveWidgetStyle);
        WidgetAction->setStyleSheet(WidgetStyleSheet);
    }
    else
        SubWindow->setVisible(WindowHasToBeVisible);

    // On force une couleur de fond sur le bouton action si la fenêtre va être active
    QWidget *WidgetAction(f_WindowToolBar->widgetForAction(triggerAction));
    // Pas de widgets sur cet action, on sort
    if (WidgetAction == nullptr)
        return;

    // Application de la couleur de fond via la feuille de style
    QString WidgetStyleSheet(WidgetAction->styleSheet());
    QString ActiveWidgetStyle(QStringLiteral("QWidget#") % WidgetAction->objectName() % BUTTON_ACTION_ACTIVE_STYLE);
    WidgetStyleSheet.remove(ActiveWidgetStyle);
    if (WindowHasToBeVisible == true) WidgetStyleSheet.append(ActiveWidgetStyle);
    WidgetAction->setStyleSheet(WidgetStyleSheet);
}

///
/// \fn SubWindowCloseRequest
/// \brief Demande de fermeture d'une fenêtre
/// \param subWindowObj : Objet représentant la sous fenêtre
///
void CLASS_DEVICE_WINDOW::SubWindowCloseRequest(QMdiSubWindow *subWindowObj)
{
    QAction *WindowAction(f_SubWindowActionTable.key(subWindowObj));

    // On ne fait rien si aucune action liée à la fenêtre
    if (WindowAction == nullptr)
        return;

    // Cache la fenêtre
    subWindowObj->hide();

    // On force une couleur de fond sur le bouton action si la fenêtre va être active
    QWidget *WidgetAction(f_WindowToolBar->widgetForAction(WindowAction));
    // Pas de widgets sur cet action, on sort
    if (WidgetAction == nullptr)
        return;

    // Supprime le style actif du bouton
    QString WidgetStyleSheet(WidgetAction->styleSheet());
    QString ActiveWidgetStyle(QStringLiteral("QWidget#") % WidgetAction->objectName() % BUTTON_ACTION_ACTIVE_STYLE);
    WidgetStyleSheet.remove(ActiveWidgetStyle);
    WidgetAction->setStyleSheet(WidgetStyleSheet);
}


///
/// \fn ModifyBackgroundColor
/// \brief Modifications de la couleur de fond
/// \param backColor : Couleur de fond
///
void CLASS_DEVICE_WINDOW::ModifyBackgroundColor(const QColor& backColor)
{
    // On copie la palette courante
    QPalette Palette(this->palette());
    // On modifie juste la couleur de fond
    Palette.setColor(QPalette::Background, backColor);
    // Puis on applique
    this->setPalette(Palette);
}

///
/// \fn closeEvent
/// \brief Override de la méthode closeEvent
/// \param closeEvent : evenement de fermeture
///
void CLASS_DEVICE_WINDOW::closeEvent (QCloseEvent * closeEvent)
{
    // Réponse de la message box
    QMessageBox::StandardButton Reply;

    // On initialise  la réponse de la message box
    Reply = QMessageBox::question(this, QStringLiteral("Machine"), QStringLiteral("Are you sure ?"), QMessageBox::Yes|QMessageBox::No);

    // On agit en fonction de la réponse
    if (Reply == QMessageBox::Yes)
    {
        closeEvent->accept();
        delete this;
    }
    else
        closeEvent->ignore();
}
/******************************************************************************
 *                             Operateur surcharge                            *
 *****************************************************************************/
