///
/// \file Class_CustomFormat.h
/// \brief Classe CLASS_CUSTOM_FORMAT
/// \author ANALOG WAY - GLE
/// \date 2018-06-08
///

#ifndef CLASS_CUSTOMFORMAT_H
#define CLASS_CUSTOMFORMAT_H

// C++ Standard
// Librairies Qt
#include <QObject>
#include <QSettings>
// Modules
#include "COMMON_TypeDefs.h"
#include "global_windows_variables.h"

///
/// \class CLASS_CUSTOM_FORMAT
/// \brief Classe CLASS_CUSTOM_FORMAT
///
class CLASS_CUSTOM_FORMAT : public QObject
{
      Q_OBJECT

   public:

      /*--------------------------- Types publiques ---------------------------*/

      /*------------------------- Constantes publiques ------------------------*/

      /*------------------------- Variables publiques -------------------------*/



      /*---------------------- Constructeur / Destructeur ---------------------*/

      ///
      /// \fn CLASS_CUSTOM_FORMAT(QObject *parent)
      /// \brief Constructeur de CLASS_CUSTOM_FORMAT
      ///
      /// \param[in] parent : Objet parent
      ///
      CLASS_CUSTOM_FORMAT(QObject *parent = nullptr);

      ///
      /// \fn ~CLASS_CUSTOM_FORMAT()
      /// \brief Destructeur de CLASS_CUSTOM_FORMAT
      ///
      ~CLASS_CUSTOM_FORMAT();

      /*------------------------------ Accesseurs -----------------------------*/

      ///
      /// \fn LoadConfigFileForCustomFormat
      /// \brief Charge le fichier config adéquate pour l'éxécution des autres fonctions
      ///
      void LoadConfigFileForCustomFormat(void);

      ///
      /// \brief GetCustomFormatSetting
      /// \return
      ///
      QSettings *GetCustomFormatSetting();

      ///
      /// \fn SelectSlot0
      /// \brief Pour dire à la machine de sélection le SLOT 0 pour pouvoir stocker notre format custom
      /// \return la commande en QByteArray
      ///
      QByteArray SelectSlot0(void);

      ///
      /// \fn SelectSlot1
      /// \brief Pour dire à la machine de sélection le SLOT 1 pour pouvoir stocker notre format custom
      /// \return la commande en QByteArray
      ///
      QByteArray SelectSlot1(void);

      ///
      /// \fn CheckStatus
      /// \brief Permet de vérifier si le format créer est conforme
      ///        Etape obligatoire avant de sauvegarder le format
      /// \return la commande en QByteArray
      ///
      QByteArray CheckStatus(void);

      ///
      /// \fn SaveCustomFormat
      /// \brief Sauvegarde le format dans le slot précédemment sélectionner
      /// \return la commande en QByteArray
      ///
      QByteArray SaveCustomFormat(void);


      ///
      /// \fn LoadActiveH
      /// \brief Creer la commande pour charger un nombre pour l'ActiveH
      /// \return la commande en QByteArray
      ///
      QByteArray LoadActiveH(void);

      ///
      /// \fn LoadFrontPorchH
      /// \brief Creer la commande pour charger un nombre pour le FrontPorchH
      /// \return la commande en QByteArray
      ///
      QByteArray LoadFrontPorchH(void);

      ///
      /// \fn LoadBackPorchH
      /// \brief Creer la commande pour charger un nombre pour le BackPorchH
      /// \return la commande en QByteArray
      ///
      QByteArray LoadBackPorchH(void);

      ///
      /// \fn LoadSynchroH
      /// \brief Creer la commande pour charger un nombre pour le BackPorchH
      /// \return la commande en QByteArray
      ///
      QByteArray LoadSynchroH(void);

      ///
      /// \fn LoadPolarityH
      /// \brief Creer la commande pour charger un nombre pour le SynchroH
      /// \return la commande en QByteArray
      ///
      QByteArray LoadPolarityH(void);

      ///
      /// \fn LoadActiveV
      /// \brief Creer la commande pour charger un nombre pour le ActiveV
      /// \return la commande en QByteArray
      ///
      QByteArray LoadActiveV(void);

      ///
      /// \fn LoadFrontPorchV
      /// \brief Creer la commande pour charger un nombre pour le FrontPorchV
      /// \return la commande en QByteArray
      ///
      QByteArray LoadFrontPorchV(void);

      ///
      /// \fn LoadBackPorchV
      /// \brief Creer la commande pour charger un nombre pour le BackPorchV
      /// \return la commande en QByteArray
      ///
      QByteArray LoadBackPorchV(void);

      ///
      /// \fn LoadSynchroV
      /// \brief Creer la commande pour charger un nombre pour le SynchroV
      /// \return la commande en QByteArray
      ///
      QByteArray LoadSynchroV(void);

      ///
      /// \fn LoadPolarityV
      /// \brief Creer la commande pour charger un nombre pour le PolarityV
      /// \return la commande en QByteArray
      ///
      QByteArray LoadPolarityV(void);

      ///
      /// \fn LoadRandomRate
      /// \brief Creer la commande pour charger un nombre pour le Rate
      /// \return la commande en QByteArray
      ///
      QByteArray LoadRandomRate(void);

      ///
      /// \brief GetPixels
      /// \return
      ///
      QByteArray GetPixels(void);

      ///
      /// \brief GetLines
      /// \return
      ///
      QByteArray GetLines(void);

      ///
      /// \fn NextCustomFormat
      /// \brief Permet de passer au format suivant stocker dans la liste f_ListCustomFormatGenerated
      ///
      void NextCustomFormat(void);

      ///
      /// \brief NextFormatInfo
      ///
      void NextFormatInfo(void);

      ///
      /// \brief ReinitializeIndexCustomList
      ///
      void ReinitializeIndexCustomList(void);

      ///
      /// \fn GenerateRandomFormat
      /// \brief Génèrer 20 formats custom aléatoires
      ///
      void GenerateRandomFormat(void);

      /*------------------------------ Mutateurs ------------------------------*/

      /*-------------------------- MÃ©thodes publiques -------------------------*/

      /*--------------------------- Signaux publiques -------------------------*/

   signals:

      /*---------------------------- Slots publiques --------------------------*/

   public slots:



      /*----------------------------- Types privÃ©s ----------------------------*/

   private:

       struct CUSTOM_FORMAT                                       ///< Structure pour stocker toutes les informations utiles a un format custom créé
      {
         SDWord ActiveH;
         SDWord FrontPorchH;
         SDWord BackPorchH;
         SDWord SynchroH;
         SDWord PolarityH;
         SDWord ActiveV;
         SDWord FrontPorchV;
         SDWord BackPorchV;
         SDWord SynchroV;
         SDWord PolarityV;
         SDWord Rate;
      };



      /*--------------------------- Méthodes privées --------------------------*/

   protected:

   private:

      /*------------------------------ Variables ------------------------------*/

   protected:

   private:

       QSettings *f_settingSource;                                ///< Pour récupérer le fichier de config adéquate


   public:
       Bool f_slot1Selected;
       QList<CUSTOM_FORMAT> f_ListCustomFormatGenerated;          ///< Pour stocker 20 formats custom générer aléatoirement
       SDWord f_indexListFormatGenerated;                            ///< Pour se déplacer dans la liste de format
       SDWord f_indexListInfo;
       QString SourceName;
       QString ReceiverName;

};

#endif // CLASS_CUSTOMFORMAT_H
