#pragma once
#include <ostream>
#include "player.h"
#include "perceptron.h"

/**
 * This is example player, that plays absolutely randomly.
 */
class RandomPlayer : public Player {
    std::string _name;
public:
    RandomPlayer(const std::string& name): _name(name) {}
    std::string get_name() const override { return _name; }
    Point play(const GameView& game) override;
    void assign_mark(Mark player_mark) override { /*does nothing*/ }
    void notify(const GameView&, const Event&) override { /*does nothing*/ }
};

class MyPlayer : public Player {
    std::string _name;
public:
    MyPlayer(const std::string& name) : _name(name) {};
    std::string get_name() const override { return _name; }
    Point play(const GameView& game) override;
    void assign_mark(Mark player_mark) override {};
    void notify(const GameView&, const Event&) override {};
    double calculateWeight(const Point&, const GameView&);
    bool isWithinBoundary(const Point&, const Boundary&);
};

//this player places dots in rows sequentially
class SillyPlayer : public Player {
    std::string _name;
public:
    SillyPlayer(const std::string& name) : _name(name) {};
    std::string get_name() const override { return _name; }
    Point play(const GameView& game) override;
    void assign_mark(Mark player_mark) override {};
    void notify(const GameView&, const Event&) override {};
};

//this is player with manual control
class HandPlayer : public Player {
    std::string _name;
public:
    HandPlayer(const std::string& name) :_name(name) {};
    std::string get_name() const override { return _name; }
    Point play(const GameView& game) override;
    void assign_mark(Mark player_mark) override {};
    void notify(const GameView&, const Event&) override {};
};



/**
 * Simple observer, that logs every event in given output stream.
 */
class BasicObserver : public Observer {
    std::ostream& _out;

    std::ostream& _print_mark(Mark mark);
public:
    BasicObserver(std::ostream& out_stream): _out(out_stream) {}
    void notify(const GameView&, const Event& event) override;
};
