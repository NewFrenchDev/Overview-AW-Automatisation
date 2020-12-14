///
/// \file DEVICE_ClassDevice.h
/// \brief Classe CLASS_MACHINE
/// \author ANALOG WAY - ADE
/// \date 2017-12-26
///

#ifndef CLASSMACHINE_H
#define CLASSMACHINE_H

// C++ Standard
// Librairies Qt
#include <QObject>
#include <QColor>
#include <QTimer>
// Modules
#include "COMMON_TypeDefs.h"
#include "VARSMANAGER_ClassVarsAsciiTranscoder.h"
// Applicatif
#include "GUI_ClassDeviceWindow.h"
#include "COMM_ClassCommManager.h"
#include "COMM_ClassAbstractInfoCom.h"
#include "Class_ConfigSource.h"

///
/// \class CLASS_MACHINE
/// \brief Classe CLASS_MACHINE
///
class CLASS_MACHINE : public QObject
{
      Q_OBJECT

   public:

      /*--------------------------- Types publiques ---------------------------*/

      /*------------------------- Constantes publiques ------------------------*/

      /*------------------------- Variables publiques -------------------------*/

      /*---------------------- Constructeur / Destructeur ---------------------*/

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
      CLASS_MACHINE(
         CLASS_DEVICE_WINDOW *machineWindow,
         const QString &machineName, const QColor &machineWindowsColor, const CLASS_ABSTRACT_INFO_COM *commInfo, const CLASS_INFO_COM_TCP_SERVER *gatewayInfo,
         QObject *parent = nullptr);

      ///
      /// \fn ~CLASS_MACHINE()
      /// \brief Destructeur de CLASS_MACHINE
      ///
      ~CLASS_MACHINE();

      /*------------------------------ Accesseurs -----------------------------*/

      ///
      /// \fn GetVarsData
      /// \brief Retourne l'objet contenant les variables de la machine
      /// \return const CLASS_VARS_DATA * : Pointe sur l'objet
      ///
      //const CLASS_VARS_DATA * GetVarsData(void) const;

      ///
      /// \fn GetDeviceName
      /// \brief Retourne le nom de la machine
      /// \return QString : Nom de la machine
      ///
      QString GetDeviceName(void) const;

      ///
      /// \fn GetDeviceWindowsColor
      /// \brief Retourne la couleur des fenêtres liées à la machine
      /// \return QColor : Couleur des fenêtres
      ///
      QColor GetDeviceWindowsColor(void) const;

      ///
      /// \fn ToJson
      /// \brief Créer l'objet JSON relatant ses données au format JSON
      /// \return QJsonObject : L'objet JSON contenant l'objet sérializé en JSON
      ///
      //QJsonObject ToJson(void) const;

      /*------------------------------ Mutateurs ------------------------------*/

      /*-------------------------- Méthodes publiques -------------------------*/

      ///
      /// \brief RemoveAllEntryFromFile
      /// \param fileName
      ///
      //void RemoveAllEntryFromFile(const QStringList &fileList);

      /*--------------------------- Signaux publiques -------------------------*/

   signals:
/*
      ///
      /// \fn SIGNAL_FilesParseDone
      /// \brief Signal émis à la suite d'une extraction
      /// \param parseStatus : Statut de l'extraction
      /// \param errorsList : Liste des messages d'erreurs/warnings
      ///
      void SIGNAL_FilesParseDone(Bool parseStatus, QStringList errorsList);

      ///
      /// \fn SIGNAL_DisplayParsedVariable
      /// \brief Signal émis à la suite d'une extraction pour afficher les variables parsées
      /// \param variablesList : Liste des variables parsées
      ///
      void SIGNAL_DisplayParsedVariables(QList<CLASS_VARS*> variablesList);*/

      /*---------------------------- Slots publiques --------------------------*/

   public slots:

      /*----------------------------- Types privés ----------------------------*/

      /*----------------------------- Slots privées ---------------------------*/

   private slots :

     /* ///
      /// \fn SLOT_FilesParseRequest
      /// \brief Demande d'extraction à partir des fichiers donnés
      /// \param filesList : Liste des fichiers
      ///
      void SLOT_FilesParseRequest(const QStringList& filesList);

      ///
      /// \fn SLOT_ParserTimerProcess
      /// \brief Process appelé en tâche de fond pour attendre la fin de l'extraction
      ///
      void SLOT_ParserTimerProcess(void);*/

      ///
      /// \fn SLOT_CreateConnectionRequest
      /// \brief Créer le moyen de communication souhaite par l'utilisateur
      /// \param commInfo : info sur moyen de communication
      ///
      void SLOT_CreateConnectionRequest(const CLASS_ABSTRACT_INFO_COM *commInfo);

      ///
      /// \fn SLOT_DisconnectConnectionRequest
      /// \brief Déconnecte le moyen de communication
      ///
      void SLOT_DisconnectConnectionRequest(void);

      ///
      /// \fn SLOT_CreateTcpGatewayServerRequest
      /// \brief Demande la création d'un serveur TCP réalisant la passerelle vers la machine et l'extérieur
      /// \param commInfo : info sur le moyen de communication
      ///
      void SLOT_CreateTcpGatewayServerRequest(const CLASS_INFO_COM_TCP_SERVER *commInfo);

      ///
      /// \fn SLOT_AskToCloseTcpGatewayServer
      /// \brief Demande la déconnexion du serveur TCP réalisant la passerelle
      ///
      void SLOT_CloseTcpGatewayServerRequest(void);

      /*--------------------------- Méthodes privées --------------------------*/

   private:

      /*------------------------------ Variables ------------------------------*/

   private:

      QString f_DeviceName;                                          ///< Nom de la machine
      QColor f_DeviceWindowsColor;                                   ///< Couleur des fenêtres liées à la machine
      //CLASS_VARS_DATA *f_CurrentVarsData;                            ///< Données liées aux variables actuellement utilisée

      //QTimer f_NewParserTimer;                                       ///< Timer utilisé pour l'exécution de la requête d'une nouvelle extraction
      //CLASS_VARS_DATA *f_ParsingReqVarsData;                         ///< Données en cours d'extraction
      //QStringList *f_ParsingReqErrorsList;                           ///< Liste des erreurs/warnings liées à la nouvelle extraction
      //CLASS_ABSTRACT_PARSER *f_ParsingReqNewParser;                  ///< Pointe sur le parser en cours d'exécution

      CLASS_COMMUNICATION_MANAGER f_CommunicationManager;            ///< Manager de communication
      CLASS_DEVICE_WINDOW *f_MachineWindow;                          ///< Class d'interface graphique de la machine

};

#endif // CLASSMACHINE_H
