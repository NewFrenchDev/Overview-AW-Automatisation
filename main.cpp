///
/// \file main.cpp
/// \brief Point d'entrée du programme
/// \author ANALOG WAY - ADE
/// \date 2017-12-26
///


// C++ Standard
// Librairies Qt
#include <QApplication>
#include <QThread>
#include <QDir>
// Modules
#include "COMMON_TypeDefs.h"
// Applicatif
#include "ClassAutomatisation.h"
#include "ClassWriteMachineConfig.h"
#include "global_windows_variables.h"

/************************* DEFINITIONS *****************************/
// Nom du thread principal
#define  THREAD_MAIN_NAME           QStringLiteral("THREAD_MAIN")

/*-------------------- Variables publiques ------------------------*/
/*--------------------- Constantes publiques ----------------------*/
/*------------------------ Types privés ---------------------------*/
/*---------------- Prototypes fonctions privées -------------------*/
/*--------------------- Constantes privées ------------------------*/
/*--------------------- Variables privées -------------------------*/

/*********************** FIN DES DEFINITIONS ***********************/

/********************************************************************
 *                    FONCTIONS PRIVEES                             *
 *******************************************************************/

/*******************************************************************
 *                     FONCTIONS PUBLIQUES                         *
 *******************************************************************/

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);

   // On nomme le thread courant
   QThread::currentThread()->setObjectName(THREAD_MAIN_NAME);

   // Création de répertoire pour les settings machine et les logs des tests
   QDir SettingDirectory, LogDirectory;
   SettingDirectory.mkdir("settings");
   if(!SettingDirectory.exists())
      qWarning("Dossier introuvable");
   LogDirectory.mkdir("log");
   if(!LogDirectory.exists())
      qWarning("Dossier introuvable");

   // Création de la classe graphique maitresse de Automatisation
   CLASS_AUTOMATISATION Automatisation;

   CLASS_WRITE_MACHINE_CONFIG WriteMachineConfig;

   return a.exec();
}
