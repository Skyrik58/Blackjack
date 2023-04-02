#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <limits>

using namespace std;

void build_deck(vector <vector <int>> *deck_ptr);
void get_card(vector <int>& suit, vector <int>& pip, vector <vector <int>>& deck);
bool is_all_zero(const vector <vector <int>> *deck);
void draw_cards(vector <int> p_suit, vector <int> p_pip, vector <int> d_suit, vector <int> d_pip, bool reveal);
int get_input(vector <int>& p_hand, vector <int>& p_split, int* balance, int wager);
void reset_hands(vector <int>& p_suit, vector <int>& p_pip, vector <int>& s_suit, vector <int>& s_pip, vector <int>& d_suit, vector <int>& d_pip);
int check_hand(vector <int> hand);
bool check_win(int p_total, int d_total, int selection, int* balance, int wager, vector <int>& p_pip);


int main () {
    //initialize variables
    srand(time(NULL));
    vector <vector <int>> main_deck(4, vector <int> (13), {});
    int balance {100};
    int wager {10};
    vector <int> split_suit {};
    vector <int> split_pip {};
    vector <int> player_suit {};
    vector <int> player_pip {};
    vector <int> dealer_suit {};
    vector <int> dealer_pip {};
    //I added some text
    
    //build the deck with 4 rows of 1-13
    build_deck(&main_deck);
    
    //initiate game loop
    bool game_over {false};
    while (!game_over){
        //class or function to ascii art draw cards
        bool done {false};
        
        balance -= wager;
            
        cout << "You bet $10" << endl;
        cout << "Remaining balance is: " << balance << endl << endl;
        
        do {   //draw dealer and player initial hand
            get_card(player_suit, player_pip, main_deck);
            get_card(dealer_suit, dealer_pip, main_deck);
            get_card(player_suit, player_pip, main_deck);
            get_card(dealer_suit, dealer_pip, main_deck);
            done = true;
        } while (!done);
        
        done = false;
        do {
            done = false;
            system("cls");
            draw_cards(player_suit, player_pip, dealer_suit, dealer_pip, done);
            //hit - stay - split
            int player_total {};
            int dealer_total {};
            int player_selection {};
            
            player_total = check_hand(player_pip);
            dealer_total = check_hand(dealer_pip);
            
//            cout << "Dealer total " << dealer_total << endl << endl;
//            cout << "Player total " << player_total << endl << endl;
            
            if (check_win(player_total, dealer_total, player_selection, &balance, wager, player_pip)) {
                cout << endl << "Your new balance is: " << balance << endl << endl;
                done = true;
                break;
            }
            
            player_selection = get_input(player_pip, split_pip, &balance, wager);
            
            if (player_selection == 1) {
                get_card(player_suit, player_pip, main_deck);
            }
            else if (player_selection == 2) {
                while (dealer_total <= 17) {
                    for (auto card: dealer_pip) {
                        vector <bool> aces {};
                        if (card == 0) {
                            aces.push_back(true);
                        }
                        if (dealer_total == 17 && aces.size() > 0) {
                            aces.pop_back();
                            dealer_total -= 10;
                        }
                    }
                    if (dealer_total < 17) {
                        get_card(dealer_suit, dealer_pip, main_deck);
                        dealer_total = check_hand(dealer_pip);
                    }
                    else{
                        break;
                    }
                }
                done = true;
            }
            else if (player_selection == 3) {
                split_suit.push_back(player_suit.back());
                split_pip.push_back(player_pip.back());
                player_suit.pop_back();
                player_pip.pop_back();
                get_card(player_suit, player_pip, main_deck);
                get_card(split_suit, split_pip, main_deck);
                cout << player_suit.at(0) << " " << player_pip.at(0) << endl;
                cout << split_suit.at(0) << " " << split_pip.at(0) << endl;
                cout << player_suit.at(1) << " " << player_pip.at(1) << endl;
                cout << split_suit.at(1) << " " << split_pip.at(1) << endl;
            }
                                        
            if (done) {
                system("cls");
                draw_cards(player_suit, player_pip, dealer_suit, dealer_pip, done);
                check_win(player_total, dealer_total, player_selection, &balance, wager, player_pip);
            }
            
            if (split_pip.size() != 0 && done) {
                player_pip.clear();
                player_suit.clear();
                player_suit.push_back(split_suit.back());
                player_pip.push_back(split_pip.back());
                split_suit.pop_back();
                split_pip.pop_back();
                player_suit.push_back(split_suit.back());
                player_pip.push_back(split_pip.back());
                split_suit.pop_back();
                split_pip.pop_back();
                done = false;
            }
            
            player_selection = 0;
            cout << "Your balance is: " << balance << endl << endl;
            
        } while (!done);
        
        //reset hands
        reset_hands(player_suit, player_pip, split_suit, split_pip, dealer_suit, dealer_pip);
        
        //game over once out of money
        if (balance <= 0) {
            game_over = true;
        }
        
        //end the game
        int game_loop {};
        do {
            cout << "1: Yes" << endl;
            cout << "2: No" << endl;
            cout << "Would you like to continue: ";
            cin >> game_loop;
        } while (game_loop != 1 && game_loop != 2);
        
        if (game_loop == 2)
            game_over = true;
    }
    return 0;
}
// Assign the value of cards
void build_deck(vector <vector <int>> *deck_ptr) {
    for (size_t i {}; i < 4; i++) {
        for (size_t j {}; j < 13; j++) {
            (*deck_ptr).at(i).at(j) = j + 1; // this will leave 0 as an option for random cards not being pulled again until the deck is reset
//            cout << "Building deck currently at " << i << " and " << j << endl;
        }
    }
}

void get_card(vector <int>& suit,vector <int>& pip, vector <vector <int>>& deck) {
    // if the deck is empty (all elements are '0') the deck will be rebuilt
    if (is_all_zero(&deck))
        build_deck(&deck);
    bool done {false};
    
    do {
        // pulls two random numbers to serve as the suit and the pip
        int rand_suit = rand() % 4;
        int rand_pip = rand() % 13;
        int chosen_pip {rand_pip};
        int chosen_suit {rand_suit};
        // int rand_deck = rand() % 6;
        // ensures random card has not already been pulled on this "shuffle" and then assigns this
        // to either the dealers hand or the players hand depending on who called the function
        if (deck.at(chosen_suit).at(chosen_pip) != 0) {
            suit.push_back(chosen_suit);
            pip.push_back(chosen_pip);
//            cout << "current player was given the card " << deck.at(chosen_suit).at(chosen_pip) << endl;
            deck.at(chosen_suit).at(chosen_pip) = 0;
            done = true;
        }
    } while (!done);
}

bool is_all_zero(const vector<vector<int>>* deck) {
    for (const auto& suit : *deck) {
        for (int pip : suit) {
            if (pip != 0) {
                return false;
            }
        }
    }
    return true;
}

void draw_cards(vector <int> p_suit, vector <int> p_pip, vector <int> d_suit, vector <int> d_pip, bool reveal) {
    vector <char> player_suit {};
    vector <char> player_pip {};
    vector <char> dealer_suit {};
    vector <char> dealer_pip {};
    for (auto suits: p_suit) {
        if (suits == 0)
            player_suit.push_back('\x03');
        else if (suits == 1)
            player_suit.push_back('\x04');
        else if (suits == 2)
            player_suit.push_back('\x05');
        else
            player_suit.push_back('\x06');
    }
    for (auto pips: p_pip) {
        if (pips == 0)
            player_pip.push_back('A');
        else if (pips == 9)
            player_pip.push_back('T');
        else if (pips == 10)
            player_pip.push_back('J');
        else if (pips == 11)
            player_pip.push_back('Q');
        else if (pips == 12)
            player_pip.push_back('K');
        else
            player_pip.push_back(pips + '1');
    }
    for (auto suits: d_suit) {
        if (suits == 0)
            dealer_suit.push_back('\x03');
        else if (suits == 1)
            dealer_suit.push_back('\x04');
        else if (suits == 2)
            dealer_suit.push_back('\x05');
        else
            dealer_suit.push_back('\x06');
    }
    for (auto pips: d_pip) {
        if (pips == 0)
            dealer_pip.push_back('A');
        else if (pips == 9)
            dealer_pip.push_back('T');
        else if (pips == 10)
            dealer_pip.push_back('J');
        else if (pips == 11)
            dealer_pip.push_back('Q');
        else if (pips == 12)
            dealer_pip.push_back('K');
        else
            dealer_pip.push_back(pips + '1');
    }
    
    cout << "           DEALERS HAND" << endl;
    for (size_t i {}; i < dealer_pip.size(); i++)
        cout << " _____ ";
    cout << endl;
    for (size_t i {}; i < dealer_pip.size(); i++) {
        if (reveal == false && i == 0){
            dealer_pip.at(i) = '*';
        }
        cout << "|" << dealer_pip.at(i) << "    |";
    }
    cout << endl;
    for (size_t i {}; i < dealer_pip.size(); i++)
        cout << "|     |";
    cout << endl;
    for (size_t i {}; i < dealer_pip.size(); i++) {
        if (!reveal && i == 0) {
            dealer_suit.at(i) = '*';
        }
        cout << "|  " << dealer_suit.at(i) << "  |";
    }
    cout << endl;
    for (size_t i {}; i < dealer_pip.size(); i++)
        cout << "|     |";
    cout << endl;
    for (size_t i {}; i < dealer_pip.size(); i++) {
        if (!reveal && i == 0) {
            dealer_pip.at(i) = '*';
        }
        cout << "|____" << dealer_pip.at(i) << "|";
    } 
    cout << endl;
    
    cout << endl << endl << endl << endl;
    
    cout << "           PLAYERS HAND" << endl;
    for (size_t i {}; i < player_pip.size(); i++)
        cout << " _____ ";
    cout << endl;
    for (size_t i {}; i < player_pip.size(); i++)
        cout << "|" << player_pip.at(i) << "    |";
    cout << endl;
    for (size_t i {}; i < player_pip.size(); i++)
        cout << "|     |";
    cout << endl;
    for (size_t i {}; i < player_pip.size(); i++)
        cout << "|  " << player_suit.at(i) << "  |";
    cout << endl;
    for (size_t i {}; i < player_pip.size(); i++)
        cout << "|     |";
    cout << endl;
    for (size_t i {}; i < player_pip.size(); i++)
        cout << "|____" << player_pip.at(i) << "|"; 
    cout << endl;
    
    cout << endl << endl;
}

int get_input(vector <int>& p_pip, vector <int>& s_pip, int* balance, int wager) {
    int input {};
    
    cout << "1: Hit" << endl;
    cout << "2: Stay" << endl;
    if (p_pip.at(0) == p_pip.at(1) && p_pip.size() == 2 && s_pip.size() == 0 && *balance >= wager) {
        cout << "3: Split" << endl;
    }
    cout << "Please make a selection: ";
    bool validated {false};
    while (!validated) {
        cin >> input;
        if (cin.fail()) {
            cout << "Please enter a correct selection: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else if (input == 3 && s_pip.size() == 0 && p_pip.at(0) == p_pip.at(1) && *balance >= wager) {
            cout << input;
            validated = true;
        }
        else if (input != 1 && input != 2) {
            cout << "Please enter a correct selection: ";
        }
        else {
            validated = true;
        }
    cout << endl << endl;
    }
    return input;
}

void reset_hands(vector <int>& p_suit, vector <int>& p_pip, vector <int>& s_suit, vector <int>& s_pip, vector <int>& d_suit, vector <int>& d_pip) {
    p_suit.clear();
    p_pip.clear();
    s_suit.clear();
    s_pip.clear();
    d_suit.clear();
    d_pip.clear();
}

int check_hand(vector <int> hand) {
    vector <bool> ace {};
    int total {};
    
    for (auto card: hand) {
        if (card == 0) {
            ace.push_back(true);
            total += 11;
        }
        else if (card >= 9) {
            total += 10;
        }
        else {
            total += ++card;
        }
        if (total > 21 && ace.size() > 0) {
            total -= 10;
            ace.pop_back();
        }
    }
    return total;
}

bool check_win(int p_total, int d_total, int selection, int* balance, int wager, vector <int>& p_pip) {
    if (p_total == 21 && p_pip.size() == 2) {
        //player wins their money back plus 50%        example:  $10 becomes $15
        *balance += (wager * 1.5);
        cout << "BLACKJACK!!!" << endl;
        return true;
    }
    else if (p_total > 21) {
        cout << "Bust! Better luck next hand." << endl << endl;
        return true;
    }
    else if ((p_total == 21 && d_total == 21) && (p_total > 21 && d_total < 21)) {
        //the player neither wins nor loses money
        *balance += wager;
        cout << "Push! Try again." << endl << endl;
        return true;
    }
    else if (d_total > 21 && p_total < 22) {
        *balance += (wager * 2);
        cout << "Winner!" << endl << endl;
        return true;
    }
    else if (selection == 2) {
        if (p_total == d_total) {
            *balance += wager;
            cout << "Push! Try again." << endl << endl;
            return true;
        }
        else if (p_total > d_total) {
            *balance += (wager * 2);
            cout << "Winner!" << endl << endl;
            return true;
        }
        else if (p_total < d_total) {
            cout << "House wins! Better luck next hand." << endl << endl;
            return true;
        }
    }
    return false;