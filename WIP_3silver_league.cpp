#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#define TRAINING 0
#define CODING 1
#define DAILY_ROUTINE 2
#define TASK_PRIORITIZATION 3
#define ARCHITECTURE_STUDY 4
#define CONTINUOUS_INTEGRATION 5
#define CODE_REVIEW 6
#define REFACTORING 7

#define BONUS 8
#define DEBT 9

#define WAIT 0

using namespace std;

class   Data
{
    private:

    public:
        Data();
        ~Data();

        int current_location;
        int nb_release;
        int daily_routine_cards;
        int architecture_study_cards;
        int is_first;
        int turn;
};

class   Application
{
    private:

    public:
        Application();
        ~Application();

        string  app_name;
        int     id;
        int     need[8];
};

class   Player
{
    private:

    public:
        Player();
        ~Player();

        int location;
        int score;
        int permanent_daily_routine_cards;
        int permanent_architecture_study_cards;
};

class   Deck
{
    private:

    public:
        Deck();
        ~Deck();

        string  deck_name;
        int     cards_count[10];
};

Application::Application(){}
Application::~Application(){}

Player::Player(){}
Player::~Player(){}

Deck::Deck() : deck_name("NA") {}
Deck::~Deck(){}

Data::Data() : current_location(-1), nb_release(0), daily_routine_cards(0), architecture_study_cards(0) {}
Data::~Data(){}

/////////////      UTILS         /////////////
bool    is_in_hand(vector<Deck> decks, int deck_count, int card_type)
{
    for (int i = 0; i < deck_count; i++)
    {
        if (decks[i].deck_name == "HAND")
        {
            if (decks[i].cards_count[card_type] > 0)
                return (true);
        }
    }
    return (false);
}

bool    is_possible_move(const vector<string>& possible_moves, string move)
{
    for (int i = 0; i < possible_moves.size(); i++)
    {
        if (possible_moves[i] == move)
            return (true);
    }
    return (false);
}

/////////////     FUNCTIONS      /////////////

void    get_remaining_location_cards(int *location_cards, vector<Deck> decks, int deck_count)
{
    for (int i = 0; i < deck_count; i++)
    {
        for (int j = 0; j < 8; j++)
            location_cards[j] -= decks[i].cards_count[j];
    }
}

int least_wanted_card(const vector<Application>& apps, const vector<Deck>& decks, const int *my_cards)
{
    int card;
    int needed[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int least_value = 100000;

    for (int i = 0; i < apps.size(); i++)
    {
        for (int j = 0; j < 8; j++)
            needed[j] += apps[i].need[j];
    }
    for (int i = 0; i < 8; i++)
    {
        if (needed[i] <= least_value && is_in_hand(decks, decks.size(), i))
        {
            least_value = needed[i];
            card = i;
        }
    }
    return (card);
}

int best_to_give(const vector<Application>& apps, const vector<Deck>& decks, int deck_count)
{
    int special_card = 0;

    for (int i = 0; i < 8; i++)
    {
        if (is_in_hand(decks, deck_count, i))
            special_card = 1;
    }
    if (special_card == 0)
        return (-1);
    else
    {
        int is_me = 1;

        for (int i = 0; i < decks.size(); i++)
        {
            if (decks[i].deck_name == "HAND")
                is_me = i;
        }
        return (least_wanted_card(apps, decks, decks[is_me].cards_count));
    }
}

int get_best_card(const vector<Application>& apps, const vector<Deck>& decks)
{
    int best_card;
    int biggest_score = -1;
    int draw = -1;
    int automated = -1;
    int needed[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    for (int i = 0; i < decks.size(); i++)
    {
        if (decks[i].deck_name == "DRAW")
            draw = i;
        else if (decks[i].deck_name == "AUTOMATED")
            automated = i;
    }
    for (int i = 0; i < apps.size(); i++)
    {
        for (int j = 0; j < 8; j++)
            needed[j] += apps[i].need[j];
    }
    if (draw != -1)
    {
        for (int i = 0; i < 8; i++)
            needed[i] = needed[i] - decks[draw].cards_count[i];
    }
    if (automated != -1)
    {
        for (int i = 0; i < 8; i++)
            needed[i] = needed[i] - decks[automated].cards_count[i];
    }
    for (int i = 0; i < 8; i++)
    {
        if (needed[i] > biggest_score)
        {
            biggest_score = needed[i];
            best_card = i;
        }
    }
    return (best_card);
}

void    rank_ressources(int *ressources_rank, const vector<Application>& apps)
{
    int score[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    for (int i = 0; i < apps.size(); i++)
    {
        for (int j = 0; j < 8; j++)
            score[j] += apps[i].need[j];
    }
    for (int i = 7; i > -1; i--)
    {
        int smallest = 0;
        int smallest_score = 1000;
        for (int j = 0; j < 8; j++)
        {
            if (score[j] < smallest_score)
            {
                smallest_score = score[j];
                smallest = j;
            }
        }
        ressources_rank[i] = smallest;
    }
}

int optimized_integration(int *best_ressources, Deck hand)
{
    int card = -1;

    for (int i = 0; i < 8; i++)
    {
        if (best_ressources[i] == 5)
        {
            if (hand.cards_count[best_ressources[i]] > 1)
            {
                card = best_ressources[i];
                break;
            }
        }
        else
        {
            if (hand.cards_count[best_ressources[i]] > 0)
            {
                card = best_ressources[i];
                break;
            }
        }
    }
    return (card);
}

int count_auto_cards(Deck automated)
{
    int nb_auto = 0;

    if (automated.deck_name == "NA")
        return (nb_auto);
    for (int i = 0; i < 9; i++)
        nb_auto += automated.cards_count[i];
    cerr << nb_auto << endl;
    return (nb_auto);
}

bool    opponent_is_near(Data ennemy, int position)
{
    if (ennemy.current_location == 7)
    {
        if (position == 6 || position == 7 || position == 0)
            return (true);
    }
    else if (ennemy.current_location == 0)
    {
        if (position == 7 || position == 0 || position == 1)
            return (true);
    }
    else
    {
        if (position == ennemy.current_location - 1 || position == ennemy.current_location || position == ennemy.current_location + 1)
            return (true);
    }
    return (false);
}

int possessed_card(Deck draw, Deck hand, Deck discard, Deck played, Deck automated, int card)
{
    int nb_card = 0;

    if (draw.deck_name != "NA")
        nb_card += draw.cards_count[card];
    if (hand.deck_name != "NA")
        nb_card += hand.cards_count[card];
    if (discard.deck_name != "NA")
        nb_card += discard.cards_count[card];
    if (played.deck_name != "NA")
        nb_card += played.cards_count[card];
    if (automated.deck_name != "NA")
        nb_card += automated.cards_count[card];
    return (nb_card);
}

bool    is_release_possible(Deck hand, Deck automated, const vector<Application>& apps, int target)
{
    int ressources[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    int bonus = 0;

    for (int i = 0; i < 8; i++)
        ressources[i] += hand.cards_count[i] * 2;
    if (automated.deck_name != "NA")
    {
        for (int i = 0; i < 8; i++)
            ressources[i] += automated.cards_count[i] * 2;
        bonus += automated.cards_count[8];
    }
    bonus += hand.cards_count[8];
    ressources[BONUS] = bonus;

    for (int i = 0; i < apps.size(); i++)
    {
        int price = 0;
        cerr << "APP:";
        for (int j = 0; j < 8; j++)
        {
            cerr << " " << apps[i].need[j] << " ";
            if (ressources[j] < apps[i].need[j])
                price += apps[i].need[j] - ressources[j];
        }
        cerr << endl;
        cerr << "BONUS: " << ressources[BONUS] << endl;
        price -= ressources[BONUS];
        if (price <= target)
            return (true);
        cerr << "PRICE: " << price << endl;
    }
    return (false);
}

int get_price(const int *app_need, Deck hand, Deck automated)
{
    int ressources[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    int bonus = 0;
    int price = 0;

    for (int i = 0; i < 8; i++)
        ressources[i] += hand.cards_count[i] * 2;
    if (automated.deck_name != "NA")
    {
        for (int i = 0; i < 8; i++)
            ressources[i] += automated.cards_count[i] * 2;
        bonus += automated.cards_count[8];
    }
    bonus += hand.cards_count[8];
    ressources[BONUS] = bonus;

    for (int i = 0; i < 8; i++)
    {
        if (ressources[i] < app_need[i])
            price += app_need[i] - ressources[i];
    }
    price -= ressources[BONUS];
    return (price);
}

int main()
{
    Data    data;
    data.current_location = -1;
    data.is_first = 0;
    data.architecture_study_cards = 0;
    data.turn = 0;
    while (1)
    {
        vector<Application> apps;
        Player              player[2];
        vector<Deck>        decks;
        vector<string>      possible_moves;

        string              game_phase;
        int                 app_count;
        int                 deck_count;
        int                 possible_moves_count;

        int                 location_cards[8] = {5, 5, 5, 5, 5, 5, 5, 5};
        int                 ressources_rank[8] = {0, 0, 0, 0, 0, 0, 0, 0};

        //////////////////////////        INITIALIZATION         //////////////////////////////////

        cin >> game_phase; cin.ignore();

        //INIT APPS
        cin >> app_count; cin.ignore();
        for (int i = 0; i < app_count; i++)
        {
            Application tmp_app;

            cin >> tmp_app.app_name;
            cin >> tmp_app.id;
            for (int j = 0; j < 8; j++)
                cin >> tmp_app.need[j];
            cin.ignore();
            apps.push_back(tmp_app);
        }

        //INIT PLAYER
        for (int i = 0; i < 2; i++)
            cin >> player[i].location >> player[i].score >> player[i].permanent_daily_routine_cards >> player[i].permanent_architecture_study_cards; cin.ignore();

        //INIT DECKS
        cin >> deck_count; cin.ignore();
        for (int i = 0; i < deck_count; i++)
        {
            Deck    tmp_deck;

            cin >> tmp_deck.deck_name;
            for (int j = 0; j < 10; j++)
                cin >> tmp_deck.cards_count[j];
            cin.ignore();
            decks.push_back(tmp_deck);
        }

        //INIT MOVES
        cin >> possible_moves_count; cin.ignore();
        for (int i = 0; i < possible_moves_count; i++)
        {
            string  tmp_move;

            getline(cin, tmp_move);
            possible_moves.push_back(tmp_move);
        }

        ///////////////////////      GET INTEL       //////////////////////////

        get_remaining_location_cards(location_cards, decks, deck_count);

        rank_ressources(ressources_rank, apps);

        Deck    hand;
        Deck    draw;
        Deck    discard;
        Deck    played;
        Deck    automated;
        Deck    opponent;
        Deck    opponent_auto;

        for (int i = 0; i < deck_count; i++)
        {
            if (decks[i].deck_name == "HAND")
                hand = decks[i];
            else if (decks[i].deck_name == "DRAW")
                draw = decks[i];
            else if (decks[i].deck_name == "DISCARD")
                discard = decks[i];
            else if (decks[i].deck_name == "PLAYED_CARDS")
                played = decks[i];
            else if (decks[i].deck_name == "AUTOMATED")
                automated = decks[i];
            else if (decks[i].deck_name == "OPPONENT_CARDS")
                opponent = decks[i];
            else if (decks[i].deck_name == "OPPONENT_AUTOMATED")
                opponent_auto = decks[i];
        }

        //CHECK IF FIRST PLAYER AND GET PLAYERS DATA
        if (data.current_location == -1 && player[0].location == -1)
            data.is_first = 1;

        Data    ennemy;

        if (data.is_first == 0)
        {
            data.nb_release = player[1].score;
            if (data.current_location > player[1].location)
                data.turn++;
            data.current_location = player[1].location;
            data.daily_routine_cards = player[1].permanent_daily_routine_cards;
            data.architecture_study_cards = player[1].permanent_architecture_study_cards;

            ennemy.nb_release = player[0].score;
            ennemy.current_location = player[0].location;
            ennemy.daily_routine_cards = player[0].permanent_daily_routine_cards;
            ennemy.architecture_study_cards = player[0].permanent_architecture_study_cards;
            ennemy.is_first = 1;
        }
        else
        {
            data.nb_release = player[0].score;
            if (data.current_location > player[0].location)
                data.turn++;
            data.current_location = player[0].location;
            data.daily_routine_cards = player[0].permanent_daily_routine_cards;
            data.architecture_study_cards = player[0].permanent_architecture_study_cards;

            ennemy.nb_release = player[1].score;
            ennemy.current_location = player[1].location;
            ennemy.daily_routine_cards = player[1].permanent_daily_routine_cards;
            ennemy.architecture_study_cards = player[1].permanent_architecture_study_cards;
            ennemy.is_first = 0;
        }
        ///////////////////////      EXECUTION       //////////////////////////


        cerr << endl << game_phase << endl;
        //MOVE
        if (game_phase == "MOVE")
        {
            int movevement = -1;
            int card_num = -1;

            //FIRST_TURN FIRST
            // if (data.turn == 0 /*&& data.is_first == 1*/)
            // {
            //     if (data.current_location == -1)
            //         move = 2;
            //     else if (data.current_location == 2)
            //     {
            //         move = 3;
            //         card_num = 4;
            //     }
            //     else if (data.current_location == 3)
            //     {
            //         move = 4;
            //         card_num = 5;
            //     }
            //     else if (data.current_location == 4)
            //     {
            //         move = 5;
            //         card_num = 5;
            //     }
            //     else if (data.current_location == 5)
            //     {
            //         move = 6;
            //         card_num = 5;
            //     }
            //     else if (data.current_location == 6)
            //     {
            //         move = 7;
            //         card_num = 6;
            //     }
            // }
            // //FIRST_TURN SECOND
            // // else if (data.turn == 0 && data.is_first != 1)
            // // {

            // // }
            // //OTHER_TURNS
            // else
            // {
            if (data.current_location == -1)
                movevement = 2;
            else if (data.current_location == 2 && ennemy.current_location == 4 && possessed_card(draw, hand, discard, played, automated, 5) < 3 && count_auto_cards(automated) < 3)
                movevement = 6;
            else if (data.current_location != 4 &&  data.current_location != 5 && possessed_card(draw, hand, discard, played, automated, 5) < 3 && count_auto_cards(automated) < 3 && possessed_card(draw, hand, discard, played, automated, BONUS) > 3)
                movevement = 4;
            else if (data.current_location != 5 && possessed_card(draw, hand, discard, played, automated, 5) < 3 && count_auto_cards(automated) < 3 && possessed_card(draw, hand, discard, played, automated, BONUS) > 3)
                movevement = 5;
            else if (data.current_location != 6 && possessed_card(draw, hand, discard, played, automated, 5) < 3 && count_auto_cards(automated) < 3 && possessed_card(draw, hand, discard, played, automated, BONUS) > 3)
                movevement = 6;
            else
            {
                if (data.current_location == 4 || data.current_location == 5 || data.current_location == 6 || data.current_location == 7)
                {
                    if (data.current_location == 4 && !opponent_is_near(ennemy, 5))
                        movevement = 5;
                    else if (data.current_location == 5 && !opponent_is_near(ennemy, 6))
                        movevement = 6;
                    else if (data.current_location == 6 && !opponent_is_near(ennemy, 7))
                        movevement = 7;
                    else if (!opponent_is_near(ennemy, 0))
                        movevement = 0;
                    else if (!opponent_is_near(ennemy, 1))
                        movevement = 1;
                    else if (!opponent_is_near(ennemy, 2))
                        movevement = 2;
                    else if (!opponent_is_near(ennemy, 3))
                        movevement = 3;
                }
                else
                {
                    for (int i = data.current_location + 1; i < 8; i++)
                    {
                        if (!opponent_is_near(ennemy, i))
                        {
                            movevement = i;
                            break;
                        }
                    }
                }
            }
            cerr << "Mov: " << movevement << " Daily: " << data.daily_routine_cards << " location_card = " << location_cards[5] << " " << possessed_card(draw, hand, discard, played, automated, 5) << " " << count_auto_cards(automated) << endl;
            if ((movevement == 4 || movevement == 5 || movevement == 6) && data.daily_routine_cards > 0 && location_cards[5] > 0 && possessed_card(draw, hand, discard, played, automated, 5) < 3 && count_auto_cards(automated) < 3)
                card_num = 5;
            else if (movevement == 3 && location_cards[4] > 0 && data.architecture_study_cards < 1 && data.daily_routine_cards > 0)
                card_num = 4;
            cerr << card_num << endl;
            // if (movevement != -1 && data.daily_routine_cards > 0)
            //     cout << "MOVE " << movevement << " " << card_num << endl;
            if (movevement != -1 && card_num != -1 && data.daily_routine_cards != 0)
                cout << "MOVE " << movevement << " " << card_num << endl;
            else if (movevement != -1)
                cout << "MOVE " << movevement << endl;
            else
                cout << "RANDOM" << endl;
            data.current_location = movevement;
        }
        //GIVE
        else if (game_phase == "GIVE_CARD")
        {
            int card = -1;

            card = best_to_give(apps, decks, deck_count);
            if (is_in_hand(decks, deck_count, BONUS))
                cout << "GIVE " << BONUS << endl;
            else if (card == -1)
                cout << "RANDOM" << endl;
            else
                cout << "GIVE " << card << endl;
        }
        //THROW
        else if (game_phase == "THROW_CARD")
        {
            int card = -1;

            card = best_to_give(apps, decks, deck_count);
            if (is_in_hand(decks, deck_count, BONUS))
                cout << "THROW " << BONUS << endl;
            else if (card == -1)
                cout << "RANDOM" << endl;
            else
                cout << "THROW " << card << endl;
        }
        //PLAY
        else if (game_phase == "PLAY_CARD")
        {
            if (data.nb_release < 4)
            {
                cerr << is_release_possible(hand, automated, apps, 4) << endl;
                if (is_in_hand(decks, deck_count, CONTINUOUS_INTEGRATION) && is_in_hand(decks, deck_count, BONUS))
                    cout << "CONTINUOUS_INTEGRATION 8" << endl;
                else if (is_in_hand(decks, deck_count, CONTINUOUS_INTEGRATION) && optimized_integration(ressources_rank, hand) > 0)
                    cout << "CONTINUOUS_INTEGRATION " << optimized_integration(ressources_rank, hand) << endl;
                else if (is_in_hand(decks, deck_count, TRAINING))
                    cout << "TRAINING" << endl;
                else if (is_in_hand(decks, deck_count, CODING))
                    cout << "CODING" << endl;
                else if (is_in_hand(decks, deck_count, REFACTORING) && is_in_hand(decks, deck_count, DEBT))
                    cout << "REFACTORING" << endl;
                else if (is_in_hand(decks, deck_count, CODE_REVIEW))
                    cout << "CODE_REVIEW" << endl;
                else if (is_in_hand(decks, deck_count, ARCHITECTURE_STUDY))
                    cout << "ARCHITECTURE_STUDY" << endl;
                else if (is_in_hand(decks, deck_count, DAILY_ROUTINE) && data.daily_routine_cards < 2)
                    cout << "DAILY_ROUTINE" << endl;
                else
                    cout << "WAIT" << endl;
            }
            else
            {
                if (is_in_hand(decks, deck_count, CONTINUOUS_INTEGRATION) && is_in_hand(decks, deck_count, BONUS) && !is_release_possible(hand, automated, apps, 0))
                    cout << "CONTINUOUS_INTEGRATION 8" << endl;
                else if (is_in_hand(decks, deck_count, CONTINUOUS_INTEGRATION) && optimized_integration(ressources_rank, hand) > 0 && !is_release_possible(hand, automated, apps, 0))
                    cout << "CONTINUOUS_INTEGRATION " << optimized_integration(ressources_rank, hand) << endl;
                else if (is_in_hand(decks, deck_count, TRAINING) && !is_release_possible(hand, automated, apps, 0))
                    cout << "TRAINING" << endl;
                else if (is_in_hand(decks, deck_count, CODING) && !is_release_possible(hand, automated, apps, 0))
                    cout << "CODING" << endl;
                else if (is_in_hand(decks, deck_count, REFACTORING) && is_in_hand(decks, deck_count, DEBT) && !is_release_possible(hand, automated, apps, 0))
                    cout << "REFACTORING" << endl;
                else if (is_in_hand(decks, deck_count, CODE_REVIEW) && !is_release_possible(hand, automated, apps, 0))
                    cout << "CODE_REVIEW" << endl;
                else if (is_in_hand(decks, deck_count, ARCHITECTURE_STUDY) && !is_release_possible(hand, automated, apps, 0))
                    cout << "ARCHITECTURE_STUDY" << endl;
                else if (is_in_hand(decks, deck_count, DAILY_ROUTINE) && data.daily_routine_cards < 2 && !is_release_possible(hand, automated, apps, 0))
                    cout << "DAILY_ROUTINE" << endl;
                else
                    cout << "WAIT" << endl;
            }
        }
        //RELEASE
        // else if (game_phase == "RELEASE")
        // {
        //     if (count_auto_cards(automated) < 3 && data.nb_release != 4)
        //             cout << "WAIT" << endl;
        //     else if (data.nb_release < 4 && is_release_possible(hand, automated, apps, 2))
        //     {
        //         int id = -1;

        //         for (int i = 0; i < apps.size(); i++)
        //         {
        //             if (is_possible_move(possible_moves, "RELEASE " + to_string(apps[i].id)))
        //             {
        //                 id = apps[i].id;
        //                 break;
        //             }
        //         }
        //         if (id != -1)
        //             cout << "RELEASE " + to_string(id) << endl;
        //         else
        //             cout << "WAIT" << endl;
        //     }
        //     else
        //     {
        //         int id = -1;

        //         for (int i = 0; i < apps.size(); i++)
        //         {
        //             if (is_possible_move(possible_moves, "RELEASE " + to_string(apps[i].id)))
        //             {
        //                 id = apps[i].id;
        //                 break;
        //             }
        //         }
        //         if (id != -1)
        //             cout << "RELEASE " + to_string(id) << endl;
        //         else
        //             cout << "WAIT" << endl;
        //     }
        // }
        else if (game_phase == "RELEASE")
        {
            int release_id = -1;

            if (count_auto_cards(automated) < 3 && data.nb_release != 4)
                    cout << "WAIT" << endl;
            else if (data.nb_release < 4)
            {
                for (int i = 0; i < apps.size(); i++)
                {
                    if (get_price(apps[i].need, hand, automated) < 2)
                    {
                        release_id = apps[i].id;
                        break;
                    }
                }
                if (release_id != -1)
                    cout << "RELEASE " << release_id << endl;
                else
                    cout << "WAIT" << endl;
            }
            else
            {
                int id = -1;

                for (int i = 0; i < apps.size(); i++)
                {
                    if (is_possible_move(possible_moves, "RELEASE " + to_string(apps[i].id)))
                    {
                        id = apps[i].id;
                        break;
                    }
                }
                if (id != -1)
                    cout << "RELEASE " + to_string(id) << endl;
                else
                    cout << "WAIT" << endl;
            }
        }
        else
            cout << "RANDOM" << endl;
    }
}
