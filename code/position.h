#pragma once

struct Position{
    Position(int r = 0, int c = 0) : row(r), col(c){}
    
    int row;
    int col;

    bool operator==(const Position& other) const {
        return (row == other.row) && (col == other.col);
    }

    bool operator!=(const Position& other) const {
        return !(*this == other);
    }

    Position operator+(const Position& other) const {
        return Position(row + other.row, col + other.col);
    }

    Position& operator+=(const Position& other){
        this->col += other.col;
        this->row += other.row;
        return *this;
    }
};