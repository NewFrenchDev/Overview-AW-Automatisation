///
/// \file ClassWriteMachineConfig.h
/// \brief Classe CLASS_WRITE_MACHINE_CONFIG
/// \author ANALOG WAY - GLE
/// \date 2018-06-06
///

#ifndef CLASSWRITEMACHINECONFIG_H
#define CLASSWRITEMACHINECONFIG_H

// C++ Standard
// Librairies Qt
#include <QObject>
#include <QFile>
#include <QSettings>
// Modules
#include "COMMON_TypeDefs.h"

///
/// \class CLASS_WRITE_MACHINE_CONFIG
/// \brief Classe CLASS_WRITE_MACHINE_CONFIG
///
class CLASS_WRITE_MACHINE_CONFIG : public QObject
{
      Q_OBJECT

   public:

      /*--------------------------- Types publiques ---------------------------*/

      /*------------------------- Constantes publiques ------------------------*/

      /*------------------------- Variables publiques -------------------------*/

      /*---------------------- Constructeur / Destructeur ---------------------*/

      ///
      /// \fn CLASS_WRITE_MACHINE_CONFIG(QObject *parent)
      /// \brief Constructeur de CLASS_WRITE_MACHINE_CONFIG
      ///
      /// \param[in] parent : Objet parent
      ///
      CLASS_WRITE_MACHINE_CONFIG(QObject *parent = nullptr);

      ///
      /// \fn ~CLASS_WRITE_MACHINE_CONFIG()
      /// \brief Destructeur de CLASS_WRITE_MACHINE_CONFIG
      ///
      ~CLASS_WRITE_MACHINE_CONFIG();

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
      /// \fn WriteSettings
      /// \brief Ecrit le fichier de config du Vio dans le dossier settings
      ///
      void WriteSettingsVio(void);

      ///
      /// \fn WriteSettingsLivecore
      /// \brief Ecrit le fichier de config du Livecore dans le dossier settings
      ///
      void WriteSettingsLivecore(void);

      ///
      /// \fn WriteSettingsNLC
      /// \brief Ecrit le fichier de config du NLC dans le dossier settings
      ///
      void WriteSettingsNLC(void);

      ///
      /// \fn WriteSettingsMNG
      /// \brief Ecrit le fichier de config du MNG dans le dossier settings
      ///
      void WriteSettingsMNG(void);

      /*------------------------------ Variables ------------------------------*/

   protected:

   private:

};

#endif // CLASSWRITEMACHINECONFIG_H
