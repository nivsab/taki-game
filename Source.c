// Niv Saban - 315240895 //


#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#define MAX_SIZE_NAME 21
#define MAX_NAME_IN_CARDS 6
#define SIZE_IND_ARR 15
#define START_CARDS 8
#define MAX_CARDS_PER_PLAYER 50
#define TAKE_ONE_CARD 0


typedef struct CARDS
{
	char color; // 1 - Y, 2 - G, 3 - R, 4 - B
	int cards;
}CARDS;

typedef struct PLAYER
{
	char name[MAX_SIZE_NAME];
	int allCards;
	CARDS* arrCards;
	int size_of_arrCards;
	int roundNumber;

} PLAYER;

enum specielCards
{
	taki = 10, stop, changeDirection, plus, changeColor
};

//All functions
void printCard(CARDS* card);
void matchColorToCard(CARDS* upperCard);
void addCardToPlayer(PLAYER* pArrPlayers, int* player, int* indexArr);
bool isCardPlayable(int chosenCard, PLAYER* pArrPlayers, CARDS* upperCard);
int getPlayerCardChoice(PLAYER* pArrPlayers, int player, CARDS* upperCard);
void swapOrderOfPlayers(PLAYER* pArrPlayers, int* i);
void changeColorCard(CARDS* upperCard);
void printGameStatistics(int indexArr[]);
void handlePlayerTurnEnd(PLAYER* pArrPlayers, int* player, int chosenCard, CARDS* upperCard, int* numOfPlayers, int* indexArr, bool* continueGame);
void dealInitialCards(PLAYER* pArrPlayers, int numOfPlayers, int* indexArr);


void removeCardFromPlayer(PLAYER* pArrPlayers, int* player, int indexCard, CARDS* upperCard, int* numOfPlayers, int* indexArr);
void removeOneCard(PLAYER* pArrPlayers, int* player, int card, CARDS* upperCard);
void removeRegularCard(PLAYER* pArrPlayers, int* player, int indexCard, CARDS* upperCard);
void removeSpecialCard(PLAYER* pArrPlayers, int* player, int indexCard, CARDS* upperCard, int* numOfPlayers, int* indexArr);
void removeTakiCard(PLAYER* pArrPlayers, int* player, int indexCard, CARDS* upperCard);
void removeStopCard(PLAYER* pArrPlayers, int* player, int indexCard, CARDS* upperCard, int* numOfPlayers, int* indexArr);
void removeChangeDirectionCard(PLAYER* pArrPlayers, int* player, int indexCard, CARDS* upperCard, int* numOfPlayers);
void removePlusCard(PLAYER* pArrPlayers, int* player, int indexCard, CARDS* upperCard, int* indexArr);
void removeColorCard(PLAYER* pArrPlayers, int* player, int indexCard, CARDS* upperCard);




int main()
{
	CARDS upperCard;
	PLAYER* pArrPlayers = NULL; //array of players
	int indexCard, chosenCard, player = 0, numOfPlayers, maxIndex, winner, Count_Card_2 = 0;
	int indexArr[SIZE_IND_ARR] = { 0 }, maxArrCard[SIZE_IND_ARR] = { 0 }, temp[SIZE_IND_ARR];
	bool continueGame = true, rightDirection = true;
	srand(time(NULL));// seeds the random number generator
	printf("************ Welcome to TAKI game !!! ************\n");
	do
	{
		printf("Please enter the number of players (1-4):\n");
		scanf("%d", &numOfPlayers);
		if (numOfPlayers < 1 || numOfPlayers > 4)
			printf("Invalid number of players. Please try again\n");
	} while (numOfPlayers < 1 || numOfPlayers > 4);


	pArrPlayers = (PLAYER*)malloc(numOfPlayers * sizeof(PLAYER));
	if (pArrPlayers == NULL) // check if pointer was saved or null
	{
		printf("Error! Memory allocation failed\n");
		exit(1);
	}

	for (int i = 0; i < numOfPlayers; i++) //Insert the name of the player in the array
	{
		printf("Please enter the name of player #%d\n", i + 1);
		scanf("%s", pArrPlayers[i].name);
	}

	dealInitialCards(pArrPlayers, numOfPlayers, indexArr); // Deals initial cards to all players

	// put up the first card
	do {
		upperCard.cards = rand() % 9 + 1;
	} while (upperCard.cards == 2); // Can't start with the card 2

	matchColorToCard(&upperCard);
	printCard(&upperCard);

	while (continueGame)
	{
		for (player; player < numOfPlayers; player++) // uppercard, i , continugame, numof player
		{
			bool correctCard = false;
			printf("\n%s's turn:\n", pArrPlayers[player].name);

			for (indexCard = 0; indexCard < pArrPlayers[player].allCards; indexCard++) // print all the cards of the current player
				printCard(&pArrPlayers[player].arrCards[indexCard]);

			printf("The current top card is:\n");
			printCard(&upperCard);

			if (upperCard.cards == 2) // now, the player can put just card "2", or take cards
			{
				Count_Card_2 += 2;
				do
				{
					printf("Reminder! The top card is a 2. Play a 2 of your own, or press 0 to receive a penalty accordingly\n ");
					chosenCard = getPlayerCardChoice(pArrPlayers, player, &upperCard);

					if (chosenCard > 0 && pArrPlayers[player].arrCards[chosenCard - 1].cards == 2)
						removeCardFromPlayer(pArrPlayers, &player, chosenCard, &upperCard, &numOfPlayers, &indexArr);

					else if (chosenCard == 0)
					{
						while (Count_Card_2 > 0)
						{
							addCardToPlayer(pArrPlayers, &player, &indexArr);
							Count_Card_2--;
						}
						Count_Card_2 = 0;
					}
					else
						printf("Invalid card. try again\n");
				} while (pArrPlayers[player].arrCards[chosenCard - 1].cards != 2 && chosenCard != 0);
			}
			else
			{
				chosenCard = getPlayerCardChoice(pArrPlayers, player, &upperCard);
				if (chosenCard == TAKE_ONE_CARD)
					addCardToPlayer(pArrPlayers, &player, &indexArr);
				else if (pArrPlayers[player].allCards != 0)
					removeCardFromPlayer(pArrPlayers, &player, chosenCard, &upperCard, &numOfPlayers, &indexArr);
				else
					handlePlayerTurnEnd(pArrPlayers, player, chosenCard, &upperCard, numOfPlayers, indexArr, &continueGame);
			}
			if (pArrPlayers[player].allCards == 0)
			{
				continueGame = false;
				winner = player;
				player = numOfPlayers;
			}
		}
		player = 0;
	}
	printf("The winner is ... %s! Congratulations!\n\n", pArrPlayers[winner].name);
	printGameStatistics(indexArr);

	//free(pArrPlayers->arrCards);
	for (int i = 0; i < numOfPlayers; i++)
	{
		free(pArrPlayers[i].arrCards);
		pArrPlayers[i].arrCards = NULL;
	}
	free(pArrPlayers);
	pArrPlayers = NULL;

	return 0;
}


// The function recieved card and match a color for it.
void matchColorToCard(CARDS* card)
{
	int temp = 0;

	if (card->cards != 14)
		temp = rand() % 4 + 1;

	switch (temp)
	{
	case 1:
		card->color = 'Y';
		break;
	case 2:
		card->color = 'R';
		break;
	case 3:
		card->color = 'B';
		break;
	case 4:
		card->color = 'G';
		break;
	default:
		card->color = ' ';
		break;
	}
}

// The function recieved card and print it.
void printCard(CARDS* card)
{
	printf("*********\n");
	printf("*       *\n");

	if (card->cards <= 9)
		printf("*   %d   *\n", card->cards);
	else if (card->cards == taki)
		printf("*  TAKI *\n", card->cards);
	else if (card->cards == stop)
		printf("*  STOP *\n", card->cards);
	else if (card->cards == changeDirection)
		printf("*  <->  *\n");
	else if (card->cards == plus)
		printf("*   +   *\n");
	else if (card->cards == changeColor)
		printf("* COLOR *\n", card->cards);

	printf("*   %c   *\n", card->color);
	printf("*       *\n");
	printf("*********\n");
}

// The function check if the card that the player chose is correct or not
bool isCardPlayable(int chosenCard, PLAYER* pArrPlayers, CARDS* upperCard)
{
	if (pArrPlayers->arrCards[chosenCard - 1].cards == 14 || chosenCard == 0)
		return true;
	if (chosenCard > pArrPlayers->allCards || chosenCard < 0)
		return false;
	if (pArrPlayers->arrCards[chosenCard - 1].cards != upperCard->cards && pArrPlayers->arrCards[chosenCard - 1].color != upperCard->color)
		return false;
	return true;
}

// Gets a valid card choice from the player
int getPlayerCardChoice(PLAYER* pArrPlayers, int player, CARDS* upperCard)
{
	int chosenCard;
	bool correctCard = false;
	while (!correctCard) {
		printf("Please enter 0 if you want to take a card from the deck\nor 1-%d if you want to put your cards in the middle:\n", pArrPlayers[player].allCards);
		scanf("%d", &chosenCard);

		if (!isCardPlayable(chosenCard, &pArrPlayers[player], upperCard)) {
			printf("Invalid card! Try again.\n");
		}
		else {
			correctCard = true;
		}
	}
	return chosenCard;
}

// Deals initial cards to all players
void dealInitialCards(PLAYER* pArrPlayers, int numOfPlayers, int* indexArr)
{
	for (int player = 0; player < numOfPlayers; player++) {
		// Initialize player fields
		pArrPlayers[player].roundNumber = START_CARDS;
		pArrPlayers[player].allCards = START_CARDS;
		pArrPlayers[player].size_of_arrCards = START_CARDS;

		// Allocate memory for player's cards
		pArrPlayers[player].arrCards = (CARDS*)malloc(pArrPlayers[player].allCards * sizeof(CARDS));
		if (pArrPlayers[player].arrCards == NULL) {
			printf("Error! Memory allocation failed\n");
			exit(1);
		}

		// Deal 8 cards to the player
		for (int indexCard = 0; indexCard < START_CARDS; indexCard++) {
			addCardToPlayer(pArrPlayers, &player, indexArr);
			matchColorToCard(&pArrPlayers[player].arrCards[indexCard]);
		}
	}
}

// The function add card to player
void addCardToPlayer(PLAYER* pArrPlayers, int* player, int* indexArr)
{
	//check if the cards not more than 50 cards per player
	if (pArrPlayers[*player].allCards >= MAX_CARDS_PER_PLAYER) {
		printf("Error: Player has too many cards! Skipping turn.\n");
	}
	else {
		if (pArrPlayers[*player].allCards >= pArrPlayers[*player].size_of_arrCards)// check if we nead to allocate the memory
		{
			pArrPlayers[*player].size_of_arrCards += 4; // increase 4 every addition
			CARDS* temp = realloc(pArrPlayers[*player].arrCards, pArrPlayers[*player].size_of_arrCards * sizeof(CARDS));
			if (!temp) {
				free(pArrPlayers[*player].arrCards);
				pArrPlayers[*player].arrCards = NULL;
				printf("Error: Memory allocation failed\n");
				exit(1);
			}
			pArrPlayers[*player].arrCards = temp;
		}

		// add new card
		pArrPlayers[*player].arrCards[pArrPlayers[*player].allCards].cards = rand() % 14 + 1;
		matchColorToCard(&pArrPlayers[*player].arrCards[pArrPlayers[*player].allCards]);
		indexArr[pArrPlayers[*player].arrCards[pArrPlayers[*player].allCards].cards]++;
		pArrPlayers[*player].allCards++;
	}
}


// Main function to remove a card from a player
void removeCardFromPlayer(PLAYER* pArrPlayers, int* player, int indexCard, CARDS* upperCard, int* numOfPlayers, int* indexArr) {
	indexCard--;
	if (pArrPlayers[*player].arrCards[indexCard].cards <= 9 && pArrPlayers[*player].arrCards[indexCard].cards != 2) {
		removeRegularCard(pArrPlayers, player, indexCard, upperCard);
	}
	else {
		removeSpecialCard(pArrPlayers, player, indexCard, upperCard, numOfPlayers, indexArr);
	}
}

// Function to remove a regular card
void removeRegularCard(PLAYER* pArrPlayers, int* player, int indexCard, CARDS* upperCard) {
	removeOneCard(pArrPlayers, player, indexCard, upperCard);
}


// Function to remove a special card
void removeSpecialCard(PLAYER* pArrPlayers, int* player, int indexCard, CARDS* upperCard, int* numOfPlayers, int* indexArr) {
	switch (pArrPlayers[*player].arrCards[indexCard].cards) {
	case taki:
		removeTakiCard(pArrPlayers, player, indexCard, upperCard);
		break;
	case stop:
		removeStopCard(pArrPlayers, player, indexCard, upperCard, numOfPlayers, indexArr);
		break;
	case changeDirection:
		removeChangeDirectionCard(pArrPlayers, player, indexCard, upperCard, numOfPlayers);
		break;
	case plus:
		removePlusCard(pArrPlayers, player, indexCard, upperCard, indexArr);
		break;
	case changeColor:
		removeColorCard(pArrPlayers, player, indexCard, upperCard);
		changeColorCard(upperCard);
		removeOneCard(pArrPlayers, player, indexCard, upperCard);
		break;
	}
}


// Function that removes the Taki card and all the cards allowed to be played with it  
void removeTakiCard(PLAYER* pArrPlayers, int* player, int indexCard, CARDS* upperCard)
{
	// Remove the initial Taki card
	removeOneCard(pArrPlayers, player, indexCard, upperCard);
	printf("Upper card:\n");
	printCard(upperCard);

	// Continue allowing the player to play cards of the same color
	while (pArrPlayers[*player].allCards > 0) {
		// Print player's cards
		printf("\n%s's turn:\n", pArrPlayers[*player].name);
		for (int i = 0; i < pArrPlayers[*player].allCards; i++) {
			printCard(&pArrPlayers[*player].arrCards[i]);
		}

		// Get player's choice (0 to end turn, 1 to allCards to play a card)
		int chosenCard = getPlayerCardChoice(pArrPlayers, *player, upperCard);

		if (chosenCard == 0) {
			break; // End the turn
		}

		// Remove the chosen card
		removeOneCard(pArrPlayers, player, chosenCard, upperCard);
		printf("Upper card:\n");
		printCard(upperCard);

		// Handle Change Color card
		if (pArrPlayers[*player].allCards > 0 && pArrPlayers[*player].arrCards[chosenCard - 1].cards == changeColor) {
			changeColorCard(upperCard);
		}
	}
}

// Function to remove STOP card 
void removeStopCard(PLAYER* pArrPlayers, int* player, int indexCard, CARDS* upperCard, int* numOfPlayers, int* indexArr) {
	removeOneCard(pArrPlayers, player, indexCard, upperCard);
	if (pArrPlayers[*player].allCards == 0 && *numOfPlayers == 2)
		addCardToPlayer(pArrPlayers, player, indexArr);
	if (*player < *numOfPlayers - 1)
		(*player)++;
	else
		*player = 0;
}

// Function to remove Change direction card 
void removeChangeDirectionCard(PLAYER* pArrPlayers, int* player, int indexCard, CARDS* upperCard, int* numOfPlayers) {
	removeOneCard(pArrPlayers, player, indexCard, upperCard);
	swapOrderOfPlayers(pArrPlayers, numOfPlayers);
	if (*numOfPlayers - 1 - *player == 0)
		*player = 0;
	else if (*numOfPlayers - 1 - *player == 1)
		*player = 1;
	else if (*numOfPlayers % 2 == 0)
		(*player) -= 3;
	else
		(*player) -= 2;
}

// Function to remove Plus card 
void removePlusCard(PLAYER* pArrPlayers, int* player, int indexCard, CARDS* upperCard, int* indexArr) {
	bool correctCard = false;
	removeOneCard(pArrPlayers, player, indexCard, upperCard);

	printf("Upper card:\n");
	printCard(upperCard);

	printf("\n%s's turn:\n", pArrPlayers[*player].name);
	for (indexCard = 0; indexCard < pArrPlayers[*player].allCards; indexCard++)
		printCard(&pArrPlayers[*player].arrCards[indexCard]);

	printf("Please enter 0 if you want to take a card from the deck\nor 1-%d if you want to put your cards in the middle:\n", pArrPlayers[*player].allCards);
	scanf("%d", &indexCard);
	if (indexCard == 0)
		addCardToPlayer(pArrPlayers, player, indexArr);
	else {
		while (correctCard == false) {
			if (!isCardPlayable(indexCard, &pArrPlayers[*player], upperCard)) {
				printf("Invalid card! Try again.");
				printf("Please enter 0 if you want to take a card from the deck\nor 1-%d if you want to put your cards in the middle:\n", pArrPlayers[*player].allCards);
				scanf("%d", &indexCard);
			}
			else {
				if (indexCard == 0)
					addCardToPlayer(pArrPlayers, player, indexArr);
				else
					removeOneCard(pArrPlayers, player, indexCard - 1, upperCard);
				correctCard = true;
			}
		}
		if (pArrPlayers[*player].allCards == 0)
			addCardToPlayer(pArrPlayers, player, indexArr);
	}
}

// Function to remove Color card 
void removeColorCard(PLAYER* pArrPlayers, int* player, int indexCard, CARDS* upperCard)
{
	changeColorCard(upperCard);
	removeOneCard(pArrPlayers, player, indexCard, upperCard);
}


// The function change the upper card when the player chose to remove a color card
void changeColorCard(CARDS* upperCard)
{
	int colorChoice;
	printf("Please enter your color choice:\n");
	printf("1 - Yellow\n2 - Red\n3 - Blue\n4 - Grean\n");
	scanf("%d", &colorChoice);
	while (colorChoice < 1 || colorChoice>4)
	{
		printf("Invalid choice! Try again.");
		printf("Please enter your color choice:\n");
		printf("1 - Yellow\n2 - Red\n3 - Blue\n4 - Grean\n");
		scanf("%d", &colorChoice);
	}
	upperCard->cards = 14;
	switch (colorChoice)
	{
	case 1:
		upperCard->color = 'Y';
		break;
	case 2:
		upperCard->color = 'R';
		break;
	case 3:
		upperCard->color = 'B';
		break;
	case 4:
		upperCard->color = 'G';
		break;
	default:
		upperCard->color = ' ';
		break;
	}

}

// The function recived the array of card od the player and "remoev" the card that was chosen
void removeOneCard(PLAYER* pArrPlayers, int* player, int card, CARDS* upperCard)
{
	upperCard->cards = pArrPlayers[*player].arrCards[card].cards;
	if (upperCard->color == ' ' || upperCard->cards != 14)
		upperCard->color = pArrPlayers[*player].arrCards[card].color;

	for (int i = card; i < pArrPlayers[*player].allCards - 1; i++)
	{
		pArrPlayers[*player].arrCards[i].cards = pArrPlayers[*player].arrCards[i + 1].cards;
		pArrPlayers[*player].arrCards[i].color = pArrPlayers[*player].arrCards[i + 1].color;
	}
	pArrPlayers[*player].allCards--;
}

// This function change the order of the players when a player chose chnage direction card
void swapOrderOfPlayers(PLAYER* pArrPlayers, int* numOfPlayers)
{
	int i = 0, j = *numOfPlayers - 1;
	int check = (int)(*numOfPlayers / 2);
	PLAYER temp;
	while (check > 0)
	{
		temp = pArrPlayers[i];
		pArrPlayers[i] = pArrPlayers[j];
		pArrPlayers[j] = temp;
		i++;
		j--;
		check--;
	}
}

/*
This function calculates and prints the statistics of card occurrences during the game,
displaying the card order by frequency and the frequency of each card.
*/
void printGameStatistics(int indexArr[]) {
	int temp[SIZE_IND_ARR];
	int maxArrCard[SIZE_IND_ARR];
	int maxIndex;


	for (int i = 1; i < SIZE_IND_ARR; i++) {
		temp[i] = indexArr[i];
	}


	for (int i = 1; i < SIZE_IND_ARR; i++) {
		maxIndex = 0;
		maxArrCard[i] = maxIndex;
		for (int j = 1; j < SIZE_IND_ARR; j++) {
			if (temp[maxIndex] <= temp[j] && temp[j] != -1) {
				maxIndex = j;
				maxArrCard[i] = maxIndex;
			}
		}
		temp[maxIndex] = -1;
	}


	printf("************ Game Statistic ************\n");
	printf("Card # | Frequency\n");
	printf("__________________\n");
	for (int i = 1; i < SIZE_IND_ARR; i++) {
		if (maxArrCard[i] <= 9) {
			printf("    %d    |    %d\n", maxArrCard[i], indexArr[maxArrCard[i]]);
		}
		else if (maxArrCard[i] == taki) {
			printf(" TAKI   |    %d\n", indexArr[maxArrCard[i]]);
		}
		else if (maxArrCard[i] == stop) {
			printf(" STOP   |    %d\n", indexArr[maxArrCard[i]]);
		}
		else if (maxArrCard[i] == changeDirection) {
			printf("  <->   |    %d\n", indexArr[maxArrCard[i]]);
		}
		else if (maxArrCard[i] == plus) {
			printf("    +    |    %d\n", indexArr[maxArrCard[i]]);
		}
		else if (maxArrCard[i] == changeColor) {
			printf(" COLOR  |    %d\n", indexArr[maxArrCard[i]]);
		}
	}
}

// The function handles the scenario where a player plays their last card
void handlePlayerTurnEnd(PLAYER* pArrPlayers, int* player, int chosenCard, CARDS* upperCard, int* numOfPlayers, int* indexArr, bool* continueGame)
{
	if (pArrPlayers->roundNumber > 1)
	{
		(pArrPlayers->roundNumber)--;

		for (int i = 0; i < pArrPlayers->roundNumber; i++) {
			addCardToPlayer(pArrPlayers, player, indexArr);
		}
	}
	else
		*continueGame = false; // There is a winner. Game stop.
}




