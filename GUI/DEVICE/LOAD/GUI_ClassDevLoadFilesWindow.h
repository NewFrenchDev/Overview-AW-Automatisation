///
/// \file GUI_ClassDevLoadFilesWindow.h
/// \brief Classe CLASS_DEV_LOAD_FILES_WINDOW
/// \author ANALOG WAY - ADE
/// \date 2017-12-29
///

#ifndef PARSERS_CLASSPARSERWINDOW_H
#define PARSERS_CLASSPARSERWINDOW_H

// C++ Standard
// Librairies Qt
#include <QColor>
#include <QFileDialog>
#include <QFrame>
#include <QFutureWatcher>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMdiSubWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
// Modules
#include "COMMON_TypeDefs.h"
// Applicatif
#include "GUI_ClassAbstractSubwindow.h"

///
/// \class CLASS_DEV_LOAD_FILES_WINDOW
/// \brief Classe CLASS_DEV_LOAD_FILES_WINDOW
///
class CLASS_DEV_LOAD_FILES_WINDOW : public CLASS_ABSTRACT_MDI_SUBWINDOW
{
      Q_OBJECT

   public:

      /*--------------------------- Types publiques ---------------------------*/

      /// \enum T_FILE_ACTION
      /// \brief Actions disponible pour les fichiers
      enum class T_FILE_ACTION
      {
         REMOVE = 0,
         RELOAD,
      };

      /*------------------------- Constantes publiques ------------------------*/

      /*------------------------- Variables publiques -------------------------*/

      /*---------------------- Constructeur / Destructeur ---------------------*/

      ///
      /// \fn CLASS_DEV_LOAD_FILES_WINDOW(QObject *parent)
      /// \brief Constructeur de CLASS_DEV_LOAD_FILES_WINDOW
      ///
      /// \param[in] parent : widget parent
      /// \param[in] windowColor : couleur de la fenêtre
      ///
      explicit CLASS_DEV_LOAD_FILES_WINDOW(const QColor &windowColor, QWidget *parent = nullptr);

      ///
      /// \fn ~CLASS_DEV_LOAD_FILES_WINDOW()
      /// \brief Destructeur de CLASS_DEV_LOAD_FILES_WINDOW
      ///
      ~CLASS_DEV_LOAD_FILES_WINDOW();

      /*------------------------------ Accesseurs -----------------------------*/

      /*------------------------------ Mutateurs ------------------------------*/

      /*-------------------------- Méthodes publiques -------------------------*/

      /// \fn DisplayNewParsingResult
      /// \brief Affiche le résultat d'une extraction
      /// \param extractStatus : Statut de l'extraction
      /// \param errorsList : Liste des erreurs/warnings levées lors de l'extraction
      ///
      void DisplayNewParsingResult(Bool extractStatus, QStringList errorsList);

      /*--------------------------- Signaux publiques -------------------------*/

   signals:

      ///
      /// \fn SIGNAL_FileNameToParse
      /// \brief Signal envoyé à la classe machine pour le passer au parseur
      /// \param fileName : nom du fichier à parser
      ///
      void SIGNAL_FilesParseRequest(const QStringList& filesList);

      ///
      /// \fn SIGNAL_RemoveFileRequest
      /// \brief Signal envoyé à la classe machine pour effacer des entrées dans le conteneur
      /// \param filesList
      ///
      void SIGNAL_RemoveFileRequest(const QStringList& filesList);

      /*---------------------------- Slots publiques --------------------------*/

   public slots:



      /*----------------------------- Types privés ----------------------------*/

      /*----------------------------- Slots privées ---------------------------*/

   private slots:

      ///
      /// \fn SLOT_NewLoadRequest
      /// \brief Demande de chargement utilisateur
      /// \param loadResult : Résultat de l'opération
      ///
      void SLOT_NewLoadRequest(SDWord loadResult);

      ///
      /// \fn SLOT_OnLoadButtonClicked
      /// \brief Slot enclencher lors de l'appuis sur le bouton "Load"
      ///
      void SLOT_OnLoadButtonClicked(void);

      ///
      /// \fn SLOT_OnRemoveFileButtonClicked
      /// \brief Slot enclencher lors de l'appuis sur le bouton Remove file
      ///
      void SLOT_OnRemoveFileButtonClicked(void);

      ///
      /// \fn SLOT_OnReloadFileButtonClicked
      /// \brief Slot enclencher lors de l'appuis sur le bouton Reload file
      ///
      void SLOT_OnReloadFileButtonClicked(void);

      ///
      /// \fn SLOT_OnRemoveAllButtonClicked
      /// \brief Slot enclencher lors de l'appuis sur le bouton Remove All
      ///
      void SLOT_OnRemoveAllButtonClicked(void);

      /*--------------------------- Méthodes privées --------------------------*/

   private:

      ///
      /// \fn SetUpUserInterface
      /// \brief Met en place l'interface graphique de la fenêtre
      ///
      void SetUpUserInterface(void);

      ///
      /// \brief ExecuteActionOnfile
      /// \param requestedAction
      ///
      void ExecuteActionOnfile(const T_FILE_ACTION& requestedAction, const QStringList& files);

       /*------------------------------ Variables ------------------------------*/

   private:

      QVBoxLayout *f_WindowWidgetLayout;        ///< Layout principal de la fenêtre
      QHBoxLayout *f_ButtonLayout;              ///< Layout qui contient les deux boutons add et remove all
      QVBoxLayout *f_FilesButtonLayout;         ///< Layout pour les boutons et les fichiers
      QPushButton *f_AddFilesButton;            ///< Bouton qui charge les fichiers
      QPushButton *f_RemoveAllFilesButton;      ///< Bouton qui enleve tous les fichiers
      QFrame *f_Line;                           ///< Ligne qui sépare les boutons des fichiers
      QGroupBox *f_FileGroupBox;                ///< GroupBox qui regroup les fichiers parsé

      QList<QPushButton*> f_RemoveButtonList;   ///< Liste des boutons remove
      QList<QPushButton*> f_ReloadButtonList;   ///< Liste des boutons reload
      QList<QLabel*> f_LabelsList;              ///< Liste des Labels
      QList<QStringList> f_FilesLists;          ///< Liste des fichiers

      QStringList f_FilesSelectedByUser;        ///< Fichiers selectionnés par l'utilisateur

      QFileDialog *f_SelectFilesDialog;         ///< Interface permettant à l'utilisateur de sélectionner les fichiers à importer
};

#endif // PARSERS_CLASSPARSERWINDOW_H
