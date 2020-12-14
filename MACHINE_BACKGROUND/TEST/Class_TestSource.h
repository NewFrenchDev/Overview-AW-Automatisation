///
/// \file Class_TestSource.h
/// \brief Classe CLASS_TEST_SOURCE
/// \author ANALOG WAY - GLE
/// \date 2018-07-04
///

#ifndef CLASS_TESTSOURCE_H
#define CLASS_TESTSOURCE_H

// C++ Standard
// Librairies Qt
#include <QObject>
#include <QSettings>
#include <QFile>
// Modules
#include "COMMON_TypeDefs.h"
#include "global_windows_variables.h"
#include "Class_CustomFormat.h"
#include "Class_ConfigSource.h"

///
/// \class CLASS_TEST_SOURCE
/// \brief Classe CLASS_TEST_SOURCE
///
class CLASS_TEST_SOURCE : public QObject
{
      Q_OBJECT

   public:

      /*--------------------------- Types publiques ---------------------------*/

      // Etape des customs
      typedef enum
      {
         NONE,
         SELECT_SLOT,
         RANDOM_RATE,
         ACTIVE_H,
         FRONT_PORCH_H,
         BACK_PORCH_H,
         SYNCHRO_H,
         POLARITY_H,
         ACTIVE_V,
         FRONT_PORCH_V,
         BACK_PORCH_V,
         SYNCHRO_V,
         POLARITY_V,
         CHECK_CUSTOM,
         SAVE_CUSTOM,
         NEXT_CUSTOM,
      } CUSTOM_STEP;

      /*------------------------- Constantes publiques ------------------------*/

      /*------------------------- Variables publiques -------------------------*/

      /*---------------------- Constructeur / Destructeur ---------------------*/

      ///
      /// \fn CLASS_TEST_SOURCE()
      /// \brief Constructeur de CLASS_TEST_SOURCE
      ///
      ///
      CLASS_TEST_SOURCE();

      ///
      /// \fn ~CLASS_TEST_SOURCE()
      /// \brief Destructeur de CLASS_TEST_SOURCE
      ///
      ~CLASS_TEST_SOURCE();

      /*------------------------------ Accesseurs -----------------------------*/

      /*------------------------------ Mutateurs ------------------------------*/

      /*-------------------------- MÃ©thodes publiques -------------------------*/

      ///
      /// \fn AccessToConfigSourceData
      /// \brief Récupère le pointeur sur le configSource créé
      /// \param configSourceToAccess : le configSource à accéder pour les informations
      ///
      void AccessToConfigSourceData(CLASS_CONFIG_SOURCE *configSourceToAccess);

      ///
      /// \fn SendFormat
      /// \brief Indique a la fenetre d'envoyer le format
      ///
      void SendFormat(void);

      ///
      /// \fn SendRate
      /// \brief Indique à la fenetre d'envoyer le rate
      ///
      void SendRate(void);

      ///
      /// \fn CheckOutputPlugStatus
      /// \brief Indique à la fenetre de vérifier le status du plug
      ///
      void CheckOutputPlugStatus(void);

      ///
      /// \fn WriteFileLog
      /// \brief Ecrit dans le log de la machine source
      ///
      void WriteFileLog(void);

      ///
      /// \fn AddOutputAndFormatTagInLogFile
      /// \brief Ecrit le tag de l'Output et du format dans le fichier log
      ///
      void AddOutputAndFormatTagInLogFile(void);

      ///
      /// \fn CreateListFormat
      /// \brief Crée la liste de format
      /// \param outputNumber : nombre d'output
      /// \param numberFormat : nombre de format
      ///
      void CreateListFormat(int outputNumber, int formatNumber);

      ///
      /// \fn CreateListRate
      /// \brief Crée la liste de rate
      /// \param outputNumber : nombre d'output
      /// \param rateNumber : nombre de rate
      ///
      void CreateListRate(int outputNumber, int rateNumber);

      ///
      /// \fn ListOutputFromConfigWindow
      /// \brief Permet de créer notre liste d'output à partir des données stockées dans la fenêtre de configuration
      /// \return Liste de QString
      ///
      QStringList ListOutputFromConfigWindow(void);

      ///
      /// \fn ListPlugFromConfigWindow
      /// \brief Permet de créer notre liste d'output à partir des données stockées dans la fenêtre de configuration
      /// \return Liste de QString
      ///
      QStringList ListPlugFromConfigWindow(void);

      ///
      /// \fn GenerateCustomFormatList
      /// \brief Génère une liste de format custom
      ///
      void GenerateCustomFormatList(void);

      ///
      /// \fn  CreateCommandListCustomFormat
      /// \brief Créer une liste de commande pour l'application des formats custom
      /// \param outputNumber : nombre d'output
      ///
      void CreateCommandListCustomFormat(int outputNumber);

      ///
      /// \fn SelectCustomSlot
      /// \brief Selectionne le slot à utiliser
      ///
      void SelectCustomSlot(void);

      ///
      /// \fn SetRandomRate
      /// \brief Applique un rate aléatoire
      ///
      QByteArray SetRandomRate(void);

      ///
      /// \fn SetActiveH
      /// \brief Applique le nouveau ActiveH
      ///
      QByteArray SetActiveH(void);

      ///
      /// \fn SetFrontPorchH
      /// \brief Applique le nouveau FrontPorchH
      ///
      QByteArray SetFrontPorchH(void);

      ///
      /// \fn SetBackPorchH
      /// \brief Applique le nouveau BackPorchH
      ///
      QByteArray SetBackPorchH(void);

      ///
      /// \fn SetSynchroH
      /// \brief Applique la nouvelle SynchroH
      ///
      QByteArray SetSynchroH(void);

      ///
      /// \fn SetPolarityH
      /// \brief Applique la PolarityH
      ///
      QByteArray SetPolarityH(void);

      ///
      /// \fn SetActiveV
      /// \brief Applique le nouveau ActiveV
      ///
      QByteArray SetActiveV(void);

      ///
      /// \fn FrontPorchV
      /// \brief Applique le nouveau FrontPorchV
      ///
      QByteArray SetFrontPorchV(void);

      ///
      /// \fn SetBackPorchV
      /// \brief Applique le nouveau BackPorchV
      ///
      QByteArray SetBackPorchV(void);

      ///
      /// \fn SetSynchroV
      /// \brief Applique la nouvelle SynchroV
      ///
      QByteArray SetSynchroV(void);

      ///
      /// \fn SetPolarityV
      /// \brief Applique la PolarityV
      ///
      QByteArray SetPolarityV(void);

      ///
      /// \fn CheckCustomStatus
      /// \brief Applique la vérification du format obligatoire avant la sauvegarde
      ///
      QByteArray CheckCustomStatus(void);

      ///
      /// \fn SaveCustomFormat
      /// \brief Sauvegarde le format dans le slot sélectionné
      ///
      QByteArray SaveCustomFormat(void);

      ///
      /// \fn NextCustom
      /// \brief Passe au custom suivant suivi de son envoi à la machine précédé du chargement du second slot si besoin
      ///
      void NextCustom(void);


      /*--------------------------- Signaux publiques -------------------------*/

   signals:

      ///
      /// \fn SIGNAL_CurrentOutput
      /// \brief Permet de transmettre la sortie actuel à la fenêtre de configuration
      /// \param currentOutput
      ///
      void SIGNAL_CurrentOutput(const QString &currentOutput);

      ///
      /// \fn SIGNAL_CurrentFormat
      /// \brief Permet de transmettre le format actuel à la fenetre de configuration
      /// \param currentFormat
      ///
      void SIGNAL_CurrentFormat(const QString &currentFormat);

      ///
      /// \fn SIGNAL_CurrentRate
      /// \brief Permet de transmettre le rate actuel à la fenêtre de configuration
      /// \param currentRate
      ///
      void SIGNAL_CurrentRate(const QString &currentRate);

      ///
      /// \fn SIGNAL_Start
      /// \brief Permet de lier la fenetre de l'appareil source au bouton start de l'appareil receiver à travers la fenetre de configuration
      ///
      void SIGNAL_Start(void);

      ///
      /// \fn SIGNAL_GetOuputInfo
      /// \brief Permet de lancer la fonction WriteOutputInfo de la fenetre receiver à travers la fenetre de configuration
      ///
      void SIGNAL_GetOutputInfo(void);

      ///
      /// \fn SIGNAL_NextOutputApplied
      /// \brief Permet d'indiquer à la fenetre receiver de passer à l'ensemble d'input suivant
      ///
      void SIGNAL_NextOutputApplied(void);

      ///
      /// \brief SIGNAL_Stop
      ///
      void SIGNAL_Stop(void);

      ///
      /// \brief SIGNAL_SendFormat
      ///
      void SIGNAL_SendFormat(void);

      ///
      /// \brief SIGNAL_SendRate
      ///
      void SIGNAL_SendRate(void);

      ///
      /// \brief SIGNAL_StatusPlug
      ///
      void SIGNAL_StatusPlug(void);

      ///
      /// \brief SIGNAL_NextPlugToTest
      ///
      void SIGNAL_NextPlugToTest(void);

      ///
      /// \brief SIGNAL_CustomSlotSelectionned
      /// \param command
      ///
      void SIGNAL_CustomSlotSelectionned(const QByteArray &command);

      void SIGNAL_ChangeToRateCustom(void);

      ///
      /// \brief SIGNAL_RetardLaunchStatusPlug
      ///
      void SIGNAL_RetardLaunchStatusPlug(void);

      ///
      /// \brief SIGNAL_AddTimeValue
      /// \param value
      ///
      void SIGNAL_AddTimeValue(int value);

      /*---------------------------- Slots publiques --------------------------*/

   public slots:

      /*----------------------------- Types privÃ©s ----------------------------*/

   private:



      /*--------------------------- MÃ©thodes privÃ©es --------------------------*/

   protected:

   private:



      /*------------------------------ Variables ------------------------------*/

   protected:

   private:


   public:

      SDWord f_IndexListFormat;                          ///< Indication sur l'index de la liste de format
      SDWord f_IndexListRate;                            ///< Indication sur l'index de la liste de rate
      QList<QByteArray> f_FormatList;                    ///< Liste de format
      QList<QByteArray> f_RateList;                      ///< Liste de rate

      SDWord f_CurrentFormat;                            ///< Stocke valeur du format actuel
      SDWord f_CurrentRate;                              ///< Stocke valeur du rate actuel
      SDWord f_CurrentOutputPlugValue;                   ///< Stocke valeur du plug de sortie actuel
      SDWord f_CurrentOutput;                            ///< Indicateur sur la sortie dans la partie lancement

      QStringList f_ListOutput;                          ///< Pour stocker la liste créer dans la fonction ListOutputFromConfigWindow
      SDWord f_IndexListOutput;                          ///< Pour se déplacer dans la liste f_ListOutput

      QString f_CurrentPlug;                             ///< Indique le plug actuellement utilisé
      QStringList f_ListPlug;                            ///< Pour stocker la liste créer dans la fonction ListPlugFromConfigWindow
      SDWord f_IndexListPlug;                            ///< Pour se déplacer dans la liste f_ListPlug

      Bool f_TwoSlotSaved;                               ///< Indique si les deux slot ont été utilisé ou non
      Bool f_FirstCustomSlot;                            ///< Indicateur premier slot custom est sélectionné
      Bool f_FirstLaunch;                                ///< Indicateur premier lancement

      CLASS_CUSTOM_FORMAT *f_ListCustomFormat;           ///< Liste de format custom générée aléatoirement

      Byte f_InfoCompatibility;                          ///< Information sur la compatibilité format et rate
      QString f_PlugStatus;                              ///< Stocke la réponse de la machine sur le plug
      QByteArray f_OutputValue;                          ///< Récupère la valeur de l'output
      QByteArray f_OutputPlugValue;                      ///< Récupère la valeur du plug

      CUSTOM_STEP f_StepCustom;                          ///< Information sur la fonction en cours en mode custom

      SDWord f_Time;                                     ///< Temps pour lancer les fonctions associées aux timers de la fenetre graphique source

      CLASS_CONFIG_SOURCE *f_ConfigSourceAccessed;       ///< Pointeur pour accéder à la class ConfigSource du thread source
};

#endif // CLASS_TESTSOURCE_H
