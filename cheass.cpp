#include <cmath>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

enum class Color {
    white,
    black
};

enum class Type {
    king,
    queen,
    rooke,
    bishop,
    knight,
    pawn
};

// Классы фигур (упрощённые реализации проверок перемещений)
class King {
private:
    Type name;
    int x;
    int y;
    Color color;
public:
    King(Type name, int x, int y, Color color) {
        this->name = name;
        this->x = x;
        this->y = y;
        this->color = color;
    }
    bool king_move(int x1, int y1, int x2_new, int y2_new) {
        return (abs(x2_new - x1) <= 1 && abs(y2_new - y1) <= 1);
    }
};

class Queen {
private:
    Type name;
    int x;
    int y;
    Color color;
public:
    Queen(Type name, int x, int y, Color color) {
        this->name = name;
        this->x = x;
        this->y = y;
        this->color = color;
    }
    inline bool queen_move(int x1, int y1, int x2_new, int y2_new) {
        // Вертикальный, горизонтальный или диагональный ход
        return ((x1 == x2_new || y1 == y2_new) || (abs(x2_new - x1) == abs(y2_new - y1)));
    }
};

class Rooke {
private:
    Type name;
    int x;
    int y;
    Color color;
public:
    Rooke(Type name, int x, int y, Color color) {
        this->name = name;
        this->x = x;
        this->y = y;
        this->color = color;
    }
    inline bool rooke_move(int x1, int y1, int x2_new, int y2_new) {
        return (x1 == x2_new || y1 == y2_new);
    }
};

class Bishop {
private:
    Type name;
    int x;
    int y;
    Color color;  
public:
    Bishop(Type name, int x, int y, Color color) {
        this->name = name;
        this->x = x;
        this->y = y;
        this->color = color;
    }
    inline bool bishop_move(int x1, int y1, int x2_new, int y2_new) {
        return (abs(x2_new - x1) == abs(y2_new - y1));
    }        
};

class Knight {
private:
    Type name;
    int x;
    int y;
    Color color;
public:
    Knight(Type name, int x, int y, Color color) {
        this->name = name;
        this->x = x;
        this->y = y;
        this->color = color;
    }
    inline bool knights_move(int x1, int y1, int x2_new, int y2_new) {
        return ((abs(x2_new - x1) == 2 && abs(y2_new - y1) == 1) ||
                (abs(x2_new - x1) == 1 && abs(y2_new - y1) == 2));
    }
};

class Pawn {
private:
    Type name;
    int x;
    int y;
    Color color;
public:
    Pawn(Type name, int x, int y, Color color) {
        this->name = name;
        this->x = x;
        this->y = y;
        this->color = color;
    }
    inline bool pawn_move(int x1, int y1, int x2_new, int y2_new) {
        if (x1 != x2_new)
            return false;
        if (color == Color::white) {
            if (y1 == 2 && y2_new == 4)
                return true;
            else if (y2_new == y1 + 1)
                return true;
            else 
                return false;
        } else { // black
            if (y1 == 7 && y2_new == 5)
                return true;
            else if (y2_new == y1 - 1)
                return true;
            else 
                return false;
        }
    }
    inline bool pawn_attack(int x1, int y1, int x2_new, int y2_new) {
        if (abs(x2_new - x1) == 1) {
            if (color == Color::white && y2_new == y1 + 1)
                return true;
            else if (color == Color::black && y2_new == y1 - 1)
                return true;
            else 
                return false;
        }
        return false;
    }
};

// Глобальное представление доски (8x8)
std::vector<std::vector<std::string>> board(8, std::vector<std::string>(8, " "));

// Глобальные флаги рокировки
bool whiteKingMoved = false, whiteRookKingsideMoved = false, whiteRookQueensideMoved = false;
bool blackKingMoved = false, blackRookKingsideMoved = false, blackRookQueensideMoved = false;

// Функция для вывода доски
void printBoard() {
    for (int i = 0; i < 8; i++){
        cout << 8 - i << " ";
        for (int j = 0; j < 8; j++){
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
    cout << "  a b c d e f g h" << endl;
}

// Функция проверки свободного пути для скользящих фигур
bool pathClear(int srcRow, int srcCol, int destRow, int destCol) {
    int dRow = destRow - srcRow;
    int dCol = destCol - srcCol;
    int stepRow = (dRow == 0 ? 0 : dRow / abs(dRow));
    int stepCol = (dCol == 0 ? 0 : dCol / abs(dCol));
    int currRow = srcRow + stepRow;
    int currCol = srcCol + stepCol;
    while (currRow != destRow || currCol != destCol) {
        if (board[currRow][currCol] != " ")
            return false;
        currRow += stepRow;
        currCol += stepCol;
    }
    return true;
}

// Функция-помощник для проверки, может ли фигура атаковать заданную клетку.
// Внимание: для перевода между индексами доски и шахматными координатами используется формула: rank = 8 - row.
bool canPieceAttack(int srcRow, int srcCol, int destRow, int destCol, const string &piece) {
    int srcRank = 8 - srcRow;
    int destRank = 8 - destRow;
    // Для белых фигур
    if(piece == "\u2654") { // белый король
         King king(Type::king, srcCol, srcRank, Color::white);
         return king.king_move(srcCol, srcRank, destCol, destRank);
    } else if(piece == "\u2655") { // белая ферзь
         Queen queen(Type::queen, srcCol, srcRank, Color::white);
         if(!((srcCol == destCol || srcRank == destRank) || (abs(destCol - srcCol) == abs(destRank - srcRank))))
             return false;
         return pathClear(srcRow, srcCol, destRow, destCol);
    } else if(piece == "\u2656") { // белая ладья
         Rooke rooke(Type::rooke, srcCol, srcRank, Color::white);
         if(srcCol == destCol || srcRank == destRank)
             return pathClear(srcRow, srcCol, destRow, destCol);
         return false;
    } else if(piece == "\u2657") { // белый слон
         Bishop bishop(Type::bishop, srcCol, srcRank, Color::white);
         if(abs(destCol - srcCol) == abs(destRank - srcRank))
             return pathClear(srcRow, srcCol, destRow, destCol);
         return false;
    } else if(piece == "\u2658") { // белый конь
         Knight knight(Type::knight, srcCol, srcRank, Color::white);
         return knight.knights_move(srcCol, srcRank, destCol, destRank);
    } else if(piece == "\u2659") { // белая пешка (атака)
         Pawn pawn(Type::pawn, srcCol, srcRank, Color::white);
         return pawn.pawn_attack(srcCol, srcRank, destCol, destRank);
    }
    // Для чёрных фигур
    else if(piece == "\u265A") { // чёрный король
         King king(Type::king, srcCol, srcRank, Color::black);
         return king.king_move(srcCol, srcRank, destCol, destRank);
    } else if(piece == "\u265B") { // чёрная ферзь
         Queen queen(Type::queen, srcCol, srcRank, Color::black);
         if(!((srcCol == destCol || srcRank == destRank) || (abs(destCol - srcCol) == abs(destRank - srcRank))))
             return false;
         return pathClear(srcRow, srcCol, destRow, destCol);
    } else if(piece == "\u265C") { // чёрная ладья
         Rooke rooke(Type::rooke, srcCol, srcRank, Color::black);
         if(srcCol == destCol || srcRank == destRank)
             return pathClear(srcRow, srcCol, destRow, destCol);
         return false;
    } else if(piece == "\u265D") { // чёрный слон
         Bishop bishop(Type::bishop, srcCol, srcRank, Color::black);
         if(abs(destCol - srcCol) == abs(destRank - srcRank))
             return pathClear(srcRow, srcCol, destRow, destCol);
         return false;
    } else if(piece == "\u265E") { // чёрный конь
         Knight knight(Type::knight, srcCol, srcRank, Color::black);
         return knight.knights_move(srcCol, srcRank, destCol, destRank);
    } else if(piece == "\u265F") { // чёрная пешка (атака)
         Pawn pawn(Type::pawn, srcCol, srcRank, Color::black);
         return pawn.pawn_attack(srcCol, srcRank, destCol, destRank);
    }
    return false;
}

// Проверка, атакована ли клетка (row, col) фигурами цвета byColor.
bool isSquareAttacked(int row, int col, Color byColor) {
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            string p = board[i][j];
            if(p == " ")
                continue;
            Color pColor = (p == "\u2654" || p == "\u2655" || p == "\u2656" || p == "\u2657" || p == "\u2658" || p == "\u2659")
                            ? Color::white : Color::black;
            if(pColor == byColor){
                if(canPieceAttack(i, j, row, col, p))
                    return true;
            }
        }
    }
    return false;
}

// Находит позицию короля заданного цвета и проверяет, находится ли он под атакой.
bool isKingInCheck(Color kingColor) {
    string kingSymbol = (kingColor == Color::white ? "\u2654" : "\u265A");
    int kingRow = -1, kingCol = -1;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if(board[i][j] == kingSymbol){
                kingRow = i;
                kingCol = j;
                break;
            }
        }
        if(kingRow != -1)
            break;
    }
    if(kingRow == -1)
        return false; // Не должно происходить
    Color opponent = (kingColor == Color::white ? Color::black : Color::white);
    return isSquareAttacked(kingRow, kingCol, opponent);
}

// Симулирует ход: перемещает фигуру с (srcRow, srcCol) на (destRow, destCol),
// проверяет, останется ли король в безопасности, затем откатывает изменения.
bool moveLeavesKingSafe(int srcRow, int srcCol, int destRow, int destCol, Color movingColor) {
    string tempSrc = board[srcRow][srcCol];
    string tempDest = board[destRow][destCol];
    board[destRow][destCol] = board[srcRow][srcCol];
    board[srcRow][srcCol] = " ";
    bool safe = !isKingInCheck(movingColor);
    board[srcRow][srcCol] = tempSrc;
    board[destRow][destCol] = tempDest;
    return safe;
}

// Перебирает все фигуры цвета color и проверяет, существует ли хотя бы один допустимый ход.
bool hasLegalMoves(Color color) {
    for (int srcRow = 0; srcRow < 8; srcRow++) {
        for (int srcCol = 0; srcCol < 8; srcCol++) {
            string piece = board[srcRow][srcCol];
            if(piece == " ")
                continue;
            Color pColor = (piece == "\u2654" || piece == "\u2655" || piece == "\u2656" ||
                            piece == "\u2657" || piece == "\u2658" || piece == "\u2659")
                           ? Color::white : Color::black;
            if(pColor != color)
                continue;
            for (int destRow = 0; destRow < 8; destRow++){
                for (int destCol = 0; destCol < 8; destCol++){
                    if(srcRow == destRow && srcCol == destCol)
                        continue;
                    // Если на целевой клетке своя фигура – пропускаем
                    string destPiece = board[destRow][destCol];
                    if(destPiece != " ") {
                        Color destColor = (destPiece == "\u2654" || destPiece == "\u2655" || destPiece == "\u2656" ||
                                           destPiece == "\u2657" || destPiece == "\u2658" || destPiece == "\u2659")
                                          ? Color::white : Color::black;
                        if(destColor == color)
                            continue;
                    }
                    bool valid = false;
                    // Для пешек обрабатываем отдельно
                    if(piece == "\u2659" || piece == "\u265F") {
                        int srcRank = 8 - srcRow;
                        int destRank = 8 - destRow;
                        int direction = (pColor == Color::white ? 1 : -1);
                        if(abs(destCol - srcCol) == 1 && destRank == srcRank + direction)
                            valid = true;
                        else if(destCol == srcCol && destRank == srcRank + direction && board[destRow][destCol] == " ")
                            valid = true;
                        else if(destCol == srcCol && ((pColor == Color::white && srcRank == 2 && destRank == 4) ||
                                                      (pColor == Color::black && srcRank == 7 && destRank == 5))) {
                            int intermediateRow = (pColor == Color::white ? srcRow - 1 : srcRow + 1);
                            if(board[destRow][destCol] == " " && board[intermediateRow][srcCol] == " ")
                                valid = true;
                        }
                    } else {
                        valid = canPieceAttack(srcRow, srcCol, destRow, destCol, piece);
                    }
                    if(valid && moveLeavesKingSafe(srcRow, srcCol, destRow, destCol, color))
                        return true;
                }
            }
        }
    }
    return false;
}

// Проверка возможности рокировки
bool canCastleKingside(Color color) {
    if(color == Color::white) {
        if(whiteKingMoved || whiteRookKingsideMoved)
            return false;
        if(board[7][4] != "\u2654")
            return false;
        if(board[7][5] != " " || board[7][6] != " ")
            return false;
        if(isKingInCheck(Color::white))
            return false;
        if(isSquareAttacked(7,5, Color::black) || isSquareAttacked(7,6, Color::black))
            return false;
        return true;
    } else {
        if(blackKingMoved || blackRookKingsideMoved)
            return false;
        if(board[0][4] != "\u265A")
            return false;
        if(board[0][5] != " " || board[0][6] != " ")
            return false;
        if(isKingInCheck(Color::black))
            return false;
        if(isSquareAttacked(0,5, Color::white) || isSquareAttacked(0,6, Color::white))
            return false;
        return true;
    }
}

bool canCastleQueenside(Color color) {
    if(color == Color::white) {
        if(whiteKingMoved || whiteRookQueensideMoved)
            return false;
        if(board[7][4] != "\u2654")
            return false;
        if(board[7][1] != " " || board[7][2] != " " || board[7][3] != " ")
            return false;
        if(isKingInCheck(Color::white))
            return false;
        if(isSquareAttacked(7,3, Color::black) || isSquareAttacked(7,2, Color::black))
            return false;
        return true;
    } else {
        if(blackKingMoved || blackRookQueensideMoved)
            return false;
        if(board[0][4] != "\u265A")
            return false;
        if(board[0][1] != " " || board[0][2] != " " || board[0][3] != " ")
            return false;
        if(isKingInCheck(Color::black))
            return false;
        if(isSquareAttacked(0,3, Color::white) || isSquareAttacked(0,2, Color::white))
            return false;
        return true;
    }
}

int main(){
    cout << "Добро пожаловать в шахматы!" << endl;
    
    // Инициализация пустых клеток
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            board[i][j] = " ";
        }
    }
    
    // Расстановка фигур согласно шахматной нотации:
    // Чёрные фигуры занимают 8-й (индекс 0) и 7-й (индекс 1) ряды:
    // Ранг 8 (индекс 0):
    board[0][0] = "\u265C"; // ладья
    board[0][1] = "\u265E"; // конь
    board[0][2] = "\u265D"; // слон
    board[0][3] = "\u265B"; // ферзь
    board[0][4] = "\u265A"; // король
    board[0][5] = "\u265D"; // слон
    board[0][6] = "\u265E"; // конь
    board[0][7] = "\u265C"; // ладья
    // Ранг 7 (индекс 1):
    for (int i = 0; i < 8; i++){
        board[1][i] = "\u265F"; // пешка
    }
    
    // Белые фигуры занимают 2-й (индекс 6) и 1-й (индекс 7) ряды:
    // Ранг 2 (индекс 6):
    for (int i = 0; i < 8; i++){
        board[6][i] = "\u2659"; // пешка
    }
    // Ранг 1 (индекс 7):
    board[7][0] = "\u2656"; // ладья
    board[7][1] = "\u2658"; // конь
    board[7][2] = "\u2657"; // слон
    board[7][3] = "\u2655"; // ферзь
    board[7][4] = "\u2654"; // король
    board[7][5] = "\u2657"; // слон
    board[7][6] = "\u2658"; // конь
    board[7][7] = "\u2656"; // ладья
    
    printBoard();
    
    // Белые ходят первыми
    Color currentTurn = Color::white;
    
    while (true){
        cout << ((currentTurn == Color::white) ? "Ход белых" : "Ход чёрных") << endl;
        cout << "Введите ход (например, e2e4, o-o или o-o-o) или 'exit' для выхода: ";
        string move;
        cin >> move;
        if (move == "exit")
            break;
        
        // Обработка рокировки
        if(move == "o-o" || move == "o-o-o") {
            if(currentTurn == Color::white) {
                if(move == "o-o" && canCastleKingside(Color::white)) {
                    // Король с e1 -> g1, ладья с h1 -> f1
                    board[7][6] = board[7][4];  // король на g1
                    board[7][4] = " ";
                    board[7][5] = board[7][7];  // ладья на f1
                    board[7][7] = " ";
                    whiteKingMoved = true;
                    whiteRookKingsideMoved = true;
                    cout << "Белые выполнили короткую рокировку." << endl;
                } else if(move == "o-o-o" && canCastleQueenside(Color::white)) {
                    // Король с e1 -> c1, ладья с a1 -> d1
                    board[7][2] = board[7][4];  // король на c1
                    board[7][4] = " ";
                    board[7][3] = board[7][0];  // ладья на d1
                    board[7][0] = " ";
                    whiteKingMoved = true;
                    whiteRookQueensideMoved = true;
                    cout << "Белые выполнили длинную рокировку." << endl;
                } else {
                    cout << "Рокировка невозможна." << endl;
                    continue;
                }
            } else { // чёрные
                if(move == "o-o" && canCastleKingside(Color::black)) {
                    // Король с e8 -> g8, ладья с h8 -> f8
                    board[0][6] = board[0][4];  // король на g8
                    board[0][4] = " ";
                    board[0][5] = board[0][7];  // ладья на f8
                    board[0][7] = " ";
                    blackKingMoved = true;
                    blackRookKingsideMoved = true;
                    cout << "Чёрные выполнили короткую рокировку." << endl;
                } else if(move == "o-o-o" && canCastleQueenside(Color::black)) {
                    // Король с e8 -> c8, ладья с a8 -> d8
                    board[0][2] = board[0][4];  // король на c8
                    board[0][4] = " ";
                    board[0][3] = board[0][0];  // ладья на d8
                    board[0][0] = " ";
                    blackKingMoved = true;
                    blackRookQueensideMoved = true;
                    cout << "Чёрные выполнили длинную рокировку." << endl;
                } else {
                    cout << "Рокировка невозможна." << endl;
                    continue;
                }
            }
            printBoard();
            // После рокировки переключаем ход и переходим к следующей итерации
            currentTurn = (currentTurn == Color::white ? Color::black : Color::white);
            continue;
        }
        
        // Если ввод не соответствует формату обычного хода (длина 4), сообщаем об ошибке.
        if(move.length() != 4) {
            cout << "Неверный формат ввода. Используйте, например, e2e4." << endl;
            continue;
        }
        

        char srcFile = move[0];
        char srcRankChar = move[1];
        char destFile = move[2];
        char destRankChar = move[3];
        
        int srcX = srcFile - 'a';
        int srcY = srcRankChar - '0';
        int destX = destFile - 'a';
        int destY = destRankChar - '0';
        
        int srcRow = 8 - srcY;
        int destRow = 8 - destY;
        
        if(srcX < 0 || srcX > 7 || destX < 0 || destX > 7 || srcY < 1 || srcY > 8 || destY < 1 || destY > 8) {
            cout << "Координаты вне диапазона." << endl;
            continue;
        }
        
        string piece = board[srcRow][srcX];
        if(piece == " ") {
            cout << "На исходной клетке нет фигуры." << endl;
            continue;
        }
        
        Color pieceColor = (piece == "\u2654" || piece == "\u2655" || piece == "\u2656" ||
                            piece == "\u2657" || piece == "\u2658" || piece == "\u2659")
                           ? Color::white : Color::black;
        if(pieceColor != currentTurn) {
            cout << "Не ваша очередь." << endl;
            continue;
        }
        
       
        bool valid = false;
        if(piece == "\u2659" || piece == "\u265F") {
            int srcRank = srcY; 
            int destRank = destY;
            int direction = (pieceColor == Color::white ? 1 : -1);
            if(abs(destX - srcX) == 1 && destRank == srcRank + direction)
                valid = true;
            else if(destX == srcX && destRank == srcRank + direction && board[destRow][destX] == " ")
                valid = true;
            else if(destX == srcX && ((pieceColor == Color::white && srcRank == 2 && destRank == 4) ||
                                      (pieceColor == Color::black && srcRank == 7 && destRank == 5))) {
                int intermediateRow = (pieceColor == Color::white ? srcRow - 1 : srcRow + 1);
                if(board[destRow][destX] == " " && board[intermediateRow][srcX] == " ")
                    valid = true;
            }
        } else {
            valid = canPieceAttack(srcRow, srcX, destRow, destX, piece);
        }
        
        if(!valid) {
            cout << "Недопустимый ход по правилам перемещения фигуры." << endl;
            continue;
        }
        

        if(!moveLeavesKingSafe(srcRow, srcX, destRow, destX, currentTurn)) {
            cout << "Нельзя совершить ход, оставляющий короля под шахом." << endl;
            continue;
        }
        
        board[destRow][destX] = piece;
        board[srcRow][srcX] = " ";

        if(piece == "\u2654") { // белый король
            whiteKingMoved = true;
        }
        if(piece == "\u265A") { // чёрный король
            blackKingMoved = true;
        }

        if(piece == "\u2656") { // белая ладья
            if(srcRow == 7 && srcX == 0)
                whiteRookQueensideMoved = true;
            if(srcRow == 7 && srcX == 7)
                whiteRookKingsideMoved = true;
        }
        if(piece == "\u265C") { // чёрная ладья
            if(srcRow == 0 && srcX == 0)
                blackRookQueensideMoved = true;
            if(srcRow == 0 && srcX == 7)
                blackRookKingsideMoved = true;
        }
        
        printBoard();
        
        Color opponent = (currentTurn == Color::white ? Color::black : Color::white);
        if(isKingInCheck(opponent)) {
            if(!hasLegalMoves(opponent)) {
                cout << "Шах и мат! " << (currentTurn == Color::white ? "Белые" : "Чёрные") << " выиграли." << endl;
                break;
            } else {
                cout << "Шах противнику!" << endl;
            }
        }
        
        currentTurn = opponent;
    }
    
    return 0;
}
