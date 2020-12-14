///
/// \file GUI_ClassAbstractSubwindow.cpp
/// \brief Classe CLASS_ABSTRACT_MDI_SUBWINDOW
/// \author ANALOG WAY - ADE
/// \date 2018-01-24
///
/// Fonctions de la classe CLASS_ABSTRACT_MDI_SUBWINDOW
///

// C++ Standard
// Librairies Qt
#include <QPalette>
// Modules
#include "GUI_ClassAbstractSubwindow.h"

/******************************** DEFINITIONS ********************************/

/*----------------------------- Types publiques -----------------------------*/
/*--------------------------- Constantes publiques --------------------------*/
/*--------------------------- Variables publiques ---------------------------*/
/*------------------------------- Types privés ------------------------------*/
/*----------------------- Prototypes fonctions privées ----------------------*/
/*---------------------------- Constantes privées ---------------------------*/

// Clés des différents éléments faisant partie de la sérialisation JSON pour une sous-fenêtre
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
/// \fn CLASS_ABSTRACT_MDI_SUBWINDOW(QObject *parent)
/// \brief Constructeur de CLASS_ABSTRACT_MDI_SUBWINDOW
///
/// \param[in] parent : Objet parent
///
CLASS_ABSTRACT_MDI_SUBWINDOW::CLASS_ABSTRACT_MDI_SUBWINDOW(const QColor &backGroundColor, QWidget *parent):
   QMdiSubWindow(parent),
   f_SubWindowBckGrdColor(backGroundColor),
   f_WindowSizeHint()
{
   this->SetBackGroundColor();
}

///
/// \fn ~CLASS_ABSTRACT_MDI_SUBWINDOW()
/// \brief Destructeur de CLASS_ABSTRACT_MDI_SUBWINDOW
///
CLASS_ABSTRACT_MDI_SUBWINDOW::~CLASS_ABSTRACT_MDI_SUBWINDOW()
{
}

/******************************************************************************
 *                                 Accesseurs                                 *
 *****************************************************************************/

///
/// \fn sizeHint
/// \brief Retourne la taille idéale souhaitée pour la fenêtre (redéfinition QWidget::sizeHint())
/// \return QSize : Taille idéale
///
QSize CLASS_ABSTRACT_MDI_SUBWINDOW::sizeHint() const
{
   // Si la taille n'a pas été précisée, on reste sur l'implémentation standard
   if (f_WindowSizeHint.isValid() == false)
      return QMdiSubWindow::sizeHint();

   // Dans le cas contraire on renvoie la notre
   return f_WindowSizeHint;
}

///
/// \fn ToJson
/// \brief Sérialise la géometrie des sous-fenêtres
/// \return QJsonObject \e Sérialisation de la géometrie de la sous-fenêtre
///
QJsonObject CLASS_ABSTRACT_MDI_SUBWINDOW::ToJson(void) const
{
   // Objet JSON qui contient les informations sur les sous-fenêtres
   QJsonObject SubWindowJsonObject;

   // On insert des données sur la sous-fenêtre
   SubWindowJsonObject.insert(GEOMETRY_JSON_KEY_X, static_cast<double>(this->x()));                // Position en abscisse de la sous-fenêtre
   SubWindowJsonObject.insert(GEOMETRY_JSON_KEY_Y, static_cast<double>(this->y()));                // Position en ordonnée de la sous-fenêtre
   SubWindowJsonObject.insert(GEOMETRY_JSON_KEY_WIDTH, static_cast<double>(this->width()));        // Largeur de la sous-fenêtre
   SubWindowJsonObject.insert(GEOMETRY_JSON_KEY_HEIGHT, static_cast<double>(this->height()));      // Hauteur de la sous-fenêtre

   // On retourne l'objet json
   return SubWindowJsonObject;
}

/******************************************************************************
 *                                  Mutateurs                                 *
 *****************************************************************************/

///
/// \fn ModifyBackGroundColor
/// \brief Modification de la couleur de fond
/// \param backColor : Couleur de fond
///
void CLASS_ABSTRACT_MDI_SUBWINDOW::ModifyBackGroundColor(const QColor& backColor)
{
   // On copie la palette courante
   QPalette Palette(this->palette());
   // On modifie juste la couleur de fond
   Palette.setColor(QPalette::Background, backColor);
   // Puis on applique
   this->setPalette(Palette);
}

///
/// \fn SetSizeHint
/// \brief Définit la taille idéale de la fenêtre
/// \param sizeHint : Taille idéale
///
void CLASS_ABSTRACT_MDI_SUBWINDOW::SetSizeHint(const QSize& sizeHint)
{
   f_WindowSizeHint = sizeHint;
}

/******************************************************************************
 *                             Méthodes publiques                             *
 *****************************************************************************/

/******************************************************************************
 *                               Slots publiques                              *
 *****************************************************************************/

/******************************************************************************
 *                              Méthodes privées                              *
 *****************************************************************************/

///
/// \fn closeEvent
/// \brief Redéfinition du close event
/// \param e : infos sur l'evènement
///
void CLASS_ABSTRACT_MDI_SUBWINDOW::closeEvent(QCloseEvent *e)
{
   // On ignore l'evènement
   e->ignore();

   // on émet un signal signalant le refus
   emit this->SIGNAL_CloseRequested(this);
}

///
/// \fn SetBackGroundColor
/// \brief Met en place la couleur de background de la subwindow
///
void CLASS_ABSTRACT_MDI_SUBWINDOW::SetBackGroundColor(void)
{
   // On change la couleur de la fenêtre
   // On récupère la palette de la fenêtre
   QPalette Palette(this->palette());

   // On met en place la nouvelle couleur voulue sur la palette
   Palette.setColor(QPalette::Background, f_SubWindowBckGrdColor);

   // On set les options pour le background
   this->setAutoFillBackground(true);

   // On applique la palette a la fenêtre
   this->setPalette(Palette);
}
/******************************************************************************
 *                             Operateur surcharge                            *
 *****************************************************************************/
