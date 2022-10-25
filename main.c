/* IT161 Project -: Group 62 -: Plagiarism Checker
Group Members are -
Aditya Dilip Kakad (202052302)
Amal Shakya (202052303)
Aman Varshney (202052304)
Amartya (202052305)
Anubhav Kushwaha (202052306)
Archit Agrawal (202052307)
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
const int MAX_CHARS = 7000;
const int MAX_WORDS = 2000;
const int MAX_WORD_LENGTH = 40;
const int MAX_FILE_NAME = 20;
const char *END_STR = "--WORDS--END--";
const char usual_chars[] = "?/.';[{]}=+-_`~!@#$%^&*()";
const char helping_verbs[][10] = {"is", "am", "are", "was", "were","do", "does", "did", "has", "have", "had", "be ", " being ", " been "};
const char modal_verbs[][10] = {"will", "shall", "would", "should", "can", "could ", " may ", " might ", " must "};
const char conjunctions[][10] = {"for", "or", "nor", "yet", "so", "and", "but", "if"};
const char preposition[][10] = {"as", "on", "of", "to", "by", "at", "up", "for", " with ", " down ", " within ", " without "};
const char adverb[][10] = {"now", "very", "never", "soon", "then", "much", "well "};
const char article[][10] = {"a", "an", "the"};
int is_one_word_no_or_char = 1;
int is_helping_verbs = 1;
int is_modal_verbs = 1;
int is_conjunctions = 1;
int is_preposition = 1;
int is_adverb = 1;
int is_article = 1;
int contains_till_index(char word[], int lastIndex, char wordlist[][MAX_WORD_LENGTH]);
void readFile(FILE *file, char str[]);
void remove_usual_chars_and_lowercase(char usual_char[], char str[]);

void remove_usual_words(char wordlist[][MAX_WORD_LENGTH]);
void string_to_words(char str[], char wordlist[][MAX_WORD_LENGTH]);
double similarity(char wordlist1[][MAX_WORD_LENGTH], char wordlist2[][MAX_WORD_LENGTH]);
void popout(int index, char wordlist[][MAX_WORD_LENGTH]);
int contains(char word[], char wordlist[][10]);
void similarity_meter(char file1name[], char file2name[], double value);
void clear_screen();
int homeScreen();
int getUserOptionHomeScreen(int start);
void settingsScreen();
int getUserOptionSettingsScreen(int start);
void twoFileScreen();
void multipleFileScreen();
void plagiarismCheck(char file1_name[], char file2_name[], double MinPercentage);
int main()
{
    int option = homeScreen(); // function call, homeScreen() returns an integer
    switch (option){
    case 1:
    {
        clear_screen();
        twoFileScreen(); // function call, takes to twoFileScreen
        break;
    }
    case 2:
        clear_screen();
        multipleFileScreen(); // function call, takes to multipleFileScreen
        break;
    case 3:
        clear_screen();
        settingsScreen(); // function call, takes to settingsScreen

        break;
    default:
        exit(0);
    }
    main();
    return 0;
}
void multipleFileScreen()
{
    char files[100][MAX_FILE_NAME];
    printf("We Only Support 100 files at a time as of now\n");
    printf("** Enter 0 when you done entering all file Names **\n\n");
    int noOfFiles = 0;
    for (noOfFiles = 0; noOfFiles < 100;)
    {
        char tempFileName[20];
        printf("File %d : ", noOfFiles + 1);
        gets(tempFileName); // for taking file name
        if (strcmp(tempFileName, "0") == 0)
        { // to stop taking more file names
            break;
        }
        if (strstr(tempFileName, ".txt") == NULL)
        { // to check if file contains.txt
                strcat(tempFileName, ".txt");
        }
        FILE *tempFile;
        char filePath[100] = "documents\\";
        strcat(filePath, tempFileName);
        tempFile = fopen(filePath, "r"); // to open the file for reading
        if (tempFile == NULL)
        { // If the file is not found
            printf("^^ This file can't be Found\n");
            continue;
        }
        fclose(tempFile); // close file
        strcpy(files[noOfFiles], tempFileName);
        noOfFiles++;
    }
    fflush(stdin); // to remove any buffer
    clear_screen();
    double minPercentage;
printf("Enter Minimum Percentage of similarity in results\nex: 32 or 54.32\n");

scanf("%lf", &minPercentage); // take minimum percentage of Plagiarism from the user
for (int i = 0; i < noOfFiles - 1; ++i) {
        for (int j = i + 1; j < noOfFiles; ++j)
        {
            plagiarismCheck(files[i], files[j], minPercentage); // to check Plagiarism for every pair of files.
        }
}
fflush(stdin); // to remove any buffer
printf("Press Enter for Home Screen\n");
getchar();
}
void twoFileScreen()
{
    char file1_name[MAX_FILE_NAME];
    char file2_name[MAX_FILE_NAME];
    printf("Enter First File Name: ");
    gets(file1_name); // to get file 1 name
    printf("Enter Second File Name: ");
    gets(file2_name);                             // to get file 2 name
    plagiarismCheck(file1_name, file2_name, 0.0); // check Plagiarism function
    printf("\nPress Enter to Exit\n");
    fflush(stdin); // to remove any buffer
    getchar();
    clear_screen();
}
void plagiarismCheck(char file1_name[], char file2_name[], double MinPercentage)
{
    fflush(stdin);
    FILE *file1;
    FILE *file2;
    if (strstr(file1_name, ".txt") == NULL)
    { // checking if file name has .txt in it
            strcat(file1_name, ".txt"); // if file name does not contain .txt, it concatenates.txt to it
    }
    if (strstr(file2_name, ".txt") == NULL)
    {
        strcat(file2_name, ".txt");
    }
    char file1_path[100] = "documents\\"; // file 1 path
    char file2_path[100] = "documents\\"; // file 2 path
    strcat(file1_path, file1_name);       // to concatenate the file path and file name
        strcat(file2_path, file2_name);

    file1 = fopen(file1_path, "r"); // opening file and using it as reading file
        file2 = fopen(file2_path, "r");
    if (file1 == NULL || file2 == NULL)
    {
        printf("First File: %s\nSecond File: %s\n", file1_name, file2_name);
        printf("One or more files are not found.");
        return;
    }
    char file1str[MAX_CHARS]; // the text in the file will be contained in this string 
    char file2str[MAX_CHARS];
    readFile(file1, file1str); // function call, to convert the text in file into string
    readFile(file2, file2str);
    fclose(file1); // closing the files
    fclose(file2);
    remove_usual_chars_and_lowercase(usual_chars, file1str); // function call,to remove usual characters as mentioned in Global Scope and to make every character lower case.
    remove_usual_chars_and_lowercase(usual_chars, file2str);
    char wordlist1[MAX_WORDS][MAX_WORD_LENGTH]; // this will contain a string of words of the file1str 
    char wordlist2[MAX_WORDS][MAX_WORD_LENGTH];
    string_to_words(file1str, wordlist1); // function call, to convert the string to string of words
    string_to_words(file2str, wordlist2);
    remove_usual_words(wordlist1); // function call, to remove usual words from the wordlist
    remove_usual_words(wordlist2);
    double a = similarity(wordlist1, wordlist2); // function call, to check the similarity between the files 
    double value = acos(a);
    if (MinPercentage <= 100 - (value * 100.0) / (M_PI / 2))
    {
        similarity_meter(file1_name, file2_name, value);
    }
    fflush(stdin);
}

void settingsScreen()
{
printf(" Starred options will not be considered while finding similarity between documents.\n\n");
printf(" Toggle options by entering their index\n");
printf(" 1. [%c] Helping Verbs\n", (is_helping_verbs) ? '*' : ' ');
printf(" 2. [%c] Modal Verbs\n", (is_modal_verbs) ? '*' : ' ');
printf(" 3. [%c] Conjunctions\n", (is_conjunctions) ? '*' : ' ');
printf(" 4. [%c] Prepositions\n", (is_preposition) ? '*' : ' ');
printf(" 5. [%c] Adverbs\n", (is_adverb) ? '*' : ' ');
printf(" 6. [%c] Articles\n", (is_article) ? '*' : ' ');
printf(" 7. [%c] One letter word and no's\n", (is_one_word_no_or_char)? '*' : ' ');
printf("\nPress 0 for home screen\n");
fflush(stdin);
int option = getUserOptionSettingsScreen(1);
if (option == 0) {
        return; // for returning to home screen
}
settingsScreen();
}
int homeScreen()
{
    clear_screen(); // function call, to print '- ' in one line and then leaving two blank lines
    printf("Welcome to Plagiarism Checker\n\n");
    printf("Enter 1, 2 or 3 to choose option\n");
    printf(" 1. Check Two Files for Plagiarism\n");
    printf(" 2. Check Multiple Files for Plagiarism\n");
    printf(" 3. Settings\n");
    printf("\nEnter 0 to quit\n");
    return getUserOptionHomeScreen(1); // function call, for the user to inputhis / her choice.
}
int getUserOptionHomeScreen(int start)
{
    char option;
    if (!start)
    { // just in case if the user enters an invalid input
        printf("Sorry, please choose a valid option\n");
    }
    scanf("%c", &option);
    getchar();     // for caught '\n' when user enter a char
    fflush(stdin); // if user enter a string it will take first char and flush all input 
    switch (option)
    {
    case '0':
        return 0;
    case '1':
        return 1;

    case '2':
        return 2;
    case '3':
        return 3;
    default:
        return getUserOptionHomeScreen(0);
    }
}
int getUserOptionSettingsScreen(int start)
{
    char option;
    if (!start)
    { // just in case if the user enters an invalid input
        printf("Sorry, please choose a valid option\n");
    }
    scanf("%c", &option); // option chosen by user.
    getchar();
    fflush(stdin); // if user enter a string it will take first char and flushall input
        // Each category of words are not to be considered in Plagiarism Check by default
        // To toggle between the categories of words, user should input the index of that category of words.
        switch (option)
    {
    case '0':
        return 0;
    case '1':
        if (is_helping_verbs)
        {
            is_helping_verbs = 0;
        }
        else
        {
            is_helping_verbs = 1;
        }
        return 1;
    case '2':
        if (is_modal_verbs)
        {
            is_modal_verbs = 0;
        }
        else
        {
            is_modal_verbs = 1;
        }
        return 2;
    case '3':
        if (is_conjunctions)
        {
            is_conjunctions = 0;
        }
        else
        {
            is_conjunctions = 1;
        }
        return 3;
    case '4':

        if (is_preposition)
        {
            is_preposition = 0;
        }
        else
        {
            is_preposition = 1;
        }
        return 4;
    case '5':
        if (is_adverb)
        {
            is_adverb = 0;
        }
        else
        {
            is_adverb = 1;
        }
        return 5;
    case '6':
        if (is_article)
        {
            is_article = 0;
        }
        else
        {
            is_article = 1;
        }
        return 6;
    case '7':
        if (is_one_word_no_or_char)
        {
            is_one_word_no_or_char = 0;
        }
        else
        {
            is_one_word_no_or_char = 1;
        }
        return 7;
    default:
        return getUserOptionSettingsScreen(0);
    }
}
void similarity_meter(char file1name[], char file2name[], double value)
{
    int blockLength = 27;
    printf("File: %s and File %s\n", file1name, file2name);
    double percentageMatched = 100 - (value * 100.0) / (M_PI / 2);
    printf("Percentage Matched : %.2lf%%\n", percentageMatched);
    for (int i = 0; i < blockLength; ++i)
    {
        if (i < percentageMatched * (blockLength * 1.0 / 100))
        {
            printf("|");
        }
        else
        {
            printf(":");
        }
    }
    printf("\n\n");
}
void readFile(FILE *filepo, char str[])
{ // to convert the file into a string

    char c;
    c = fgetc(filepo);
    int i = 0;
    while (c != EOF && i < MAX_CHARS - 2)
    {
        str[i] = c;
        i += 1;
        c = fgetc(filepo); // fgetc takes the next character of the file every time it is called.
    }
    str[i] = c;
    str[i += 1] = '\0'; // to end the string
}

void remove_usual_chars_and_lowercase(char usual_char[], char str[])
{
    char response[MAX_CHARS];
    int i = 0;
    int r = 0;
    while (str[i] != EOF)
    {
        char c = str[i];
        if (strchr(usual_char, c) == NULL)
        {
            if (c >= 65 && c <= 90)
            {
                c += 32; // make uppercase to lowercase
            }
            response[r] = c;
            r += 1;
        }
        i += 1;
    }
    response[r] = '\0';    // end the string
    strcpy(str, response); // copy response to str
}

double similarity(char wordlist1[][MAX_WORD_LENGTH], char wordlist2[][MAX_WORD_LENGTH])
{
    int matches = 0;
    int wordlist1len = 0;
    int wordlist2len = 0;
    int count = 1;
    for (int i = 0; i < MAX_WORDS && strcmp(wordlist1[i], END_STR) != 0; i++)
    {
        if (contains_till_index(wordlist1[i], i, wordlist1))
        {
            popout(i, wordlist1);
        }
    }
    for (int i = 0; i < MAX_WORDS && strcmp(wordlist2[i], END_STR) != 0; i++)
    {
        if (contains_till_index(wordlist2[i], i, wordlist2))
        {
            popout(i, wordlist2);
        }
    }
    for (int i = 0; i < MAX_WORDS && strcmp(wordlist1[i], END_STR) != 0; ++i)
    {
        wordlist1len++;
        for (int j = 0; j < MAX_WORDS && strcmp(wordlist2[j], END_STR) != 0; ++j)
        {
            if (count == 1)
            {
                wordlist2len++;
            }
            if (!strcmp(wordlist1[i], wordlist2[j]))
            {
                matches++; // to count number of words similar (in order) in both the wordlists
            }
        }
        count = 0;
    }
    if (!matches)
    {
        return 0.0;
    }
    return (matches * 1.0) / (sqrt(wordlist2len * wordlist1len * 1.0)); // calculate cos(theta)
}

void string_to_words(char str[], char wordlist[][MAX_WORD_LENGTH])
{
    int i, j, cnt;
    j = 0;
    cnt = 0;
    int firstChar = 1;
    for (i = 0; i <= (strlen(str)); i++)
    {
        if (str[i] == ' ' || str[i] == ',' || str[i] == '\0')
        {
            wordlist[cnt][j] = '\0';
            cnt++;
            firstChar = 1;
            j = 0;
        }
        else
        {
            wordlist[cnt][j] = str[i];
            firstChar = 0;
            j++;
        }
    }
    strcpy(wordlist[cnt], END_STR);
}
void clear_screen()
{
    // #ifdef WIN32
    // system("cls");
    // #else

    // system("clear");
    // #endif
    for (int i = 0; i < 30; ++i)
    {
        printf("- ");
    }
    printf("\n\n");
}
int contains_till_index(char word[], int lastIndex, char wordlist[][MAX_WORD_LENGTH])
{
    for (int i = 0; i < lastIndex; ++i)
    {
        if (!strcmp(word, wordlist[i]))
        {
            return 1;
        }
    }
    return 0;
}
void remove_usual_words(char wordlist[][MAX_WORD_LENGTH])
{
    for (int i = 0; i < MAX_WORDS && strcmp(wordlist[i], END_STR) != 0; ++i)
    {
        if (strlen(wordlist[i]) == 0)
        {
            popout(i, wordlist);
            i--;
            continue;
        }
        if (is_one_word_no_or_char)
        {
            if (strlen(wordlist[i]) == 1)
            {
                popout(i, wordlist);
                i--;
                continue;
            }
        }
        if (is_helping_verbs)
        {
            if (contains(wordlist[i], helping_verbs))
            {
                popout(i, wordlist);
                i--;
                continue;
            }
        }
        if (is_modal_verbs)
        {
            if (contains(wordlist[i], modal_verbs))
            {
                popout(i, wordlist);
                i--;
                continue;
            }
        }
        if (is_conjunctions)
        {

            if (contains(wordlist[i], conjunctions))
            {
                popout(i, wordlist);
                i--;
                continue;
            }
        }
        if (is_preposition)
        {
            if (contains(wordlist[i], preposition))
            {
                popout(i, wordlist);
                i--;
                continue;
            }
        }
        if (is_adverb)
        {
            if (contains(wordlist[i], adverb))
            {
                popout(i, wordlist);
                i--;
                continue;
            }
        }
        if (is_article)
        {
            if (contains(wordlist[i], article))
            {
                popout(i, wordlist);
                i--;
                continue;
            }
        }
    }
}
int contains(char word[], char wordlist[][10])
{
    int len = strlen(wordlist);
    for (int i = 0; i < len; ++i)
    {
        if (!strcmp(word, wordlist[i]))
        {
            return 1;
        }
    }
    return 0;
}

void popout(int index, char wordlist[][MAX_WORD_LENGTH])
{
    for (int i = index; strcmp(wordlist[i], END_STR) != 0; i++)
    {
        strcpy(wordlist[i], wordlist[i + 1]);
    }
}