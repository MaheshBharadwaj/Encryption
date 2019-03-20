/**
Function to generate UI for selecting file present on computer
    @param hwnd : Window handler
    @param path : Dynamic which stores path of selected file
    @param type : Whether file is being chosen for encryption ( value 1)
                  Or decryption  (value 2)

*/
void ChooseFile(HWND hwnd,char *const path,const int type=0){
    OPENFILENAME ofn;

    ///Sets all bytes to 0
    ZeroMemory( &ofn , sizeof(OPENFILENAME) );

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwnd ;///This windows is child to Main program window
    ofn.lpstrFile = path ;///Array to store path
    ofn.lpstrFile[0]='\0';
    ofn.nMaxFile = 150;   ///Max Length of file path

    ///Allow any file to be chosen for encryption
    if(type==ENC)
        ofn.lpstrFilter = "All Files(*.*)\0*.*\0";

    ///Restrict to only files having extension .ENC
    ///For decryption
    if(type==DEC)
        ofn.lpstrFilter = "Encrypted files(*.ENC)\0*.ENC\0";

    ///Default file extension
    ofn.nFilterIndex=0;

    ///Part of libcomdlg32a That has to be linked before executing this file
    GetOpenFileName(&ofn);

    ///Store selected file's path in the array passed
    strcpy(path,ofn.lpstrFile);
}



/**
Creating the buttons for the User Interface

*/
void InitLayout(HWND hwnd){
   ///Encryption Button
   CreateWindow(
        "Button",    //Window type
        "Encrypt",   //Text on button
        WS_VISIBLE | WS_CHILD | WS_BORDER,
        25,50,       //x,y coordinates
        150,25,      //width and height
        hwnd,        //Owner/Parent window
        HMENU(ENC),  //Value returned on click
        0,0);

    ///Decryption Button
    CreateWindow(
         "Button",
         "Decrypt",
         WS_VISIBLE | WS_CHILD | WS_BORDER,
         25,100,
         150,25,
         hwnd,
         HMENU(DEC),
         0,0);

    ///Exit Button
    CreateWindow(
        "Button",
        "Exit",
        WS_VISIBLE | WS_CHILD | WS_BORDER,
        25,150,
        150,25,
        hwnd,
        HMENU(EXIT),
        0,0);
}

