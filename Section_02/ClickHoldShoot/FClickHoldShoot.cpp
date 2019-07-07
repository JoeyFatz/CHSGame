#pragma once
#include <iostream>
#include <ctime>
#include "FClickHoldShoot.h"
#include <map>

// To make the syntax Unreal Engine friendly
#define TMap std::map
using int32 = int; 

// Ran at run-time
FClickHoldShoot::FClickHoldShoot() { Reset(); } // Default constructor

// Get-ers:
int32 FClickHoldShoot::GetCurrentTry() const { return MyCurrentTry; }
int32 FClickHoldShoot::GetHiddenWordLength() const { return MyHiddenWord.length(); }
bool FClickHoldShoot::IsGameWon() const { return bGameIsWon; }

// Matches word length to guess tries for difficulty level
int32 FClickHoldShoot::GetMaxTries() const 
{
	TMap<int32, int32> WordLengthToMaxTries{ {3,5}, {4,8}, {5,10}, {6,15}, {7,20} };
	return WordLengthToMaxTries[MyHiddenWord.length()];
}

FString FClickHoldShoot::GetWordList() const // WIP
{
	srand(time(0));
	const FString WordList[4] = { "ant" , "taco" , "plant" , "planet" };
	FString word = WordList[rand() % 4];
	return word;
}

char FClickHoldShoot::GetPlayerHint() const // Prints the first character from the hidden word
{
	char hint = MyHint;
	return hint;
}

// Methods:
void FClickHoldShoot::Reset()
{
	const FString HIDDEN_WORD = GetWordList(); // This MUST be an isogram
	MyHiddenWord = HIDDEN_WORD;
	MyCurrentTry = GetMaxTries();
	MyHint = MyHiddenWord[0];
	bGameIsWon = false;
	return;
}

EGuessStatus FClickHoldShoot::CheckGuessValidity(FString Guess) const // This function is an implicit dependency
{
	if (Guess == "quit") // If player types quit
	{
		return EGuessStatus::Quit_Game;
	}
	else if (Guess == "hint") // If player types hint
	{
		 return EGuessStatus::Display_Hint; 
	}
	else if (!IsIsogram(Guess)) // If guess is not an isogram
	{
		return EGuessStatus::Not_Isogram; 
	} 
	else if (!IsLowercase(Guess)) // If guess isn't all lowercase 
	{
		return EGuessStatus::Not_Lowercase;
	}
	else if (Guess.length() != GetHiddenWordLength()) // If guess is the wrong length
	{
		return EGuessStatus::Wrong_Length;
	}
	else
	{
		return EGuessStatus::OK; 
	}
}

// Receives a VALID guess, increments turn, and returns count
FClickHoldCount FClickHoldShoot::SubmitValidGuess(FString Guess)
{
	FClickHoldCount ClickHoldCount;
	int32 WordLength = MyHiddenWord.length(); // Assuming same length as guess

	// Loop through all letters in the hidden word
	for (int32 MHWChar = 0; MHWChar < WordLength; MHWChar++)
	{
		// Compare letters against the guess
		for (int32 GChar = 0; GChar < WordLength; GChar++)
		{
			if (Guess[GChar] == MyHiddenWord[MHWChar]) // If they match then
			{
				if (MHWChar == GChar) // If they're in the same place
				{
					ClickHoldCount.Holds++;
				}
				else // If they're NOT in the same place
				{
					ClickHoldCount.Clicks++;
				}
			}
		}
	}
	if (ClickHoldCount.Holds == WordLength)
	{
		bGameIsWon = true;
	}
	else
	{
		bGameIsWon = false;
		MyCurrentTry--;
	}
	return ClickHoldCount;
}

bool FClickHoldShoot::IsIsogram(FString Word) const
{
	// Treat 0 and 1 letter words as Isograms
	if (Word.length() <= 1) { return true; }

	TMap<char, bool> LetterSeen;
	for (auto Letter : Word) // for all letters of the word
	{
		Letter = tolower(Letter); // handle mixed case
		if (LetterSeen[Letter]) { return false;	} // we do NOT  have an isogram
		else { LetterSeen[Letter] = true; }	// add the letter to the map as seen
	}
	return true; 
}

bool FClickHoldShoot::IsLowercase(FString Word) const
{
	for (auto Letter : Word) // For, all letters of the word...
	{
		if (!islower(Letter))
		{
			return false;
		}
	}
	return true;
}
