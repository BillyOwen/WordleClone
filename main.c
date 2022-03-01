#define OLC_PGE_APPLICATION
#include "olcPixelGameEngineC.h"



struct keyboardLetter
{
    char value[2];
    olc_Pixel backgroundColour;
};

struct keyboardLetter keyboard[26];

char theWord[5];
bool displayAnswer = false;

char* wordList;
int wordsInWordList = 0;

int wordPointer = 0;
int guessPointer = 0;

#define WORD_LENGTH  5
#define MAX_GUESSES  6

struct keyboardLetter guessArray[WORD_LENGTH * MAX_GUESSES];

void drawGuessingGrid(int x, int y, int size)
{
    for (int i = 0; i < MAX_GUESSES; ++i)
        for (int j = 0; j < WORD_LENGTH; ++j)
        {
            PGE_DrawRect(x + j * size * 11, y + i * size * 11, size * 9, size * 9, olc_BLACK);

            const int index = i * WORD_LENGTH + j;
            if (index < wordPointer + guessPointer * WORD_LENGTH)
            {
                //Draw letter stuff
                PGE_FillRect(x + j * size * 11 + 1, y + i * size * 11 + 1, size * 9 - 1, size * 9 - 1, guessArray[index].backgroundColour);
                PGE_DrawString(x + j * size * 11 + 1 + size, y + i * size * 11 + 1 + size, guessArray[index].value, olc_BLACK, size);
            }
            else
            {
                PGE_FillRect(x + j * size * 11 + 1, y + i * size * 11 + 1, size * 9 - 1, size * 9 - 1, olc_WHITE);
            }
        }

    if (displayAnswer)
    {
        PGE_DrawString(x, y + MAX_GUESSES * size * 11, theWord, olc_RED, size);
    }

}

void drawKeyboard(int x, int y, int size)
{
    //Top Row
    for (int i = 0; i < 10; ++i)
    {

        PGE_FillRect(x + i * size * 11, y, size * 9, size * 9, keyboard[i].backgroundColour);
        PGE_DrawRect(x + i * size * 11 - 1, y - 1, size * 9 + 1, size * 9 + 1, olc_BLACK);

        PGE_DrawString(x + i * size * 11 + 2, y + size, keyboard[i].value, olc_WHITE, size);
    }
    //Home Row
    for (int i = 0; i < 9; ++i)
    {

        PGE_FillRect(x + i * size * 11 + size * 11.0f / 2.0f, y + size * 11, size * 9, size * 9, keyboard[i + 10].backgroundColour);
        PGE_DrawRect(x + i * size * 11 - 1 + size * 11.0f / 2.0f, y - 1 + size * 11, size * 9 + 1, size * 9 + 1, olc_BLACK);

        PGE_DrawString(x + i * size * 11 + 2 + size * 11.0f / 2.0f, y + size + size * 11, keyboard[i + 10].value, olc_WHITE, size);
    }
    //Bottom Row
    for (int i = 0; i < 7; ++i)
    {

        PGE_FillRect(x + i * size * 11 + size * 11 * 3.0f / 2.0f, y + size * 22, size * 9, size * 9, keyboard[i + 19].backgroundColour);
        PGE_DrawRect(x + i * size * 11 - 1 + size * 11 * 3.0f / 2.0f, y - 1 + size * 22, size * 9 + 1, size * 9 + 1, olc_BLACK);

        PGE_DrawString(x + i * size * 11 + 2 + size * 11 * 3.0f / 2.0f, y + size + size * 22, keyboard[i + 19].value, olc_WHITE, size);
    }

    //Draw Enter and Backspace
    PGE_FillRect(x, y + size * 22, size * 15, size * 9, olc_PixelRGB(170, 170, 170));
    PGE_DrawRect(x - 1, y + size * 22 - 1, size * 15 + 1, size * 9 + 1, olc_BLACK);
    PGE_DrawString(x + 3, y + size * 25, "ENT", olc_WHITE, size - 1);

    PGE_FillRect(x + 7 * size * 11 + size * 11 * 3.0f / 2.0f, y + size * 22, size * 17, size * 9, olc_PixelRGB(170, 170, 170));
    PGE_DrawRect(x + 7 * size * 11 - 1 + size * 11 * 3.0f / 2.0f, y - 1 + size * 22, size * 17 + 1, size * 9 + 1, olc_BLACK);
    PGE_DrawString(x + 7 * size * 11 + size * 11 * 3.0f / 2.0f + size / 2.0f, y + size * 25, "BACK", olc_WHITE, size - 1);
}

int normalToQwerty(int num)
{
    switch (num)
    {
    case 0: return 10; //A
    case 1: return 23; //B
    case 2: return 21; //C
    case 3: return 12; //D
    case 4: return 2; //E

    case 5: return 13; //F
    case 6: return 14; //G
    case 7: return 15; //H
    case 8: return 7; //I
    case 9: return 16; //J

    case 10: return 17; //K
    case 11: return 18; //L
    case 12: return 25; //M
    case 13: return 24; //N
    case 14: return 8; //O

    case 15: return 9; //P
    case 16: return 0; //Q
    case 17: return 3; //R
    case 18: return 11; //S
    case 19: return 4; //T

    case 20: return 6; //U
    case 21: return 22; //V
    case 22: return 1; //W
    case 23: return 20; //X
    case 24: return 5; //Y

    case 25: return 19; //Z

    }
}

void letterPress(char letter)
{
    //function this
    if (wordPointer < WORD_LENGTH && guessPointer < MAX_GUESSES)
    {
        guessArray[wordPointer + guessPointer * WORD_LENGTH].value[0] = letter;
        wordPointer++;
    }
}

void newGame()
{
    displayAnswer = false;
    srand(time(NULL));
    rand(); //Without it the randomness is.. strange

    int randomWord = rand() % wordsInWordList;


    for (int i = 0; i < WORD_LENGTH; ++i)
    {
        theWord[i] = wordList[i + randomWord * WORD_LENGTH];
    }


    for (int i = 0; i < WORD_LENGTH * MAX_GUESSES; ++i)
    {
        guessArray[i].backgroundColour = olc_WHITE;
        guessArray[i].value[1] = '\0';
    }

    for (int i = 0; i < 26; ++i)
        keyboard[i].backgroundColour = olc_PixelRGB(170, 170, 170);
    
    wordPointer = 0;
    guessPointer = 0;
}

bool OnUserCreate()
{

    //Get random word
    FILE* fp;
    fp = fopen("wordList.txt", "r");

    char buffer[WORD_LENGTH + 2];

    while (fgets(buffer, WORD_LENGTH + 2, fp))
    {
        wordsInWordList++;
    }


    wordList = (char*)malloc(sizeof(char) * WORD_LENGTH * wordsInWordList);

    fseek(fp, 0, SEEK_SET);

    for (int i = 0; i < wordsInWordList * WORD_LENGTH; i += WORD_LENGTH)
    {
        fgets(buffer, WORD_LENGTH + 2, fp);
        for (int j = 0; j < WORD_LENGTH; ++j)
        {
            wordList[i + j] = buffer[j];
        }
    }
    fclose(fp);

    
    keyboard[0].value[0] = 'Q';
    keyboard[1].value[0] = 'W';
    keyboard[2].value[0] = 'E';
    keyboard[3].value[0] = 'R';
    keyboard[4].value[0] = 'T';
    keyboard[5].value[0] = 'Y';
    keyboard[6].value[0] = 'U';
    keyboard[7].value[0] = 'I';
    keyboard[8].value[0] = 'O';
    keyboard[9].value[0] = 'P';

    keyboard[10].value[0] = 'A';
    keyboard[11].value[0] = 'S';
    keyboard[12].value[0] = 'D';
    keyboard[13].value[0] = 'F';
    keyboard[14].value[0] = 'G';
    keyboard[15].value[0] = 'H';
    keyboard[16].value[0] = 'J';
    keyboard[17].value[0] = 'K';
    keyboard[18].value[0] = 'L';

    keyboard[19].value[0] = 'Z';
    keyboard[20].value[0] = 'X';
    keyboard[21].value[0] = 'C';
    keyboard[22].value[0] = 'V';
    keyboard[23].value[0] = 'B';
    keyboard[24].value[0] = 'N';
    keyboard[25].value[0] = 'M';

    newGame();

    return true;
}


int binarySearch(char wordlist[], int l, int r, char x[])
{
    while (l <= r) {
        int m = l + (r - l) / 2;


        for (int i = 0; i < WORD_LENGTH; ++i)
        {
            if (wordlist[WORD_LENGTH * m + i] < x[i])
            {
                l = m + 1;
                goto end;
            }
            else if (wordlist[WORD_LENGTH * m + i] > x[i])
            {
                r = m - 1;
                goto end;
            }
        }

        //They are equal
        return m;
    end:
        ;
    }

    // if we reach here, then element was
    // not present
    return -1;
}

bool OnUserUpdate(float fElapsedTime)
{

    if (PGE_GetKey(olc_ENTER).bPressed)
    {
        if (wordPointer == WORD_LENGTH)
        {
            //Submit word

            //Is it a valid word? (binary search wordlist)

            char input[WORD_LENGTH];
            for (int i = 0; i < WORD_LENGTH; ++i)
            {
                input[i] = guessArray[guessPointer * WORD_LENGTH + i].value[0] + 32;
            }

            if (binarySearch(wordList, 0, wordsInWordList, input) == -1) return true;






            bool flag[5] = { false, false, false, false, false };

            for (int i = 0; i < WORD_LENGTH; ++i)
            {
                //Grey part
                guessArray[guessPointer * WORD_LENGTH + i].backgroundColour = olc_GREY;
                keyboard[normalToQwerty(guessArray[guessPointer * WORD_LENGTH + i].value[0] - 65)].backgroundColour = olc_VERY_DARK_GREY;

            }

            for (int i = 0; i < WORD_LENGTH; ++i)
            {
                //Yellow part
                for (int j = 0; j < WORD_LENGTH; ++j)
                {
                    if (guessArray[guessPointer * WORD_LENGTH + i].value[0] + 32 == theWord[j] && !flag[j])
                    {
                        guessArray[guessPointer * WORD_LENGTH + i].backgroundColour = olc_YELLOW;
                        keyboard[normalToQwerty(guessArray[guessPointer * WORD_LENGTH + i].value[0] - 65)].backgroundColour = olc_YELLOW;
                        flag[j] = true;
                        break;
                    }
                }
            }

            for (int i = 0; i < WORD_LENGTH; ++i)
            {
                //Green stuff
                if (guessArray[guessPointer * WORD_LENGTH + i].value[0] + 32 == theWord[i])
                {
                    guessArray[guessPointer * WORD_LENGTH + i].backgroundColour = olc_GREEN;
                    keyboard[normalToQwerty(theWord[i] - 97)].backgroundColour = olc_GREEN;
                }
            }

            wordPointer = 0;
            guessPointer++;
        }

        if (guessPointer == MAX_GUESSES)
        {
            displayAnswer = true;
            for (int i = 0; i < WORD_LENGTH; ++i)
            {
                theWord[i] -= 32;
            }
        }
    }
    if (PGE_GetKey(olc_BACK).bPressed)
    {
        if (wordPointer > 0)
        {
            //Backspace stuff (if it exists which it might not actually)
            wordPointer--;
        }
    }

    if (PGE_GetKey(olc_A).bPressed) letterPress('A');
    if (PGE_GetKey(olc_B).bPressed) letterPress('B');
    if (PGE_GetKey(olc_C).bPressed) letterPress('C');
    if (PGE_GetKey(olc_D).bPressed) letterPress('D');
    if (PGE_GetKey(olc_E).bPressed) letterPress('E');
    if (PGE_GetKey(olc_F).bPressed) letterPress('F');
    if (PGE_GetKey(olc_G).bPressed) letterPress('G');
    if (PGE_GetKey(olc_H).bPressed) letterPress('H');
    if (PGE_GetKey(olc_I).bPressed) letterPress('I');
    if (PGE_GetKey(olc_J).bPressed) letterPress('J');
    if (PGE_GetKey(olc_K).bPressed) letterPress('K');
    if (PGE_GetKey(olc_L).bPressed) letterPress('L');
    if (PGE_GetKey(olc_M).bPressed) letterPress('M');
    if (PGE_GetKey(olc_N).bPressed) letterPress('N');
    if (PGE_GetKey(olc_O).bPressed) letterPress('O');
    if (PGE_GetKey(olc_P).bPressed) letterPress('P');
    if (PGE_GetKey(olc_Q).bPressed) letterPress('Q');
    if (PGE_GetKey(olc_R).bPressed) letterPress('R');
    if (PGE_GetKey(olc_S).bPressed) letterPress('S');
    if (PGE_GetKey(olc_T).bPressed) letterPress('T');
    if (PGE_GetKey(olc_U).bPressed) letterPress('U');
    if (PGE_GetKey(olc_V).bPressed) letterPress('V');
    if (PGE_GetKey(olc_W).bPressed) letterPress('W');
    if (PGE_GetKey(olc_X).bPressed) letterPress('X');
    if (PGE_GetKey(olc_Y).bPressed) letterPress('Y');
    if (PGE_GetKey(olc_Z).bPressed) letterPress('Z');


    if (PGE_GetMouse(0).bPressed && PGE_GetMouseX() < 30 && PGE_GetMouseY() < 30)
    {
        newGame();
    }


    PGE_Clear(olc_WHITE);

    PGE_FillRect(0, 0, 30, 30, olc_YELLOW);

    drawKeyboard(50, 230, 2);
    drawGuessingGrid(75, 0, 3);


    return !PGE_GetKey(olc_ESCAPE).bPressed;
}

bool OnUserDestroy()
{
    free(wordList);
    return true;
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    PGE_SetAppName("Wordle");
    if (PGE_Construct(320, 320, 2, 2, false, false))
        PGE_Start(&OnUserCreate, &OnUserUpdate, &OnUserDestroy);

    return 0;
}


