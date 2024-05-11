/**
 *  This is example of game, that you can use for debugging.
 */
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "my_player.h"
#include "game_engine.h"


int main() {
    srand(time(NULL));

    /// Creates observer to log every event to std::cout
    BasicObserver obs(std::cout);

    /// Create setting for game
    GameSettings settings = {
        .field_size = {
            .min = {.x = -19, .y = -19},
            .max = {.x = 19, .y = 19},
        },
        .max_moves = 0,
        .win_length = 5,
        .move_timelimit_ms = 50,
    };
    
    int countS = 0;
    int countR = 0;
    int cc = 1000;

    std::cout << "Random vs My" << std::endl;
    for (int i = 0; i < cc; i++) {
        /// Creates game engine with given settings
        GameEngine game(settings);

        /// Adds observer to the view of this game engine
        //game.get_view().add_observer(obs);
        /// Creates first player
        RandomPlayer player1("Vasya"); //X
        /// Adds it to play Xs
        game.set_player(player1);
        /// Create second player
        MyPlayer player2("Kolya"); //O
        /// Adds it to play Os
        game.set_player(player2);
        /// Starts game until someone wins.
        game.play_game();
        if (game.play_game() == Mark::Cross) { countS++; }
        if (game.play_game() == Mark::Zero) { countR++; }
    }
    //statistics
    std::cout <<"Cross: "<< countS << std::endl;
    std::cout << "Zero: " << countR << std::endl;
    std::cout<<std::endl;

    countS = 0;
    countR = 0;
    std::cout << "My vs Random" << std::endl;
    for (int i = 0; i < cc; i++) {
        /// Creates game engine with given settings
        GameEngine game(settings);

        /// Adds observer to the view of this game engine
        //game.get_view().add_observer(obs);
        /// Creates first player
        MyPlayer player1("Vasya"); //X
        /// Adds it to play Xs
        game.set_player(player1);
        /// Create second player
        RandomPlayer player2("Kolya"); //O
        /// Adds it to play Os
        game.set_player(player2);
        /// Starts game until someone wins.
        game.play_game();
        if (game.play_game() == Mark::Cross) { countS++; }
        if (game.play_game() == Mark::Zero) { countR++; }
    }
    //statistics
    std::cout << "Cross: " << countS << std::endl;
    std::cout << "Zero: " << countR << std::endl;
    std::cout << std::endl;

    countS = 0;
    countR = 0;
    std::cout << "Silly vs My" << std::endl;
    for (int i = 0; i < cc; i++) {
        /// Creates game engine with given settings
        GameEngine game(settings);

        /// Adds observer to the view of this game engine
        //game.get_view().add_observer(obs);
        /// Creates first player
        SillyPlayer player1("Vasya"); //X
        /// Adds it to play Xs
        game.set_player(player1);
        /// Create second player
        MyPlayer player2("Kolya"); //O
        /// Adds it to play Os
        game.set_player(player2);
        /// Starts game until someone wins.
        game.play_game();
        if (game.play_game() == Mark::Cross) { countS++; }
        if (game.play_game() == Mark::Zero) { countR++; }
    }
    //statistics
    std::cout << "Cross: " << countS << std::endl;
    std::cout << "Zero: " << countR << std::endl;
    std::cout << std::endl;

    countS = 0;
    countR = 0;
    std::cout << "My vs Silly" << std::endl;
    for (int i = 0; i < cc; i++) {
        /// Creates game engine with given settings
        GameEngine game(settings);

        /// Adds observer to the view of this game engine
        //game.get_view().add_observer(obs);
        /// Creates first player
        MyPlayer player1("Vasya"); //X
        /// Adds it to play Xs
        game.set_player(player1);
        /// Create second player
        SillyPlayer player2("Kolya"); //O
        /// Adds it to play Os
        game.set_player(player2);
        /// Starts game until someone wins.
        game.play_game();
        if (game.play_game() == Mark::Cross) { countS++; }
        if (game.play_game() == Mark::Zero) { countR++; }
    }
    //statistics
    std::cout << "Cross: " << countS << std::endl;
    std::cout << "Zero: " << countR << std::endl;
    std::cout << std::endl;

    countS = 0;
    countR = 0;
    std::cout << "Hand vs My" << std::endl;
    for (int i = 0; i < cc; i++) {
        /// Creates game engine with given settings
        GameEngine game(settings);

        /// Adds observer to the view of this game engine
        game.get_view().add_observer(obs);
        /// Creates first player
        MyPlayer player1("Vasya"); //X
        /// Adds it to play Xs
        game.set_player(player1);
        /// Create second player
        HandPlayer player2("Kolya"); //O
        /// Adds it to play Os
        game.set_player(player2);
        /// Starts game until someone wins.
        game.play_game();
        if (game.play_game() == Mark::Cross) { countS++; }
        if (game.play_game() == Mark::Zero) { countR++; }
    }
    //statistics
    std::cout << "Cross: " << countS << std::endl;
    std::cout << "Zero: " << countR << std::endl;
    return 0;
}
