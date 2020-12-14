///
/// \file PARSERS_ClassParserWindow.cpp
/// \brief Classe CLASS_DEV_LOAD_FILES_WINDOW
/// \author ANALOG WAY - ADE
/// \date 2017-12-29
///
/// Fonctions de la classe CLASS_DEV_LOAD_FILES_WINDOW
///

// C++ Standard
// Librairies Qt
#include <QtConcurrent>
#include <QHeaderView>
#include <QMessageBox>
#include <QString>
// Modules
#include "TRACES.h"
// Applicatif
#include "GUI_ClassDevLoadFilesWindow.h"
#include "GUI_ClassWarningWindow.h"

/******************************** DEFINITIONS ********************************/

// Titre de la fenêtre
#define WINDOW_NAME                QStringLiteral("Load Files Window")

// Taille par défaut de la fenêtre
#define WINDOW_DEFAULT_WIDTH       470
#define WINDOW_DEFAULT_HEIGHT      250

// Taille minimale de la fenêtre
#define WINDOW_MIN_WIDTH           400
#define WINDOW_MIN_HEIGHT          150

// Nom des colonnes du tableau
#define TAB_FILE_COLUMN_NAME       QStringLiteral("Files")
#define TAB_ACTIONS_COLUMN_NAME    QStringLiteral("Actions")

// Nom des boutons
#define BUTTON_ADD_FILES_NAME     QStringLiteral("Add Files")
#define BUTTON_RM_ALL_NAME        QStringLiteral("Remove All")
#define BUTTON_RELOAD_NAME        QStringLiteral("Reload")
#define BUTTON_REMOVE_NAME        QStringLiteral("Remove")

// Taille des bouton
#define BUTTON_SIZE_WIDTH         60
#define BUTTON_SIZE_HEIGHT        25

// Nom de la groupbox
#define GROUPBOX_FILES_NAME       QStringLiteral("Files")

// Séparateur des fichiers
#define FILE_NAME_SEPARATOR       QStringLiteral("\n")

// Séparateur des erreur
#define ERRORS_SEPARATOR          QStringLiteral("\n\r")

/*----------------------------- Types publiques -----------------------------*/
/*--------------------------- Constantes publiques --------------------------*/
/*--------------------------- Variables publiques ---------------------------*/
/*------------------------------- Types privés ------------------------------*/
/*----------------------- Prototypes fonctions privées ----------------------*/
static QStringList GetUserFilesSelection(QWidget *parent);

/*---------------------------- Constantes privées ---------------------------*/
/*---------------------------- Variables privées ----------------------------*/

/**************************** FIN DES DEFINITIONS ****************************/

/******************************************************************************
 *                             Fonctions privées                              *
 *****************************************************************************/

///
/// \fn GetUserFilesSelection
/// \brief Retourne les fichiers sélectionnés par l'utlisateur
/// \return QStringList : Sélection de fichiers
///
static QStringList GetUserFilesSelection(QWidget *parent)
{
   return QFileDialog::getOpenFileNames(parent, QStringLiteral("Load Design"), QStringLiteral("/home/"), QStringLiteral("JSON (*json)"));
}

/******************************************************************************
 *                         Constructeur / Destructeur                         *
 *****************************************************************************/

///
/// \fn CLASS_DEV_LOAD_FILES_WINDOW(QObject *parent)
/// \brief Constructeur de CLASS_DEV_LOAD_FILES_WINDOW
///
/// \param[in] parent : widget parent
/// \param[in] windowColor : couleur de la fsenêtre
///
CLASS_DEV_LOAD_FILES_WINDOW::CLASS_DEV_LOAD_FILES_WINDOW(const QColor &windowColor, QWidget *parent) :
   CLASS_ABSTRACT_MDI_SUBWINDOW(windowColor, parent),
   f_WindowWidgetLayout(nullptr),
   f_ButtonLayout(nullptr),
   f_FilesButtonLayout(nullptr),
   f_AddFilesButton(nullptr),
   f_Line(nullptr),
   f_FileGroupBox(nullptr),
   f_FilesSelectedByUser(),
   f_SelectFilesDialog(nullptr)
{
   // On met place l'interface graphique
   this->SetUpUserInterface();

   // On connecte le signal ou slot du bouton pour charger un design
   QObject::connect(f_AddFilesButton, &QPushButton::clicked, this, &CLASS_DEV_LOAD_FILES_WINDOW::SLOT_OnLoadButtonClicked);
   QObject::connect(f_RemoveAllFilesButton, &QPushButton::clicked, this, &CLASS_DEV_LOAD_FILES_WINDOW::SLOT_OnRemoveAllButtonClicked);
}

///
/// \fn ~CLASS_DEV_LOAD_FILES_WINDOW()
/// \brief Destructeur de CLASS_DEV_LOAD_FILES_WINDOW
///
CLASS_DEV_LOAD_FILES_WINDOW::~CLASS_DEV_LOAD_FILES_WINDOW()
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

/// \fn DisplayNewParsingResult
/// \brief Affiche le résultat d'une extraction
/// \param extractStatus : Statut de l'extraction
/// \param errorsList : Liste des erreurs/warnings levées lors de l'extraction
///
void CLASS_DEV_LOAD_FILES_WINDOW::DisplayNewParsingResult(Bool extractStatus, QStringList errorsList)
{
   // Si le parsing à été effectué
   if (extractStatus == true)
   {
      QStringList f_FilesWithoutPath;
      for (QString files : f_FilesSelectedByUser)
      {
         QFileInfo FileInfo(files);
         f_FilesWithoutPath.append(FileInfo.fileName());
      }

      // On crée un layout pour les widget
      QHBoxLayout *FilesLayout = new QHBoxLayout();
      FilesLayout->setContentsMargins(2, 2, 2, 2);
      FilesLayout->setAlignment(Qt::AlignTop);

      // on crée un QLabel
      QLabel *FilesLabel = new QLabel(f_FilesWithoutPath.join(FILE_NAME_SEPARATOR), this);
      FilesLabel->setStyleSheet("background-color: white;");
      FilesLayout->addWidget(FilesLabel);

      // On crée un layout pour les boutons
      QHBoxLayout *ButtonsLayout = new QHBoxLayout();
      ButtonsLayout->setContentsMargins(0, 0, 0, 0);

      // On crée les boutons d'action
      QPushButton *ReloadButton = new QPushButton(BUTTON_RELOAD_NAME, this);
      QPushButton *RemoveButton = new QPushButton(BUTTON_REMOVE_NAME, this);
      ReloadButton->setFixedSize(BUTTON_SIZE_WIDTH, BUTTON_SIZE_HEIGHT);
      RemoveButton->setFixedSize(BUTTON_SIZE_WIDTH, BUTTON_SIZE_HEIGHT);
      ButtonsLayout->addWidget(ReloadButton);
      ButtonsLayout->addWidget(RemoveButton);

      FilesLayout->addLayout(ButtonsLayout);

      // On connecte les boutons a leurs slots
      QObject::connect(ReloadButton, &QPushButton::clicked, this, &CLASS_DEV_LOAD_FILES_WINDOW::SLOT_OnReloadFileButtonClicked);
      QObject::connect(RemoveButton, &QPushButton::clicked, this, &CLASS_DEV_LOAD_FILES_WINDOW::SLOT_OnRemoveFileButtonClicked);

      f_FilesLists.append(f_FilesSelectedByUser);
      f_LabelsList.append(FilesLabel);
      f_RemoveButtonList.append(RemoveButton);
      f_ReloadButtonList.append(ReloadButton);

      // Si il y a des warning, on les affiche dans une message box
      if (errorsList.isEmpty() == false)
      {
         QString ErrorsToDisplay(errorsList.join(ERRORS_SEPARATOR));
         CLASS_WARNING_WINDOW *MessageBox = new CLASS_WARNING_WINDOW(ErrorsToDisplay);
         MessageBox->raise();
      }

      f_FilesButtonLayout->insertLayout(f_FilesButtonLayout->count() - 1, FilesLayout);
   }
   // Si le parsing n'as pas pu être effectuer
   else
   {
      QString ErrorsToDisplay(errorsList.join(ERRORS_SEPARATOR));
       CLASS_WARNING_WINDOW *MessageBox = new CLASS_WARNING_WINDOW(ErrorsToDisplay);
       MessageBox->raise();
   }

   // On vide les fichier selectionnés auparavant par l'utilisateur
   f_FilesSelectedByUser.clear();
}

/******************************************************************************
 *                               Slots publiques                              *
 *****************************************************************************/

/******************************************************************************
 *                               Slots privées                               *
 *****************************************************************************/

///
/// \fn SLOT_NewLoadRequest
/// \brief Demande de chargement utilisateur
/// \param loadResult : Résultat de l'opération
///
void CLASS_DEV_LOAD_FILES_WINDOW::SLOT_NewLoadRequest(SDWord loadResult)
{
   // Si l'utilisateur a validé
   if (loadResult > 0)
   {
      QStringList ExtractFiles = f_SelectFilesDialog->selectedFiles();

      // Si aucun fichier fourni par l'utilisateur, on stop la requête de load
      if (ExtractFiles.count() >= 1)
      {
         f_FilesSelectedByUser.append(ExtractFiles);
         // On emet un signal pour le passer au parseur
         emit this->SIGNAL_FilesParseRequest(ExtractFiles);
      }
   }

   // Supprime la fenêtre
   f_SelectFilesDialog->deleteLater();
   f_SelectFilesDialog = nullptr;
}

///
/// \fn SLOT_OnLoadButtonClicked
/// \brief Slot enclencher lors de l'appuis sur le bouton "Load"
/// \param clicked : état du bouton
///
void CLASS_DEV_LOAD_FILES_WINDOW::SLOT_OnLoadButtonClicked(void)
{
   // Si une fenêtre est déjà active, on ignore l'évènement
   if (f_SelectFilesDialog != nullptr)
      return;

   f_SelectFilesDialog = new QFileDialog(this, QStringLiteral("Add Files"), QStringLiteral("/home/"), QStringLiteral("AWT (*awt);;All Files (*)"));
   f_SelectFilesDialog->setFileMode(QFileDialog::ExistingFiles);
   QObject::connect(f_SelectFilesDialog, &QFileDialog::finished, this, &CLASS_DEV_LOAD_FILES_WINDOW::SLOT_NewLoadRequest);
   f_SelectFilesDialog->setVisible(true);
}

///
/// \fn SLOT_OnRemoveFileButtonClicked
/// \brief Slot enclencher lors de l'appuis sur le bouton Remove file
///
void CLASS_DEV_LOAD_FILES_WINDOW::SLOT_OnRemoveFileButtonClicked(void)
{
   // On demande à l 'utilisateur si il est sur
   QMessageBox::StandardButton Reply;
   // On initialise la réponse de la message box
   Reply = QMessageBox::question(this, QStringLiteral("Add Files"), QStringLiteral("Are you sure ?"), QMessageBox::Yes|QMessageBox::No);

   // On agit en fonction de la réponse
   if (Reply == QMessageBox::Yes)
   {
      QPushButton *RemoveButton( static_cast<QPushButton*>(sender()));
      DWord ButtonIndex(f_RemoveButtonList.indexOf(RemoveButton));
      this->ExecuteActionOnfile(T_FILE_ACTION::REMOVE, f_FilesLists.at(ButtonIndex));
   }
}

///
/// \fn SLOT_OnReloadFileButtonClicked
/// \brief Slot enclencher lors de l'appuis sur le bouton Reload file
///
void CLASS_DEV_LOAD_FILES_WINDOW::SLOT_OnReloadFileButtonClicked(void)
{
   QPushButton *ReloadButton( static_cast<QPushButton*>(sender()));
   DWord ButtonIndex(f_ReloadButtonList.indexOf(ReloadButton));
   this->ExecuteActionOnfile(T_FILE_ACTION::RELOAD, f_FilesLists.at(ButtonIndex));
}


///
/// \fn SLOT_OnRemoveAllButtonClicked
/// \brief Slot enclencher lors de l'appuis sur le bouton Remove All
///
void CLASS_DEV_LOAD_FILES_WINDOW::SLOT_OnRemoveAllButtonClicked(void)
{
   for (SDWord indexinList(0); indexinList<f_FilesLists.count(); indexinList++)
   {
      emit this->SIGNAL_RemoveFileRequest(f_FilesLists.at(indexinList));
      QPushButton *RemoveButton(f_RemoveButtonList.at(indexinList));
      QPushButton *ReloadButton(f_ReloadButtonList.at(indexinList));
      QLabel *Label(f_LabelsList.at(indexinList));
      RemoveButton->deleteLater();
      ReloadButton->deleteLater();
      Label->deleteLater();
   }

   f_FilesLists.clear();
   f_ReloadButtonList.clear();
   f_RemoveButtonList.clear();
   f_LabelsList.clear();
}

/******************************************************************************
 *                              Méthodes privées                              *
 *****************************************************************************/

///
/// \fn SetUpUserInterface
/// \brief Met en place l'interface graphique de la fenêtre
///
void CLASS_DEV_LOAD_FILES_WINDOW::SetUpUserInterface(void)
{
   // On change le nom de la fenêtre
   this->setWindowTitle(WINDOW_NAME);

   // On resize avec les mesures par défaut
   this->SetSizeHint(QSize(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT));
   this->setMinimumSize(QSize(WINDOW_MIN_WIDTH, WINDOW_MIN_HEIGHT));
   // Le fenêtre essaye de se mettre à la taille par défaut
   this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

   // On set le layout principal
   f_WindowWidgetLayout = new QVBoxLayout();
   f_WindowWidgetLayout->setContentsMargins(0, 0, 0, 0);

   // Associe ce layout à la fenêtre MDI via un QWidget intermédiaire
   this->setWidget(new QWidget(this));
   this->widget()->setLayout(f_WindowWidgetLayout);

   // On met en place la groupbox
   f_FileGroupBox = new QGroupBox(GROUPBOX_FILES_NAME);
   f_FilesButtonLayout = new QVBoxLayout(f_FileGroupBox);
   f_FilesButtonLayout->setContentsMargins(0, 0, 0, 0);
   f_FilesButtonLayout->addSpacerItem(new QSpacerItem(0,0, QSizePolicy::Minimum, QSizePolicy::Expanding));
   f_WindowWidgetLayout->addWidget(f_FileGroupBox);

   // On met en place la ligne qui sépare les fichiers des boutons
   f_Line = new QFrame(this);
   f_Line->setFrameShape(QFrame::HLine);
   f_Line->setFrameShadow(QFrame::Sunken);
   f_WindowWidgetLayout->addWidget(f_Line);

   // On set le layout des boutons
   f_ButtonLayout = new QHBoxLayout();
   f_AddFilesButton = new QPushButton(BUTTON_ADD_FILES_NAME, this);
   f_RemoveAllFilesButton = new QPushButton(BUTTON_RM_ALL_NAME, this);
   f_ButtonLayout->addWidget(f_AddFilesButton);
   f_ButtonLayout->addWidget(f_RemoveAllFilesButton);

   f_WindowWidgetLayout->addLayout(f_ButtonLayout);
}

///
/// \brief ExecuteActionOnfile
/// \param requestedAction
///
void CLASS_DEV_LOAD_FILES_WINDOW::ExecuteActionOnfile(const T_FILE_ACTION& requestedAction, const QStringList& files)
{
   #warning "Action afficher les informations sur le fichier a faire"

      // Si le texte du label est vide, on quitte la méthode
      if (files.isEmpty() == true)
         return;

      // Sinon on demande de retirer les données associées au fichier
      emit this->SIGNAL_RemoveFileRequest(files);

      // Si l'action est un rechargement, on redemande a parser le fichier.
      if (requestedAction == T_FILE_ACTION::RELOAD)
      {
         emit this->SIGNAL_FilesParseRequest(files);
         f_FilesSelectedByUser.append(files);
      }

      DWord IndexInList(f_FilesLists.indexOf(files));
      QPushButton *RemoveButton(f_RemoveButtonList.at(IndexInList));
      QPushButton *ReloadButton(f_ReloadButtonList.at(IndexInList));
      QLabel *Label(f_LabelsList.at(IndexInList));
      f_RemoveButtonList.removeOne(RemoveButton);
      f_ReloadButtonList.removeOne(ReloadButton);
      f_LabelsList.removeOne(Label);
      f_FilesLists.removeOne(files);
      RemoveButton->deleteLater();
      ReloadButton->deleteLater();
      Label->deleteLater();
}
/******************************************************************************
 *                             Operateur surcharge                            *
 *****************************************************************************/
