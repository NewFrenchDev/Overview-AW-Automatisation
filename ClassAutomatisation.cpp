///
/// \file ClassAUTOMATISATION.cpp
/// \brief Classe CLASS_AUTOMATISATION
/// \author ANALOG WAY - ADE
/// \date 2017-12-26
///
/// Fonctions de la classe CLASS_AUTOMATISATION
///

// C++ Standard
// Librairies Qt
// Modules
#include "VARSMANAGER_ClassVarsAsciiTranscoder.h"
// Applicatif
#include "ClassAutomatisation.h"

/******************************** DEFINITIONS ********************************/

/*----------------------------- Types publiques -----------------------------*/
/*--------------------------- Constantes publiques --------------------------*/
#define DEFAULT_DEVICE_NAME_PREFIX     QStringLiteral("Configuration")

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
/// \fn CLASS_AUTOMATISATION(QObject *parent)
/// \brief Constructeur de CLASS_AUTOMATISATION
///
/// \param[in] parent : Objet parent
///
CLASS_AUTOMATISATION::CLASS_AUTOMATISATION(QObject *parent):
   QObject(parent),
   f_ConfigsHashTable(),
   f_AutomatisationWindow(nullptr)
   //f_AutomatisationConf()
{
   // On instancie une nouvelle classe d'interface graphique sans lien de parenté (objet graphique principal)
   f_AutomatisationWindow = new CLASS_AUTOMATISATION_WINDOW();

   // On affiche la fenêtre de AUTOMATISATION
   f_AutomatisationWindow->show();

   // On vérifie la présence d'une configuration et on applique les reglages
   //this->CheckIfConfigurationHasBeenSaved();

   // On connecte le signal pour créer une machine
   QObject::connect(f_AutomatisationWindow, &CLASS_AUTOMATISATION_WINDOW::SIGNAL_CreateConfigRequest, this, &CLASS_AUTOMATISATION::CreateConfig);
   // On connecte le signal pour détruire une machine
   QObject::connect(f_AutomatisationWindow, &CLASS_AUTOMATISATION_WINDOW::SIGNAL_DeleteConfigRequest, this, &CLASS_AUTOMATISATION::DeleteConfig);
   // On connecte le signal pour le chargement d'une feuille de style
   QObject::connect(f_AutomatisationWindow, &CLASS_AUTOMATISATION_WINDOW::SIGNAL_LoadStyleSheet, this, &CLASS_AUTOMATISATION::LoadStyleSheet);
}

///
/// \fn ~CLASS_AUTOMATISATION()
/// \brief Destructeur de CLASS_AUTOMATISATION
///
CLASS_AUTOMATISATION::~CLASS_AUTOMATISATION()
{
   // Deconnexion des signaux générés par l'interface graphique
   QObject::disconnect(f_AutomatisationWindow, &CLASS_AUTOMATISATION_WINDOW::SIGNAL_CreateConfigRequest, this, &CLASS_AUTOMATISATION::CreateConfig);
   QObject::disconnect(f_AutomatisationWindow, &CLASS_AUTOMATISATION_WINDOW::SIGNAL_DeleteConfigRequest, this, &CLASS_AUTOMATISATION::DeleteConfig);
   QObject::disconnect(f_AutomatisationWindow, &CLASS_AUTOMATISATION_WINDOW::SIGNAL_LoadStyleSheet, this, &CLASS_AUTOMATISATION::LoadStyleSheet);

   // On parcours la QHash et on supprime les classes machines
   QHash<CLASS_CONFIG_WINDOW *, CLASS_CONFIGURATION*>::const_iterator HashConfigIterator(f_ConfigsHashTable.constBegin());
   while (HashConfigIterator != f_ConfigsHashTable.constEnd())
   {
      if (HashConfigIterator.value() != nullptr)
        HashConfigIterator.value()->deleteLater();

      // On incrémente l'itérateur
      HashConfigIterator++;
   }

   // On efface complètement la QHash de machines
   f_ConfigsHashTable.clear();

   // On détruit l'interface graphique de AUTOMATISATION
   f_AutomatisationWindow->deleteLater();
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

/******************************************************************************
 *                              Méthodes privées                              *
 *****************************************************************************/

///
/// \fn CreateConfig
/// \brief Création d'une configuration
///
void CLASS_AUTOMATISATION::CreateConfig(void)
{
   // On commence par créer la fenêtre
   CLASS_CONFIG_WINDOW * NewConfigWindow = f_AutomatisationWindow->CreateNewConfigWindow();

   // Création et insertion d'une classe configuration dans la QHash
   f_ConfigsHashTable.insert(NewConfigWindow, new CLASS_CONFIGURATION);
}

///
/// \fn DeleteConfig
/// \brief Appelé lors de la suppression d'une configuration
/// \param machineWindow : Fenêtre associé à la configuration
///
void CLASS_AUTOMATISATION::DeleteConfig(CLASS_CONFIG_WINDOW *configWindow)
{
   // Si rien à supprimer
   if (configWindow == nullptr)
      return;

   // Suppression de la fenêtre
   configWindow->deleteLater();
}

/*
///
/// \fn CheckIfConfigurationHasBeenSaved
/// \brief Vérifie qu'une configuration à été sauvegardé
///
void CLASS_AUTOMATISATION::CheckIfConfigurationHasBeenSaved(void)
{
   // On récupère la géométrie de la fenêntre
   QVariant SavedGeometry(CLASS_AUTOMATISATION_CONF::GetKeyValueInGroup(CONF_WINDOW_GEOMETRY_GROUP, CONF_WINDOW_GEOMETRY));

   // Si les deux valeurs ne sont pas null on va redimensionner la fenêtre
   if (SavedGeometry.isNull() == false)
   {
      // On converti le QVariant en QRect
      QRect WindowGeometry(SavedGeometry.toRect());

      // On applique ensuite la géométrie à la fenêtre
      f_AutomatisationWindow->setGeometry(WindowGeometry);

      // On affiche la fenêtre de AUTOMATISATION
      f_AutomatisationWindow->show();
  }

   // Si les valeurs sont null, on montre la fenêtre en full screen
   else
      f_AutomatisationWindow->showMaximized();

   // On commence par charger la style sheet par défaut
   QFile DefaultStyleSheet(CLASS_AUTOMATISATION_CONF::GetKeyValueInGroup(CONF_PREFERENCES, CONF_THEME).toString());
   // Puis on récupère la feuille de style chargée par l'utilisateur
   QVariant StyleSheet(CLASS_AUTOMATISATION_CONF::GetKeyValueInGroup(CONF_PREFERENCES, CONF_THEME));
   // On charge la feuille de style sur les widgets
   this->LoadStyleSheet(StyleSheet.toString());

}*/


///
/// \fn LoadStyleSheet
/// \brief Méthode charge une nouvelle feuille de style
///
void CLASS_AUTOMATISATION::LoadStyleSheet(const QString& sheetFileName)
{
   // On commence par charger la style sheet par défaut
   QFile DefaultStyleSheet(QStringLiteral("://STYLESHEET/DefaultStyle.css"));
   // Puis on récupère la feuille de style chargée par l'utilisateur
   QFile StyleSheetFile(sheetFileName);

   // Contenu du fichier css
   QString StyleSheetContent;

   if (sheetFileName.isEmpty() == false)
   {
      if ((DefaultStyleSheet.open(QIODevice::ReadOnly|QIODevice::Text) == true) &&
          (StyleSheetFile.open(QIODevice::ReadOnly|QIODevice::Text) == true))
      {
         // Crée un objet de type QTextStream qui va permettre de lire le texte du fichier .css
         QTextStream DefaultStyleSheetStream(&DefaultStyleSheet);
         QTextStream StyleSheetStream(&StyleSheetFile);

         // On lit le contenu du fichier css et on le stock dans une variable qui doit etre de type QString
         StyleSheetContent = StyleSheetStream.readAll() + DefaultStyleSheetStream.readAll();

         // On ferme les fichiers ouverts
         DefaultStyleSheet.close();
         StyleSheetFile.close();

         // On enregistre la styleshhet charger dans le fichier de configuration
         //CLASS_AUTOMATISATION_CONF::AddKeyInGroup(CONF_PREFERENCES, CONF_THEME, sheetFileName);
      }
   }

   else if (DefaultStyleSheet.open(QIODevice::ReadOnly|QIODevice::Text) == true)
   {
      // Crée un objet de type QTextStream qui va permettre de lire le texte du fichier .css
      QTextStream StyleSheetStream(&DefaultStyleSheet);

      // On lit le contenu du fichier css et on le stock dans une variable qui doit etre de type QString
      StyleSheetContent = StyleSheetStream.readAll();

      // On ferme le fichier ouvert
      DefaultStyleSheet.close();
   }

   // On propage ensuite la style sheet sur tout les widget de l'application
   f_AutomatisationWindow->setStyleSheet(StyleSheetContent);

   // On l'applique à tout les widgets enfants (fenêtres de configuration)
   QHash<CLASS_CONFIG_WINDOW *, CLASS_CONFIGURATION *>::const_iterator HashConfigIterator(f_ConfigsHashTable.constBegin());
   while (HashConfigIterator != f_ConfigsHashTable.constEnd())
   {
      HashConfigIterator.key()->setStyleSheet(StyleSheetContent);

      // On incrémente l'itérateur
      HashConfigIterator++;
   }
}
/******************************************************************************
 *                             Operateur surcharge                            *
 *****************************************************************************/
