///
/// \file GUI_ClassAbstractSubwindow.h
/// \brief Classe CLASS_ABSTRACT_MDI_SUBWINDOW
/// \author ANALOG WAY - ADE
/// \date 2018-01-24
///

#ifndef GUI_CLASSABSTRACTSUBWINDOW_H
#define GUI_CLASSABSTRACTSUBWINDOW_H

// C++ Standard
// Librairies Qt
#include <QCloseEvent>
#include <QColor>
#include <QJsonObject>
#include <QSize>
#include <QMdiSubWindow>

// Modules
#include "COMMON_TypeDefs.h"

///
/// \class CLASS_ABSTRACT_MDI_SUBWINDOW
/// \brief Classe CLASS_ABSTRACT_MDI_SUBWINDOW
///
class CLASS_ABSTRACT_MDI_SUBWINDOW : public QMdiSubWindow
{
      Q_OBJECT

   public:

      /*--------------------------- Types publiques ---------------------------*/

      /*------------------------- Constantes publiques ------------------------*/

      /*------------------------- Variables publiques -------------------------*/

      /*---------------------- Constructeur / Destructeur ---------------------*/

      ///
      /// \fn CLASS_ABSTRACT_MDI_SUBWINDOW(QObject *parent)
      /// \brief Constructeur de CLASS_ABSTRACT_MDI_SUBWINDOW
      ///
      /// \param[in] parent : Objet parent
      ///
      CLASS_ABSTRACT_MDI_SUBWINDOW(const QColor &backGroundColor, QWidget *parent = nullptr);

      ///
      /// \fn ~CLASS_ABSTRACT_MDI_SUBWINDOW()
      /// \brief Destructeur de CLASS_ABSTRACT_MDI_SUBWINDOW
      ///
      ~CLASS_ABSTRACT_MDI_SUBWINDOW();

      /*------------------------------ Accesseurs -----------------------------*/

      ///
      /// \fn sizeHint
      /// \brief Retourne la taille idéale souhaitée pour la fenêtre (redéfinition QWidget::sizeHint())
      /// \return QSize : Taille idéale
      ///
      virtual QSize sizeHint() const;

      ///
      /// \fn ToJson
      /// \brief Sérialise la géometrie des sous-fenêtres
      /// \return QJsonObject \e Sérialisation de la géometrie de la sous-fenêtre
      ///
      QJsonObject ToJson(void) const;

      /*------------------------------ Mutateurs ------------------------------*/

      ///
      /// \fn ModifyBackGroundColor
      /// \brief Modification de la couleur de fond
      /// \param backColor : Couleur de fond
      ///
      void ModifyBackGroundColor(const QColor& backColor);

      ///
      /// \fn SetSizeHint
      /// \brief Définit la taille idéale de la fenêtre
      /// \param sizeHint : Taille idéale
      ///
      void SetSizeHint(const QSize& sizeHint);

      /*-------------------------- Méthodes publiques -------------------------*/

      /*--------------------------- Signaux publiques -------------------------*/

   signals:

      ///
      /// \fn SIGNAL_CloseRequested
      /// \brief Signal émis lors d'une demande de fermeture refusée
      /// \param currentWindow : Pointe sur l'objet lui-même
      ///
      void SIGNAL_CloseRequested(CLASS_ABSTRACT_MDI_SUBWINDOW *currentWindow);

      /*---------------------------- Slots publiques --------------------------*/

   public slots:

      /*----------------------------- Types privés ----------------------------*/

      /*--------------------------- Méthodes privées --------------------------*/

   protected:

   private:

      ///
      /// \fn closeEvent
      /// \brief Redéfinition du close event
      /// \param e : infos sur l'evènement
      ///
      virtual void closeEvent(QCloseEvent *e);

      ///
      /// \fn SetBackGroundColor
      /// \brief Met en place la couleur de background de la subwindow
      ///
      void SetBackGroundColor(void);

      /*------------------------------ Variables ------------------------------*/

   protected:

   private:
      const QColor f_SubWindowBckGrdColor;        ///< Couleur de background de la fenêtre
      QSize f_WindowSizeHint;                     ///< Taille idéale de la fenêtre
};

#endif // GUI_CLASSABSTRACTSUBWINDOW_H
