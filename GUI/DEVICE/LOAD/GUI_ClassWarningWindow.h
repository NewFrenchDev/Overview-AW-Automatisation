///
/// \file GUI_ClassParsingWarningWindow.h
/// \brief Classe CLASS_PARSING_WARNING_WINDOW
/// \author ANALOG WAY - ADE
/// \date 2018-02-09
///

#ifndef GUI_CLASSPARSINGWARNINGWINDOW_H
#define GUI_CLASSPARSINGWARNINGWINDOW_H

// C++ Standard
// Librairies Qt
#include <QObject>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
// Modules
#include "COMMON_TypeDefs.h"

///
/// \class CLASS_PARSING_WARNING_WINDOW
/// \brief Classe CLASS_PARSING_WARNING_WINDOW
///
class CLASS_WARNING_WINDOW : public QWidget
{
      Q_OBJECT

   public:

      /*--------------------------- Types publiques ---------------------------*/

      /*------------------------- Constantes publiques ------------------------*/

      /*------------------------- Variables publiques -------------------------*/

      /*---------------------- Constructeur / Destructeur ---------------------*/

      ///
      /// \fn CLASS_PARSING_WARNING_WINDOW(QObject *parent)
      /// \brief Constructeur de CLASS_PARSING_WARNING_WINDOW
      ///
      /// \param[in] parent : Objet parent
      ///
      CLASS_WARNING_WINDOW(const QString &errorsToDisplay, QWidget *parent = nullptr);

      ///
      /// \fn ~CLASS_PARSING_WARNING_WINDOW()
      /// \brief Destructeur de CLASS_PARSING_WARNING_WINDOW
      ///
      ~CLASS_WARNING_WINDOW();

      /*------------------------------ Accesseurs -----------------------------*/

      /*------------------------------ Mutateurs ------------------------------*/

      /*-------------------------- Méthodes publiques -------------------------*/

      /*--------------------------- Signaux publiques -------------------------*/

   signals:

      /*---------------------------- Slots publiques --------------------------*/

   public slots:

      ///
      /// \fn SLOT_OnOkButtonClicked
      /// \brief Slot enclencher lors de l'appuis sur le bouton Ok
      ///
      void SLOT_OnOkButtonClicked(void);

      /*----------------------------- Types privés ----------------------------*/

      /*--------------------------- Méthodes privées --------------------------*/

   protected:

      ///
      /// \fn SetUpUserInterface
      /// \brief Met en place l'interface graphique
      ///
      void SetUpUserInterface(void);

   private:

      /*------------------------------ Variables ------------------------------*/

   protected:

   private:

      QVBoxLayout *f_WindowWidgetLayout;        ///< Layout principal de la fenêtre
      QPlainTextEdit *f_ErrorsDisplay;          ///< Plain text qui affiche les erreur
      QPushButton *f_OkButton;                  ///< Bouton pour quitter la fenêtre
};

#endif // GUI_CLASSPARSINGWARNINGWINDOW_H
