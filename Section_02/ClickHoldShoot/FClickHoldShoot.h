/* 
The game logic (no view code or direct user interaction)
The game is a simple guess the word game based on Mastermind
*/

#pragma once
#include <string>

// To make the syntax Unreal Engine friendly
using FString = std::string;
using int32 = int;

struct FClickHoldCount
{
	int32 Clicks = 0;
	int32 Holds = 0;
};

enum EGuessStatus
{
	Invalid_Status,
	OK,
	Not_Isogram,
	Not_Lowercase,
	Wrong_Length,
	Quit_Game,
	Display_Hint
};

// Ran at compile-time
class FClickHoldShoot 
{
public:	
	FClickHoldShoot(); // Constructor

	FString GetWordList() const;
	int32 GetMaxTries() const;
	int32 GetCurrentTry() const;
	int32 GetHiddenWordLength() const;
	char GetPlayerHint() const;
	bool IsGameWon() const;

	EGuessStatus CheckGuessValidity(FString) const; 

	void Reset(); 
	FClickHoldCount SubmitValidGuess(FString Guess);



// ^^ Please try to ignore this and focus on the interface above ^^
private:
	// See constructor for intialization
	int32 MyCurrentTry;
	FString MyHiddenWord;
	char MyHint;
	
	bool bGameIsWon;
	bool IsIsogram(FString) const;
	bool IsLowercase(FString) const;
};
