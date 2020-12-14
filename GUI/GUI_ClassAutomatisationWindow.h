///
/// \file GUI_ClassAUTOMATISATIONWindow.h
/// \brief Classe CLASS_AUTOMATISATION_WINDOW
/// \author ANALOG WAY - ADE
/// \date 2017-12-26
///
/// Fonctions de la classe CLASS_AUTOMATISATION_WINDOW
///

#ifndef GUI_CLASS_AUTOMATISATION_WINDOW_H
#define GUI_CLASS_AUTOMATISATION_WINDOW_H

// C++ Standard
// Librairies Qt
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QWidget>
#include <QMdiArea>
#include <QVBoxLayout>
#include <QBrush>
#include <QPixmap>
#include <QMdiSubWindow>
// Modules
#include "COMMON_TypeDefs.h"
// Applicatif
//#include "GUI_ClassDeviceWindow.h"
#include "GUI_ClassConfigWindow.h"
#include "GUI_ClassAutomatisationWindow.h"

///
/// \class CLASS_AUTOMATISATION_WINDOW
/// \brief Classe CLASS_AUTOMATISATION_WINDOW
///
class CLASS_AUTOMATISATION_WINDOW : public QMainWindow
{
      Q_OBJECT

   public:

      /*--------------------------- Types publiques ---------------------------*/

      /*------------------------- Constantes publiques ------------------------*/

      /*------------------------- Variables publiques -------------------------*/

      /*---------------------- Constructeur / Destructeur ---------------------*/

      ///
      /// \fn CLASS_AUTOMATISATION_WINDOW(QObject *parent)
      /// \brief Constructeur de CLASS_AUTOMATISATION_WINDOW
      ///
      /// \param[in] parent : Objet parent
      ///
      explicit CLASS_AUTOMATISATION_WINDOW(QWidget *parent = nullptr);

      ///
      /// \fn ~CLASS_AUTOMATISATION_WINDOW()
      /// \brief Destructeur de CLASS_AUTOMATISATION_WINDOW
      ///
      ~CLASS_AUTOMATISATION_WINDOW();

      /*------------------------------ Accesseurs -----------------------------*/

      /*------------------------------ Mutateurs ------------------------------*/

      /*-------------------------- Méthodes publiques -------------------------*/

   public :

      ///
      /// \fn CreateNewDeviceWindow
      /// \brief Instancie une nouvelle fenêtre machine
      /// \return CLASS_DEVICE_WINDOW * : Objet représentant la fenêtre
      ///
      CLASS_CONFIG_WINDOW * CreateNewConfigWindow(void);

      ///
      /// \fn DeleteDeviceWindow
      /// \brief Instancie une nouvelle fenêtre machine
      /// \param machineWindowObj : Objet représentant la fenêtre machine
      ///
      void DeleteConfigWindow(CLASS_CONFIG_WINDOW *configWindowObj);

      /*--------------------------- Signaux publiques -------------------------*/

   signals:

      ///
      /// \fn SIGNAL_CreateConfigRequest
      /// \brief Signal émit pour la création d'une configuration
      ///
      void SIGNAL_CreateConfigRequest(void);

      ///
      /// \fn SIGNAL_DeleteMachineRequest
      /// \brief Signal émit pour la destruction d'une configuration
      /// \param configWindow : Fenêtre associée à la configuration
      ///
      void SIGNAL_DeleteConfigRequest(CLASS_CONFIG_WINDOW *configWindow);

      ///
      /// \fn SIGNAL_LoadStyleSheet
      /// \brief Signal émit pour pouvoir charger une feuille de style
      ///
      void SIGNAL_LoadStyleSheet(const QString& sheetFileName);


      /*---------------------------- Slots publiques --------------------------*/

   public slots:

      ///
      /// \fn SLOT_LoadStyleSheetActionTriggered
      /// \brief Slot enclenché lors de l'action load style sheet
      ///
      void SLOT_LoadStyleSheetActionTriggered(void);

      ///
      /// \fn SLOT_LoadStyleSheetActionTriggered
      /// \brief Slot enclenché lors de l'action About Qt
      ///
      void SLOT_AboutQtActionTriggered(void);

      /*----------------------------- Types privés ----------------------------*/

      /*--------------------------- Méthodes privées --------------------------*/

   private :

      ///
      /// \fn SetUpUserIntertace
      /// \brief Met en place l'interface graphique de la fenêtre
      ///
      void SetUpUserIntertace(void);

      ///
      /// \fn CreateConfigRequest
      /// \brief Requête de création d'une configuration
      ///
      void CreateConfigRequest(Bool checked);

   protected:

      ///
      /// \fn closeEvent
      /// \brief OverRide du close event de la fenêtre (destruction de tous les éléments correctement)
      /// \param closeEvent : évènement de fermeture
      ///
      void closeEvent(QCloseEvent* closeEvent);

   private:

       /*------------------------------ Variables ------------------------------*/

   protected:

   private:
      QMenu *f_FileMenu;                                   ///< Menu "File" de AUTOMATISATION
      QMenu *f_LoadMenu;                                   ///< Menu "Load" de AUTOMATISATION
      QMenu *f_AboutMenu;                                  ///< Menu "About" de AUTOMATISATION
      QAction *f_NewConfigAction;                          ///< Action "ToolBox" du menu "File"
      QAction *f_LoadStyleSheetAction;                     ///< Action "Load Style Sheet" du menu "Load"
      QAction *f_AboutAutomatisationAction;                ///< Action "AUTOMATISATION" du menu "About"

      QList<CLASS_CONFIG_WINDOW *> f_ConfigWindowList;     ///< Liste des fenêtres liées à une machine
};

#endif // CLASSAUTOMATISATIONWINDOW_H
