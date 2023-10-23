#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "strucusAndHelpF.h"

//================================
// function to free the card list
//================================
void freeCardList(CardList *list)
{
    while (list->len--)
    {
        Card *c = list->head;
        list->head = c->next;
        c->next = NULL;
        free(c);
    }
}

//=============================================
// function to free all the objects of the game
//=============================================

void freeObjects(CardList *deck, User *player, User *dealer)
{
    freeCardList(deck);
    freeCardList(player->hand);
    freeCardList(dealer->hand);
    free(deck);
    free(dealer);
    free(player);
}

//============================================
// function to calculate the value of the card
//============================================
int getValueCard(Card *c)
{
    int val = c->data >> 4;
    switch (val)
    {
    case 11:
    case 12:
    case 13:
        return 10;
    default:
        break;
    }
    return val;
}

//===================================
// function to add a card to the list
//===================================
void addCard(Card *c, CardList *list)
{
    c->next = list->head;
    list->head = c;
    list->len++;
}

//================================
// function to create a card
//================================
Card *createCard(size_t val, size_t shape)
{
    Card *c = (Card *)malloc(sizeof(Card));
    c->data = (val << 4) | 1 << shape;
    c->next = NULL;
    return c;
}

//=====================================
// function to random a card from list
//=====================================
Card *cardRemoveAt(CardList *list)
{
    size_t index = rand() % list->len;
    Card *prevCard = NULL;
    Card *currCard = list->head;
    while (index--)
    {
        prevCard = currCard;
        currCard = currCard->next;
    }
    if (prevCard == NULL)
    {
        list->head = currCard->next;
    }
    else
    {
        prevCard->next = currCard->next;
    }
    list->len--;
    return currCard; 
}

//==============================================
// function to calculate the sum of value cards
//==============================================
int sumValueCards(CardList *list)
{
    int sum = 0;
    bool is_Ace = false;
    Card *c = list->head;
    for (size_t i = 0; i < list->len; i++)
    {
        int val = c->data >> 4;
        if (c->data >> 4 == 1)
        {
            is_Ace = true;
        }
        sum += getValueCard(c);
        c = c->next;
    }
    if (is_Ace && sum + 10 <= 21)
    {
        sum += 10;
    }
    return sum;
}
//=====================================================
// print functions
//=====================================================

void printCard(const Card c)
{
    switch (c.data >> 4)
    {
    case 1:
        printf("Ace: ");
        break;
    case 11:
        printf("Jack: ");
        break;
    case 12:
        printf("Queen: ");
        break;
    case 13:
        printf("King: ");
        break;
    default:
        printf("%d : ", c.data >> 4);
        break;
    }
    switch (c.data & 0x0F)
    {
    case 1 << hearts:
        printf("Hearts\n");
        break;
    case 1 << clubs:
        printf("Clubs\n");
        break;
    case 1 << diamond:
        printf("Diamond\n");
        break;
    case 1 << spades:
        printf("Spades\n");
        break;
    default:
        break;
    }
}

void printCardList(CardList *deck, size_t len, const char *title)
{
    printf("============%s============\n\n", title);
    Card *c = deck->head;
    for (size_t i = 0; i < len && c; i++)
    {
        printCard(*c);
        c = c->next;
    }
    puts("");
}

//=====================================================
// init functions
//=====================================================

void initDeck(CardList *deck)
{
    deck->head = createCard(1, 0);
    deck->len = 1;
    size_t val = 2;
    for (size_t shape = hearts; shape <= spades; shape++)
    {
        for (; val <= 13; val++)
        {
            addCard(createCard(val, shape), deck);
        }
        val = 1;
    }
    printCardList(deck, deck->len, "deck");
}

void initUser(User *user, CardList *deck, int cash)
{
    user->cash = cash;
    user->hand = (CardList *)malloc(sizeof(CardList));
    user->hand->len = 0;
    user->hand->head = NULL;
    addCard(cardRemoveAt(deck), user->hand);
    addCard(cardRemoveAt(deck), user->hand);
}

//=====================================================
// reset function
//=====================================================
void restartList(CardList *deck, CardList *list)
{
    Card *c = list->head;
    while (c->next)
    {
        c = c->next;
    }
    c->next = deck->head;
    deck->head = list->head;
    deck->len += list->len;
    list->head = NULL;
    list->len = 0;
    c = NULL;
}
