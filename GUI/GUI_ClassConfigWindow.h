///
/// \file GUI_ClassConfigWindows.h
/// \brief Classe CLASS_CONFIGWINDOWS
/// \author ANALOG WAY - GLE
/// \date 2018-05-02
///

#ifndef GUI_CLASSCONFIGWINDOW_H
#define GUI_CLASSCONFIGWINDOW_H

// C++ Standard
// Librairies Qt
#include <QObject>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QWidget>
#include <QComboBox>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QScrollArea>
#include <QProgressBar>
#include <QThread>
// Modules
#include "COMMON_TypeDefs.h"
//Applicatifs
#include "GUI_ClassDeviceWindow.h"


///
/// \class CLASS_CONFIG_WINDOW
/// \brief Classe CLASS_CONFIG_WINDOW
///
class CLASS_CONFIG_WINDOW : public QMainWindow
{
      Q_OBJECT

   public:

      /*--------------------------- Types publiques ---------------------------*/

      /*------------------------- Constantes publiques ------------------------*/

      /*------------------------- Variables publiques -------------------------*/

      /*---------------------- Constructeur / Destructeur ---------------------*/

      ///
      /// \fn CLASS_CONFIG_WINDOWS(QObject *parent)
      /// \brief Constructeur de CLASS_CONFIG_WINDOWS
      ///
      /// \param[in] parent : Widget parent
      ///
      explicit CLASS_CONFIG_WINDOW(QWidget *parent = nullptr);

      ///
      /// \fn ~CLASS_CONFIG_WINDOWS()
      /// \brief Destructeur de CLASS_CONFIG_WINDOWS
      ///
      ~CLASS_CONFIG_WINDOW();

      /*------------------------------ Accesseurs -----------------------------*/

      /*------------------------------ Mutateurs ------------------------------*/
      void SetProgressBarTimeValue(int value);

      /*-------------------------- Méthodes publiques -------------------------*/

      ///
      /// \fn CreateNewDeviceWindow
      /// \brief Instancie une nouvelle fenêtre machine
      /// \return CLASS_DEVICE_WINDOW * : Objet représentant la fenêtre
      ///
      CLASS_DEVICE_WINDOW * CreateNewDeviceWindow(void);

      ///
      /// \fn DeleteDeviceWindow
      /// \brief Instancie une nouvelle fenêtre machine
      /// \param machineWindowObj : Objet représentant la fenêtre machine
      ///
      void DeleteDeviceWindow(CLASS_DEVICE_WINDOW *machineWindowObj);

      /*--------------------------- Signaux publiques -------------------------*/


      void OnEraseInterfaceButtonClicked();
signals:

      ///
      /// \fn SIGNAL_CreateMachineRequest
      /// \brief Signal émit pour la création d'une machine
      ///
      void SIGNAL_CreateMachineRequest(void);

      ///
      /// \fn SIGNAL_DeleteMachineRequest
      /// \brief Signal émit pour la destruction d'une machine
      /// \param machineWindow : Fenêtre associée à la machine
      ///
      void SIGNAL_DeleteMachineRequest(CLASS_DEVICE_WINDOW *machineWindow);

      ///
      /// \fn SIGNAL_RebootSource
      /// \brief Signal émit pour enclenché le reboot de la fenetre de la machine source
      ///
      void SIGNAL_RebootSource(void);

      ///
      /// \fn SIGNAL_RebootReceiver
      /// \brief Signal émit pour enclenché le reboot de la fenetre de la machine réceptrice
      ///
      void SIGNAL_RebootReceiver(void);

      ///
      /// \fn SIGNAL_NewSettingSource
      /// \brief Signal pour transmettre le nom du nouveau fichier de setting pour l'appareil source
      /// \param fileName : Nom du nouveau fichier de config sélectionné par l'utilisateur
      ///
      void SIGNAL_NewSettingSource(const QString& fileName);

      /*---------------------------- Slots publiques --------------------------*/

   public slots:

      /*----------------------------- Types privées ----------------------------*/

      /*--------------------------- Méthodes privées --------------------------*/

   protected:

      ///
      /// \fn closeEvent
      /// \brief OverRide du close event de la fenêtre (destruction de tous les éléments correctement)
      /// \param closeEvent : évènement de fermeture
      ///
      void closeEvent(QCloseEvent* closeEvent);

      ///
      /// \fn eventFilter
      /// \brief Override du eventfilter pour les QObject
      /// \param obj : QObject avec lequel interagit l'utilisateur
      /// \param event : évènement dans la fenetre
      ///
      bool eventFilter(QObject* obj, QEvent* event);

   private:

      ///
      /// \fn SetUpUserIntertace
      /// \brief Met en place l'interface graphique de la fenêtre
      ///
      void SetUpUserIntertace(void);

      ///
      /// \fn CreateAllArray
      /// \brief Creer tous les tableaux nécessaire à la fenêtre
      ///
      void CreateAllArray(void);

      ///
      /// \fn OnGetConfigButtonClicked
      /// \brief Slot enclenché lors de l'appui sur le bouton "Get Config Rx and Tx "
      ///
      void OnGetConfigButtonClicked(void);

      ///
      /// \fn CreateOutputInterface
      /// \brief Creer le nombre d'output selon la demande de l'utilisateur
      /// \param currentText : Texte indiquant le nombre d'output à créer dans la combobox
      /// \param currentTextChanged : changement de texte de la combobox
      ///
      void OnComboxNumOutputCreateOutputInterface(QString currentText);

      ///
      /// \fn AddNewComboBox
      /// \brief Ajoute un nouveau comboBox dans le layout désigné
      /// \param OutputOrInputGroupName : Indication si le combobox sera dans la partie Input ou Output
      ///
      void AddNewComboBox(void);

      ///
      /// \fn ComboBoxInputListChanged
      /// \brief Adapte la liste du combobox de plug d'entrée
      /// \param currentText : texte sélectionné dans le combobox d'entrée
      ///
      void ComboBoxInputListChanged(QString currentText);

      ///
      /// \fn ComboBoxOutputListChanged
      /// \brief  Adapte la liste du combobox de plug de sortie
      /// \param currentText : Texte sélectionné dans le combobox de sortie
      ///
      void ComboBoxOutputListChanged(QString currentText);


      ///
      /// \fn Fill2dArray
      /// \brief Remplit le tableau 2D stockant les inputs outputs et plugs
      ///
      void Fill2dArray(void);

      ///
      /// \fn CreateComboBoxList
      /// \brief Création de la liste du comboBox
      /// \param IndicOutputOrInput : Indication si il s'agit d'un input ou output
      /// \param IndicPlug : Indication si il s'agit plug de sortie ou d'entrée
      /// \param currentText : Texte sélectionné dans la combobox
      /// \param ComboBox : Pointeur vers le combobox adéquate
      ///
      void CreateComboBoxList(QString IndicOutputOrInput, QString IndicPlug, QString currentText, QComboBox* ComboBox);

      ///
      /// \fn CreateGroupBoxInput
      /// \brief Creer les GroupeBox regroupant les input pour un output
      /// \param currentText : Texte indiquant le nombre d'output sélectionnée pour créer le nombre de groupBox équivalent
      ///
      void CreateGroupBoxInput(QString currentText);

      ///
      /// \fn ReorderList
      /// \brief Réorganise la liste en remettant les éléments dans l'ordre
      /// \param List : Liste originale mal ordonnée
      /// \return QStringList : la liste réordonnée
      ///
      QStringList ReorderList(QStringList List);


      ///
      /// \fn RewriteInfo
      /// \brief Créer le fichier de configuration RxTx
      /// \param settingsToRead : Fichier de config à lire
      /// \param settingsToWrite : Fichier de config dans lequel écrire
      /// \param groupToRead : Groupe du fichier de config à lire
      /// \param groupToCreate : Groupe du fichier de config à créer
      ///
      void RewriteInfo(QSettings* settingsToRead, QSettings* settingsToWrite, QString groupToRead, QString groupToCreate);


      ///
      /// \fn OnApplyButtonClicked
      /// \brief Slot enclenché lors de l'appui sur le bouton "Apply"
      /// \param clicked : état du bouton
      ///
      void OnApplyButtonClicked(void);

      ///
      /// \fn OnRebootButtonClicked
      /// \brief Slot enclenché lors de l'appui sur le bouton "Remise à zéro"
      /// \param clicked : état du bouton
      ///
      void OnRebootButtonClicked(void);


      ///
      /// \fn GetSourceWindow
      /// \brief Permet de récupérer le pointeur de la fenetre de l'appareil désignée comme source via un signal émit par la dite fenêtre
      /// \param sourceWindow : pointeur vers la fenetre définie pour la machine source
      ///
      void SetSourceWindow(CLASS_SOURCE_WINDOW *sourceWindow);

      ///
      /// \fn GetReceiverWindow
      /// \brief Permet de récupérer le pointeur de la fenetre de l'appareil désignée comme réceptrice via un signal émit par la dite fenêtre
      /// \param receiverWindow : pointeur vers la fenetre définie pour la machine réceptrice
      ///
      void SetReceiverWindow(CLASS_RECEIVER_WINDOW *receiverWindow);


      ///
      /// \fn OnStartButtonClicked
      /// \brief Slot enclenché lors de l'appui sur le bouton "Start"
      /// \param clicked : état du bouton
      ///
      void OnStartButtonClicked(void);

      ///
      /// \fn OnStopButtonClicked
      /// \brief Slot enclenché lors de l'appui sur le bouton "Stop"
      /// \param clicked : état du bouton
      ///
      void OnStopButtonClicked(void);

      ///
      /// \fn WriteOutputIndic
      /// \brief Permet d'afficher l'output testé
      /// \param text : Texte indiquant l'output
      ///
      void WriteOutputIndic(const QString &text);

      ///
      /// \fn WriteFormatIndic
      /// \brief Permet d'afficher le format appliqué sur l'output
      /// \param text : texte indiquant le format
      ///
      void WriteFormatIndic(const QString &text);

      ///
      /// \fn WriteRateIndic
      /// \brief Permet d'afficher le rate appliqué sur l'output et l'input
      /// \param text : texte indiquant le rate
      ///
      void WriteRateIndic(const QString &text);

      ///
      /// \fn WriteInputIndic
      /// \brief Permet d'afficher l'input en cours de test
      /// \param text : texte indiquant l'input
      ///
      void WriteInputIndic(const QString &text);

      ///
      /// \fn SaveToFile
      /// \brief Sauvegarde une interface de configuration dans un fichier Configuration.txt
      /// \param clicked : état du bouton
      ///
      void OnSaveButtonClicked(void);

      ///
      /// \fn LoadFromFile
      /// \brief Charge une interface de configuration et stocke dans le Tableau2DCopy via un fichier Configuration.txt
      /// \param clicked : état du bouton
      ///
      void OnLoadButtonClicked(void);

      ///
      /// \fn UpdateInterface
      /// \brief Charge une interface selon les données du tableau Tableau2DCopy
      ///
      void UpdateInterface(void);

      ///
      /// \fn OnLoadConfigFileButtonClicked
      /// \brief Slot enclenché lors de l'appui sur le bouton "Load config file"
      /// \param clicked : état du bouton
      ///
      void OnLoadConfigFileButtonClicked(void);

      ///
      /// \fn CompleteProgressBar
      /// \brief Mets la progress Bar à 100% si ce n'est pas le cas à la fin du test
      ///
      void CompleteProgressBar(void);

      ///
      /// \fn CustomFormatSelectionned
      /// \brief Indique si les format custom ont été sélectionnés
      /// \param toggled : sélectionné ou non
      ///
      void OnComboBoxModeSelectionned(int currentIndex);

      /*------------------------------ Variables ------------------------------*/

   protected:

   private:

      QMenu *f_NewMachineMenu;                             ///< Menu "New Machine" de l'interface de Configuration
      QAction *f_NewMachineAction;                         ///< Action "ToolBox" du menu "New Machine"

      QList<CLASS_DEVICE_WINDOW *> f_MachineWindowList;    ///< Liste des fenêtres liées à une machine

      QWidget *f_CentralWindow;

      QVBoxLayout *f_PrincipalLayout;                      ///< Layout vertical contenant tous les autres layout
      QHBoxLayout *f_GetConfigLayout;                      ///< Layout horizontal contenant le bouton de config et la liste déroule des output a créer
      QHBoxLayout *f_LabelLayout;                          ///< Layout horizontal contenant les labels Source et Récepteur
      QVBoxLayout *f_CentralLayout;                        ///< Layout vertical contenant contenant le GlobalScroll et les bouton Apply et Save
      QHBoxLayout *f_ConfigLayout;                         ///< Layout horizontal contenant le Tx et Rx layout
      QVBoxLayout *f_TxLayout;                             ///< Layout vertical contenant les HorizontalRxLayout correspondant aux outputs
      QVBoxLayout *f_RxLayout;                             ///< Layout vertical contenant les HorizontalTxLayout correspondant aux inputs
      QHBoxLayout *f_HorizontalRxLayout;                   ///< Layout horizontal contenant les widgets corresponds aux outputs
      QHBoxLayout *f_HorizontalTxLayout;                   ///< Layout horizontal contenant les widgets corresponds aux inputs
      QHBoxLayout *f_ApplicationLayout;                    ///< Layout horizontal contenant les bouton apply et remise à zéro
      QHBoxLayout *f_InfoLabelLayout;                      ///< Layout horizontal contenant les Labels des QLineEdit
      QHBoxLayout *f_InfoLayout;                           ///< Layout horizontal contenant les QLineEdit indiquant format rate sortie et entrée
      QHBoxLayout *f_SaveLoadLayout;                       ///< Layout horizontal contenant les bouton start pause save et load
      QHBoxLayout *f_ProgressBarLayout;                    ///< Layout horizontal contenant la QProgressBar

      QList<QHBoxLayout *> mesLayoutRx;                    ///< Liste stockant les layouts f_HorizontalRxLayout
      QList<QHBoxLayout *> mesLayoutTx;                    ///< Liste stockant les layouts f_HorizontalTxLayout
      QVector<QComboBox *> mesComboBoxInput;               ///< Tableau stockant les listes déroulantes des Inputs
      QVector<QComboBox *> mesComboBoxOutput;              ///< Tableau stockant les listes déroulantes des Outputs
      QVector<QGroupBox *> mesGroupBox;                    ///< Tableau stockant les GroupBox
      QVector<QVBoxLayout *> mesVerticalLayout;            ///< Tableau stockant les layouts verticaux

      QFrame *f_Line;                                      ///< Frame pour séparer visuellement le layout Rx et le Tx

      QPushButton *f_GetConfigButton;                      ///< Bouton GetConfig pour lancer la récupération des informations sur les machines
      QPushButton *f_LoadConfigFileButton;                 ///< Bouton "Load Config File" pour changer de fichier de config
      QPushButton *f_ApplyButton;                          ///< Bouton "Apply" pour la envoyer les informations dans les fenetres devices ouvert
      QPushButton *f_StartButton;                          ///< Bouton "Start" pout démarrer les tests
      QPushButton *f_StopButton;                           ///< Bouton "Stop" pour arrêter les tests
      QPushButton *f_SaveButton;                           ///< Bouton "Save" pour sauvegarder la configuration actuelle de la fenetre
      QPushButton *f_LoadButton;                           ///< Bouton "Load" pour charger une configuration sauvegardée
      QPushButton *f_RebootButton;                         ///< Bouton "Reboot" pour la redémarrer de zéro
      QPushButton *f_EraseInterfaceButton;

      QLabel *f_LabelOutputIndic;                          ///< Label pour la sortie actuelle
      QLabel *f_LabelFormat;                               ///< Label pour le format actuel
      QLabel *f_LabelRate;                                 ///< Label pour le rate actuel
      QLabel *f_LabelInputIndic;                           ///< Label pour l'input en test
      QLineEdit *f_OutputIndic;                            ///< Indique la sortie sur l'interface
      QLineEdit *f_FormatIndic;                            ///< Indique le format sur l'interface
      QLineEdit *f_RateIndic;                              ///< Indique le rate sur l'interface
      QLineEdit *f_InputIndic;                             ///< Indique l'entrée sur l'interface

      QProgressBar *f_ProgressBar;                         ///< ProgressBar pour visualiser le temps restant
      SDWord f_NbreTotalOutput;                            ///< Stocke le nombre d'input charger sur l'interface graphique
      SDWord f_TotalTime;                                  ///< Temps total estimé

      QComboBox *f_ComboBoxNumberOutput;                              ///< Liste déroulante du nombre d'output que l'utilisateur peut créer
      QStringList f_NumberOutputList;                      ///< Liste du nombre d'output qu'on peut sélectionner

      QComboBox *f_ComboBoxModeSelect;                  ///< RadioButton pour indiquer si l'on veut tester les format custom ou standard

      QComboBox *f_ComboboxInputPlug ;                     ///< Liste déroulante des plugs d'entrée
      QComboBox *f_ComboboxOutputPlug ;                    ///< Liste déroulante des plugs de sortie

      QComboBox *f_ComboBoxInputAdded;                       ///< Liste déroulante des entrées
      QComboBox *f_ComboBoxOutputAdded;                      ///< Liste déroulante des sorties
      QComboBox *f_SelectNumberInput;                        ///< Liste déroulante pour sélectionner le nombre d'input

      QScrollArea *f_GlobalScroll;                           ///< ScrollArea pour adapter le contenu à la fenetre via un scroll
      QWidget *f_RxContainer;                                ///< Widget pour contenir le layout Rx
      QWidget *f_TxContainer;                                ///< Widget pour contenir le layout Tx
      QWidget *f_InfoContainer;                              ///< Widget pour visualiser les informations
      QWidget *f_Container;                                  ///< Widget pour contenir les widget Rx et Tx

      QLabel *f_SourceNameLabel;                           ///< Label indiquant l'appareil source
      QLabel *f_ReceiverNameLabel;                         ///< Label indiquant l'appareil récepteur

      QSettings *f_SettingRx;                              ///< Settings donnant acces aux settings de la machine réceptrice
      QSettings *f_SettingTx;                              ///< Settings donnant acces aux settings de la machine source
      QSettings *f_SettingRxTx;                            ///< Settings permettant d'écrire un nouveau fichier config regroupant les informations nécessaires des machines réceptrice et source
      QSettings *f_SettingSource;                          ///< Pointeur pour récupérer l'information sur le setting de l'appareil source

      SDWord f_IndexComboBoxRx;                            ///< Garde en mémoire l'index du tableau des comboxBox d'entrée
      SDWord f_IndexComboBoxTx;                            ///< Garde en mémoire l'index du tableau des comboxBox de sortie
      SDWord f_NumberOutput;                               ///< Indique à quel output nous sommes
      SDWord f_NumberGroupInput;                           ///< Indique à quel groupbox nous sommes
      SDWord f_StockNumOutputAdded;                        ///< Récupére le nombre d'output créé

      QVector<QVector<QString>*> f_Tableau2D;              ///< Tableau stockant les informations des menus déroulants de l'interface
      QVector<QVector<QString>*> f_Tableau2DCopy;          ///< Tableau servant à stocker les données lors du chargement d'une configuration interface

      Bool f_Connection;                                   ///< Indique si les connexions ont déjà été réalisées ou non

      SDWord f_TimeValue;                                   ///< Indique le temps écoulé en pourcentage via la ProgressBar

      CLASS_SOURCE_WINDOW *f_SourceWindow;                 ///< Pointeur vers la fenetre qui désigne l'appareil source
      CLASS_RECEIVER_WINDOW *f_ReceiverWindow;             ///< Pointeur vers la fenetre qui désigne l'appareil réceptrice

      CLASS_TEST_SOURCE *f_TestSource;                     ///< Pointeur vers le TestSource crée via le thread de la machine source
      CLASS_TEST_RECEIVER *f_TestReceiver;                 ///< Pointeur vers le TestReceiver créé via le thread de la machine réceptrice
};

#endif // GUI_CLASSCONFIGWINDOW_H
