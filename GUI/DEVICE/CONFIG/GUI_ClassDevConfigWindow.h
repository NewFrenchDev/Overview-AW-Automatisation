///
/// \file GUI_ClassDeviceConfigWindow.h
/// \brief Classe CLASS_DEVICE_CONFIG_WINDOW
/// \author ANALOG WAY - ADE
/// \date 2018-01-25
///

#ifndef GUI_CLASSDEVCONFIGWINDOW_H
#define GUI_CLASSDEVCONFIGWINDOW_H

// C++ Standard
// Librairies Qt
#include <QColor>
#include <QColorDialog>
#include <QFrame>
#include <QGridLayout>
#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QVBoxLayout>
// Modules
#include "COMMON_TypeDefs.h"
// Applicatif
#include "GUI_ClassAbstractSubwindow.h"

///
/// \class CLASS_DEVICE_CONFIG_WINDOW
/// \brief Classe CLASS_DEVICE_CONFIG_WINDOW
///
class CLASS_DEV_CONFIG_WINDOW : public CLASS_ABSTRACT_MDI_SUBWINDOW
{
      Q_OBJECT

   public:

      /*--------------------------- Types publiques ---------------------------*/

      /*------------------------- Constantes publiques ------------------------*/

      /*------------------------- Variables publiques -------------------------*/

      /*---------------------- Constructeur / Destructeur ---------------------*/

      ///
      /// \fn CLASS_DEVICE_CONFIG_WINDOW(QObject *parent)
      /// \brief Constructeur de CLASS_DEVICE_CONFIG_WINDOW
      ///
      /// \param[in] parent : Objet parent
      ///
      CLASS_DEV_CONFIG_WINDOW(const QColor &windowColor, QWidget *parent = nullptr);

      ///
      /// \fn ~CLASS_DEVICE_CONFIG_WINDOW()
      /// \brief Destructeur de CLASS_DEVICE_CONFIG_WINDOW
      ///
      ~CLASS_DEV_CONFIG_WINDOW();

      /*------------------------------ Accesseurs -----------------------------*/

      /*------------------------------ Mutateurs ------------------------------*/

      /*-------------------------- Méthodes publiques -------------------------*/

      /*--------------------------- Signaux publiques -------------------------*/

   signals:

      ///
      /// \fn SIGNAL_ChangeDeviceName
      /// \brief Emit pour changer le nom du device
      /// \param newDeviceName : nouveau nom du device
      ///
      void SIGNAL_ChangeDeviceName(const QString& newDeviceName);

      ///
      /// \fn SIGNAL_ChangeDeviceColor
      /// \brief Emit pour changer la couleur du device
      /// \param newDeviceColor : nouvelle couleur du device
      ///
      void SIGNAL_ChangeDeviceColor(const QColor& newDeviceColor);

      /*---------------------------- Slots publiques --------------------------*/

   public slots:

      ///
      /// \fn SLOT_OnChangeNameButtonClicked
      /// \brief Slot enclenché lors de l'appui sur le bouton de validation
      ///
      void SLOT_OnChangeNameButtonClicked(void);

      ///
      /// \fn SLOT_OnChangeColorButtonClicked
      /// \brief Slot enclenché lors de l'appui sur le bouton de validation
      ///
      void SLOT_OnChangeColorButtonClicked(void);

      /*----------------------------- Types privés ----------------------------*/

      /*--------------------------- Méthodes privées --------------------------*/

   protected:

   private:

      ///
      /// \fn SetUpUserInterface
      /// \brief Met en place l'interface graphique de la fenêtre
      ///
      void SetUpUserInterface(void);

      /*------------------------------ Variables ------------------------------*/

   protected:

   private:

      QVBoxLayout *f_WindowWidgetLayout;                 ///< Layout principal de la fenêtre
      QGridLayout *f_ParametersLayout;                   ///< Layout pour les paramètres, sauf pour le QDialog
      QLineEdit *f_DeviceNameEdit;                       ///< Line edit pour modifier le nom de la machine
      QColorDialog *f_ColorDialog;                       ///< Color dialog pour choisir le couleur de la fenêtre
      QPushButton *f_ChangeNameButton;                   ///< Bouton permettant seulement de changer le nom
      QPushButton *f_ChangeColorButton;                  ///< Bouton permettant de seulement changer la couleur de la fenêtre de la machine
      QFrame *f_Line;                                    ///<
};

#endif // GUI_CLASSDEVCONFIGWINDOW_H
