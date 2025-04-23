#include <iostream>
#include <string>
#include <cstdlib> // For rand() and srand()
#include <ctime>  // For time()
#include <windows.h> //For system-level functions such as changing console color and clearing the screen

using namespace std;

// Change the console text color based on the input color code
void SetColor(string ColorCode)
{
    string command = "color " + ColorCode;
    system(command.c_str());
}

void ClearScreen()
{
    system("cls");
}

void CenterText(string Text)
{
    int width = 80; // Console width
    int pad = (width - Text.length()) / 2; // Calculate padding to center the text
    cout << string(pad, ' ') << Text << endl;
}

struct stQuizResult {
    int TotalQuestions;
    int CorrectAnswers;
    int WrongAnswers;
};

enum enLevels
{
    Easy = 1,
    Med = 2,
    Hard = 3,
    MixedLevels = 4
};

enum enOperations
{
    Add = 1,
    Sub = 2,
    Mul = 3,
    Div = 4,
    MixedOps = 5
};

int ReadNumber(string Message)
{
    int Number;
    do {
        cout << Message;
        cin >> Number; // Get the user input
    } while (Number <= 0); // Ensure the number is positive
    return Number;
}

int GenerateRandomNumber(int From, int To)
{
    return (rand() % (To - From + 1)) + From; // Generate a random number in the range
}

enLevels ReadPlayerLevel()
{
    int PlayerLevel = ReadNumber("Enter Question Level [1]: Easy, [2]: Med, [3]: Hard, [4]: Mix.  ");
    while (PlayerLevel < 1 || PlayerLevel > 4)  // Validate the input to allow Mix as well
    {
        PlayerLevel = ReadNumber("Enter Question Level [1]: Easy, [2]: Med, [3]: Hard, [4]: Mix.  ");
    }
    return static_cast<enLevels>(PlayerLevel); // Convert the input to an enum
}

enOperations ReadOperationType()
{
    int PlayerOps = ReadNumber("Enter Question Type [1]: Add, [2]: Sub, [3]: Mul, [4]: Div, [5]: Mix.  ");
    while (PlayerOps < 1 || PlayerOps > 5)  // Validate the input to allow Mix as well
    {
        PlayerOps = ReadNumber("Enter Question Type [1]: Add, [2]: Sub, [3]: Mul, [4]: Div, [5]: Mix.  ");
    }
    return static_cast<enOperations>(PlayerOps); // Convert the input to an enum
}

enLevels GetRandomLevel()
{
    // Generate a random level between Easy and Hard (1 to 3)
    return (enLevels)GenerateRandomNumber(enLevels::Easy, enLevels::Hard);
}

enOperations GetRandomOperation()
{
    // Generate a random operation between Add and Div (1 to 4)
    return (enOperations)GenerateRandomNumber(enOperations::Add, enOperations::Div);
}

// Function that will ask the question based on the level and operation
bool AskQuestion(enLevels Level, enOperations Operation)
{
    int Num1, Num2;
    float Result;
    string OperationSymbol;

    // Generate numbers based on the selected level
    switch (Level)
    {
    case enLevels::Easy:
        Num1 = GenerateRandomNumber(1, 10);
        Num2 = GenerateRandomNumber(1, 10);
        break;
    case enLevels::Med:
        Num1 = GenerateRandomNumber(10, 50);
        Num2 = GenerateRandomNumber(10, 50);
        break;
    case enLevels::Hard:
        Num1 = GenerateRandomNumber(50, 100);
        Num2 = GenerateRandomNumber(50, 100);
        break;
    case enLevels::MixedLevels:
        Level = GetRandomLevel();
        return AskQuestion(Level, Operation);  // Recursive call with random level
    default:
        Num1 = Num2 = 0;
        break;
    }

    // Determine the operation type
    switch (Operation)
    {
    case enOperations::Add:
        Result = Num1 + Num2;
        OperationSymbol = "+";
        break;
    case enOperations::Sub:
        Result = Num1 - Num2;
        OperationSymbol = "-";
        break;
    case enOperations::Mul:
        Result = Num1 * Num2;
        OperationSymbol = "*";
        break;
    case enOperations::Div:
        if (Num2 == 0) Num2 = 1;  // Prevent division by zero
        Result = Num1 / Num2;
        OperationSymbol = "/";
        break;
    case enOperations::MixedOps:
        Operation = GetRandomOperation();
        return AskQuestion(Level, Operation);  // Recursive call with random operation
    default:
        Result = 0;
        OperationSymbol = "?";
        break;
    }

    // Ask the player the question
    cout << "\tWhat is: " << Num1 << " " << OperationSymbol << " " << Num2 << "?  ";
    float PlayerAnswer;
    cin >> PlayerAnswer;

    // Check if the player's answer is correct
    bool isCorrect = (PlayerAnswer == Result);

    // Display result for the question
    if (isCorrect)
    {
        SetColor("0A");  // Green color for correct answer
        cout << "\tCorrect!" << endl;
    }
    else
    {
        SetColor("0C");  // Red color for incorrect answer
        cout << "\tWrong! The correct answer is: " << Result << endl;
    }

    // Return the correctness of the answer
    return isCorrect;
}


bool AskToPlayAgain()
{
    char Answer;
    cout << "\nDo you want to play again? [Y/N]: ";
    cin >> Answer;
    Answer = toupper(Answer); // Ensure that the answer is in uppercase
    return (Answer == 'Y'); // Return true if player chooses to play again
}

void NewGame()
{
    // Read number of questions
    int NumQuestions = ReadNumber("Enter the number of questions: ");

    // Read player level and operation type
    enLevels Level = ReadPlayerLevel();
    enOperations Operation = ReadOperationType();

    cout << endl;

    // Start the quiz
    int CorrectAnswers = 0;
    // Ask questions based on the selected number of questions
    for (int i = 1; i <= NumQuestions; i++)
    {
        cout << "Question " << i << "/" << NumQuestions << endl;
        bool Correct = AskQuestion(Level, Operation);
        cout << endl;
        if (Correct)
        {
            CorrectAnswers++; // Increment if the answer is correct
        }
    }

    // Show the final results summary
    CenterText("");
    CenterText("========================================");
    CenterText("             QUIZ SUMMARY               ");
    CenterText("========================================");
    CenterText("");

    // Display performance summary
    CenterText("Total Questions   : " + to_string(NumQuestions));
    CenterText("Correct Answers   : " + to_string(CorrectAnswers));
    CenterText("Wrong Answers     : " + to_string(NumQuestions - CorrectAnswers));

    // Calculate the score percentage
    float ScorePercentage = (float)CorrectAnswers / NumQuestions * 100;
    CenterText("Score Percentage  : " + to_string(ScorePercentage) + "%");

    // Determine pass/fail and display message with color
    if (ScorePercentage >= 50)
    {
        SetColor("0A"); // Green color for Pass
        CenterText(">>> PASS <<<");
    }
    else
    {
        SetColor("0C"); // Red color for Fail
        CenterText(">>> FAIL <<<");
    }
    cout << endl;
}

// Start the game and handle multiple sessions
void StartTheGame()
{
    do
    {
        ClearScreen(); // Clear the screen before starting a new game
        SetColor("1F"); // Set initial color
        CenterText("==============================");
        CenterText("Let's Start The Math Quiz");
        CenterText(" ==============================\n");
        NewGame(); // Start a new game
    } while (AskToPlayAgain()); // Repeat if the player chooses to play again
}

int main() {
    srand((unsigned)time(NULL)); // Initialize random seed based on current time

    StartTheGame(); // Start the game

    return 0;
}
