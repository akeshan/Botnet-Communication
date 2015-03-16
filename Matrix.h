//file name: Matrix5.cpp
//complie: g++ -o A Matrix5.cpp
//run:     ./A
//date: 3/14/15--night
//This code can encoding/decoding message of size 3 with character ' ','a'--'z','0'--'9' 

#include <iostream>
using namespace std;

struct MatrixBank
{
 //Size 3 Matrix is complete now
 int bank3[11][20] = { { 1, -1,  1,   -2,  1, -1,   2,  3, -4 ,0,0,0,0,0,0,0,0,0,0,0},
                       { 2,  3,  2,    3,  3,  4,   1,  1,  1 ,0,0,0,0,0,0,0,0,0,0,0},
                       { 1,  1,  1,    3,  4,  3,   3,  3,  4 ,0,0,0,0,0,0,0,0,0,0,0},
                       {-1, -1,  0,  -10, -6, -1,  -8, -5, -1 ,0,0,0,0,0,0,0,0,0,0,0},
                       {-1, -1,  6,    1,  0, -2,   0,  1, -3 ,0,0,0,0,0,0,0,0,0,0,0},
                       { 7, -1, -1,   -3,  1,  0,  -3,  0,  1 ,0,0,0,0,0,0,0,0,0,0,0},
                       { 2, -1,  0,   -17, 11, 3,  11, -7, -2 ,0,0,0,0,0,0,0,0,0,0,0},

                       { 1,  3,  3,     1,  5, 6,   1,  4,  5 ,0,0,0,0,0,0,0,0,0,0,0},
                       { 1,  1,  1,     3,  5, 4,   3,  6,  5 ,0,0,0,0,0,0,0,0,0,0,0},
                       { 1,  3,  1,     2,  7, 4,   2,  9,  7 ,0,0,0,0,0,0,0,0,0,0,0},
                       { 1,  2,  2,     3,  7, 9,   1,  4,  7 ,0,0,0,0,0,0,0,0,0,0,0}  
                    };

 char mapping[37] = {' ','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q',
                  'r','s','t','u','v','w','x','y','z','0','1','2','3','4','5','6','7','8','9'};

 int selectMatrix[12] = {0,3,4, 4,5,10,  1,2,7, 7,8,9};

 int bank4[7][20] = { 
                     // I am working on size 4 by 4
                    };
}b;

class Encode {
public:

Encode(char arr[], int field, int random)
{
       m_size = 3;
       m_field = field;
       
       //store original message
       m_original = new char[m_size];
       for (int i = 0; i < m_size; i++){
          m_original[i] = arr[i];
       }

       //use mapping table convert original massage to int format 
       m_message = new int[m_size];
       for (int i = 0; i < m_size; i++){
         if ( arr[i] ==' ')
             m_message[i] = 0;
         else
            if ( arr[i]<='z' && arr[i]>= 'a')
                m_message[i] = arr[i]-'a'+1;
            else
               if (arr[i]<='9' && arr[i]>='0')
                   m_message[i] = arr[i]-'0'+27;
               else  m_message[i] = 0;
       }

       //select encoding matrix with field number and random number
       if ( (random > 2) || (random <0) )
          random = 2;
       if ( (field > 3) || (field <0) )
          field = 3;
       
       bMatrix = b.bank3[ b.selectMatrix[field*3 + random]];

        // Display selected matrix  
 //       cout << " selected matrix : ";
  //      for (int i = 0; i < 20; i++) 
   //        cout << bMatrix[i] << " ";
 //       cout << "\n";

       
}

void printMessage()
{
        // Display original message   
        cout << " Original Message : ";
        for (int i = 0; i < m_size; i++) 
           cout << m_original[i] << " ";
        cout << "\n";
}


void encoding(int *encodedMessage) {
                              
        //encoding and output results
 //       cout << " Encoded  Message : ";
        for (int col = 0; col < m_size; col++) {
            
            for (int inner = 0; inner < m_size; inner++) {
                encodedMessage[col] += m_message[inner] * bMatrix[inner+col*m_size];
            }
   //         cout << encodedMessage[col] << "  ";
        }
   //     cout << "\n";
}

~Encode ()
{
 delete [] m_message;
 delete [] m_original;
}

private:

char *m_original;
int *m_message;
int  m_size;
int  m_field;
int *bMatrix;
};


struct MatrixInverseBank
{
 // This part is comp;ete now
 int bankInverse3[11][20] = {{-1, -1,  0,   -10, -6, -1,   -8, -5, -1 ,0,0,0,0,0,0,0,0,0,0,0},
                             {-1, -1,  6,     1,  0, -2,    0,  1, -3 ,0,0,0,0,0,0,0,0,0,0,0},
                             { 7, -1, -1,    -3,  1,  0,   -3,  0,  1 ,0,0,0,0,0,0,0,0,0,0,0},
                             { 1, -1,  1,    -2,  1, -1,    2,  3, -4 ,0,0,0,0,0,0,0,0,0,0,0},
                             { 2,  3,  2,     3,  3,  4,    1,  1,  1 ,0,0,0,0,0,0,0,0,0,0,0},
                             { 1,  1,  1,     3,  4,  3,    3,  3,  4 ,0,0,0,0,0,0,0,0,0,0,0},
                             { 1,  2,  3,     1,  4,  6,    2, -3, -5 ,0,0,0,0,0,0,0,0,0,0,0},

                             { 1, -3,  3,     1,  2, -3,   -1, -1,  2 ,0,0,0,0,0,0,0,0,0,0,0},
                             { 1,  1, -1,    -3,  2,  -1,   3, -3,  2 ,0,0,0,0,0,0,0,0,0,0,0},
                             {-13, 12, -5,    6,  -5,  2,  -4,  3, -1 ,0,0,0,0,0,0,0,0,0,0,0},
                             {-13,  6, -4,   12,  -5,  3,  -5,  2, -1 ,0,0,0,0,0,0,0,0,0,0,0}
                    
                          };
 char mapping[37] = {' ','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q',
                   'r','s','t','u','v','w','x','y','z','0','1','2','3','4','5','6','7','8','9'};

 int selectMatrixInverse[12] ={0,3,4, 4,5,10,  1,2,7, 7,8,9};


 int bankInverse4[7][20] = { 
                            //I am working on size 4 by 4
                            };

}bi;


class Decode {
public:

Decode(int arr[], int field, int random)
{
       m_size = 3;
       m_field = field;
       
       //store message that need to be decoded
       m_message = new int[m_size];
       for (int i = 0; i < m_size; i++) 
           m_message[i] = arr[i];

       temp = new int[m_size];
       for (int i = 0; i < m_size; i++) 
           temp[i] = 0;
       
       //use field and random numbers to select decoding matrix
       if( (random > 2) || (random <0) )
          random = 2;
        if ( (field > 3) || (field <0) )
          field = 3;
       
       bMatrix = bi.bankInverse3[ bi.selectMatrixInverse[field*3 + random]];


        // Display selected matrix  
 //       cout << " selected matrix : ";
  //      for (int i = 0; i < 20; i++) 
  //         cout << bMatrix[i] << " ";
 //       cout << "\n";

       
}

void printMessage()
{
        // Display original message   
        cout << " Original Message : ";
        for (int i = 0; i < m_size; i++) 
           cout << m_message[i] << " ";
        cout << "\n";
}


void decoding(char *decodedMessage) {
                              
        //decoding and output results
 //       cout << " decoded  Message : ";
        for (int col = 0; col < m_size; col++) {
            
            for (int inner = 0; inner < m_size; inner++) {
                temp[col] += m_message[inner] * bMatrix[inner+col*m_size];
                
            }
            decodedMessage[col] = bi.mapping[temp[col]];
 //           cout << decodedMessage[col] << "  ";
        }
 //       cout << "\n";
}

~Decode ()
{
 delete [] m_message;
 delete [] temp;
}

private:

int *m_message;
int *temp;
int  m_size;
int  m_field;
int *bMatrix;
};

/*
int main() {

    char message[3] = {'k', 'l', 'd'}; // message you want to encode

    int result[3] = {0,0,0}; // use result to hold encoded message
    Encode e(message,3,2); //message: message to encode, 3: field, 2: random number to pick a matrix
    e.printMessage();     
    e.encoding(result); //pass in a same size matrix to get result

    int messageI[3] ={51,122,158}; // message you want to decode

    char resultI[3] = {' ',' ',' '};//to hold decoded message


    Decode d(messageI,3,2);//messageI :message to decode, 3: field, 2: random number to pick a Inverse of a matrix

    d.decoding(resultI);//pass in a same size matrix to get result

    return 0;
} */