///
/// \file GUI_ClassVariableTreeView.h
/// \brief Classe CLASS_VARIABLES_TREE_VIEW
/// \author ANALOG WAY - ADE
/// \date 2018-02-12
///

#ifndef GUI_CLASSVARIABLETREEVIEW_H
#define GUI_CLASSVARIABLETREEVIEW_H

// C++ Standard
// Librairies Qt
#include <QLineEdit>
#include <QTabWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QShortcut>
// Modules
#include "COMMON_TypeDefs.h"
#include "VARSMANAGER_ClassVars.h"
// Applicatif
#include "GUI_ClassAbstractSubwindow.h"

///
/// \class CLASS_VARIABLES_TREE_VIEW
/// \brief Classe CLASS_VARIABLES_TREE_VIEW
///
class CLASS_VARIABLES_TREE_VIEW : public CLASS_ABSTRACT_MDI_SUBWINDOW
{
      Q_OBJECT

   public:

      /*--------------------------- Types publiques ---------------------------*/

      ///
      /// \fn GroupInfo
      /// \brief Structure qui modélise les informations d'un groupe de variables dans l'arbre
      ///
      struct GroupInfo
      {
            QTreeWidgetItem* GroupTreeItem;
            QList<QString> *GroupIndexesHash;
      };

      ///
      /// \fn SheetInfo
      /// \brief Structure qui modélise les informations d'une feuille de variables dans l'arbre
      ///
      struct SheetInfo
      {
            QTreeWidgetItem* SheetTreeItem;
            QHash<QString, GroupInfo*> *SheetGroupsHash;
      };

      /*------------------------- Constantes publiques ------------------------*/

      /*------------------------- Variables publiques -------------------------*/

      /*---------------------- Constructeur / Destructeur ---------------------*/

      ///
      /// \fn CLASS_VARIABLES_TREE_VIEW(QObject *parent)
      /// \brief Constructeur de CLASS_VARIABLES_TREE_VIEW
      ///
      /// \param[in] parent : Objet parent
      ///
      CLASS_VARIABLES_TREE_VIEW(const QColor &windowColor, QWidget *parent = nullptr);

      ///
      /// \fn ~CLASS_VARIABLES_TREE_VIEW()
      /// \brief Destructeur de CLASS_VARIABLES_TREE_VIEW
      ///
      ~CLASS_VARIABLES_TREE_VIEW();

      /*------------------------------ Accesseurs -----------------------------*/

      /*------------------------------ Mutateurs ------------------------------*/

      /*-------------------------- Méthodes publiques -------------------------*/

      ///
      /// \fn FillTreeVariables
      /// \brief Remplie l'arbres des variables classées par feuilles / groupes / sous groupes
      /// \param variablesList : liste des variables à représenter dans l'arbre
      ///
      void FillTreeVariables(QList<CLASS_VARS *> variablesList);

      /*--------------------------- Signaux publiques -------------------------*/

   signals:

      /*---------------------------- Slots publiques --------------------------*/

   public slots:

      ///
      /// \fn SLOT_OnLineEditTextEdited
      /// \brief Slot appelé a chaque fois que le texte est modifié dans le line edit
      /// \param text : texte écrit dans le line edit par l'utilisateur
      ///
      void SLOT_OnLineEditTextEdited(const QString& text);

      ///
      /// \fn SLOT_OnSearchShortcutActivated
      /// \brief Slot appelé a chaque fois que le raccourcis (CTRL + F) est fait
      ///
      void SLOT_OnSearchShortcutActivated(void);

      /*----------------------------- Types privés ----------------------------*/

      /*--------------------------- Méthodes privées --------------------------*/

   protected:

   private:

      ///
      /// \fn SetUpUserInterface
      /// \brief Met en place l'interface graphique de la fenêtre
      ///
      void SetUpUserInterface(void);

      ///
      /// \fn AddTreeRoot
      /// \brief Ajoute un noeud principal à notre arbre
      /// \param[in] name : Nom du noeud
      /// \return QTreeWidgetItem* \e L'objet généré
      ///
      QTreeWidgetItem* AddTreeRoot(QTreeWidget* treeVariables, const QString &name);

      ///
      /// \fn AddTreeChild
      /// \brief Ajoute un noeud secondaire à notre arbre
      /// \param[in] parent : Noeud primaire
      /// \param[in] name : Nom du noeud
      /// \return QTreeWidgetItem* \e L'objet généré
      ///
      QTreeWidgetItem* AddTreeChild(QTreeWidgetItem* parent, const QString &name);

      ///
      /// \fn GetLowLevelItems
      /// \brief Récupères les items de bas niveau d'un item de l'arbre
      /// \return QList<QTreeWidgetItem*> \e Liste des items de bas niveau de l'arbre
      ///
      QList<QTreeWidgetItem*> GetLowLevelItems(QTreeWidgetItem* itemToParse);

      /*------------------------------ Variables ------------------------------*/

   protected:

   private:

      QVBoxLayout *f_WindowWidgetLayout;        ///< Layout principal de la fenêtre
      QLineEdit *f_SearchLineEdit;              ///< Line edit pour rechercher une variable en particulier
      QTabWidget *f_TabWidget;                  ///< TabWidget pour afficher les variables par feuille ou par indexes
      QTreeWidget *f_VariablesTreeWidget;       ///< Tree Widget qui affiche les variables classées par feuilles / sous groupes / groupes
      QShortcut *f_SearchShortcut;              ///< Shortcut (CTRL+F) pour mettre le focus sur le line edit pour effectuer une recherche
};

#endif // GUI_CLASSVARIABLETREEVIEW_H
