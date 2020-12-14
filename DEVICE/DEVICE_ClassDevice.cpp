///
/// \file DEVICE_ClassDevice.cpp
/// \brief Classe CLASS_MACHINE
/// \author ANALOG WAY - ADE
/// \date 2017-12-26
///
/// Fonctions de la classe CLASS_MACHINE
///

// C++ Standard
// Librairies Qt
// Modules
#include "DEVICE_ClassDevice.h"
#include "TRACES.h"
// Applicatif
//#include "VARS_ClassVarsContainer.h"
//#include "PARSERS_ClassAbstractParser.h"
//#include "PARSERS_ClassGenericVarsParser.h"


/******************************** DEFINITIONS ********************************/

/*----------------------------- Types publiques -----------------------------*/
/*--------------------------- Constantes publiques --------------------------*/
/*--------------------------- Variables publiques ---------------------------*/
/*------------------------------- Types privés ------------------------------*/
/*----------------------- Prototypes fonctions privées ----------------------*/
/*---------------------------- Constantes privées ---------------------------*/
// Période entre deux appels aux parsers en tâche de fond
#define TIMER_PARSER_REQUEST_PERIOD    100

// Clés des différents éléments faisant partie de la sérialisation JSON pour une machine
#define DEVICE_JSON_KEY_NAME                    QStringLiteral("Name")
#define DEVICE_JSON_KEY_COLOR                   QStringLiteral("Color")
#define DEVICE_JSON_KEY_GEOMETRY                QStringLiteral("Geometry")
#define DEVICE_JSON_KEY_CONSOLE_GEOMETRY        QStringLiteral("ConsoleGeometry")
#define DEVICE_JSON_KEY_CONNECTION_GEOMETRY     QStringLiteral("ConnexionGeometry")
#define DEVICE_JSON_KEY_LOAD_GEOMETRY           QStringLiteral("LoadGeometry")
#define DEVICE_JSON_KEY_CONFIG_GEOMETRY         QStringLiteral("ConfigGeometry")
#define DEVICE_JSON_KEY_CONNECTION_TYPE         QStringLiteral("ConnectionType")
#define DEVICE_JSON_KEY_CONNECTION_SETTINGS     QStringLiteral("ConnectionSettings")
#define DEVICE_JSON_KEY_TERM_MODE_ENCODE        QStringLiteral("TerminaisonEncode")
#define DEVICE_JSON_KEY_TERM_MODE_DECODE        QStringLiteral("TerminaisonDecode")
#define DEVICE_JSON_KEY_GATEWAY_SETTINGS        QStringLiteral("GatewaySettings")
#define GEOMETRY_JSON_KEY_X                     QStringLiteral("x")
#define GEOMETRY_JSON_KEY_Y                     QStringLiteral("y")
#define GEOMETRY_JSON_KEY_WIDTH                 QStringLiteral("Width")
#define GEOMETRY_JSON_KEY_HEIGHT                QStringLiteral("Height")

/*---------------------------- Variables privées ----------------------------*/

/**************************** FIN DES DEFINITIONS ****************************/

/******************************************************************************
 *                             Fonctions privées                              *
 *****************************************************************************/

/******************************************************************************
 *                         Constructeur / Destructeur                         *
 *****************************************************************************/

///
/// \fn CLASS_MACHINE(QObject *parent)
/// \brief Constructeur de CLASS_MACHINE
///
/// \param[in] machineWindow : Fenêtre graphique à lier à la machine
/// \param[in] machineName : Nom de la machine
/// \param[in] machineWindowsColor : Couleur à appliquer aux fenêtres liées à la machine
/// \param[in] commInfo : Infos sur la comm
/// \param[in] gatewayInfo : Infos sur la passerelle
/// \param[in] parent : Objet parent
///
CLASS_MACHINE::CLASS_MACHINE(
      CLASS_DEVICE_WINDOW *machineWindow,
      const QString &machineName, const QColor &machineWindowsColor, const CLASS_ABSTRACT_INFO_COM *commInfo, const CLASS_INFO_COM_TCP_SERVER *gatewayInfo,
      QObject *parent):
   QObject(parent),
   f_DeviceName(machineName),
   f_DeviceWindowsColor(machineWindowsColor),
   f_CommunicationManager(commInfo, gatewayInfo, this),
   f_MachineWindow(machineWindow)
{
   // On ne peut créer de machine sans interface graphique
   if (machineWindow == nullptr)
   {
      AwTracesLevelError(AW_TRACE_SRC_INFO % QStringLiteral("Can't create a machine without a valid graphic window"));
      return;
   }

   // On connecte les signaux pour la création des moyens de communications et de leurs déconnexions
   QObject::connect(f_MachineWindow->GetCommWindowObj(), &CLASS_COMMUNICATION_WINDOW::SIGNAL_CreateConnectionRequest, this, &CLASS_MACHINE::SLOT_CreateConnectionRequest);
   QObject::connect(f_MachineWindow->GetCommWindowObj(), &CLASS_COMMUNICATION_WINDOW::SIGNAL_DisconnectConnectionRequest, this, &CLASS_MACHINE::SLOT_DisconnectConnectionRequest);
   QObject::connect(f_MachineWindow->GetCommWindowObj(), &CLASS_COMMUNICATION_WINDOW::SIGNAL_CreateGatewayRequest, this, &CLASS_MACHINE::SLOT_CreateTcpGatewayServerRequest);
   QObject::connect(f_MachineWindow->GetCommWindowObj(), &CLASS_COMMUNICATION_WINDOW::SIGNAL_CloseGatewayRequest, this, &CLASS_MACHINE::SLOT_CloseTcpGatewayServerRequest);

   // Avant de connecter une interface de communication, les données envoyées en direct par la console sont rejetés
   QObject::connect(f_MachineWindow->GetSourceWindowObj(), &CLASS_SOURCE_WINDOW::SIGNAL_SendRawCommand, f_MachineWindow->GetSourceWindowObj(), &CLASS_SOURCE_WINDOW::SLOT_NewTxDataRefused, Qt::QueuedConnection);
   QObject::connect(f_MachineWindow->GetReceiverWindowObj(), &CLASS_RECEIVER_WINDOW::SIGNAL_SendRawCommand, f_MachineWindow->GetReceiverWindowObj(), &CLASS_RECEIVER_WINDOW::SLOT_NewTxDataRefused, Qt::QueuedConnection);

   // Informe le gestionnaire de fenêtres du nom et de la couleur liés à la machine
   f_MachineWindow->SetDisplayedWindowName(f_DeviceName);
   f_MachineWindow->SetWindowColor(f_DeviceWindowsColor);
}

///
/// \fn ~CLASS_MACHINE()
/// \brief Destructeur de CLASS_MACHINE
///
CLASS_MACHINE::~CLASS_MACHINE()
{
}

/******************************************************************************
 *                                 Accesseurs                                 *
 *****************************************************************************/

///
/// \fn GetDeviceName
/// \brief Retourne le nom de la machine
/// \return QString : Nom de la machine
///
QString CLASS_MACHINE::GetDeviceName(void) const
{
   return f_DeviceName;
}

///
/// \fn GetDeviceWindowsColor
/// \brief Retourne la couleur des fenêtres liées à la machine
/// \return QColor : Couleur des fenêtres
///
QColor CLASS_MACHINE::GetDeviceWindowsColor(void) const
{
   return f_DeviceWindowsColor;
}

/******************************************************************************
 *                               Slots publiques                              *
 *****************************************************************************/

/******************************************************************************
 *                               Slots privées                                *
 *****************************************************************************/
///
/// \fn SLOT_AskToCreateConnection
/// \brief Créer le moyen de communication souhaite par l'utilisateur
/// \param commInfo : info sur moyen de communication
///
void CLASS_MACHINE::SLOT_CreateConnectionRequest(const CLASS_ABSTRACT_INFO_COM* commInfo)
{
   // On demande la création du lien de communication
   if (f_CommunicationManager.CreateCommPort(commInfo) == true)
   {
      // On connecte le signal pour l'envoie de commandes depuis la fenêtre de test
      QObject::connect(f_MachineWindow->GetSourceWindowObj(), &CLASS_SOURCE_WINDOW::SIGNAL_SendRawCommand, f_CommunicationManager.GetVarsInterface(), &CLASS_ABSTRACT_VARS_IF::SendRawData, Qt::QueuedConnection);
      QObject::disconnect(f_MachineWindow->GetSourceWindowObj(), &CLASS_SOURCE_WINDOW::SIGNAL_SendRawCommand, f_MachineWindow->GetSourceWindowObj(), &CLASS_SOURCE_WINDOW::SLOT_NewTxDataRefused);
      QObject::connect(f_CommunicationManager.GetVarsInterface()->GetUniversalPortObj(), &CLASS_UNIVERSALPORT::SIGNAL_TxSpyNewData, f_MachineWindow->GetSourceWindowObj(), &CLASS_SOURCE_WINDOW::SLOT_NewTxDataVailable, Qt::QueuedConnection);
      QObject::connect(f_CommunicationManager.GetVarsInterface()->GetUniversalPortObj(), &CLASS_UNIVERSALPORT::SIGNAL_TxSpyNewRefusedData, f_MachineWindow->GetSourceWindowObj(), &CLASS_SOURCE_WINDOW::SLOT_NewTxDataRefused, Qt::QueuedConnection);
      QObject::connect(f_CommunicationManager.GetVarsInterface()->GetUniversalPortObj(), &CLASS_UNIVERSALPORT::SIGNAL_NewDataAvailable, f_MachineWindow->GetSourceWindowObj(), &CLASS_SOURCE_WINDOW::SLOT_NewRxDataAvailable, Qt::QueuedConnection);


      // On connecte le signal pour l'envoie de commandes depuis la fenêtre de l'appareil receveur
      QObject::connect(f_MachineWindow->GetReceiverWindowObj(), &CLASS_RECEIVER_WINDOW::SIGNAL_SendRawCommand, f_CommunicationManager.GetVarsInterface(), &CLASS_ABSTRACT_VARS_IF::SendRawData, Qt::QueuedConnection);
      QObject::disconnect(f_MachineWindow->GetReceiverWindowObj(), &CLASS_RECEIVER_WINDOW::SIGNAL_SendRawCommand, f_MachineWindow->GetReceiverWindowObj(), &CLASS_RECEIVER_WINDOW::SLOT_NewTxDataRefused);
      QObject::connect(f_CommunicationManager.GetVarsInterface()->GetUniversalPortObj(), &CLASS_UNIVERSALPORT::SIGNAL_TxSpyNewData, f_MachineWindow->GetReceiverWindowObj(), &CLASS_RECEIVER_WINDOW::SLOT_NewTxDataVailable, Qt::QueuedConnection);
      QObject::connect(f_CommunicationManager.GetVarsInterface()->GetUniversalPortObj(), &CLASS_UNIVERSALPORT::SIGNAL_TxSpyNewRefusedData, f_MachineWindow->GetReceiverWindowObj(), &CLASS_RECEIVER_WINDOW::SLOT_NewTxDataRefused, Qt::QueuedConnection);
      QObject::connect(f_CommunicationManager.GetVarsInterface()->GetUniversalPortObj(), &CLASS_UNIVERSALPORT::SIGNAL_NewDataAvailable, f_MachineWindow->GetReceiverWindowObj(), &CLASS_RECEIVER_WINDOW::SLOT_NewRxDataAvailable, Qt::QueuedConnection);

      // On connecte la fenêtre de communication sur le statut de l'interface de comm
      QObject::connect(f_CommunicationManager.GetVarsInterface()->GetUniversalPortObj(), &CLASS_UNIVERSALPORT::SIGNAL_StatusChanged, f_MachineWindow->GetCommWindowObj(), &CLASS_COMMUNICATION_WINDOW::ConnectionStatusChanged);
      f_MachineWindow->GetCommWindowObj()->ConnectionStatusChanged(f_CommunicationManager.GetVarsInterface()->GetUniversalPortObj()->GetMessageStatus());
   }
   else
   {
      f_MachineWindow->GetCommWindowObj()->ConnectionStatusChanged(CLASS_UNIVERSALPORT::UPORT_CONNECTION_STATE_DISCONNECTED);
   }

   return;
}

///
/// \fn SLOT_DisconnectConnectionRequest
/// \brief Déconnecte le moyen de communication
///
void CLASS_MACHINE::SLOT_DisconnectConnectionRequest(void)
{
   // On demande la déconnexion des interfaces de communications
   f_CommunicationManager.DisconnectCommunication();

   // On déconnecte les signaux d'envoi de données à la fenêtre de TEST
   QObject::connect(f_MachineWindow->GetSourceWindowObj(), &CLASS_SOURCE_WINDOW::SIGNAL_SendRawCommand, f_MachineWindow->GetSourceWindowObj(), &CLASS_SOURCE_WINDOW::SLOT_NewTxDataRefused, Qt::QueuedConnection);
   QObject::disconnect(f_MachineWindow->GetSourceWindowObj(), &CLASS_SOURCE_WINDOW::SIGNAL_SendRawCommand, f_CommunicationManager.GetVarsInterface(), &CLASS_ABSTRACT_VARS_IF::SendRawData);
   QObject::disconnect(f_CommunicationManager.GetVarsInterface()->GetUniversalPortObj(), &CLASS_UNIVERSALPORT::SIGNAL_TxSpyNewData, f_MachineWindow->GetSourceWindowObj(), &CLASS_SOURCE_WINDOW::SLOT_NewTxDataVailable);
   QObject::disconnect(f_CommunicationManager.GetVarsInterface()->GetUniversalPortObj(), &CLASS_UNIVERSALPORT::SIGNAL_TxSpyNewRefusedData, f_MachineWindow->GetSourceWindowObj(), &CLASS_SOURCE_WINDOW::SLOT_NewTxDataRefused);
   QObject::disconnect(f_CommunicationManager.GetVarsInterface()->GetUniversalPortObj(), &CLASS_UNIVERSALPORT::SIGNAL_NewDataAvailable, f_MachineWindow->GetSourceWindowObj(), &CLASS_SOURCE_WINDOW::SLOT_NewRxDataAvailable);

   // On déconnecte les signaux d'envoi de données à la fenêtre de reception
   QObject::connect(f_MachineWindow->GetReceiverWindowObj(), &CLASS_RECEIVER_WINDOW::SIGNAL_SendRawCommand, f_MachineWindow->GetReceiverWindowObj(), &CLASS_RECEIVER_WINDOW::SLOT_NewTxDataRefused, Qt::QueuedConnection);
   QObject::disconnect(f_MachineWindow->GetReceiverWindowObj(), &CLASS_RECEIVER_WINDOW::SIGNAL_SendRawCommand, f_CommunicationManager.GetVarsInterface(), &CLASS_ABSTRACT_VARS_IF::SendRawData);
   QObject::disconnect(f_CommunicationManager.GetVarsInterface()->GetUniversalPortObj(), &CLASS_UNIVERSALPORT::SIGNAL_TxSpyNewData, f_MachineWindow->GetReceiverWindowObj(), &CLASS_RECEIVER_WINDOW::SLOT_NewTxDataVailable);
   QObject::disconnect(f_CommunicationManager.GetVarsInterface()->GetUniversalPortObj(), &CLASS_UNIVERSALPORT::SIGNAL_TxSpyNewRefusedData, f_MachineWindow->GetReceiverWindowObj(), &CLASS_RECEIVER_WINDOW::SLOT_NewTxDataRefused);
   QObject::disconnect(f_CommunicationManager.GetVarsInterface()->GetUniversalPortObj(), &CLASS_UNIVERSALPORT::SIGNAL_NewDataAvailable, f_MachineWindow->GetReceiverWindowObj(), &CLASS_RECEIVER_WINDOW::SLOT_NewRxDataAvailable);

   f_MachineWindow->GetCommWindowObj()->ConnectionStatusChanged(CLASS_UNIVERSALPORT::UPORT_CONNECTION_STATE_DISCONNECTED);

   return;
}

///
/// \fn SLOT_CreateTcpGatewayServerRequest
/// \brief Demande la création d'un serveur TCP réalisant la passerelle vers la machine et l'extérieur
/// \param commInfo : info sur le moyen de communication
///
void CLASS_MACHINE::SLOT_CreateTcpGatewayServerRequest(const CLASS_INFO_COM_TCP_SERVER *commInfo)
{
   if (f_CommunicationManager.CreateTcpGateway(commInfo) == true)
   {
      // On connecte la fenêtre de communication sur le statut de l'interface de comm
      QObject::connect(f_CommunicationManager.GetGatewayCommObj(), &CLASS_UNIVERSALPORT::SIGNAL_StatusChanged, f_MachineWindow->GetCommWindowObj(), &CLASS_COMMUNICATION_WINDOW::ConnectionStatusChanged);
      f_MachineWindow->GetCommWindowObj()->GatewayStatusChanged(f_CommunicationManager.GetGatewayCommObj()->GetMessageStatus());
   }
   else
   {
      f_MachineWindow->GetCommWindowObj()->GatewayStatusChanged(CLASS_UNIVERSALPORT::UPORT_CONNECTION_STATE_DISCONNECTED);
   }  
   return;
}

///
/// \fn SLOT_CloseTcpGatewayServerRequest
/// \brief Demande la déconnexion du serveur TCP réalisant la passerelle
///
void CLASS_MACHINE::SLOT_CloseTcpGatewayServerRequest(void)
{
   // On ferme le server TCP
   f_CommunicationManager.CloseTcpGateway();
   f_MachineWindow->GetCommWindowObj()->GatewayStatusChanged(CLASS_UNIVERSALPORT::UPORT_CONNECTION_STATE_DISCONNECTED);
   return;
}

/******************************************************************************
 *                              Méthodes privées                              *
 *****************************************************************************/

/******************************************************************************
 *                             Operateur surcharge                            *
 *****************************************************************************/
