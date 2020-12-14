///
/// \file ClassConfiguration.h
/// \brief Classe CLASS_CONFIGURATION
/// \author ANALOG WAY - GLE
/// \date 2018-05-02
///

#ifndef CLASSCONFIGURATION_H
#define CLASSCONFIGURATION_H

// C++ Standard
// Librairies Qt
#include <QObject>
#include <QHash>
// Modules
#include "COMMON_TypeDefs.h"
// Applicatif
//#include "CONF_ClassAutomatisationConf.h"
//#include "CONF_Define.h"
#include "DEVICE_ClassDevice.h"
#include "GUI_ClassDeviceWindow.h"
#include "GUI_ClassConfigWindow.h"



///
/// \class CLASS_CONFIGURATION
/// \brief Classe CLASS_CONFIGURATION
///
class CLASS_CONFIGURATION : public QObject
{
      Q_OBJECT


   public:

      /*--------------------------- Types publiques ---------------------------*/

      /*------------------------- Constantes publiques ------------------------*/

      /*------------------------- Variables publiques -------------------------*/

      /*---------------------- Constructeur / Destructeur ---------------------*/

      ///
      /// \fn CLASS_CONFIGURATION(QObject *parent)
      /// \brief Constructeur de CLASS_CONFIGURATION
      ///
      /// \param[in] parent : Objet parent
      ///
      CLASS_CONFIGURATION(QObject *parent = nullptr);

      ///
      /// \fn ~CLASS_CONFIGURATION()
      /// \brief Destructeur de CLASS_CONFIGURATION
      ///
      ~CLASS_CONFIGURATION();

      /*------------------------------ Accesseurs -----------------------------*/

      /*------------------------------ Mutateurs ------------------------------*/

      /*-------------------------- MÃ©thodes publiques -------------------------*/

      /*--------------------------- Signaux publiques -------------------------*/

   signals:

      /*---------------------------- Slots publiques --------------------------*/

   public slots:

      /*----------------------------- Types privÃ©s ----------------------------*/

      /*--------------------------- MÃ©thodes privÃ©es --------------------------*/

   protected:

   private:

      ///
      /// \fn GetDefaultMachineName
      /// \brief Retourne le nom par défaut de la machine
      /// \return QString : Nom par défaut
      ///
      QString GetDefaultMachineName(void);

      ///
      /// \fn GetDefaultMachineColor
      /// \brief Retourne la couleur par défaut des fenêtres de la machine
      /// \return QString : Nom par défaut
      ///
      QColor GetDefaultMachineColor(void);

      ///
      /// \fn GetDefaultCommInfo
      /// \brief Retourne les paramètres de communications par défaut
      /// \return const CLASS_ABSTRACT_INFO_COM * : Paramètres de comm par défaut
      ///
      const CLASS_ABSTRACT_INFO_COM * GetDefaultCommInfo(void);

      ///
      /// \fn GetDefaultGatewayInfo
      /// \brief Retourne les paramètres par défaut de la passerelle
      /// \return const CLASS_INFO_COM_TCP_SERVER * : Paramètres passerelle par défaut
      ///
      const CLASS_INFO_COM_TCP_SERVER * GetDefaultGatewayInfo(void);

      ///
      /// \fn CreateMachine
      /// \brief Fonction qui créer une machine
      ///
      void CreateMachine(void);

      ///
      /// \fn DeleteMachine
      /// \brief Appelé lors de la suppression d'une machine
      /// \param machineWindow : Fenêtre associé à la machine
      ///
      void DeleteMachine(CLASS_DEVICE_WINDOW *machineWindow);



      /*------------------------------ Variables ------------------------------*/

   protected:

   private:
      QHash<CLASS_DEVICE_WINDOW *, CLASS_MACHINE*> f_MachinesHashTable;     ///< QHash regroupant la liste des machines créées
      CLASS_CONFIG_WINDOW* f_ConfigurationWindow;                           ///< Interface graphique de Configuration

};

#endif // CLASSCONFIGURATION_H
