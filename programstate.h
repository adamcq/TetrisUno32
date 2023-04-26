
#ifndef PROGRAMSTATE_H
#define PROGRAMSTATE_H



#define HIGHSCORE_NAME_MAX_CHAR 3
#define HIGHSCORE_MAX_ENTRIES 3

typedef char HighscoreName[HIGHSCORE_NAME_MAX_CHAR];

/*
 * Struct contains pair {highscoreName, score} data
 */
typedef struct HighscoreEntry {
   HighscoreName highscoreName;
   int score;
} HighscoreEntry;

typedef struct HighscoreState {
    HighscoreEntry higscores[HIGHSCORE_MAX_ENTRIES];
} HighscoreState;


typedef struct GameoverPageState {
    /*
     * Chars for the name that have been entered so far
     *
     * '\0' is used as placeholder
     */
    char enteredChars[HIGHSCORE_NAME_MAX_CHAR];
    /*
     * Points at the index of [enteredChars] which is currently modified
     *
     * The following condition is always satisfied:
     * 0 <= inputPointer <= HIGHTSCORE_NAME_MAX_CHAR
     */
    unsigned int inputPointer;

    /*
     * The score that the player has reached during the game
     */
    int score;

    /*
     * Here the alphabet index of each char in [enteredChars] is stored
     */
    int letterAlphabetIndexes[HIGHSCORE_MAX_ENTRIES];
} GameoverPageState;


typedef enum ProgramPage {
    MENU, HIGHSCORE, GAME, GAME_OVER
} ProgramPage;

typedef enum MenuEntry {
    MENU_ENTRY_START,
    MENU_ENTRY_SCORE
} MenuEntry;

typedef struct MenuState {
    MenuEntry menuEntry;
} MenuState;

typedef enum ProgramCommand {
    BTN1,
    BTN2,
    BTN3,
    BTN4,
    VOID
} ProgramCommand;

typedef enum MenuCommand {
    CURSOR_DOWN,
    CURSOR_UP,
    ENTER
} MenuCommand;

typedef enum GameoverPageCommand {
    INC,
    DEC,
    BACK,
    ACCEPT,
} GameoverPageCommand;

typedef enum HighscorePageCommand {
    EXIT
} HighscorePageCommand;

void run(ProgramCommand programCommand);
void dispatchDisplayUpdate();
void saveHighscore(const char name[], int score, HighscoreState *highscoreState);

ProgramPage currentProgramPage;
MenuState currentMenuState;
GameoverPageState currentGameoverPageState;
HighscoreState currentHighscoreState;


#endif