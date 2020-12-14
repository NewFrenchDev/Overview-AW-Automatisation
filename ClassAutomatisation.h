///
/// \file ClassAutomatisation.h
/// \brief Classe CLASS_Automatisation
/// \author ANALOG WAY - ADE
/// \date 2017-12-26
///

#ifndef CLASSAUTOMATISATION_H
#define CLASSAUTOMATISATION_H

// C++ Standard
// Librairies Qt
#include <QObject>
#include <QHash>
// Modules
#include "COMMON_TypeDefs.h"
// Applicatif
#include "DEVICE_ClassDevice.h"
#include "ClassConfiguration.h"
#include "GUI_ClassDeviceWindow.h"
#include "GUI_ClassConfigWindow.h"
#include "GUI_ClassAutomatisationWindow.h"

///
/// \class CLASS_Automatisation
/// \brief Classe CLASS_Automatisation
///
class CLASS_AUTOMATISATION : public QObject
{
      Q_OBJECT

   public:

      /*--------------------------- Types publiques ---------------------------*/

      /*------------------------- Constantes publiques ------------------------*/

      /*------------------------- Variables publiques -------------------------*/

      /*---------------------- Constructeur / Destructeur ---------------------*/

      ///
      /// \fn CLASS_Automatisation(QObject *parent)
      /// \brief Constructeur de CLASS_Automatisation
      ///
      /// \param[in] parent : Objet parent
      ///
      CLASS_AUTOMATISATION(QObject *parent = nullptr);

      ///
      /// \fn ~CLASS_Automatisation()
      /// \brief Destructeur de CLASS_Automatisation
      ///
      ~CLASS_AUTOMATISATION();

      /*------------------------------ Accesseurs -----------------------------*/

      /*------------------------------ Mutateurs ------------------------------*/

      /*-------------------------- Méthodes publiques -------------------------*/

      /*--------------------------- Signaux publiques -------------------------*/

   signals:

      /*---------------------------- Slots publiques --------------------------*/

   public slots:

      /*----------------------------- Types privés ----------------------------*/

      /*--------------------------- Méthodes privées --------------------------*/

   private:

      ///
      /// \fn GetDefaultConfigName
      /// \brief Retourne le nom par défaut de la configuration
      /// \return QString : Nom par défaut
      ///
      QString GetDefaultConfigName(void);

      ///
      /// \fn CreateConfiguration
      /// \brief Fonction qui créer une configuration
      ///
      void CreateConfig(void);

      ///
      /// \fn DeleteConfiguration
      /// \brief Appelé lors de la suppression d'une configuration
      /// \param machineWindow : Fenêtre associé à la configuration
      ///
      void DeleteConfig(CLASS_CONFIG_WINDOW *configWindow);

      ///
      /// \fn CheckIfConfigurationHasBeenSaved
      /// \brief Vérifie qu'une configuration à été sauvegardé
      ///
      void CheckIfConfigurationHasBeenSaved(void);

      ///
      /// \fn LoadStyleSheet
      /// \brief Méthode charge une nouvelle feuille de style
      ///
      void LoadStyleSheet(const QString &sheetFileName);

      /*------------------------------ Variables ------------------------------*/

   private:

      QHash<CLASS_CONFIG_WINDOW *, CLASS_CONFIGURATION*> f_ConfigsHashTable;     ///< QHash regroupant la liste des machines créées
      CLASS_AUTOMATISATION_WINDOW* f_AutomatisationWindow;                       ///< Interface graphique de Automatisation
};

#endif // CLASSAUTOMATISATION_H
