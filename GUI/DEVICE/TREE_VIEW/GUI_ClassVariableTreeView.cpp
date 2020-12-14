///
/// \file GUI_ClassVariableTreeView.cpp
/// \brief Classe CLASS_VARIABLES_TREE_VIEW
/// \author ANALOG WAY - ADE
/// \date 2018-02-12
///
/// Fonctions de la classe CLASS_VARIABLES_TREE_VIEW
///

// C++ Standard
// Librairies Qt
// Modules
#include "GUI_ClassVariableTreeView.h"

/******************************** DEFINITIONS ********************************/

/*----------------------------- Types publiques -----------------------------*/
/*--------------------------- Constantes publiques --------------------------*/
/*--------------------------- Variables publiques ---------------------------*/
/*------------------------------- Types privés ------------------------------*/
/*----------------------- Prototypes fonctions privées ----------------------*/
/*---------------------------- Constantes privées ---------------------------*/

// Nom de la fenêtre
#define WINDOW_NAME                       QStringLiteral("Data Tree View")

// Valeurs par default de la taille de la fenêtre
#define WINDOW_DEFAULT_WIDTH              400
#define WINDOW_DEFAULT_HEIGHT             600

// Valeurs minimum de la taille de la fenêtre
#define WINDOW_MIN_WIDTH                  300
#define WINDOW_MIN_HEIGHT                 500

// Label du header du TreeWidget
#define TREEWIDGET_HEADER_LABEL           QStringLiteral("Variables")

// Numéro de la colonne des item dans l'arbre
#define VARIABLE_TREE_COLUMN              0

// Nom des onglets dans le tab widget
#define VARIABLES_BY_SHEET_TAB_NAME       QStringLiteral("&Variables")

// Séparateur groupe / index de variable
#define INDEX_GROUP_NAME_SEPARATOR        QStringLiteral("_")

/*---------------------------- Variables privées ----------------------------*/

/**************************** FIN DES DEFINITIONS ****************************/

/******************************************************************************
 *                             Fonctions privÃ©es                              *
 *****************************************************************************/

/******************************************************************************
 *                         Constructeur / Destructeur                         *
 *****************************************************************************/

///
/// \fn CLASS_VARIABLES_TREE_VIEW(QObject *parent)
/// \brief Constructeur de CLASS_VARIABLES_TREE_VIEW
///
/// \param[in] parent : Objet parent
///
CLASS_VARIABLES_TREE_VIEW::CLASS_VARIABLES_TREE_VIEW(const QColor &windowColor, QWidget *parent):
   CLASS_ABSTRACT_MDI_SUBWINDOW(windowColor, parent),
   f_WindowWidgetLayout(nullptr),
   f_SearchLineEdit(nullptr),
   f_TabWidget(nullptr),
   f_VariablesTreeWidget(nullptr),
   f_SearchShortcut(nullptr)
{
   this->SetUpUserInterface();

   // On connecte le signal / slot pour le raccourcis clavier
   QObject::connect(f_SearchShortcut, &QShortcut::activated, this, &CLASS_VARIABLES_TREE_VIEW::SLOT_OnSearchShortcutActivated);

   // On connecte le singal / slot pour la barre de recherche
   QObject::connect(f_SearchLineEdit, &QLineEdit::textEdited, this, &CLASS_VARIABLES_TREE_VIEW::SLOT_OnLineEditTextEdited);
}

///
/// \fn ~CLASS_VARIABLES_TREE_VIEW()
/// \brief Destructeur de CLASS_VARIABLES_TREE_VIEW
///
CLASS_VARIABLES_TREE_VIEW::~CLASS_VARIABLES_TREE_VIEW()
{
}

/******************************************************************************
 *                                 Accesseurs                                 *
 *****************************************************************************/

/******************************************************************************
 *                                  Mutateurs                                 *
 *****************************************************************************/

/******************************************************************************
 *                             Méthodes publiques                             *
 *****************************************************************************/

///
/// \fn FillTreeVariables
/// \brief Remplie l'arbres des variables classées par feuilles / groupes / sous groupes
/// \param variablesList : liste des variables à représenter dans l'arbre
///
void CLASS_VARIABLES_TREE_VIEW::FillTreeVariables(QList<CLASS_VARS *> variablesList)
{
   // On efface l'arbre par précaution
   f_VariablesTreeWidget->clear();

   // Hash Map qui associe unn om de feuille à un QTreeWidgetItem*
   QHash<QString, SheetInfo*> SheetsHash;

   // On parcours la liste de variables
   for (CLASS_VARS *variable : variablesList)
   {
      // On récupère les informations necessaires
      QString VariableSheetName(variable->GetVarType());
      QString VariableGroupName(variable->GetVarGroup());
      QString VariableIndexName(QStringLiteral(""));
      if (variable->GetIndex() != nullptr)
         VariableIndexName = variable->GetIndex()->GetName();

      // Structure qui modélise une page
      SheetInfo *CurrentSheetInfo;

      // On vérifie que la SheetHash ne contient pas déjà une entrée concernant la feuille
      if (SheetsHash.contains(VariableSheetName) == false)
      {
         // On crée les nouvelles info concernant la page
         CurrentSheetInfo = new SheetInfo;
         CurrentSheetInfo->SheetTreeItem = this->AddTreeRoot(f_VariablesTreeWidget, VariableSheetName);
         CurrentSheetInfo->SheetGroupsHash = new QHash<QString, GroupInfo*>;
         // On insert le tout dans la hash
         SheetsHash.insert(VariableSheetName, CurrentSheetInfo);
      }
      // Si elle existe deja, on récupère les informations de la page
      else
      {
         CurrentSheetInfo = SheetsHash.value(VariableSheetName);
      }

      // Structure qui modélise un groupe de variable
      GroupInfo *CurrentGroupInfo;
      // Item correspondant au groupe de la variable
      QTreeWidgetItem* CurrentGroupItem;


      // Vérification de l'existance du groupe
      if (CurrentSheetInfo->SheetGroupsHash->contains(VariableGroupName) == false)
      {
         CurrentGroupInfo = new GroupInfo;
         CurrentGroupItem = CurrentGroupInfo->GroupTreeItem = this->AddTreeChild(CurrentSheetInfo->SheetTreeItem, VariableGroupName);
         CurrentGroupInfo->GroupIndexesHash = new QList<QString>;
         // On insert le tout dans la QHash
         CurrentSheetInfo->SheetGroupsHash->insert(VariableGroupName, CurrentGroupInfo);
      }
      // Si il existe, on récupère le tree item
      else
      {
         CurrentGroupItem = CurrentSheetInfo->SheetGroupsHash->value(VariableGroupName)->GroupTreeItem;
      }

      // On vérifie que l'index n'est pas déjà présent dans les infos du groupe
      if ((CurrentGroupInfo->GroupIndexesHash->contains(VariableIndexName) == false) &&
          (VariableIndexName != QStringLiteral("")))
      {
         this->AddTreeChild(CurrentGroupItem, VariableGroupName + INDEX_GROUP_NAME_SEPARATOR + VariableIndexName );
         // On ajoute le nom à la liste
         CurrentGroupInfo->GroupIndexesHash->append(VariableIndexName);
      }

      // On ajoute enfin la variable à l'arbre
      QString VariableName(variable->GetVarName());
      this->AddTreeChild(CurrentGroupItem, VariableName);
   }

   // On trie ensuite les items par ordre alphabétique
   f_VariablesTreeWidget->sortItems(VARIABLE_TREE_COLUMN, Qt::AscendingOrder);
}

/******************************************************************************
 *                               Slots publiques                              *
 *****************************************************************************/

///
/// \fn SLOT_OnLineEditTextEdited
/// \brief Slot appelé a chaque fois que le texte est modifié dans le line edit
/// \param text : texte écrit dans le line edit par l'utilisateur
///
void CLASS_VARIABLES_TREE_VIEW::SLOT_OnLineEditTextEdited(const QString& text)
{
   // On récupère tout les éléments de bas niveau de l'arbre
   QList<QTreeWidgetItem*> LowLevelItems(this->GetLowLevelItems(f_VariablesTreeWidget->invisibleRootItem()));

   // On démasque tout les items bas niveau de l'arbre
   for (QTreeWidgetItem * treeItem : LowLevelItems)
      treeItem->setHidden(false);

   // Si le texte entré par l'utilisateur n'est pas vide
   if (text.isEmpty() == false)
   {
      // On récupère les items qui correspondent au texte entré par l'utilisateur
      QList<QTreeWidgetItem*> SearchedItems(f_VariablesTreeWidget->findItems(text, Qt::MatchContains|Qt::MatchRecursive));

      // On soutrait pour récupérer les items à masquer
      QSet<QTreeWidgetItem*> SubstractItems(LowLevelItems.toSet().subtract(SearchedItems.toSet()));

      // On parcours ensuite les éléments à masquer
      for (QTreeWidgetItem* treeItem : SubstractItems)
         treeItem->setHidden(true);
   }
}

///
/// \fn SLOT_OnSearchShortcutActivated
/// \brief Slot appelé a chaque fois que le raccourcis (CTRL + F) est fait
///
void CLASS_VARIABLES_TREE_VIEW::SLOT_OnSearchShortcutActivated(void)
{
   f_SearchLineEdit->setFocus();
}

/******************************************************************************
 *                              Méthodes privées                              *
 *****************************************************************************/

///
/// \fn SetUpUserInterface
/// \brief Met en place l'interface graphique de la fenêtre
///
void CLASS_VARIABLES_TREE_VIEW::SetUpUserInterface(void)
{
   // On change le nom de la fenêtre
   this->setWindowTitle(WINDOW_NAME);

   // On resize avec les valeur par default
   this->SetSizeHint(QSize(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT));
   this->setMinimumSize(QSize(WINDOW_MIN_WIDTH, WINDOW_MIN_HEIGHT));
   // La fenêtre essaye de se mettre à la taille par défaut
   this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

   // On set le layout principal
   f_WindowWidgetLayout = new QVBoxLayout();
   f_WindowWidgetLayout->setContentsMargins(0, 0, 0, 0);
   f_WindowWidgetLayout->setSpacing(0);
   // Associe ce layout à la fenêtre MDI via un QWidget intermediaire
   this->setWidget(new QWidget(this));
   this->widget()->setLayout(f_WindowWidgetLayout);

   // On set le line edit pour la recherche
   f_SearchLineEdit = new QLineEdit(this);
   f_SearchLineEdit->setClearButtonEnabled(true);
   f_SearchLineEdit->installEventFilter(this);
   f_WindowWidgetLayout->addWidget(f_SearchLineEdit);

   // Création du tab widget
   f_TabWidget = new QTabWidget(this);

   // On crée le widget pour le classement des variables par feuilles / sous groupes / groupes
   f_VariablesTreeWidget = new QTreeWidget();
   f_VariablesTreeWidget->setHeaderLabel(TREEWIDGET_HEADER_LABEL);
   f_TabWidget->addTab(f_VariablesTreeWidget, VARIABLES_BY_SHEET_TAB_NAME);

   f_WindowWidgetLayout->addWidget(f_TabWidget);

   // On crée le shortcut
   f_SearchShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F), this);
}

///
/// \fn AddTreeRoot
/// \brief Ajoute un noeud principal à notre arbre
/// \param[in] name : Nom du noeud
/// \return QTreeWidgetItem* \e L'objet généré
///
QTreeWidgetItem* CLASS_VARIABLES_TREE_VIEW::AddTreeRoot(QTreeWidget* treeVariables, const QString &name)
{
   // On crée un nouvel item sur l'arbre voulu
   QTreeWidgetItem* TreeItem(new QTreeWidgetItem(treeVariables));
   // On set son texte
   TreeItem->setText(0, name);

   // On le retourne
   return TreeItem;
}

///
/// \fn AddTreeChild
/// \brief Ajoute un noeud secondaire à notre arbre
/// \param[in] parent : Noeud primaire
/// \param[in] name : Nom du noeud
/// \return QTreeWidgetItem* \e L'objet généré
///

QTreeWidgetItem* CLASS_VARIABLES_TREE_VIEW::AddTreeChild(QTreeWidgetItem* parent, const QString &name)
{
   // On crée un nouvel item
   QTreeWidgetItem* TreeItem = new QTreeWidgetItem();
   // On set son texte
   TreeItem->setText(0, name);
   // On l'ajoute a l'item souhaiter
   parent->addChild(TreeItem);

   // On le retoune
   return TreeItem;
}

///
/// \fn GetLowLevelItems
/// \brief Récupères les items de bas niveau d'un item de l'arbre
/// \return QList<QTreeWidgetItem*> \e Liste des items de bas niveau de l'arbre
///
QList<QTreeWidgetItem*> CLASS_VARIABLES_TREE_VIEW::GetLowLevelItems(QTreeWidgetItem* itemToParse)
{
   // Liste  qui contient les éléments bas niveau du TreeWidget
     QList<QTreeWidgetItem*> TreeItems;

     // Si l'item n'a pas d'enfant
     if (itemToParse->childCount() == 0)
        // On ajoute l'enfant à la liste
        TreeItems.append(itemToParse);

     // Si l'item à plusieurs enfants
     else
        // On parcours ses enfants
        for (SDWord index(0); index < itemToParse->childCount(); index ++)
           // On récupère ses enfants et on les ajoutes à la liste
           TreeItems.append(GetLowLevelItems(itemToParse->child(index)));

     // On retourne la liste des éléments bas niveau du TreeWidget
     return TreeItems;
}

/******************************************************************************
 *                             Operateur surcharge                            *
 *****************************************************************************/
