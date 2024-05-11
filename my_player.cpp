#include "my_player.h"
#include <cstdlib>
#include <algorithm> 
#include "vector"
#include <iostream>
using namespace std;

static field_index_t rand_int(field_index_t min, field_index_t max) {
    return min + rand() % (max - min + 1);
}

Point RandomPlayer::play(const GameView& game) {
    Boundary b = game.get_settings().field_size;
    Point result;
    do {
        result = {
            .x = rand_int(b.min.x, b.max.x),
            .y = rand_int(b.min.y, b.max.y),
        };
    } while(game.get_state().field->get_value(result) != Mark::None);
    return result;
}

void BasicObserver::notify(const GameView&, const Event& event) {
    if (event.get_type() == MoveEvent::TYPE) {
        auto &data = get_data<MoveEvent>(event);
        _out << "Move:\tx = " <<  data.point.x 
            << ",\ty = " << data.point.y << ":\t";
        _print_mark(data.mark) << '\n';
        return;
    }
    if (event.get_type() == PlayerJoinedEvent::TYPE) {
        auto &data = get_data<PlayerJoinedEvent>(event);
        _out << "Player '" << data.name << "' joined as ";
        _print_mark(data.mark) << '\n';
        return;
    }
    if (event.get_type() == GameStartedEvent::TYPE) {
        _out << "Game started\n";
        return;
    }
    if (event.get_type() == WinEvent::TYPE) {
        auto &data = get_data<WinEvent>(event);
        _out << "Player playing ";
        _print_mark(data.winner_mark) << " has won\n";
        return;
    }
    if (event.get_type() == DrawEvent::TYPE) {
        auto &data = get_data<DrawEvent>(event);
        _out << "Draw happened, reason: '" << data.reason << "'\n";
        return;
    }
    if (event.get_type() == DisqualificationEvent::TYPE) {
        auto &data = get_data<DisqualificationEvent>(event);
        _out << "Player playing ";
        _print_mark(data.player_mark) << " was disqualified, reason: '" 
            << data.reason << "'\n";
        return;
    }
}

std::ostream& BasicObserver::_print_mark(Mark m) {
    if (m == Mark::Cross) return _out << "X";
    if (m == Mark::Zero) return _out << "O";
    return _out << "?";
}

Point SillyPlayer::play(const GameView& game) {
    Point result{};
    Boundary b = game.get_settings().field_size;
    for (int row = b.min.x; row < b.max.x; row++) {
        for (int col = b.min.y; col < b.max.y; col++) {
            Point cur = { .x = row, .y = col, };
            if (game.get_state().field->get_value(cur) == Mark::None) {
                result = { .x = row, .y = col, };
            }
            else { continue; }
        }
    }
    return result;
}

Point HandPlayer::play(const GameView& game) {
    Point result{};
    Boundary b = game.get_settings().field_size;
    int row, col;
    cin >> row >> col;
    if (row <= b.max.x && col <= b.max.y && row >= b.min.x && col >= b.min.y) {
        result = { .x = row, .y = col, };
    }
    return result;
}



Point MyPlayer::play(const GameView& game) {
    Point result{};
    Boundary b = game.get_settings().field_size;
    int cRow = (b.min.x + b.max.x) / 2;
    int cCol = (b.min.y + b.max.y) / 2;
    Point center = { .x = cRow, .y = cCol };
    if (game.get_state().field->get_value(center) == Mark::None) {
        return center;
    }

    vector <Point> potenMoves; //массив потенциальных ходов
    for (int row = b.min.x; row <= b.max.x; row++) {
        for (int col = b.min.y; col <= b.max.y; col++) {
            Point cur = { .x = row, .y = col };
            if (game.get_state().field->get_value(cur) != Mark::None) {
                continue;
            }
            potenMoves.push_back(cur); //добавляем в конец
        }
    }
    
    vector<double> weights(potenMoves.size(), 0.0); //массив весов
    int psize = potenMoves.size();
    for (int i = 0; i < psize; i++) {
        Point temp = potenMoves[i];
        double weight = calculateWeight(temp, game);
        weights[i] = weight;
    }

    double maxWeight = -1.0;
    int maxWeightIndex = -1;
    for (int i = 0; i < weights.size(); i++) {
        if (weights[i] > maxWeight) {
            maxWeight = weights[i];
            maxWeightIndex = i;
        }
    }
    result = potenMoves[maxWeightIndex];
    return result;
}
bool MyPlayer::isWithinBoundary(const Point& point, const Boundary& boundary) {
    return (point.x >= boundary.min.x && point.x <= boundary.max.x &&
        point.y >= boundary.min.y && point.y <= boundary.max.y);
}


//рабочая версия

double MyPlayer::calculateWeight(const Point& temp, const GameView& game) {
    double weight = 0.0;
    double maxWeight = 0.0;
    Mark my = game.get_state().current_move;
    Mark opponent = (my == Mark::Cross ? Mark::Zero : Mark::Cross);
    vector<Point> dir = { {1,0}, {0,1}, {1,1}, {1,-1} };
    for (const auto& direct : dir) {
        int row = temp.x;
        int col = temp.y;
        int count = 0;
        int opponentCount = 0;
        bool openEnd = true;
        bool opponOpenEnd = true;

        for (int i = 0; i <= 4; ++i) {
            int curR = row + direct.x * i;
            int curC = col + direct.y * i;
            Point cur = { .x = curR, .y = curC };
            if (isWithinBoundary(cur, game.get_settings().field_size)) {
                Mark value = game.get_state().field->get_value(cur);
                if (value == my) {
                    count++;
                    opponOpenEnd = false;
                }
                else if (value == opponent) {
                    opponentCount++;
                    openEnd = false;
                    opponOpenEnd = true;
                }
                else {
                    if (count > 0) { openEnd = true; }
                    if (opponentCount > 0) { opponOpenEnd = true; }
                }
            }
            else {
                openEnd = false;
                opponOpenEnd = false;
                break;
            }
        }

        for (int i = 0; i <= 4; i++) {
            int curR = row - direct.x * i;
            int curC = col - direct.y * i;
            Point cur = { .x = curR, .y = curC };
            if (isWithinBoundary(cur, game.get_settings().field_size)) {
                Mark value = game.get_state().field->get_value(cur);
                if (value == my) {
                    count++;
                    opponOpenEnd = false;
                }
                else if (value == opponent) {
                    opponentCount++;
                    openEnd = false;
                    opponOpenEnd = true;
                }
                else {
                    if (count > 0) { openEnd = true; }
                    if (opponentCount > 0) { opponOpenEnd = true; }
                }
            }
            else {
                openEnd = false;
                opponOpenEnd = false;
                break;
            }
        }

        if (count == 4 && openEnd) {
            maxWeight = max(1.0, maxWeight); //открытая четверка
        }
        else if (count == 4 && !openEnd) {
            maxWeight = max(0.8, maxWeight); //полузакрытая четверка (атака)
        }
        else if (opponentCount == 4 && !opponOpenEnd) {
            maxWeight = max(0.75, maxWeight); //полузакрытая четверка противника (защита)
        }
        else if (count == 3 && openEnd) {
            maxWeight = max(0.6, maxWeight); //открытая тройка (атака)
        }
        else if (opponentCount == 3 && opponOpenEnd) {
            maxWeight = max(0.7, maxWeight); //открытая тройка противника (защита)
        }
        else if (opponentCount == 3 && !opponOpenEnd) {
            maxWeight = max(0.65, maxWeight); //полузакрытые тройки противника (защита)
        }
        else if (count == 3 && !openEnd) {
            maxWeight = max(0.5, maxWeight); //полузакрытые тройки (атака)
        }
        else if (count == 2 && openEnd) {
            maxWeight = max(0.4, maxWeight); //открытые двойки (атака)
        }
        else if (opponentCount == 2 && opponOpenEnd) {
            maxWeight = max(0.35, maxWeight); //открытые двойки противника (защита)
        }
        else if (count == 2 && !openEnd) {
            maxWeight = max(0.3, maxWeight); //полузакрытые двойки
        }
        else if (count == 1 && openEnd) {
            maxWeight = max(0.2, maxWeight); //одиночная фигура
        }
        else if (opponentCount == 1) {
            maxWeight = max(0.1, maxWeight);
        }
        weight = max(weight, maxWeight);
    }
       
    return weight;
}

