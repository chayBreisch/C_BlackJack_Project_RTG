#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include "strucusAndHelpF.h"
//=====================================================
// init function
//=====================================================
int initialGame(User *player, User *dealer, CardList *deck)
{
    initDeck(deck);
    initUser(player, deck, 1000);
    initUser(dealer, deck, 0);
    return betting;
}

//=====================================================
// betting function
//=====================================================

int bettingF(User *player, User *dealer)
{
    printf("player cash: %d\n", player->cash);
    printf("pot: %d\n", dealer->cash);
    if (player->cash < 10 && !dealer->cash)
    {
        printf("Run out of cash - The game is over:(.");
        return endGame;
    }
    int amountBet = player->cash + 1;
    while (amountBet > player->cash || amountBet % 10)
    {
        printf("Enter an amount to bet: - can't be more than your cash...");
        scanf("%d", &amountBet);
        if (amountBet > player->cash)
        {
            printf("You don't have enough money to bet this amount.\n");
        }
        else if (amountBet % 10)
        {
            printf("You have to bet on something that is divisible by 10.\n");
            amountBet = player->cash + 1;
        }
    }
    player->cash -= amountBet;
    dealer->cash += amountBet;
    printf("player cash: %d\n", player->cash);
    printf("pot: %d\n", dealer->cash);
    return userTurn;
}

//=====================================================
// player turn function
//=====================================================

int playerTurn(User *player, User *dealer, CardList *deck)
{
    int sumHand = sumValueCards(player->hand);
    char choice[6] = "";
    if (sumHand == 21)
    {
        puts("Black jack-------");
        player->cash += (int)(2.5 * dealer->cash);
        dealer->cash = 0;
        return resetCards;
    }

    while (sumHand < 21)
    {
        puts("Enter Hit or Stand: ");
        scanf("%s", &choice);
        // int i;
        for (int i = 0; choice[i] != '\0'; i++)
        {
            choice[i] = toupper(choice[i]);
        }
        if (!strcmp(choice, "HIT"))
        {
            addCard(cardRemoveAt(deck), player->hand);
            sumHand = sumValueCards(player->hand);
            printCardList(player->hand, player->hand->len, "player");
        }
        else if (!strcmp(choice, "STAND"))
        {
            return dealerTurn;
        }
        else
        {
            puts("ERROR: invalid choice, try again\n");
        }
    }
    return endGame;
}

//=====================================================
// dealer turn function
//=====================================================

int dealeTurn(User *dealer, const User *player, CardList *deck)
{
    int max = sumValueCards(player->hand);
    if (max > 21)
    {
        return endGame;
    }
    int sumHand = sumValueCards(dealer->hand);
    printCardList(dealer->hand, dealer->hand->len, "dealer");
    while (sumHand < 17 && sumHand < max)
    {
        addCard(cardRemoveAt(deck), dealer->hand);
        sumHand = sumValueCards(dealer->hand);
        printCardList(dealer->hand, dealer->hand->len, "dealer");
    }
    return endGame;
}

//=====================================================
// end game function
//=====================================================

int endGameTurn(User *player, User *dealer)
{
    int sumPlayer = sumValueCards(player->hand);
    int sumDealer = sumValueCards(dealer->hand);
    if (sumPlayer > 21)
    {
        puts("player lose!");
        dealer->cash = 0;
    }
    else if (sumDealer > 21)
    {
        puts("dealer bust!!");
        player->cash += (2 * dealer->cash);
        dealer->cash = 0;
    }
    else if (sumPlayer > sumDealer)
    {
        puts("player won!!");
        player->cash += (2 * dealer->cash);
        dealer->cash = 0;
    }
    else if (sumDealer > sumPlayer)
    {
        puts("dealer won!!");
        dealer->cash = 0;
    }
    else
    {
        puts("Tie!");
    }
    return resetCards;
}

//=====================================================
// reset function
//=====================================================
int resetCardsGame(User *player, User *dealer, CardList *deck)
{
    char choice = 0;
    if (player->cash < 10 && !dealer->cash)
    {
        puts("game over!!");
        return exitGame;
    }
    puts("Do you want to play again? (Y/N)");
    while (choice != 'Y' && choice != 'N')
    {
        scanf(" %c", &choice);
        choice = toupper(choice);
        switch (choice)
        {
        case 'Y':
            restartList(deck, player->hand);
            restartList(deck, dealer->hand);
            printCardList(deck, deck->len, "deck");
            initUser(player, deck, player->cash);
            initUser(dealer, deck, dealer->cash);
            break;
        case 'N':
            return exitGame;
        default:
            puts("ERROR: invalid selection. try again: ");
            break;
        }
    }
    return betting;
}

//=====================================================
// main function
//=====================================================
void main(void)
{
    srand(time(NULL));
    int state = initiaGame;
    User *player = (User *)malloc(sizeof(User));
    User *dealer = (User *)malloc(sizeof(User));
    CardList *deck = (CardList *)malloc(sizeof(CardList));
    deck->len = 0;
    deck->head = NULL;
    while (state != exitGame)
    {
        switch (state)
        {
        case initiaGame:
            state = initialGame(player, dealer, deck);
            break;

        case betting:
            state = bettingF(player, dealer);
            break;

        case userTurn:
            printCardList(player->hand, player->hand->len, "player");
            printCardList(dealer->hand, 1, "dealer");
            state = playerTurn(player, dealer, deck);
            break;

        case dealerTurn:
            state = dealeTurn(dealer, player, deck);
            break;

        case endGame:
            state = endGameTurn(player, dealer);
            break;
        case resetCards:
            state = resetCardsGame(player, dealer, deck);
            break;
        default:
            break;
        }
    }
    freeObjects(deck, player, dealer);
    printf("See you next time soon... Good Bye!!!");
}
