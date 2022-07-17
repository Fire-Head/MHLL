# MHLL
Manhunt Language Loader is a plugin that gives you an ability to add extra languages for your game without replacing the default ones.

# Requirements
* UAL (https://github.com/ThirteenAG/Ultimate-ASI-Loader)

# Brief instruction
Create a folder inside "MHLL" and then add lang.ini to it.
<details>
  <summary>"MHLL\MYLANG\lang.ini"</summary>

  ```ini
[LANG]
; Name of your language in menu. Accepts both custom string and GXT key.
; Examples:
;	NameKey = ESP
;		In this case LL will load "ESP" key string from your PC_GAME.GXT.
;		Useful when your language originally made to replace a default one.
;		Don't worry, you don't have to edit anything, LL takes each language name from its own PC_GAME_X.GXT file unlike the original game.
;
;	NameKey = MYLANGUAGE
;		In this case "MYLANGUAGE" will be used as language name in the menu.
;		Be careful, it's encoding and case sensitive!
;
NameKey =


; Name of txd file that contains font textures for your language.
; LL is looking for {"loading", "manhunt", "lcd", "lcdm"} textures in this txd. If one or more textures are missing - LL will take a default one instead.
; Examples:
;	TextureFile = font_c.txd
;		In this case LL will use "MHLL\MYLANG\font_c.txd" as fonts for your language.
;
;	TextureFile =
;		In this case default game fonts will be used.
;
TextureFile =


; Name of dat file that contains font settings for your language.
; Examples:
;	DataFile = font_c.dat
;		In this case LL will use "MHLL\MYLANG\font_c.dat" as font for your language.
;
;	DataFile =
;		In this case default game font settings will be used.
;
DataFile =


; Name of subdirectory where your language GXT files are located.
; Examples:
;	TextDirectory = Text\
;		In this case LL will search for GXT files in "Text" directory:
;			"MHLL\MYLANG\Text\pc_asylum.gxt"
;			"MHLL\MYLANG\Text\pc_zoo.gxt"
;			"MHLL\MYLANG\Text\pc_game.gxt"
;
;	TextDirectory = %GAMEPATH%
;		In this case LL will use original directory structure:
;			"MHLL\MYLANG\levels\asylum\pc_text\pc_asylum.gxt"
;			"MHLL\MYLANG\levels\zoo\pc_text\pc_zoo.gxt"
;			"MHLL\MYLANG\levels\GLOBAL\pc_text\pc_game.gxt"
;
TextDirectory =


; Name of global GXT(PC_GAME.GXT) file.
; Examples:
;	GameGXTName = PC_GAME_SPA.GXT
;		In this case LL will load "PC_GAME_SPA.GXT".
;
;	GameGXTName = PC_GAME.GXT
;		In this case LL will load "PC_GAME.GXT".
;
;	GameGXTName = PC_GAME_K.GXT
;		In this case LL will load "PC_GAME_K.GXT".
;
GameGXTName =


; Postfix at the and of each level GXT name.
; Examples:
;	LevelGXTPostfix = _s
;		In this case LL will load:
;			"MHLL\MYLANG\%TextDirectory%\pc_asylum_s.gxt"
;			"MHLL\MYLANG\%TextDirectory%\pc_zoo_s.gxt"
;
;	LevelGXTPostfix =
;		In this case LL will load:
;			"MHLL\MYLANG\%TextDirectory%\pc_asylum.gxt"
;			"MHLL\MYLANG\%TextDirectory%\pc_zoo.gxt"
;
LevelGXTPostfix =


; Extends ascii table from 128 to 256 characters. Accepts 0 and 1.
; Examples:
;	UseWholeAsciiTable = 0
;		Disables the option.
;
;	UseWholeAsciiTable = 1
;		Enables the option.
;
UseWholeAsciiTable =


; Windows ID of your language.
; See langid.txt for your language ID.
; The game uses it to set default game language to your system language when "\Manhunt User Files\SaveGames\Settings.dat" does not exist(e.g. before first launch).
; Examples:
;	WindowsLangID = 0x0b
;		In this case LL will set your language as default if your system language is LANG_FINNISH and "Settings.dat" file is missing.
;
;	WindowsLangID =
;		In this case LL will ignore this feature for your language.
;
WindowsLangID =
  ```
</details>

Using hints inside the ini configure and add .gxt/.txd/.dat files.

Examples can be found in [Releases section](https://github.com/Fire-Head/MHLL/releases)


# Showcase
![1](https://user-images.githubusercontent.com/26774830/179380085-f292f22b-72ec-4a68-a492-6377fd77ad5b.png)
![2](https://user-images.githubusercontent.com/26774830/179380087-fc0fe651-d992-4ffd-a641-a032c6d6003e.png)
![3](https://user-images.githubusercontent.com/26774830/179380090-5aa8feb1-65a7-44cb-9fd0-b2052932004d.png)

