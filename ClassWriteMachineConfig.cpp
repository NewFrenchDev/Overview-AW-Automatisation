///
/// \file ClassWriteMachineConfig.cpp
/// \brief Classe CLASS_WRITE_MACHINE_CONFIG
/// \author ANALOG WAY - GLE
/// \date 2018-06-06
///
/// Fonctions de la classe CLASS_WRITE_MACHINE_CONFIG
///

// C++ Standard
// Librairies Qt
// Modules
#include "ClassWriteMachineConfig.h"

/******************************** DEFINITIONS ********************************/

/*----------------------------- Types publiques -----------------------------*/
/*--------------------------- Constantes publiques --------------------------*/
/*--------------------------- Variables publiques ---------------------------*/
/*------------------------------- Types privÃ©s ------------------------------*/
/*----------------------- Prototypes fonctions privÃ©es ----------------------*/
/*---------------------------- Constantes privÃ©es ---------------------------*/
/*---------------------------- Variables privÃ©es ----------------------------*/

/**************************** FIN DES DEFINITIONS ****************************/

/******************************************************************************
 *                             Fonctions privÃ©es                              *
 *****************************************************************************/

/******************************************************************************
 *                         Constructeur / Destructeur                         *
 *****************************************************************************/

///
/// \fn CLASS_WRITE_MACHINE_CONFIG(QObject *parent)
/// \brief Constructeur de CLASS_WRITE_MACHINE_CONFIG
///
/// \param[in] parent : Objet parent
///
CLASS_WRITE_MACHINE_CONFIG::CLASS_WRITE_MACHINE_CONFIG(QObject *parent):
   QObject(parent)
{
   this->WriteSettingsVio();
   this->WriteSettingsLivecore();
   this->WriteSettingsNLC();
   this->WriteSettingsMNG();
}

///
/// \fn ~CLASS_WRITE_MACHINE_CONFIG()
/// \brief Destructeur de CLASS_WRITE_MACHINE_CONFIG
///
CLASS_WRITE_MACHINE_CONFIG::~CLASS_WRITE_MACHINE_CONFIG()
{
}

/******************************************************************************
 *                                 Accesseurs                                 *
 *****************************************************************************/

/******************************************************************************
 *                                  Mutateurs                                 *
 *****************************************************************************/

/******************************************************************************
 *                             MÃ©thodes publiques                             *
 *****************************************************************************/

/******************************************************************************
 *                               Slots publiques                              *
 *****************************************************************************/

/******************************************************************************
 *                              MÃ©thodes privÃ©es                              *
 *****************************************************************************/

///
/// \fn WriteSettingsVio
/// \brief Ecrit le fichier de config du Vio dans le dossier settings
///
void CLASS_WRITE_MACHINE_CONFIG::WriteSettingsVio(void)
{
   QSettings settings(QStringLiteral("settings/settingVio.ini"), QSettings::IniFormat);

   // Si le fichier existe le supprimer pour et le réécrire en cas de modification
   if(QFile(settings.fileName()).exists())
      QFile(settings.fileName()).remove();

   //Création de la liste des appareils correspondant à ce fichier de config
   settings.beginGroup(QStringLiteral("DEVICE"));

   settings.setValue("DEV1024" , "VIO4K");

   settings.endGroup();

   //Création de la liste d'informations
   settings.beginGroup(QStringLiteral("INFO"));

   settings.setValue("MAX_INPUT" , 9);
   settings.setValue("MAX_INPUT_PLUG" , 4);
   settings.setValue("MAX_OUTPUT", 3);
   settings.setValue("MAX_OUTPUT_PLUG", 8);
   settings.setValue("MAX_OUTPUT_FORMAT" , 58);
   settings.setValue("MAX_OUTPUT_FORMAT_SDI_OPT" , 19);
   settings.setValue("MAX_RATE" , 16);
   settings.setValue("OUTPUT_NOT_AVAILABLE" , 0);
   settings.setValue("INPUT_NOT_AVAILABLE", 0);
   settings.setValue("OUTPUT_ACTIVE" , 1);
   settings.setValue("INPUT_ACTIVE", 1);

   settings.endGroup();


   //Création de la liste de COMMANDE
   settings.beginGroup(QStringLiteral("COMMAND"));

   settings.setValue("OUT_FORMAT" , "OUfor");
   settings.setValue("OUT_RATE" , "OUrat");
   settings.setValue("OUT_FORMAT_UPDATE" , "OUfru");
   settings.setValue("OUT_AVAILABLE" , "OUava");
   settings.setValue("OUT_PLUG_STATUS" , "OUpls");

   settings.setValue("SIG_DETECTED_FORMAT", "ISdfo");
   settings.setValue("SIG_SCAN_VALID" , "ISsva");
   settings.setValue("IN_AVAILABLE" , "INava");
   settings.setValue("IN_PLUG_CONNECTOR_TYPE" , "INplt");
   settings.setValue("IN_PLUG_AVAILABLE" , "INpav");
   settings.setValue("SIG_SIGNAL_WIDHT" , "ISswi");
   settings.setValue("SIG_SIGNAL_HEIGHT" , "ISshe");
   settings.setValue("SIG_FORMAT_WIDHT" , "ISfwi");
   settings.setValue("SIG_FORMAT_HEIGHT" , "ISfhe");
   settings.setValue("SIG_FREQ_FIELD" , "ISffi");

   settings.setValue("CUSTOM_COMP_GENE_MODE", "CCmod");
   settings.setValue("CUSTOM_COMP_LOAD_FROM_CUSTOM" , "CClfc");
   settings.setValue("CUSTOM_COMP_FULL_CVT_RATE" , "CCfra");
   settings.setValue("CUSTOM_COMP_CHECK_REQUEST" , "CCcrq");
   settings.setValue("CUSTOM_COMP_SAVE_FORMAT" , "CCsfo");
   settings.setValue("CUSTOM_COMP_CVT_REDUCED_BLK" , "CCrdb");

   settings.setValue("CUSTOM_COMP_FULL_CVT_HUTIL" , "CCfht");
   settings.setValue("CUSTOM_COMP_FULL_HFRONTPORCH" , "CChfp");
   settings.setValue("CUSTOM_COMP_FULL_HBACKPORCH" , "CChbp");
   settings.setValue("CUSTOM_COMP_FULL_HSYNC" , "CCfhs");
   settings.setValue("CUSTOM_COMP_FULL_HSYNC_POL" , "CChpo");

   settings.setValue("CUSTOM_COMP_FULL_CVT_VUTIL" , "CCvua");
   settings.setValue("CUSTOM_COMP_FULL_VFRONTPORCH" , "CCvfp");
   settings.setValue("CUSTOM_COMP_FULL_VBACKPORCH" , "CCvbp");
   settings.setValue("CUSTOM_COMP_FULL_VSYNC" , "CCvsy");
   settings.setValue("CUSTOM_COMP_FULL_VSYNC_POL" , "CCvpo");

   settings.endGroup();


   //Création de la liste d'Input
   settings.beginGroup(QStringLiteral("INPUT"));

   settings.setValue("DISPLAY PORT" , 0);
   settings.setValue("HDMI BACK" , 1);
   settings.setValue("HD15" , 2);
   settings.setValue("OPTICAL" , 3);
   settings.setValue("SDI" , 4);
   settings.setValue("DVI D" , 5);
   settings.setValue("HDMI FRONT" , 6);
   settings.setValue("OPTION 1" , 7);
   settings.setValue("OPTION 2" , 8);

   settings.endGroup();


   //Création  de la liste de valeur d'input
   settings.beginGroup(QStringLiteral("INPUT_VALUE"));

   settings.setValue("0" , "DISPLAY PORT" );
   settings.setValue("1" , "HDMI BACK");
   settings.setValue("2" , "HD15");
   settings.setValue("3" , "OPTICAL");
   settings.setValue("4" , "SDI");
   settings.setValue("5" , "DVI D");
   settings.setValue("6" , "HDMI FRONT");
   settings.setValue("7" , "OPTION 1");
   settings.setValue("8" , "OPTION 2");

   settings.endGroup();


   //Création de la liste des plug en entrée
   settings.beginGroup(QStringLiteral("INPUT_PLUG"));

   settings.setValue("DISPLAY PORT" , 0);
   settings.setValue("HDMI" , 1);
   settings.setValue("ANALOG HD15" , 2);
   settings.setValue("OPTICAL SFP" , 3);
   settings.setValue("SDI" , 4);
   settings.setValue("DVI D" , 5);
   settings.setValue("HDBASET" , 6);
   settings.setValue("QUAD SDI" , 7);

   settings.endGroup();

   //Création de la liste des valeur de plug en entrée
   settings.beginGroup(QStringLiteral("INPUT_PLUG_VALUE"));

   settings.setValue("0" , "DISPLAY PORT");
   settings.setValue("1" , "HDMI");
   settings.setValue("2" , "ANALOG HD15");
   settings.setValue("3" , "OPTICAL SFP");
   settings.setValue("4" , "SDI");
   settings.setValue("5" , "DVI D");
   settings.setValue("6" , "HDBASET");
   settings.setValue("7" , "QUAD SDI");

   settings.endGroup();


   //Création de la liste de format d'entrée
   settings.beginGroup(QStringLiteral("INPUT_FORMAT"));

   settings.setValue("NONE" , 0);
   settings.setValue("INVALID" , 1);
   settings.setValue("UNKNOWN" , 2);
   settings.setValue("SDTV NTSC", 3);
   settings.setValue("SDTV PAL" , 4);
   settings.setValue("SDTV SECAM" , 5);
   settings.setValue("SDTV 480i" , 6);
   settings.setValue("SDTV 576i" , 7);
   settings.setValue("EDTV 480p", 8);
   settings.setValue("EDTV 576p" , 9);
   settings.setValue("HDTV 720p" , 10);
   settings.setValue("HDTV 1035i", 11);
   settings.setValue("HDTV 1080i" , 12);
   settings.setValue("HDTV 1080p" , 13);
   settings.setValue("PROJ 1200p" , 14);
   settings.setValue("CINEMA 2K" , 15);
   settings.setValue("UHDTV 2160p", 16);
   settings.setValue("CINEMA 4K" , 17);
   settings.setValue("CEA 240p", 18);
   settings.setValue("CEA 288p" , 19);
   settings.setValue("CPU 640x350" , 20);
   settings.setValue("CPU 640x400", 21);
   settings.setValue("CPU 720x400" , 22);
   settings.setValue("CPU 640x480" , 23);
   settings.setValue("CPU 800x480" , 24);
   settings.setValue("CPU 848x480" , 25);
   settings.setValue("CPU 800x600", 26);
   settings.setValue("CPU 1280x600" , 27);
   settings.setValue("CPU 1280x720", 28);
   settings.setValue("CPU 1680x720" , 29);
   settings.setValue("CPU 1024x768" , 30);
   settings.setValue("CPU 1280x768", 31);
   settings.setValue("CPU 1360x768" , 32);
   settings.setValue("CPU 1366x768" , 33);
   settings.setValue("CPU 1280x800" , 34);
   settings.setValue("CPU 1366x800" , 35);
   settings.setValue("CPU 1088x817", 36);
   settings.setValue("CPU 1152x864" , 37);
   settings.setValue("CPU 1440x900" , 38);
   settings.setValue("CPU 1600x900" , 39);
   settings.setValue("CPU 1280x960", 40);
   settings.setValue("CPU 1280x1024", 41);
   settings.setValue("CPU 1360x1024", 42);
   settings.setValue("CPU 1400x1050", 43);
   settings.setValue("CPU 1680x1050", 44);
   settings.setValue("CPU 1440x1080", 45);
   settings.setValue("CPU 1920x1080", 46);
   settings.setValue("CPU 2048x1080", 47);
   settings.setValue("CPU 2560x1080" , 48);
   settings.setValue("CPU 3840x1080" , 49);
   settings.setValue("CPU 4096x1080", 50);
   settings.setValue("CPU 2048x1152", 51);
   settings.setValue("CPU 1600x1200", 52);
   settings.setValue("CPU 1920x1200", 53);
   settings.setValue("CPU 1792x1344", 54);
   settings.setValue("CPU 1856x1392", 55);
   settings.setValue("CPU 1920x1440", 56);
   settings.setValue("CPU 2560x1440", 57);
   settings.setValue("CPU 3440x1440" , 58);
   settings.setValue("CPU 2048x1536" , 59);
   settings.setValue("CPU 2560x1600", 60);
   settings.setValue("CPU 2560x2048", 61);
   settings.setValue("CPU 1920x2160", 62);
   settings.setValue("CPU 2048x2160", 63);
   settings.setValue("CPU 4096x2160", 64);
   settings.setValue("CPU CVT" , 65);
   settings.setValue("CPU GTF 5:4 RATIO" , 66);
   settings.setValue("CPU GTF 4:3 RATIO", 67);
   settings.setValue("CPU GTF 16:10 RATIO", 68);
   settings.setValue("CPU GTF 15:9 RATIO", 69);
   settings.setValue("CPU GTF 16:9 RATIO", 70);
   settings.setValue("CPU 1080x1920", 71);
   settings.setValue("CPU 1200x1920", 72);
   settings.setValue("CUSTOM", 73);


   settings.endGroup();


   //Création de la liste des formats d'entrée
   settings.beginGroup(QStringLiteral("INPUT_FORMAT_VALUE"));

   settings.setValue("0" , "NONE");
   settings.setValue("1" , "INVALID");
   settings.setValue("2" , "UNKNOWN");
   settings.setValue("3" , "SDTV NTSC");
   settings.setValue("4" , "SDTV PAL");
   settings.setValue("5" , "SDTV SECAM");
   settings.setValue("6" , "SDTV 480i");
   settings.setValue("7" , "SDTV 576i");
   settings.setValue("8" , "EDTV 480p");
   settings.setValue("9" , "EDTV 576p");
   settings.setValue("10" , "HDTV 720p");
   settings.setValue("11" , "HDTV 1035i");
   settings.setValue("12" , "HDTV 1080i");
   settings.setValue("13" , "HDTV 1080p");
   settings.setValue("14" , "PROJ 1200p");
   settings.setValue("15" , "CINEMA 2K");
   settings.setValue("16" , "UHDTV 2160p");
   settings.setValue("17" , "CINEMA 4K");
   settings.setValue("18" , "CEA 240p");
   settings.setValue("19" , "CEA 288p");
   settings.setValue("20" , "CPU 640x350");
   settings.setValue("21" , "CPU 640x400");
   settings.setValue("22" , "CPU 720x400");
   settings.setValue("23" , "CPU 640x480");
   settings.setValue("24" , "CPU 800x480");
   settings.setValue("25" , "CPU 848x480");
   settings.setValue("26" , "CPU 800x600");
   settings.setValue("27" , "CPU 1280x600");
   settings.setValue("28" , "CPU 1280x720");
   settings.setValue("29" , "CPU 1680x720");
   settings.setValue("30" , "CPU 1024x768");
   settings.setValue("31" , "CPU 1280x768");
   settings.setValue("32" , "CPU 1360x768");
   settings.setValue("33" , "CPU 1366x768");
   settings.setValue("34" , "CPU 1280x800");
   settings.setValue("35" , "CPU 1366x800");
   settings.setValue("36" , "CPU 1088x817");
   settings.setValue("37" , "CPU 1152x864");
   settings.setValue("38" , "CPU 1440x900");
   settings.setValue("39" , "CPU 1600x900");
   settings.setValue("40" , "CPU 1280x960");
   settings.setValue("41" , "CPU 1280x1024");
   settings.setValue("42" , "CPU 1360x1024");
   settings.setValue("43" , "CPU 1400x1050");
   settings.setValue("44" , "CPU 1680x1050");
   settings.setValue("45" , "CPU 1440x1080");
   settings.setValue("46" , "CPU 1920x1080");
   settings.setValue("47" , "CPU 2048x1080");
   settings.setValue("48" , "CPU 2560x1080");
   settings.setValue("49" , "CPU 3840x1080");
   settings.setValue("50" , "CPU 4096x1080");
   settings.setValue("51" , "CPU 2048x1152");
   settings.setValue("52" , "CPU 1600x1200");
   settings.setValue("53" , "CPU 1920x1200");
   settings.setValue("54" , "CPU 1792x1344");
   settings.setValue("55" , "CPU 1856x1392");
   settings.setValue("56" , "CPU 1920x1440");
   settings.setValue("57" , "CPU 2560x1440");
   settings.setValue("58" , "CPU 3440x1440");
   settings.setValue("59" , "CPU 2048x1536");
   settings.setValue("60" , "CPU 2560x1600");
   settings.setValue("61" , "CPU 2560x2048");
   settings.setValue("62" , "CPU 1920x2160");
   settings.setValue("63" , "CPU 2048x2160");
   settings.setValue("64" , "CPU 4096x2160");
   settings.setValue("65" , "CPU CVT");
   settings.setValue("66" , "CPU GTF 5:4 RATIO");
   settings.setValue("67" , "CPU GTF 4:3 RATIO");
   settings.setValue("68" , "CPU GTF 16:10 RATIO");
   settings.setValue("69" , "CPU GTF 15:9 RATIO");
   settings.setValue("70" , "CPU GTF 16:9 RATIO");
   settings.setValue("71" , "CPU 1080x1920");
   settings.setValue("72" , "CPU 1200x1920");
   settings.setValue("73" , "CUSTOM");

   settings.endGroup();


   //Création de la liste d'Output
   settings.beginGroup(QStringLiteral("OUTPUT"));

   settings.setValue("MASTER MAIN" , 0);
   settings.setValue("MASTER OPTION 1" , 1);
   settings.setValue("MASTER OPTION 2" , 2);

   settings.endGroup();

   //Création de la liste pour récupérer le nom en fonction de la valeur
   settings.beginGroup(QStringLiteral("OUTPUT_VALUE"));

   settings.setValue("0" ,"MASTER MAIN");
   settings.setValue("1" , "MASTER OPTION 1" );
   settings.setValue("2" , "MASTER OPTION 2");

   settings.endGroup();

   //Création de la liste de plug de sortie
   settings.beginGroup(QStringLiteral("OUTPUT_PLUG"));

   settings.setValue("DISPLAY PORT" , 0);
   settings.setValue("HDMI" , 1);
   settings.setValue("ANALOG HD15" , 2);
   settings.setValue("OPTICAL SFP" , 3);
   settings.setValue("SDI" , 4);
   settings.setValue("DVI D" , 5);
   settings.setValue("HDBASET" , 6);
   settings.setValue("BNC YPBPR" , 7);
   settings.setValue("BNC CVBS" , 8);

   settings.endGroup();

   //Création de la liste des valeurs des plugs de sortie
   settings.beginGroup(QStringLiteral("OUTPUT_PLUG_VALUE"));

   settings.setValue("0" , "DISPLAY PORT");
   settings.setValue("1" , "HDMI");
   settings.setValue("2" , "ANALOG HD15");
   settings.setValue("3" , "OPTICAL SFP");
   settings.setValue("4" , "SDI");
   settings.setValue("5" , "DVI D");
   settings.setValue("6" , "HDBASET");
   settings.setValue("7" , "BNC YPBPR");
   settings.setValue("8" , "BNC CVBS");

   settings.endGroup();


   //Création de la liste des status du plug de sortie
   settings.beginGroup(QStringLiteral("OUTPUT_PLUG_STATUS"));

   settings.setValue("NOT_AVAILABLE" , 0);
   settings.setValue("ACTIVE" , 1);
   settings.setValue("WARNING_EDID_INVALID" , 2);
   settings.setValue("DISABLE_HDCP" , 3);
   settings.setValue("DISABLE_FORMAT" , 4);
   settings.setValue("DISABLE_NO_DISPLAY" , 5);
   settings.setValue("DISABLE_DISPLAY_OFF" , 6);
   settings.setValue("DISABLE_CONNECTION_FAILED" , 7);
   settings.setValue("DISABLE_REF_INCOMPATIBLE" , 8);
   settings.setValue("FORMAT_NOT_SUPPORT_BY_DISPLAY" , 9);
   settings.setValue("DISABLE_LOOP_INVALID" , 10);

   settings.endGroup();

   //Création de la liste des valeurs des status des plugs de sortie
   settings.beginGroup(QStringLiteral("OUTPUT_PLUG_STATUS_VALUE"));

   settings.setValue("0" , "NOT_AVAILABLE");
   settings.setValue("1" , "ACTIVE");
   settings.setValue("2" , "WARNING_EDID_INVALID");
   settings.setValue("3" , "DISABLE_HDCP");
   settings.setValue("4" , "DISABLE_FORMAT");
   settings.setValue("5" , "DISABLE_NO_DISPLAY");
   settings.setValue("6" , "DISABLE_DISPLAY_OFF");
   settings.setValue("7" , "DISABLE_CONNECTION_FAILED");
   settings.setValue("8" , "DISABLE_REF_INCOMPATIBLE");
   settings.setValue("9" , "FORMAT_NOT_SUPPORT_BY_DISPLAY");
   settings.setValue("10" , "DISABLE_LOOP_INVALID");

   settings.endGroup();


   //Création de la liste des format de sortie
   settings.beginGroup(QStringLiteral("OUTPUT_FORMAT"));

   settings.setValue("SDTV PAL 4:3" , 0);
   settings.setValue("SDTV PAL 16:9" , 1);
   settings.setValue("SDTV NTSC 4:3" , 2);
   settings.setValue("SDTV NTSC 16:9" , 3);
   settings.setValue("CEA861 480I 4:3" , 4);
   settings.setValue("CEA861 480I 16:9" , 5);
   settings.setValue("EDTV 480P 4:3" , 6);
   settings.setValue("EDTV 480P 16:9" , 7);
   settings.setValue("EDTV 576P 4:3" , 8);
   settings.setValue("EDTV 576P 16:9" , 9);
   settings.setValue("HDTV 720p" , 10);
   settings.setValue("HDTV 1035i" , 11);
   settings.setValue("HDTV 1080i" , 12);
   settings.setValue("HDTV 1080sF" , 13);
   settings.setValue("HDTV 1080p" , 14);
   settings.setValue("PROJ 1200p" , 15);
   settings.setValue("CINEMA 2K" , 16);
   settings.setValue("UHDTV 2160p" , 17);
   settings.setValue("CINEMA 4K" , 18);
   settings.setValue("COMPUTER VGA" , 19);
   settings.setValue("COMPUTER 800x480" , 20);
   settings.setValue("COMPUTER SVGA" , 21);
   settings.setValue("COMPUTER WVGA" , 22);
   settings.setValue("COMPUTER XGA" , 23);
   settings.setValue("COMPUTER 1088x817" , 24);
   settings.setValue("COMPUTER 1152x864" , 25);
   settings.setValue("COMPUTER 720p" , 26);
   settings.setValue("COMPUTER WXGA" , 27);
   settings.setValue("COMPUTER WXGA2" , 28);
   settings.setValue("COMPUTER 1280x960" , 29);
   settings.setValue("COMPUTER SXGA" , 30);
   settings.setValue("COMPUTER 1360x768" , 31);
   settings.setValue("COMPUTER 1360x1024" , 32);
   settings.setValue("COMPUTER SWXGAPB" , 33);
   settings.setValue("COMPUTER SWXGAP", 34);
   settings.setValue("COMPUTER SXGAP" , 35);
   settings.setValue("COMPUTER 900p" , 36);
   settings.setValue("COMPUTER 1440x1080" , 37);
   settings.setValue("COMPUTER 1600x900" , 38);
   settings.setValue("COMPUTER UXGA" ,  39);
   settings.setValue("COMPUTER WSXGAP" , 40);
   settings.setValue("COMPUTER 1792x1344" , 41);
   settings.setValue("COMPUTER 1856x1392" , 42);
   settings.setValue("COMPUTER 1080p" , 43);
   settings.setValue("COMPUTER WUXGA" , 44);
   settings.setValue("COMPUTER 1920x1440" , 45);
   settings.setValue("COMPUTER 1920x2160" , 46);
   settings.setValue("COMPUTER 2K" , 47);
   settings.setValue("COMPUTER 2048x1152" , 48);
   settings.setValue("COMPUTER QXGA" , 49);
   settings.setValue("COMPUTER 2048x2160" , 50);
   settings.setValue("COMPUTER 2560x1080" , 51);
   settings.setValue("COMPUTER 2560x1440" , 52);
   settings.setValue("COMPUTER WQXGA" , 53);
   settings.setValue("COMPUTER QSXGA" , 54);
   settings.setValue("COMPUTER 3440x1440" , 55);
   settings.setValue("COMPUTER 3840x1080" , 56);
   settings.setValue("COMPUTER 4096x1080" , 57);
   settings.setValue("COMPUTER CUSTOM 1" , 58);
   settings.setValue("COMPUTER CUSTOM 2" , 59);

   settings.endGroup();


   //Création de la liste des valeurs de format
   settings.beginGroup(QStringLiteral("OUTPUT_FORMAT_VALUE"));

   settings.setValue("0" , "SDTV PAL 4:3");
   settings.setValue("1" , "SDTV PAL 16:9");
   settings.setValue("2" , "SDTV NTSC 4:3");
   settings.setValue("3" , "SDTV NTSC 16:9");
   settings.setValue("4" , "CEA861 480I 4:3");
   settings.setValue("5" , "CEA861 480I 16:9");
   settings.setValue("6" , "EDTV 480P 4:3");
   settings.setValue("7" , "EDTV 480P 16:9");
   settings.setValue("8" , "EDTV 576P 4:3");
   settings.setValue("9" , "EDTV 576P 16:9");
   settings.setValue("10" , "HDTV 720p");
   settings.setValue("11" , "HDTV 1035i");
   settings.setValue("12" , "HDTV 1080i");
   settings.setValue("14" , "HDTV 1080p");
   settings.setValue("15" , "PROJ 1200p");
   settings.setValue("16" , "CINEMA 2K");
   settings.setValue("17" , "UHDTV 2160p");
   settings.setValue("18" , "CINEMA 4K");
   settings.setValue("19" , "COMPUTER VGA");
   settings.setValue("20" , "COMPUTER 800x480");
   settings.setValue("21" , "COMPUTER SVGA");
   settings.setValue("22" , "COMPUTER WVGA");
   settings.setValue("23" , "COMPUTER XGA");
   settings.setValue("24" , "COMPUTER 1088x817");
   settings.setValue("25" , "COMPUTER 1152x864");
   settings.setValue("26" , "COMPUTER 720p");
   settings.setValue("27" , "COMPUTER WXGA");
   settings.setValue("28" , "COMPUTER WXGA2");
   settings.setValue("29" , "COMPUTER 1280x960");
   settings.setValue("30" , "COMPUTER SXGA");
   settings.setValue("31" , "COMPUTER 1360x768");
   settings.setValue("32" , "COMPUTER 1360x1024");
   settings.setValue("33" , "COMPUTER SWXGAPB");
   settings.setValue("34" , "COMPUTER SWXGAP");
   settings.setValue("35" , "COMPUTER SXGAP");
   settings.setValue("36" , "COMPUTER 900p");
   settings.setValue("37" , "COMPUTER 1440x1080");
   settings.setValue("38" , "COMPUTER 1600x900");
   settings.setValue("39" , "COMPUTER UXGA");
   settings.setValue("40" , "COMPUTER WSXGAP");
   settings.setValue("41" , "COMPUTER 1792x1344");
   settings.setValue("42" , "COMPUTER 1856x1392");
   settings.setValue("43" , "COMPUTER 1080p");
   settings.setValue("44" , "COMPUTER WUXGA");
   settings.setValue("45" , "COMPUTER 1920x1440");
   settings.setValue("46" , "COMPUTER 1920x2160");
   settings.setValue("47" , "COMPUTER 2K");
   settings.setValue("48" , "COMPUTER 2048x1152");
   settings.setValue("49" , "COMPUTER QXGA");
   settings.setValue("50" , "COMPUTER 2048x2160");
   settings.setValue("51" , "COMPUTER 2560x1080");
   settings.setValue("52" , "COMPUTER 2560x1440");
   settings.setValue("53" , "COMPUTER WQXGA");
   settings.setValue("54" , "COMPUTER QSXGA");
   settings.setValue("55" , "COMPUTER 3440x1440");
   settings.setValue("56" , "COMPUTER 3840x1080");
   settings.setValue("57" , "COMPUTER 4096x1080");
   settings.setValue("58" , "COMPUTER CUSTOM 1");
   settings.setValue("59" , "COMPUTER CUSTOM 2");

   settings.endGroup();

   //Création de la liste des rates
   settings.beginGroup(QStringLiteral("RATE"));

   settings.setValue("23.97HZ", 0);
   settings.setValue("24HZ", 1);
   settings.setValue("25HZ", 2);
   settings.setValue("29.97HZ", 3);
   settings.setValue("30HZ", 4);
   settings.setValue("47.95HZ", 5);
   settings.setValue("48HZ", 6);
   settings.setValue("50HZ", 7);
   settings.setValue("59.94HZ", 8);
   settings.setValue("60HZ", 9);
   settings.setValue("72HZ", 10);
   settings.setValue("75HZ", 11);
   settings.setValue("85HZ", 12);
   settings.setValue("100HZ", 13);
   settings.setValue("119.88HZ", 14);
   settings.setValue("120HZ", 15);

   settings.endGroup();


   //Création de la liste des valeurs de rate
   settings.beginGroup(QStringLiteral("RATE_VALUE"));

   settings.setValue("0" , "23.97HZ");
   settings.setValue("1" , "24HZ");
   settings.setValue("2" , "25HZ");
   settings.setValue("3" , "29.97HZ");
   settings.setValue("4" , "30HZ");
   settings.setValue("5" , "47.95HZ");
   settings.setValue("6" , "48HZ");
   settings.setValue("7" , "50HZ");
   settings.setValue("8" , "59.94HZ");
   settings.setValue("9" , "60HZ");
   settings.setValue("10" , "72HZ");
   settings.setValue("11" , "75HZ");
   settings.setValue("12" , "85HZ");
   settings.setValue("13" , "100HZ");
   settings.setValue("14" , "119.88HZ");
   settings.setValue("15" , "120HZ");

   settings.endGroup();
}


///
/// \fn WriteSettingsLivecore
/// \brief Ecrit le fichier de config du Livecore dans le dossier settings
///
void CLASS_WRITE_MACHINE_CONFIG::WriteSettingsLivecore(void)
{
   QSettings settings(QStringLiteral("settings/settingLiveCore.ini"), QSettings::IniFormat);

   // Si le fichier existe le supprimer pour et le réécrire en cas de modification
   if(QFile(settings.fileName()).exists())
      QFile(settings.fileName()).remove();

   //Création de la liste des appareils correspondant à ce fichier de config
   settings.beginGroup(QStringLiteral("DEVICE"));

   settings.setValue("DEV97" , "Livecore NXT1604");
   settings.setValue("DEV98" , "Livecore SMX12x4");
   settings.setValue("DEV99" , "Livecore ASC3204");
   settings.setValue("DEV100" , "Livecore ASC4806");
   settings.setValue("DEV101" , "Livecore ORX5");
   settings.setValue("DEV102" , "Livecore LOE016");
   settings.setValue("DEV103" , "Livecore LOE032");
   settings.setValue("DEV104" , "Livecore LOE048");
   settings.setValue("DEV105" , "Livecore NXT1604-4K");
   settings.setValue("DEV106" , "Livecore SMX12x4-4K");
   settings.setValue("DEV107" , "Livecore ASC3204-4K");
   settings.setValue("DEV108" , "Livecore ASC4806-4K" );
   settings.setValue("DEV109" , "Livecore LOE016-4K");
   settings.setValue("DEV110" , "Livecore LOE032-4K");
   settings.setValue("DEV111" , "Livecore LOE048-4K");
   settings.setValue("DEV112" , "Livecore ASC1602");
   settings.setValue("DEV113" , "Livecore ASC1602-4K");
   settings.setValue("DEV114" , "Livecore ASC4806-PL");
   settings.setValue("DEV115" , "Livecore LOE48-PL");
   settings.setValue("DEV116" , "Livecore NXT0802");
   settings.setValue("DEV117" , "Livecore NXT0802-4K");
   settings.setValue("DEV118" , "Livecore ASC3204-PL" );
   settings.setValue("DEV119" , "Livecore LOE032-PL");

   settings.endGroup();


   //Création de la liste d'informations
   settings.beginGroup(QStringLiteral("INFO"));

   settings.setValue("MAX_OUTPUT", 8);
   settings.setValue("MAX_OUTPUT_PLUG" , 4);
   settings.setValue("MAX_OUTPUT_FORMAT" , 55);
   settings.setValue("MAX_OUTPUT_FORMAT_SDI_OPT" , 10);
   settings.setValue("MAX_INPUT", 24);
   settings.setValue("MAX_INPUT_PLUG" , 6);
   settings.setValue("MAX_INPUT_FORMAT" , 63);
   settings.setValue("MAX_RATE" , 12);
   settings.setValue("OUTPUT_NOT_AVAILABLE" , 0);
   settings.setValue("OUTPUT_ACTIVE" , 1);
   settings.setValue("INPUT_NOT_AVAILABLE" , 0);
   settings.setValue("INPUT_ACTIVE" , 1);

   settings.endGroup();


   //Création de la liste de COMMANDE
   settings.beginGroup(QStringLiteral("COMMAND"));

   settings.setValue("OUT_FORMAT" , "OUfor");
   settings.setValue("OUT_RATE", "OUrat");
   settings.setValue("OUT_AVAILABLE" , "OUava");
   settings.setValue("OUT_PLUG_STATUS" , "OUpls");
   settings.setValue("OUT_FORMAT_RATE_VALIDITY" , "OUfrv");

   settings.setValue("SIG_DETECTED_FORMAT", "ISdfo");
   settings.setValue("SIG_SCAN_VALID" , "ISsva");
   settings.setValue("IN_AVAILABLE" , "INava");
   settings.setValue("IN_PLUG_AVAILABLE" , "INpav");
   settings.setValue("SIG_SIGNAL_WIDHT" , "ISswi");
   settings.setValue("SIG_SIGNAL_HEIGHT" , "ISshe");
   settings.setValue("SIG_FORMAT_WIDHT" , "ISfwi");
   settings.setValue("SIG_FORMAT_HEIGHT" , "ISfhe");
   settings.setValue("SIG_FREQ_FIELD" , "ISffi");

   settings.setValue("OCCOMPUTE_TYPE", "OCcty");
   settings.setValue("OCCOMPUTE_LOAD" , "OCloa");
   settings.setValue("OCCOMPUTE_FULL_RATE" , "OCfra");
   settings.setValue("OCCOMPUTE_CHECK_REQUEST" , "OCccr");
   settings.setValue("OCCOMPUTE_SAVE" , "OCcsa");
   settings.setValue("OCCOMPUTE_CVT_REDUCED_BLK" , "OCcrb");

   settings.setValue("OCCOMPUTE_FULL_HUTIL" , "OCfhu");
   settings.setValue("OCCOMPUTE_FULL_HFRONTPORCH" , "Ocfhf");
   settings.setValue("OCCOMPUTE_FULL_HBACKPORCH" , "OCfhb");
   settings.setValue("OCCOMPUTE_FULL_HSYNC" , "OCfhs");
   settings.setValue("OCCOMPUTE_FULL_HSYNC_POL" , "OCfpo");

   settings.setValue("OCCOMPUTE_FULL_VUTIL" , "OCfvu");
   settings.setValue("OCCOMPUTE_FULL_VFRONTPORCH" , "OCfvf");
   settings.setValue("OCCOMPUTE_FULL_VBACKPORCH" , "OCfvb");
   settings.setValue("OCCOMPUTE_FULL_VSYNC" , "OCfvs");
   settings.setValue("OCCOMPUTE_FULL_VSYNC_POL" , "OCfvp");

   settings.endGroup();


   //Création  de la liste d'input
   settings.beginGroup(QStringLiteral("INPUT"));

   settings.setValue("IN 1" , 0);
   settings.setValue("IN 2" , 1);
   settings.setValue("IN 3" , 2);
   settings.setValue("IN 4" , 3);
   settings.setValue("IN 5" , 4);
   settings.setValue("IN 6" , 5);
   settings.setValue("IN 7" , 6);
   settings.setValue("IN 8" , 7);
   settings.setValue("IN 9" , 8);
   settings.setValue("IN 10" , 9);
   settings.setValue("IN 11" , 10);
   settings.setValue("IN 12" , 11);
   settings.setValue("IN 13" , 12);
   settings.setValue("IN 14" , 13);
   settings.setValue("IN 15" , 14);
   settings.setValue("IN 16" , 15);
   settings.setValue("IN 17" , 16);
   settings.setValue("IN 18" , 17);
   settings.setValue("IN 19" , 18);
   settings.setValue("IN 20" , 19);
   settings.setValue("IN 21" , 20);
   settings.setValue("IN 22" , 21);
   settings.setValue("IN 23" , 22);
   settings.setValue("IN 24" , 23);

   settings.endGroup();

   //Création  de la liste de valeur d'input
   settings.beginGroup(QStringLiteral("INPUT_VALUE"));

   settings.setValue("0" , "IN 1");
   settings.setValue("1" , "IN 2");
   settings.setValue("2" , "IN 3");
   settings.setValue("3" , "IN 4");
   settings.setValue("4" , "IN 5");
   settings.setValue("5" , "IN 6");
   settings.setValue("6" , "IN 7");
   settings.setValue("7" , "IN 8");
   settings.setValue("8" , "IN 9");
   settings.setValue("9" , "IN 10");
   settings.setValue("10" , "IN 11");
   settings.setValue("11" , "IN 12");
   settings.setValue("12" , "IN 13");
   settings.setValue("13" , "IN 14");
   settings.setValue("14" , "IN 15");
   settings.setValue("15" , "IN 16");
   settings.setValue("16" , "IN 17");
   settings.setValue("17" , "IN 18");
   settings.setValue("18" , "IN 19");
   settings.setValue("19" , "IN 20");
   settings.setValue("20" , "IN 21");
   settings.setValue("21" , "IN 22");
   settings.setValue("22" , "IN 23");
   settings.setValue("23" , "IN 24");

   settings.endGroup();


   //Création de la liste des plug en entrée
   settings.beginGroup(QStringLiteral("INPUT_PLUG"));

   settings.setValue("ANALOG HD15" , 0);
   settings.setValue("ANALOG DVI" , 1);
   settings.setValue("DVI" , 2);
   settings.setValue("SDI" , 3);
   settings.setValue("HDMI" , 4);
   settings.setValue("DISPLAY PORT" , 5);

   settings.endGroup();

   //Création de la liste des valeur de plug en entrée
   settings.beginGroup(QStringLiteral("INPUT_PLUG_VALUE"));

   settings.setValue("0" , "ANALOG HD15");
   settings.setValue("1" , "ANALOG DVI");
   settings.setValue("2" , "DVI");
   settings.setValue("3" , "SDI");
   settings.setValue("4" , "HDMI");
   settings.setValue("5" , "DISPLAY PORT");

   settings.endGroup();



   //Création de la liste des formats d'entrée
   settings.beginGroup(QStringLiteral("INPUT_FORMAT"));

   settings.setValue("NONE" , 0);
   settings.setValue("INVALID" , 1);
   settings.setValue("UNKNOWN" , 2);
   settings.setValue("SDTV NTSC" , 3);
   settings.setValue("SDTV PAL" , 4);
   settings.setValue("SDTV SECAM" , 5);
   settings.setValue("SDTV 480i" , 6);
   settings.setValue("SDTV 576i" , 7);
   settings.setValue("EDTV 480p" , 8);
   settings.setValue("EDTV 576p" , 9);
   settings.setValue("HDTV 720p" , 10);
   settings.setValue("HDTV 1035i" , 11);
   settings.setValue("HDTV 1080i" , 12);
   settings.setValue("HDTV 1080p" , 13);
   settings.setValue("HDTV 2K" , 14);
   settings.setValue("CEA 240p" , 15);
   settings.setValue("CEA 288p" , 16);
   settings.setValue("CPU 640x350" , 17);
   settings.setValue("CPU 640x400" , 18);
   settings.setValue("CPU 720x400" , 19);
   settings.setValue("CPU 640x480" , 20);
   settings.setValue("CPU 800x480" , 21);
   settings.setValue("CPU 848x480" , 22);
   settings.setValue("CPU 800x600" , 23);
   settings.setValue("CPU 1280x600" , 24);
   settings.setValue("CPU 1280x720" , 25);
   settings.setValue("CPU 1024x768" , 26);
   settings.setValue("CPU 1280x768" , 27);
   settings.setValue("CPU 1360x768" , 28);
   settings.setValue("CPU 1366x768" , 29);
   settings.setValue("CPU 1280x800" , 30);
   settings.setValue("CPU 1366x800" , 31);
   settings.setValue("CPU 1152x864" , 32);
   settings.setValue("CPU 1440x900" , 33);
   settings.setValue("CPU 1600x900" , 34);
   settings.setValue("CPU 1280x960" , 35);
   settings.setValue("CPU 1280x1024" , 36);
   settings.setValue("CPU 1360x1024" , 37);
   settings.setValue("CPU 1400x1050" , 38);
   settings.setValue("CPU 1680x1050" , 39);
   settings.setValue("CPU 1920x1080" , 40);
   settings.setValue("CPU 2048x1080" , 41);
   settings.setValue("CPU 2048x1152" , 42);
   settings.setValue("CPU 1600x1200" , 43);
   settings.setValue("CPU 1920x1200" , 44);
   settings.setValue("CPU 1792x1344" , 45);
   settings.setValue("CPU 1856x1392" , 46);
   settings.setValue("CPU 1920x1440" , 47);
   settings.setValue("CPU 2560x1440" , 48);
   settings.setValue("CPU 1920x2160" , 49);
   settings.setValue("CPU 2048x2160" , 50);
   settings.setValue("CPU 2048x1536" , 51);
   settings.setValue("CPU 2560x1600" , 52);
   settings.setValue("CPU 3840x1080" , 53);
   settings.setValue("CPU 4096x1080" , 54);
   settings.setValue("CPU CVT" , 55);
   settings.setValue("CPU GTF 5:4 RATIO", 56);
   settings.setValue("CPU GTF 4:3 RATIO" , 57);
   settings.setValue("CPU GTF 16:10 RATIO" , 58);
   settings.setValue("CPU GTF 15:9 RATIO" , 59);
   settings.setValue("CPU GTF 16:9 RATIO" , 60);
   settings.setValue("UHDTV 2160p" , 61);
   settings.setValue("CINEMA 4K" , 62);

   settings.endGroup();

   //Création de la liste des formats d'entrée
   settings.beginGroup(QStringLiteral("INPUT_FORMAT_VALUE"));

   settings.setValue("0" , "NONE");
   settings.setValue("1" , "INVALID");
   settings.setValue("2" , "UNKNOWN");
   settings.setValue("3" , "SDTV NTSC");
   settings.setValue("4" , "SDTV PAL");
   settings.setValue("5" , "SDTV SECAM");
   settings.setValue("6" , "SDTV 480i");
   settings.setValue("7" , "SDTV 576i");
   settings.setValue("8" , "EDTV 480p");
   settings.setValue("9" , "EDTV 576p");
   settings.setValue("10" , "HDTV 720p");
   settings.setValue("11" , "HDTV 1035i");
   settings.setValue("12" , "HDTV 1080i");
   settings.setValue("13" , "HDTV 1080p");
   settings.setValue("14" , "HDTV 2K");
   settings.setValue("15" , "CEA 240p");
   settings.setValue("16" , "CEA 288p");
   settings.setValue("17" , "CPU 640x350");
   settings.setValue("18" , "CPU 640x400");
   settings.setValue("19" , "CPU 720x400");
   settings.setValue("20" , "CPU 640x480");
   settings.setValue("21" , "CPU 800x480");
   settings.setValue("22" , "CPU 848x480");
   settings.setValue("23" , "CPU 800x600");
   settings.setValue("24" , "CPU 1280x600");
   settings.setValue("25" , "CPU 1280x720");
   settings.setValue("26" , "CPU 1024x768");
   settings.setValue("27" , "CPU 1280x768");
   settings.setValue("28" , "CPU 1360x768");
   settings.setValue("29" , "CPU 1366x768");
   settings.setValue("30" , "CPU 1280x800");
   settings.setValue("31" , "CPU 1366x800");
   settings.setValue("32" , "CPU 1152x864");
   settings.setValue("33" , "CPU 1440x900");
   settings.setValue("34" , "CPU 1600x900");
   settings.setValue("35" , "CPU 1280x960");
   settings.setValue("36" , "CPU 1280x1024");
   settings.setValue("37" , "CPU 1360x1024");
   settings.setValue("38" , "CPU 1400x1050");
   settings.setValue("39" , "CPU 1680x1050");
   settings.setValue("40" , "CPU 1920x1080");
   settings.setValue("41" , "CPU 2048x1080");
   settings.setValue("42" , "CPU 2048x1152");
   settings.setValue("43" , "CPU 1600x1200");
   settings.setValue("44" , "CPU 1920x1200");
   settings.setValue("45" , "CPU 1792x1344");
   settings.setValue("46" , "CPU 1856x1392");
   settings.setValue("47" , "CPU 1920x1440");
   settings.setValue("48" , "CPU 2560x1440");
   settings.setValue("49" , "CPU 1920x2160");
   settings.setValue("50" , "CPU 2048x2160");
   settings.setValue("51" , "CPU 2048x1536");
   settings.setValue("52" , "CPU 2560x1600");
   settings.setValue("53" , "CPU 3840x1080");
   settings.setValue("54" , "CPU 4096x1080");
   settings.setValue("55" , "CPU CVT");
   settings.setValue("56" , "CPU GTF 5:4 RATIO");
   settings.setValue("57" , "CPU GTF 4:3 RATIO");
   settings.setValue("58" , "CPU GTF 16:10 RATIO");
   settings.setValue("59" , "CPU GTF 15:9 RATIO");
   settings.setValue("60" , "CPU GTF 16:9 RATIO");
   settings.setValue("61" , "UHDTV 2160p");
   settings.setValue("62" , "CINEMA 4K");

   settings.endGroup();

   //Création de la liste des sorties
   settings.beginGroup(QStringLiteral("OUTPUT"));

   settings.setValue("OUTPUT 1" , 0);
   settings.setValue("OUTPUT 2" , 1);
   settings.setValue("OUTPUT 3" , 2);
   settings.setValue("OUTPUT 4" , 3);
   settings.setValue("OUTPUT 5" , 4);
   settings.setValue("OUTPUT 6" , 5);
   settings.setValue("OUTPUT 7" , 6);
   settings.setValue("OUTPUT 8" , 7);

   settings.endGroup();

   //Création de la liste de valeur des sorties
   settings.beginGroup("OUTPUT_VALUE");

   settings.setValue("0" , "OUTPUT 1");
   settings.setValue("1" , "OUTPUT 2");
   settings.setValue("2" , "OUTPUT 3");
   settings.setValue("3" , "OUTPUT 4");
   settings.setValue("4" , "OUTPUT 5");
   settings.setValue("5" , "OUTPUT 6");
   settings.setValue("6" , "OUTPUT 7");
   settings.setValue("7" , "OUTPUT 8");

   settings.endGroup();

   //Création de la liste des plugs de sortie
   settings.beginGroup(QStringLiteral("OUTPUT_PLUG"));

   settings.setValue("ANALOG" , 0);
   settings.setValue("DVI" , 1);
   settings.setValue("SDI" , 2);
   settings.setValue("OPTICAL" , 3);

   settings.endGroup();

   //Création de la liste des plugs de sortie
   settings.beginGroup(QStringLiteral("OUTPUT_PLUG_VALUE"));

   settings.setValue("0" , "ANALOG");
   settings.setValue("1" , "DVI");
   settings.setValue("2" , "SDI");
   settings.setValue("3" , "OPTICAL");

   settings.endGroup();


   //Création de la liste des status des plugs de sortie
   settings.beginGroup(QStringLiteral("OUT_PLUG_STATUS"));

   settings.setValue("NOT_AVAILABLE", 0);
   settings.setValue("DISABLE" , 1);
   settings.setValue("ACTIVE" , 2);

   settings.endGroup();

   //Création de la liste des format de sortie
   settings.beginGroup(QStringLiteral("OUTPUT_FORMAT"));

   settings.setValue("SDTV PAL", 0);
   settings.setValue("SDTV NTSC", 1);
   settings.setValue("EDTV 480p", 2);
   settings.setValue("EDTV 576p", 3);
   settings.setValue("HDTV 720p", 4);
   settings.setValue("HDTV 1035i", 5);
   settings.setValue("HDTV 1080i", 6);
   settings.setValue("HDTV 1080p", 7);
   settings.setValue("HDTV 1080sF" , 8);
   settings.setValue("DCDM 2k", 9);
   settings.setValue("COMPUTER VGA", 10);
   settings.setValue("COMPUTER SVGA", 11);
   settings.setValue("COMPUTER WVGA", 12);
   settings.setValue("COMPUTER XGA", 13);
   settings.setValue("COMPUTER 1152x864", 14);
   settings.setValue("COMPUTER 720p", 15);
   settings.setValue("COMPUTER WXGA", 16);
   settings.setValue("COMPUTER WXGA2", 17);
   settings.setValue("COMPUTER 1280x960", 18);
   settings.setValue("COMPUTER SXGA", 19);
   settings.setValue("COMPUTER 1360x768", 20);
   settings.setValue("COMPUTER 1360x1024", 21);
   settings.setValue("COMPUTER SWXGAPB", 22);
   settings.setValue("COMPUTER SWXGAP", 23);
   settings.setValue("COMPUTER SXGAP", 24);
   settings.setValue("COMPUTER 900p", 25);
   settings.setValue("COMPUTER 1440x960", 26);
   settings.setValue("COMPUTER 1600x900", 27);
   settings.setValue("COMPUTER UXGA", 28);
   settings.setValue("COMPUTER WSXGAP", 29);
   settings.setValue("COMPUTER 1080p", 30);
   settings.setValue("COMPUTER WUXGA", 31);
   settings.setValue("COMPUTER 1920x1440", 32);
   settings.setValue("COMPUTER 2K", 33);
   settings.setValue("COMPUTER 2048 1152", 34);
   settings.setValue("COMPUTER QXGA", 35);
   settings.setValue("COMPUTER 2560 1440", 36);
   settings.setValue("COMPUTER WQXGA", 37);
   settings.setValue("COMPUTER CUSTOM 1", 38);
   settings.setValue("COMPUTER CUSTOM 2", 39);
   settings.setValue("COMPUTER CUSTOM 3", 40);
   settings.setValue("COMPUTER CUSTOM 4", 41);
   settings.setValue("COMPUTER CUSTOM 5", 42);
   settings.setValue("COMPUTER CUSTOM 6", 43);
   settings.setValue("COMPUTER CUSTOM 7", 44);
   settings.setValue("COMPUTER CUSTOM 8", 45);
   settings.setValue("COMPUTER CUSTOM 9", 46);
   settings.setValue("COMPUTER CUSTOM 10", 47);
   settings.setValue("UHDTV 2160p", 48);
   settings.setValue("CINEMA 4K", 49);
   settings.setValue("COMPUTER 2560x1080", 50);
   settings.setValue("COMPUTER 1920x2160", 51);
   settings.setValue("COMPUTER 2048x2160", 52);
   settings.setValue("COMPUTER 3840x1080", 53);
   settings.setValue("COMPUTER 4096x1080", 54);

   settings.endGroup();


   //Création de la liste des format de sortie
   settings.beginGroup(QStringLiteral("OUTPUT_FORMAT_VALUE"));

   settings.setValue("0" , "SDTV PAL");
   settings.setValue("1" , "SDTV NTSC");
   settings.setValue("2" , "EDTV 480p");
   settings.setValue("3" , "EDTV 576p");
   settings.setValue("4" , "HDTV 720p");
   settings.setValue("5" , "HDTV 1035i");
   settings.setValue("6" , "HDTV 1080i");
   settings.setValue("7" , "HDTV 1080p");
   settings.setValue("9" , "DCDM 2k");
   settings.setValue("10" , "COMPUTER VGA");
   settings.setValue("11" , "COMPUTER SVGA");
   settings.setValue("12" , "COMPUTER WVGA");
   settings.setValue("13" , "COMPUTER XGA");
   settings.setValue("14" , "COMPUTER 1152x864");
   settings.setValue("15" , "COMPUTER 720p");
   settings.setValue("16" , "COMPUTER WXGA");
   settings.setValue("17" , "COMPUTER WXGA2");
   settings.setValue("18" , "COMPUTER 1280x960");
   settings.setValue("19" , "COMPUTER SXGA");
   settings.setValue("20" , "COMPUTER 1360x768");
   settings.setValue("21" , "COMPUTER 1360x1024");
   settings.setValue("22" , "COMPUTER SWXGAPB");
   settings.setValue("23" , "COMPUTER SWXGAP");
   settings.setValue("24" , "COMPUTER SXGAP");
   settings.setValue("25" , "COMPUTER 900p");
   settings.setValue("26" , "COMPUTER 1440x960");
   settings.setValue("27" , "COMPUTER 1600x900");
   settings.setValue("28" , "COMPUTER UXGA");
   settings.setValue("29" , "COMPUTER WSXGAP");
   settings.setValue("30" , "COMPUTER 1080p");
   settings.setValue("31" , "COMPUTER WUXGA");
   settings.setValue("32" , "COMPUTER 1920x1440");
   settings.setValue("33" , "COMPUTER 2K");
   settings.setValue("34" , "COMPUTER 2048x1152");
   settings.setValue("35" , "COMPUTER QXGA");
   settings.setValue("36" , "COMPUTER 2560x1440");
   settings.setValue("37" , "COMPUTER WQXGA");
   settings.setValue("38" , "COMPUTER CUSTOM 1");
   settings.setValue("39" , "COMPUTER CUSTOM 2");
   settings.setValue("40" , "COMPUTER CUSTOM 3");
   settings.setValue("41" , "COMPUTER CUSTOM 4");
   settings.setValue("42" , "COMPUTER CUSTOM 5");
   settings.setValue("43" , "COMPUTER CUSTOM 6");
   settings.setValue("44" , "COMPUTER CUSTOM 7");
   settings.setValue("45" , "COMPUTER CUSTOM 8");
   settings.setValue("46" , "COMPUTER CUSTOM 9");
   settings.setValue("47" , "COMPUTER CUSTOM 10");
   settings.setValue("48" , "UHDTV 2160p");
   settings.setValue("49" , "CINEMA 4K");
   settings.setValue("50" , "COMPUTER 2560x1080");
   settings.setValue("51" , "COMPUTER 1920x2160");
   settings.setValue("52" , "COMPUTER 2048x2160");
   settings.setValue("53" , "COMPUTER 3840x1080");
   settings.setValue("54" , "COMPUTER 4096x1080");

   settings.endGroup();

   //Création de la liste des rates
   settings.beginGroup(QStringLiteral("RATES"));

   settings.setValue("CUSTOM", 0);
   settings.setValue("INTERNAL RATE", 1);
   settings.setValue("23.97HZ", 2);
   settings.setValue("24HZ", 3);
   settings.setValue("25HZ", 4);
   settings.setValue("29.97HZ", 5);
   settings.setValue("30HZ", 6);
   settings.setValue("50HZ", 7);
   settings.setValue("59.94HZ", 8);
   settings.setValue("60HZ", 9);
   settings.setValue("72HZ", 10);
   settings.setValue("75HZ", 11);

   settings.endGroup();

   //Création de la liste des valeurs de rate
   settings.beginGroup(QStringLiteral("RATE_VALUE"));

   settings.setValue("0" , "CUSTOM");
   settings.setValue("1" , "INTERNAL RATE");
   settings.setValue("2" , "23.97HZ");
   settings.setValue("3" , "24HZ");
   settings.setValue("4" , "25HZ");
   settings.setValue("5" , "29.97HZ");
   settings.setValue("6" , "30HZ");
   settings.setValue("7" , "50HZ");
   settings.setValue("8" , "59.94HZ");
   settings.setValue("9" , "60HZ");
   settings.setValue("10" , "72HZ");
   settings.setValue("11" , "75HZ");

   settings.endGroup();
}


///
/// \fn WriteSettingsNLC
/// \brief Ecrit le fichier de config du NLC dans le dossier settings
///
void CLASS_WRITE_MACHINE_CONFIG::WriteSettingsNLC(void)
{
   QSettings settings(QStringLiteral("settings/settingNLC.ini"), QSettings::IniFormat);

   // Si le fichier existe le supprimer pour et le réécrire en cas de modification
   if(QFile(settings.fileName()).exists())
      QFile(settings.fileName()).remove();

   //Création de la liste des appareils correspondant à ce fichier de config
   settings.beginGroup(QStringLiteral("DEVICE"));

   settings.setValue("?0" , "NLC 4U");
   settings.setValue("DEV1" , "NLC 5U");

   settings.endGroup();

   //Création de la liste d'informations
   settings.beginGroup(QStringLiteral("INFO"));

   settings.setValue("MAX_INPUT" , 24);
   settings.setValue("MAX_INPUT_PLUG" , 4);
   settings.setValue("MAX_OUTPUT", 20);
   settings.setValue("MAX_OUTPUT_PLUG", 8);
   settings.setValue("MAX_OUTPUT_FORMAT" , 58);
   settings.setValue("MAX_OUTPUT_FORMAT_SDI_OPT" , 19);
   settings.setValue("MAX_RATE" , 16);
   settings.setValue("OUTPUT_NOT_AVAILABLE" , 0);
   settings.setValue("INPUT_NOT_AVAILABLE", 0);
   settings.setValue("OUTPUT_ACTIVE" , 1);
   settings.setValue("INPUT_ACTIVE", 1);

   settings.endGroup();


   //Création de la liste de COMMANDE
   settings.beginGroup(QStringLiteral("COMMAND"));

   settings.setValue("OUT_FORMAT" , "OUfor");
   settings.setValue("OUT_RATE", "OUrat");
   settings.setValue("OUT_FORMAT_UPDATE", "OUfru");
   settings.setValue("OUT_AVAILABLE" , "OSava");
   settings.setValue("OUT_PLUG_STATUS" , "OUpls");

   settings.setValue("SIG_DETECTED_FORMAT", "ISdfo");
   settings.setValue("SIG_SCAN_VALID" , "ISsva");
   settings.setValue("IN_AVAILABLE" , "INava");
   settings.setValue("IN_PLUG_AVAILABLE" , "INpav");
   settings.setValue("SIG_SIGNAL_WIDHT" , "ISswi");
   settings.setValue("SIG_SIGNAL_HEIGHT" , "ISshe");
   settings.setValue("SIG_FORMAT_WIDHT" , "ISfwi");
   settings.setValue("SIG_FORMAT_HEIGHT" , "ISfhe");
   settings.setValue("SIG_FREQ_FIELD" , "ISffi");

   settings.setValue("CUSTOM_COMP_GENE_MODE", "CCmod");
   settings.setValue("CUSTOM_COMP_LOAD_FROM_CUSTOM" , "CClfc");
   settings.setValue("CUSTOM_COMP_FULL_CVT_RATE" , "CCfra");
   settings.setValue("CUSTOM_COMP_CHECK_REQUEST" , "CCcrq");
   settings.setValue("CUSTOM_COMP_SAVE_FORMAT" , "CCsfo");
   settings.setValue("CUSTOM_COMP_CVT_REDUCED_BLK" , "CCrdb");

   settings.setValue("CUSTOM_COMP_FULL_CVT_HUTIL" , "CCfht");
   settings.setValue("CUSTOM_COMP_FULL_HFRONTPORCH" , "CChfp");
   settings.setValue("CUSTOM_COMP_FULL_HBACKPORCH" , "CChbp");
   settings.setValue("CUSTOM_COMP_FULL_HSYNC" , "CCfhs");
   settings.setValue("CUSTOM_COMP_FULL_HSYNC_POL" , "CChpo");

   settings.setValue("CUSTOM_COMP_FULL_CVT_VUTIL" , "CCvua");
   settings.setValue("CUSTOM_COMP_FULL_VFRONTPORCH" , "CCvfp");
   settings.setValue("CUSTOM_COMP_FULL_VBACKPORCH" , "CCvbp");
   settings.setValue("CUSTOM_COMP_FULL_VSYNC" , "CCvsy");
   settings.setValue("CUSTOM_COMP_FULL_VSYNC_POL" , "CCvpo");

   settings.endGroup();


   //Création  de la liste d'input
   settings.beginGroup(QStringLiteral("INPUT"));

   settings.setValue("IN_1" , 0);
   settings.setValue("IN_2" , 1);
   settings.setValue("IN_3" , 2);
   settings.setValue("IN_4" , 3);
   settings.setValue("IN_5" , 4);
   settings.setValue("IN_6" , 5);
   settings.setValue("IN_7" , 6);
   settings.setValue("IN_8" , 7);
   settings.setValue("IN_9" , 8);
   settings.setValue("IN_10" , 9);
   settings.setValue("IN_11" , 10);
   settings.setValue("IN_12" , 11);
   settings.setValue("IN_13" , 12);
   settings.setValue("IN_14" , 13);
   settings.setValue("IN_15" , 14);
   settings.setValue("IN_16" , 15);
   settings.setValue("IN_17" , 16);
   settings.setValue("IN_18" , 17);
   settings.setValue("IN_19" , 18);
   settings.setValue("IN_20" , 19);
   settings.setValue("IN_21" , 20);
   settings.setValue("IN_22" , 21);
   settings.setValue("IN_23" , 22);
   settings.setValue("IN_24" , 23);

   settings.endGroup();

   //Création  de la liste de valeur d'input
   settings.beginGroup(QStringLiteral("INPUT_VALUE"));

   settings.setValue("0" , "IN_1");
   settings.setValue("1" , "IN_2");
   settings.setValue("2" , "IN_3");
   settings.setValue("3" , "IN_4");
   settings.setValue("4" , "IN_5");
   settings.setValue("5" , "IN_6");
   settings.setValue("6" , "IN_7");
   settings.setValue("7" , "IN_8");
   settings.setValue("8" , "IN_9");
   settings.setValue("9" , "IN_10");
   settings.setValue("10" , "IN_11");
   settings.setValue("11" , "IN_12");
   settings.setValue("12" , "IN_13");
   settings.setValue("13" , "IN_14");
   settings.setValue("14" , "IN_15");
   settings.setValue("15" , "IN_16");
   settings.setValue("16" , "IN_17");
   settings.setValue("17" , "IN_18");
   settings.setValue("18" , "IN_19");
   settings.setValue("19" , "IN_20");
   settings.setValue("20" , "IN_21");
   settings.setValue("21" , "IN_22");
   settings.setValue("22" , "IN_23");
   settings.setValue("23" , "IN_24");

   settings.endGroup();


   //Création de la liste des plug en entrée
   settings.beginGroup(QStringLiteral("INPUT_PLUG"));

   settings.setValue("DISPLAY_PORT" , 0);
   settings.setValue("HDMI" , 1);
   settings.setValue("ANALOG_HD15" , 2);
   settings.setValue("OPTICAL_SFP" , 3);
   settings.setValue("SDI" , 4);
   settings.setValue("DVI_D" , 5);
   settings.setValue("HDBASET" , 6);
   settings.setValue("QUAD_SDI" , 7);

   settings.endGroup();

   //Création de la liste des valeur de plug en entrée
   settings.beginGroup("INPUT_PLUG_VALUE");

   settings.setValue("0" , "DISPLAY_PORT");
   settings.setValue("1" , "HDMI");
   settings.setValue("2" , "ANALOG_HD15");
   settings.setValue("3" , "OPTICAL_SFP");
   settings.setValue("4" , "SDI");
   settings.setValue("5" , "DVI_D");
   settings.setValue("6" , "HDBASET");
   settings.setValue("7" , "QUAD_SDI");

   settings.endGroup();


   //Création de la liste de format d'entrée
   settings.beginGroup("INPUT_FORMAT");

   settings.setValue("NONE" , 0);
   settings.setValue("INVALID" , 1);
   settings.setValue("UNKNOWN" , 2);
   settings.setValue("SDTV_NTSC", 3);
   settings.setValue("SDTV_PAL" , 4);
   settings.setValue("SDTV_SECAM" , 5);
   settings.setValue("SDTV_480i" , 6);
   settings.setValue("SDTV_576i" , 7);
   settings.setValue("EDTV_480p", 8);
   settings.setValue("EDTV_576p" , 9);
   settings.setValue("HDTV_720p" , 10);
   settings.setValue("HDTV_1035i", 11);
   settings.setValue("HDTV_1080i" , 12);
   settings.setValue("HDTV_1080p" , 13);
   settings.setValue("PROJ_1200p" , 14);
   settings.setValue("CINEMA_2K" , 15);
   settings.setValue("UHDTV_2160p", 16);
   settings.setValue("CINEMA_4K" , 17);
   settings.setValue("CEA_240p", 18);
   settings.setValue("CEA_288p" , 19);
   settings.setValue("CPU_640x350" , 20);
   settings.setValue("CPU_640x400", 21);
   settings.setValue("CPU_720x400" , 22);
   settings.setValue("CPU_640x480" , 23);
   settings.setValue("CPU_800x480" , 24);
   settings.setValue("CPU_848x480" , 25);
   settings.setValue("CPU_800x600", 26);
   settings.setValue("CPU_1280x600" , 27);
   settings.setValue("CPU_1280x720", 28);
   settings.setValue("CPU_1680x720" , 29);
   settings.setValue("CPU_1024x768" , 30);
   settings.setValue("CPU_1280x768", 31);
   settings.setValue("CPU_1360x768" , 32);
   settings.setValue("CPU_1366x768" , 33);
   settings.setValue("CPU_1280x800" , 34);
   settings.setValue("CPU_1366x800" , 35);
   settings.setValue("CPU_1088x817", 36);
   settings.setValue("CPU_1152x864" , 37);
   settings.setValue("CPU_1440x900" , 38);
   settings.setValue("CPU_1600x900" , 39);
   settings.setValue("CPU_1280x960", 40);
   settings.setValue("CPU_1280x1024", 41);
   settings.setValue("CPU_1360x1024", 42);
   settings.setValue("CPU_1400x1050", 43);
   settings.setValue("CPU_1680x1050", 44);
   settings.setValue("CPU_1440x1080", 45);
   settings.setValue("CPU_1920x1080", 46);
   settings.setValue("CPU_2048x1080", 47);
   settings.setValue("CPU_2560x1080" , 48);
   settings.setValue("CPU_3840x1080" , 49);
   settings.setValue("CPU_4096x1080", 50);
   settings.setValue("CPU_2048x1152", 51);
   settings.setValue("CPU_1600x1200", 52);
   settings.setValue("CPU_1920x1200", 53);
   settings.setValue("CPU_1792x1344", 54);
   settings.setValue("CPU_1856x1392", 55);
   settings.setValue("CPU_1920x1440", 56);
   settings.setValue("CPU_2560x1440", 57);
   settings.setValue("CPU_3440x1440" , 58);
   settings.setValue("CPU_2048x1536" , 59);
   settings.setValue("CPU_2560x1600", 60);
   settings.setValue("CPU_2560x2048", 61);
   settings.setValue("CPU_1920x2160", 62);
   settings.setValue("CPU_2048x2160", 63);
   settings.setValue("CPU_4096x2160", 64);
   settings.setValue("CPU_CVT" , 65);
   settings.setValue("CPU_GTF_5_4_RATIO" , 66);
   settings.setValue("CPU_GTF_4_3_RATIO", 67);
   settings.setValue("CPU_GTF_16_10_RATIO", 68);
   settings.setValue("CPU_GTF_15_9_RATIO", 69);
   settings.setValue("CPU_GTF_16_9_RATIO", 70);
   settings.setValue("CPU_1080x1920", 71);
   settings.setValue("CPU_1200x1920", 72);
   settings.setValue("CUSTOM", 73);


   settings.endGroup();


   //Création de la liste des formats d'entrée
   settings.beginGroup("INPUT_FORMAT_VALUE");

   settings.setValue("0" , "NONE");
   settings.setValue("1" , "INVALID");
   settings.setValue("2" , "UNKNOWN");
   settings.setValue("3" , "SDTV_NTSC");
   settings.setValue("4" , "SDTV_PAL");
   settings.setValue("5" , "SDTV_SECAM");
   settings.setValue("6" , "SDTV_480i");
   settings.setValue("7" , "SDTV_576i");
   settings.setValue("8" , "EDTV_480p");
   settings.setValue("9" , "EDTV_576p");
   settings.setValue("10" , "HDTV_720p");
   settings.setValue("11" , "HDTV_1035i");
   settings.setValue("12" , "HDTV_1080i");
   settings.setValue("13" , "HDTV_1080p");
   settings.setValue("14" , "PROJ_1200p");
   settings.setValue("15" , "CINEMA_2K");
   settings.setValue("16" , "UHDTV_2160p");
   settings.setValue("17" , "CINEMA_4K");
   settings.setValue("18" , "CEA_240p");
   settings.setValue("19" , "CEA_288p");
   settings.setValue("20" , "CPU_640x350");
   settings.setValue("21" , "CPU_640x400");
   settings.setValue("22" , "CPU_720x400");
   settings.setValue("23" , "CPU_640x480");
   settings.setValue("24" , "CPU_800x480");
   settings.setValue("25" , "CPU_848x480");
   settings.setValue("26" , "CPU_800x600");
   settings.setValue("27" , "CPU_1280x600");
   settings.setValue("28" , "CPU_1280x720");
   settings.setValue("29" , "CPU_1680x720");
   settings.setValue("30" , "CPU_1024x768");
   settings.setValue("31" , "CPU_1280x768");
   settings.setValue("32" , "CPU_1360x768");
   settings.setValue("33" , "CPU_1366x768");
   settings.setValue("34" , "CPU_1280x800");
   settings.setValue("35" , "CPU_1366x800");
   settings.setValue("36" , "CPU_1088x817");
   settings.setValue("37" , "CPU_1152x864");
   settings.setValue("38" , "CPU_1440x900");
   settings.setValue("39" , "CPU_1600x900");
   settings.setValue("40" , "CPU_1280x960");
   settings.setValue("41" , "CPU_1280x1024");
   settings.setValue("42" , "CPU_1360x1024");
   settings.setValue("43" , "CPU_1400x1050");
   settings.setValue("44" , "CPU_1680x1050");
   settings.setValue("45" , "CPU_1440x1080");
   settings.setValue("46" , "CPU_1920x1080");
   settings.setValue("47" , "CPU_2048x1080");
   settings.setValue("48" , "CPU_2560x1080");
   settings.setValue("49" , "CPU_3840x1080");
   settings.setValue("50" , "CPU_4096x1080");
   settings.setValue("51" , "CPU_2048x1152");
   settings.setValue("52" , "CPU_1600x1200");
   settings.setValue("53" , "CPU_1920x1200");
   settings.setValue("54" , "CPU_1792x1344");
   settings.setValue("55" , "CPU_1856x1392");
   settings.setValue("56" , "CPU_1920x1440");
   settings.setValue("57" , "CPU_2560x1440");
   settings.setValue("58" , "CPU_3440x1440");
   settings.setValue("59" , "CPU_2048x1536");
   settings.setValue("60" , "CPU_2560x1600");
   settings.setValue("61" , "CPU_2560x2048");
   settings.setValue("62" , "CPU_1920x2160");
   settings.setValue("63" , "CPU_2048x2160");
   settings.setValue("64" , "CPU_4096x2160");
   settings.setValue("65" , "CPU_CVT");
   settings.setValue("66" , "CPU_GTF_5_4_RATIO");
   settings.setValue("67" , "CPU_GTF_4_3_RATIO");
   settings.setValue("68" , "CPU_GTF_16_10_RATIO");
   settings.setValue("69" , "CPU_GTF_15_9_RATIO");
   settings.setValue("70" , "CPU_GTF_16_9_RATIO");
   settings.setValue("71" , "CPU_1080x1920");
   settings.setValue("72" , "CPU_1200x1920");
   settings.setValue("73" , "CUSTOM");

   settings.endGroup();


   //Création de la liste d'Output
   settings.beginGroup("OUTPUT");

   settings.setValue("OUTPUT 1" , 0);
   settings.setValue("OUTPUT 2" , 1);
   settings.setValue("OUTPUT 3" , 2);
   settings.setValue("OUTPUT 4" , 3);
   settings.setValue("OUTPUT 5" , 4);
   settings.setValue("OUTPUT 6" , 5);
   settings.setValue("OUTPUT 7" , 6);
   settings.setValue("OUTPUT 8" , 7);
   settings.setValue("OUTPUT 9" , 8);
   settings.setValue("OUTPUT 10" , 9);
   settings.setValue("OUTPUT 11" , 10);
   settings.setValue("OUTPUT 12" , 11);
   settings.setValue("OUTPUT 13" , 12);
   settings.setValue("OUTPUT 14" , 13);
   settings.setValue("OUTPUT 15" , 14);
   settings.setValue("OUTPUT 16" , 15);
   settings.setValue("OUTPUT 17" , 16);
   settings.setValue("OUTPUT 18" , 17);
   settings.setValue("OUTPUT 19" , 18);
   settings.setValue("OUTPUT 20" , 19);

   settings.endGroup();

   //Création de la liste pour récupérer le nom en fonction de la valeur
   settings.beginGroup("OUTPUT_VALUE");

   settings.setValue("0" , "OUTPUT 1");
   settings.setValue("1" , "OUTPUT 2");
   settings.setValue("2" , "OUTPUT 3");
   settings.setValue("3" , "OUTPUT 4");
   settings.setValue("4" , "OUTPUT 5");
   settings.setValue("5" , "OUTPUT 6");
   settings.setValue("6" , "OUTPUT 7");
   settings.setValue("7" , "OUTPUT 8");
   settings.setValue("8" , "OUTPUT 9");
   settings.setValue("9" , "OUTPUT 10");
   settings.setValue("10" , "OUTPUT 11");
   settings.setValue("11" , "OUTPUT 12");
   settings.setValue("12" , "OUTPUT 13");
   settings.setValue("13" , "OUTPUT 14");
   settings.setValue("14" , "OUTPUT 15");
   settings.setValue("15" , "OUTPUT 16");
   settings.setValue("16" , "OUTPUT 17");
   settings.setValue("17" , "OUTPUT 18");
   settings.setValue("18" , "OUTPUT 19");
   settings.setValue("19" , "OUTPUT 20");

   settings.endGroup();

   //Création de la liste de plug de sortie
   settings.beginGroup("OUTPUT_PLUG");

   settings.setValue("DISPLAY_PORT" , 0);
   settings.setValue("HDMI" , 1);
   settings.setValue("SDI" , 2);

   settings.endGroup();

   //Création de la liste des valeurs des plugs de sortie
   settings.beginGroup("OUTPUT_PLUG_VALUE");

   settings.setValue("0" , "DISPLAY_PORT");
   settings.setValue("1" , "HDMI");
   settings.setValue("2" , "SDI");

   settings.endGroup();


   //Création de la liste des status du plug de sortie
   settings.beginGroup("OUTPUT_PLUG_STATUS");

   settings.setValue("NOT_AVAILABLE" , 0);
   settings.setValue("ACTIVE" , 1);
   settings.setValue("WARNING_EDID_INVALID" , 2);
   settings.setValue("DISABLE_HDCP" , 3);
   settings.setValue("DISABLE_FORMAT" , 4);
   settings.setValue("DISABLE_NO_DISPLAY" , 5);
   settings.setValue("DISABLE_DISPLAY_OFF" , 6);
   settings.setValue("DISABLE_CONNECTION_FAILED" , 7);
   settings.setValue("DISABLE_REF_INCOMPATIBLE" , 8);
   settings.setValue("FORMAT_NOT_SUPPORT_BY_DISPLAY" , 9);

   settings.endGroup();

   //Création de la liste des valeurs des status des plugs de sortie
   settings.beginGroup("OUTPUT_PLUG_STATUS_VALUE");

   settings.setValue("0" , "NOT_AVAILABLE");
   settings.setValue("1" , "ACTIVE");
   settings.setValue("2" , "WARNING_EDID_INVALID");
   settings.setValue("3" , "DISABLE_HDCP");
   settings.setValue("4" , "DISABLE_FORMAT");
   settings.setValue("5" , "DISABLE_NO_DISPLAY");
   settings.setValue("6" , "DISABLE_DISPLAY_OFF");
   settings.setValue("7" , "DISABLE_CONNECTION_FAILED");
   settings.setValue("8" , "DISABLE_REF_INCOMPATIBLE");
   settings.setValue("9" , "FORMAT_NOT_SUPPORT_BY_DISPLAY");

   settings.endGroup();


   //Création de la liste des format de sortie
   settings.beginGroup("OUTPUT_FORMAT");

   settings.setValue("SDTV_PAL_4_3" , 0);
   settings.setValue("SDTV_PAL_16_9" , 1);
   settings.setValue("SDTV_NTSC_4_3" , 2);
   settings.setValue("SDTV_NTSC_16_9" , 3);
   settings.setValue("CEA861_480I_4_3" , 4);
   settings.setValue("CEA861_480I_16_9" , 5);
   settings.setValue("EDTV_480P_4_3" , 6);
   settings.setValue("EDTV_480P_16_9" , 7);
   settings.setValue("EDTV_576P_4_3" , 8);
   settings.setValue("EDTV_576P_16_9" , 9);
   settings.setValue("HDTV_720p" , 10);
   settings.setValue("HDTV_1035i" , 11);
   settings.setValue("HDTV_1080i" , 12);
   settings.setValue("HDTV_1080sF" , 13);
   settings.setValue("HDTV_1080p" , 14);
   settings.setValue("PROJ_1200p" , 15);
   settings.setValue("CINEMA_2K" , 16);
   settings.setValue("UHDTV_2160p" , 17);
   settings.setValue("CINEMA_4K" , 18);
   settings.setValue("COMPUTER_VGA" , 19);
   settings.setValue("COMPUTER_800_480" , 20);
   settings.setValue("COMPUTER_SVGA" , 21);
   settings.setValue("COMPUTER_WVGA" , 22);
   settings.setValue("COMPUTER_XGA" , 23);
   settings.setValue("COMPUTER_1088_817" , 24);
   settings.setValue("COMPUTER_1152_864" , 25);
   settings.setValue("COMPUTER_720p" , 26);
   settings.setValue("COMPUTER_WXGA" , 27);
   settings.setValue("COMPUTER_WXGA2" , 28);
   settings.setValue("COMPUTER_1280_960" , 29);
   settings.setValue("COMPUTER_SXGA" , 30);
   settings.setValue("COMPUTER_1360_768" , 31);
   settings.setValue("COMPUTER_1360_1024" , 32);
   settings.setValue("COMPUTER_SWXGAPB" , 33);
   settings.setValue("COMPUTER_SWXGAP", 34);
   settings.setValue("COMPUTER_SXGAP" , 35);
   settings.setValue("COMPUTER_900p" , 36);
   settings.setValue("COMPUTER_1440_1080" , 37);
   settings.setValue("COMPUTER_1600_900" , 38);
   settings.setValue("COMPUTER_UXGA" ,  39);
   settings.setValue("COMPUTER_WSXGAP" , 40);
   settings.setValue("COMPUTER_1792_1344" , 41);
   settings.setValue("COMPUTER_1856_1392" , 42);
   settings.setValue("COMPUTER_1080p" , 43);
   settings.setValue("COMPUTER_WUXGA" , 44);
   settings.setValue("COMPUTER_1920_1440" , 45);
   settings.setValue("COMPUTER_1920_2160" , 46);
   settings.setValue("COMPUTER_2K" , 47);
   settings.setValue("COMPUTER_2048_1152" , 48);
   settings.setValue("COMPUTER_QXGA" , 49);
   settings.setValue("COMPUTER_2048_2160" , 50);
   settings.setValue("COMPUTER_2560_1080" , 51);
   settings.setValue("COMPUTER_2560_1440" , 52);
   settings.setValue("COMPUTER_WQXGA" , 53);
   settings.setValue("COMPUTER_QSXGA" , 54);
   settings.setValue("COMPUTER_3440_1440" , 55);
   settings.setValue("COMPUTER_3840_1080" , 56);
   settings.setValue("COMPUTER_4096_1080" , 57);
   settings.setValue("COMPUTER_CUSTOM_1" , 58);
   settings.setValue("COMPUTER_CUSTOM_2" , 59);

   settings.endGroup();


   //Création de la liste des valeurs de format
   settings.beginGroup("OUTPUT_FORMAT_VALUE");

   settings.setValue("0" , "SDTV_PAL_4_3");
   settings.setValue("1" , "SDTV_PAL_16_9");
   settings.setValue("2" , "SDTV_NTSC_4_3");
   settings.setValue("3" , "SDTV_NTSC_16_9");
   settings.setValue("4" , "CEA861_480I_4_3");
   settings.setValue("5" , "CEA861_480I_16_9");
   settings.setValue("6" , "EDTV_480P_4_3");
   settings.setValue("7" , "EDTV_480P_16_9");
   settings.setValue("8" , "EDTV_576P_4_3");
   settings.setValue("9" , "EDTV_576P_16_9");
   settings.setValue("10" , "HDTV_720p");
   settings.setValue("11" , "HDTV_1035i");
   settings.setValue("12" , "HDTV_1080i");
   settings.setValue("14" , "HDTV_1080p");
   settings.setValue("15" , "PROJ_1200p");
   settings.setValue("16" , "CINEMA_2K");
   settings.setValue("17" , "UHDTV_2160p");
   settings.setValue("18" , "CINEMA_4K");
   settings.setValue("19" , "COMPUTER_VGA");
   settings.setValue("20" , "COMPUTER_800_480");
   settings.setValue("21" , "COMPUTER_SVGA");
   settings.setValue("22" , "COMPUTER_WVGA");
   settings.setValue("23" , "COMPUTER_XGA");
   settings.setValue("24" , "COMPUTER_1088_817");
   settings.setValue("25" , "COMPUTER_1152_864");
   settings.setValue("26" , "COMPUTER_720p");
   settings.setValue("27" , "COMPUTER_WXGA");
   settings.setValue("28" , "COMPUTER_WXGA2");
   settings.setValue("29" , "COMPUTER_1280_960");
   settings.setValue("30" , "COMPUTER_SXGA");
   settings.setValue("31" , "COMPUTER_1360_768");
   settings.setValue("32" , "COMPUTER_1360_1024");
   settings.setValue("33" , "COMPUTER_SWXGAPB");
   settings.setValue("34" , "COMPUTER_SWXGAP");
   settings.setValue("35" , "COMPUTER_SXGAP");
   settings.setValue("36" , "COMPUTER_900p");
   settings.setValue("37" , "COMPUTER_1440_1080");
   settings.setValue("38" , "COMPUTER_1600_900");
   settings.setValue("39" , "COMPUTER_UXGA");
   settings.setValue("40" , "COMPUTER_WSXGAP");
   settings.setValue("41" , "COMPUTER_1792_1344");
   settings.setValue("42" , "COMPUTER_1856_1392");
   settings.setValue("43" , "COMPUTER_1080p");
   settings.setValue("44" , "COMPUTER_WUXGA");
   settings.setValue("45" , "COMPUTER_1920_1440");
   settings.setValue("46" , "COMPUTER_1920_2160");
   settings.setValue("47" , "COMPUTER_2K");
   settings.setValue("48" , "COMPUTER_2048_1152");
   settings.setValue("49" , "COMPUTER_QXGA");
   settings.setValue("50" , "COMPUTER_2048_2160");
   settings.setValue("51" , "COMPUTER_2560_1080");
   settings.setValue("52" , "COMPUTER_2560_1440");
   settings.setValue("53" , "COMPUTER_WQXGA");
   settings.setValue("54" , "COMPUTER_QSXGA");
   settings.setValue("55" , "COMPUTER_3440_1440");
   settings.setValue("56" , "COMPUTER_3840_1080");
   settings.setValue("57" , "COMPUTER_4096_1080");
   settings.setValue("58" , "COMPUTER_CUSTOM_1");
   settings.setValue("59" , "COMPUTER_CUSTOM_2");

   settings.endGroup();

   //Création de la liste des rates
   settings.beginGroup("RATE");

   settings.setValue("23_97HZ", 0);
   settings.setValue("24HZ", 1);
   settings.setValue("25HZ", 2);
   settings.setValue("29_97HZ", 3);
   settings.setValue("30HZ", 4);
   settings.setValue("47_95HZ", 5);
   settings.setValue("48HZ", 6);
   settings.setValue("50HZ", 7);
   settings.setValue("59_94HZ", 8);
   settings.setValue("60HZ", 9);
   settings.setValue("72HZ", 10);
   settings.setValue("75HZ", 11);
   settings.setValue("85HZ", 12);
   settings.setValue("100HZ", 13);
   settings.setValue("119_88HZ", 14);
   settings.setValue("120HZ", 15);

   settings.endGroup();


   //Création de la liste des valeurs de rate
   settings.beginGroup("RATE_VALUE");

   settings.setValue("0" , "23_97HZ");
   settings.setValue("1" , "24HZ");
   settings.setValue("2" , "25HZ");
   settings.setValue("3" , "29_97HZ");
   settings.setValue("4" , "30HZ");
   settings.setValue("5" , "47_95HZ");
   settings.setValue("6" , "48HZ");
   settings.setValue("7" , "50HZ");
   settings.setValue("8" , "59_94HZ");
   settings.setValue("9" , "60HZ");
   settings.setValue("10" , "72HZ");
   settings.setValue("11" , "75HZ");
   settings.setValue("12" , "85HZ");
   settings.setValue("13" , "100HZ");
   settings.setValue("14" , "119_88HZ");
   settings.setValue("15" , "120HZ");

   settings.endGroup();
}


///
/// \fn WriteSettingsMNG
/// \brief Ecrit le fichier de config du MNG dans le dossier settings
///
void CLASS_WRITE_MACHINE_CONFIG::WriteSettingsMNG()
{
   QSettings settings(QStringLiteral("settings/settingMNG.ini"), QSettings::IniFormat);

   // Si le fichier existe le supprimer pour et le réécrire en cas de modification
   if(QFile(settings.fileName()).exists())
      QFile(settings.fileName()).remove();

   settings.beginGroup(QStringLiteral("Creation fichier"));

   settings.setValue("Test creation fichier" , "Reussie");

   settings.endGroup();

}

/******************************************************************************
 *                             Operateur surcharge                            *
 *****************************************************************************/
