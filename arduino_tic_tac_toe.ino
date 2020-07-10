#include<math.h>
#include<string.h>
#include <Keypad.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define TX_PIN_PLAYER 2
#define TX_PIN_AI 4
#define END_TX_PLAYER 3
#define END_TX_AI 5
#define PLAYER_WINNER 12
#define COMPUTER_WINNER 13
      
char* board = "123456789";

const byte ROWS = 3; //four rows
const byte COLS = 3; //four columns

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'}
};

byte rowPins[ROWS] = {6, 7, 8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 10, 11}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

struct value_move{

    int eval;
    int move;

};

void setup(){

    Serial.begin(9600);
  
  	pinMode(TX_PIN_PLAYER, OUTPUT);
  	pinMode(TX_PIN_AI, OUTPUT);
  	pinMode(END_TX_PLAYER, OUTPUT);
  	pinMode(END_TX_AI, OUTPUT);
  	pinMode(PLAYER_WINNER, OUTPUT);
  	pinMode(COMPUTER_WINNER, OUTPUT);
  
    char* result = game();
  	if(result == "X") digitalWrite(PLAYER_WINNER, HIGH);
  	if(result == "O") digitalWrite(COMPUTER_WINNER, HIGH);
    else{

        digitalWrite(PLAYER_WINNER, HIGH);
        digitalWrite(PLAYER_WINNER, HIGH);

    }
    
}

int check_for_win(){
  	if(board[0] == board[1] && board[1] == board[2])
        if(board[0] == 'X') return -1;
        else return 1;
    if(board[3] == board[4] && board[4] == board[5])
        if(board[3] == 'X') return -1;
        else return 1;
    if(board[6] == board[7] && board[7] == board[8])
        if(board[6] == 'X') return -1;
        else return 1;
    if(board[0] == board[3] && board[3] == board[6])
        if(board[0] == 'X') return -1;
        else return 1;
    if(board[1] == board[4] && board[4] == board[7])
        if(board[1] == 'X') return -1;
        else return 1;
    if(board[2] == board[5] && board[5] == board[8])
        if(board[2] == 'X') return -1;
        else return 1;
    if(board[0] == board[4] && board[4] == board[8])
        if(board[0] == 'X') return -1;
        else return 1;
    if(board[2] == board[4] && board[4] == board[6])
        if(board[2] == 'X') return -1;
        else return 1;
    bool flag = true;
    for(int i = 0; i < 9; i++)
    	if(board[i] != 'X' && board[i] != 'O'){
            flag = false;
            break;
        }
    if(flag) return 0;
    return NULL;
          
}

void get_possible_moves(int (&free)[9]){

    for(int i = 0; i < 9; i++)
        if(board[i] != 'X' && board[i] != 'O') free[i] = 1;
      	else free[i] = 0;

}

void update_board(int i, bool player){

    board[i] = (player) ? 'X' : 'O';

}

void undo(int i){

    board[i] = i + 1;

}

int evaluate(){

    int score = 0;
    score += evaluate_line(0, 1, 2);
    score += evaluate_line(3, 4, 5);
    score += evaluate_line(6, 7, 8);
    score += evaluate_line(0, 3, 6);
    score += evaluate_line(1, 4, 7);
    score += evaluate_line(2, 5, 8);
    score += evaluate_line(0, 4, 8);
    score += evaluate_line(2, 4, 6);
    return score;

}

int evaluate_line(int cell_1, int cell_2, int cell_3){

    int score = 0;
 
    if(board[cell_1] == 'O') score = 1;
    if(board[cell_1] == 'X') score = -1;
 
    if(board[cell_2] == 'O')
        if(score == 1) score = 10;
        else if(score == -1) return 0;
        else score = 1;
    else if(board[cell_2] == 'X')
        if(score == -1) score = -10;
        else if(score == 1) return 0;
        else score = -1;

    if(board[cell_3] == 'O')
        if(score > 0) score *= 10;
        else if(score < 0) return 0;
        else score = 1;
    else if(board[cell_3] == 'X')
        if(score < 0) score *= 10;
        else if(score > 1) return 0;
        else score = -1;

    return score;

}      

struct value_move minimax(int depth, bool isMaximizing){
  
    int result = check_for_win();
    struct value_move evaluated;

    if(depth == 0 || result != NULL){

        evaluated.eval = evaluate();
        evaluated.move = -1;
        return evaluated;

    }

    if(isMaximizing){

        struct value_move max;
        max.eval = -INFINITY;
        int moves[9];
        get_possible_moves(moves);
        for(int i = 0; i < 9; i++){

            if(moves[i] == 0) continue;

            update_board(i, false);
            struct value_move evaluation = minimax(depth - 1, false);
            undo(i);
            if(evaluation.eval > max.eval){

                max.eval = evaluation.eval;
                max.move = i;

            }

        }
        return max;

    }

    else{

        struct value_move min;
        min.eval = INFINITY;
        int moves[9];
        get_possible_moves(moves);
        for(int i = 0; i < 9; i++){

            if(moves[i] == 0) continue;

            update_board(i, 'X');
            struct value_move evaluation = minimax(depth - 1, true);
            undo(i);
            if(evaluation.eval < min.eval){

                min.eval = evaluation.eval;
                min.move = i;

            }
        
        }
      
        return min;
    
    }

}

struct value_move minimax_with_pruning(int depth, int alpha, int beta, bool isMaximizing){
  
    int result = check_for_win();
    struct value_move evaluated;
    
    if(depth == 0 || result != NULL){

        evaluated.eval = evaluate();
        evaluated.move = -1;
        return evaluated;

    }

    if(isMaximizing){

        struct value_move max;
        max.eval = -INFINITY;
        int moves[9];
        get_possible_moves(moves);
        for(int i = 0; i < 9; i++){

            if(moves[i] == 0) continue;

            update_board(i, false);
            struct value_move evaluation = minimax(depth - 1, false);
            undo(i);
            if(evaluation.eval > max.eval){

                max.eval = evaluation.eval;
                max.move = i;

            }
            alpha = MAX(alpha, evaluation.eval);
            if(beta <= alpha) break;

        }

        return max;

    }

    else{

        struct value_move min;
        min.eval = INFINITY;
        int moves[9];
        get_possible_moves(moves);
        for(int i = 0; i < 9; i++){

            if(moves[i] == 0) continue;

            update_board(i, 'X');
            struct value_move evaluation = minimax(depth - 1, true);
            undo(i);
            if(evaluation.eval < min.eval){

                min.eval = evaluation.eval;
                min.move = i;

            }
            beta = MIN(beta, evaluation.eval);
            if(beta <= alpha) break;
        
        }
      
        return min;
    
    }
      
}   
          
void computer_turn(){
  
    //move = minimax(9, True).move;
    int move = minimax_with_pruning(3, -INFINITY, INFINITY, true).move;
  	update_board(move, false);
  	for(int i = 0; i < move + 1; i++){
    
      	digitalWrite(TX_PIN_AI, HIGH);
      	delay(20);
      	digitalWrite(TX_PIN_AI, LOW);
    
  	}
  	digitalWrite(END_TX_AI, HIGH);
  	delay(20);
  	digitalWrite(END_TX_AI, LOW);
    show_game_state();
    
}

void player_turn(){

    int moves[9];
    get_possible_moves(moves);
  	int selection = -1;
    bool flag = true;
    while(flag){

        selection = keypad.getKey() - 49;
      	if(moves[selection] == 1){
          	
          	update_board(selection, true);
        	flag = false;
          
        }
        
    }
  	for(int i = 0; i < selection + 1; i++){
    
      	digitalWrite(TX_PIN_PLAYER, HIGH);
      	delay(20);
      	digitalWrite(TX_PIN_PLAYER, LOW);
    
  	}
  	digitalWrite(END_TX_PLAYER, HIGH);
  	delay(20);
  	digitalWrite(END_TX_PLAYER, LOW);
    show_game_state();
   
}

char* game(){
  
  	char* end_game[3] = {"X", "O", "TIE"};
  
    while(true){

        player_turn();
        int result = check_for_win();
        if(result != NULL) return (result == -1) ? end_game[0] : (result == 1) ? end_game[1] : end_game[2];

        computer_turn();
        result = check_for_win();
        if(result != NULL) return (result == -1) ? end_game[0] : (result == 1) ? end_game[1] : end_game[2];

    }
  
}

void loop(){}