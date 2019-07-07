/* 
This is the console executable, that makes use of the ClickHoldShoot Class.
This acts as the view in a MVC pattern and is responsible for all 
user interaction. For game logic see the FClickHoldShoot class.
*/

#pragma once
#include <iostream>	
#include <string>
#include "FClickHoldShoot.h"

// To make the syntax Unreal Engine friendly
using FText = std::string;
using int32 = int;

// Function prototypes as outside a class
void PlayIntro();
void PlayGame();
void PrintGameSummary();
FText GetValidGuess();
bool AskToPlayAgain();

FClickHoldShoot CHSGame;  // Instatiate a new game, which we re-use across plays (These allow me to pull get-ers from my cpp file)

// The entry point for the application
int main()
{
	bool bPlayAgain = false;

	do
	{
		PlayIntro();
		PlayGame();
		bPlayAgain = AskToPlayAgain();

	} while (bPlayAgain);
	
	// TODO Add an EndGame function
	std::cout << std::endl;
	std::cout << "Game Over! Thank you for playing!" << std::endl;
	std::cout << std::endl;

	system("pause"); // Exit the application
	//return 0;
}

void PlayIntro()
{
	std::cout << " 		                            ____     _     __     _    ____                         " << std::endl;
	std::cout << " 	                                   |####`-- |#|---|##|---|#|--'##|#|                        " << std::endl;
	std::cout << "	                                   |____,-- |#|---|##|---|#|--.__|_|                        " << std::endl;
	std::cout << "  _|#)_____________________________________,--'EEEEEEEEEEEEEE'_=-.                             " << std::endl;
	std::cout << " ((_____((_________________________, --------[JW](___(____(____(_==)             _________         " << std::endl;
	std::cout << "	                            .--|##,----o  o  o  o  o  o  o___|/`---, -,-'=========`=+==.   " << std::endl;
	std::cout << "	                            |##|_Y__,__.-._,__,   __,-.___/ J     .----.#############|##|   " << std::endl;
	std::cout << "	                            |##|                `-.|#|##|#|`=== l##     _ ############|##|   " << std::endl;
	std::cout << "	                            ====== = -= == l       |_|__|_|       ##`-'__,=======.###|##|   " << std::endl;
	std::cout << "	                                                                   __,'          '======'   " << std::endl;


	std::cout << "\t\tWelcome to \"Click and Hold\" sniper!\n" << std::endl;
}

// Plays a single game to completion
void PlayGame()
{	
	CHSGame.Reset();
	std::cout << "    Zero-in on your target by guessing the " << CHSGame.GetHiddenWordLength() << " letter isogram." << std::endl;
	std::cout << "\n      (Type [hint] for help or [quit] to leave the game)" << std::endl;

	int32 MaxTries = CHSGame.GetMaxTries();

	/* This loop asks for guesses while the game
	   is NOT won and there are still tries remaining */
	while (!CHSGame.IsGameWon() && CHSGame.GetCurrentTry() >= 1)
	{
		// Main gameplay loop
		FText Guess = GetValidGuess();

		// Submit valid guess to game and receive counts
		FClickHoldCount ClickHoldCount = CHSGame.SubmitValidGuess(Guess);

		if (CHSGame.GetCurrentTry() == 0) // Decreases for every incorrect guess
		{
			std::cout << "\nYour target has escaped. [MISSION FAILED]" << std::endl; // GameOver condition
		}
		else if (CHSGame.GetCurrentTry() > 0 && !CHSGame.IsGameWon())
		{
			// Print number of "Clicks" and "Holds" 
			std::cout << "Target missed! Adjust your sights: " << std::endl;
			std::cout << "Hold(s) = " << ClickHoldCount.Holds; // "Hold steady on _ "
			std::cout << " Click(s) = " << ClickHoldCount.Clicks << std::endl; // "Adjust _ Clicks"
		}
	}
	
	PrintGameSummary(); // TODO Add MissionFailed function to game summary
	return;
}

// Continue loop until the player gives a valid guess
FText GetValidGuess()
{
	FText Guess = "";
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	do
	{
		// Get a guess from the player
		int32 CurrentTry = CHSGame.GetCurrentTry();
		std::cout << std::endl;
		std::cout << "AMMO: " << CurrentTry << "/" << CHSGame.GetMaxTries() << " remaining" << std::endl; 
		std::cout << "\nEnter your guess: ";
		std::getline(std::cin, Guess); // TODO Add function to display the first letter of the hidden word.
		std::cout << std::endl;

		// Check for valid guess
		Status = CHSGame.CheckGuessValidity(Guess);
		switch (Status)
		{
		case EGuessStatus::Quit_Game:
			return 0; // TODO Create boolean that exits into PlayGame function. In play game create corresponding if statement that takes boolean and exits to game summary.
			break;
		case EGuessStatus::Display_Hint:
			std::cout << "The first letter of the word is: " << CHSGame.GetPlayerHint() << std::endl; // TODO Implement a display hint function
			break;
		case EGuessStatus::Wrong_Length:
			std::cout << "Please enter a " << CHSGame.GetHiddenWordLength() << " letter word.";
			break;
		case EGuessStatus::Not_Isogram:
			std::cout << "Please enter a word without repeating letters.";
			break;
		case EGuessStatus::Not_Lowercase:
			std::cout << "Please only use lowercase letters.";
			break;
		default:
			// Assume the guess is valid
			break;
		}
		std::cout << std::endl;
	} while (Status != EGuessStatus::OK); // Keep looping until we get no errors
	return Guess;
}

bool AskToPlayAgain()
{
	FText Response = "";
	std::cout << "Play Again? (y/n) ";
	std::getline(std::cin, Response);
	return (Response[0] == 'y') || (Response[0] == 'Y');
}

void PrintGameSummary()
{
	if (CHSGame.IsGameWon() == true)
	{
		std::cout << "Target Down! [MISSION ACCOMPLISHED]" << std::endl;
	}
	// TODO Move mission failure condition into this method.
	return;
}
