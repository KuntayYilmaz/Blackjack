#include <iostream>
#include <vector>
#include <array>
#include <string_view>
#include "Random.h"
#include <cassert>

namespace Settings
{
	int bustLimit{ 21 };
	int dealerStop{ 17 };

}

struct Card
{
	enum Rank
	{
		rank_ace,
		rank_2,
		rank_3,
		rank_4,
		rank_5,
		rank_6,
		rank_7,
		rank_8,
		rank_9,
		rank_10,
		rank_jack,
		rank_queen,
		rank_king,
		max_ranks,

	};

	enum Suit
	{
		suit_club,
		suit_diamond,
		suit_heart,
		suit_spade,
		max_suits,
	};

	Rank rank{};
	Suit suit{};

	static constexpr std::array allRanks{ rank_ace, rank_2, rank_3, rank_4, rank_5, rank_6, rank_7, rank_8, rank_9, rank_10, rank_jack, rank_queen, rank_king };
	static constexpr std::array allSuits{ suit_club, suit_diamond, suit_heart, suit_spade };

	friend std::ostream& operator<<(std::ostream& out, const Card& card)
	{
		static constexpr std::array ranks{ 'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K' };
		static constexpr std::array suits{ 'C', 'D', 'H', 'S' };

		out << ranks[card.rank] << suits[card.suit];
		return out;
	}

	int returnValue() const
	{
		static constexpr std::array values { 11,2,3,4,5,6,7,8,9,10,10,10,10 };
		return values[rank];
	}

};

class Deck
{
private:
	std::array <Card, 52> m_Cards{};
	std::size_t m_nextCardIndex{ 0 };
public:
	Deck()
	{
		std::size_t index{ 0 };
		for (auto suit : Card::allSuits)
		{
			for (auto rank : Card::allRanks)
			{
				m_Cards[index] = Card{ rank,suit };
				index++;
			}
		}
	}
	std::array <Card, 52> getDeck() const { return m_Cards; }

public:
	Card dealCard(){
		assert(m_nextCardIndex != 52 && "Deck::dealCard ran out of cards");
		return m_Cards[m_nextCardIndex++];
	}


	void shuffle()
	{
		std::shuffle(m_Cards.begin(), m_Cards.end(), Random::mt);
		m_nextCardIndex = 0;
	}

};

struct Player
{
	int score{0};

};

bool play_a_round()
{
	Deck deck{};

	deck.shuffle();

	Player dealer{ deck.dealCard().returnValue() };
	Player player{ deck.dealCard().returnValue() + deck.dealCard().returnValue() };

	std::cout << "The dealer is showing: " << dealer.score << std::endl;
	std::cout << "You have score: " << player.score << std::endl;
	
	while (player.score < Settings::bustLimit)
	{
		char answer{};
		std::cout << "(h) to hit, or (s) to stand: ";
		std::cin >> answer;
		if (answer == 'h')
		{
			Card newCard{ deck.dealCard() };
			player.score += newCard.returnValue();
			std::cout << "You were dealt " << newCard << ". You now have: " << player.score << std::endl;
		}
		else if (answer == 's')
		{
			break;
		}
		else
		{
			continue;
		}
		if (player.score > Settings::bustLimit) {
			std::cout << "You went bust!" << std::endl;
			return false;
		}
	}

	while (dealer.score < Settings::dealerStop) 
	{
		Card newCard{ deck.dealCard() };
		dealer.score += newCard.returnValue();
		std::cout << "The dealer flips a " << newCard << ". They now have: " << dealer.score << std::endl;

	}
	if (dealer.score > Settings::bustLimit) {
		std::cout << "The dealer went bust!" << std::endl;
		return true;
	}

	return (player.score > dealer.score);
}



int main()
{
	(play_a_round()) ? (std::cout << "You win!") : (std::cout << "You lose!");

	char exit{};
	std::cout << "\nEnter some key to exit the program!";
	std::cin >> exit;
	return 0;
}