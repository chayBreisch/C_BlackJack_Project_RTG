#ifndef STRUCTS
#define STRUCTS
#include <inttypes.h>

typedef struct Card Card;
struct Card
{
    // uint8_t shape: 4;
    // uint8_t number: 4;
    uint8_t data;
    Card *next;
};

typedef struct
{
    Card *head;
    size_t len;
} CardList;

typedef struct
{
    CardList *hand;
    int cash;
} User;

enum States
{
    initiaGame,
    betting,
    userTurn,
    dealerTurn,
    endGame,
    resetCards,
    exitGame
};

enum Shapes
{
    hearts = 0,
    clubs,
    diamond,
    spades
};

void freeCardList(CardList *list);
void freeObjects(CardList *deck, User *player, User *dealer);
int getValueCard(Card*c);
void addCard(Card *c, CardList *list);
Card *createCard(size_t val, size_t shape);
Card *cardRemoveAt(CardList *list);
int sumValueCards(CardList *list);
void printCardList(CardList *deck, size_t len, const char *title);
void printCard(const Card c);
void initUser(User *user, CardList *deck, int cash);
void initDeck(CardList *deck);
void restartList(CardList *deck, CardList *list);

#endif