#include <memory>
#include <iostream>
#include <string>
#include <optional>

using NameType = std::string;

struct FootBallPlayer {
	std::string name;
	int num;
};

class FootBallRoster {
	private:
		class Node {
		private:
			std::shared_ptr<Node> next;
			std::weak_ptr<Node> prev;
			FootBallPlayer m_P;

		public:
			Node(FootBallPlayer P) { m_P = P; }
			void setPrev(std::shared_ptr<Node> prev) { this->prev = prev; }
			void setNext(std::shared_ptr<Node> next) { this->next = next; }
			std::weak_ptr<Node> getPrev() { return prev; }
			std::weak_ptr<Node> getNext() { return next; }
			NameType getName() const { return m_P.name; }
			int getNum() const { return m_P.num; }
			FootBallPlayer getP() { return m_P; }
		};

		std::shared_ptr<Node> head;
		std::weak_ptr<Node> tail, favorite;
		int size;

		std::shared_ptr<Node> find(NameType name) {
			std::shared_ptr<Node> current = head;
			while (current != nullptr) {
				if (current->getName() == name) {
					return current;
				}
				current = current->getNext().lock();
			}
			return nullptr;
		}

	public:
		FootBallRoster() { size = 0; }

		// Adds a FootBallPlayer to the roster.
		void addPlayer(FootBallPlayer player) {
			std::shared_ptr<Node> newNode = std::make_shared<Node>(player);
			if (size == 0) { tail = newNode; }
			else { 
				newNode->setNext(head);
				head->setPrev(newNode);
			}
			head = newNode;
			size += 1;
		}

		// deletes the first FootBallPlayer that matches name
		bool deletePlayer(NameType name) {
			std::shared_ptr<Node> target = find(name);
			if (target == nullptr) { return false; }
			else if (target == head) { head = target->getNext().lock(); }
			else if (target == tail.lock()) { 
				target->getPrev().lock()->setNext(nullptr);
				tail = target->getPrev();
			}
			else {
				target->getPrev().lock()->setNext(target->getNext().lock());
				target->getNext().lock()->setPrev(target->getPrev().lock());
			}
			target.reset();
			size -= 1;
			return true;
		}

		// Uses a weak_ptr to return true if there exists a favorite player, false otherwise 
		bool setFavorite(NameType name) {
			std::shared_ptr<Node> player = find(name);
			if (player == nullptr) { return false; }
			favorite = find(name);
			return true;

		}

		// Returns an optional parameter if there is a favorite player
		std::optional<FootBallPlayer> getFavorite() const { 
			if (favorite.expired()) { return std::nullopt; }
			return favorite.lock()->getP(); }

		// Prints the list of FootBallPlayers on the roster
		void printPlayers() const { for (std::shared_ptr<Node> nav = head; nav != nullptr; nav = nav->getNext().lock()) { std::cout << nav->getName() << " #" << nav->getNum() << "\n"; } }

};

// prints out the name of the favorite player or a message 
// stating there is no favorite player.
void favoritePlayer(const FootBallRoster& roster) {
	if (roster.getFavorite() == std::nullopt) { std::cout << "You don't have a favorite player\n"; }
	else { std::cout << "Your favorite player is: " << roster.getFavorite()->name << "\n"; }
}

int main() {

	FootBallPlayer fb1{ "Matthew Stafford", 9 };
	FootBallPlayer fb2{ "Aaron Donald", 99 };
	FootBallPlayer fb3{ "Jalen Ramsey", 5 };

	FootBallRoster roster;

	roster.addPlayer(fb1);
	roster.addPlayer(fb2);
	roster.addPlayer(fb3);

	roster.printPlayers();
	roster.setFavorite("Aaron Donald");

	favoritePlayer(roster);

	roster.deletePlayer("Aaron Donald");
	favoritePlayer(roster);
}
