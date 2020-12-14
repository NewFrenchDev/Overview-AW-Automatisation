///
/// \file GUI_ClassDeviceWindow.h
/// \brief Classe CLASS_DEVICE_WINDOW
/// \author ANALOG WAY - ADE
/// \date 2017-12-26
///

#ifndef CLASSMACHINEWINDOW_H
#define CLASSMACHINEWINDOW_H

// C++ Standard
// Librairies Qt
#include <QAction>
#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QToolButton>
#include <QToolBar>
// Modules
#include "COMMON_TypeDefs.h"
// Applicatif
#include "GUI_ClassCommWindow.h"
#include "GUI_ClassDevConsoleWindow.h"
#include "GUI_ClassDevConfigWindow.h"
#include "GUI_ClassSourceWindow.h"
#include "GUI_ClassReceiverWindow.h"


///
/// \class CLASS_DEVICE_WINDOW
/// \brief Classe CLASS_DEVICE_WINDOW
///
class CLASS_DEVICE_WINDOW : public QMainWindow
{
      Q_OBJECT

   public:

      /*--------------------------- Types publiques ---------------------------*/

      /*------------------------- Constantes publiques ------------------------*/

      /*------------------------- Variables publiques -------------------------*/

      /*---------------------- Constructeur / Destructeur ---------------------*/

      ///
      /// \fn CLASS_DEVICE_WINDOW(QObject *parent)
      /// \brief Constructeur de CLASS_DEVICE_WINDOW
      ///
      /// \param[in] parent : Objet parent
      ///
      explicit CLASS_DEVICE_WINDOW(QWidget *parent = nullptr);

      ///
      /// \fn ~CLASS_DEVICE_WINDOW()
      /// \brief Destructeur de CLASS_DEVICE_WINDOW
      ///
      ~CLASS_DEVICE_WINDOW();

      /*------------------------------ Accesseurs -----------------------------*/

      ///
      /// \fn GetCommWindowObj
      /// \brief Renvoie l'objet représentant la fenêtre de communication
      /// \return CLASS_COMMUNICATION_WINDOW * : Pointeur vers la fenêtre
      ///
      CLASS_COMMUNICATION_WINDOW * GetCommWindowObj(void) const;

      ///
      /// \fn GetConfigWindowObj
      /// \brief Renvoie l'objet représenant la fenêtre "configuration"
      /// \return CLASS_DEV_CONFIG_WINDOW * : Pointeur vers la fenêtre
      ///
      CLASS_DEV_CONFIG_WINDOW * GetConfigWindowObj(void) const;


      ///
      /// \fn GetTestWindowObj
      /// \brief Renvoie l'objet représenant la fenêtre "test"
      /// \return CLASS_TEST_WINDOW * : Pointeur vers la fenêtre
      ///
      CLASS_SOURCE_WINDOW * GetSourceWindowObj(void) const;


      ///
      /// \fn GetReceiverWindowObj
      /// \brief Renvoie l'objet représenant la fenêtre "receiver"
      /// \return CLASS_RECEIVER_WINDOW * : Pointeur vers la fenêtre
      ///
      CLASS_RECEIVER_WINDOW * GetReceiverWindowObj(void) const;

      /*------------------------------ Mutateurs ------------------------------*/

      ///
      /// \fn SetDisplayedWindowName
      /// \brief Modifie le nom affiché dans la fenêtre
      /// \param name : Nom à afficher
      ///
      void SetDisplayedWindowName(const QString& name);

      ///
      /// \fn SetWindowColor
      /// \brief Définit la couleur de la fenêtre
      /// \param windowColor
      ///
      void SetWindowColor(const QColor& windowColor);

      ///
      /// \fn SetNewStyleSheet
      /// \brief Met en place une nouvelle feuille de style
      /// \param StyleSheetContent : contenu de la feuille de style
      ///
      void SetNewStyleSheet(const QString &StyleSheetContent);


      /*-------------------------- Méthodes publiques -------------------------*/

      /*--------------------------- Signaux publiques -------------------------*/

   signals:

      ///
      /// \fn SIGNAL_MySourceWindow
      /// \brief Permet de transmettre le pointeur de la fenetre de l'appareil source à la fenêtre de configuration
      /// \param sourceWindow : pointeur vers la fenetre de la machine source
      ///
      void SIGNAL_MySourceWindow(CLASS_SOURCE_WINDOW *sourceWindow);

      ///
      /// \fn SIGNAL_MyReceiverWindow
      /// \brief Permet de transmettre le pointeur de la fenetre de l'appareil récepteur à la fenêtre de configuration
      /// \param receiverWindow : pointeur vers la fenetre de la machine réceptrice
      ///
      void SIGNAL_MyReceiverWindow(CLASS_RECEIVER_WINDOW *receiverWindow);


      /*---------------------------- Slots publiques --------------------------*/

      /*----------------------------- Types privés ----------------------------*/

      /*--------------------------- Méthodes privées --------------------------*/
   private:

      ///
      /// \fn ActionMenuTriggered
      /// \brief Evènement sur le champ du menu
      /// \param triggerAction : Action déclenchée
      ///
      void ActionMenuTriggered(QAction *triggerAction);

      ///
      /// \fn SubWindowCloseRequest
      /// \brief Demande de fermeture d'une fenêtre
      /// \param subWindowObj : Objet représentant la sous fenêtre
      ///
      void SubWindowCloseRequest(QMdiSubWindow *subWindowObj);

      ///
      /// \fn ModifyBackgroundColor
      /// \brief Modifications de la couleur de fond
      /// \param backColor : Couleur de fond
      ///
      void ModifyBackgroundColor(const QColor& backColor);

   protected:

      ///
      /// \fn closeEvent
      /// \brief Override de la méthode closeEvent
      /// \param closeEvent : evenement de fermeture
      ///
      void closeEvent (QCloseEvent* closeEvent);

   private:

       /*------------------------------ Variables ------------------------------*/

   private:

      QMdiArea *f_MdiArea;                                     ///< Zone d'acceuil des fenêtres liées à la machine
      QColor f_WindowBackGroundColor;                          ///< Couleur qui sera en background de la fenêtre
      QToolBar *f_WindowToolBar;                               ///< Toolbar regroupant les boutons actions
      QMap<QAction *, QMdiSubWindow *> f_SubWindowActionTable; ///< Table liant le bouton action à sa fenêtre
      CLASS_COMMUNICATION_WINDOW *f_CommWindow;                ///< Fenêtre d'interface graphique pour les connexion
      CLASS_DEV_CONFIG_WINDOW *f_ConfigDeviceWindow;           ///< Fenêtre d'interface graphique qui permet de modifier des caractéristique de la machine
      CLASS_SOURCE_WINDOW *f_SourceWindow;                     ///< Fenêtre d'interface graphique qui envoie et affiche les commandes envoyées et reçues.
      CLASS_RECEIVER_WINDOW *f_ReceiverWindow;                 ///< Fenêtre d'interface graphique qui envoie et affiche les commandes envoyées et reçues.

};

#endif // CLASSMACHINEWINDOW_H
