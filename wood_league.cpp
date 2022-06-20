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

Deck::Deck(){}
Deck::~Deck(){}

void    print_remaining_location_cards(int *location_cards);
void    print_price(Application *apps, int *ressources, int app_count);


//         //TESTS//////////////////////////////////////////////////////////////////////////////////
//         cerr << game_phase << endl;
//         for (int i = 0; i < possible_moves_count; i++)
//             cerr << possible_move[i] << endl;
//         cerr << endl << endl << endl;
//         for (int i = 0; i < app_count; i++)
//             cerr << "TRAINING " << apps[i].need[0] << ", CODING " << apps[i].need[1] << ", DAILY_ROUTINE " << apps[i].need[2] << ", TASK_PRIORITIZATION " << apps[i].need[3] << ", ARCHITECTURE_STUDY " << apps[i].need[4] << ", CONITNUOUS_INTEGRATION " << apps[i].need[5] << ", CODE_REVIEW " << apps[i].need[6] << ", REFACTORING " << apps[i].need[7] << endl;;
//     }
// }


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

/////////////     FUNCTIONS      /////////////
int get_price(int *app_need, int *ressources)
{
    int price = 0;

    for (int i = 0; i < 8; i++)
    {
        if (app_need[i] > ressources[i])
            price += app_need[i] - ressources[i] * 2;
    }
    price -= ressources[BONUS];
    return (price);
}

int best_possible_release(vector<Application> apps, vector<Deck> decks, int app_count, int deck_count, int tolerated_debt)
{
    int my_deck;
    int price;
    int best_price = 1000;
    int id_app = WAIT;

    for (int i = 0; i < deck_count; i++)
    {
        if (decks[i].deck_name == "HAND")
            my_deck = i;
    }
    for (int i = 0; i < app_count; i++)
    {
        price = get_price(apps[i].need, decks[my_deck].cards_count);
        if (price <= tolerated_debt && price < best_price)
        {
            best_price = price;
            id_app = apps[i].id;
        }
    }
    return (id_app);
}

void    get_remaining_location_cards(int *location_cards, vector<Deck> decks, int deck_count)
{
    for (int i = 0; i < deck_count; i++)
    {
        for (int j = 0; j < 8; j++)
            location_cards[j] -= decks[i].cards_count[j];
    }
}

int get_best_move(vector<Application> apps, int app_count, vector<Deck> decks, int deck_count, int *location_cards, int best_rank)
{
    int needed[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int move = -1;
    int move2 = -1;
    int biggest_need = 0;
    int second_biggest_need = 0;

    for (int i = 0; i < app_count; i++)
    {
        for (int j = 0; j < 8; j++)
            needed[j] += apps[i].need[j];
    }
    for (int i = 0; i < 8; i++)
    {
        if (needed[i] > biggest_need && location_cards[i] > 0 && !is_in_hand(decks, deck_count, i))
        {
            biggest_need = needed[i];
            move2 = move;
            move = i;
        }
    }
    if (best_rank == 1)
        return (move);
    else
        return (move2);
}

int main()
{
    int                 current_location = -1;
    int                 nb_release = 0;
    int                 daily_routine_cards = 0;
    int                 architecture_study_cards = 0;
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
        vector<int>         apps_rank;

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
                         //print_remaining_location_cards(location_cards);

        ///////////////////////      EXECUTION       //////////////////////////

        //MOVE
        if (game_phase == "MOVE")
        {
            int move;
            int rank = 1;

            move = get_best_move(apps, app_count, decks, deck_count, location_cards, rank);
            if (move == -1)
                cout << "RANDOM" << endl;
            else
            {
                if (current_location == move)
                {
                    rank++;
                    move = get_best_move(apps, app_count, decks, deck_count, location_cards, rank);
                }
                current_location = move;
                if (move == -1)
                    cout << "RANDOM" << endl;
                else
                    cout << "MOVE " << move << endl;
            }
        }
        //RELEASE
        else if (game_phase == "RELEASE")
        {
            int release = -1;

            if (release < 5)
                release = best_possible_release(apps, decks, app_count, deck_count, 2);
            else
                release = best_possible_release(apps, decks, app_count, deck_count, 0);
            if (release != WAIT)
            {
                cout << "RELEASE " << release << endl;
                nb_release++;
            }
            else
                cout << "WAIT" << endl;
        }
        //UNEXPECTED CASES
        else
            cout << "RANDOM" << endl;
    }
}

/////////////     TESTEURS       /////////////
void    print_remaining_location_cards(int *location_cards)
{
    for (int i = 0; i < 8; i++)
        cerr << i << ":  " << location_cards[i] << endl;
}

void    print_price(Application *apps, int *ressources, int app_count)
{
    for (int i = 0; i < app_count; i++)
        cerr << "Index: " << i << " / Price: " << get_price(apps[i].need, ressources) << endl;
    cerr << endl << endl << endl;
}
