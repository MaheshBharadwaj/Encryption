/** LOGIC OF CODE
   the code follows Diffe Helman public key exchange for initialisation of the
    common key
   of computers a and b after initialisation the data provided by computer a is
    hashed and is stored in a file this file is later accessed by computer b for
    decryption of data with the common key.

   According to the Diffe Helman approach a function F(x) is done on a public
   number x by one computer A
   times and another computer B times where A and B are random generated
   numbers
   The values produced by the computers are exchanged
   the value produced by first computer is put into function,
    B times, by second computer
   the value produced by second computer is put into function,
    A times, by first computer
   the values so obtained by the computers is the same number,
   which is their common key

   here the function used is the sec(x) function where x is in degrees
   sec(x) cannot be reversed unless range is mentioned, hence it is
   secure method of making keys
*/


const double x = 33 * 3.14159265 / 180 ;  ///This is the public key
double value_by_A=0,
       value_by_B=0;

double sec(double x){
    double num = 1.0/cos(x);
    return num;
}

void ComputerB(double result2,int&j){ /// Does the operations expected
                                ///of computer B for exchange of pre-key

    j = rand() % 250000;
    for(int z=j;z;z--)
        result2 = sec(result2);

        value_by_B=result2;
}

void ComputerA(double result1,int&i){  /// Does the operations expected
                                /// of computer A for exchange of pre-key
    i = rand() % 250000;
    for(int z=i;z;z--)
        result1=sec(result1);

    value_by_A=result1;
}


/**
Function to encrypt a byte
 @param  &buffer: Character to be enrypted
 @param  &key: key for encryption which is constantly changing

*/
inline void hashing(char *buffer,int size , int &key){
        for(int i=0;i<size;i++)
           buffer[i] ^= key;              ///XOR

        key += (key % 10) + (key % 3);    ///Running key
}



/**
Function to decrypt files based on the valid key

    @param key_B: Key of computer B(receiver / decrypt-or)
    @param Fpath: String containing file path of the file
                  To be decrypted
*/

void decrypt(int key_B,const char * const Fpath){
    ifstream f(Fpath,ios::binary);
    if(!f)
        MessageBox(NULL,
                   "Failed to open File",
                   "Fatal Error!",
                    MB_ICONERROR | MB_OK);
    else{
        string Ftemp = string(Fpath);

        ///Removing .enc from the file extension
        Ftemp.erase ( Ftemp.end() - 4 , Ftemp.end() );

        ofstream fout ( Ftemp.c_str() , ios::binary);

        f.seekg(0L,ios::end);
        unsigned int counter = f.tellg() / BuffSize ;
        unsigned int spillover = f.tellg() % BuffSize;
        f.seekg(0L,ios::beg);

        char buffer[BuffSize];
        while(counter>0){
            f.read(buffer,BuffSize);
            hashing(buffer,BuffSize,key_A);
            fout.write(buffer,BuffSize);
            counter--;
        }

        if(spillover){
            char *buffer = new char[spillover];
            f.read (buffer,spillover);
            hashing(buffer,spillover,key_A);
            fout.write(buffer,spillover);
        }
        f.close();
        fout.close();
        MessageBox(NULL,
                    "Successfully decrypted!" ,
                    "",
                     MB_OK | MB_ICONINFORMATION);
        f.close();
        fout.close();
    }
}

/**
Function to encrypted selected file such that it can be decrypted
by and only by Computer B

    @param Key_A: Key of computer A based on which file is to be
                  Encrypted
    @param Fpath: String containing file path of file
                  To be encrypted
*/
void encrypt(int key_A, const char* const Fpath){
    ifstream f(Fpath,ios::binary);

    if(!f)
        MessageBox(NULL,
                   "Failed to open File",
                   "Fatal Error",
                   MB_ICONERROR | MB_OK);
    else{
        ///For calculating time taken for encryption:
        clock_t Start = clock();

        ///Appending .ENC extension to indicate that file is Encrypted
        string Ftemp = string(Fpath) + ".ENC";
        ofstream fout(Ftemp.c_str() ,ios::binary);

        if(!fout)
            MessageBox(NULL,
                       "Failed to create File",
                       "Fatal Error",
                       MB_ICONERROR | MB_OK);
        else{
            f.seekg(0L,ios::end);
            unsigned int counter = f.tellg() / BuffSize ;
            unsigned int spillover = f.tellg() % BuffSize;
            f.seekg(0L,ios::beg);

            char buffer[BuffSize];
            while(counter>0){
                f.read(buffer,BuffSize);
                hashing(buffer,BuffSize,key_A);
                fout.write(buffer,BuffSize);
                counter--;
            }

            if(spillover){
                char *buffer = new char[spillover];
                f.read (buffer,spillover);
                hashing(buffer,spillover,key_A);
                fout.write(buffer,spillover);
            }
            f.close();
            fout.close();
            clock_t End = clock();


            ///Calculating time taken
            double time = double(End-Start)/CLOCKS_PER_SEC;
            stringstream s;
            s<<time;
            ///t stores the value in the stream as a string
            string t=s.str();
            char buff[40]="Successfully encrypted\nTime taken: ";
            strcat( buff , t.c_str() );
            MessageBox(NULL,
                       buff ,
                       "" ,
                       MB_OK | MB_ICONINFORMATION);
        }
    }
}


/**
Initial setup which emulates the exchange of public keys generated
 by 2 computers
And generates a unique key for Encryption Decryption

    @param key_A: Variable which will store the common key for
                  Computer A
    @param key_B: Variable which will store the common key for
                  Computer B

*/
void INITPROG(int &key_A,int&key_B){
   double num_A,num_B;
   do{
        srand(time(0)); /// For random number production
        int i,j;
        ComputerA(x,i);
        ComputerB(x,j);
        ///Assuming exchange of results has occured,
        ///computer A has access to result 2 and
        /// computer B has access to result 1

        ///for computer A:

        num_A=value_by_B;
        ///The number received by computer A from computer B

        for(int z=i;z;z--)
            num_A = sec(num_A);


        ///For computer B

        num_B=value_by_A;
        /// The number received by computer B from computer A

        for(int z=j;z;z--)
            num_B = sec(num_B);

   }while( (int)num_A == 0 && (int)num_B == 0); ///repeat loop as long
                               ///as keys generated are not of value 0

    key_A = int(num_A) + int(10 * num_A),
    key_B = int(num_B) + int(10 * num_B);

    ///key_A is now equal to key_B
}

