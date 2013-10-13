/* Mancala.cpp
 *     Play the board game of Mancala against the computer.  This version uses recursion to lookahead several moves to evaluate possible moves.
 *
 *   The rules are as follows:
 *
 *   The Mancala 'board' is made up of two rows of six holes each. If you don't have a Mancala board handy, an empty egg carton is perfect.
 *   Three pieces (marbles, stones or seeds) are placed in each of the 12 holes. The color of the pieces is irrelevant.
 *   Each player has a 'store' to the right side of the Mancala board. Cereal bowls work well for this purpose.
 *   The game begins with one player picking up all of the pieces in any one of the holes on his side.
 *   Moving counter-clockwise, the player deposits one of the stones in each hole (including your or your opponent's store) until the stones run out.
 *   If the last piece you drop is in your own store, you get a free turn.
 *   If the last piece you drop is in an empty hole on your side, you capture that piece and any pieces in the hole directly opposite.  All captured pieces are placed in your store.
 *   The game ends when all six spaces on one side of the Mancala board are empty.
 *   The player who still has pieces on his side of the board when the game ends captures all of those pieces.
 *   Count all the pieces in each store. The winner is the player with the most pieces.
 *
 * Author: Dale Reed
 * Program 5: Mancala
 * 11/27/2012
 *
 *   ToDo: delete Nodes once they are no longer needed in recursion
 *
 * Running the program looks like:
 
 Author: Dale Reed
 Program 5: Mancala
 TA: Susan Rice, Wed 7am
 Nov 27, 2012
 
 Welcome to the game of Mancala, where you are playing against
 a computer opponent.  Your holes are on the bottom row and
 you get to go first.
 
 A    B    C    D    E    F
 -----------------------------
 ----|  3 |  3 |  3 |  3 |  3 |  3 |----
 |    |                             |    |
 ----|  3 |  3 |  3 |  3 |  3 |  3 |----
 -----------------------------
 A    B    C    D    E    F
 -------------------------------------------
 
 1. Select a move (A..F)-> d
 
 A    B    C    D    E    F
 -----------------------------
 ----|  3 |  3 |  3 |  3 |  3 |  3 |----
 |    |                             |  1 |
 ----|  3 |  3 |  3 |    |  4 |  4 |----
 -----------------------------
 A    B    C    D    E    F
 You get another move -> c
 
 A    B    C    D    E    F
 -----------------------------
 ----|  3 |  3 |  3 |  3 |  3 |  3 |----
 |    |                             |  1 |
 ----|  3 |  3 |    |  1 |  5 |  5 |----
 -----------------------------
 A    B    C    D    E    F
 ----------------------------------------
 
 Computer chooses C
 
 A    B    C    D    E    F
 -----------------------------
 ----|  4 |  4 |    |  3 |  3 |  3 |----
 |  1 |                             |  1 |
 ----|  3 |  3 |    |  1 |  5 |  5 |----
 -----------------------------
 A    B    C    D    E    F
 Computer gets another move
 Computer chooses A
 
 A    B    C    D    E    F
 -----------------------------
 ----|    |  4 |    |  3 |  3 |  3 |----
 |  2 |                             |  1 |
 ----|  4 |  4 |  1 |  1 |  5 |  5 |----
 -----------------------------
 A    B    C    D    E    F
 -------------------------------------------
 
 2. Select a move (A..F)-> f
 
 .
 .
 .
 -------------------------------------------
 
 6. Select a move (A..F)-> a
 Squares in column B are captured.
 
 A    B    C    D    E    F
 -----------------------------
 ----|    |    |  1 |    |    |    |----
 | 15 |                             | 20 |
 ----|    |    |    |    |    |    |----
 -----------------------------
 A    B    C    D    E    F
 ----------------------------------------
 
 Computer chooses A
 
 A    B    C    D    E    F
 -----------------------------
 ----|    |    |  1 |    |    |    |----
 | 15 |                             | 20 |
 ----|    |    |    |    |    |    |----
 -----------------------------
 A    B    C    D    E    F
 Game is over.  Remaining pieces are being placed into the store.
 
 A    B    C    D    E    F
 -----------------------------
 ----|    |    |    |    |    |    |----
 | 16 |                             | 20 |
 ----|    |    |    |    |    |    |----
 -----------------------------
 A    B    C    D    E    F
 Human wins!
 Thanks for playing!  Goodbye...
 
 
 */

#include <iostream>
using namespace std;

// Global constants
const int BoardSize = 14;           // array size to hold playing pieces for both players
const int MaxRecursionDepth = 3;    // maximum depth for recursive move checking
const bool DisplayMessages = true;  // flag used to display messages when making actual moves
const bool NoMessages = false;      // flag used to turn off messages when exploring possible moves

struct Node {
    int theBoard[ BoardSize];// copy of the board
    int moveNumber;          // current move Number
    Node * nextMoves[ 6];    // array of pointers to Nodes containing all possible nexts moves
    int bestMoveIndex;       // which next move (0..5) is the
    int bestMoveValue;       // value of the best next move found so far
};
//struct of playmove containing the move made and a pointer for nextMove
struct PlayerMove{
       char move; //The move selected by player
       int theBoard[ BoardSize];// copy of the board after move
       PlayerMove * nextMove; //Pointer to the next PlayerMove
};

// Function prototype, without which there is no ordering of functions that gets rid
// of all compiler errors
char findBestMove( int theBoard[], int whoseTurn, bool displayFlag);


//--------------------------------------------------------------------------------
// Convert the board integer values into an array of strings.
// This is necessary only because when the value to be displayed is a 0, then
// we want to display two blanks instead of the numerical value
void setDisplayValues( int theBoard[ BoardSize],
                      char displayValues[ BoardSize][3])
{
    for (int i=0; i<BoardSize; i++) {
        if( theBoard[ i]==0) {
            strcpy(displayValues[ i], "  ");  // make the entry two blanks
        }
        else {
            // print the number into the array location, NULL terminating it
            sprintf(displayValues[ i], "%2d",theBoard[ i],NULL);
        }
    }
}//end setDisplayValues()


//--------------------------------------------------------------------------------
// Display the board, which will look something like:
//
//         A    B    C    D    E    F
//       -----------------------------
//  ----|  3 |  3 |  3 |  3 |  3 |  3 |----
// |  0 |                             |  0 |
//  ----|  3 |  3 |  3 |  3 |  3 |  3 |----
//       -----------------------------
//         A    B    C    D    E    F
//
// where the array index positions of the board pieces are:
//         A    B    C    D    E    F
//       -----------------------------
//  ----| 12 | 11 | 10 |  9 |  8 |  7 |----
// | 13 |                             |  6 |
//  ----|  0 |  1 |  2 |  3 |  4 |  5 |----
//       -----------------------------
//         A    B    C    D    E    F
//
void displayBoard( int theBoard[])
{
    // Convert the board integer values into array of strings.
    // This is necessary only because we want to display two blanks, not a 0 when the value is 0
    char displayValues[ BoardSize][ 3];
    setDisplayValues( theBoard, displayValues);
    
    printf("\n");
    printf("           A    B    C    D    E    F        \n");
    printf("          -----------------------------       \n");
    printf( "    ----| %2s | %2s | %2s | %2s | %2s | %2s |----  \n", displayValues[12], displayValues[11],displayValues[10],
           displayValues[9],  displayValues[8], displayValues[7]);
    printf("   | %2s |                             | %2s | \n",      displayValues[13], displayValues[6]);
    printf( "    ----| %2s | %2s | %2s | %2s | %2s | %2s |----  \n", displayValues[0], displayValues[1], displayValues[2],
           displayValues[3], displayValues[4], displayValues[5]);
    printf("         -----------------------------       \n");
    printf("           A    B    C    D    E    F        \n");
}//end displayBoard()


//--------------------------------------------------------------------------------
// movesRemaining() returns true if there are moves remaining.  It returns false
//      only if one of the players has exhausted all pieces on that player's side.
bool movesRemaining(int theBoard[])
{
    bool returnValue = true;
    
    // if one of the players has exhausted all pieces, set the returnValue to true.
    // First check the top row
    if( ( theBoard[12]==0 && theBoard[11]==0 && theBoard[10]==0 && theBoard[9]==0 && theBoard[8]==0 && theBoard[7]==0) ||
       ( theBoard[0]==0 && theBoard[1]==0 && theBoard[2]==0 && theBoard[3]==0 && theBoard[4]==0 && theBoard[5]==0)
       ) {
        returnValue = false;  // there are no more moves remaining
    }
    
    return returnValue;
}//end movesRemaining()


//--------------------------------------------------------------------------------
// Create a new node, initializing values, returning the address of the new node
Node * createNode( int theBoard[ BoardSize])
{
    // allocate memory for new node.
    Node *pTemp = new Node();
    
    // place copy of current board into node
    for( int i=0; i<BoardSize; i++) {
        pTemp->theBoard[ i] = theBoard[ i];
    }
    // Initialize pointers to next move nodes to all be NULL
    for (int i=0; i<6; i++) {
        pTemp->nextMoves[ i] = NULL;
    }
    // Initialize best move index and value
    pTemp->bestMoveIndex = -1;   // default value
    pTemp->bestMoveValue = -36;  // Worst possible relative score
    
    return pTemp;
    
}//end createNode()

//--------------------------------------------------------------------------------
// Create a player move, initializing values, returning the address of the player move
PlayerMove * createPlayerMove( char pmove, int theBoard[BoardSize])
{
    // allocate memory for new node.
    PlayerMove * pTemp = new PlayerMove(); 


    // place copy of current board into node
    for( int i=0; i<BoardSize; i++) {
        pTemp->theBoard[i] = theBoard[i];
    }

    pTemp->move = pmove;    //start of 
    pTemp->nextMove = NULL;
        
    return pTemp;
    
}//end createPlayerMove()

//start of insert insertPlayerMove function and using the insertion algorithm
void insertPlayerMove(PlayerMove * pHead, PlayerMove * humanMove){ 
     //two paramters of the datatype to a pointer of playerMove, phead for the 
     //start of the list and the humanmove for what user enters
     PlayerMove * pTemp; 
     pTemp=pHead; //both pTemp and pHead are pointing to the same thing 
     
     //Traverse list
     while(pTemp->nextMove != NULL){
           pTemp=pTemp->nextMove;
     }
     
     //update last PlayerMove to point to new move of what the user enters
     pTemp->nextMove = humanMove;
     
     
}
//printing out linked list for validation
void printPlayerMoves(PlayerMove * pHead){
     PlayerMove * pTemp; 
     pTemp=pHead; //both pTemp and pHead are pointing to the same thing 
     cout<<"Moves: ";
     //Traverse list
     while(pTemp->nextMove !=NULL){
           cout<<"  "<<pTemp->move; //Print current move
           pTemp=pTemp->nextMove;
     }
     cout<<"  "<<pTemp->move<<endl; //Print current move

}

//function for undoing the last move made by user entered on the list
void undoPlayerMove(PlayerMove * pHead, int theBoard[] )
{
     PlayerMove * pTemp; 
     PlayerMove * prev;S
     pTemp=pHead; //both pTemp and pHead are pointing to the same thing 
     
     //Traverse list
     while(pTemp->nextMove != NULL){
           prev = pTemp;
           pTemp=pTemp->nextMove;
     }
     
     //update second to last PlayerMove to point to NULL
     prev->nextMove = NULL; 
     
     // place copy of previous board into current board 
    for( int i=0; i<BoardSize; i++) {
        theBoard[i] = prev->theBoard[i]; 
    }
     
}

//--------------------------------------------------------------------------------
// Make a single move, returning the index position of the last piece dropped,
// which allows checking for extra moves.
int makeSingleMove( int theBoard[],    // the board
                   char userInput,    // user input 'A'.. 'F'
                   int whoseTurn,   // when even, it is Human's turn.  When odd, computer's turn
                   bool displayIsOn)  // indicates whether messages should be displayed.  Is false when exploring possible moves
{
    // convert userInput to upper case, in case it was entered as lower case
    userInput = toupper( userInput);
    
    // Find the position of pieces to be picked up
    int index = -1;   // starting position for making moves, set to a default value
    // find index depending on whose move it is
    if( whoseTurn % 2 == 0) {
        // Even number, so it is the bottom row player's turn (Human)
        index = userInput - 'A';
    }
    else {
        // top row player's turn
        index = 12 - (userInput - 'A');
    }
    
    // retrieve stones from that square, setting that square to now have 0 pieces
    int stones = theBoard[ index];
    theBoard[ index] = 0;
    
    // Drop the stones one at a time in sequential squares, including both "stores"
    // The mod function (using %) implements the wrap-around
    for( int i=0; i<stones; i++) {
        theBoard[ (index+1+i)%BoardSize ]++;
    }
    
    // See if the last piece dropped was dropped in an empty square on the user's side.
    // If so, capture it and any pieces opposite it.
    int indexOfLastSquare =(index+stones) % BoardSize;
    if( theBoard[ indexOfLastSquare]==1 && indexOfLastSquare != 6 && indexOfLastSquare != 13) {
        
        // We are in a capture situation.
        int storeIndex = -1;               // will be set to 6 for human, 13 for computer if it is on the correct side
        char capturePositionLetter = ' ';  // will be set to 'A'..'F'
        
        // Check from perspective of human (even numbered moves)
        if( whoseTurn % 2 == 0) {
            if( indexOfLastSquare < 6) {
                // Capture for human, so set the store index
                storeIndex = 6;
                capturePositionLetter = (char)('A' + indexOfLastSquare);
            }
        }
        else { // odd (computer) moves
            if( indexOfLastSquare > 6 && indexOfLastSquare < 13) {
                // Capture for computer, so set the store index
                storeIndex = 13;
                capturePositionLetter = (char)('A' + 12 - indexOfLastSquare);
            }
        }
        // If the last square wrapped around into the other user's side, then the value for
        // storeIndex will still be the default of -1 and we should not do a capture.
        // If the last square is one of the user's store, then do the following.
        if( storeIndex==6 || storeIndex==13) {
            if( displayIsOn) cout << "   Squares in column " << capturePositionLetter << " are captured." << endl;
            // Do the capture.  First add user pieces and opposite pieces into the store
            theBoard[ storeIndex] += theBoard[ indexOfLastSquare] + theBoard[ 12-indexOfLastSquare];
            // clear out those spots on the board where the pieces were taken from
            theBoard[ indexOfLastSquare] = 0;
            theBoard[ 12-indexOfLastSquare] = 0;
        }
        
        // display the resulting board showing the capture
        //displayBoard( theBoard);
    }//end if( theBoard[ indexOfLastSquare]...
    
    // return the index position of the last piece dropped
    return indexOfLastSquare;
    
}//end makeSingleMove()


//--------------------------------------------------------------------------------
//  Return true if the user's last piece ended up in that user's store (6 for human,
//     13 for computer)
bool userGetsExtraMove( int whoseTurn,  // is even for human moves, odd for computer moves
                       int lastIndex)    // index of the last piece dropped
{
    if( (whoseTurn%2==0 && lastIndex == 6) ||    // last piece for human was in the human's store
       (whoseTurn%2==1 && lastIndex == 13)      // last piece for computer was in the computer's store
       ) {
        return true;
    }
    else {
        return false;
    }
}//end getExtraMove()


//--------------------------------------------------------------------------------
// Make one or more moves.  For each move check the index position of the last piece dropped.
// If this index position is the user's store, then allow additional move(s)
void makeMove( int theBoard[],    // the board
              char userInput,    // user input 'A'.. 'F'
              int whoseTurn,   // when even, it is Human's turn.  When odd, computer's turn
              bool displayFlag)  // indicates whether messages should be displayed.  Is false when exploring possible moves
{
    int indexOfLastPieceDropped;  // if 6 (for human) or 13 (for computer) then player gets extra move
    
    // Allow making extra moves when the last piece dropped is in the store for the user to move
    do {
        // Make a single move, keeping track of the position where the last piece was dropped.
        indexOfLastPieceDropped = makeSingleMove(theBoard, userInput, whoseTurn, displayFlag);
        if( userGetsExtraMove(whoseTurn, indexOfLastPieceDropped)) {
            if( displayFlag) displayBoard( theBoard);
            
            // user gets another move.  Give appropriate prompt
            if( whoseTurn%2==0) {
                // When Human is giving input, Human is prompted for another move here.
                // However when computer is running recursively, automatically generating moves, then
                // we have to generate an automatic move here.
                if( displayFlag) {
                    // Human gets another move
                    cout << "   You get another move -> ";
                    cin >> userInput;
                }
                else {
                    // display is not on, so computer is recursively checking moves, so automatically choose a move for Human
                    userInput = findBestMove(theBoard, whoseTurn, displayFlag);
                }
            }
            else {
                // Computer gets another move
                if( displayFlag) cout << "   Computer gets another move \n";
                userInput = findBestMove( theBoard, 1, NoMessages);
                if( displayFlag) cout << "   Computer chooses " << userInput << " \n";
            }
        }
    }while( userGetsExtraMove(whoseTurn, indexOfLastPieceDropped));
    
}//end makeMove()


//--------------------------------------------------------------------------------
// If it is the end of the game, adjust the score recorded for this node, since
//    all opponent pieces go into the opponent's store.
void adjustScoreIfEndOfGame( Node * pCurrent)
{
    // Find out if all human squares are empty and count how many human pieces left if any
    bool humanSquaresEmpty = true;
    int remainingHumanPieces = 0;
    for( int i=0; i<6; i++) {
        if( pCurrent->theBoard[ i] > 0) {
            humanSquaresEmpty = false;
            remainingHumanPieces += pCurrent->theBoard[ i];
        }
    }
    // Same thing, except now for computer squares
    bool computerSquaresEmpty = true;
    int remainingComputerPieces = 0;
    for( int i=6; i<13; i++) {
        if( pCurrent->theBoard[ i] > 0) {
            computerSquaresEmpty = false;
            remainingComputerPieces += pCurrent->theBoard[ i];
        }
    }
    
    // Now adjust scores
    if( humanSquaresEmpty) {
        pCurrent->theBoard[13] = remainingComputerPieces;
    }
    if( computerSquaresEmpty) {
        pCurrent->theBoard[6] = remainingHumanPieces;
    }
    
}//end adjustScoreIfEndOfGame()


//--------------------------------------------------------------------------------
// Expand a given node, making the move for each child node representing a possible move.
// This assumes that the root node already exists.
void expandNode( Node * pRoot,     // existing root node
                int whoseTurn)    // whose turn it is, which also could be used to determine
//    how many levels of recursion.  Odd=computer, even=human
{
    // This is done recursively.  Check for the maximum depth
    if ( whoseTurn < MaxRecursionDepth) {
        // Create a node for each possible move, for now containing a copy of the
        // current board.  Connect each of these nodes to the root node.
        for( int i=0; i<6; i++) {
            pRoot->nextMoves[ i] = createNode( pRoot->theBoard);
            
            // for convenience make a pointer to this node so we don't need to keep using "pRoot->nextMoves[ i]"
            Node * pCurrentNode = pRoot->nextMoves[i];
            // Recursively continue expanding
            expandNode( pCurrentNode, whoseTurn+1);
            
            // Make the move from the ith position, updating this copy of the board.
            // In the line below, the next-to-last parameter (..., 1) indicates it is the computer's move,
            // and the last parameter indicates whether messages should be displayed or not
            makeMove( pCurrentNode->theBoard, (char)('A'+i), whoseTurn, NoMessages);
            
            // Adjust store values if one side of board is empty and opponent gets all remaining squares
            adjustScoreIfEndOfGame( pCurrentNode);
            // Estimate of the goodness of this node, which is the difference between number of computer and human store pieces
            pCurrentNode->bestMoveValue = pCurrentNode->theBoard[13] - pCurrentNode->theBoard[6];
            
            // At odd levels of recursion we choose the maximum score, which is best for the computer
            // At even levels of recursion we choose the minimum score, which is best for the human
            //    and worst for the computer
            if( ((whoseTurn % 2 == 1) && ( pCurrentNode->bestMoveValue > pRoot->bestMoveValue)) ||   // odd case, for computer choose max
               ((whoseTurn % 2 == 0) && ( pCurrentNode->bestMoveValue < pRoot->bestMoveValue))      // even case, for human choose min
               ) {
                // store new best move into the root node
                pRoot->bestMoveValue = pCurrentNode->bestMoveValue;  // new best move value
                pRoot->bestMoveIndex = i;                            // which move is best
            }//end if( whoseTurn...
            
        }//end for( int i...
    }
}//end expandNode()


//--------------------------------------------------------------------------------
// Find the computer move, returning which of the six characters ('A'..'F') is the best
char findBestMove( int theBoard[],
                  int whoseTurn,       // Even numbers are Human, odd are Computer
                  bool displayFlag)    // indicates whether messages should be displayed.  Is false when exploring possible moves)
{
    // create tree root Node with current information
    Node * pRoot = createNode( theBoard);
    
    // Create a node for each possible move, for now containing a copy of the
    // current board.  Connect each of these nodes to the root node.
    // The number parameter represents the level, which dictates whether it is from
    // the perspective of the human or the computer.
    expandNode( pRoot, whoseTurn);
    
    return (char) (pRoot->bestMoveIndex + 'A');
}//end findBestMove()

//function for valdating user input and returning true if user selects A-F and a-f and returning false if user input isnt 
int validateInput(char userInput){
    if(userInput == 'A' || userInput == 'B' || userInput == 'C' || userInput == 'D' || userInput == 'E'
          || userInput == 'F' || userInput == 'a' || userInput == 'b' || userInput == 'c' || userInput == 'd' 
          || userInput == 'e' || userInput == 'f' )
          {
                       return 1;  
          }
    return 0;
}

//--------------------------------------------------------------------------------
int main(int argc, char * argv[])
{
    //    index :    0 1 2 3 4 5 6 7 8 9 10 11 12 13
    int theBoard[ ]={3,3,3,3,3,3,0,3,3,3, 3, 3, 3, 0};
    
    if(argc == 15){ //User provided command prompts for board initilization
       // place copy of current board into node
       for( int i=0; i<BoardSize; i++) {
          theBoard[ i] =atoi(argv[i+1]);
       }
    }
    

    int moveNumber = 1;      // counts moves that are made by the user
    char userInput = ' ';    // stores user input of moves to make
    char computerMove = ' '; // computer move position letter 'A'..'F'
    PlayerMove * pHead = createPlayerMove(' ', theBoard);
    // seed the random number generator to be the time of day.  Otherwise
    // the random sequence will be the same each time prog. is run
    // srand( time(NULL));
    srand( 1);   // make it predictable for now
    
    // display identifying information
    cout << "Author: Dale Reed " << endl;
    cout << "Program 5: Mancala" << endl;
    cout << "TA: Susan Rice, Wed 7am" << endl;
    cout << "Nov 27, 2012" << endl;
    cout << endl;
    
    cout << "Welcome to the game of Mancala, where you are playing against " << endl;
    cout << "a computer opponent.  Your holes are on the bottom row and " << endl;
    cout << "you get to go first. " << endl;
    
    // main loop, alternating between human and computer.
    // Loop counter keeps incrementing. When it is even (0,2,4,...) it is the Human's turn
    // to move.  When it is odd (1,3,5,...) it is the computer's turn to move
    // By having a single loop handle both moves the code checks for the end of game
    // condition after each player's move.
    int userCounter = 0;
    while( movesRemaining( theBoard)) {
        displayBoard( theBoard);
        cout << "-------------------------------------------\n" << endl;
        
        // Prompt for human move and make the move
        cout << moveNumber << ". Select a move (A..F)-> ";
        cin >> userInput;
        //checking user input for correct validation for undo move
        while(userInput == 'u' || userInput == 'U' )
        {
            moveNumber=moveNumber-2;
            undoPlayerMove(pHead, theBoard);
            
            // Prompt for human move and make the move
            cout<< "Previous move undone."<<endl;
            displayBoard( theBoard);
            cout << moveNumber << ". Select a move (A..F)-> ";
            cin >> userInput;
          
        }
        if (userInput == 'x' || userInput == 'X' ) //and exiting the program
        {
             cout<< "See ya next time"<<endl;
             exit(0);
         }
        while(validateInput(userInput) != 1){         //this while loop us for when userput isnt true meaning that
                                                      //A-F and a-f the loop will continue to run if the the input isnt valid and display 
             cout<<"Incorrect input choosen."<<endl;    //the error message and ask user for the correct input again
             cout << moveNumber << ". Select a move (A..F)-> "<<endl;
             cin >> userInput;
        }               
        makeMove( theBoard, userInput, userCounter++, DisplayMessages);  // make a move from the userInput position for the player to move
        //create PlayerMove
        PlayerMove * pmove = createPlayerMove(userInput, theBoard);
        ///insert PlayerMove into linked list
        insertPlayerMove(pHead, pmove);
        printPlayerMoves(pHead);
        
        moveNumber++;  // update moveNumber
        
        displayBoard( theBoard);
        cout << "   ----------------------------------------\n" << endl;
        
        // Find the computer move and make that move
        computerMove = findBestMove( theBoard, 1, NoMessages);
        cout << "   Computer chooses " << computerMove << endl;
        makeMove( theBoard, computerMove, userCounter++, DisplayMessages);  // make a move from the userInput position for the player to move

        
    }//end while( movesRemaining(...)
    
    displayBoard( theBoard);
    
    cout << "Game is over.  Remaining pieces are being placed into the store." << endl;
    // Place all remaining pieces in the appropriate store and blank out where those pieces came from
    for (int i=0; i<13; i++) {
        if( i<6) {
            theBoard[ 6] += theBoard[ i];
            theBoard[ i] = 0;
        }
        else if( i>6) {
            theBoard[ 13] += theBoard[ i];
            theBoard[ i] = 0;
        }
    }//end for( int i...
    displayBoard( theBoard);
    
    // Display who won
    if( theBoard[ 6] > theBoard[13]) {
        cout << "Human wins!" << endl;
    }
    else if( theBoard[ 13] > theBoard[6]) {
        cout << "Computer wins!" << endl;
    }
    else {
        cout << "Tie Game." << endl;
    }
    
    cout << "Thanks for playing!  Goodbye..." << endl;
    return 0;
}// end main()

