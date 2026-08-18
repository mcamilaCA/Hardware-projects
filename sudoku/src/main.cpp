#include <Keypad.h>

const byte rows = 4;
const byte cols = 4; 

char keypad[rows][cols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[rows] = {9, 8, 7, 6} ;
byte colPins[cols]= {5,4,3, 2};

Keypad sudoku = Keypad(makeKeymap(keypad), rowPins, colPins, rows, cols);

// Sudoku Section

int board[9][9];
int solved[9][9];
bool curr_cell[9][9];

int curr_row= 0;
int curr_col= 0;

// Function declarations:
void generate_sudoku();
void print_board(int b[9][9]);
void keypad_input();


void setup() {
  Serial.begin(9600);

  randomSeed(analogRead(A0));

  generate_sudoku();

  Serial.println();
  Serial.println("======  S U D O K U      G A M E  =====");
  Serial.println("A = Up");
  Serial.println("D = Down");
  Serial.println("* = Left");
  Serial.println("# = Right");
  Serial.println("1-9 = Enter your number");
  Serial.println("B = Delete");
  Serial.println();

  print_board(board);

}

void loop() {
  keypad_input();
}

// Keypad Controlling 
void keypad_input(){
  char button = sudoku.getKey();

  if(!button){
    return;
  }

  switch(button){

    // Curson will move up
    case 'A':
      if (curr_row > 0){
        curr_row--;
        break;
      }

    // Cursor with delete
    case 'B':
      if(!curr_cell[curr_row][curr_col]){
        board[curr_row][curr_col] = 0;
      }
      break;

    // Cursor will move down  
    case 'D':
      if(curr_row < 8){
        curr_row ++;
      }
      break;

    // Cursor moves left
    case '*':
      if(curr_col > 0){
        curr_col --;
      }
      break;

    // Cursor moves right
    case '#':
      if(curr_col < 8){
        curr_col ++;
      }
      break;

    // Entry numbers
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      if (!curr_cell[curr_row][curr_col]){
        board[curr_row][curr_col] = button - '0';
      }
      break;
  }

  Serial.println();
  print_board(board);
}

// Create Sudoku
void generate_sudoku(){
  int base[9][9]= 
  {
    {1,2,3,4,5,6,7,8,9},
    {4,5,6,7,8,9,1,2,3},
    {7,8,9,1,2,3,4,5,6},
    {2,3,4,5,6,7,8,9,1},
    {5,6,7,8,9,1,2,3,4},
    {8,9,1,2,3,4,5,6,7},
    {3,4,5,6,7,8,9,1,2},
    {6,7,8,9,1,2,3,4,5},
    {9,1,2,3,4,5,6,7,8},
  };

  // copy into solution
  for (int row = 0; row< 9; row ++){
    for (int col = 0; col < 9; col ++){
      solved[row][col] = base[row][col];
    }
  }

  // row swapping to create new sudokus
  for (int band = 0; band < 3; band ++){
    int row1 = band*3 + random(3);
    int row2 = band*3 + random(3);

    for (int col = 0; col < 9; col ++){
      int temp = solved[row1][col];

      solved[row1][col]= solved[row2][col];
      solved[row2][col] = temp;
    } 
  }

  // copy solution in the board

  for(int row = 0; row < 9; row ++){
    for (int col = 0; col < 9; col ++){
      board[row][col] = solved[row][col];
    }
  }

  // Delete random cells to give sudoku to fill
  int removed = 0;

  while (removed < 40){

    int row = random(9);
    int col = random(9);
    
    if(board[row][col] !=0){
      board[row][col] = 0;
      removed ++;
    }
  }

  // Marking fixed cells
  for(int row = 0; row < 9; row ++){
    for(int col = 0; col < 9; col ++){
      curr_cell[row][col] = (board[row][col] != 0);
    }
  }

  curr_row = 0;
  curr_col = 0;
}

// Print the board 
void print_board(int b[9][9]){

  Serial.println("+-----------+-----------+-----------+");

  for (int row = 0; row < 9; row++) {

    for (int col = 0; col < 9; col++) {

      if (col % 3 == 0)
        Serial.print("| ");

      bool cursorHere = (row == curr_row && col == curr_col);

      if (cursorHere) {

        Serial.print("[");

        if (b[row][col] == 0)
          Serial.print(".");
        else
          Serial.print(b[row][col]);

        Serial.print("]");
      }
      else {

        if (b[row][col] == 0)
          Serial.print(". ");
        else {
          Serial.print(b[row][col]);
          Serial.print(" ");
        }
      }
    }

    Serial.println("|");

    if ((row + 1) % 3 == 0)
      Serial.println("+-----------+-----------+-----------+");
  }
  Serial.print("Cursor location: (");
  Serial.print(curr_row+1);
  Serial.print(", ");
  Serial.print(curr_col);
  Serial.print(")");
  
}