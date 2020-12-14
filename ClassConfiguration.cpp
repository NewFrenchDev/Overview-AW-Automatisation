///
/// \file ClassConfiguration.cpp
/// \brief Classe CLASS_CONFIGURATION
/// \author ANALOG WAY - GLE
/// \date 2018-05-02
///
/// Fonctions de la classe CLASS_CONFIGURATION
///

// C++ Standard
// Librairies Qt
// Modules
#include "VARSMANAGER_ClassVarsAsciiTranscoder.h"
// Applicatif
#include "ClassConfiguration.h"

/******************************** DEFINITIONS ********************************/

/*----------------------------- Types publiques -----------------------------*/
/*--------------------------- Constantes publiques --------------------------*/
/*--------------------------- Variables publiques ---------------------------*/
/*------------------------------- Types privÃ©s ------------------------------*/
/*----------------------- Prototypes fonctions privÃ©es ----------------------*/
/*---------------------------- Constantes privÃ©es ---------------------------*/
///< List qui contient les couleurs des machines possible
static const QList<QColor> MachineWindowsColorList
{
   { QColor(89,   90, 109) },
   { QColor(154, 160, 153) },
   { QColor(171, 201, 189) },
   { QColor(215, 162, 158) },
   { QColor(232, 216, 191) },
   { QColor(39,   35,  62) },
   { QColor(65,  108, 133) },
   { QColor(175, 209, 234) },
   { QColor(213, 227, 212) },
   { QColor(171, 160, 205) },
};

// Terminaisons par défaut des messages ASCII


// Préfixe du nom par défaut des machines
#define DEFAULT_DEVICE_NAME_PREFIX     QStringLiteral("Device ")

// Paramètres par défaut de la communication
#define DEFAULT_COMM_DEVICE_IP         QStringLiteral("192.168.2.92")
#define DEFAULT_COMM_TCP_PORT          10500
#define DEFAULT_COMM_ASCII_RX_TERM     CLASS_VARS_ASCII_TRANSCODER::T_TERM_MODE::TERM_LF
#define DEFAULT_COMM_ASCII_TX_TERM     CLASS_VARS_ASCII_TRANSCODER::T_TERM_MODE::TERM_CR_LF

// Paramètres par défaut pour la passerelle
#define DEFAULT_GATEWAY_IP             QStringLiteral("127.0.0.1")
#define DEFAULT_GATEWAY_TCP_PORT       10500

/*---------------------------- Variables privées ----------------------------*/

/**************************** FIN DES DEFINITIONS ****************************/

/******************************************************************************
 *                             Fonctions privÃ©es                              *
 *****************************************************************************/

/******************************************************************************
 *                         Constructeur / Destructeur                         *
 *****************************************************************************/

///
/// \fn CLASS_CONFIGURATION(QObject *parent)
/// \brief Constructeur de CLASS_CONFIGURATION
///
/// \param[in] parent : Objet parent
///
CLASS_CONFIGURATION::CLASS_CONFIGURATION(QObject *parent):
   QObject(parent),
   f_MachinesHashTable(),
   f_ConfigurationWindow(nullptr)
   //f_AutomatisationConf()
{
   // On instancie une nouvelle classe d'interface graphique sans lien de parenté (objet graphique principal)
   f_ConfigurationWindow = new CLASS_CONFIG_WINDOW();

   // On vérifie la présence d'une configuration et on applique les reglages
   //this->CheckIfConfigurationHasBeenSaved();

   f_ConfigurationWindow->show();

   // On connecte le signal pour créer une machine
   QObject::connect(f_ConfigurationWindow, &CLASS_CONFIG_WINDOW::SIGNAL_CreateMachineRequest, this, &CLASS_CONFIGURATION::CreateMachine);
   // On connecte le signal pour détruire une machine
   QObject::connect(f_ConfigurationWindow, &CLASS_CONFIG_WINDOW::SIGNAL_DeleteMachineRequest, this, &CLASS_CONFIGURATION::DeleteMachine);
}

///
/// \fn ~CLASS_CONFIGURATION()
/// \brief Destructeur de CLASS_CONFIGURATION
///
CLASS_CONFIGURATION::~CLASS_CONFIGURATION()
{
   // Deconnexion des signaux générés par l'interface graphique
   QObject::disconnect(f_ConfigurationWindow, &CLASS_CONFIG_WINDOW::SIGNAL_CreateMachineRequest, this, &CLASS_CONFIGURATION::CreateMachine);
   QObject::disconnect(f_ConfigurationWindow, &CLASS_CONFIG_WINDOW::SIGNAL_DeleteMachineRequest, this, &CLASS_CONFIGURATION::DeleteMachine);

   // On parcours la QHash et on supprime les classes machines
   QHash<CLASS_DEVICE_WINDOW *, CLASS_MACHINE*>::const_iterator HashMachineIterator(f_MachinesHashTable.constBegin());
   while (HashMachineIterator != f_MachinesHashTable.constEnd())
   {
      if (HashMachineIterator.value() != nullptr)
        HashMachineIterator.value()->deleteLater();

      // On incrémente l'itérateur
      HashMachineIterator++;
   }

   // On efface complètement la QHash de machines
   f_MachinesHashTable.clear();

   // On détruit l'interface graphique de AUTOMATISATION
   f_ConfigurationWindow->deleteLater();
}

/******************************************************************************
 *                                 Accesseurs                                 *
 *****************************************************************************/

/******************************************************************************
 *                                  Mutateurs                                 *
 *****************************************************************************/

/******************************************************************************
 *                             MÃ©thodes publiques                             *
 *****************************************************************************/

/******************************************************************************
 *                               Slots publiques                              *
 *****************************************************************************/

/******************************************************************************
 *                              MÃ©thodes privÃ©es                              *
 *****************************************************************************/

///
/// \fn GetDefaultMachineName
/// \brief Retourne le nom par défaut de la machine
/// \return QString : Nom par défaut
///
QString CLASS_CONFIGURATION::GetDefaultMachineName(void)
{
   SDWord MachineNumber(f_MachinesHashTable.count() + 1);

   // On cherche à avoir un nom de machine pas encore pris
   while (1)
   {
      Bool NameCollision(false);
      QString MachineName(DEFAULT_DEVICE_NAME_PREFIX % QString::number(MachineNumber++));

      for (const CLASS_MACHINE *MachineObj : f_MachinesHashTable.values())
      {
         if (MachineObj->GetDeviceName() == MachineName)
         {
            NameCollision = true;
            break;
         }
      }

      if (NameCollision == false)
         return MachineName;
   }
}

///
/// \fn GetDefaultMachineColor
/// \brief Retourne la couleur par défaut des fenêtres de la machine
/// \return QString : Nom par défaut
///
QColor CLASS_CONFIGURATION::GetDefaultMachineColor(void)
{
   // Recopie la liste des couleurs possibles
   QList<QColor> LocalMachineWindowsColorList(MachineWindowsColorList);

   // Supprime toutes les couleurs déjà utilisées par des machines
   for (const CLASS_MACHINE *MachineObj : f_MachinesHashTable.values())
      LocalMachineWindowsColorList.removeOne(MachineObj->GetDeviceWindowsColor());

   // On prend la 1ère couleur dispo pour la machine
   if (LocalMachineWindowsColorList.count() >= 1)
      return LocalMachineWindowsColorList.first();

   // Sinon on prend la couleur correspondant aux nombres de machines enregistrées
   return MachineWindowsColorList.at((f_MachinesHashTable.count() + 1) % MachineWindowsColorList.count());
}


///
/// \fn GetDefaultCommInfo
/// \brief Retourne les paramètres de communications par défaut
/// \return const CLASS_ABSTRACT_INFO_COM * : Paramètres de comm par défaut
///
const CLASS_ABSTRACT_INFO_COM * CLASS_CONFIGURATION::GetDefaultCommInfo(void)
{
   // Par défaut on souhaite créer une connexion TCP vers l'adresse 192.168.2.140
   // La suppression de l'objet devra se faire au niveau de la machine
   return new CLASS_INFO_COM_TCP_CLIENT(DEFAULT_COMM_DEVICE_IP, DEFAULT_COMM_TCP_PORT, DEFAULT_COMM_ASCII_RX_TERM, DEFAULT_COMM_ASCII_TX_TERM);
}

///
/// \fn GetDefaultGatewayInfo
/// \brief Retourne les paramètres par défaut de la passerelle
/// \return const CLASS_INFO_COM_TCP_SERVER * : Paramètres passerelle par défaut
///
const CLASS_INFO_COM_TCP_SERVER * CLASS_CONFIGURATION::GetDefaultGatewayInfo(void)
{
   // Par défaut on souhaite créer une connexion TCP serveur localhost
   // La suppression de l'objet devra se faire au niveau de la machine
   return new CLASS_INFO_COM_TCP_SERVER(DEFAULT_GATEWAY_IP, DEFAULT_GATEWAY_TCP_PORT, DEFAULT_COMM_ASCII_RX_TERM, DEFAULT_COMM_ASCII_TX_TERM);
}

///
/// \fn CreateMachine
/// \brief Création d'une machine
///
void CLASS_CONFIGURATION::CreateMachine(void)
{
   // On commence par créer la fenêtre
   CLASS_DEVICE_WINDOW * NewDeviceWindow = f_ConfigurationWindow->CreateNewDeviceWindow();

   // Création et insertion d'une classe machine dans la QHash
   f_MachinesHashTable.insert(NewDeviceWindow, new CLASS_MACHINE(NewDeviceWindow, GetDefaultMachineName(), GetDefaultMachineColor(), GetDefaultCommInfo(), GetDefaultGatewayInfo(), this));

   // Demande l'affichage de la fenêtre
   NewDeviceWindow->show();
}

///
/// \fn DeleteMachine
/// \brief Appelé lors de la suppression d'une machine
/// \param machineWindow : Fenêtre associé à la machine
///
void CLASS_CONFIGURATION::DeleteMachine(CLASS_DEVICE_WINDOW *machineWindow)
{
   // Si rien à supprimer
   if (machineWindow == nullptr)
      return;

   // On garde une référence sur la machine On déférence la machine
   CLASS_MACHINE *MachineToDelete(f_MachinesHashTable.value(machineWindow, nullptr));
   // On sort si aucune machine à supprimer
   if (MachineToDelete != nullptr)
      return;

   // Déréférence la machine
   f_MachinesHashTable.remove(machineWindow);
   // Supprime toutes connections des signaux
   machineWindow->disconnect();
   MachineToDelete->disconnect();

   // Suppression de la machine et de sa fenêtre
   machineWindow->deleteLater();
   MachineToDelete->deleteLater();
}


/******************************************************************************
 *                             Operateur surcharge                            *
 *****************************************************************************/
