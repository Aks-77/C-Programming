#include <stdlib.h>
#include <stdio.h>
#include "future.h"
#include "cards.h"
#include "deck.h"
#include "eval.h"

void add_future_card(future_cards_t * fc, size_t index, card_t * ptr) {
  //When new highest index in found, initialize between it and n_decks before changing n_decks
  //Initialize between new idx and 0 if there's nothing between them (n_decks = 0 in main so don't worry about it?)
  if (index >= fc->n_decks) {
    fc->decks = realloc(fc->decks, (index+1) * sizeof(deck_t));
    for (size_t i = fc->n_decks; i <= index; i++) {
      fc->decks[i].n_cards = 0;
      fc->decks[i].cards = NULL;
    }
    fc->n_decks = index+1;
  }

  fc->decks[index].n_cards++;
  fc->decks[index].cards = realloc(fc->decks[index].cards, fc->decks[index].n_cards * sizeof(card_t*));
  fc->decks[index].cards[fc->decks[index].n_cards - 1] = ptr;
}


/*This function takes a deck (which has been shuffled), and a future_cards_t
(which has been filled in with all the pointers to placeholders) and draws
cards from the deck and assigns their values and suits to the placeholders
pointed to in fc. For example if the deck is:
     As Kh 8c ....
and fc was created from the input:
     3c 4c ?0 ?1 ?2
     5h 9d ?0 ?1 ?2
then this function will draw As for ?0, and fill in the two placeholders for
?0 (which you can find with the pointers in fc, as described in the video).
Then it will draw Kh for ?1, and so on. Think about a case where this
function would need to print an error message.*/
void future_cards_from_deck(deck_t * deck, future_cards_t * fc) {
  if (deck->n_cards < fc->n_decks) {
    printf("More unknowns than cards\n");
    exit(EXIT_FAILURE);
  }

  card_t topCard;

  for (size_t i = 0; i < fc->n_decks; i++) { //Iterate through n_decks
    topCard.value = deck->cards[i]->value; //Draw a card from deck
    topCard.suit = deck->cards[i]->suit;

    for (size_t j = 0; j < fc->decks[i].n_cards; j++) { //iterate through each unknown card in current deck
      fc->decks[i].cards[j]->value = topCard.value; //Assign card values (not pointer values)
      fc->decks[i].cards[j]->suit = topCard.suit;
    }
  }
}
